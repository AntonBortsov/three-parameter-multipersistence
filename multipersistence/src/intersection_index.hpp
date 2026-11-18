#pragma once
#include "core.hpp"

namespace mp {
using GridPoint = std::array<int, 3>;

struct HomologyNode {
    int dimension = 0;
    // incoming[a] maps the preceding node on axis a into this basis.
    std::array<Columns, 3> incoming;
};

struct CriticalGrid {
    int degree = 0;
    std::array<std::vector<Rational>, 3> axes;

    std::size_t grid_size() const {
        std::size_t size = 1;
        for (const auto &axis : axes) {
            if (axis.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()) ||
                (!axis.empty() && size > std::numeric_limits<std::size_t>::max() / axis.size()))
                throw Error("critical grid exceeds addressable size");
            size *= axis.size();
        }
        return size;
    }
    std::size_t offset(const GridPoint &p) const {
        return (std::size_t(p[0]) * axes[1].size() + p[1]) * axes[2].size() + p[2];
    }
    // [Grid note, (1), preservation of active births](../../english/04-query-index.md#grid-floor)
    GridPoint locate(const Grade &g) const {
        GridPoint p;
        for (int a = 0; a < 3; ++a) {
            const auto &v = axes[a];
            p[a] = static_cast<int>(std::upper_bound(v.begin(), v.end(), g[a]) - v.begin()) - 1;
        }
        return p;
    }
    Grade at(const GridPoint &p) const {
        return {axes[0][p[0]], axes[1][p[1]], axes[2][p[2]]};
    }
};

struct IntersectionIndex : CriticalGrid {
    std::vector<HomologyNode> nodes;

    // Functoriality makes every monotone cover path induce the same map.
    // The columns start as the source identity in its own homology basis.
    // [Grid note, (4), composition of cover maps](../../english/04-query-index.md#cover-maps)
    Columns image(GridPoint p, const GridPoint &target) const {
        Columns columns;
        const int rank = nodes[offset(p)].dimension;
        for (int i = 0; i < rank; ++i)
            columns.push_back({i});
        for (int a = 0; a < 3; ++a) {
            while (p[a] < target[a]) {
                ++p[a];
                const auto &edge = nodes[offset(p)].incoming[a];
                for (auto &col : columns) {
                    Vec result;
                    for (int row : col)
                        add(result, edge[row]);
                    col = std::move(result);
                }
            }
        }
        return independent(columns);
    }

    std::size_t query(const Grade &target, const std::vector<Grade> &sources) const {
        if (sources.empty())
            throw Error("at least one source grade is required");
        for (const auto &s : sources)
            if (!le(s, target))
                throw Error("source exceeds target in at least one coordinate");
        const auto q = locate(target);
        std::vector<GridPoint> points;
        for (const auto &s : sources) {
            auto p = locate(s);
            if (*std::min_element(p.begin(), p.end()) < 0)
                return 0;
            if (p != q)
                points.push_back(p);
        }
        if (points.empty())
            return nodes[offset(q)].dimension;
        std::sort(points.begin(), points.end());
        points.erase(std::unique(points.begin(), points.end()), points.end());
        Columns common = image(points.front(), q);
        // Intersect the images in one target basis.
        // [TDA, sec. 6, equal-image equation](../../english/03-tda.md#tda-equal-images)
        for (std::size_t i = 1; i < points.size() && !common.empty(); ++i)
            common = intersection(common, image(points[i], q));
        return common.size();
    }
};

inline bool active_at(const Simplex &s, const Grade &q) {
    return std::any_of(s.births.begin(), s.births.end(), [&](const Grade &p) { return le(p, q); });
}

// Build on the original simplicial chains. The augmentation induces a natural
// isomorphism to the free model [TDA, sec. 4](../../english/03-tda.md#tda-differential).
// [TDA, sec. 5, Proposition 2 for mixed local resolutions](../../english/03-tda.md#tda-mixed-lifts)
inline CriticalGrid critical_grid(const Filtration &f, int degree) {
    if (degree < 0 || degree > 4)
        throw Error("supported chain degrees are 0 through 4");
    if (f.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw Error("too many simplices for integer indices");
    CriticalGrid idx;
    idx.degree = degree;
    std::array<std::set<Rational>, 3> cuts;
    for (const auto &s : f) {
        for (const auto &p : s.births)
            for (int a = 0; a < 3; ++a)
                cuts[a].insert(p[a]);
    }
    for (int a = 0; a < 3; ++a)
        idx.axes[a].assign(cuts[a].begin(), cuts[a].end());
    return idx;
}

struct SliceRows {
    Vec chains, cofaces;
    SliceRows(const Filtration &f, int degree) {
        for (int i = 0; i < static_cast<int>(f.size()); ++i) {
            const int d = static_cast<int>(f[i].vertices.size()) - 1;
            if (d == degree) chains.push_back(i);
            if (d == degree + 1) cofaces.push_back(i);
        }
    }
};

struct ReducedSlice {
    Elimination boundaries;
    Vec labels;
    Columns cycles;
    // [Grid note, (2), dependent rows as homology coordinates](../../english/04-query-index.md#cycle-labels)
    ReducedSlice(const Filtration &f, const SliceRows &rows, const Grade &q) : labels(f.size(), -1) {
        for (int i : rows.cofaces)
            if (active_at(f[i], q)) boundaries.insert(f[i].faces);
        Elimination outgoing;
        for (int i : rows.chains) {
            if (!active_at(f[i], q) || boundaries.pivots.count(i)) continue;
            // An incoming pivot has dependent outgoing column by d²=0.
            // Every remaining dependency contributes one free homology coordinate.
            auto cycle = outgoing.insert(f[i].faces, {i});
            if (!cycle.empty()) {
                labels[i] = static_cast<int>(cycles.size());
                cycles.push_back(std::move(cycle));
            }
        }
    }
};

inline IntersectionIndex build_index(const Filtration &f, int degree) {
    IntersectionIndex idx;
    static_cast<CriticalGrid &>(idx) = critical_grid(f, degree);
    const SliceRows rows(f, degree);
    idx.nodes.resize(idx.grid_size());
    // Each representative is needed until its last immediate successor has read it.
    std::map<std::size_t, Columns> live;
    for (int x = 0; x < static_cast<int>(idx.axes[0].size()); ++x)
        for (int y = 0; y < static_cast<int>(idx.axes[1].size()); ++y)
            for (int z = 0; z < static_cast<int>(idx.axes[2].size()); ++z) {
                const GridPoint pos{x, y, z};
                const Grade q{idx.axes[0][x], idx.axes[1][y], idx.axes[2][z]};
                const auto id = idx.offset(pos);
                auto &node = idx.nodes[id];
                ReducedSlice slice(f, rows, q);
                node.dimension = static_cast<int>(slice.cycles.size());
                for (int a = 0; a < 3; ++a) {
                    if (pos[a] == 0) continue;
                    GridPoint prev = pos;
                    --prev[a];
                    auto it = live.find(idx.offset(prev));
                    if (it == live.end()) throw Error("missing predecessor cycles");
                    auto &edge = node.incoming[a];
                    edge.reserve(it->second.size());
                    for (const Vec &cycle : it->second) {
                        Vec column;
                        // Canonical reduction gives the class in the target
                        // quotient [TDA, sec. 6](../../english/03-tda.md#tda-joint-image).
                        // [Grid note, (2) and (4), reading a cover column](../../english/04-query-index.md#cover-maps)
                        for (int row : slice.boundaries.normal_form(cycle))
                            if (slice.labels[row] >= 0)
                                column.push_back(slice.labels[row]);
                        edge.push_back(std::move(column));
                    }
                    // x successors have the largest stride, then y, then z.
                    bool last = true;
                    for (int b = 0; b < a; ++b)
                        if (prev[b] + 1 < static_cast<int>(idx.axes[b].size())) last = false;
                    if (last) live.erase(it);
                }
                if (id + 1 < idx.nodes.size()) live.emplace(id, std::move(slice.cycles));
            }
    return idx;
}
} // namespace mp
