# Proof audit — relative 2-adic energy and single-depth block leakage

Source: user-supplied consolidated `main(6).tex` (16 November 2026), in particular `prop:first-excess`, `prob:excursions`, `rem:rigidity`. The main paper has NOT been modified.

## What is actually proved in this note

1. **Deterministic parity threshold (not a novelty claim)**: a same-shell return at a sufficiently late clock time forces `h_t >= zeta0*t`; survival to `t_X` forces the same inequality. The parity-residue bijection is classical (Terras), and a binomial Chernoff estimate counts the dangerous cylinders.
2. **Single-depth bad-prefix compression**: for fixed `b` and `T=t_X`, the event of `b` subsequent shell returns is contained in ONE set of residue classes modulo `2^T`, whose uniform density is at most `exp(-Delta*b)/(1-exp(-Delta))`. This avoids a sum of concentration defects over all depths. No probability independence is assumed for true trajectories.
3. **Capacity defect inequality**: with `M=|nu|`, `nu_t(r)` the per-residue masses and `d_t(nu;A) = sum_r (nu_t(r)-A*M/2^t)_+`, any union of `q` residue classes has mass at most `A*q*M/2^t + d_t(nu;A)`.
4. **Event-specific cylinder correlation (preferred)**: define one arithmetic exceptional residue set C_X at depth t_X. For every actual return law nu, put c_X(nu) = (nu(C_X) - (|C_X|/2^t_X)|nu|)_+. Then s_j(n+b) <= alpha_X*s_j(n) + c_X(nu_{j,n}); summing over all n>K_j shows E_X <= (2*b*mu_X + sum_j 2^-j sum_n c_X(nu_{j,n}))/(1-a_b), where a_b<1 is fixed. This uses only one ***specific*** dangerous set, not global residue uniformity. It is the weakest recommended target; the required weighted sum of the actual correlation defects has not been shown to vanish.
5. **Exact Fourier correlation**: c_X(nu) equals the positive part of m^-1 sum over k!=0 of the nontrivial Fourier transform of the dangerous-set indicator paired with that of nu. All conditional-law dependence is explicit. This formula is exact, not an average over uniformly random orbits.
6. **Real-orbit return inequality**: `s_j(n+b) <= q*s_j(n) + d_{t_X}(nu_{j,n};A)` for sufficiently large X, with a fixed `q<1` and fixed `b` (which may be numerically large).
7. **Integrated criterion**: `E_X <= (2*b*mu_X + D_X)/(1-q)`. Under existing paper hypotheses, `D_X=o(1)` would imply `T_X=o(X)`. The missing premise is **not** proved here.
8. **Relative Fourier-energy criterion (stronger)**: `d_t(nu;A) <= |nu| chi_t^2(nu)/(A-1)` and `chi_t^2 = sum_{k!=0} |hat p_t(k)|^2` (ordinary Parseval). Thus a single-depth weighted sum of squared actual conditional Fourier coefficients equal to `o(1)` is sufficient, **but not necessary and potentially far too strong**: even a harmless single-atom terminal law can make the chi-squared energy diverge as 2^t_X. The recommended target is instead the bounded event-specific correlation defect above.
9. **Terminal-atom barrier**: `d_t(nu;A) >= |nu|*(1-A*#occupied_residues/2^t)_+`. Small support forces strong concentration; the converse is false.

## Assumptions inherited from the main manuscript

- Existence of the logarithmic first-excess entrance measure `eta` and the bound `mu_X(L_X)=O((log X)^(-epsilon0))`.
- Exact first-excess identity and equivalence between `T_X=o(X)` and the weighted excess visit sum `E_X=o(1)`.
- Positivity and definedness of the shortcut map for fixed odd c >= -1.

The paper's analytic statements are conditional on its cited GGM/Tao-based lemmas. This note does not independently certify those prior inputs.

## Mathematical limitations

- The preferred event-specific target `C_X=o(1)`, the stronger `D_X=o(1)` and the still stronger full Fourier-energy criterion are **open for genuine stopped Collatz return laws**. In particular, existing absolute-error total-variation bounds do not imply the needed relative estimate.
- The per-shell return graph is finite only on trajectories that reach the lower cutoff; nonterminating trajectories are outside the first-excess observable by definition.
- Concentrated terminal laws can have large `D_X` even when their total mass is small. A pointwise tail/occupation estimate is still needed for them unless a direct bound on the total defect budget is established.
- `T_X=o(X)` would not establish the separate `R_X=o(X)` bound.
- Everything beyond the classical parity calculation is a conditional *interface* between energy and return tails. No claim of priority over existing generic capacity and Fourier inequalities is made.

## Important correction to `main(6).tex` (not applied)

The last sentence of its arithmetic-rigidity remark says, in effect, that once anti-concentration fails the remaining law is supported on at most `X^{O(beta)}` states. **That implication is not valid.** A diffuse law with one heavy atom has large support and can violate a relative cap. The correct logical direction is that support **below** `2^t/A` forces a large defect. This is Proposition 9 in the standalone note.

## Independent checks

- LaTeX standalone compiles (6 pages), references resolved; visual rendering checked.
- 9,999 rational-exact cap/chi/support inequalities with random masses, 10,000 rational-exact renewal/tail-sum inequalities, 5,000 event-defect inequalities, 500 Fourier/Parseval identities and 300 event-specific Fourier correlations were checked. These validate finite algebra, not the asymptotic hypothesis.
- No Lean kernel compilation or independent expert review of the new theorem was run in this workspace.

## A practical next theorem to try

Prove, for the actual stopped first-excess return laws,

    sum_j 2^(-j) sum_{n>K_j}
       (nu_{j,n}(C_X) - alpha_X * |nu_{j,n}|)_+ = o(1),

where `C_X` is the single explicitly defined union of dangerous parity-prefix residue classes mod `2^t_X` and `alpha_X=|C_X|/2^t_X`. This is weaker than full relative non-concentration or a chi-square norm bound. The proof would have to exploit genuinely *conditional* transport of the repeatedly filtered entrance laws. It is not proved in this package.
