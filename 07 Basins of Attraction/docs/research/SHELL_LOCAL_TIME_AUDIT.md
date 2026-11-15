# Dyadic shell local-time criterion — proof audit

## Status

- **Proved (elementary conditional statement):** Uniform second moments for the *actual-orbit* number of visits in each dyadic shell imply a uniform second moment for the normalized first-passage mass, hence `T_X = o(X)`.
- **Not proved:** The shell second-moment assumption itself for real Syracuse trajectories.
- **Not proved:** The separate residual estimate `R_X = o(X)`.
- **Not Lean-checked yet:** Any new statements in this addendum. The previously successful Lean CI covers the earlier finite predecessor and hitting identities only.

## Definitions (match the paper)

`W_X = (X^(1-beta), X]`, `beta = 1/200`; `Phi_X(m)` is the sum of block occurrences lying in `W_X` before first passage below `X^(1-2beta)` (with the convention `Phi_X=0` when passage never occurs).

`I_(X,j) = (X / 2^(j+1), X / 2^j] intersect W_X` and `N_(X,j)(m)` is its number of occurrences in precisely those blocks.

With `H_X = X * (log X)^(eps0/2)` and `T_X = E_log[(Phi_X-H_X)_+]`, the proposed assumption is

`sup_(X,j) E_log[N_(X,j)^2] <= B < infinity`.

## Proof of implication

1. Each point in shell `j` has weight at most `X*2^(-j)`, so `Phi_X/X <= sum_j 2^(-j)*N_(X,j)` pointwise.
2. For every fixed `X` there are only finitely many nonempty shells. By Cauchy–Schwarz, `E_log[N_j*N_k] <= sqrt(E_log[N_j^2] E_log[N_k^2]) <= B`.
3. Expand the finite square, then bound its coefficient sum by `(sum_(j>=0)2^(-j))^2 = 4`. Hence `E_log[(Phi_X/X)^2] <= 4B`.
4. For `t>=0`, `(t-H)_+ <= t^2/H`. Thus `T_X <= (X^2/H_X)*4B = 4 B X / (log X)^(eps0/2) = o(X)`.

No probabilistic independence or new Collatz theorem is used in steps 1–4. Existence of each fixed-X logarithmic mean is inherited *conditionally* from the corresponding bounded-observable first-passage framework in the paper. A separately written proof of this existence for N_j should be checked before promoting the proposition to the main paper.

## Important warning about the previous V_X criterion

The *model* renewal count in `W_X` has mean `gamma0 * beta * log X + O(1)`, `gamma0 = 2/log(4/3)`. Therefore its raw return count does **not** have a bounded uniform moment. This does not yet prove the corresponding statement for real orbits, but it makes the previous assumption `sup_X E_log[V_X^(1+theta)] < infinity` a poor candidate for an actual theorem. Use shell moments or weighted masses instead.

## Research program

1. Prove a uniform second-moment (or geometric tail) bound for the number of visits in an **individual fixed-ratio shell** under the limiting logarithmic starting measure.
2. Show that the conditional distribution after a first shell visit does not accumulate uncontrolled mass on rare long-return trajectories. A deterministic trajectory has no new independent randomness after re-entry; a Markov/geometric 'restart' assumption must be established, not presumed.
3. Use this lemma to derive `T_X = o(X)` as above.
4. Separately attack `R_X = o(X)`; the local-time criterion alone never controls it.

## Reproducibility

`C/tests/shell_visit_probe.py` measures finite log-weighted start windows, checks the pointwise shell inequality, and reports shell second moments. These finite samples are **not** estimates of `E_log` without a justified transport limit, and are **not** a proof.
