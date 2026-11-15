# Return potential / Green operator: independent finite-stage audit

**Status (2026-11-15):** finite algebraic identities proved on paper and tested by exact integer arithmetic. A new Lean core is a **candidate**, not kernel-checked. The desired uniform shell moment estimate for real Collatz orbits remains open.

## 1. Why we revisited the transfer-operator proposal

The previous research note established a **conditional** criterion: a measure \(\nu\) satisfying \(q_1\le C\nu\) and \(Q\nu\le \rho\nu\), with \(\rho<1\), implies geometric return tails. This demands **uniform exponential-moment control**, generally stronger than the uniform second moments actually needed. A cycle intersecting the return graph also prevents strict contraction for a positive reference measure on that cycle.

A more precise route is the *Green/occupation measure*, which is an exact deterministic object and does not posit repeated independent valuation vectors.

## 2. Killed potential = expected occupation before first passage

Fix threshold \(a\), finite shell \(I\subset\{a+1,a+2,\ldots\}\), a finite initial probability law \(\mu\), and assume every start in its support reaches \(\le a\) in finite time.

* \(N_I(n)\) is the number of *time visits* to \(I\) before that passage.
* \(h_{a,\mu}(y)=\mu(\text{the orbit visits }y\text{ before passage})\).
* \(r_{I,a}(y)\) is the count of subsequent shell visits starting from \(y\) before passage, defined only when its orbit crosses the threshold.

A finite-passage trajectory cannot repeat any state above threshold: any repetition would produce a periodic orbit that never crosses. Thus the shell visit sequence \(y_1,\dots,y_N\) consists of distinct states and \(r(y_i)=N-i\). Consequently, pointwise:

\[
 N^2=\sum_{i=1}^N[1+2(N-i)].
\]

Averaging and changing the order of **finite** sums gives the exact formula:

\[
 \boxed{\mathbb E_\mu N_I^2
    =\sum_{y\in I:h_{a,\mu}(y)>0}
       h_{a,\mu}(y)\,[1+2r_{I,a}(y)].}
\]

This works for arbitrary initial distributions, not just uniform samples, and for every deterministic map satisfying the stated assumptions.

## 3. Exact return Green identities

Define \(q_1\) as the first-entrance subprobability measure on \(I\). Define \(Q\) as push-forward under the *partial* next-return map, with mass killed when the orbit reaches threshold before returning. On the states reachable from \(q_1\), the graph is acyclic. Hence for sufficiently large finite \(L\), \(Q^Lq_1=0\), and

\[
 \nu=\sum_{j\ge0}Q^j q_1, \quad
 \sigma=\sum_{j\ge0}(j+1)Q^jq_1.
\]

Algebra yields

\[
 \nu-Q\nu=q_1,\quad \sigma-Q\sigma=\nu,\quad
 \mathbb E N_I=\nu(I),\quad
 \mathbb E N_I^2=2\sigma(I)-\nu(I).
\]

And because every visited shell state occurs at most once before passage,
\(\nu(y)=h_{a,\mu}(y)\) exactly. Thus the Green measure is an **initial-law-and-killing-dependent predecessor potential**.

## 4. No-go result for the naive choice of potential weight

The exact first Green identity holds **pointwise**:

\[
 (Q\nu)(y)=\nu(y)-q_1(y).
\]

If a state is visited after a return but never occurs as a first-entrance state under the given law, then \(q_1(y)=0<\nu(y)\). At that state \((Q\nu)(y)=\nu(y)>0\), so the candidate **\(\nu=h_{a,\mu}\) cannot satisfy \(Q\nu\le\rho\nu\) for any \(\rho<1\)**. This is a rigorous obstruction to the *naive* choice of the exact occupation potential as the strict contraction reference weight; it does not rule out other weights.

The finite dyadic test at \(X=100{,}000\), with actual first entries from uniformly spaced outside starts, found respectively 5,384, 4,967, 4,477, and 4,538 occupation states for \(c=-1,1,5,13\) without any first-entry mass. Thus the obstruction is already present in the finite models.

There is a canonical *discounted potential*, for each fixed \(0<\rho<1\):

\[
 \nu_\rho=\sum_{j\ge0}\rho^{-j}Q^j q_1,
 \quad Q\nu_\rho=\rho(\nu_\rho-q_1)\le \rho\nu_\rho,
 \quad q_1\le\nu_\rho.
\]

But this does not solve the real problem, because

\[
 \nu_\rho(I)=\mathbb E_\mu\Big[\sum_{j=0}^{N_I-1}\rho^{-j}\Big]
\]

requires a **uniform exponential return moment**, stronger than a uniform second moment. The exact formulas simply make this distinction explicit. Any Lean theorem for this algebra would certify only the finite implication, not the missing uniform bound.

## 5. How this relates (and does not relate) to the paper's h(y)

The paper's \(h(y)\) is the **unrestricted** logarithmic density of all positive starting values that ever reach \(y\). By contrast, \(h_{a,\mu}\) uses a specific initial law and excludes paths that hit \(\le a\) earlier. In general:

\[
 h_{a,\mu}(y)\ne h(y).
\]

That distinction is essential: the exact conservation law for \(h\) does **not** automatically establish a bound for the *killed* second moment.

**An exact new consequence for fixed shells.** Suppose \(I\) is finite and contains no periodic point, and the logarithmic hitting densities \(h(y)\) exist for all \(y\in I\). Define \(r_I(y)\) as the number of distinct *later* shell states visited by the forward orbit of \(y\), without killing. Then for \(N_I^{\rm full}(n)\), the number of distinct shell states on the entire orbit of \(n\),

\[
 \boxed{\lim_{M\to\infty}\frac{1}{H_M}\sum_{n\le M}\frac{N_I^{\rm full}(n)^2}{n}
   =\sum_{y\in I}h(y)\,[1+2r_I(y)],\quad H_M=\sum_{n\le M}\frac1n.}
\]

This follows from the *same pointwise finite identity*, then exchanging a finite sum with the limit. It needs no independent statement about densities of pairs of predecessor sets. The excluded-periodic-point assumption is important: otherwise an orbit could revisit a point infinitely often. The result concerns **fixed** finite shells and cannot by itself yield a bound uniform in moving dyadic shells.

## 6. Quantitative work still open

To deduce \(T_X=o(X)\) using the local shell criterion, we need a uniform bound on \(\mathbb E_\log[N_{X,j}^2]\), equivalently a bound on the matching killed-potential sums *if the relevant logarithmic hitting laws and means exist*. None is currently proved for the full actual-orbit process.

Potential attack: estimate \(h_{a_X}(y)\) from above using \(h(y)\) **under a common logarithmic initial law**, and control the return depth \(r_{I_{X,j},a_X}(y)\). Merely inserting the heuristic \(h(y)\approx\kappa/y\) would not be a proof. In particular, the paper's potential conservation identity does not provide the required \(\ell^2\)-type uniform estimate.

## 7. Exact numerical checks (scope and caveats)

`C/tests/return_potential_exact.py` constructs the complete finite first-return graph for positive integers in either:

- `--shell-kind narrow`: \((\lfloor X^{199/200}\rfloor,X]\), threshold \(\lfloor X^{99/100}\rfloor\);
- `--shell-kind dyadic`: \((\lfloor X/2\rfloor,X]\), threshold \(\lfloor X/4\rfloor\).

The dyadic mode is a genuine full factor-two shell, but its threshold is **not** the paper's moving \(X^{1-\beta}\) threshold. These checks validate the finite identities, not the asymptotic theorem. The tool marks unresolved first-event paths and return cycles instead of assuming convergence. The outside-start experiment ignores trajectories that do not hit the shell before passage and reports any unresolved samples separately.

For each resolved acyclic graph, it verifies **at every node** \(\nu-Q\nu=q_1\) and \(\sigma-Q\sigma=\nu\), plus the two independent calculations of total squared visit counts. All calculations are integer exact (no Monte Carlo arithmetic in these identities).

For the uniform first-entrance distribution on \((X/2,X]\) with threshold \(X/4\), the finite second moments were:

| c | X=10,000 | X=100,000 | X=1,000,000 |
|---:|---:|---:|---:|
| -1 | 19.61020 | 18.98152 | 19.224834 |
| 1 | 19.18880 | 18.67078 | 18.947698 |
| 5 | 19.27880 | 18.62690 | 19.033318 |
| 13 | 19.68160 | 18.52676 | 18.923398 |

These numbers are finite diagnostics under an *artificial uniform entry law*. They say nothing about the conditioned true entrance laws at large scales.

## 8. Lean verification

`ReturnGreen.lean` contains only finite counting algebra (the square-as-odd-sum identity and weighted analogue). It has **not** been compiled here with Lean. Add it to `CollatzBasins.lean` and the two theorem-specific axiom-print commands only when ready to run CI. A green `lake check` would certify the *finite algebra*, not the log-density passage or uniform moment bound.

## 9. Recommended next move

Formalize the exact killed-potential identity for finite resolved trajectories, including a proof that first-passage segments cannot repeat a state. Then investigate a *scale-uniform potential-depth estimate*, not a blanket iid regeneration assumption. This is a more direct route from the paper's h(x) to the moment problem, but no convergence result is claimed.
