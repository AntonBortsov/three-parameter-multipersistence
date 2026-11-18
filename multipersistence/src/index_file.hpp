#pragma once
#include "intersection_index.hpp"
#include <chrono>
#include <filesystem>

namespace mp {
namespace index_format {
// A fixed version tag, unsigned LEB128 fields, then an eight-byte little-endian
// FNV-1a checksum over the preceding bytes. Sparse row lists use positive gaps.
inline constexpr char magic[] = "MPHGRID1";
inline U64 checksum(std::string_view data) {
    U64 h = 14695981039346656037ULL;
    for (unsigned char c : data) h = (h ^ c) * 1099511628211ULL;
    return h;
}
inline void put(std::string &out, U64 v) {
    while (v >= 128) {
        out.push_back(static_cast<char>((v & 127) | 128));
        v >>= 7;
    }
    out.push_back(static_cast<char>(v));
}
class Reader {
    std::string_view data;
public:
    explicit Reader(std::string_view payload) : data(payload) {}
    std::size_t left() const { return data.size(); }
    std::string take(std::size_t n) {
        if (n > left()) throw Error("incomplete index payload");
        std::string part(data.substr(0,n));
        data.remove_prefix(n);
        return part;
    }
    U64 integer() {
        U64 value = 0;
        for (int shift = 0; shift <= 63 && !data.empty(); shift += 7) {
            const auto c = static_cast<unsigned char>(data.front());
            data.remove_prefix(1);
            if (shift == 63 && c > 1) break;
            value |= U64(c & 127) << shift;
            if (!(c & 128)) {
                if (shift && !c) break;
                return value;
            }
        }
        throw Error("invalid index integer");
    }
    int bounded(U64 maximum) {
        const auto n = integer();
        if (n > maximum || n > static_cast<U64>(std::numeric_limits<int>::max()))
            throw Error("index field exceeds bounds");
        return static_cast<int>(n);
    }
};
} // namespace index_format

inline void encode_grid(std::string &data, const CriticalGrid &idx) {
    using namespace index_format;
    put(data, idx.degree);
    for (const auto &axis : idx.axes) {
        put(data, axis.size());
        for (const auto &r : axis) {
            put(data, r.negative);
            put(data, r.n);
            put(data, r.d);
        }
    }
}

inline void publish_checked(const std::filesystem::path &dest, std::string data) {
    using namespace index_format;
    const auto h = checksum(data);
    for (int i = 0; i < 8; ++i) data.push_back(static_cast<char>(h >> (8 * i)));
    // Publish only a closed, complete file. Concurrent writers have separate
    // staging directories on the destination filesystem.
    std::filesystem::path staging;
    do {
        staging = dest;
        staging += ".tmp-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
    } while (!std::filesystem::create_directory(staging));
    const auto tmp = staging / "index";
    try {
        std::ofstream out(tmp, std::ios::binary);
        out.exceptions(std::ios::badbit | std::ios::failbit);
        out.write(data.data(), static_cast<std::streamsize>(data.size()));
        out.close();
        std::filesystem::rename(tmp, dest);
    } catch (...) {
        std::error_code ec;
        std::filesystem::remove(tmp, ec);
        std::filesystem::remove(staging, ec);
        throw;
    }
    std::filesystem::remove(staging);
}

inline void save_index(const IntersectionIndex &idx, const std::filesystem::path &dest) {
    using namespace index_format;
    std::string data(magic);
    encode_grid(data, idx);
    for (const auto &node : idx.nodes) {
        put(data, node.dimension);
        for (const auto &edge : node.incoming)
            for (const auto &col : edge) {
                put(data, col.size());
                int previous = -1;
                for (int row : col) {
                    put(data, static_cast<U64>(I64(row) - previous));
                    previous = row;
                }
            }
    }
    publish_checked(dest, std::move(data));
}

inline std::string read_checked(const std::filesystem::path &path, std::string_view signature) {
    using namespace index_format;
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    const auto len = in.tellg();
    if (len < 16) throw Error("missing or incomplete homology index");
    std::string file(static_cast<std::size_t>(len), '\0');
    in.seekg(0);
    if (!in.read(file.data(), len)) throw Error("cannot read homology index");
    std::string_view data(file.data(), file.size() - 8);
    U64 expected = 0;
    for (int i = 0; i < 8; ++i)
        expected |= U64(static_cast<unsigned char>(file[data.size() + i])) << (8 * i);
    if (data.substr(0, 8) != signature || checksum(data) != expected)
        throw Error("unrecognized or corrupt homology index");
    file.resize(file.size() - 8);
    return file;
}

inline void decode_grid(index_format::Reader &reader, CriticalGrid &idx) {
    idx.degree = reader.bounded(4);
    for (auto &axis : idx.axes) {
        const int count = reader.bounded(reader.left() / 3);
        axis.reserve(count);
        for (int i = 0; i < count; ++i) {
            const bool negative = reader.bounded(1) != 0;
            const auto n = reader.integer(), d = reader.integer();
            Rational r{n, d, negative};
            if (!d || std::gcd(n, d) != 1 || (!n && negative) || (i && !(axis.back() < r)))
                throw Error("invalid critical coordinates");
            axis.push_back(r);
        }
    }
    const bool empty = idx.axes[0].empty();
    if (idx.axes[1].empty() != empty || idx.axes[2].empty() != empty) throw Error("inconsistent empty axes");
}

inline IntersectionIndex load_index(const std::filesystem::path &path) {
    using namespace index_format;
    auto file = read_checked(path, magic);
    Reader reader(std::string_view(file).substr(8));
    IntersectionIndex idx;
    decode_grid(reader, idx);
    const auto size = idx.grid_size();
    if (size > reader.left() || (!size && (!idx.axes[0].empty() || !idx.axes[1].empty() || !idx.axes[2].empty())))
        throw Error("invalid critical grid size");
    idx.nodes.resize(size);
    for (int x = 0; x < static_cast<int>(idx.axes[0].size()); ++x)
        for (int y = 0; y < static_cast<int>(idx.axes[1].size()); ++y)
            for (int z = 0; z < static_cast<int>(idx.axes[2].size()); ++z) {
                const GridPoint pos{x, y, z};
                auto &node = idx.nodes[idx.offset(pos)];
                node.dimension = reader.bounded(std::numeric_limits<int>::max());
                for (int a = 0; a < 3; ++a) {
                    if (!pos[a]) continue;
                    auto prev = pos;
                    --prev[a];
                    const int rank = idx.nodes[idx.offset(prev)].dimension;
                    if (static_cast<std::size_t>(rank) > reader.left()) throw Error("incomplete cover map");
                    auto &edge = node.incoming[a];
                    edge.resize(rank);
                    for (auto &col : edge) {
                        const int count = reader.bounded(std::min<std::size_t>(node.dimension, reader.left()));
                        int previous = -1;
                        for (int i = 0; i < count; ++i) {
                            const int gap = reader.bounded(I64(node.dimension) - 1 - previous);
                            if (!gap) throw Error("unordered index column");
                            previous += gap;
                            col.push_back(previous);
                        }
                    }
                }
            }
    if (reader.left()) throw Error("trailing homology index data");
    return idx;
}
} // namespace mp
