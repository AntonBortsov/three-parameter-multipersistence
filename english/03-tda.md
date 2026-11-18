# Free Chain Models for Three-Parameter Filtrations and Intersections of Images

**Abstract.** We consider finite simplicial complexes of dimension at most two in which a simplex may appear at several incomparable grades of a three-parameter filtration. A maximum spanning tree criterion determines whether the support of a simplex has a tree resolution. The local resolutions assemble into a free chain complex naturally quasi-isomorphic to the original filtration. The complex gives both ranks of maps and intersections of images from several grades. Two explicit filtrations have identical one-parameter restrictions in first homology and are distinguished by an intersection of images.

## 1. Filtrations with several birth grades

<a id="tda-support"></a>

Let K be a finite simplicial complex of dimension at most two. Assign a finite antichain of rational triples to each nonempty simplex σ. Its support and the subcomplex at a parameter p are

$$
U_\sigma=\{p\in\mathbb R^3:\exists a\in G_\sigma,\ a\le p\},\qquad
K_p=\{\sigma:p\in U_\sigma\}.
$$

The order is coordinatewise. For every face τ of σ, the support of σ must be contained in that of τ. It suffices to check the generators and codimension-one faces. Simplices with empty support can be discarded. All coefficients are in the binary field.

Let C be the chain complex of the filtration. At p it takes the simplicial chains of the corresponding subcomplex. Its structure maps are inclusions. A principal free module born at a has one copy of the field at each p ≥ a, with identity structure maps. We seek a complex of finite direct sums of such modules, with a natural chain map to C inducing a homology isomorphism at every p. This model also represents all homology maps under changes of parameter.

## 2. A criterion for tree resolutions

Fix the nonempty antichain G of one simplex. A tree T on G defines a free complex. A local vertex has its birth grade, an edge has the coordinatewise maximum of its endpoint grades, and its boundary is their sum. Augmentation sends every vertex to the unit generator of the support module. At p the remaining forest is induced on the active set

$$
G_p=\{a\in G:a\le p\}.
$$

<a id="tda-active-tree"></a>

The augmented complex is exact at every grade precisely when each nonempty active set induces a connected subtree. A forest has zero first homology, and its reduced zeroth homology has dimension one less than the number of components whenever it is nonempty. This is the tree case of the cellular exactness criterion. [D. Bayer and B. Sturmfels](https://arxiv.org/abs/alg-geom/9711023).

<a id="tda-prefix-defect"></a>

For each coordinate and each distinct value it takes on G, form the nonempty set of vertices at or below that value in that coordinate. Write ℱ for this family of prefixes, allowing multiplicities. Set

$$
\begin{aligned}
B&=\sum_{S\in\mathcal F}(|S|-1),&
w_{ij}&=\sum_{S\in\mathcal F}\mathbf1_{\{i,j\}\subseteq S},\\
\Delta&=B-\max_{T\ {\rm tree}}\sum_{ij\in E(T)}w_{ij}.
\end{aligned}
$$

**Theorem 1.** An exact tree resolution on these births exists if and only if Δ = 0. When the defect vanishes, every maximum-weight spanning tree gives such a resolution.

**Proof.** Any single-coordinate prefix is realized at some p by making the other two coordinates sufficiently large. A nonempty joint sublevel set is the intersection of three prefixes. If each induces a connected subtree, the unique path between any two common vertices belongs to all three. Their intersection is therefore connected.

For every spanning tree,

$$
B-\sum_{ij\in E(T)}w_{ij}
=\sum_{S\in\mathcal F}\bigl(|S|-1-|E(T[S])|\bigr)
=\sum_{S\in\mathcal F}\bigl(\#\operatorname{comp}(T[S])-1\bigr).
$$

The right-hand side is a sum of nonnegative integers. Its minimum is zero exactly when all prefixes are connected simultaneously.

<a id="tda-weight-ranks"></a>

If one coordinate takes L distinct values and the endpoint ranks are i and j, it contributes L−max(i,j)+1 to the pair weight. Sorting each coordinate makes weight evaluation constant-time. For m births the dense version of Prim’s algorithm uses O(m²) comparisons and O(m) auxiliary memory. A single birth gives a one-vertex tree with zero defect.

**Exchanging spanning trees.** A tree is maximum-weight precisely when each non-tree edge weighs at most the lightest edge on the tree path between its endpoints. Necessity follows by exchanging an edge. For sufficiency, consider the graph of edges at or above each weight threshold. Its edge endpoints are connected by tree paths at the same threshold. The tree thus takes the largest possible number of edges at or above every threshold. An equal-weight exchange preserves optimality and, when Δ = 0, exactness.

At each weight level contract components of strictly heavier edges and choose a spanning tree of each remaining component at that level. This describes all maximum-weight spanning trees. Their choices are the bases of a direct sum of graphic matroids, with loops for excluded edges. Basis exchange thus applies directly to the choice of a resolution. Path lengths can differ among optimal trees.

## 3. Branching and an obstruction on three births

For m ≥ 1 define a center and three arms by

$$
o=(m,m,m),\qquad (a_{it})_j=
\begin{cases}m-t,&j=i,\\ m+t,&j\ne i,\end{cases}
\qquad 1\le i\le3,\quad 1\le t\le m .
$$

These 3m+1 points form an antichain. Join consecutive points on each arm and its first point to the center. Below m a coordinate prefix is a tail of the corresponding arm. From m onward it contains the center, that whole arm, and initial segments of the others. Every prefix is connected.

The sublevel set at the join of adjacent points on one arm consists of exactly that pair, as does the join of the center and its first neighbor. All listed edges are forced. The exact tree is unique and has a vertex of degree three. In a planar antichain, by comparison, increasing first coordinates correspond to decreasing second coordinates. The forced adjacent pairs form a path.

<a id="tda-three-births"></a>

The three coordinate unit vectors give a different case. The join of any two contains exactly these two. An exact tree would have to contain all three edges. In every spanning tree one of the three two-point prefixes is disconnected, giving Δ = 1. The filled local triangle is exact, since every nonempty slice is a full simplex on its active vertices. Its two-cell is born at (1,1,1) and kills the cycle that appears with the third edge.

## 4. The global differential

First suppose that each simplex σ has an exact tree. Its local vertices contribute free generators in the chain degree equal to the dimension of σ, and its local edges contribute in the next degree. Write ∂ for the internal boundary.

For each codimension-one face τ choose, for every birth of σ, a birth of τ no higher than it. Denote this map by f. Send each edge of the tree of σ to the unique path between the images of its endpoints in the tree of τ. Both images lie in the active subtree at the edge grade, so the whole path respects grades. Its boundary is the sum of the endpoint images, making f a local chain map.

A triangle σ and its vertex v have two intermediate edges. Write F₁ and F₂ for the corresponding composites. On a local vertex of σ let H be the path in the tree of v between its two images. Put H = 0 on local edges. Then

$$
\partial H+H\partial=F_1+F_2 .
$$

On local vertices this is the boundary formula for a path. On a local edge the difference of the two sides has zero boundary in the tree of v and is therefore zero.

<a id="tda-differential"></a>

**Theorem 2.** On the direct sum of the local models the operator

$$
D=\partial+\sum_{\sigma\gtrdot\tau}f_{\sigma\tau}
+\sum_{\dim\sigma=2,\ v\in\sigma}H_{\sigma v}
$$

squares to zero. The map α that sends each local vertex over σ to σ and each local edge to zero is a natural quasi-isomorphism to C.

**Proof.** Expand the square by the drop in dimension of the underlying simplex. Drop zero gives the square of the internal boundary. Drop one gives the chain-map equations for f. Drop two gives the two face composites and the two terms of the homotopy equation for H. Their sum vanishes in the binary field. There is no greater drop for simplices of dimension at most two. Augmentation commutes with boundaries and vanishes in positive local degrees.

At fixed p filter source and target by the dimension of the underlying simplex. In each successive quotient only internal differentials remain. Each active local model has one-dimensional homology in local degree zero only, and augmentation identifies this with the original summand. Inactive summands vanish. Induction along the long exact homology sequences proves the result for the entire complex. The images are chosen at generator birth grades, so the map is natural in p.

**A nonzero correction.** In triangle 012 give vertex 0 two births. The other vertices and edge 12 are born at zero. Assign

$$
a=(0,2,0),\qquad b=(2,0,1),\qquad
g_{01}=a,\quad g_{02}=b,\quad g_{012}=(2,2,2).
$$

The two composites from the triangle to vertex 0 reach different births. The correction H is the unique local edge between a and b. Omitting it leaves the sum of two distinct local generators in the square of the triangle boundary. At (2,2,1) the original complex has an unfilled loop. Its first homology class dies on passage to (2,2,2).

Let N count simplices, births and codimension-one incidences together. If all tree models exist, the number of free generators is linear in N. Scanning for admissible images, constructing the trees, and listing their paths take O(N²) comparisons and coefficient operations. There are linearly many path requests and each path has length bounded by N. The same quadratic bound holds for the number of nonzero coefficients written. This estimate is for model construction. Computing its homology has a separate cost.

## 5. A general grid model and mixed resolutions

<a id="tda-height"></a>

Choose two coordinates, say x and z, and sort their distinct values over the entire filtration. For each σ define the nonincreasing height function

$$
h_\sigma(i,j)=\min\{g_y:g\in G_\sigma,\ g_x\le x_i,\ g_z\le z_j\},
\qquad\min\varnothing=+\infty .
$$

<a id="tda-grid-grade"></a>

Take the rectangular grid on these coordinates, including its vertices, elementary edges and squares. A cell Q with lower corner (i,j) and upper corner (i+u,j+v) yields one free generator when its lower-corner height is finite. Its chain degree is the sum of the dimensions of σ and Q. Its parameter grade and differential are

$$
\begin{aligned}
\deg(\sigma,Q)&=(x_{i+u},h_\sigma(i,j),z_{j+v}),\\
D(\sigma,Q)&=\sum_{\tau\lessdot\sigma}(\tau,Q)
+\sum_{F\text{ a facet of }Q}(\sigma,F).
\end{aligned}
$$

The empty faces are omitted. Augmentation sends a generator with point cell Q to σ and all other generators to zero.

**Proposition 1.** These formulas define a free complex naturally quasi-isomorphic to C for every filtration of Section 1.

**Proof.** Taking a simplicial face can only lower the height. On passing to a cell facet, the upper corner does not increase and the lower-corner height does not increase. The two parts of the differential each square to zero and the mixed terms cancel in pairs. At fixed p the active grid vertices of one simplex form an upset in the rectangle truncated by x and z. A cell is present precisely when all its vertices are active. A nonempty realization contracts to the upper right corner by sliding each point horizontally to the right side and then vertically to the top. Monotonicity keeps both segments inside it. Nonemptiness is equivalent to σ being present. The filtration by simplex dimension from Theorem 2 completes the proof.

For S simplices and coordinate-list lengths L₁ and L₂ there are O(SL₁L₂) generators. Each column has at most three simplicial faces and four grid facets. Prefix minima give the height tables. Including the initial coordinate sorts gives the bound

$$
O(N\log N+SL_1L_2).
$$

Choosing the two axes with the smallest product of lengths gives the best of the three bounds. In the worst case it is cubic in the input size.

**Proposition 2.** Different simplices may use different free support resolutions. It suffices that the augmented local complex be exact at every active grade p and zero at every inactive grade.

<a id="tda-mixed-lifts"></a>

**Proof.** For a face map f choose each local degree-zero image with the required augmentation. For a generator u of local chain degree q, solve the following equations inductively.

$$
\partial f_q(u)=f_{q-1}(\partial u),\qquad
\partial H_q(u)=F_1(u)+F_2(u)+H_{q-1}(\partial u).
$$

For H at q = 0 the last term is zero. For the first equation at q ≥ 1 the right-hand side is a cycle, of augmentation zero when it is in degree zero. For the second, this follows from the homotopy equation already constructed in lower degrees and from equal augmentations of the two composites. Local exactness supplies fillings at the birth grade of u. Extend those choices on the free generators. The remainder of the proof of Theorem 2 applies.

<a id="tda-mixed-choice"></a>

In particular, choose a tree when the defect is zero, a filled triangle for three forced pairwise connections, and a local grid in the remaining cases. The maps f and H reconcile different local grids. The total generator count is the sum of the local counts. The quadratic tree bound does not include the cost of solving arbitrary local systems.

## 6. Ranks and simultaneous images

<a id="tda-rank"></a>

Let p ≤ q. In the common space of j-chains at q, let Z have as columns a cycle basis from p and B be the matrix of boundaries at q. Then

$$
\operatorname{rank}\bigl(H_j(K_p)\longrightarrow H_j(K_q)\bigr)
=\operatorname{rank}[B\mid Z]-\operatorname{rank}B .
$$

Indeed, the homology image is the image of the cycles modulo boundaries. The formula applies to both the simplicial chains and the free model. Equal grades give the Betti number.

<a id="tda-joint-image"></a>

For several grades below q one can compute the dimension of the intersection of the images. Let Zᵢ be their cycle spaces embedded in the chains at q, and let B be the boundary space at q. The inverse image of each homology image in the cycles at q is the sum of Zᵢ and B. Thus

$$
\dim\bigcap_{i=1}^{m}\operatorname{im}\bigl(H_j(K_{p_i})\to H_j(K_q)\bigr)
=\dim\bigcap_{i=1}^{m}(Z_i+B)-\dim B .
$$

<a id="tda-equal-images"></a>

All spaces are specified by columns in the same basis. For instance, the intersection of the column spaces of U and V consists of Ux over the solutions of Ux = Vy. No decomposition of the module into summands is required.

**An example with identical slices.** Take two triangular loops sharing vertex 0 and containing no two-simplices. The first loop L has edges 01, 12, 02 and the second loop T has edges 03, 34, 04. All vertices are born at zero. Define three pairwise incomparable grades and their common upper node by

$$
p_1=(0,1,1),\qquad p_2=(1,0,1),\qquad p_3=(1,1,0),\qquad q=(1,1,1).
$$

In filtration A each edge of L has three minimal births, one at each of p₁, p₂, p₃. Loop T appears at q. In filtration B each edge of L is born at p₁ and p₃, and each edge of T at p₂. Both loops are visible from q upward in each filtration.

The dimension of first homology records which branches have appeared. Outside the union of the upper orthants of p₁, p₂, p₃ it is zero. Inside this union but outside the upper orthant of q it is one. On the upper orthant of q it is two. Every point above two distinct pᵢ is above q, since their coordinatewise maximum is q. Thus a monotone chain can meet at most one of the three branches before reaching this orthant. All first-homology structure maps are injective and the restrictions to each chain are isomorphic in the two filtrations. Their barcodes agree on every monotone line and their ranks agree for all comparable pairs.

In the first filtration all three images at q are generated by the class of L. In the second the first and third images are generated by L and the second by T. Consequently

$$
\dim\bigcap_{i=1}^{3}\operatorname{im}\bigl(H_1(A_{p_i})\to H_1(A_q)\bigr)=1,\qquad
\dim\bigcap_{i=1}^{3}\operatorname{im}\bigl(H_1(B_{p_i})\to H_1(B_q)\bigr)=0 .
$$

Module isomorphisms preserve intersections of images, so the first homology modules of these filtrations are nonisomorphic. Even the entire family of one-parameter slice barcodes does not determine the dimension of their simultaneous image intersection. For comparison, RIVET provides access to barcodes of linear slices of bifiltrations and to two-parameter algebraic invariants. The example distinguishes invariants and does not require encoding a three-parameter input in RIVET. [RIVET documentation](https://rivet.readthedocs.io/en/latest/).

## 7. Verifying a model and stability of the input

<a id="tda-cone"></a>

For a finite filtration one may independently verify the quasi-isomorphism α using its cone. At each critical grade p and in each chain degree j check

$$
\operatorname{rank}\partial_j^{\rm cone}(p)+
\operatorname{rank}\partial_{j+1}^{\rm cone}(p)
=\dim\operatorname{Cone}(\alpha)_j(p).
$$

This is equivalent to acyclicity of the cone provided the differential squares to zero and α is a chain map. On each axis it suffices to use all grade coordinates of the input and the model, together with one value below the minimum. The resulting rectangular regions of constancy include threshold faces on the side where a generator is present. Naturality follows from using the same graded matrices on every region.

<a id="tda-cloud"></a>

For a stability example, fix a finite sample of marked points. At (r,s,t) retain points with mark at most t, count neighbors at distance at most r among the retained points, and require at least n−1−s neighbors. On qualifying vertices take the two-skeleton of the clique complex with edge length bounded by r. Here n is the full sample size and the integer s is the allowed number of missing neighbors.

If pairwise distances on the same sample change by at most η and marks by at most θ, then

$$
K(r,s,t)\subseteq K'(r+\eta,s,t+\theta),\qquad
K'(r,s,t)\subseteq K(r+\eta,s,t+\theta).
$$

Every original vertex, counted neighbor and edge survives the indicated shift. The inclusions are induced by the identity on the points. Their composites are structure inclusions, giving a homology interleaving with shift vector (η,0,θ). The point set and the normalization of the neighbor count are fixed.

## References

D. Bayer, B. Sturmfels. Cellular Resolutions of Monomial Modules. [arXiv:alg-geom/9711023](https://arxiv.org/abs/alg-geom/9711023).

S. Faridi, B. Hersey. Resolutions of Monomial Ideals of Projective Dimension 1. [arXiv:1703.04110](https://arxiv.org/abs/1703.04110).

U. Bauer, T. K. Dey, M. Kerber, F. Russold, M. Söls. Fast Free Resolutions of Bifiltered Chain Complexes. SoCG 2026. [DOI:10.4230/LIPIcs.SoCG.2026.10](https://doi.org/10.4230/LIPIcs.SoCG.2026.10).

RIVET. User Guide for Visualization and Computation with Two-Parameter Persistence Modules. [Documentation](https://rivet.readthedocs.io/en/latest/).
