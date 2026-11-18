# Three-parameter multipersistence

Anton Bortsov

Given several source grades below a common target, this C++17 program computes the dimension of the homology classes that can be represented at **every** source, in the coordinates of that target. A simplex can have several incomparable births. The implementation supports finite filtrations of vertices, edges and triangles, exact rational grades and coefficients in the field with two elements.

The example filtrations have identical first-homology barcodes on every monotone line. In one filtration the same loop comes from all three source grades. In the other, the three images lie on two distinct lines in target homology. Their triple intersections have dimensions **1** and **0**. [Construction and proof, §6](english/03-tda.md#tda-joint-image)

## Build and try the example

Use a C++17 compiler and CMake. Python 3.9 or later enables the mathematical and command-line regression suites. Run from the repository root.

~~~sh
cmake -S multipersistence -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
./build/multipersistence intersect multipersistence/examples/slices-a.txt 1 1,1,1 0,1,1 1,0,1 1,1,0
./build/multipersistence intersect multipersistence/examples/slices-b.txt 1 1,1,1 0,1,1 1,0,1 1,1,0
~~~

Visual Studio places the executable in `build/Release/multipersistence.exe` for a Release configuration. The last two commands return dimensions 1 and 0, respectively.

## Manuscripts

| Subject | English | Russian original |
|---|---|---|
| A coarse theory of matroid basis exchange | [Text](english/01-exchanges.md) · [PDF](english/01-exchanges.pdf) | [Text](manuscripts/01-exchanges.md) · [PDF](manuscripts/01-exchanges.pdf) |
| Bjerkevik’s conjecture on approximating interleavings by matchings | [Text](english/02-bjerkevik.md) · [PDF](english/02-bjerkevik.pdf) | [Text](manuscripts/02-bjerkevik.md) · [PDF](manuscripts/02-bjerkevik.pdf) |
| Free chain models for three-parameter filtrations and intersections of images | [Text](english/03-tda.md) · [PDF](english/03-tda.pdf) | [Text](manuscripts/03-tda.md) · [PDF](manuscripts/03-tda.pdf) |

The exchange command searches for serial exchanges of a represented pair of bases with a prescribed prefix. The interleaving command compares explicit finite direct sums of upset modules. Bjerkevik’s uniform-constant conjecture remains open.

## Implementation and measurements

- [Code-to-manuscript map](MATHEMATICS.md) pairs source lines with derivations across all three manuscripts and the [query index note](english/04-query-index.md).
- [Command reference](multipersistence/README.md) covers input formats, query modes and limits.
- [Direct query design](multipersistence/docs/adr/0001-homology-queries.md) and [critical-grid design](multipersistence/docs/adr/0002-critical-grid-index.md) describe the representation and its costs.
- [Tissue benchmarks](benchmarks/README.md) record the cold and resident-model measurements, the comparison with RIVET and multipers, and the cost of the three query strategies.
