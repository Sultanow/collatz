# Exceptional first-passage mass: research audit

**Status (15 Nov 2026): CONDITIONAL MATHEMATICAL RESULT.** Not in the main paper. The argument is a deduction from the manuscript's Lemma 2 (uniform reduction for *every* bounded first-passage-invariant test function), Step 3 of Theorem 2 (high-probability coupling with actual Syracuse trajectories), and the *unstopped* ideal-window L2 bound extracted from Lemma 4. These upstream claims still need independent line-by-line certification. Nothing in this note proves `T_X=o(X)`.

## Definitions (retain exact manuscript notation)

- Fix `beta=1/200`, `W_X=(X^(1-beta),X]`, `X_1=X^(1-2beta)`.
- `Phi_X(m)`: sum with multiplicity of targets in `W_X` in the Syracuse blocks through the first passage below `X_1`; it is zero if no passage occurs. For finite passage no visited point repeats before that passage, so `0 <= Phi_X <= B_X=sum_{a in W_X} a = O(X^2)`.
- `mu_X`: the limiting distribution of `Phi_X(w^flat)` under `(log N)^(-1) sum_{w<N} (1/w) delta_{Phi_X(w^flat)}`. This exists by Lemma 2 applied to the indicator of every value (finite support).
- `nu_X`: distribution of `Phi_X(N)` when `N` is the logarithmic random odd integer in the comparison window immediately above `X`.
- `Z_X^+`: the unrestricted *ideal* weighted window count, summing all model points, without a first-passage stop or residue condition.
- `H_X=X(log X)^(epsilon_0/2)`, `T_X=E_{mu_X}(Y_X-H_X)_+`.

## The genuinely new mathematical reduction

**1. Total-variation comparison of whole distributions.** The implied constant in Lemma 2 is independent of the chosen bounded invariant function. Applying it to `1_{Phi_X in A}` and taking the supremum over all `A` gives:

`dTV(mu_X,nu_X) <= C (log X)^(-epsilon_0)`.

This is stronger than comparing only the means of one clipped observable, but follows from the same uniformity. The finite support of `Phi_X` allows no measure-theoretic subtleties.

**2. Exceptional coupling.** Maximal coupling between the `mu_X` and `nu_X` variables, followed by the existing `nu_X`/ideal-process coupling, yields a joint law `(Y_X,Z_X^+,E_X)` with

`P(E_X) <= delta_X := C[(log X)^(-epsilon_0) + X^(-eta)]`,

`Y_X <= (1+O(X^(-1/2))) Z_X^+` off `E_X`, and `E[(Z_X^+)^2] <= C X^2`.

**3. Weighted exceptional-mass identity.** For each `H>=X`,

`-H delta_X <= E(Y_X-H)_+ - E[Y_X 1_{E_X}] <= C X^2/H`.

At `H=H_X`, the two errors are `o(X)`, so

`T_X = E[Y_X 1_{E_X}] + o(X)`.

This is a **necessary and sufficient** localization criterion **for the chosen coupling**:

`T_X=o(X) <=> E[Y_X 1_{E_X}]=o(X)`.

The exceptional event is a *coupling event*, not a canonical class of initial Collatz integers. Do not describe it as if it were an intrinsic arithmetic set.

**4. Intrinsic equivalent criterion.** The preceding tail-dist bound gives

`mu_X(Y_X>t) <= C X^2/t^2 + delta_X`.

For `K_X=X(log X)^(epsilon_0)/(loglog X)`, `K_X mu_X(Y_X>K_X)=o(X)`.
Combine this with the intermediate-tail proposition (another conditional research addendum) to obtain

`T_X = E_{mu_X}[Y_X 1_{Y_X>K_X}] + o(X)`.

Unlike `E_X`, the event `Y_X>K_X` depends only on the *actual* first-passage observable. Thus the **remaining precise theorem to prove** is

`E_log[Phi_X(w^flat) 1_{Phi_X(w^flat)>K_X}] = o(X)`.

## Why the current estimates cannot close the theorem

The logarithmic-transport error is `delta_X ~ (log X)^(-epsilon_0)`, while the deterministic cap is `B_X ~ X^2/2`. A toy distribution with a mass `delta_X` at `X^2/4` and the remaining mass at `gamma_0 X` has exactly the desired model L2 bound away from the exceptional event, total-variation error `delta_X`, and the correct clipped calibration up to `o(X)`. But it has `T_X` of order `delta_X X^2`, much larger than `X`. **This is an information-theoretic obstruction to the present *method*, not a counterexample to the Syracuse conjecture or a construction of a genuine orbit.**

## Next *independent* proof target

Either of the following suffices:

1. A uniform actual-orbit moment bound `E_{mu_X}[(Y_X/X)^(1+theta)] <= C` for some `theta>0`; this is stronger than necessary.
2. The sharper intrinsic weighted-tail bound `E_mu[Y_X 1_{Y_X>K_X}]=o(X)`.
3. A conditioned tail bound on the exceptional coupling event `E[Y_X 1_{E_X}]=o(X)` (depends on a carefully constructed coupling).

The ideal second moment cannot be used for the actual tail on `E_X`; that would assume precisely what is missing. An approach via entrance-state distributions must establish a **conditional** return-control theorem for the highly non-uniform states visited after atypical valuation sequences. Unconditional random starting-value estimates do not directly imply this.

## Review checklist before main-paper integration

- [ ] Check Lemma 2's TV error is uniform in *arbitrary* bounded invariant functions, including indicators of every level set of `Phi_X`.
- [ ] Check the endpoint of the source first-passage event: `(3/4)x exp(-(log x)^0.7)` and that this dominates `X` for `x >= x_0`.
- [ ] Prove the *unstopped* full ideal-window L2 bound, including the first crossing block and possible overshoots.
- [ ] Verify joint maximal-coupling construction preserves the joint law `(N,Z_X^+,G_X)`.
- [ ] Do not confuse finite-window numerical averages with `E_log`.
- [ ] Keep the proof as `docs/research/candidate_exceptional_mass_section.tex` until audited.
- [ ] Do not claim a proof of `T_X=o(X)`, `R_X=o(X)`, the upper Syracuse potential profile, or Collatz convergence.

## Further reduction: count visits rather than weight them

Let `V_X(m)` be the number of visits to points of `W_X` before first passage below `X_1` (including the final block). Because the points are distinct before first passage, `Phi_X(m) <= X V_X(m)`. If for some `theta>0` one could establish the **actual-orbit** uniform logarithmic moment bound

`sup_X E_log[V_X(w^flat)^(1+theta)] < infinity`,

then, for the original clipping threshold `H_X`,

`T_X <= X E_log[V_X 1_{V_X>H_X/X}] <= C X (log X)^(-epsilon_0 theta/2) = o(X)`.

This is a particularly concrete new target: prove a high-moment bound for the number of returns to the moving target window under **the actual logarithmic limiting law**. Uniform ideal-model moment bounds and unconditioned estimates for randomly sampled starts do **not** yet provide it. Any proposed regeneration argument must control the distribution of the state *conditioned on a previous return*, which may be strongly size-biased.
