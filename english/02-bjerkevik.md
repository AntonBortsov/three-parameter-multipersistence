# Bjerkevik’s Conjecture on Approximating Interleavings by Matchings

**Abstract.** Bjerkevik’s Conjecture 6.10 can be stated in terms of directed paths between the supports of mutually inverse matrices. We study decomposition into strongly connected components, reduction of the Hall deficiency as the radius increases, and an exact interleaving criterion for finite direct sums of upset modules. We give an example with distances one and three, a bound for planar two-generator summands, and a counterexample to induction by eliminating a mutual pair. The existence of a uniform constant remains open.

## 1. Constrained invertibility and the radius

Let k be a field, and let X and Y have n ≥ 1 elements each. Masks P and Q specify the allowed entries of two matrices. The first has rows indexed by Y and columns by X; the second has the indices reversed. The constrained invertibility problem is feasible if matrices over k satisfy

$$
\operatorname{supp} A\subseteq P,\qquad
\operatorname{supp} B\subseteq Q,\qquad BA=I_X,\qquad AB=I_Y .
$$

An allowed entry may take any value, including zero. A simple solution is a pair of mutually inverse permutation matrices. Draw arcs from X to Y on P and from Y to X on Q. For odd c ≥ 1 define the undirected bipartite graph

$$
H_c=\bigl\{\{x,y\}:d(x,y)\le c,\ d(y,x)\le c,\quad x\in X,\ y\in Y\bigr\}.
$$

Here d is directed shortest-path distance, infinite if the target is unreachable. Replacing each mask by the positions reachable in its direction by paths of length at most c gives the c-weakening. A simple solution of the weakening is equivalent to a perfect matching in the mutual-path graph at threshold c.

**Bjerkevik’s conjecture (6.10).** For fixed k there is an odd c, independent of n, such that every feasible constrained invertibility problem over k has a simple solution after c-weakening. Uniformity over fields would be a further strengthening. [H. B. Bjerkevik, Section 6.2](https://arxiv.org/abs/2305.15550).

For a given field it suffices to use the exact supports of an arbitrary invertible matrix and its inverse. Every feasible pair of masks contains such supports, and adding arcs can only shorten paths. Write Q₃ for the assertion that c = 3 always suffices. Section 4 gives a pair for which threshold one fails.

## 2. Components and Hall deficiency

<a id="bj-scc"></a>

**Theorem 1.** A constrained invertibility problem is feasible exactly when every strongly connected component of its mask digraph has equally many vertices in both parts and the diagonal subproblem on each component is feasible over k. For a feasible instance, the least perfect-matching radius is the maximum of the local radii. If b is the largest number of vertices of one color in a component, radius 2b−1 suffices.

**Proof.** Choose a nonzero permutation term in each determinant. The arcs of the resulting permutations cover all vertices by directed alternating cycles. Each cycle is contained in one component, proving balance. In reverse topological order on the components both matrices are block upper triangular. The diagonal blocks of BA = I are products of corresponding blocks. Conversely, take the direct sum of local solutions and set all other entries to zero.

Vertices at finite mutual distance lie in one component. Every shortest path between them stays in it. In a component on 2b vertices its length is at most 2b−1. At that threshold the local matching graph is complete. The global matching combines independent local matchings.

Bounding b gives a uniform constant for the resulting class of feasible instances. Another sufficient condition allows large components. For exact supports define the deficiency and matrix

$$
\delta_c=n-\nu(H_c),\qquad W_{yx}=A_{yx}B_{xy},
$$

where ν denotes maximum matching size. Every row and column of W sums to one. The deficiency form of Hall’s theorem expresses the deficiency as the maximum excess in size of two sets with no edges between them.

$$
\delta_c=\max\{|S|+|J|-n:S\subseteq X,\ J\subseteq Y,\ E_{H_c}(S,J)=\varnothing\}.
$$

**Theorem 2.** If the integer deficiency at level c has nonzero image in k, then

$$
\delta_{2c+1}\le\delta_c-1 .
$$

In particular every pair with initial deficiency one satisfies Q₃. In characteristic zero, a sufficient radius depends only on the initial deficiency, namely

$$
c=2^{\delta_1+1}-1 .
$$

**Proof.** Deficiency is nonincreasing. Suppose it retains a positive value δ on passing from c to 2c+1. Choose sets S and J attaining the maximum at the new threshold. They also attain it at the old threshold. Hence the complement of J is precisely the old neighborhood of S, since otherwise an extra vertex could be added to J. The analogous statement holds for the complement of S.

The rectangle of W on rows J and columns S vanishes, since a mutual arc would have radius one. Subtract the sums over those full rows and columns from the total to obtain

$$
\sum_{y\notin J,\ x\notin S}W_{yx}=n-|J|-|S|=-\delta .
$$

There exist t outside J and z outside S with nonzero product. They are connected both ways in one step. Vertex t has a radius-c neighbor in S and z has one in J. Concatenating the paths both ways produces an edge from S to J of radius at most 2c+1, a contradiction. In characteristic zero the estimate can be iterated from c = 1 until the deficiency vanishes.

In positive characteristic a nonzero integer deficiency may vanish in k. The rectangle sum remains valid but no longer forces a bridging pair.

**Refinement in characteristic zero.** As long as deficiency is positive, increasing the radius by two decreases it by at least one. Consequently a sufficient radius is

$$
c=2\delta_1+1 .
$$

Suppose instead that equal positive deficiencies occur at c and c+2. A maximizing pair S,J at c+2 also maximizes at c. Write T for the complement of J and Z for the complement of S. Every vertex of T is within mutual distance c of S, and every vertex of Z is within mutual distance c of J. Let T₁ consist of vertices of T having a mutual arc to S and T₂ be the remainder. As in Theorem 2, the sum on T by Z is −δ. For rows in T₂ every nonzero entry is in Z, so each such row contributes one. Hence

$$
\sum_{t\in T_1,\ z\in Z}W_{tz}=-\delta-|T_2|\ne0 .
$$

A nonzero term gives a mutual arc from T₁ to Z. Combining it with a mutual arc from S and paths of length at most c to J gives an edge between S and J at level c+2. This contradicts the choice of the sets. Iteration from level one gives the formula. In positive characteristic its right-hand-side sum can vanish in the field.

## 3. Interleavings of upset modules

Use the coordinatewise order. A finite nonempty antichain G generates the upset U of all points lying above some member of G. Its indicator module takes value k on U and zero elsewhere, with identity maps inside U. The ε-shift at p is the original value at p+ε1, where 1 is the all-ones vector. Two morphisms to the opposite ε-shifts form an interleaving if their composites are the structure morphisms for a shift by 2ε.

<a id="bj-inclusion"></a>

The directed inclusion threshold and the cost of matching two summands are

$$
\begin{aligned}
h(U,V)&=\max_{u\in G_U}\min_{v\in G_V}
\max\{0,v_1-u_1,\ldots,v_d-u_d\},\\
w(U,V)&=\max\{h(U,V),h(V,U)\}.
\end{aligned}
$$

**Lemma 1.** The morphism space from the indicator of U to the ε-shift of the indicator of V has dimension one when U+ε1 is contained in V and dimension zero otherwise. The least ε giving this inclusion is h.

**Proof.** Any two points of U have a common upper bound. Naturality therefore specifies one scalar wherever source and target are simultaneously nonzero. If the target vanishes at a point of U, the arrow from that point to a sufficiently high common upper bound forces the scalar to vanish. When inclusion holds every scalar is allowed. Checking the generators of U gives the formula.

<a id="bj-masks"></a>

**Theorem 3.** An ε-interleaving of two finite direct sums of these modules is equivalent to feasibility for the masks

$$
P_{ji}=[h(U_i,V_j)\le\varepsilon],\qquad
Q_{ij}=[h(V_j,U_i)\le\varepsilon].
$$

**Proof.** At a sufficiently high common point both structure maps are identities, and the composites are products of the scalar matrices. The matrices must be mutually inverse and respect the masks by Lemma 1. Conversely, permitted matrices give natural morphisms. Their composites equal the structure shifts everywhere, since each nonzero target value embeds into its value at a common upper point.

<a id="bj-bottleneck"></a>

For two lists of the same positive cardinality, the interleaving and ordinary bottleneck distances are

$$
\begin{aligned}
d_I(M,N)&=\inf\{\varepsilon\ge 0:M,N\text{ admit an }\varepsilon\text{-interleaving}\},\\
d_B(M,N)&=\min_{\pi}\max_i w(U_i,V_{\pi(i)}).
\end{aligned}
$$

A nonempty upset has infinite distance to zero, so every finite-cost ordinary matching is a bijection. Unequal cardinalities give infinite distance for both measures. Two empty lists have distance zero.

<a id="bj-enumeration"></a>

For equal nonempty lists the minimum interleaving scale lies in the finite set consisting of zero and the thresholds h. Feasibility is constant between consecutive thresholds, with new entries allowed at the threshold itself. Over a field with q elements one can decide a fixed threshold by enumerating assignments to the smaller of the two local masks and testing invertibility and the inverse mask. Given the masks, the number of field operations is bounded by

$$
O\!\left(n^2+\sum_C q^{\min(m_C,m'_C)}b_C^3\right),
$$

where b is block width and m and m′ count permitted entries. An unbalanced component can be rejected immediately. Over an infinite field, enumeration of values in even one finite mask need not terminate.

**Corollary.** If the maximum width at the smallest feasible threshold is b, then

$$
d_I\le d_B\le(2b-1)d_I .
$$

The first bound follows by taking the direct sum of matched interleavings. For the second apply Theorem 1. A path of c inclusions at scale ε gives an inclusion at cε. For widths at most two the distances coincide. In a nonsingular two-by-two matrix the two entries of a nonzero permutation term both have nonzero cofactors. Thus the inverse permutation is allowed at the original threshold. The width-one case is immediate.

## 4. An explicit pair of planar modules

Over the binary field take the matrices

$$
A=\begin{pmatrix}1&1&0\\1&0&1\\1&1&1\end{pmatrix},\qquad
B=\begin{pmatrix}1&1&1\\0&1&1\\1&0&1\end{pmatrix}.
$$

They are mutually inverse. In the mutual-arc graph the second and third X-vertices both have only the third Y-vertex as a neighbor, violating Hall’s condition at radius one. At radius three the identity permutation works. The first and third pairs have mutual arcs. The missing path from the second X to the second Y goes through the first Y and the first X. The reverse direction has a direct arc.

Number the six vertices from zero to five. To each vertex v assign the upset with generating antichain

$$
G_v=\{(20t,-20t+d(t,v)):0\le t<6\},\qquad U_v=\uparrow G_v .
$$

The digraph is strongly connected, so all distances are at most five. Up to scale five a generator indexed by t can be covered only by the generator with the same index. A larger index fails in the first coordinate and a smaller one in the second. Thus for any u,v,

$$
h(U_u,U_v)=\max_t\bigl(d(t,v)-d(t,u)\bigr)=d(u,v).
$$

The triangle inequality gives the upper bound and t = u attains it. At threshold one A and B give an interleaving. Below one there is no allowed entry between opposite parts. A matching requires threshold three because directed paths between the parts have odd length. Consequently

$$
d_I\!\left(\bigoplus_{x\in X}k_{U_x},\bigoplus_{y\in Y}k_{U_y}\right)=1,\qquad
d_B\!\left(\bigoplus_{x\in X}k_{U_x},\bigoplus_{y\in Y}k_{U_y}\right)=3 .
$$

## 5. The principal envelope and the rectangle quotient

For an upset U write a for the coordinatewise minima of its generators. The principal module Fₐ on the upper orthant at a contains the indicator of U. Define the quotient Q and deletion cost relative to the principal modules by

$$
0\longrightarrow k_U\longrightarrow F_a\longrightarrow Q_U\longrightarrow0,\qquad
\eta(U)=\inf_b d_I(k_U,F_b)=\frac12\min_{g\in G_U}\|g-a\|_\infty .
$$

Indeed, mutual inclusions at scale ε with a principal module at b imply that b is at most a+ε1 and that b+ε1 belongs to U. Thus a+2ε1 must lie in U. This condition suffices on choosing b = a+ε1. The same η is the distance from Q to zero. The quotient is generated at a, and its shift morphism vanishes when the image of this generator reaches U.

**Lemma 2.** For two quotients,

$$
d_I(Q_U,Q_V)=
\min\{w(U,V),\max(\eta(U),\eta(V))\}.
$$

**Proof.** A permitted shifted inclusion extends by its scalar to the principal envelopes and induces a map of quotients. Hence an interleaving of the original modules induces one on the quotients. At a scale where both quotient structure shifts vanish the zero maps suffice. These give the two upper bounds.

Below the larger deletion cost at least one structure shift is nonzero. Both interleaving morphisms on the quotients must be nonzero. In particular the bottom generator of the first quotient has nonzero image in the second. Every grade in U kills that generator and must therefore lie in V after the shift. The other morphism gives the reverse inclusion and hence the lower bound.

Define the relative bottleneck distance by minimizing over partial bijections, charging w for a pair and η for an unmatched summand. By Lemma 2 it equals the ordinary bottleneck distance on the lists of nonzero quotients. A quotient pair whose cost equals the larger deletion cost can be split and both summands deleted. Every remaining pair has its allowed original cost.

**Two-generator corollary.** Suppose every summand of two planar modules has at most two minimal generators. An ε-interleaving yields a partial 3ε-matching covering every summand on either side whose deletion cost exceeds 3ε. If all deletion costs on one side exceed 3ε, the matching is complete.

A principal summand has zero quotient. For a summand with two corners, where aₓ < rₓ and aᵧ < rᵧ, we obtain

$$
U=\uparrow\{(a_x,r_y),(r_x,a_y)\},\qquad
Q_U=k_{[a_x,r_x)\times[a_y,r_y)},\qquad
\eta(U)=\tfrac12\min\{r_x-a_x,r_y-a_y\}.
$$

Morphism matrices extend to direct sums of envelopes and descend to direct sums of quotients, preserving composition. Thus the rectangle quotient sums are ε-interleaved. Planar rectangle stability gives a 3ε-matching, which Lemma 2 converts into the asserted partial matching. The numbers of summands agree by the ranks sufficiently far above all generators. Without the condition on η the conclusion concerns the relative distance. [Bjerkevik’s stability theorem](https://arxiv.org/abs/1609.02086).

## 6. Reconstructing a complete matching from envelopes and quotients

Suppose each list has n ≥ 1 summands. In this section h denotes the bottleneck distance between the complete lists of principal envelopes, and q the bottleneck distance between the lists of nonzero quotients. The former matches the envelope vertices in the maximum norm.

**Theorem 4.** In any parameter dimension,

$$
d_B(M,N)\le nh+(n+1)q .
$$

**Proof.** For one pair with envelope vertices a and b,

$$
w(U,V)\le\|a-b\|_\infty+2\max\{\eta(U),\eta(V)\}.
$$

Indeed, V has a generator no higher than b+2η(V)1. Translating each point of U by the right-hand side puts it above that generator. Interchange U and V for the reverse inclusion.

Choose a complete envelope matching of cost h and a relative matching of the original summands of cost q, which exists by Lemma 2. Their union is a bipartite multigraph consisting of alternating paths and cycles. Retain all relative-matching edges. The endpoints of each path lie on opposite sides, are relatively unmatched and have deletion costs at most q. A path with r relative edges has r+1 envelope edges. Its endpoint anchors differ by at most (r+1)h+rq, since the distance between coordinatewise minima is at most the cost of matching the upsets. Its endpoints can therefore be matched at cost at most (r+1)h+(r+2)q. As r ≤ n−1, adding all endpoint pairs completes the required matching.

**Corollary.** For planar summands with at most two minimal generators, set

$$
D(M,N)=\max\{h,q/3\}.
$$

For lists of fixed cardinality we have

$$
D(M,N)\le d_I(M,N)\le d_B(M,N)\le(4n+3)D(M,N).
$$

The lower bound uses the interleaving–bottleneck isometry for planar free modules and rectangle stability for the quotients. Passing to either envelopes or quotients preserves interleavings. The remaining bounds follow from Theorem 4. In particular, envelopes and quotients together determine the isomorphism class in this family. [Bjerkevik’s free and rectangle module theorems](https://arxiv.org/abs/1609.02086).

**Sharpness.** For any n choose h = 1 and q = 3, and set C = 4n+3, L = C−6 and H = 10(C+5). Consider the following planar upsets.

$$
\begin{aligned}
W_t&=\uparrow\{(t,t+H),(t+H,t)\},\\
U_0&=\uparrow\{(0,6),(H,0)\},&
U_i&=W_{4i}\quad(1\le i<n),\\
V_j&=W_{4j+1}\quad(0\le j<n-1),&
V_{n-1}&=\uparrow\{(L,L+H),(L+6,L)\}.
\end{aligned}
$$

Envelope vertices at the same index differ by (1,1), and the last target vertex is at least one away from every source vertex. The envelope distance is exactly one. The square quotients of the interior summands match at cost three after shifting the index by one. Each end quotient has deletion cost three. Thus D ≤ 1, and the envelope lower bound gives equality.

At (0,6) the source contains a nonzero element surviving all positive shifts. The first nonzero target value along the diagonal ray from this point occurs at scale C, at generator (L+6,L). The remaining generators require at least this shift by the choice of H. At a smaller scale the interleaving composite would factor a nonzero structure morphism through zero. Thus the two original distances both equal C, and the factor 4n+3 is sharp. This example measures the loss in reconstruction from the envelopes and quotients.

## 7. An obstruction to eliminating a mutual pair

Consider an auxiliary model. Matrix X has rows T and columns S, and Y has the opposite indexing. Their products G = YX and H = XY are only required to have unit diagonals. Impose strong orientation in both directions by

$$
G_{ij}Y_{jt}X_{ti}=0\quad(i\ne j,\ t\in T),\qquad
H_{ab}X_{bs}Y_{sa}=0\quad(a\ne b,\ s\in S).
$$

A mutual index pair s,t has both entries nonzero. Delete its indices, using the Schur complement in X and the ordinary submatrix in Y. Call the pivot admissible for induction if the reduced matrices satisfy the same conditions.

**Theorem 5.** Over every field, for each odd n ≥ 5 there is a square model of order n in which no mutual pair is admissible for induction.

**Proof.** Write n = 2m+3 and let R be the cyclic shift taking a to a+1. Set

$$
X=I+R,\qquad Y=I+R^m,\qquad G=H=(I+R)(I+R^m).
$$

A two-step path advances by 0, 1, m or m+1. Two positive advances sum to less than n, so no such path has a reverse two-step path. The compositions have unit diagonals. Only diagonal pairs are mutual, and cyclic shift makes them equivalent.

Eliminate the zero pair. The Schur complement in X creates an arc from −1 to 1 with coefficient −1. There is now a unique two-step path from −1 to m+1 through 1, since its advance m+2 exceeds all previous advances. The reverse path from m+1 through m+2 to −1 survives. Thus

$$
G'_{m+1,-1}\,Y'_{-1,m+2}\,X'_{m+2,m+1}=-1\ne0 .
$$

Translation of the indices gives a violation after every diagonal pivot. For n = 5 take X = Y = I+R. In characteristic two the resulting forbidden product is one.

Even separate invertibility of the two matrices does not guarantee an admissible pivot. For n = 10 over the binary field take X = Y = I+R+R². The polynomial 1+z+z² is coprime to z¹⁰−1, so both matrices are invertible. Before elimination the two-step advances range from zero to four. After the zero pivot a path of coefficient one from 8 through 2 to 4 appears, and the reverse path via 6 survives. For a genuine inverse pair, the remaining submatrix of the inverse is the inverse of the Schur complement. The counterexample concerns the weaker strong-orientation conditions. The uniform radius question in Section 1 remains open.

**Theorem 6.** If a rectangular model satisfying the conditions of this section exists with unequal cardinalities of S and T, then such a model exists with the same sign of the size difference and with no admissible mutual pivot.

**Proof.** For the shift on five indices put L = I+R. Tensor both original matrices with L. The two products factor and have unit diagonals. Each forbidden coefficient also factors. Distinct original indices give a zero factor by the original assumptions. If only the five-cycle indices differ, the five-index model from Theorem 5 gives zero. Both side sizes are multiplied by five.

A mutual pair must have the same cyclic index on each side. Translate it to zero and call its original indices s,t. Use tildes for the enlarged matrices. Eliminate k = (s,0) and l = (t,0), and put i = (s,2), j = (s,4), v = (t,3). Expanding the reduced product gives

$$
G'_{ij}=\widetilde G_{ij}
-\widetilde G_{ik}\frac{\widetilde X_{lj}}{\widetilde X_{lk}} .
$$

The first term vanishes since 4 cannot advance to 2 in two steps on the five-cycle. The coefficient at i,k equals one by the original diagonal and the unique path from 0 to 2. The fraction has identical numerator and denominator. Thus the entry is −1. The entry of Y′ at j,v is the original Y-entry at s,t. The entry of X′ at v,i is the original X-entry at t,s because there is no single step from cyclic index 2 to 0 and the Schur correction vanishes. Both are nonzero. Thus every mutual pivot gives the forbidden nonzero product

$$
G'_{ij}Y'_{jv}X'_{vi}\ne0 .
$$

Existence of an unbalanced model in the hypothesis is a separate question. Restricting the induction to such models does not in itself provide a valid pivot.

## 8. Local models of arbitrary depth

Another local problem requires a unit diagonal for only one product. It arises directly from a violation of Hall’s condition. In the graph at threshold c, choose an inclusion-minimal deficient set S. Its neighborhood T has one fewer vertex than S because deleting any vertex of S restores Hall’s inequality for the remaining set. Put

$$
X=B[S,T],\qquad Y=A[T,S],\qquad C=XY .
$$

Matrix C has unit diagonal. Any contribution from an external index to such an entry would give mutual arcs between S and the complement of T. Introduce a local arc from a to b whenever two steps through T are available on the supports of Y and X. If the off-diagonal entry of C in row i, column j is nonzero, the identity BA = I requires cancellation from an index z outside T. There is a two-step path from j through z to i.

If a return from i to j used at most c−1 local arcs, there would be a closed walk from z through S of length at most 2c. Every odd position from z along this walk is an S-vertex. For a walk of length L ≤ c take position one. For L > c take position L−c, which is odd and at most c. Either choice gives a vertex of S at mutual distance at most c from z, contrary to its choice. Thus every nonzero off-diagonal composition forbids a short local return.

**Example.** For any k ≥ 1 and any field there are matrices of sizes n by n−1 and n−1 by n, with n = 2ᵏ, in which every such return has length exactly k. On vertices cyclically numbered from 0 to n−1 take the Fenwick tree. The root has all k bits set. The parent of any other vertex is obtained by setting its least significant zero bit. Form X from oriented edge boundaries and let column a of Y record the signed tree path from a to a+1 modulo n. Choose compatible signs so that

$$
XYe_a=e_a-e_{a+1}.
$$

The local arcs out of a then reach exactly the vertices on the tree path from a to a+1. Each column has one nonzero off-diagonal composition. The required length is the distance from a+1 back to a in this local digraph.

**Proof of the distance formula.** Write a nonroot vertex as a binary word with prefix P, one zero and j trailing ones. Apart from a loop, its outgoing arcs replace the suffix according to

$$
P\,0\,1^j\ \longrightarrow\ P\,1\,0^{j-i}1^i,\qquad 0\le i\le j.
$$

Indeed, the common ancestor of a and a+1 is obtained by filling the lowest j bits with ones after incrementing a. The path consists of the listed suffixes and a itself. From the root one can move to any value 2ᵐ−1 with 0 ≤ m ≤ k. A nonloop arc from a nonroot vertex strictly increases the integer and increases its population count by at most one.

Let π be the number of ones in P. Reaching the root from a+1 takes at least k−π−1 arcs, and following the parent pointers attains this. Any return to the smaller vertex a passes through the root. After leaving it the path starts at a string consisting of trailing ones, at most a. Exactly π further nonroot arcs are needed to reach a. For the upper bound, set the required ones of P from high to low. Start with all bits below the highest required position set. After setting each required bit leave trailing ones up to the next required position. The final step leaves exactly j trailing ones. If P contains no ones, choose a at the root exit.

For the lower bound let h be the highest set bit in P. The starting value is below a and does not contain h. Along an increasing path to a, bit h must first be set from 2ʰ−1. All lower bits have to be one and higher bits cannot become nonzero. After this step the part below h again consists of trailing ones. Induction on the remaining width requires one step for each further set bit of P. Adding the three contributions gives

$$
(k-\pi-1)+1+\pi=k .
$$

When a is the root the return runs from zero to the root. The population count must increase from zero to k, and the parent path attains the bound. All coefficients are 1 or −1 and the supports persist in every characteristic.

Thus local models cannot be excluded at any fixed level. To refute Bjerkevik’s conjecture one would also have to complete such models to mutually inverse pairs while retaining the required Hall violation at arbitrarily large thresholds.

**Finite sizes.** Bounds for Q₃ also come from classification of small local models. For n ≤ 14 over an arbitrary field and n ≤ 16 over the binary field, combinatorial exclusions are combined with finite checks. The result for n = 17 over the binary field is computational. Of 370 infeasibility cases, 299 have formal certificates; for the rest, two solvers gave the same answer. These finite bounds concern radius three and give no uniform estimate as n grows. See The Bjerkevik Conjecture, second revision, the section on bounds and Part V.

## References

H. B. Bjerkevik. Stabilizing Decomposition of Multiparameter Persistence Modules. [arXiv:2305.15550](https://arxiv.org/abs/2305.15550).

H. B. Bjerkevik. Stability of Higher-Dimensional Interval Decomposable Persistence Modules. [arXiv:1609.02086](https://arxiv.org/abs/1609.02086).

H. B. Bjerkevik, M. B. Botnan, M. Kerber. Computing the Interleaving Distance Is NP-hard. [arXiv:1811.09165](https://arxiv.org/abs/1811.09165).

The Bjerkevik Conjecture. Stability of Two-Parameter Modules, Its Combinatorial Forms, and the State of the Program. Unpublished manuscript, second revision, 24 September 2026.
