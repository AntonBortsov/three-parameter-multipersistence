#pragma once
#include "core.hpp"
namespace mp {
struct Cell {
    int degree;
    Grade birth;
    Vec boundary;
};
struct Local {
    std::string kind;
    std::vector<Cell> cells;
    Vec parent, depth, parent_edge;
    I64 defect = 0;
    int vertex_at(const Grade &p) const {
        for (int i = 0; i < static_cast<int>(cells.size()); ++i)
            if (cells[i].degree == 0 && le(cells[i].birth, p))
                return i;
        throw Error("active face has no vertex");
    }
    Vec fill(const Vec &cycle, int degree, const Grade &p) const {
        if (cycle.empty())
            return {};
        // At this grade the active vertices induce a subtree. Its unique path fills an endpoint pair.
        // [TDA, sec. 2, active subtree criterion](../../english/03-tda.md#tda-active-tree)
        if (kind == "tree") {
            if (degree != 1)
                throw Error("unexpected nonzero tree cycle");
            Vec result;
            if (cycle.size() % 2)
                throw Error("odd tree boundary");
            for (std::size_t k = 0; k < cycle.size(); k += 2) {
                int a = cycle[k], b = cycle[k + 1];
                Vec path;
                while (a != b) {
                    if (depth[a] < depth[b])
                        std::swap(a, b);
                    path.push_back(parent_edge[a]);
                    a = parent[a];
                }
                std::sort(path.begin(), path.end());
                add(result, path);
            }
            return result;
        }
        Elimination e;
        for (int i = 0; i < static_cast<int>(cells.size()); ++i) {
            const auto &c = cells[i];
            if (c.degree == degree && le(c.birth, p))
                e.insert(c.boundary, {i});
        }
        auto [remain, coeff] = e.reduce(cycle);
        if (!remain.empty())
            throw Error("local filling failed at " + show(p));
        return coeff;
    }
};
inline std::vector<Rational> values(const std::vector<Grade> &g, int axis) {
    std::vector<Rational> a;
    for (auto p : g)
        a.push_back(p[axis]);
    std::sort(a.begin(), a.end());
    a.erase(std::unique(a.begin(), a.end()), a.end());
    return a;
}
inline int index_of(const std::vector<Rational> &a, const Rational &x) {
    return static_cast<int>(std::lower_bound(a.begin(), a.end(), x) - a.begin());
}
// The defect starts at the prefix sum B. Each edge of a maximum spanning tree
// subtracts its weight; the residue counts disconnected prefix components.
// [TDA, sec. 2, Theorem 1 and defect identity](../../english/03-tda.md#tda-prefix-defect)
inline Local tree(const std::vector<Grade> &g) {
    const int n = static_cast<int>(g.size());
    if (n == 1) {
        Local point;
        point.kind = "tree";
        point.cells.push_back({0, g.front(), {}});
        point.parent = {0};
        point.depth = {0};
        point.parent_edge = {-1};
        return point;
    }
    std::vector<std::array<int, 3>> ranks(n);
    std::array<int, 3> levels{};
    Local out;
    out.kind = "tree";
    for (int d = 0; d < 3; ++d) {
        auto a = values(g, d);
        levels[d] = static_cast<int>(a.size());
        out.defect -= levels[d];
        for (int i = 0; i < n; ++i) {
            ranks[i][d] = index_of(a, g[i][d]);
            out.defect += levels[d] - ranks[i][d];
        }
    }
    for (auto p : g)
        out.cells.push_back({0, p, {}});
    Vec best(n, -1);
    std::vector<bool> used(n);
    out.parent.assign(n, 0);
    out.depth.assign(n, 0);
    out.parent_edge.assign(n, -1);
    best[0] = 0;
    for (int step = 0; step < n; ++step) {
        int u = -1;
        for (int j = 0; j < n; ++j)
            if (!used[j] && (u < 0 || best[j] > best[u]))
                u = j;
        used[u] = true;
        if (step) {
            int v = out.parent[u];
            out.depth[u] = out.depth[v] + 1;
            out.parent_edge[u] = static_cast<int>(out.cells.size());
            out.cells.push_back({1, join(g[u], g[v]), {std::min(u, v), std::max(u, v)}});
            out.defect -= best[u];
        }
        for (int v = 0; v < n; ++v)
            if (!used[v]) {
                // With zero-based rank the number of shared prefixes on one axis is L - max(i,j).
                // [TDA, sec. 2, rank formula for the edge weight](../../english/03-tda.md#tda-weight-ranks)
                int w = 0;
                for (int d = 0; d < 3; ++d)
                    w += levels[d] - std::max(ranks[u][d], ranks[v][d]);
                if (w > best[v]) {
                    best[v] = w;
                    out.parent[v] = u;
                }
            }
    }
    return out;
}
// Pair joins grade the edges; the triple join grades the face that fills their boundary.
// [TDA, sec. 3, three forced pairs](../../english/03-tda.md#tda-three-births)
inline Local triangle(const std::vector<Grade> &g) {
    Local out;
    out.kind = "filled-triangle";
    for (const auto &p : g)
        out.cells.push_back({0, p, {}});
    for (int i = 0; i < 3; ++i)
        for (int j = i + 1; j < 3; ++j)
            out.cells.push_back({1, join(g[i], g[j]), {i, j}});
    out.cells.push_back({2, join(join(g[0], g[1]), g[2]), {3, 4, 5}});
    return out;
}
inline Local grid(const std::vector<Grade> &g) {
    Local out;
    out.kind = "grid";
    std::array<std::vector<Rational>, 3> coords;
    for (int a = 0; a < 3; ++a)
        coords[a] = values(g, a);
    std::array<int, 3> ax = {0, 1, 2};
    std::stable_sort(ax.begin(), ax.end(),
                     [&](int a, int b) { return coords[a].size() < coords[b].size(); });
    int x = ax[0], z = ax[1], y = ax[2];
    int nx = static_cast<int>(coords[x].size()), nz = static_cast<int>(coords[z].size());
    int absent = static_cast<int>(coords[y].size());
    std::vector<Vec> height(nx, Vec(nz, absent));
    for (const auto &p : g) {
        int &h = height[index_of(coords[x], p[x])][index_of(coords[z], p[z])];
        h = std::min(h, index_of(coords[y], p[y]));
    }
    // Prefix minima compute the omitted coordinate over births below the two base coordinates.
    // [TDA, sec. 5, height function](../../english/03-tda.md#tda-height)
    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < nz; ++j) {
            if (i)
                height[i][j] = std::min(height[i][j], height[i - 1][j]);
            if (j)
                height[i][j] = std::min(height[i][j], height[i][j - 1]);
        }
    std::map<std::array<int, 4>, int> id;
    for (int dim = 0; dim <= 2; ++dim)
        for (int u = 0; u <= 1; ++u) {
            int v = dim - u;
            if (v < 0 || v > 1)
                continue;
            for (int i = 0; i + u < nx; ++i)
                for (int j = 0; j + v < nz; ++j) {
                    if (height[i][j] == absent)
                        continue;
                    // Use the upper corner for the base coordinates and the lower corner for height.
                    // [TDA, sec. 5, degree of a grid cell](../../english/03-tda.md#tda-grid-grade)
                    Grade p;
                    p[x] = coords[x][i + u];
                    p[z] = coords[z][j + v];
                    p[y] = coords[y][height[i][j]];
                    Vec b;
                    if (u) {
                        b.push_back(id.at({i, j, 0, v}));
                        b.push_back(id.at({i + 1, j, 0, v}));
                    }
                    if (v) {
                        b.push_back(id.at({i, j, u, 0}));
                        b.push_back(id.at({i, j + 1, u, 0}));
                    }
                    std::sort(b.begin(), b.end());
                    id[{i, j, u, v}] = static_cast<int>(out.cells.size());
                    out.cells.push_back({dim, p, std::move(b)});
                }
        }
    return out;
}

// Local exactness is the condition needed to mix these three support models.
// [TDA, sec. 5, choice of local model after Proposition 2](../../english/03-tda.md#tda-mixed-choice)
inline Local support_resolution(const std::vector<Grade> &births, bool force_grid) {
    Local candidate = tree(births);
    if (!force_grid && candidate.defect == 0)
        return candidate;
    Local replacement = !force_grid && births.size() == 3 ? triangle(births) : grid(births);
    replacement.defect = candidate.defect;
    return replacement;
}
} // namespace mp
