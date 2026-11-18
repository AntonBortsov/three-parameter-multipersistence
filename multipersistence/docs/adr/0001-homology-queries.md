# ADR 0001 — Compute common images in the target quotient

**Accepted · 18 November 2026**

## Context

The program represents a multicritical filtration by a graded free chain model over the binary field. Different source grades can contribute cycles representing the same target class. The original implementation adjoined every target boundary to every source space, intersected the enlarged spaces and subtracted the boundary rank. The eight-query tissue workload had 510,791 free generators. The baseline CPU profile recorded 4,024,527 calls to the echelon insertion routine and 36,976,225 calls to sparse column addition. Copying the boundary basis and intersecting its copies made the common boundary space part of the workload for every source. [Free Chain Models, §6, simultaneous-image formula](../../../english/03-tda.md#tda-joint-image)

## Decision

Keep one echelon basis of target boundaries during a query. For several proper sources, reduce the source cycles to canonical representatives modulo this fixed basis, retain a basis for each image and intersect the resulting column spaces. Stream cycles to the consumer as soon as a source boundary column yields a dependence. All sources share the same target chain coordinates. [Query implementation](../../src/model.hpp) · [Sparse binary operations](../../src/binary_columns.hpp)

For a single proper source, extend the target boundary basis with source cycles and return the increase in rank. If all sources equal the target, compute the Betti number by rank-nullity without constructing a cycle basis. In a mixed query the target itself contributes the whole homology space. Validate every source before applying that simplification or returning an empty intersection. [Free Chain Models, §6, one-source and equal-grade cases](../../../english/03-tda.md#tda-rank)

Retain the same tree, triangle and grid resolutions and their common generator ordering. The assembler first counts the generators and reserves their final storage. Edge-to-vertex maps survive until incident triangles have been processed. Each triangle's face maps and filling homotopies have triangle-local lifetimes. Every later use follows one of the two composites through an intermediate edge. [Free Chain Models, §5, Proposition 2, lifting equations](../../../english/03-tda.md#tda-mixed-lifts) · [Assembly](../../src/chain_model.hpp)

## Correctness

Let B be the target boundary space. Echelon columns have distinct highest occupied rows. In descending row order, eliminate each pivot and retain each free coefficient. This defines a linear projection P onto the span of the nonpivot coordinate vectors. Every elimination changes a vector by an element of B. A nonzero vector of B has a pivot as its highest row after expressing it in the echelon basis, so the free-coordinate subspace intersects B trivially. Thus the kernel of P is exactly B.

For every source cycle space Z, the vectors in P(Z) are the unique free-coordinate representatives of the classes in (Z + B)/B. A representative belongs to every P(Z) precisely when its class belongs to every source image. Intersection and dimension can therefore be computed in the shared quotient coordinates. Stopping when an unoccupied leading row is reached suffices for an independence test. A canonical representative also requires clearing all lower pivots. The regression test includes a free leading row above two successive pivots and checks the kernel, idempotence and linearity on all 32 vectors of a five-dimensional example. [Free Chain Models, §6, common images](../../../english/03-tda.md#tda-joint-image) · [Projection regression](../../tests/quotient.cpp)

## Alternatives and consequences

The original ambient-space algorithm implements the same identity directly. The archived source remains the baseline for measuring this change. The one-source rank increment is cheaper than producing explicit quotient representatives, which explains the separate one-source path. A model-wide cache of bases keyed by target grade would benefit repeated targets at the cost of retaining bases across independent requests. This design gives query-local storage and requires no cache invalidation policy.

The free model, face lifts and augmentation remain available to the dump and verification commands. On the 2,048-cell patch the old and new programs emitted byte-identical complete model dumps for both the default construction and the forced grid. The existing comparison and exchange solvers now share graph and search-budget definitions without importing one another. Their mathematical entry points are documented beside the relevant loops. [Bjerkevik, §3, interleaving masks](../../../english/02-bjerkevik.md#bj-masks) · [Coarse Theory of Matroid Basis Exchange, §3, complementary minors](../../../english/01-exchanges.md#ex-minors)

## Measurements

Each row is the median of five executions of an optimized, uninstrumented binary. Each invocation starts a fresh process on the same logical CPU, reads the same prepared three-parameter input, constructs and checks the model, and executes the requested queries. Both revisions receive a warm-up. The order alternates within each workload. Stage medians are computed separately and can come from different executions. Peak RSS is reported in MiB.

| Workload | Revision | Build and check, s | Queries, s | Total, s | Peak RSS, MiB |
|---|---|---:|---:|---:|---:|
| One triple intersection | Before | 1.358 | 1.401 | 2.831 | 333.7 |
| One triple intersection | After | 0.825 | 0.814 | 1.631 | 262.1 |
| Eight answers at one target | Before | 1.375 | 8.716 | 10.386 | 333.8 |
| Eight answers at one target | After | 1.045 | 3.841 | 5.640 | 262.0 |

The eight answers, ordered as target dimension, three ranks, three pair intersections and the triple intersection, were 629, 235, 261, 476, 151, 191, 219 and 131 in every run. On the full second region the eight results agreed with the recorded independent multipers cone calculation. On the first region, all 61 answers for eleven lines of the two-parameter slice agreed with the recorded RIVET results.

The CPU profiles come from separate optimized builds with GNU gprof instrumentation. They cover loading, building, checking and querying in one process. Echelon insertion accounts for 29.1% of sampled self time in the baseline and 44.7% after the change. The emitted sparse-add function was called 36,976,225 and 5,483,513 times respectively. Inlining can change where samples and calls are attributed. The uninstrumented total for the eight-answer batch ranged from 8.88 to 12.29 seconds before and 4.42 to 5.97 seconds after. Results concern this machine and filtration. [Measurements](../profile/timings.csv) · [Function profile](../profile/cpu.csv) · [Environment and source hashes](../profile/run.json)

## Reproduction

From the multipersistence source directory on Linux, supply the companion benchmark archive from the tissue comparison. It contains the baseline headers, identical measurement driver, queries and prepared input. The script compiles both versions with GCC, uses GNU time for peak RSS and runs the optional CPU profiles separately. Select a fresh output directory for a new observation.

~~~sh
python3 tools/profile.py /path/to/tissue-benchmark-reproduction.zip profile-run --cpu-profile --repeats 5
~~~
