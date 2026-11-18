#pragma once
#include "index_file.hpp"
#include "lazy_intersections.hpp"

namespace mp {
struct TripleTable : CriticalGrid {
    using Key = std::array<std::size_t, 3>;
    static constexpr auto none = std::numeric_limits<std::size_t>::max();
    static constexpr const char *signature = "MPTRIPL1";
    std::array<std::size_t,3> counts{};
    std::array<unsigned,3> widths{};
    std::array<std::string,3> answers;
    std::map<Key,std::size_t> starts;

    GridPoint top() const {
        return {static_cast<int>(axes[0].size())-1, static_cast<int>(axes[1].size())-1,
                static_cast<int>(axes[2].size())-1};
    }
    static GridPoint upper(const GridPoint &a,const GridPoint &b) {
        return {std::max(a[0],b[0]), std::max(a[1],b[1]), std::max(a[2],b[2])};
    }
    // [Grid note, (6), rank within the upper orthant](../../english/04-query-index.md#table-offset)
    std::size_t local_offset(const GridPoint &low,const GridPoint &q) const {
        return ((std::size_t(q[0]-low[0]) * (axes[1].size()-low[1]) + q[1]-low[1]) *
                (axes[2].size()-low[2]) + q[2]-low[2]);
    }
    std::vector<GridPoint> points(const GridPoint &q) const {
        std::vector<GridPoint> result;
        for (int x=0; x<=q[0]; ++x)
            for (int y=0; y<=q[1]; ++y)
                for (int z=0; z<=q[2]; ++z) result.push_back({x,y,z});
        return result;
    }
    // One contiguous upper orthant per unordered antichain. File positions
    // depend only on the axes, so source keys need no serialized directory.
    // [Grid note, (5) and (6), unordered minimal-source keys](../../english/04-query-index.md#table-offset)
    template<class Visitor>
    static void enumerate(const std::vector<GridPoint> &ps, Visitor visit) {
        for (std::size_t i=0; i<ps.size(); ++i) {
            visit(i,none,none);
            std::vector<std::size_t> later;
            for (std::size_t j=i+1; j<ps.size(); ++j)
                if (!grid_le(ps[i],ps[j])) later.push_back(j);
            for (std::size_t j=0; j<later.size(); ++j) {
                visit(i,later[j],none);
                for (std::size_t k=j+1; k<later.size(); ++k)
                    if (!grid_le(ps[later[j]],ps[later[k]])) visit(i,later[j],later[k]);
            }
        }
    }
    void directory() {
        const auto n=grid_size();
        if (!n) return;
        const auto last=top();
        auto ps=points(last);
        enumerate(ps,[&](auto i,auto j,auto k) {
            const int order=k!=none ? 2 : (j!=none ? 1 : 0);
            auto low=ps[i];
            if (j!=none) low=upper(low,ps[j]);
            if (k!=none) low=upper(low,ps[k]);
            const auto block=local_offset(low,last)+1;
            if (block > none-counts[order]) throw Error("triple table is too large");
            starts.emplace(Key{i,j,k},counts[order]);
            counts[order]+=block;
        });
    }
    std::size_t query(const Grade &q, const std::vector<Grade> &sources) const {
        if (sources.size()>3) throw Error("triple table accepts at most three sources");
        auto ps=minimal_sources(*this,q,sources);
        if (ps.empty()) return 0;
        const auto order=ps.size()-1;
        Key key{none,none,none};
        auto low=ps.front();
        for (std::size_t i=0;i<ps.size();++i) {
            key[i]=offset(ps[i]);
            low=upper(low,ps[i]);
        }
        const auto pos=(starts.at(key)+local_offset(low,locate(q)))*widths[order];
        std::uint32_t answer=0;
        for (unsigned b=0;b<widths[order];++b)
            answer |= std::uint32_t(static_cast<unsigned char>(answers[order][pos+b])) << (8*b);
        return answer;
    }
};

inline TripleTable build_table(const IntersectionIndex &idx) {
    TripleTable t;
    static_cast<CriticalGrid &>(t)=idx;
    t.directory();
    std::array<std::vector<std::uint32_t>,3> values;
    for (int a=0; a<3; ++a) values[a].resize(t.counts[a]);
    for (const auto &q : t.points(t.top())) {
        auto ps=t.points(q);
        std::vector<Columns> images;
        for (const auto &p : ps) images.push_back(idx.image(p,q));
        Columns pair;
        TripleTable::enumerate(ps,[&](auto i,auto j,auto k) {
            int order=0;
            auto low=ps[i];
            TripleTable::Key key{t.offset(low),t.none,t.none};
            std::size_t value=images[i].size();
            if (j!=t.none) {
                key[1]=t.offset(ps[j]);
                low=t.upper(low,ps[j]);
                // Enumeration visits the pair immediately before all its triples.
                // [TDA, sec. 6, equal-image equation](../../english/03-tda.md#tda-equal-images)
                if (k==t.none) pair=intersection(images[i],images[j]);
                order=1;
                value=pair.size();
            }
            if (k!=t.none) {
                order=2;
                key[2]=t.offset(ps[k]);
                low=t.upper(low,ps[k]);
                // [Grid note, (7), reuse of the pair intersection](../../english/04-query-index.md#pair-reuse)
                value=pair.empty() ? 0 : intersection(pair,images[k]).size();
            }
            values[order][t.starts.at(key)+t.local_offset(low,q)]=static_cast<std::uint32_t>(value);
        });
    }
    for (int a=0; a<3; ++a) {
        const auto &v=values[a];
        const auto largest=v.empty() ? 0 : *std::max_element(v.begin(),v.end());
        t.widths[a]=largest<=255 ? 1 : (largest<=65535 ? 2 : 4);
        if (v.size()>t.answers[a].max_size()/t.widths[a]) throw Error("answer payload is too large");
        t.answers[a].reserve(v.size()*t.widths[a]);
        for (auto answer:v)
            for (unsigned b=0;b<t.widths[a];++b) t.answers[a].push_back(static_cast<char>(answer>>(8*b)));
    }
    return t;
}
inline void save_table(const TripleTable &t,const std::filesystem::path &path) {
    std::string bytes(t.signature);
    encode_grid(bytes,t);
    for (int a=0; a<3; ++a) {
        index_format::put(bytes,t.widths[a]);
        index_format::put(bytes,t.counts[a]);
        bytes+=t.answers[a];
    }
    publish_checked(path,std::move(bytes));
}
inline TripleTable load_table(const std::filesystem::path &path) {
    TripleTable t;
    auto file=read_checked(path,t.signature);
    index_format::Reader input(std::string_view(file).substr(8));
    decode_grid(input,t);
    std::array<std::size_t,3> expected{};
    for (int a=0;a<3;++a) {
        auto w=input.integer(), count=input.integer();
        if ((w!=1 && w!=2 && w!=4) || count>input.left()/w)
            throw Error("invalid triple table length");
        t.widths[a]=static_cast<unsigned>(w);
        expected[a]=static_cast<std::size_t>(count);
        t.answers[a]=input.take(expected[a]*w);
    }
    if (input.left() || t.grid_size()>expected[0]) throw Error("invalid triple table shape");
    t.directory();
    if (t.counts!=expected) throw Error("inconsistent triple table directory");
    return t;
}
} // namespace mp
