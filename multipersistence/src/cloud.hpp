#pragma once
#include "core.hpp"

namespace mp {
struct Point {
    int id;
    std::array<I64, 3> coordinate;
    Rational mark;
};
inline std::vector<Point> load_points(const std::string &path) {
    std::ifstream input(path);
    if (!input)
        throw Error("cannot open " + path);
    std::map<int, Point> points;
    int number = 0;
    std::string line;
    while (std::getline(input, line)) {
        ++number;
        if (number == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0)
            line.erase(0, 3);
        std::istringstream row(line.substr(0, line.find('#')));
        std::string label, marks, extra;
        if (!(row >> label))
            continue;
        try {
            Point p;
            p.id = id_number(label);
            for (auto &x : p.coordinate) {
                std::string val;
                if (!(row >> val))
                    throw Error("expected three spatial coordinates");
                auto z = Rational::parse(val);
                if (z.d != 1 || z.n > 1000000000)
                    throw Error(
                        "spatial coordinates must be integers of magnitude at most 1000000000");
                x = z.negative ? -static_cast<I64>(z.n) : static_cast<I64>(z.n);
            }
            if (!(row >> marks) || (row >> extra))
                throw Error("expected one rational mark after the coordinates");
            p.mark = Rational::parse(marks);
            if (!points.emplace(p.id, p).second)
                throw Error("duplicate point identifier");
        } catch (const Error &e) {
            throw Error(path + ":" + std::to_string(number) + ": " + e.what());
        }
    }
    if (input.bad())
        throw Error("read failed: " + path);
    std::vector<Point> out;
    for (const auto &entry : points)
        out.push_back(entry.second);
    return out;
}
// Radius is recorded as squared Euclidean distance. All events and all birth grades are exact.
inline void cloud(const std::string &file, U64 cap) {
    auto pts = load_points(file);
    int n = static_cast<int>(pts.size());
    std::set<U64> radii{0};
    std::set<Rational> marks;
    std::vector<std::vector<U64>> distance(n, std::vector<U64>(n));
    for (int i = 0; i < n; ++i) {
        marks.insert(pts[i].mark);
        for (int j = 0; j < i; ++j) {
            U64 total = 0;
            for (int k = 0; k < 3; ++k) {
                I64 dx = pts[i].coordinate[k] - pts[j].coordinate[k];
                total += static_cast<U64>(dx * dx);
            }
            distance[i][j] = distance[j][i] = total;
            if (total <= cap)
                radii.insert(total);
        }
    }
    std::map<Vec, std::vector<Grade>> supports;
    for (auto t : marks) {
        Vec active;
        for (int i = 0; i < n; ++i)
            if (!(t < pts[i].mark))
                active.push_back(i);
        for (U64 radius : radii) {
            Vec missing(n, n - 1);
            for (int i : active)
                for (int j : active)
                    if (i != j && distance[i][j] <= radius)
                        --missing[i];
            // The largest missing-neighbour count among the vertices is the first density
            // level admitting the clique. The radius coordinate here is squared distance.
            // [TDA, sec. 7, marked clique filtration](../../english/03-tda.md#tda-cloud)
            auto emit = [&](const Vec &ids) {
                Vec simplex;
                int s = 0;
                for (int i : ids) {
                    simplex.push_back(pts[i].id);
                    s = std::max(s, missing[i]);
                }
                Grade g{Rational{radius, 1, false}, Rational{static_cast<U64>(s), 1, false}, t};
                auto &old = supports[simplex];
                if (std::none_of(old.begin(), old.end(), [&](const Grade &p) { return le(p, g); }))
                    old.push_back(g);
            };
            for (std::size_t a = 0; a < active.size(); ++a) {
                int i = active[a];
                emit({i});
                for (std::size_t b = a + 1; b < active.size(); ++b) {
                    int j = active[b];
                    if (distance[i][j] > radius)
                        continue;
                    emit({i, j});
                    for (std::size_t c = b + 1; c < active.size(); ++c) {
                        int k = active[c];
                        if (distance[i][k] <= radius && distance[j][k] <= radius)
                            emit({i, j, k});
                    }
                }
            }
        }
    }
    for (const auto &[simplex, births] : supports) {
        for (std::size_t i = 0; i < simplex.size(); ++i)
            std::cout << (i ? "," : "") << simplex[i];
        std::cout << " :";
        for (const auto &g : births)
            std::cout << " " << show(g);
        std::cout << "\n";
    }
}
} // namespace mp
