#pragma once
#include "block_search.hpp"

namespace mp {
inline Columns read_binary_matrix(const std::string &path) {
    std::ifstream in(path);
    if (!in)
        throw Error("cannot open " + path);
    std::string line;
    std::vector<std::string> rows;
    while (std::getline(in, line)) {
        if (rows.empty() && line.compare(0, 3, "\xEF\xBB\xBF") == 0)
            line.erase(0, 3);
        std::istringstream words(line.substr(0, line.find('#')));
        std::string bit, row;
        while (words >> bit)
            row += bit;
        if (row.empty())
            continue;
        if (row.find_first_not_of("01") != row.npos)
            throw Error("matrix entries must be binary");
        rows.push_back(row);
    }
    if (in.bad())
        throw Error("matrix read failed");
    Columns out(rows.size());
    for (int j = 0; j < static_cast<int>(rows.size()); ++j) {
        if (rows[j].size() != rows.size())
            throw Error("expected a square matrix");
        for (int i = 0; i < static_cast<int>(rows.size()); ++i)
            if (rows[j][i] == '1')
                out[i].push_back(j);
    }
    return out;
}
using Steps = std::vector<std::array<int, 2>>;
// Both the selected minor and its complementary minor must have full rank over F2.
// [Exchange, sec. 3, complementary-minor identity](../../english/01-exchanges.md#ex-minors)
inline bool admissible(const Columns &a, const std::string &flags) {
    Elimination chosen, complement;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        bool take = flags[i] & 1;
        Vec col;
        for (int j : a[i])
            if (bool(flags[j] & 2) == take)
                col.push_back(j);
        auto &basis = take ? chosen : complement;
        auto count = basis.pivots.size();
        basis.insert(std::move(col));
        if (basis.pivots.size() == count)
            return false;
    }
    return true;
}
// Enforce the prescribed set in the first k steps. Its internal order is free.
// [Exchange, sec. 1, flexible serial exchange](../../english/01-exchanges.md#ex-prefix)
inline Decision prefix_path(const Columns &a, const Vec &prescribed, Budget &budget, Steps &path) {
    int n = static_cast<int>(a.size()),
        k = std::accumulate(prescribed.begin(), prescribed.end(), 0);
    std::set<std::string> rejected;
    std::string flags(n, 0);
    auto walk = [&](int level, auto &&self) -> Decision {
        if (level == n)
            return Decision::yes;
        for (int i = 0; i < n; ++i)
            if (!(flags[i] & 1) && bool(prescribed[i]) == (level < k)) {
                for (int j = 0; j < n; ++j)
                    if (!(flags[j] & 2)) {
                        flags[i] |= 1;
                        flags[j] |= 2;
                        if (!rejected.count(flags)) {
                            if (!budget.remaining)
                                return Decision::unknown;
                            --budget.remaining;
                            ++budget.examined;
                            if (admissible(a, flags)) {
                                path.push_back({i, j});
                                auto d = self(level + 1, self);
                                if (d != Decision::no)
                                    return d;
                                path.pop_back();
                            }
                            rejected.insert(flags);
                        }
                        flags[j] &= static_cast<char>(~2);
                        flags[i] &= static_cast<char>(~1);
                    }
            }
        return Decision::no;
    };
    return walk(0, walk);
}
inline void exchange(const std::string &file, const std::string &prefix, U64 limit) {
    auto a = read_binary_matrix(file);
    int n = static_cast<int>(a.size());
    Vec wanted(n);
    if (prefix != "-")
        for (auto word : split(prefix, ',')) {
            int i = id_number(word);
            if (i >= n || wanted[i])
                throw Error("invalid or repeated prefix index");
            wanted[i] = 1;
        }
    Elimination basis;
    // Columns of the matrix give forward arcs; solving for unit vectors gives the inverse arcs.
    // [Exchange, sec. 3, exact-support digraph](../../english/01-exchanges.md#ex-support-graph)
    ArcList graph(2 * n);
    for (int i = 0; i < n; ++i) {
        basis.insert(a[i], {i});
        for (int j : a[i])
            graph[i].push_back(n + j);
    }
    if (static_cast<int>(basis.pivots.size()) != n)
        throw Error("basis matrix is singular");
    for (int j = 0; j < n; ++j)
        for (int i : basis.reduce({j}).second)
            graph[n + j].push_back(i);
    Budget budget{limit};
    Steps left, right;
    // A step changes one factor of the product state graph, so each block can be searched locally.
    // [Exchange, sec. 3, Theorem 2](../../english/01-exchanges.md#ex-product)
    for (const auto &group : components(graph)) {
        Vec x, y;
        for (int v : group) {
            if (v < n)
                x.push_back(v);
            else
                y.push_back(v - n);
        }
        Columns block(x.size());
        Vec local(x.size());
        for (int i = 0; i < static_cast<int>(x.size()); ++i) {
            local[i] = wanted[x[i]];
            for (int j = 0; j < static_cast<int>(y.size()); ++j)
                if (std::binary_search(a[x[i]].begin(), a[x[i]].end(), y[j]))
                    block[i].push_back(j);
        }
        Steps path;
        auto d = prefix_path(block, local, budget, path);
        if (d != Decision::yes) {
            std::cout << "{\"status\":\"" << (d == Decision::no ? "impossible" : "undecided")
                      << "\",\"examined\":" << budget.examined << "}\n";
            return;
        }
        for (auto s : path)
            (local[s[0]] ? left : right).push_back({x[s[0]], y[s[1]]});
    }
    // Perform every block's prescribed prefix before any block's suffix.
    // [Exchange, sec. 3, prefix assembly corollary](../../english/01-exchanges.md#ex-assembly)
    left.insert(left.end(), right.begin(), right.end());
    std::string flags(n, 0);
    std::cout << "{\"status\":\"realized\",\"examined\":" << budget.examined << ",\"steps\":[";
    for (std::size_t t = 0; t < left.size(); ++t) {
        auto [i, j] = left[t];
        flags[i] |= 1;
        flags[j] |= 2;
        if (!admissible(a, flags))
            throw Error("invalid assembled exchange certificate");
        if (t)
            std::cout << ",";
        std::cout << "[" << i << "," << j << "]";
    }
    std::cout << "]}\n";
}
} // namespace mp
