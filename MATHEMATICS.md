# From C++ to the manuscripts

Each source link opens the comment beside the computation. The adjacent link opens the corresponding definition, formula or proof. The last section covers the three query strategies.

## A Coarse Theory of Matroid Basis Exchange

| C++ location | Mathematical reference |
|---|---|
| [block_search.hpp, line 8](multipersistence/src/block_search.hpp#L8) | [Exchange, Theorem 2, exact supports](english/01-exchanges.md#ex-product) |
| [exchange.hpp, line 38](multipersistence/src/exchange.hpp#L38) | [Exchange, sec. 3, complementary-minor identity](english/01-exchanges.md#ex-minors) |
| [exchange.hpp, line 56](multipersistence/src/exchange.hpp#L56) | [Exchange, sec. 1, flexible serial exchange](english/01-exchanges.md#ex-prefix) |
| [exchange.hpp, line 106](multipersistence/src/exchange.hpp#L106) | [Exchange, sec. 3, exact-support digraph](english/01-exchanges.md#ex-support-graph) |
| [exchange.hpp, line 121](multipersistence/src/exchange.hpp#L121) | [Exchange, sec. 3, Theorem 2](english/01-exchanges.md#ex-product) |
| [exchange.hpp, line 149](multipersistence/src/exchange.hpp#L149) | [Exchange, sec. 3, prefix assembly corollary](english/01-exchanges.md#ex-assembly) |

## Bjerkevik’s Conjecture on Approximating Interleavings by Matchings

| C++ location | Mathematical reference |
|---|---|
| [block_search.hpp, line 7](multipersistence/src/block_search.hpp#L7) | [Bjerkevik, Theorem 1, allowed masks](english/02-bjerkevik.md#bj-scc) |
| [distance.hpp, line 42](multipersistence/src/distance.hpp#L42) | [Bjerkevik, sec. 3, Lemma 1](english/02-bjerkevik.md#bj-inclusion) |
| [distance.hpp, line 106](multipersistence/src/distance.hpp#L106) | [Bjerkevik, sec. 3, finite-threshold enumeration](english/02-bjerkevik.md#bj-enumeration) |
| [distance.hpp, line 162](multipersistence/src/distance.hpp#L162) | [Bjerkevik, sec. 2, Theorem 1](english/02-bjerkevik.md#bj-scc) |
| [distance.hpp, line 243](multipersistence/src/distance.hpp#L243) | [Bjerkevik, sec. 3, candidate thresholds](english/02-bjerkevik.md#bj-enumeration) |
| [distance.hpp, line 254](multipersistence/src/distance.hpp#L254) | [Bjerkevik, sec. 3, Theorem 3, the two masks](english/02-bjerkevik.md#bj-masks) |
| [distance.hpp, line 268](multipersistence/src/distance.hpp#L268) | [Bjerkevik, sec. 3, bottleneck formula](english/02-bjerkevik.md#bj-bottleneck) |

## Free Chain Models for Three-Parameter Filtrations and Intersections of Images

| C++ location | Mathematical reference |
|---|---|
| [binary_columns.hpp, line 31](multipersistence/src/binary_columns.hpp#L31) | [TDA, sec. 6, inverse image of a homology image](english/03-tda.md#tda-joint-image) |
| [binary_columns.hpp, line 79](multipersistence/src/binary_columns.hpp#L79) | [TDA, sec. 6, column-space intersection](english/03-tda.md#tda-equal-images) |
| [chain_model.hpp, line 49](multipersistence/src/chain_model.hpp#L49) | [TDA, sec. 4, Theorem 2](english/03-tda.md#tda-differential) |
| [chain_model.hpp, line 67](multipersistence/src/chain_model.hpp#L67) | [TDA, sec. 5, Proposition 2, first lifting equation](english/03-tda.md#tda-mixed-lifts) |
| [chain_model.hpp, line 91](multipersistence/src/chain_model.hpp#L91) | [TDA, sec. 5, Proposition 2, second lifting equation](english/03-tda.md#tda-mixed-lifts) |
| [chain_model.hpp, line 111](multipersistence/src/chain_model.hpp#L111) | [TDA, sec. 4, Theorem 2](english/03-tda.md#tda-differential) |
| [chain_model.hpp, line 131](multipersistence/src/chain_model.hpp#L131) | [TDA, sec. 7, cone rank equation](english/03-tda.md#tda-cone) |
| [cloud.hpp, line 88](multipersistence/src/cloud.hpp#L88) | [TDA, sec. 7, marked clique filtration](english/03-tda.md#tda-cloud) |
| [core.hpp, line 197](multipersistence/src/core.hpp#L197) | [TDA, sec. 1, support of a simplex](english/03-tda.md#tda-support) |
| [intersection_index.hpp, line 91](multipersistence/src/intersection_index.hpp#L91) | [TDA, sec. 6, equal-image equation](english/03-tda.md#tda-equal-images) |
| [intersection_index.hpp, line 103](multipersistence/src/intersection_index.hpp#L103) | [TDA, sec. 4](english/03-tda.md#tda-differential) |
| [intersection_index.hpp, line 104](multipersistence/src/intersection_index.hpp#L104) | [TDA, sec. 5, Proposition 2 for mixed local resolutions](english/03-tda.md#tda-mixed-lifts) |
| [intersection_index.hpp, line 183](multipersistence/src/intersection_index.hpp#L183) | [TDA, sec. 6](english/03-tda.md#tda-joint-image) |
| [lazy_intersections.hpp, line 120](multipersistence/src/lazy_intersections.hpp#L120) | [TDA, sec. 6, simultaneous images in the quotient](english/03-tda.md#tda-joint-image) |
| [local_resolution.hpp, line 24](multipersistence/src/local_resolution.hpp#L24) | [TDA, sec. 2, active subtree criterion](english/03-tda.md#tda-active-tree) |
| [local_resolution.hpp, line 70](multipersistence/src/local_resolution.hpp#L70) | [TDA, sec. 2, Theorem 1 and defect identity](english/03-tda.md#tda-prefix-defect) |
| [local_resolution.hpp, line 119](multipersistence/src/local_resolution.hpp#L119) | [TDA, sec. 2, rank formula for the edge weight](english/03-tda.md#tda-weight-ranks) |
| [local_resolution.hpp, line 132](multipersistence/src/local_resolution.hpp#L132) | [TDA, sec. 3, three forced pairs](english/03-tda.md#tda-three-births) |
| [local_resolution.hpp, line 162](multipersistence/src/local_resolution.hpp#L162) | [TDA, sec. 5, height function](english/03-tda.md#tda-height) |
| [local_resolution.hpp, line 181](multipersistence/src/local_resolution.hpp#L181) | [TDA, sec. 5, degree of a grid cell](english/03-tda.md#tda-grid-grade) |
| [local_resolution.hpp, line 204](multipersistence/src/local_resolution.hpp#L204) | [TDA, sec. 5, choice of local model after Proposition 2](english/03-tda.md#tda-mixed-choice) |
| [model.hpp, line 25](multipersistence/src/model.hpp#L25) | [TDA, sec. 6, equal-grade rank](english/03-tda.md#tda-rank) |
| [model.hpp, line 39](multipersistence/src/model.hpp#L39) | [TDA, sec. 6, simultaneous-image formula](english/03-tda.md#tda-joint-image) |
| [model.hpp, line 67](multipersistence/src/model.hpp#L67) | [TDA, sec. 6, one-source formula](english/03-tda.md#tda-rank) |
| [triple_table.hpp, line 106](multipersistence/src/triple_table.hpp#L106) | [TDA, sec. 6, equal-image equation](english/03-tda.md#tda-equal-images) |
| [quotient.cpp, line 16](multipersistence/tests/quotient.cpp#L16) | [TDA, sec. 6, simultaneous-image formula](english/03-tda.md#tda-joint-image) |

## Homology coordinates for three query strategies

| C++ location | Mathematical reference |
|---|---|
| [binary_columns.hpp, line 32](multipersistence/src/binary_columns.hpp#L32) | [Grid note, (2), unique complement representative](english/04-query-index.md#cycle-labels) |
| [intersection_index.hpp, line 30](multipersistence/src/intersection_index.hpp#L30) | [Grid note, (1), preservation of active births](english/04-query-index.md#grid-floor) |
| [intersection_index.hpp, line 49](multipersistence/src/intersection_index.hpp#L49) | [Grid note, (4), composition of cover maps](english/04-query-index.md#cover-maps) |
| [intersection_index.hpp, line 138](multipersistence/src/intersection_index.hpp#L138) | [Grid note, (2), dependent rows as homology coordinates](english/04-query-index.md#cycle-labels) |
| [intersection_index.hpp, line 184](multipersistence/src/intersection_index.hpp#L184) | [Grid note, (2) and (4), reading a cover column](english/04-query-index.md#cover-maps) |
| [lazy_intersections.hpp, line 11](multipersistence/src/lazy_intersections.hpp#L11) | [Grid note, (5), minimal sources](english/04-query-index.md#minimal-sources) |
| [lazy_intersections.hpp, line 50](multipersistence/src/lazy_intersections.hpp#L50) | [Grid note, (3), projection by triangular row recurrence](english/04-query-index.md#projection) |
| [lazy_intersections.hpp, line 69](multipersistence/src/lazy_intersections.hpp#L69) | [Grid note, (4), source cycles in a fixed target basis](english/04-query-index.md#cover-maps) |
| [triple_table.hpp, line 22](multipersistence/src/triple_table.hpp#L22) | [Grid note, (6), rank within the upper orthant](english/04-query-index.md#table-offset) |
| [triple_table.hpp, line 36](multipersistence/src/triple_table.hpp#L36) | [Grid note, (5) and (6), unordered minimal-source keys](english/04-query-index.md#table-offset) |
| [triple_table.hpp, line 115](multipersistence/src/triple_table.hpp#L115) | [Grid note, (7), reuse of the pair intersection](english/04-query-index.md#pair-reuse) |
