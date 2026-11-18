#pragma once
#include "binary_columns.hpp"
#include <algorithm>
#include <array>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace mp {
using Error = std::runtime_error;
using I64 = std::int64_t;
using U64 = std::uint64_t;

inline U64 digits(std::string_view s) {
    U64 n = 0;
    if (s.empty())
        throw Error("expected an unsigned integer");
    auto result = std::from_chars(s.data(), s.data() + s.size(), n);
    if (result.ec != std::errc{} || result.ptr != s.data() + s.size())
        throw Error("invalid or oversized integer: " + std::string(s));
    return n;
}
inline int id_number(const std::string &s) {
    U64 v = digits(s);
    if (v > std::numeric_limits<int>::max())
        throw Error("integer index out of range");
    return static_cast<int>(v);
}
inline std::vector<std::string> split(const std::string &s, char sep) {
    std::vector<std::string> out;
    std::size_t pos = 0;
    for (;;) {
        auto end = s.find(sep, pos);
        out.push_back(s.substr(pos, end - pos));
        if (end == std::string::npos)
            return out;
        pos = end + 1;
    }
}
// Compare nonnegative fractions by continued fractions, without cross multiplication.
inline int fraction_order(U64 a, U64 b, U64 c, U64 d) {
    int sign = 1;
    for (;;) {
        U64 x = a / b, y = c / d;
        if (x != y)
            return x < y ? -sign : sign;
        a %= b;
        c %= d;
        if (!a || !c)
            return a == c ? 0 : (!a ? -sign : sign);
        std::swap(a, b);
        std::swap(c, d);
        sign = -sign;
    }
}
struct Rational {
    U64 n = 0, d = 1;
    bool negative = false;
    static Rational parse(std::string s) {
        if (s.empty())
            throw Error("missing coordinate");
        bool neg = s[0] == '-';
        if (neg || s[0] == '+')
            s.erase(0, 1);
        auto slash = s.find('/'), dot = s.find('.');
        U64 n = 0, d = 1;
        if (slash != s.npos) {
            n = digits(s.substr(0, slash));
            d = digits(s.substr(slash + 1));
        } else if (dot != s.npos) {
            std::string f = s.substr(dot + 1);
            if (f.empty() && dot == 0)
                throw Error("invalid decimal");
            if (f.find_first_not_of("0123456789") != std::string::npos)
                throw Error("invalid decimal");
            while (!f.empty() && f.back() == '0')
                f.pop_back();
            for (std::size_t i = 0; i < f.size(); ++i) {
                if (d > std::numeric_limits<U64>::max() / 10)
                    throw Error("decimal precision out of range");
                d *= 10;
            }
            n = digits((dot ? s.substr(0, dot) : "0") + f);
        } else
            n = digits(s);
        if (!d)
            throw Error("zero denominator");
        U64 g = std::gcd(n, d);
        return {n / g, d / g, n != 0 && neg};
    }
    bool operator==(const Rational &b) const {
        return n == b.n && d == b.d && negative == b.negative;
    }
    bool operator<(const Rational &b) const {
        if (negative != b.negative)
            return negative;
        if (d == b.d)
            return negative ? n > b.n : n < b.n;
        int cmp = fraction_order(n, d, b.n, b.d);
        return negative ? cmp > 0 : cmp < 0;
    }
    std::string str() const {
        return (negative ? "-" : "") + std::to_string(n) + (d == 1 ? "" : "/" + std::to_string(d));
    }
};
using Grade = std::array<Rational, 3>;
inline Grade grade(const std::string &s) {
    auto t = split(s, ',');
    if (t.size() != 3)
        throw Error("expected three comma-separated coordinates: " + s);
    return {Rational::parse(t[0]), Rational::parse(t[1]), Rational::parse(t[2])};
}
inline bool le(const Grade &a, const Grade &b) {
    for (int i = 0; i < 3; ++i)
        if (b[i] < a[i])
            return false;
    return true;
}
inline Grade join(const Grade &a, const Grade &b) {
    return {std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2])};
}
inline std::string show(const Grade &g) { return g[0].str() + "," + g[1].str() + "," + g[2].str(); }
struct Record {
    std::string key;
    std::vector<Grade> births;
};
inline std::vector<Record> read_records(const std::string &path) {
    std::ifstream in(path);
    if (!in)
        throw Error("cannot open " + path);
    std::vector<Record> out;
    std::string line;
    int number = 0;
    while (std::getline(in, line)) {
        ++number;
        if (number == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0)
            line.erase(0, 3);
        line = line.substr(0, line.find('#'));
        if (line.find_first_not_of(" \t\r") == std::string::npos)
            continue;
        try {
            auto colon = line.find(':');
            if (colon == line.npos)
                throw Error("missing ':'");
            std::istringstream left(line.substr(0, colon)), right(line.substr(colon + 1));
            std::string key, tmp;
            if (!(left >> key) || (left >> tmp))
                throw Error("expected one record label");
            std::vector<Grade> all, antichain;
            while (right >> tmp)
                all.push_back(grade(tmp));
            if (all.empty())
                throw Error("record has no births");
            std::sort(all.begin(), all.end());
            all.erase(std::unique(all.begin(), all.end()), all.end());
            for (const auto &p : all) {
                if (std::none_of(antichain.begin(), antichain.end(),
                                 [&](const Grade &q) { return le(q, p); }))
                    antichain.push_back(p);
            }
            out.push_back({key, std::move(antichain)});
        } catch (const Error &e) {
            throw Error(path + ":" + std::to_string(number) + ": " + e.what());
        }
    }
    if (in.bad())
        throw Error("read failed: " + path);
    return out;
}
inline std::vector<Vec> facets(const Vec &s) {
    std::vector<Vec> out;
    if (s.size() > 1)
        for (std::size_t i = 0; i < s.size(); ++i) {
            auto face = s;
            face.erase(face.begin() + i);
            out.push_back(face);
        }
    return out;
}
struct Simplex {
    Vec vertices, faces;
    std::vector<Grade> births;
};
using Filtration = std::vector<Simplex>;
// A coface is active on the upset of its births. Testing each birth against each
// facet therefore checks the inclusion on the entire parameter space.
// [TDA, sec. 1, support of a simplex](../../english/03-tda.md#tda-support)
inline Filtration load(const std::string &path) {
    std::map<Vec, std::vector<Grade>> data;
    for (auto &rec : read_records(path)) {
        Vec v;
        for (const auto &s : split(rec.key, ','))
            v.push_back(id_number(s));
        std::sort(v.begin(), v.end());
        if (v.size() > 3 || std::adjacent_find(v.begin(), v.end()) != v.end())
            throw Error("invalid simplex: " + rec.key);
        if (!data.emplace(v, std::move(rec.births)).second)
            throw Error("duplicate simplex: " + rec.key);
    }
    Filtration out;
    std::map<Vec, int> ids;
    for (std::size_t size = 1; size <= 3; ++size)
        for (auto &[v, g] : data)
            if (v.size() == size) {
                Vec f;
                for (const auto &face : facets(v)) {
                    if (!ids.count(face))
                        throw Error("absent face of simplex starting at " + std::to_string(v[0]));
                    int i = ids.at(face);
                    for (const auto &p : g) {
                        auto &h = out[i].births;
                        if (std::none_of(h.begin(), h.end(),
                                         [&](const Grade &a) { return le(a, p); }))
                            throw Error("face inactive at coface birth " + show(p));
                    }
                    f.push_back(i);
                }
                std::sort(f.begin(), f.end());
                ids[v] = static_cast<int>(out.size());
                out.push_back({v, f, std::move(g)});
            }
    return out;
}
} // namespace mp
