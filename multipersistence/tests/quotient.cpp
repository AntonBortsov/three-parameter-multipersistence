#include "binary_columns.hpp"
#include <iostream>

namespace {
mp::Vec support(unsigned mask) {
    mp::Vec out;
    for (int i = 0; i < 5; ++i)
        if (mask & (1u << i))
            out.push_back(i);
    return out;
}
}

int main() {
    // The free row 4 can mask the pivots 3 and 2 under ordinary echelon reduction.
    // [TDA, sec. 6, simultaneous-image formula](../../english/03-tda.md#tda-joint-image)
    mp::Elimination boundaries;
    boundaries.insert({0, 2});
    boundaries.insert({1, 2, 3});
    if (boundaries.normal_form({3, 4}) != mp::Vec{0, 1, 4}) {
        std::cerr << "pivot below a free row survived quotient projection\n";
        return 1;
    }
    for (unsigned x = 0; x < 32; ++x) {
        const auto projected = boundaries.normal_form(support(x));
        const bool in_kernel = x == 0 || x == 5 || x == 14 || x == (5 ^ 14);
        if (projected.empty() != in_kernel || boundaries.normal_form(projected) != projected)
            return 2;
        for (unsigned y = 0; y < 32; ++y) {
            auto sum = projected;
            mp::add(sum, boundaries.normal_form(support(y)));
            if (sum != boundaries.normal_form(support(x ^ y)))
                return 3;
        }
    }
    return 0;
}
