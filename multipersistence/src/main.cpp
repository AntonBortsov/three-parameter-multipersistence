#include "model.hpp"
#include "triple_table.hpp"
#include "distance.hpp"
#include "exchange.hpp"
#include "cloud.hpp"

namespace {
using namespace mp;
const char *usage = R"(multipersistence 0.1 | exact three-parameter homology over F2

  multipersistence summary FILE [--grid]
  multipersistence dump FILE [--grid]
  multipersistence verify FILE [--grid]
  multipersistence betti FILE DEGREE GRADE [--grid]
  multipersistence rank FILE DEGREE SOURCE TARGET [--grid]
  multipersistence intersect FILE DEGREE TARGET SOURCE [SOURCE ...] [--grid]
  multipersistence index-build FILE DEGREE INDEX
  multipersistence index-query INDEX TARGET SOURCE [SOURCE ...]
  multipersistence index-batch INDEX QUERIES
  multipersistence lazy-query FILE DEGREE TARGET SOURCE [SOURCE ...]
  multipersistence lazy-batch FILE DEGREE QUERIES
  multipersistence table-build FILE DEGREE TABLE
  multipersistence table-from-index INDEX TABLE
  multipersistence table-query TABLE TARGET SOURCE [SOURCE ...]  (up to three sources)
  multipersistence table-batch TABLE QUERIES
  multipersistence compare LEFT RIGHT [--budget MATRIX_CANDIDATES]
  multipersistence exchange MATRIX PREFIX [--budget STATES]
  multipersistence cloud POINTS [--max-radius2 SQUARED_RADIUS]

A grade is x,y,z. Coordinates may be integers, decimals or fractions.
Filtration rows: comma-separated simplex vertices : one or more birth grades
Upset-list rows for compare: label : one or more birth grades
PREFIX uses comma-separated zero-based column indices or '-' for empty.
Cloud writes filtration text. Other computations write JSON.
Index queries use the stored homology degree. Batch rows are TARGET SOURCE [SOURCE ...].
An index-batch emits one JSON object per nonblank query row.
Run verify for an exhaustive critical-grid cone check.
)";
void report(const Filtration &f, const Model &m, bool dump) {
    std::size_t nnz = 0;
    for (auto &c : m.cells)
        nnz += c.boundary.size();
    std::cout << "{\"simplices\":" << f.size() << ",\"generators\":" << m.cells.size()
              << ",\"nonzeros\":" << nnz << ",\"homotopy_nonzeros\":" << m.corrections
              << ",\"local_models\":[";
    for (std::size_t i = 0; i < f.size(); ++i) {
        if (i)
            std::cout << ",";
        auto &l = m.locals[i];
        std::cout << "{\"simplex\":";
        json_vec(f[i].vertices);
        std::cout << ",\"type\":\"" << l.kind << "\",\"tree_defect\":" << l.defect
                  << ",\"births\":" << f[i].births.size() << "}";
    }
    std::cout << "]";
    if (dump) {
        std::cout << ",\"cells\":[";
        for (std::size_t i = 0; i < m.cells.size(); ++i) {
            if (i)
                std::cout << ",";
            auto &c = m.cells[i];
            std::cout << "{\"id\":" << i << ",\"degree\":" << c.degree << ",\"grade\":\""
                      << show(c.birth) << "\",\"simplex_index\":" << c.simplex
                      << ",\"local_index\":" << c.local
                      << ",\"augmentation\":" << (c.augments ? std::to_string(c.simplex) : "null")
                      << ",\"boundary\":";
            json_vec(c.boundary);
            std::cout << "}";
        }
        std::cout << "]";
    }
    std::cout << "}\n";
}
template<class Engine>
int serve(Engine &idx, const std::vector<std::string> &args, std::size_t start, bool batch) {
    auto answer = [&](const std::string &target, auto first, auto end) {
        std::vector<Grade> sources;
        for (; first != end; ++first) sources.push_back(grade(*first));
        auto dimension = idx.query(grade(target), sources);
        std::cout << "{\"degree\":" << idx.degree << ",\"dimension\":" << dimension << "}\n";
    };
    if (!batch) {
        answer(args[start], args.begin() + start + 1, args.end());
    } else {
        std::ifstream in(args[start]);
        if (!in) throw Error("cannot open query file " + args[start]);
        std::string line, target;
        std::size_t number = 0;
        while (std::getline(in, line)) {
            ++number;
            std::istringstream row(line);
            if (!(row >> target)) continue;
            try {
                answer(target, std::istream_iterator<std::string>(row), std::istream_iterator<std::string>());
            } catch (const Error &e) {
                throw Error("query line " + std::to_string(number) + ": " + e.what());
            }
        }
        if (in.bad()) throw Error("failed to read query file");
    }
    return 0;
}
int run(std::vector<std::string> args) {
    if (args.empty() || args[0] == "--help" || args[0] == "help") {
        std::cout << usage;
        return 0;
    }
    auto cmd = args[0];
    if (cmd == "index-build") {
        if (args.size() != 4) throw Error("index-build expects a filtration, degree and output path");
        const int degree = id_number(args[2]);
        auto idx = build_index(load(args[1]), degree);
        save_index(idx, args[3]);
        std::cout << "{\"degree\":" << degree << ",\"nodes\":" << idx.nodes.size()
                  << ",\"bytes\":" << std::filesystem::file_size(args[3]) << "}\n";
        return 0;
    }
    if (cmd == "table-build" || cmd == "table-from-index") {
        bool from_index = cmd == "table-from-index";
        if (args.size() != (from_index ? 3u : 4u)) throw Error("wrong table build arguments; see --help");
        auto idx = from_index ? load_index(args[1]) : build_index(load(args[1]),id_number(args[2]));
        auto table = build_table(idx);
        save_table(table,args.back());
        std::cout << "{\"degree\":" << table.degree << ",\"triples\":" << table.counts[2]
                  << ",\"bytes\":" << std::filesystem::file_size(args.back()) << "}\n";
        return 0;
    }
    if (cmd == "index-query" || cmd == "index-batch" || cmd == "lazy-batch" ||
        cmd == "lazy-query" || cmd == "table-query" || cmd == "table-batch") {
        const bool lazy = cmd.compare(0,4,"lazy") == 0;
        const bool batch = cmd.find("-batch") != std::string::npos;
        const std::size_t start = lazy ? 3 : 2;
        if (batch ? args.size() != start+1 : args.size() < start+2)
            throw Error("wrong query arguments; see --help");
        if (lazy) {
            LazyIntersections engine(load(args[1]),id_number(args[2]));
            return serve(engine,args,start,batch);
        }
        if (cmd.compare(0,5,"table") == 0) {
            auto table = load_table(args[1]);
            return serve(table,args,start,batch);
        }
        auto idx = load_index(args[1]);
        return serve(idx,args,start,batch);
    }
    if (cmd == "cloud") {
        if (args.size() != 2 && !(args.size() == 4 && args[2] == "--max-radius2"))
            throw Error("cloud expects a point file and optional --max-radius2 N");
        cloud(args[1], args.size() == 4 ? digits(args[3]) : std::numeric_limits<U64>::max());
        return 0;
    }
    if (cmd == "exchange") {
        if (args.size() != 3 && !(args.size() == 5 && args[3] == "--budget"))
            throw Error("exchange expects a matrix file, prefix and optional --budget N");
        exchange(args[1], args[2], args.size() == 5 ? digits(args[4]) : 1000000);
        return 0;
    }
    if (cmd == "compare") {
        if (args.size() != 3 && !(args.size() == 5 && args[3] == "--budget"))
            throw Error("compare expects two upset-list files and optional --budget N");
        compare(args[1], args[2], args.size() == 5 ? digits(args[4]) : 1000000);
        return 0;
    }
    bool force_grid = args.back() == "--grid";
    if (force_grid)
        args.pop_back();
    if (args.size() < 2)
        throw Error("expected a filtration file; see --help");
    bool run_query = cmd == "betti" || cmd == "rank" || cmd == "intersect";
    if (!run_query && cmd != "dump" && cmd != "summary" && cmd != "verify")
        throw Error("unknown command: " + cmd);
    if ((!run_query && args.size() != 2) || (cmd == "betti" && args.size() != 4) ||
        (cmd == "rank" && args.size() != 5) || (cmd == "intersect" && args.size() < 5))
        throw Error("wrong argument count; see --help");
    auto f = load(args[1]);
    auto m = build(f, force_grid);
    check_chain(f, m);
    if (cmd == "summary" || cmd == "dump")
        report(f, m, cmd == "dump");
    else if (cmd == "verify") {
        auto cells = verify(f, m);
        std::cout << "{\"acyclic_cones\":true,\"critical_grades\":" << cells << "}\n";
    } else {
        int degree = id_number(args[2]);
        if (degree > 4)
            throw Error("supported chain degrees are 0 through 4");
        Grade target = grade(args[cmd == "rank" ? 4 : 3]);
        std::vector<Grade> sources;
        if (cmd == "intersect")
            for (std::size_t i = 4; i < args.size(); ++i)
                sources.push_back(grade(args[i]));
        else
            sources.push_back(grade(args[3]));
        auto dimension = query(m, degree, target, sources);
        std::cout << "{\"degree\":" << degree << ",\"dimension\":" << dimension << "}\n";
    }
    return 0;
}
} // namespace
int main(int argc, char **argv) {
    try {
        return run({argv + 1, argv + argc});
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
