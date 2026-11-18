#pragma once
#include "local_resolution.hpp"

namespace mp {
inline Vec map_chain(const Columns &map, const Vec &input) {
    Vec out;
    for (int i : input)
        add(out, map.at(i));
    return out;
}
struct Generator : Cell {
    int simplex;
    int local;
    bool augments;
};
struct Model {
    std::vector<Generator> cells;
    std::vector<Local> locals;
    std::size_t corrections = 0;
};
inline Model build(const Filtration &f, bool force_grid = false) {
    Model model;
    std::vector<int> base;
    base.reserve(f.size());
    model.locals.reserve(f.size());
    std::size_t generator_count = 0;
    for (const auto &s : f) {
        auto local = support_resolution(s.births, force_grid);
        if (local.cells.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()) - generator_count)
            throw Error("too many model generators for integer indices");
        base.push_back(static_cast<int>(generator_count));
        generator_count += local.cells.size();
        model.locals.push_back(std::move(local));
    }
    model.cells.reserve(generator_count);
    for (int sid = 0; sid < static_cast<int>(f.size()); ++sid) {
        const auto &l = model.locals[sid];
        for (int j = 0; j < static_cast<int>(l.cells.size()); ++j) {
            auto c = l.cells[j];
            for (int &x : c.boundary)
                x += base[sid];
            bool aug = c.degree == 0;
            c.degree += static_cast<int>(f[sid].vertices.size()) - 1;
            model.cells.push_back({std::move(c), sid, j, aug});
        }
    }
    // Triangle-to-edge maps are consumed inside one triangle. Edge-to-vertex maps
    // are reused by all incident triangles in the two composites of the homotopy.
    // [TDA, sec. 4, Theorem 2](../../english/03-tda.md#tda-differential)
    std::vector<std::vector<Columns>> edge_vertex_maps(f.size());
    for (int s = 0; s < static_cast<int>(f.size()); ++s) {
        const auto &local = model.locals[s];
        auto attach = [&](int j, int target, const Vec &image) {
            auto &col = model.cells[base[s] + j].boundary;
            for (int x : image)
                col.push_back(base[target] + x);
        };
        std::vector<Columns> face_maps(f[s].faces.size());
        for (std::size_t k = 0; k < f[s].faces.size(); ++k) {
            int t = f[s].faces[k];
            auto &map = face_maps[k];
            map.reserve(local.cells.size());
            const auto &target = model.locals[t];
            for (int j = 0; j < static_cast<int>(local.cells.size()); ++j) {
                const auto &c = local.cells[j];
                // In positive local degree lift the image of the boundary at the generator's birth.
                // [TDA, sec. 5, Proposition 2, first lifting equation](../../english/03-tda.md#tda-mixed-lifts)
                Vec image = c.degree == 0
                                ? Vec{target.vertex_at(c.birth)}
                                : target.fill(map_chain(map, c.boundary), c.degree, c.birth);
                attach(j, t, image);
                map.push_back(std::move(image));
            }
        }
        if (f[s].vertices.size() == 2)
            edge_vertex_maps[s] = std::move(face_maps);
        if (f[s].vertices.size() == 3) {
            struct Route { std::size_t edge_slot, vertex_slot; };
            std::map<int, std::vector<Route>> through;
            for (std::size_t i = 0; i < f[s].faces.size(); ++i) {
                const auto &ends = f[f[s].faces[i]].faces;
                for (std::size_t k = 0; k < ends.size(); ++k)
                    through[ends[k]].push_back({i, k});
            }
            for (const auto &[v, routes] : through) {
                Columns h;
                for (int j = 0; j < static_cast<int>(local.cells.size()); ++j) {
                    const auto &c = local.cells[j];
                    // Over F2 the boundary to fill is the sum of both face composites and H applied
                    // to the local boundary. The lift lands one local degree higher.
                    // [TDA, sec. 5, Proposition 2, second lifting equation](../../english/03-tda.md#tda-mixed-lifts)
                    Vec rhs = map_chain(h, c.boundary);
                    for (auto route : routes) {
                        int edge = f[s].faces[route.edge_slot];
                        const auto &to_vertex = edge_vertex_maps[edge][route.vertex_slot];
                        add(rhs, map_chain(to_vertex, face_maps[route.edge_slot][j]));
                    }
                    Vec image = model.locals[v].fill(rhs, c.degree + 1, c.birth);
                    model.corrections += image.size();
                    attach(j, v, image);
                    h.push_back(std::move(image));
                }
            }
        }
    }
    for (auto &c : model.cells)
        std::sort(c.boundary.begin(), c.boundary.end());
    return model;
}
// The two identities tested here are the square-zero differential and the augmentation chain map.
// [TDA, sec. 4, Theorem 2](../../english/03-tda.md#tda-differential)
inline void check_chain(const Filtration &f, const Model &m) {
    for (const auto &c : m.cells) {
        Vec square, aug;
        for (int j : c.boundary) {
            const auto &d = m.cells.at(j);
            if (d.degree + 1 != c.degree || !le(d.birth, c.birth))
                throw Error("invalid graded boundary");
            add(square, d.boundary);
            if (d.augments)
                add(aug, {d.simplex});
        }
        if (!square.empty())
            throw Error("differential square is nonzero");
        if (aug != (c.augments ? f[c.simplex].faces : Vec{}))
            throw Error("augmentation is not a chain map");
    }
}
// Shift model generators by one degree. The off-diagonal block of the cone is
// the augmentation; adjacent differential ranks must sum to the chain dimension.
// [TDA, sec. 7, cone rank equation](../../english/03-tda.md#tda-cone)
inline void check_cone(const Filtration &f, const Model &m, const Grade &p) {
    std::array<Elimination, 6> images;
    std::array<std::size_t, 6> sizes{};
    for (const auto &s : f) {
        if (std::none_of(s.births.begin(), s.births.end(),
                         [&](const Grade &a) { return le(a, p); }))
            continue;
        int d = static_cast<int>(s.vertices.size()) - 1;
        ++sizes[d];
        images[d].insert(s.faces);
    }
    int offset = static_cast<int>(f.size());
    for (const auto &c : m.cells)
        if (le(c.birth, p)) {
            int d = c.degree + 1;
            Vec b;
            if (c.augments)
                b.push_back(c.simplex);
            for (int j : c.boundary)
                b.push_back(offset + j);
            ++sizes[d];
            images[d].insert(std::move(b));
        }
    for (std::size_t d = 0; d < sizes.size(); ++d) {
        auto incoming = d + 1 < sizes.size() ? images[d + 1].pivots.size() : 0;
        if (images[d].pivots.size() + incoming != sizes[d])
            throw Error("non-acyclic cone at " + show(p));
    }
}
inline std::size_t verify(const Filtration &f, const Model &m) {
    // Below the least birth on any axis every point slice is identically zero.
    std::vector<Grade> births;
    for (const auto &s : f)
        births.insert(births.end(), s.births.begin(), s.births.end());
    for (const auto &c : m.cells)
        births.push_back(c.birth);
    auto xs = values(births, 0), ys = values(births, 1), zs = values(births, 2);
    std::size_t n = 0;
    for (auto x : xs)
        for (auto y : ys)
            for (auto z : zs) {
                check_cone(f, m, {x, y, z});
                ++n;
            }
    return n;
}
} // namespace mp
