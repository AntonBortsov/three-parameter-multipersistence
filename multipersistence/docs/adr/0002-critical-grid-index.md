# ADR 0002 — Store homology maps on the three-dimensional critical grid

## Query contract

A file represents one finite three-parameter simplicial filtration, one homology degree and coefficients in the binary field. A query supplies a target and an arbitrary nonempty list of sources below it. The result is the dimension of the intersection of their images in target homology. In particular, the three sources and the target may all change between requests. [Simultaneous-image identity, §6](../../../english/03-tda.md#tda-joint-image)

## Construction

Collect the distinct birth coordinates on each axis. Floor a query to the greatest recorded coordinate on each axis. Below the minimum on any axis the subcomplex is empty. Coordinatewise flooring preserves the set of active simplices, even at thresholds with several simultaneous births. The Cartesian product of these coordinates is the critical grid. For each node store its homology dimension. For each pair of adjacent comparable nodes store the inclusion-induced map in their chosen homology bases.

Work on the original simplicial complex at each node. The natural augmentation from the free model induces a homology isomorphism compatible with inclusions. The direct solver and the index therefore compute the same functor. [Free Chain Models, Theorem 2](../../../english/03-tda.md#tda-differential)

At a grid node reduce incoming simplex boundaries in increasing simplex order. Each pivot eliminates its leading simplex modulo boundaries. The remaining simplex rows span a complement to the boundary space. Clearing every pivot gives a unique representative of each homology class in this complement. On the remaining active simplex columns reduce the outgoing differential with tags. Each dependency gives a cycle with one new dependent simplex and a combination of earlier independent simplices. Its coefficient at its own dependent simplex is one. Its coefficients at all other dependent simplices are zero. These cycles form a basis of the kernel of the restricted outgoing differential, which represents homology. Reading only their dependent-simplex coordinates gives the coefficients of any cycle after clearing target boundaries. [Rank calculation, §6](../../../english/03-tda.md#tda-rank)

For each incoming grid edge, include every source basis cycle as a simplicial chain at the target, clear the target boundaries and read those coefficients. This produces one column of the cover map. Representatives are discarded after their last immediate successor consumes them. The file retains the rational axes, dimensions and sparse cover maps. [Construction and query code](../../src/intersection_index.hpp)

## Query and storage cost

Compose cover maps along a monotone axis path from each source to the target. Adjacent coordinate steps commute by functoriality of inclusion, so any such path gives the same linear map. Remove dependent columns and intersect their spans in the common target coordinates. An empty source image gives zero; a source equal to the target contributes the full target. Published tissue files were checked by comparing both matrix products around every elementary square, column by column. [Column-space intersection, §6](../../../english/03-tda.md#tda-equal-images)

Grid construction visits each node once and performs a local boundary and cycle reduction there. Its cost grows with the product of the numbers of coordinate values. Edge storage grows with the number of source basis vectors and the nonzero coefficients of their images. The current format stores a variable-length size per column and the gaps between successive nonzero rows. A query pays for the cover-map compositions and column-space intersections along its own paths. Single-source ranks, duplicate sources and arbitrary representable rational coordinates use the same file. A dimension query returns an integer. Cycle representatives in the original tissue would require additional storage.

The format has a version tag and a checksum for accidental corruption. Publications of new files use a temporary file on the destination filesystem and a final rename. The benchmark records a separate SHA-256 digest of each input and its index. [File format](../../src/index_file.hpp)
