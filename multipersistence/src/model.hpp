#pragma once
#include "chain_model.hpp"

namespace mp {
template<class Visitor>
inline void for_each_cycle(const Model &m, int degree, const Grade &p, Visitor visit) {
    Elimination e;
    for (int i = 0; i < static_cast<int>(m.cells.size()); ++i) {
        const auto &c = m.cells[i];
        if (c.degree != degree || !le(c.birth, p))
            continue;
        auto relation = e.insert(c.boundary, {i});
        if (!relation.empty())
            visit(std::move(relation));
    }
}
inline Columns cycle_basis(const Model &m, int degree, const Grade &p) {
    Columns z;
    for_each_cycle(m, degree, p, [&](Vec cycle) { z.push_back(std::move(cycle)); });
    return z;
}

// At equal grades the entire target cycle space maps to homology. Rank-nullity gives
// its dimension as chain count minus outgoing rank minus incoming rank.
// [TDA, sec. 6, equal-grade rank](../../english/03-tda.md#tda-rank)
inline std::size_t betti_at(const Model &m, int degree, const Grade &p, std::size_t boundary_rank) {
    std::size_t chains = 0;
    Elimination differential;
    for (const auto &c : m.cells)
        if (c.degree == degree && le(c.birth, p)) {
            ++chains;
            differential.insert(c.boundary);
        }
    return chains - differential.pivots.size() - boundary_rank;
}

// Project source cycles into the same complement of target boundaries. This takes
// the quotient in the intersection formula before performing intersections.
// [TDA, sec. 6, simultaneous-image formula](../../english/03-tda.md#tda-joint-image)
inline Columns image_in_quotient(const Model &m, int degree, const Grade &p,
                                 const Elimination &boundaries) {
    Elimination image;
    for_each_cycle(m, degree, p, [&](Vec cycle) {
        image.insert(boundaries.normal_form(std::move(cycle)));
    });
    return image.basis();
}
inline std::size_t query(const Model &m, int degree, const Grade &target,
                         const std::vector<Grade> &sources) {
    if (sources.empty())
        throw Error("at least one source grade is required");
    for (const auto &p : sources)
        if (!le(p, target))
            throw Error("source exceeds target in at least one coordinate");
    Elimination boundaries;
    for (const auto &c : m.cells)
        if (c.degree == degree + 1 && le(c.birth, target))
            boundaries.insert(c.boundary);
    const auto proper = std::count_if(sources.begin(), sources.end(),
                                      [&](const Grade &p) { return p != target; });
    if (proper == 0)
        return betti_at(m, degree, target, boundaries.pivots.size());
    if (proper == 1) {
        const auto &source = *std::find_if(sources.begin(), sources.end(),
                                           [&](const Grade &p) { return p != target; });
        // Appending source cycles to the target boundary basis computes rank[B | Z] - rank B.
        // [TDA, sec. 6, one-source formula](../../english/03-tda.md#tda-rank)
        const auto boundary_rank = boundaries.pivots.size();
        for_each_cycle(m, degree, source, [&](Vec cycle) { boundaries.insert(std::move(cycle)); });
        return boundaries.pivots.size() - boundary_rank;
    }

    Columns common;
    bool first = true;
    for (const auto &p : sources) {
        if (p == target)
            continue;
        auto image = image_in_quotient(m, degree, p, boundaries);
        common = first ? std::move(image) : intersection(common, image);
        first = false;
        if (common.empty())
            break;
    }
    return common.size();
}
} // namespace mp
