# Proof audit — Dyadic Block Leakage for Actual Syracuse Return Histories

**Date:** 2026-11-16  
**Reference:** `Collatz_Weighted_First_Excess_Standalone.tex` from the preceding research step; main paper unchanged.

## Source assumptions

For fixed `X`, `W_X` and `a_X` are as in the referenced research note. The actual *limiting logarithmic* first-excess entrance measure `eta_X` exists under the manuscript's bounded first-passage reduction lemma. The tightness estimate `p_X = O((log X)^(-epsilon_0))` is imported from the manuscript and is not reproved or independently certified here. The first-excess sandwich is repeated with a proof.

## Mathematical statements and audit

1. **Dyadic layer-cake, proved algebraically.** For actual shell return mass `q_{X,j}(n)` and `B_{X,l}=sum_j 2^(-j) K_j q_{X,j}(2^l K_j)`, the weighted overshoot `E_X` satisfies `D_X/2 <= E_X <= B_{X,0} + D_X` where `D_X=sum_{l>=1}2^l B_{X,l}`. The key is the exact partition of the integer tail sum into intervals `(2^l K,2^(l+1)K]`.
2. **Equivalence, conditional on the manuscript tightness input.** The dyadic sandwich and `B_0<=p_X sum_j 2^(-j) K_j=o(1)` give `T_X=o(X) iff D_X=o(1)`.
3. **Flux identity, proved algebraically.** `B_l-B_(l+1)` equals the sum of the actual first-return kernel's losses on doubling return horizons. This uses no independence assumption.
4. **Aggregate dyadic contraction, conditional criterion.** `B_(l+1)<=q B_l` with `q<1/2`, *uniformly in X and l*, yields `T_X=O(X(log X)^(sigma-epsilon_0))`. **That contraction has not been established for actual Syracuse orbits.**
5. **Per-shell polynomial survival, conditional criterion.** If `q_{j,2n}<=q q_{j,n}` for every `n>=K_j`, then `q_{j,n}<=2^kappa p_X (K_j/n)^kappa` with `kappa=-log_2 q>1`. It is strictly stronger than aggregate contraction.
6. **Robust exceptional-error version, conditional criterion.** A leakage defect `e_{X,l}` may be admitted if `sum_l 2^l e_{X,l}=o(1)`; an error floor `O((log X)^(-epsilon_0))` independent of `l` is insufficient.
7. **No-go observation, elementary.** Nilpotence of the finite successful-state return kernel does not yield scale-uniform contraction: a long deterministic path is a counterexample to that attempted inference.

The one-half threshold is the critical summability threshold **for this dyadic geometric-series argument**, not a claimed universal phase transition.

## What has actually been verified computationally

- `verify_dyadic_identities.py`: **4,000** exact-rational, synthetic finite-path tests of the two-sided layer-cake and flux identities (all passed). They support the algebra, but are not Lean proofs.
- `exhaustive_finite_window.py`: **all 49,152 odd starts** in `[32768,131072)` for each of four real maps `3x+c` (`c=-1,1,5,13`), `X=8192`, integer cutoff `786`, window lower bound `2538`, first-excess threshold `3`, and allowances `K_0=4,K_1=5`. All trajectories terminated before the cutoff without repeats or truncation; all observed *finite-window uniform-measure* dyadic block ratios were below `1/2`. Computed with exact integer arithmetic. This is not the limiting logarithmic entrance measure and does not say anything uniform as `X->infinity`.
- `finite_orbit_probe.py`: **40,000 approximately log-uniform starts** from `[4X,16X)` for each of the same four maps, `X=10^6`, beta=0.13, L=10, K_j=ceil(4*2^(0.3j))`. Ratios of the aggregate dyadic tail `B_{l+1}/B_l` were `0.3082`–`0.3376` at the first dyadic doubling. Entirely exploratory finite-window sampling, not an asymptotic bound.
- At the actual manuscript beta `1/200` and moderate `X=10^6`, first-excess events can be too rare for useful Monte Carlo inference. The diagnostic beta was intentionally enlarged; do not substitute these numerical settings into the manuscript theorem.

## Main open mathematical task

Prove, for the actual `eta_X` produced by the first-excess stopping rule, the uniform weighted block-flux estimate

`sum_j 2^(-j) K_j eta_X E_j Q_j^(a_j-1) [1-Q_j^(a_j)1] >= delta B_{X,l}`

with `a_j=2^l K_j` and `delta>1/2`, or a version with `2^l`-summable defects. Existing Terras parity cylinder arguments, Tao/GGM approximate transport and finite kernel nilpotence do **not** establish this. It is not currently known whether the hypothesis is true.

No changes to `main.tex`; no new Lean theorem or kernel certification is claimed.
