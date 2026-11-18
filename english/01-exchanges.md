# A Coarse Theory of Matroid Basis Exchange

**Abstract.** For a pair of bases we study the radius of mutual reachability, the existence of monotone sequences of symmetric exchanges, and the move size required to connect the space of basis pairs. For paving matroids and their duals, we prove that any prescribed subset of the first basis can occur as the initial segment of a full exchange sequence. For a represented pair, the monotone state graph factors over strongly connected components. A family of binary matroids has unbounded move size for connectivity.

## 1. Basis pairs and three exchange problems

Let M be a finite matroid of rank r with set of bases ℬ. Bases X and Y are complementary if they are disjoint and cover the ground set E. For arbitrary bases, contract their intersection and delete elements outside their union. Bases of the resulting minor lift by adjoining the intersection. This preserves the admissibility of every exchange considered below. Henceforth X and Y are complementary.

In the bipartite digraph draw an arc from x to y if replacing x by y preserves the basis X. Draw an arc from y to x if the reverse replacement preserves Y. For vertices of opposite colors define the mutual distance and the radius of the pair by

$$
\rho(x,y)=\max\{d(x,y),d(y,x)\},\qquad
c(X,Y)=\min_{\pi:X\overset{\sim}{\to}Y}\max_{x\in X}\rho(x,\pi(x)).
$$

The distance to an unreachable vertex is infinite. A finite radius for a nonempty pair is odd. At threshold one, a perfect matching gives each element a symmetric exchange partner in the initial pair. By Hall’s theorem, the radius is at most c precisely when every subset of X has at least as many neighbors at mutual distance at most c as it has elements.

For equal-sized subsets I and J of the first and second bases, respectively, put

$$
S(I,J)=(X\setminus I)\cup J,\qquad
\overline S(I,J)=(Y\setminus J)\cup I .
$$

The pair (I,J) is a state if both sets are bases. Its level is the size of I. A monotone step adds one element to each of I and J. A full chain runs from the empty pair to (X,Y). Consider three properties of a fixed pair of bases.

**Gabow.** There exists a full chain.

**Kotlar–Ziv.** For a given subset P of the first basis, some state (P,Q) is reachable from below by a monotone chain.

<a id="ex-prefix"></a>

**Flexible serial exchange.** For a given P there exists a full chain through a state (P,Q). The order within P and the choice of Q are free. The two parts of the chain must share the same Q. This requirement matters when upward and downward reachability are established separately.

A fourth problem concerns connectivity. Its vertices are all unordered partitions of E into two bases. A move simultaneously exchanges two subsets of equal cardinality. Let κ be the least positive bound on the move size under which any two vertices can be joined. Elements can move more than once, and the endpoints can be different partitions. These conditions distinguish κ from Gabow’s property.

**Matroid defects.** Allow a monotone step to add h elements to each of I and J simultaneously. For a fixed pair write g for the least positive bound on the step size admitting a full chain. For identical bases set both g and c equal to one. Over all basis pairs define

$$
c^*(M)=\max_{X,Y}c(X,Y),\qquad
\gamma(M)=\max_{X,Y}g(X,Y).
$$

The fiber of an unordered pair of bases consists of all pairs in which each element has the same multiplicity in the multiset union. Let κ₂(M) denote the least positive move size that connects every nonempty fiber. The earlier κ concerns the single fiber with multiplicity one everywhere. The equality γ(M) = 1 expresses Gabow’s property. A static defect of one means that every basis pair admits a bijection consisting of symmetric one-element exchanges in the initial bases.

**Proposition.** None of the three defects increases on taking a minor.

**Proof.** It suffices to delete or contract one element e. If e is not a coloop, the bases of the deletion are precisely the original bases avoiding e. If e is not a loop, bases of the contraction lift by adjoining e to each basis. In the first case e is absent throughout a lifted exchange sequence. In the second it belongs to both bases and stays fixed. Deleting a coloop uses the same lift as contraction, and contracting a loop uses the same lift as deletion. Thus the arcs, monotone chains, and all moves in a fixed fiber are preserved. Taking maxima proves the claim.

In particular, a fiber disconnected at a given move size in a minor gives such a fiber in any matroid containing that minor. Projecting a monotone chain in a direct sum onto either summand gives a local chain after repeated states are removed. Conversely, the local chains can be executed in succession. Thus γ of a direct sum is the maximum of the local values. The static defect has the same property, since exchange arcs cannot cross between summands.

## 2. Reachability in paving matroids

A rank-r matroid is paving if every circuit has at least r elements. Every set of r−1 elements is then independent. Two distinct hyperplanes intersect in at most r−2 elements, since any larger intersection contains an independent set of rank r−1 whose closure determines the hyperplane.

**Theorem 1.** If M or its dual is paving, every complementary basis pair admits flexible serial exchange for every P. Every intermediate state outside levels 2 and r−2 lies on a full chain. For r ≥ 5, at either exceptional level and for fixed P, at most one state fails to lie on a full chain.

Taking the complements of I and J in X and Y exchanges the two intermediate bases and reverses a monotone path. Dualizing preserves the set of states because the bases of the dual are the complements of the original bases. It suffices to treat a paving M and reachability from below.

**Lemma 1.** At most 2i cells of the square table of i² possible predecessors of a level-i state are inadmissible. Each row has at most one failure in the first basis and each column at most one failure in the second.

**Proof.** Cell (x,y) returns x from I and y from J. The current first basis S must admit replacement of y by x. The unique fundamental circuit in S with x adjoined has at least r elements. At most one of the r+1 elements of this union lies outside the circuit. For fixed x only such a y can fail. For the second basis fix y. In the table of first steps from the empty pair the bounds are reversed. There is at most one first-basis failure per column and one second-basis failure per row.

**Lemma 2.** Every state of level one or of level at least three is reachable from below.

**Proof.** One step suffices at level one. For a level-three state, number the exchanged elements on each side from one to three. Deleting the two elements indexed by a cell (a,b) gives a candidate predecessor of level two. Its four level-one candidates form the square obtained by striking out row a and column b. Call this square closed if every cell fails at the first step.

At most one square can be closed. If two deleted rows coincide, the two squares cover a rectangle of size two by three. It contains at most three first-basis failures and two second-basis failures. The argument for coincident columns is symmetric. If the deleted rows and columns both differ, the union contains seven cells, while the entire table has at most six failures.

By Lemma 1 at most six cells of the predecessor table fail directly. At most one more has a closed square. Some cell among the nine therefore gives a predecessor reachable through an admissible first step. For i ≥ 4 the i² candidates outnumber the 2i possible failures. At least one predecessor has level at least three, so induction applies.

**Lemma 3.** For r ≥ 3 and a fixed two-element P, at most one admissible state (P,Q) is unreachable from below. If such a state exists, P has at least r−1 admissible partners.

**Proof.** Use P as the two rows and Q as the two columns. Unreachability means that all four first steps fail. Lemma 1 forces exactly two failures of each kind. The first-basis failures occupy one diagonal, the second-basis failures the other. In each row of the full first-step table at most one column fails in the second basis. Thus the rows P uniquely determine the two columns Q.

Index the elements so that exchanging pᵢ with qᵢ fails in the first basis and the two cross exchanges fail in the second. Introduce the hyperplanes

$$
H_i=\operatorname{cl}(X-p_i),\qquad K_i=\operatorname{cl}(Y-q_i),
\qquad i=1,2 .
$$

We have qᵢ ∈ Hᵢ. The hyperplane K₁ contains p₂ and excludes p₁, with the indices reversed for K₂. For q in Y outside Q, try the partner consisting of q₁ and q. Removing P from X and adding q₁ leaves r−1 elements spanning H₁, so the first half is a basis precisely when q lies outside H₁. In the second half, Y without q₁ and q, together with p₂, is an independent set of size r−1 in K₁. Adjoining p₁ always gives a basis. With q₂ instead of q₁ the condition becomes that q lie outside H₂. The distinct hyperplanes H₁ and H₂ intersect exactly in X without P. Thus every one of the r−2 choices of q supplies a distinct additional partner. Counting the original partner gives at least r−1.

**Proof of Theorem 1.** The multiple symmetric exchange theorem of Greene and Woodall gives every P an admissible partner. Away from levels 2 and r−2, Lemma 2 and complementation give both parts of the chain. For r ≥ 5 every level-two state can be continued upward. If one state is unreachable from below, Lemma 3 supplies other partners, all reachable. Complementation handles level r−2.

For r = 4 the middle level has at most one failure from below and one from above. Either failure guarantees at least three partners, leaving one on a full chain. For r = 3 every level-two state leads to the top. If a partner fails from below, there are at least two partners and one is reachable. Complementation gives the level-one prefixes. For r ≤ 2 a single symmetric exchange or the empty chain suffices to settle the intermediate levels. The empty and full prefixes occur on every full chain.

**Example.** In the graphic matroid of the complete graph on vertices 0, 1, 2, 3, take the spanning trees

$$
X=\{01,12,23\},\qquad Y=\{02,03,13\},\qquad
P=\{01,23\},\quad Q=\{02,13\}.
$$

Both halves after the simultaneous exchange of P and Q are paths. For each of the four possible first exchanges, however, one half contains a triangle. The freedom to choose a different Q makes Theorem 1 possible for this P.

**Ordered form.** If the matroid is sparse paving and r ≥ 5, every prescribed order of the elements of the first basis can be realized for every complementary pair.

**Proof.** Two distinct r-element nonbases of a sparse paving matroid have symmetric difference of size at least four. In fact, each is a circuit-hyperplane. Paving makes it a circuit, and dual paving bounds the size of a hyperplane by r on a ground set of size 2r. Distinct such hyperplanes cannot have an independent set of r−1 elements in common.

Fix the order of the first basis. A vertex at level i is now specified by the i-element subset J of the second basis. Distinguish first-half and second-half failures. Two lower neighbors of a fixed vertex differ by one replacement. Among these neighbors there is at most one failure of each kind. At most two singleton vertices are forbidden. On level two, forbidden pairs of the same kind are disjoint.

Let Uᵢ consist of the admissible level-i vertices unreachable from below. There are none at level one. Both singletons of an unreachable pair must be forbidden, so only one such pair, say {u,v}, is possible. One predecessor of an unreachable triple must be this pair, since at most two predecessors fail directly. The other two, {u,w} and {v,w}, are forbidden. For each kind of failure u belongs to at most one forbidden pair. This gives at most two choices of w and hence at most two unreachable triples.

An unreachable four-element set must contain at least two unreachable triples and is therefore determined by their union. An unreachable level-five vertex would need at least three unreachable predecessors, but there is at most one. Induction excludes unreachable admissible vertices on every higher level. In particular the admissible top vertex of rank r ≥ 5 is reachable in the prescribed order.

## 3. Matrices and products of state spaces

<a id="ex-minors"></a>

Suppose M is represented over a field k. Let A have as columns the elements of X in Y-coordinates, and let B be its inverse. Expanding the determinant along the retained unit columns after replacing part of Y by columns of X gives

$$
(I,J)\text{ admissible}\quad\Longleftrightarrow\quad
\det A[J,I]\ne0,\qquad \det A[J^c,I^c]\ne0 .
$$

The empty minor has determinant one. For each I, existence of a partner follows directly from Cauchy–Binet and Jacobi’s complementary minor identity.

$$
1=\sum_{|J|=|I|}\det B[I,J]\,\det A[J,I],\qquad
\det B[I,J]=\pm\frac{\det A[J^c,I^c]}{\det A}.
$$

<a id="ex-support-graph"></a>

Serial exchange requires a chain of nested partners. This chain can be found independently in the following blocks. Draw arcs from X to Y on the support of A and from Y to X on the support of B. These reverse all the arcs of Section 1, because the coordinates of x specify which elements of Y it can replace. Reversal preserves the strongly connected components and mutual distances.

<a id="ex-product"></a>

**Theorem 2.** Every strongly connected component has equally many vertices of each color. The monotone state graph is the Cartesian product of the state graphs of the diagonal blocks of A. The radius of the pair is the maximum of the block radii.

**Proof.** A nonzero permutation term from each determinant gives a covering by directed alternating cycles. Each cycle is contained in a strongly connected component and has equal color counts. Ordering components in reverse topological order puts both matrices in block upper triangular form with square diagonal blocks.

In a nonsingular selected minor, each block prefix contains at least as many selected rows as columns, since its early columns vanish below those rows. The complementary minor gives the opposite inequality. Subtracting consecutive prefix equalities shows that I and J have the same local size in every block. Both determinants now factor over their diagonal blocks. A single step changes exactly one block, yielding the Cartesian product. Mutually reachable vertices lie in one component, and a path between its vertices cannot leave and reenter it. This proves the radius formula.

<a id="ex-assembly"></a>

**Corollary.** Suppose each block realizes the restriction of a prescribed prefix P. Performing all local prefixes first and all local suffixes afterward gives a full chain through P. In particular it suffices that the matroid represented by each block or its dual be paving. The criterion concerns the given basis pair.

For block size b there is a finite exact enumeration. The number of candidate states is

$$
\sum_{i=0}^{b}\binom bi^2=\binom{2b}{b}.
$$

After testing the two minors, mark reachability from below and above in level order. Their intersection gives every state on a full chain. Saving a predecessor and a successor reconstructs the chain. The field operation count is bounded by the sum of the candidate counts, each multiplied by a polynomial in the block size for the minor tests and neighbor enumeration.

A connected matroid can have singleton blocks for the chosen pair. For r ≥ 3 set

$$
x_1=y_1,\qquad x_i=y_{i-1}+y_i\quad(2\le i\le r).
$$

Both transition matrices are upper triangular. The pairs with matching indices may be exchanged in any order. Yet the two-element circuit on the first pair and the three-element circuits at successive indices connect the whole matroid. The first pair is parallel and the last pair is a two-element cocircuit. Neither the matroid nor its dual is paving.

## 4. Unbounded exchange size between basis partitions

Let k ≥ 3. Read j modulo 3k and t modulo k. In the binary vector space with standard basis define

$$
u_t=\sum_{j\equiv t\bmod k}e_j,\qquad a_j=e_{j-1}+u_{j\bmod k}.
$$

The columns e and a give a binary matroid of rank 3k. Partition them into pairs and blocks

$$
P_j=\{e_{j-1},a_j\},\qquad \Lambda_t=\bigcup_{j\equiv t\bmod k}P_j .
$$

For a partition into two bases let s count the pairs split between the two bases. Either half gives the same count. A size-h exchange changes membership for exactly 2h elements and hence changes s by at most 2h.

**Theorem 3.** For this family the connectivity threshold of the space of partitions into two bases satisfies

$$
\kappa\ \ge\ \left\lceil\frac{k}{2}\right\rceil .
$$

**Proof.** Each block contains three pairs. Representatives from different pairs project onto distinct coordinate vectors in the three-dimensional space indexed by the preceding residue class. Two complete pairs in one half are dependent because both pairs sum to uₜ. The same holds in the other half. Up to permutation, the numbers selected from the three pairs therefore have the following possibilities.

$$
(1,1,0),\quad (2,1,1),\quad (2,1,0),\quad (1,1,1).
$$

Call the types D, C, R, and, in the last case, E or O according to the parity of the number of selected a-columns. Only the u-directions couple neighboring blocks. Indeed, in a dependence the linear combination in block t must have the form

$$
v_{t-1}u_{t-1}+v_tu_t,\qquad v_t\in\mathbb F_2 .
$$

Projection onto the three coordinates of each class gives this identity, since the contribution from the next block must cancel a multiple of uₜ. The chosen elements are independent within each block. A global dependence therefore amounts to a nonzero cyclic sequence v satisfying the following local conditions.

$$
\begin{array}{c|ccccc}
\text{type}&D&C&R&E&O\\ \hline
\text{condition}&v_{t-1}=v_t=0&\text{any}&v_{t-1}=0&v_t=0&v_{t-1}=v_t
\end{array}
$$

For D the two representatives do not span the sum of the three projected coordinates. For C the full pair supplies uₜ in addition to three independent projections. For R only uₜ is available among the coupling directions. With one representative in every pair, their sum is the preceding u plus the parity of the selected a-columns times uₜ. This verifies all five columns. Complementation interchanges D and C, interchanges E and O, and preserves R.

If block t has type R, both halves allow (0,1). Unless the next block is also R, at least one half allows (1,0) there. A single nonzero entry of v at t would then give a dependence for that half. Any R therefore forces the entire ring to be R. Such a partition has s = k. With no R there are at least two split pairs per block, so s ≥ 2k.

Both types of partition exist. All R forces every entry of v to vanish in both halves. Any nonconstant cyclic word on E and O gives the other type. In increasing index order E sets the current entry to zero and O copies the preceding entry. Both the word and its complement have only the zero solution, and each half has 3k elements. Any path between the two types thus has a step changing s by at least k. The bound per exchange proves the inequality.

**Theorem 4.** With the notation of Theorem 3, for k ≥ 3 the threshold in the full-ground-set fiber is

$$
\kappa=\left\lceil\frac{k}{2}\right\rceil .
$$

**Proof.** First classify admissible words without R. Among words on E and O precisely the nonconstant ones are admissible. A word containing C or D must be a cyclic concatenation of segments

$$
C\,O^{a}\,D\,E^{b},\qquad a,b\ge0 .
$$

This follows from the coupling table. A nonzero run of v can start at C and continue through O. If the run ends at C or E, it gives a dependence. The all-O word admits the constant-one dependence. Conversely, the boundaries of a maximal nonzero run in any other solution give the same forbidden pattern; a run through an unrestricted C can be cut there. Complementation swaps C with D and E with O. The first non-O following C must therefore be D, and the first non-E following D must be C. This gives the displayed segments and equal counts of C and D.

Construct one move from the all-R word. For even k take an alternating word on D and C. Remove one element of a full pair to obtain D and add one element of an empty pair to obtain C. For odd k use one O and alternating D and C in the other positions. In the O block remove an element of a full pair and add an element of an empty pair with the required parity. In both cases the number removed equals the number added and is the claimed move size.

It remains to connect each type by single exchanges. In an R block, swap the selected half of the split pair for its mate, transfer the half to the empty pair, or move one element from the full pair to complete the split pair. These moves realize every permutation of the three roles and either choice of half. Different blocks can be treated independently.

In a word without R, a single exchange turns CO into EC by removing an element of the required parity from the full pair in C and completing a pair in O. Thus C moves toward the next D. Replace CD by EE if several segments remain, or by OE in the last segment. Transfer an element from the full pair of C to the empty pair of D, choosing the two resulting parities independently. The segment description shows that every intermediate word remains admissible. Every word without R is thus connected to a nonconstant word on E and O.

Such words are connected by single parity changes because the k-cube minus two opposite vertices is connected for k ≥ 3. One parity change swaps the elements of a split pair. Within a block of fixed parity, two configurations differ by an even number of half swaps. Perform these in pairs. If the first swap would produce a constant word, temporarily change the parity of another block and restore it after the two swaps. A third block retains the opposite value, keeping the intermediate words admissible. This proves connectivity within both types. Theorem 3 and the move constructed above give equality.

This estimate concerns partitions of the entire ground set into two bases. For the maximum connectivity threshold over all minors of the family the proof supplies a lower bound. Equality for that maximum requires a separate argument.

At k = 3 this is the matroid in Larson’s example. In Gabow’s monotone problem the final ordered pair reverses the original bases. In the unordered partition space those endpoints coincide. Theorem 3 thus concerns a different obstruction from failure to complete a monotone exchange chain.

## References

D. Kotlar, R. Ziv. On Serial Symmetric Exchanges of Matroid Bases. [arXiv:1110.1826](https://arxiv.org/abs/1110.1826).

K. Bérczi, T. Schwarcz. Exchange Distance of Basis Pairs in Split Matroids. [arXiv:2203.01779](https://arxiv.org/abs/2203.01779).

M. Larson. Counterexamples to Two Conjectures About Matroids. [arXiv:2607.02208](https://arxiv.org/abs/2607.02208).
