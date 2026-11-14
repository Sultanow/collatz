# Truncated first-passage calibration — research note

## Scope and status

A new proposition (conditional on the manuscript's Lemmas 2–4 and their stated uniform estimates) establishes a **two-sided leading asymptotic for the clipped first-passage functional**, not for the unclipped first-passage mass and not for the full predecessor-potential mean. This is a mathematical proof *within the manuscript's current assumptions*, not an independently certified Collatz result.

Notation: beta=1/200, gamma0=2/log(4/3); target window W_X=(X^(1-beta),X]; first passage below X1=X^(1-2beta). Phi_X(m) sums targets from Syracuse blocks up to the first passage; Phi_X=0 for never-passing orbits. Clip at H_X=X (log X)^(epsilon0/2). Define C_X as the harmonic/logarithmic average of min(Phi_X(w^flat),H_X).

## New claim

**C_X=gamma0 X+o(X)** as X tends to infinity.

- The lower bound is the argument of Theorem 2, Step 3, **applied to the aggregate over all residue classes before clipping** (summing separately clipped variables is not valid).
- For the upper bound, on the same high-probability coupling event, every real block point counted by Phi_X corresponds to an ideal geometric-walk point in a window enlarged by O(X^-1/2) in log coordinates. We **enlarge** the model count by removing the stopping rule altogether.
- The full ideal-model count has an *exact mean identity* via renewal count V(t)=gamma0 t+O(1). Its expectation is gamma0 X+O(X/log X+X^(1-beta)+X^(1/2)).
- On the exceptional event the clipped functional is at most H_X, so its contribution is O(H_X X^-eta)=o(X).
- Lemma 2 transports the sampled-window estimate to the logarithmic mean with error O(H_X(log X)^-epsilon0)=o(X).

## Sharper obstacle decomposition

Define the unclipped mean F_X, the residual R_X=S_X-F_X, and the **clipping tail** T_X=F_X-C_X>=0. Then

    S_X=gamma0 X+o(X)+T_X+R_X.

Both tails are nonnegative; the conjectural potential-mean upper bound is equivalent to **T_X=o(X) and R_X=o(X)**. We no longer need to prove an upper calibration of the clipped first-passage statistic — it is provided by the proposition. The principal unknowns are uniform integrability of Phi_X (to remove clipping) and mass from subsequent or excluded orbit portions.

## Dependence / caveats

1. The underlying general GGM first-passage estimates, including their uniformity in X-dependent window parameters, were **not** re-proved. The exact citation and all quantifiers should be checked before submission.
2. The previous proof of Lemma 4 and its strengthened visit-count bounds are treated as hypotheses. They have not received an independent peer review.
3. The original residual decomposition is a research candidate; its hypotheses are explicitly repeated.
4. The theorem does not exclude unknown cycles or unbounded orbits, and does not prove the full Syracuse-profile conjecture.
5. The corollary is a conditional structural reduction, not evidence that the two tails vanish.

## Next concrete tasks

(a) Prove a bound for the clipping tail, e.g. find p>1 such that suitable normalized first-passage moments are uniformly bounded in X (or control exceptional trajectories directly). (b) Analyze the residual R_X, beginning with post-first-passage returns from below X1 and the omitted target range a<=X^(1-beta). (c) Check GGM uniformities and re-audit Step 3, particularly at the terminal block j=tau**.

### Quantitative sufficient condition for the clipping tail

If for any theta>0 we could prove the uniform-in-X logarithmic moment bound

    limsup_{N->infty} (1/log N) sum_{w<N} (Phi_X(w^flat)/X)^(1+theta)/w <= K,

then the elementary inequality `(t-H)_+ <= t^(1+theta)/H^theta` gives

    T_X <= K X (log X)^(-epsilon0*theta/2) = o(X).

This is the first explicit standalone lemma target for the next proof session. It is NOT established by the clipped theorem, and it does not control R_X.
