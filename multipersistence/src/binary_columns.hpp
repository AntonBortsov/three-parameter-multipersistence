#pragma once
#include <algorithm>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

namespace mp {
using Vec = std::vector<int>; // Increasing support of a sparse vector over F2.
using Columns = std::vector<Vec>;
inline void add(Vec &a, const Vec &b) {
    if (b.empty())
        return;
    if (a.empty()) {
        a = b;
        return;
    }
    Vec tmp;
    tmp.reserve(a.size() + b.size());
    std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(tmp));
    a.swap(tmp);
}
struct Elimination {
    struct Entry {
        Vec col, tag;
    };
    std::map<int, Entry> pivots;
    // A quotient representative must have *every* pivot coordinate cleared.
    // A free leading row is retained while reduction continues on the lower rows.
    // Pivot elimination gives one linear representative for each coset modulo target boundaries.
    // [TDA, sec. 6, inverse image of a homology image](../../english/03-tda.md#tda-joint-image)
    // [Grid note, (2), unique complement representative](../../english/04-query-index.md#cycle-labels)
    Vec normal_form(Vec v) const {
        Vec free_rows;
        while (!v.empty()) {
            auto it = pivots.find(v.back());
            if (it == pivots.end()) {
                free_rows.push_back(v.back());
                v.pop_back();
            } else {
                add(v, it->second.col);
            }
        }
        std::reverse(free_rows.begin(), free_rows.end());
        return free_rows;
    }
    std::pair<Vec, Vec> reduce(Vec v, Vec t = {}) const {
        while (!v.empty()) {
            auto it = pivots.find(v.back());
            if (it == pivots.end())
                break;
            add(v, it->second.col);
            add(t, it->second.tag);
        }
        return {std::move(v), std::move(t)};
    }
    Vec insert(Vec v, Vec tag = {}) {
        auto [r, t] = reduce(std::move(v), std::move(tag));
        if (r.empty())
            return t;
        int pivot = r.back();
        pivots.emplace(pivot, Entry{std::move(r), std::move(t)});
        return {};
    }
    Columns basis() const {
        Columns out;
        for (const auto &kv : pivots)
            out.push_back(kv.second.col);
        return out;
    }
};
inline Columns independent(const Columns &cols) {
    Elimination e;
    for (const auto &v : cols)
        e.insert(v);
    return e.basis();
}
// Tags recover the A-part of a relation Ax = By, yielding the vector shared by the two images.
// [TDA, sec. 6, column-space intersection](../../english/03-tda.md#tda-equal-images)
inline Columns intersection(const Columns &a, const Columns &b) {
    Elimination e;
    for (int i = 0; i < static_cast<int>(a.size()); ++i)
        e.insert(a[i], {i});
    Columns out;
    for (const Vec &v : b) {
        auto [r, t] = e.reduce(v);
        if (r.empty()) {
            Vec common;
            for (int i : t)
                add(common, a[i]);
            if (!common.empty())
                out.push_back(std::move(common));
        } else {
            // Keep only tags for the A-part of each relation.
            int pivot = r.back();
            e.pivots.emplace(pivot, Elimination::Entry{std::move(r), std::move(t)});
        }
    }
    return independent(out);
}
} // namespace mp
