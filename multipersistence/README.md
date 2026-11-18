# Three-parameter multipersistence in C++17

This command-line program builds an exact free chain model of a multicritical simplicial filtration over the field with two elements. It computes Betti numbers, ranks of persistence maps, and intersections of images from several incomparable grades. It also solves two related finite problems: comparing explicit lists of upset summands over the binary field and finding serial exchanges for a represented pair of matroid bases.

## Run the Windows build

Build the executable with the instructions below and open PowerShell in the multipersistence source folder. The following examples use a single-configuration build. With Visual Studio, set the executable path to `./build/Release/multipersistence.exe`. The two filtrations have identical first-homology barcodes on every monotone line.

~~~powershell
$mp = "./build/multipersistence.exe"
& $mp intersect examples/slices-a.txt 1 1,1,1 0,1,1 1,0,1 1,1,0
& $mp intersect examples/slices-b.txt 1 1,1,1 0,1,1 1,0,1 1,1,0
~~~

The integer after the filename is the homological degree. The first grade is the target and the remaining grades are the sources. The first command returns dimension 1, the second dimension 0. Each individual source has rank 1 at the target in both examples.

For an end-to-end point-cloud example:

~~~powershell
& $mp cloud examples/square.points | Set-Content -Encoding ascii square.txt
& $mp betti square.txt 1 1,1,0
& $mp rank square.txt 1 1,1,0 2,1,0
& $mp verify square.txt
~~~

The square has one loop when only its sides have appeared. Its image vanishes once the diagonals and triangular faces appear. The two dimensions returned are 1 and 0.

## Repeated three-parameter intersections

Build one index per filtration and homology degree. Open it for a single query or pass a text file of queries to keep the index in memory throughout a batch. Every query row gives the target followed by one or more sources. An indexed batch writes one JSON answer per nonblank row, in input order.

~~~text
multipersistence index-build examples/slices-a.txt 1 slices-a.hgi
multipersistence index-query slices-a.hgi 1,1,1 0,1,1 1,0,1 1,1,0
multipersistence index-batch slices-a.hgi queries.txt
~~~

The example returns dimension 1. Every rational target and source is supported subject to the source being below the target. The saved file is portable between the Windows and Linux builds. An index stores the maps on a finite critical grid. Many distinct coordinate values on all three axes can make construction expensive. [Construction, correctness and storage](docs/adr/0002-critical-grid-index.md)

### Target cache and answer table

For repeated targets, the lazy engine reads the original filtration once and retains target projections and source images until the batch ends. A fresh process starts with an empty cache. The answer table materializes all intersections of one, two or three minimal sources on the critical grid. A submitted triple may contain equal or comparable grades. Conversion of an existing grid requires only its HGI file.

~~~text
multipersistence lazy-batch examples/slices-a.txt 1 queries.txt
multipersistence table-build examples/slices-a.txt 1 slices-a.htt
multipersistence table-from-index slices-a.hgi converted.htt
multipersistence table-query slices-a.htt 1,1,1 0,1,1 1,0,1 1,1,0
multipersistence table-batch slices-a.htt queries.txt
~~~

The lazy and grid modes also handle four or more sources. The table accepts up to three; it has a separate file format and can be opened without the input filtration. Linux and Windows builds use the same portable bytes.

## Build from source

The C++17 sources use the standard library. With a C++ compiler and CMake installed, run the following from this directory:

~~~text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
~~~

With a single-configuration generator the executable is in build. Visual Studio normally places it in build/Release. If Python 3 is available during configuration, CTest registers the CLI regression suite. The same suite can be run directly with a path to the executable:

~~~text
python tests/regression.py ./build/multipersistence.exe
~~~

A direct GNU or Clang build is also sufficient:

~~~text
c++ -std=c++17 -O2 src/main.cpp -o multipersistence
~~~

## Filtration input

Each row has a simplex on the left and one or more birth grades on the right. A grade is an ordered triple with no spaces. Vertex identifiers are distinct nonnegative integers within a simplex. Spaces around the colon are optional. A hash starts a comment.

~~~text
0 : 0,2,0 2,0,1
1 : 0,0,0
0,1 : 0,2,0
~~~

Rows may be unordered. Grade order is coordinatewise, with equality included. Each simplex is listed once. Every nonempty face must be listed and active at every birth of its coface. The empty file represents the empty complex. Duplicate and dominated birth grades are normalized away. The filtration can contain vertices, edges and triangles.

Integers, fractions such as -2/3 and finite decimals such as 0.125 are parsed exactly. Components use an unsigned 64-bit magnitude and positive 64-bit denominator, with a separate sign. Cross-products are avoided during comparison. Oversized input numerals are rejected. The upset distance calculation additionally requires a representable common coordinate denominator and scaled magnitudes at most half the positive signed 64-bit range.

## Homology and chain-model commands

| Command arguments following the executable | Result |
|---|---|
| summary FILE | Numbers of generators, boundary entries and correction terms, plus the chosen local resolution and tree defect for every simplex |
| dump FILE | The same summary and the full sparse differential, birth grades and augmentation |
| verify FILE | Pointwise acyclicity test for the cone of the augmentation at every critical coordinate combination |
| betti FILE DEGREE GRADE | Homology dimension at one grade |
| rank FILE DEGREE SOURCE TARGET | Rank of the map from source to target |
| intersect FILE DEGREE TARGET SOURCE ... | Dimension of the intersection of all source images in target homology |

Append --grid to any of these six commands to force local rectangular grids and compare answers with the default model. Every source must be at or below the target. Degrees 0 through 4 are accepted. Homology above degree 2 vanishes for the input class; higher chain degrees can occur in the resolution. Computation commands emit JSON and failures return a nonzero exit code with an explanation on standard error.

For each simplex the default construction computes a maximum-weight spanning tree on its minimal births. A zero coordinate-prefix defect gives an exact tree. If the defect is positive, three births use a filled triangle, and larger antichains use a grid on the two shortest coordinate lists. Incidence maps lift face inclusions; the two routes from a triangle to a vertex are connected by a chain homotopy. The total differential includes this correction. Its square and the augmentation chain identity are checked for every constructed model.

Queries compute cycle spaces over the binary field and compare them in a common target chain space modulo target boundaries. The independent verify command checks the mapping cone against the original simplicial chains. Different models, including models forced by --grid, can have different generator counts and identical homological answers. The program writes a finite free model. It does not compute a minimal presentation or a general indecomposable decomposition.

In dump, cell IDs, simplex indices and local indices are zero based. The boundary lists cell IDs with coefficient one. A non-null augmentation is an index in local_models whose simplex receives this generator with coefficient one. A rational grade is printed as a comma-separated string.

## Point clouds

Each input row for cloud contains five whitespace-separated values: point ID, three integer spatial coordinates, and one rational mark. Coordinates have absolute value at most one billion. Planar data may use zero for the third spatial coordinate. Distinct points may share a position. The square example uses all-zero marks.

The output is filtration text. Its three axes are squared Euclidean radius, allowed number of missing neighbors, and mark threshold. At a chosen mark threshold, count neighbors among the retained points without counting the point itself. With n points in the original file, a vertex must have at least n−1−s neighbors at the chosen radius. The complex contains edges and triangles of the clique complex on qualifying vertices. Births are generated at all distinct squared distances and mark thresholds, and reduced to minimal grades.

The option --max-radius2 N restricts construction to distance events of squared radius at most N. Above that cap, the exported filtration is the upward extension of its last computed slice. The exhaustive construction visits all mark and radius events and enumerates qualifying triples. It is intended for small samples.

## Comparing two lists of upset modules

~~~powershell
& $mp compare examples/upsets-left.txt examples/upsets-right.txt
& $mp compare examples/upsets-left.txt examples/upsets-right.txt --budget 0
~~~

The inputs explicitly give decompositions. Each row represents one summand, with any single-token label, a colon, and the triples generating its nonempty upset. Distinct rows remain distinct summands even when they have identical support. Row indices identify the summands in the result. A planar example embeds into three coordinates by using zero for the last coordinate.

The program computes all directed inclusion costs and finds the minimum complete matching cost by bipartite matching. At smaller critical thresholds it solves constrained invertibility over the binary field by splitting the mask digraph into strongly connected components, trying a mutual matching, and enumerating matrices on the sparser local mask when necessary. A feasible solution includes forward and backward matrices as sparse columns. The matching list maps left row indices to right row indices.

The example has exact interleaving distance 1 and bottleneck distance 3. The default budget allows one million candidate matrices over the whole comparison. With a smaller budget, unresolved feasibility produces status bounded, a null dI, and a proven closed interval [lower, upper]. The matrices then certify the upper endpoint. dB remains exact. For the example a zero budget returns [1,3]. Distances are strings containing an exact rational or infinity. These computations concern explicit upset sums; they do not return the interleaving distance between arbitrary homology modules produced by the filtration commands.

## Prescribed basis exchanges

~~~powershell
& $mp exchange examples/basis.matrix 1,2
~~~

Each row of the input is a row of an invertible square binary matrix A. Source columns are X in the coordinates of target basis Y. The prefix lists the original zero-based X indices that must be exchanged first, in any order. Use a hyphen for the empty prefix. The program computes the inverse, forms the exact-support digraph and factors the state search over strongly connected components. A state is admissible when its selected minor and its complementary minor are both nonsingular. Memoized search constructs local paths, combines their prefixes and suffixes, and checks the reported steps against both global minors.

Status realized includes pairs [x,y] in exchange order. Status impossible means the finite search ruled out the prescribed prefix. Status undecided means the candidate-state budget was exhausted. Set --budget N after the prefix to change the default of one million. The search and matrix comparison can both require exponential work.

## Reproducible checks

The test suite includes the triangle with a nonzero homotopy correction, the three-birth obstruction, a four-birth local grid, 80 independent components, exact rational thresholds, empty and malformed inputs, and the marked square. It compares the intersection queries on small generated filtrations against direct enumeration of simplicial cycle spaces. Upset distances are checked against an independent exhaustive binary-matrix oracle. The exchange sequence is checked by enumerating the spans of both minors at every step.

The CTest target `quotient_projection` checks canonical reduction modulo target boundaries, including a free leading row above two pivot rows.

## Architecture and mathematical references

Source comments link to the three English manuscripts and the [note on query indices](../english/04-query-index.md). The [implementation map](../MATHEMATICS.md) pairs source lines with individual formulas and proofs.

[ADR 0001](docs/adr/0001-homology-queries.md) records the query algorithm, the lifetime of face maps, its proof and the measured costs. The [individual measurements](docs/profile/timings.csv) compare both revisions on the same 15,724-cell filtration.
