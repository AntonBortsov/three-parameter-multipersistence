#pragma once
#include "block_search.hpp"
#include <functional>
namespace mp {
using Mask = std::vector<std::vector<char>>;
using IntGrade = std::array<I64, 3>;
using Upsets = std::vector<std::vector<IntGrade>>;
inline U64 denominator(const std::vector<Record> &a, const std::vector<Record> &b) {
    U64 scale = 1, limit = static_cast<U64>(std::numeric_limits<I64>::max());
    for (auto side : {&a, &b})
        for (const auto &rec : *side)
            for (const auto &p : rec.births)
                for (const auto &x : p) {
                    U64 factor = x.d / std::gcd(x.d, scale);
                    if (scale > limit / factor)
                        throw Error("common denominator exceeds comparison range");
                    scale *= factor;
                }
    return scale;
}
inline Upsets scaled(const std::vector<Record> &records, U64 denominator) {
    Upsets out;
    for (const auto &r : records) {
        std::vector<IntGrade> g;
        for (const auto &p : r.births) {
            IntGrade a{};
            for (int k = 0; k < 3; ++k) {
                U64 factor = denominator / p[k].d;
                if (p[k].n > static_cast<U64>(std::numeric_limits<I64>::max() / 2) / factor)
                    throw Error("scaled coordinate exceeds comparison range");
                I64 v = static_cast<I64>(p[k].n * factor);
                a[k] = p[k].negative ? -v : v;
            }
            g.push_back(a);
        }
        out.push_back(std::move(g));
    }
    return out;
}
// Every generator of the source must reach some target generator. This accounts
// for the outer maximum and the inner minimum in the directed threshold.
// [Bjerkevik, sec. 3, Lemma 1](../../english/02-bjerkevik.md#bj-inclusion)
inline I64 directed_cost(const std::vector<IntGrade> &a, const std::vector<IntGrade> &b) {
    I64 threshold = 0;
    for (auto u : a) {
        I64 best = std::numeric_limits<I64>::max();
        for (auto v : b)
            best = std::min(best, std::max({I64(0), v[0] - u[0], v[1] - u[1], v[2] - u[2]}));
        threshold = std::max(threshold, best);
    }
    return threshold;
}
// Rows are sources. The result lists a target index for every source.
inline bool matching(const Mask &edges, Vec &assignment) {
    int n = static_cast<int>(edges.size());
    Vec owner(n, -1);
    for (int i = 0; i < n; ++i) {
        Vec seen(n);
        std::function<bool(int)> augment = [&](int x) {
            for (int y = 0; y < n; ++y)
                if (edges[x][y] && !seen[y]) {
                    seen[y] = 1;
                    if (owner[y] < 0 || augment(owner[y])) {
                        owner[y] = x;
                        return true;
                    }
                }
            return false;
        };
        if (!augment(i))
            return false;
    }
    assignment.assign(n, -1);
    for (int y = 0; y < n; ++y)
        assignment[owner[y]] = y;
    return true;
}
struct Witness {
    Columns forward, backward;
};
inline Decision block_solve(const Mask &p, const Mask &q, Witness &w, Budget &budget) {
    int n = static_cast<int>(p.size());
    Mask mutual = p;
    Vec perm;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            mutual[i][j] = p[i][j] && q[j][i];
    if (matching(mutual, perm)) {
        w.forward.assign(n, {});
        w.backward.assign(n, {});
        for (int i = 0; i < n; ++i) {
            w.forward[i] = {perm[i]};
            w.backward[perm[i]] = {i};
        }
        return Decision::yes;
    }
    if (!matching(p, perm) || !matching(q, perm))
        return Decision::no;
    auto count = [](const Mask &mask) {
        std::size_t a = 0;
        for (const auto &row : mask)
            a += std::count(row.begin(), row.end(), 1);
        return a;
    };
    // Enumerate the sparser mask and check its inverse against the opposite mask.
    // [Bjerkevik, sec. 3, finite-threshold enumeration](../../english/02-bjerkevik.md#bj-enumeration)
    bool swapped = count(q) < count(p);
    const Mask &allowed = swapped ? q : p;
    const Mask &inverse = swapped ? p : q;
    // A column with a single permitted position is forced in every nonsingular matrix.
    std::vector<std::pair<int, int>> bits;
    Columns columns(n);
    for (int i = 0; i < n; ++i) {
        Vec positions;
        for (int j = 0; j < n; ++j)
            if (allowed[i][j])
                positions.push_back(j);
        if (positions.size() == 1)
            columns[i] = positions;
        else
            for (int j : positions)
                bits.emplace_back(i, j);
    }
    std::vector<char> state(bits.size(), 0);
    for (;;) {
        if (!budget.remaining)
            return Decision::unknown;
        --budget.remaining;
        ++budget.examined;
        Columns candidate = columns;
        for (std::size_t t = 0; t < bits.size(); ++t)
            if (state[t])
                candidate[bits[t].first].push_back(bits[t].second);
        Elimination basis;
        for (int i = 0; i < n; ++i)
            basis.insert(candidate[i], {i});
        if (static_cast<int>(basis.pivots.size()) == n) {
            Columns back(n);
            bool valid = true;
            for (int j = 0; j < n && valid; ++j) {
                back[j] = basis.reduce({j}).second;
                for (int i : back[j])
                    if (!inverse[j][i]) {
                        valid = false;
                        break;
                    }
            }
            if (valid) {
                w = swapped ? Witness{back, candidate} : Witness{candidate, back};
                return Decision::yes;
            }
        }
        std::size_t t = 0;
        while (t < state.size() && state[t])
            state[t++] = 0;
        if (t == state.size())
            return Decision::no;
        state[t] = 1;
    }
}
// Inverse witnesses on balanced strongly connected blocks assemble block diagonally.
// [Bjerkevik, sec. 2, Theorem 1](../../english/02-bjerkevik.md#bj-scc)
inline Decision feasible(const Mask &p, const Mask &q, Witness &w, Budget &budget) {
    int n = static_cast<int>(p.size());
    ArcList graph(2 * n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            if (p[i][j])
                graph[i].push_back(n + j);
            if (q[j][i])
                graph[n + j].push_back(i);
        }
    std::vector<std::pair<Vec, Vec>> blocks;
    for (const auto &group : components(graph)) {
        Vec x, y;
        for (int i : group) {
            if (i < n)
                x.push_back(i);
            else
                y.push_back(i - n);
        }
        if (x.size() != y.size())
            return Decision::no;
        blocks.emplace_back(x, y);
    }
    w = {Columns(n), Columns(n)};
    bool deferred = false;
    for (auto &[x, y] : blocks) {
        int k = static_cast<int>(x.size());
        Mask a(k, std::vector<char>(k)), b = a;
        for (int i = 0; i < k; ++i)
            for (int j = 0; j < k; ++j) {
                a[i][j] = p[x[i]][y[j]];
                b[j][i] = q[y[j]][x[i]];
            }
        Witness small;
        auto decision = block_solve(a, b, small, budget);
        if (decision == Decision::no)
            return decision;
        if (decision == Decision::unknown) {
            deferred = true;
            continue;
        }
        for (int i = 0; i < k; ++i) {
            for (int j : small.forward[i])
                w.forward[x[i]].push_back(y[j]);
            for (int j : small.backward[i])
                w.backward[y[i]].push_back(x[j]);
        }
    }
    for (auto &col : w.forward)
        std::sort(col.begin(), col.end());
    for (auto &col : w.backward)
        std::sort(col.begin(), col.end());
    return deferred ? Decision::unknown : Decision::yes;
}
inline void json_vec(const Vec &vals) {
    std::cout << "[";
    for (std::size_t i = 0; i < vals.size(); ++i)
        std::cout << (i ? "," : "") << vals[i];
    std::cout << "]";
}
inline void json_cols(const Columns &cols) {
    std::cout << "[";
    for (std::size_t i = 0; i < cols.size(); ++i) {
        if (i)
            std::cout << ",";
        json_vec(cols[i]);
    }
    std::cout << "]";
}
inline void compare(const std::string &left, const std::string &right, U64 limit) {
    auto ar = read_records(left), br = read_records(right);
    if (ar.size() != br.size()) {
        std::cout << "{\"status\":\"exact\",\"dI\":\"infinity\",\"dB\":\"infinity\"}\n";
        return;
    }
    U64 den = denominator(ar, br);
    auto a = scaled(ar, den), b = scaled(br, den);
    int n = static_cast<int>(a.size());
    std::vector<std::vector<I64>> h(n, std::vector<I64>(n)), back = h;
    // Only the directed inclusion thresholds can change the two allowed-entry masks.
    // [Bjerkevik, sec. 3, candidate thresholds](../../english/02-bjerkevik.md#bj-enumeration)
    std::set<I64> thresholds{0};
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            thresholds.insert(h[i][j] = directed_cost(a[i], b[j]));
            thresholds.insert(back[j][i] = directed_cost(b[j], a[i]));
        }
    // A bijection using mutual entries bounds both directed costs at the chosen scale.
    Vec perm;
    Mask p(n, std::vector<char>(n)), q = p, mutual = p;
    // In source-first storage p[i][j] is P_ji and q[j][i] is Q_ij.
    // [Bjerkevik, sec. 3, Theorem 3, the two masks](../../english/02-bjerkevik.md#bj-masks)
    auto masks = [&](I64 t) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                p[i][j] = h[i][j] <= t;
                q[j][i] = back[j][i] <= t;
                mutual[i][j] = p[i][j] && q[j][i];
            }
    };
    auto price = [&](I64 t) {
        U64 u = static_cast<U64>(t), g = std::gcd(u, den);
        return Rational{u / g, den / g, false}.str();
    };
    std::vector<I64> levels(thresholds.begin(), thresholds.end());
    // [Bjerkevik, sec. 3, bottleneck formula](../../english/02-bjerkevik.md#bj-bottleneck)
    std::size_t lo = 0, hi = levels.size() - 1;
    while (lo < hi) {
        auto mid = lo + (hi - lo) / 2;
        masks(levels[mid]);
        if (matching(mutual, perm))
            hi = mid;
        else
            lo = mid + 1;
    }
    I64 bottleneck = levels[lo];
    masks(bottleneck);
    matching(mutual, perm);
    Witness witness{Columns(n), Columns(n)};
    for (int i = 0; i < n; ++i) {
        witness.forward[i] = {perm[i]};
        witness.backward[perm[i]] = {i};
    }
    Budget budget{limit};
    I64 upper = bottleneck, lower = 0;
    for (std::size_t t = 0; t < lo; ++t) {
        masks(levels[t]);
        Witness w;
        auto decision = feasible(p, q, w, budget);
        if (decision == Decision::yes) {
            upper = levels[t];
            witness = std::move(w);
            break;
        }
        if (decision == Decision::no)
            lower = levels[t + 1];
    }
    // Validate the upper endpoint directly against the original inclusion costs.
    for (int i = 0; i < n; ++i) {
        Vec composite;
        for (int j : witness.forward[i]) {
            if (h[i][j] > upper)
                throw Error("forward interleaving certificate exceeds its grade");
            add(composite, witness.backward[j]);
        }
        for (int j : witness.backward[i])
            if (back[i][j] > upper)
                throw Error("backward interleaving certificate exceeds its grade");
        if (composite != Vec{i})
            throw Error("interleaving certificate is not invertible");
    }
    bool exact = upper == lower;
    std::cout << "{\"status\":\"" << (exact ? "exact" : "bounded") << "\",\"dI\":";
    if (exact)
        std::cout << "\"" << price(upper) << "\"";
    else
        std::cout << "null";
    std::cout << ",\"lower\":\"" << price(lower) << "\",\"upper\":\"" << price(upper)
              << "\",\"dB\":\"" << price(bottleneck) << "\",\"examined\":" << budget.examined
              << ",\"matching\":";
    json_vec(perm);
    std::cout << ",\"forward_columns\":";
    json_cols(witness.forward);
    std::cout << ",\"backward_columns\":";
    json_cols(witness.backward);
    std::cout << "}\n";
}
} // namespace mp
