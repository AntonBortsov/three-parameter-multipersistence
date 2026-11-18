#pragma once
#include "core.hpp"

namespace mp {
using ArcList = std::vector<Vec>;
// The callers supply two different digraphs to this SCC decomposition.
// [Bjerkevik, Theorem 1, allowed masks](../../english/02-bjerkevik.md#bj-scc)
// [Exchange, Theorem 2, exact supports](../../english/01-exchanges.md#ex-product)
inline std::vector<Vec> components(const ArcList &g) {
    int n = static_cast<int>(g.size());
    ArcList reverse(n);
    for (int i = 0; i < n; ++i)
        for (int j : g[i])
            reverse[j].push_back(i);
    Vec order, seen(n);
    auto visit = [&](int i, const ArcList &graph, Vec &post, auto &&self) -> void {
        seen[i] = 1;
        for (int j : graph[i])
            if (!seen[j])
                self(j, graph, post, self);
        post.push_back(i);
    };
    for (int i = 0; i < n; ++i)
        if (!seen[i])
            visit(i, g, order, visit);
    std::fill(seen.begin(), seen.end(), 0);
    std::vector<Vec> blocks;
    for (auto it = order.rbegin(); it != order.rend(); ++it)
        if (!seen[*it]) {
            blocks.emplace_back();
            visit(*it, reverse, blocks.back(), visit);
        }
    return blocks;
}
enum class Decision { no, yes, unknown };
struct Budget {
    U64 remaining, examined = 0;
};
} // namespace mp
