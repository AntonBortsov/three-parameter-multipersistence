# Tissue benchmark measurements

Two tissue regions contain 15,724 and 21,124 cells. In the original three-source H₁ query, the intersection dimensions were 131 and 223. The source tables and annotations were published with the histology study by Vipond et al. [Study](https://pmc.ncbi.nlm.nih.gov/articles/PMC8522280/) · [Data revision](https://github.com/MultiparameterTDAHistology/SpatialPatterningOfImmuneCells/tree/b86114a135b3e45dac37edf7219b0ebad9517163)

| Experiment | Observations | Interpretation |
|---|---|---|
| C++ before and after refactoring, RIVET, multipers | [Individual trials](comparison/trials.csv) · [Summaries](comparison/summary.csv) · [Reference answers](comparison/expected.json) · [Environment](comparison/environment.json) | Both C++ revisions used the same release optimization flags. Cold runs include setup. Resident runs reuse a model. RIVET was measured on planar queries. The three-parameter multipers comparison uses a mapping-cone adapter. |
| Direct queries and the grid index | [Build and query times](intersection-index/trials.csv) · [Per-query costs and answers](intersection-index/queries.csv) · [Summaries](intersection-index/summary.csv) · [Commutativity checks](intersection-index/audit.json) · [Environment](intersection-index/environment.json) | Saved-index construction, opening, peak memory, and cumulative cost for 64 distinct queries in each of five orderings. |
| Lazy target cache, grid index and table of triples | [Build time, memory and file size](cache-benchmark/builds.csv) · [Timings and memory](cache-benchmark/measurements.csv) · [Break-even observations](cache-benchmark/break-even.csv) · [Environment](cache-benchmark/environment.json) | Fixed-target and varying-target workloads are measured separately. A first lead may be temporary. The sustained crossover is relative to the observed query order and horizon. |

The environment files identify the measured toolchain, inputs and binary or source revisions. The tables preserve the historical runs. Memory units follow the column headers. Model-ready RSS, peak RSS and serialized bytes describe different costs.
