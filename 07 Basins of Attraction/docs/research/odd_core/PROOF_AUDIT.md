# Proof audit — Odd-Core Compression (2026-11-17)

## Checked paper-proof assertions
1. **Dyadic even-run charging:** each maximal consecutive even run in `(U/2,U]` contributes less than `2U`; all but the initial run require a distinct odd transition from `((U-c)/3,U]`. The no-jump-over condition holds for `U >= 4|c|+12`.
2. **Odd delayed-visit classification:** an odd state in the expanded band with at least `b` later odd visits has a parity-word prefix in the high-odd-rate language. The `t<t_X` case uses the endpoints' bounded ratio (~3); `t>=t_X` uses survival above `X_1`. This rests on the product identity and parameters in `main(9).tex`.
3. **Predecessor domination:** any later odd target on a successful first-excess path has original start in its predecessor set; no state repeats before reaching the lower cutoff. This bounds its logarithmic measure by `h_X^downarrow(a)`.
4. **Summing shells:** at most 3 expanded dyadic bands contain any `a` for large `X`, with weights `2^{-j}<=4a/X`, yielding constant 36 in the upper bound.
5. **Sparse-set UI:** the bad language has density at most `alpha_b` and modulus `2^{t_X}=O(X^0.12)=o(X)`; odd-target UI or a uniform odd-target L^p bound removes its contribution.
6. **Synthetic dyadic harmonic spine:** `H(n)=1_{n=2^k}` solves the nonperiodic recurrence for `3x+1` outside the 1-2 cycle, has bounded normalized first moment, but fails global UI. It is NOT the actual logarithmic predecessor density.
7. **Dyadic flux:** for nonperiodic `y=(3a+c)/2`, `h(a)=h(y)-h(2y)`; telescopes along nonperiodic dyadic rays. Periodic exceptions are essential.

## Program checks
Run `python verify_odd_core.py`. On the prepared sample this yielded 29,520 finite trajectory-prefix tests over c in {-1,1,5,13,31}, plus 99,997 harmonic recurrence checks. These tests verify finite algebra and do NOT certify any asymptotic statement.

## Unproved inputs / NOT claimed
- `main(9).tex` first-excess formula and its `mu_X=O((log X)^(-eps_0))` are imported as manuscript hypotheses, not independently established here.
- The uniform odd-target integrability / L^p estimate for actual `h_X^downarrow` is OPEN. Hence `T_X=o(X)` is NOT established.
- No Lean 4 kernel formalization of the new lemmas/theorems was run.
- We have NOT checked priority or novelty against all possible literature; some ingredients are elementary.
- `main.tex` remains unchanged.
