# Tail-tightness audit — 2026-11-16

## Inputs inspected
- `main(4).tex` (1725 lines)
- `truncated_calibration.tex` (172 lines)
- `tail_tightness.tex` (245 lines before the small edit)

## Status of the key statements
- Conditional clipped calibration: `C_X = (2/log(4/3)) X + o(X)` as proved in `truncated_calibration.tex`, provided Lemmas reduction, valuations and model hold with the claimed uniformity.
- Exponential model-tail comparison: `mu_X(Lambda) <= C exp(-Lambda/C) + C (log X)^(-epsilon_0)` follows from the stated reduction, coupling and a renewal-model exponential-moment bound. The renewal calculation appears coherent on inspection, but this audit is not a formal proof of all analytic inputs.
- Consequence: tightness in probability of `Y_X = Phi_X/X`, not uniform integrability.
- Clipping tail: `T_X = o(X)` is not established; same for `R_X = o(X)`, so the full Syracuse-profile upper bound remains open.

## A concrete correction
The original remark said, "unbounded orbits and cycles above X_1 do not contribute." This overstates what the definition establishes: only orbits that **never** drop below the cutoff have Phi=0. A possibly unbounded orbit that crosses the cutoff first could have a nonzero finite initial first-passage contribution. The revised LaTeX corrects the quantifier.

## Mathematical strength and prior art
Terras: finite parity-cylinder equidistribution. Tao and Gonçalves–Greenfeld–Madrid: first-passage stabilization / approximate transport. Ordinary renewal and stopping-time estimates: model local visit estimates. Hence the present distribution comparison is best presented as a derived, conditional estimate, not as a new independence theorem or a Collatz breakthrough. The genuinely missing input is control of weighted exceptional **mass**, not just exceptional probability.

## Sharp limitation (toy example)
Let `delta_X=(log X)^(-epsilon_0)` and take a random variable `Y_X` that is zero with probability `1-delta_X` and `X/4` with probability `delta_X`. For suitable absolute constants, the displayed bound
`P(Y_X>Lambda) <= C exp(-Lambda/C) + C delta_X`
holds for every Lambda, but for any polylogarithmic clipping level `L_X=o(X)`,
`E (Y_X-L_X)_+ ~ (X/4) delta_X` does not tend to zero. This is a counterexample to **deducing** uniform integrability from a bound of that form; it is not a counterexample arising from an actual Collatz orbit.

## Valid but modest consequence, to keep in research notes
For `0 < theta < 1/C`, integration of the distribution bound yields
`E_log exp(theta min(Y_X,L)) <= 1 + C theta/(1/C-theta) + C delta_X (exp(theta L)-1)`.
Taking `L=(epsilon_0/(2theta)) log log X` gives a uniform bound on the capped exponential moments. This is a useful precise intermediate statement but is a direct consequence of the preceding estimate, not a standalone headline theorem.

## Novelty-sensitive next target
Work beyond the `2`-adic/GGM coupling horizon: for any choice
`(log X)^(epsilon_0/2) <= M_X = o((log X)^(epsilon_0))`, prove
`E_log[(Phi_X - M_X X)_+] = o(X)`.
An alternative sufficient target is `E_log[(Phi_X/X)^p] <= K (log X)^A` for some `p>1` and `A<(p-1) epsilon_0`.
Neither follows from Terras parity equidistribution or the current model coupling. Keep the residual `R_X=o(X)` as a separate second problem.

### Equivalent intrinsic rare-mass condition (a precise target)
Choose `M_X=(log X)^(3 epsilon_0/4)` (between the clipping height and the transfer scale), and write `Y_X=Phi_X/X`. Under the new distribution bound,

`M_X P_log(Y_X>M_X) <= C M_X exp(-M_X/C) + C M_X (log X)^(-epsilon_0) = o(1)`.

Since `E_log[Y_X 1_{Y_X>M_X}] = E_log[(Y_X-M_X)_+] + M_X P_log(Y_X>M_X)`, and Corollary `extreme-tail` compares this to the original clipping tail, we obtain the equivalence

`T_X=o(X) <=> E_log[Y_X 1_{Y_X>M_X}]=o(1)`.

This is an exact specification of **weighted exceptional occupancy** beyond the coupling horizon; it does not assume any independence of later visits. It is still unproved for actual Collatz orbits.

## Reproducibility
The uploaded LaTeX claims numerical Run I results from `tail_check.c`, but the raw Run I outputs and `tail_check.c` were not included among the three attached files. Their numerical values were therefore not independently rerun or verified in this audit.

## Editorial recommendation
Replace the current file in `docs/` with the revised `tail_tightness.tex` as a targeted clarification. Do **not** promote this to a new main-text breakthrough claim or a Lean certification. Save the audit separately under `docs/research/`.
