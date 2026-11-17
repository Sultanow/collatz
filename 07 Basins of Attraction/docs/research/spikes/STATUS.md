# Spikes, structural no-go and live classes: status (2026-11-17)

Four notes and two audits on the hypothesis (UI) of Proposition 14 (domination by the stopped
potential of odd targets). None of them proves `T_X = o(X)`. What entered the paper is
Remark 12 ("Averages, not maxima") of the draft after version 3.

## Audit

| Note | Statement | Verdict | In the paper |
|---|---|---|---|
| `Extreme_Spike_Barrier_Standalone` | `log G_X = o((log X)^eps0)` for the maximum of `a h(a)` over odd targets implies `T_X = o(X)`; failure forces spikes `exp(kappa (log X)^eps0)` | correct; with the constants of the paper (3+2b, 8, `X_1 < a <= X`) instead of the older 5+6b, 36, `X_1/4` | Remark 12(a), as a criterion that is **not** expected to hold |
| `Last_Mile_Spike_NoGo_and_Live_Filter`, Thm 1 | explicit odd `a = 2u 3^t - 1` in the dangerous classes whose orbit falls below `X_1` after `ceil(t/30)+1` steps | correct; exact integer checks up to t = 3200 | Remark 12(c) |
| same, Thm 2 / Cor 3 | under (UF) `h(n) >= kappa/n` these targets have `a h(a) >= c X^0.35`; moments `p > 2.85` diverge | correct as an implication; (UF) is taken from an unrefereed online manuscript that we have not checked | **not** used; the paper uses the rigorous profile bound below and the Run K maxima instead |
| same, Prop 4 | the domination holds with the *live* dangerous points (witnessing prefix above `X_1`) | correct; the proof of the odd-target Proposition 14 only counts such points | Remark 12(c) |
| `Dyadic_Odd_Flux_Spine_Standalone`, Thm | indicator of one inverse ray (u_0 = 5, e_i in {8..13}) satisfies the recurrence, the level identities and a bounded mean, but violates (UI) | correct; checked: forward invariance, exactly one supported preimage, no supported preimage of unsupported points (y < 10^6), sup of the mean 3.01 | Remark 12(b) |
| same, Lemma | dangerous odd classes have `nu_2(3a+c) <= floor((1-zeta_0) t_* + 1)` | correct (brute force, c = 1, -1, 5, 13, t_* = 12, 16) | not used |
| same, Prop | finite-source densities `h_N` vanish on the ray beyond a point | correct | Remark 12(b) |
| `Boundary_Flow_NoGo_and_Antichain`, Thm | convex combination of rays through every basin point: positive on the basin, still violates (UI) | correct | Remark 12(b), one sentence |
| same, Props | finite-source Poisson equation; sibling-leakage product; orbit-antichain packing | correct, elementary | not used (the packing bound is of order 1/K, too weak, as the note says) |

Additional fact used in the paper: the Syracuse profile satisfies `max_a v_n(a) >= (2/log(4/3)) (3/2)^n`
(the valuation word (1,...,1) has probability 2^-n). Table 13 shows 1.46 times this bound for n = 8, 12, 16.
The Run K maxima over odd targets in (X/2, X] grow like X^0.37 to X^0.62 (fits over 2^12 <= X <= 2^22,
`results/potential_ui/ui_odd.csv`).

## Conclusion

The pointwise route is closed in practice: maxima of `a h(a)` grow polynomially, which is compatible
with (UI). (UI) does not follow from the recurrence, the level identities, a bounded mean and positivity;
a proof has to use that `h` is the limit of finite counts, with some uniformity in the target. The next
step proposed in the notes is a weighted bound for the live dangerous points.

## Files

- the four notes (`.tex`, compiled `.pdf`), the two audits (`PROOF_AUDIT_*.md`)
- `test_spike_construction.py`, `construction_checks.json`: exact checks of the construction (reproduced)
- `verify_spike_notes.py`, `verify_spike_notes_output.txt`: independent checks of the ray, the valuation
  lemma, the live filter on the construction and the profile bound
