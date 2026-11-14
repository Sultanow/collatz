# Intermediate first-passage tails: proof audit and conditional result

**Status (2026-11-15):** Working mathematical addendum. The new *intermediate-tail* estimate is proved **conditional on** the current manuscript's Lemma 2 (uniform logarithmic transport), Lemma 4(b)'s visit-count L² estimate, and the successful coupling with probability `1 - O(X^-eta)`. It is **not** a proof that the full clipping tail `T_X=o(X)`. Do **not** promote this draft to the main paper before an independent audit of the prerequisites and constants.

## Exact definitions from the manuscript

- `W_X=(X^(1-beta), X]`, `beta=1/200`, and the lower first-passage threshold is `X_1=X^(1-2 beta)`.
- `Phi_X(m)` is the sum of the **distinct** targets in `W_X` occurring in Syracuse blocks up to first passage below `X_1`; it is zero if that passage never occurs. For arbitrary natural `w`, use its odd part `w^flat`.
- `E_log` denotes the limit of `(1/log N) sum_{w<N} (...) / w`.
- `C_X(A) = E_log min(Phi_X(w^flat), A)`.
- Original cutoff `H_X = X (log X)^(epsilon_0/2)`; original tail `T_X= E_log (Phi_X-H_X)_+`.

## New lemma: ideal process second moment without stopping

The proof of the manuscript's Lemma 4(b) bounds the `L²` norm of the number of **all** ideal points in any unit log-coordinate interval, not merely the stopped process. Summing these point counts with weights `exp(z)` from the upper edge of the target window gives

`E[(Z_X^+)²] <= C X²`,

where `Z_X^+` counts **all** model points, without stopping or a congruence restriction. This is the exact extra fact we need. It is a statement about the ideal geometric-valuation walk, not about actual Collatz trajectories.

## Main conditional proposition: intermediate tail control

Assume Lemma 2 transports every normalized bounded first-passage-invariant observable to the sample window with an `O((log X)^(-epsilon_0))` error, and that on coupling event `G_X` the true `Phi_X` is bounded by `(1+O(X^(-1/2)))Z_X^+`, with `P(G_X^c) <= C X^(-eta)`.

For every `X <= H <= K`, we get

`0 <= C_X(K)-C_X(H) <= C [ X²/H + K X^(-eta) + K (log X)^(-epsilon_0) ]`.

Proof outline: apply Lemma 2 to the normalized, invariant difference `D(m) = [min(Phi_X(m),K)-min(Phi_X(m),H)] / K`. On `G_X`, `D*K <= Phi_X²/H <= C(Z_X^+)²/H`; off `G_X`, `D*K<=K`. The three terms come from the ideal L² bound, coupling failure, and transport.

With

`H_X=X(log X)^(epsilon_0/2)`, `K_X=X(log X)^(epsilon_0)/(log log X)`,

all three terms are `o(X)`; in particular

`C_X(K_X)-C_X(H_X)=o(X)`.

Therefore the original unknown tail admits the **strictly sharper decomposition**

`T_X = o(X) + E_log (Phi_X(w^flat)-K_X)_+`.

All logarithms are natural and this is an asymptotic theorem (the cutoffs separate for sufficiently large X).

## Why the current lemmas do NOT imply T_X=o(X)

- Distinct target points yield only the elementary cap `Phi_X <= sum_{a in W_X} a = O(X²)`.
- The logarithmic transport error is proportional to the cap `K`. Taking `K=O(X²)` leaves an error `O(X² (log X)^(-epsilon_0))`, which is much larger than `X`.
- The successful-coupling L² estimate only controls `Phi_X` on the good event. A probability estimate `P(G_X^c)=O(X^(-eta))` alone leaves a possible exceptional contribution `O(X^(2-eta))`.
- Thus **two distinct improvements** are needed to lift all clipping: stronger tail-weighted transport for true orbits, or a genuinely new bound on their exceptionally large first-passage sums.

### Additional tail distribution estimate

For fixed `X` and `t>=X`, the event `Phi_X(w^flat)>t` is first-passage invariant wherever Lemma 2 applies. Combining bounded-observable transport, the model L² bound and coupling yields the conditional inequality

`delta_log{w:Phi_X(w^flat)>t} <= C X²/t² + C X^(-eta) + C (log X)^(-epsilon_0)`.

By the finite point cap `B_X=O(X²)` and layer-cake integration, this only implies

`T_X <= C X²/H_X + C B_X[X^(-eta)+(log X)^(-epsilon_0)]`,

which is **not** `o(X)`. This pinpoints why an ideal-model second moment and a total-variation estimate alone are not enough.

## Next proof target

Prove an actual-orbit, logarithmic-density tail estimate such as

`E_log (Phi_X(w^flat)-K_X)_+ = o(X)`.

One sufficient but stronger target is uniform integrability of `Phi_X/X`, for example a uniform `1+theta` moment bound in *the actual logarithmic starting-value law*, as already stated in `truncated_calibration.tex`. A potentially weaker approach is to bound the logarithmically weighted exceptional trajectories directly, using regeneration at intermediate passage thresholds.

## Integration instructions

- Retain `docs/truncated_calibration.tex` in the main paper.
- Store `candidate_tail_control_section.tex` here in `docs/research/` as a **working addendum only**.
- When audited, insert `\\input{research/candidate_tail_control_section.tex}` after `\\input{truncated_calibration.tex}` within `docs/main.tex`.
- No claim of a proof of Collatz convergence, of the full upper Syracuse-profile bound, or of `T_X=o(X)` is made.
