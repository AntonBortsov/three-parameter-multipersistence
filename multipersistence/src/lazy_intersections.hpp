#pragma once
#include "intersection_index.hpp"

namespace mp {
inline bool grid_le(const GridPoint &p, const GridPoint &q) {
    return p[0] <= q[0] && p[1] <= q[1] && p[2] <= q[2];
}

// If p <= r <= q, the image of p lies inside that of r. The minimal
// sources suffice in the simultaneous-image formula.
// [Grid note, (5), minimal sources](../../english/04-query-index.md#minimal-sources)
inline std::vector<GridPoint> minimal_sources(const CriticalGrid &grid, const Grade &q,
                                               const std::vector<Grade> &sources) {
    if (sources.empty()) throw Error("at least one source grade is required");
    for (const auto &s : sources)
        if (!le(s, q)) throw Error("source exceeds target in at least one coordinate");
    std::vector<GridPoint> points, minimal;
    for (const auto &s : sources) {
        auto p = grid.locate(s);
        if (*std::min_element(p.begin(), p.end()) < 0) return {};
        points.push_back(p);
    }
    std::sort(points.begin(), points.end());
    for (const auto &p : points)
        if (std::none_of(minimal.begin(), minimal.end(), [&](const auto &r) { return grid_le(r, p); }))
            minimal.push_back(p);
    return minimal;
}

struct TargetImages {
    int first_row = 0;
    std::vector<std::size_t> offsets{0};
    Vec entries;
    std::map<GridPoint, Columns> images;

    void add_row(Vec &col, int simplex) const {
        const auto i = static_cast<std::size_t>(simplex - first_row);
        auto first = entries.begin() + offsets[i], last = entries.begin() + offsets[i+1];
        if (first == last) return;
        if (col.empty()) {
            col.assign(first, last);
            return;
        }
        Vec result;
        result.reserve(col.size() + (last - first));
        std::set_symmetric_difference(col.begin(), col.end(), first, last, std::back_inserter(result));
        col.swap(result);
    }

    // [Grid note, (3), projection by triangular row recurrence](../../english/04-query-index.md#projection)
    TargetImages(const SliceRows &rows, const ReducedSlice &slice) {
        if (!rows.chains.empty()) first_row = rows.chains.front();
        offsets.reserve(rows.chains.size() + 1);
        for (int i : rows.chains) {
            // Simplex ids of one degree are consecutive in load(). A pivot
            // relation expresses its row as a sum of strictly earlier rows.
            Vec col;
            auto pivot = slice.boundaries.pivots.find(i);
            if (pivot != slice.boundaries.pivots.end()) {
                const auto &relation = pivot->second.col;
                for (auto j = relation.begin(); j != relation.end()-1; ++j) add_row(col, *j);
            } else if (slice.labels[i] >= 0) {
                col.push_back(slice.labels[i]);
            }
            entries.insert(entries.end(), col.begin(), col.end());
            offsets.push_back(entries.size());
        }
    }
    // [Grid note, (4), source cycles in a fixed target basis](../../english/04-query-index.md#cover-maps)
    Columns project(const Columns &cycles) const {
        Elimination basis;
        for (const auto &cycle : cycles) {
            Vec col;
            for (int row : cycle) add_row(col, row);
            basis.insert(std::move(col));
        }
        return basis.basis();
    }
};

class LazyIntersections : public CriticalGrid {
    Filtration filtration;
    SliceRows rows;
    std::map<GridPoint, Columns> cycles;
    std::map<GridPoint, TargetImages> targets;

    const Columns &source_cycles(const GridPoint &p) {
        auto it = cycles.find(p);
        if (it != cycles.end()) return it->second;
        ReducedSlice slice(filtration, rows, at(p));
        return cycles.emplace(p, std::move(slice.cycles)).first->second;
    }
    TargetImages &target_at(const GridPoint &p) {
        auto it = targets.find(p);
        if (it != targets.end()) return it->second;
        ReducedSlice slice(filtration, rows, at(p));
        auto &target = targets.try_emplace(p, rows, slice).first->second;
        cycles.try_emplace(p, std::move(slice.cycles));
        return target;
    }
public:
    LazyIntersections(Filtration f, int d)
        : CriticalGrid(critical_grid(f,d)), filtration(std::move(f)), rows(filtration,d) {}

    std::size_t target_count() const { return targets.size(); }
    std::size_t query(const Grade &q, const std::vector<Grade> &sources) {
        const auto ps = minimal_sources(*this, q, sources);
        if (ps.empty()) return 0;
        const auto t = locate(q);
        if (ps.size() == 1 && ps.front() == t) return source_cycles(t).size();
        auto &target = target_at(t);
        Columns common;
        for (std::size_t i = 0; i < ps.size(); ++i) {
            auto it = target.images.find(ps[i]);
            if (it == target.images.end()) {
                auto image = target.project(source_cycles(ps[i]));
                it = target.images.emplace(ps[i], std::move(image)).first;
            }
            if (ps.size() == 1) return it->second.size();
            // [TDA, sec. 6, simultaneous images in the quotient](../../english/03-tda.md#tda-joint-image)
            common = i == 0 ? it->second : intersection(common, it->second);
            if (common.empty()) return 0;
        }
        return common.size();
    }
};
} // namespace mp
