#include "lazy_intersections.hpp"
#include <iostream>

int main(int argc, char **argv) {
    try {
        if (argc != 2) throw mp::Error("expected test filtration");
        mp::LazyIntersections cache(mp::load(argv[1]),1);
        const auto a=mp::grade("0,1,1"), b=mp::grade("1,0,1"), c=mp::grade("1,1,0");
        const auto top=mp::grade("1,1,1"), below=mp::grade("0,0,0");
        if (cache.target_count()!=0 || cache.query(top,{a,b,c})!=1 || cache.target_count()!=1)
            throw mp::Error("first target was not materialized on demand");
        if (cache.query(a,{below})!=0 || cache.target_count()!=2)
            throw mp::Error("second target was not retained");
        if (cache.query(mp::grade("1.5,1,1"),{c,b,a})!=1 || cache.target_count()!=2)
            throw mp::Error("returning to a critical target rebuilt the cache");
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
