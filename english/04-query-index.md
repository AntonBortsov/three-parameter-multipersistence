# Homology coordinates for three query strategies

Anton Bortsov

The [simultaneous-image identity in §6](03-tda.md#tda-joint-image) specifies the scalar returned by the direct solver, the grid index, the target cache and the table. This note derives the data stored by the latter three. All chain groups below have coefficients in the binary field. Homology is ordinary, including in degree zero.

<a id="grid-floor"></a>

## 1. Flooring a rational grade

Let Aₖ be the sorted, distinct birth coordinates on axis k. Floor a query grade q to the greatest value at or below each coordinate. For every input birth b,

$$
 b\le q \quad\Longleftrightarrow\quad b\le \lfloor q\rfloor_A . \tag{1}
$$

Indeed each coordinate of b belongs to the corresponding list. If a floor does not exist on any one axis, no birth is active. By the [definition of simplex support](03-tda.md#tda-support), flooring preserves each simplex, its boundary and the inclusions between slices. Integer grid indices in the following sections start at zero.

<a id="cycle-labels"></a>

## 2. Reading homology coordinates

Fix a target and a degree. Echelon-reduce its incoming differential using the greatest nonzero row as pivot. Let B be its image and S the span of the active nonpivot simplex rows. Eliminating from largest to smallest clears every pivot and gives a unique representative in S of each coset modulo B. Boundaries are cycles, so the outgoing differential on this complement represents the map out of the quotient.

Reduce its outgoing columns in simplex order, keeping tags for independent columns. For each dependent column d, the tags give a cycle z with coefficient one at d and additional coefficients only at earlier independent columns. These cycles are independent by their distinct dependent rows and span the kernel by rank-nullity. Label each d by its cycle number. For a cycle c and its boundary-reduced representative N(c), the coefficient of the corresponding homology basis vector is therefore

$$
 [c]_d=N(c)_d. \tag{2}
$$

An independent row in S may occur in N(c), but reading the dependent rows suffices to recover all cycle coefficients. This realizes the quotient in the [single-source rank formula](03-tda.md#tda-rank).

<a id="projection"></a>

## 3. Projection at a fixed target

Extend the coordinate-reading map on cycles linearly to target chains by first taking their normal form and then reading the labels. Let Pᵢ be the resulting column for simplex row i and let uᵢ denote its unit label when it has one. If i is a boundary pivot, its echelon column is the unit row at i plus lower rows with indices in Rᵢ. Since this column vanishes in the quotient,

$$
 P_i =
 \begin{cases}
 \sum_{j\in R_i} P_j & \text{if } i \text{ is a boundary pivot},\\
 u_i & \text{if } i \text{ has a cycle label},\\
 0 & \text{otherwise}.
 \end{cases} \tag{3}
$$

Increasing row order suffices for the recurrence. The implementation stores successive supports in one array with offsets. Inactive rows receive empty columns and never occur in an included source cycle. Applying the map to a cycle sums its row columns by symmetric difference.

<a id="cover-maps"></a>

## 4. Inclusion and composition

For p ≤ q choose source cycles representing a basis of the source homology and collect them as columns of Cₚ. Their inclusions into target chain coordinates use the same simplex identifiers. With P denoting the map in (3), the matrix of the homology map and its composition rule are

$$
 M_{pq}=P_q C_p,\qquad M_{pq}=M_{rq}M_{pr}\quad(p\le r\le q). \tag{4}
$$

A change of representative adds a source boundary, whose inclusion is a target boundary and vanishes under P. Formula (2) gives precisely the coordinates of the surviving class. The second equality follows from composition of inclusions. The grid stores these matrices on covers and multiplies along a monotone path. The lazy strategy caches a basis of the image for each requested source and target pair. Both produce subspaces in the same target coordinates. Natural comparison with the free chain model follows from [Theorem 2](03-tda.md#tda-differential) and its [mixed-resolution form](03-tda.md#tda-mixed-lifts).

<a id="minimal-sources"></a>

## 5. Reducing sources to an antichain

For a nonempty finite set S of valid sources, write I(p,q) for the image at q. By (4), p ≤ r ≤ q implies that I(p,q) is contained in I(r,q). Consequently

$$
 \bigcap_{p\in S} I(p,q)=\bigcap_{p\in\min S} I(p,q). \tag{5}
$$

Repetitions can be discarded. A source below the grid has zero image. A sole source at the target gives its Betti number. Validate all source inequalities in the original rational coordinates before applying (1), since flooring can identify distinct grades.

<a id="table-offset"></a>

## 6. Addressing the table of one, two or three sources

Sort an antichain by lexicographic order of its grid indices. This is a linear extension of the coordinate order, so a later element can never be strictly below an earlier one. Testing whether each earlier element lies below each later one enumerates every antichain of the chosen sizes exactly once. For a key S, let l be the coordinatewise maximum of its members. Its legal discrete targets form exactly the box from l to the top of the grid. With axis sizes n₀, n₁, n₂ and the last coordinate varying fastest, its target offset is

$$
 \operatorname{offset}_S(q)
 =\big((q_0-l_0)(n_1-l_1)+(q_1-l_1)\big)(n_2-l_2)+(q_2-l_2).
 \tag{6}
$$

The box contains the product of its three side lengths. Prefix sums of the box sizes, computed separately for each arity, give the start of each block. The directory can be reconstructed from the axes. Only the dimensions are written as the answer payload.

<a id="pair-reuse"></a>

## 7. Reusing a pair while building triples

For a fixed target let U, V and W be the source images in (4). The table builder computes a basis D of the pair intersection using the [equal-image equation](03-tda.md#tda-equal-images), then reuses D for each possible third member following that pair.

$$
 \dim(U\cap V\cap W)=\dim\bigl(\operatorname{span}(D)\cap W\bigr). \tag{7}
$$

When D is empty every extension returns zero. Otherwise the pair's dimension alone is insufficient, since two subspaces of equal dimension may intersect W differently. The builder retains the basis just while processing that pair.
