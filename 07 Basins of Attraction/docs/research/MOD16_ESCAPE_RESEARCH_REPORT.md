# Conditional mod-16 escape and monotone-position obstruction

Date: 2026-11-16. Research artifact, not an independent verification of the manuscript's global Syracuse-profile theorem.

## Rigorous findings (paper proofs in `docs/research/candidate_mod16_escape_transport.tex`)

**Local escape.** For odd `c >= -1`, sufficiently large shell scale `L`, and `I_L=(L/2,L]`, every `y` in

```
G_c = {y : y ≡ 0 mod 4 or 3y+c ≡ 0 mod 16}
```

reaches `<=L/4` after at most four accelerated steps, having visited `I_L` at most twice (including `y`). These are precisely residue classes `0,4,8,12,5c (mod 16)`; they constitute 5/16 of **uniformly sampled residues**, not necessarily 5/16 of conditioned entrance laws.

**Conditional transport criterion.** With `N` the number of shell visits before a GLOBAL cutoff `a<=L/4`, suppose uniformly over return index `k`:

- conditional good-class probability `P(Y_k in G_c | N>=k) >= g>0`;
- probability of a subsequent rebound to the shell after first dropping to `<= L/4`, given a good class, is `<= theta<1`.

Then `P(N>=k+2) <= (1-g(1-theta))P(N>=k)` and

```
E[N^2] <= 4*(2-g*(1-theta))/(g*(1-theta))**2.
```

An upper bound `delta<5/16` for the *conditional* total-variation discrepancy modulo 16 would imply `g>=5/16-delta`. The needed `delta` and rebound estimates are **OPEN**. Crucially, the local cutoff `L/4` is in general higher than the paper's global first-passage cutoff, so local escape does not prove `T_X=o(X)`.

**New obstruction to monotone position-only certificates.** For any odd `c>=-1`, integers `L>=16(|c|+1)`, and `64<=a<=L/4`, the terminating first-return graph in `(L/2,L]` cut off at `a` has an explicitly constructed increasing return edge *and* a decreasing return edge, both of which are guaranteed to reach `a`. Thus no globally nondecreasing or nonincreasing potential `U(y)=f_L(y/L)` can satisfy the strict additive Lyapunov inequality at every return edge, even if `f_L` depends on `L`. The proof uses a power-of-two congruence class with modulus `O(L/a)` and two intervals of length `Theta(L)`. No conjectured Collatz convergence is used.

## Numerical checks

- Exhaustive finite regression: 30 configurations, 163,760 shell states enumerated, all good-class local escape checks passed.
- Explicit opposite-direction terminal witnesses: 1,000 randomized admissible configurations and a fixed arithmetic grid passed.
- Approximate log-uniform starting values in `[4X,16X)` (10,000 per map and scale), three shell heights `X=10^4,10^5,10^6` and maps `c=-1,1,5,13`: first three good-residue fractions are roughly 0.30–0.33. These runs used a **local** global cutoff `X/4`; all sampled paths reached it.
- Separate **deeper-cutoff** test `a=X/64`: 5,000 sampled starting values per map/scale. For `X=10^6`, all four datasets resolved, and the first-visit good AND no-later-rebound fractions are 0.2154–0.2322. For `X=10^4` the `c=5` and `c=13` datasets contained unresolved trajectories (some cycles do not cross this unusually deep cutoff), so their conditional statistics are not used to draw conclusions.
- Neither finite tests nor simulations give a uniform lower bound for every return index or for asymptotic `X`.

## Priority next step

Prove **uniform, conditioned** statements for the actual logarithmic entrance law: positive mass on `G_c (mod 16)` AND a uniform lower bound for *no subsequent rebound*. If such statements hold over all relevant dyadic shells and return indices, the conditional criterion supplies the missing second-moment bound for the clipping tail. Alternatively, find a weighted joint position/residue Lyapunov certificate that directly controls return moments.

## Lean scope

`lean/CollatzBasins/Mod16Escape.lean` contains a few arithmetic and order-theoretic components of these claims, as a **candidate**; it is NOT kernel-certified until an independent green CI build and axiom audit. The whole stopping-time construction and the conditional distribution inequalities are not yet Lean-formalized.
