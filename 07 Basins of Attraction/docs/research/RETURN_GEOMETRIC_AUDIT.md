# Local shell returns: rigorous scope and next proof task

## Certified already (reported by the user)

The previous `Clipping.lean` workflow is green. This does NOT certify any theorem about the return distribution or logarithmic densities.

## Exact finite result

For a trajectory with `N` recorded visits to a shell, every later visit can be paired with each preceding visit:

    pairCount(N) = 0 + 1 + ... + (N-1)
    N^2 = N + 2*pairCount(N)

`ReturnPairs.lean` formalizes this finite arithmetic, and requires its OWN green kernel check before it can be labeled certified.

## New conditional theorem (proven on paper, hypothesis OPEN)

Let `p_X,j(k)` denote the logarithmic limiting probability of at least `k` visits before first passage. Assume the logarithmic densities of these threshold events exist. If, **uniformly** in sufficiently large X, in every shell j and every k>=1, there are constants b>=1 and 0<=rho<1 such that

    p_X,j(k+b) <= rho * p_X,j(k),

then

    E_log[N_X,j^2] <= b^2*(1+rho)/(1-rho)^2.

Combined with the paper's shell-moment criterion, this yields

    T_X <= 4*b^2*(1+rho)/(1-rho)^2 * X*(log X)^(-eps0/2) = o(X).

Proof: E[N^2] = sum_{k>=1}(2k-1) P(N>=k); divide indices into length-b blocks, and sum the geometric series.

**Important:** Neither the block-geometric bound nor the required limiting logarithmic distributions are established for actual orbits. No claim that T_X=o(X) is proved without these hypotheses.

## Key analytic obstacle

The real Syracuse map is deterministic. Its *conditional law of the next shell return* is the push-forward of the **induced entrance distribution**, not an independent or memoryless restart. A uniform geometric bound requires controlling this law after arbitrarily many earlier visits, including rare orbits with many returns. A model-law estimate alone does not transfer through an exceptional set whose weighted mass is uncontrolled.

The block formulation permits a bounded number of short correlated returns before the contraction kicks in. An averaged bound on the number of later returns per hit is another possible (and weaker) research target.

## Empirical tests

`C/tests/shell_return_probe.py` samples odd starting values reproducibly, counts visits and chronologically ordered return pairs, and reports the empirical ratios `P_hat(N>=k+b)/P_hat(N>=k)`.

These are finite-window statistics only; the random number generator does not create fresh randomness within a Collatz orbit. Finite samples cannot certify uniform tails or logarithmic limiting probabilities. The script reports unresolved trajectories rather than silently declaring them successes.

## Next steps

1. Confirm `ReturnPairs.lean` in CI; update import and axiom listing in the repo root. Do not claim kernel validation before green CI.
2. Inspect the induced distribution of entry states to dyadic shells, separately for first and subsequent visits.
3. Test finite-window block survival ratios for several X, c, and starting ranges; reproduce with alternative sampling methods to detect residue biases.
4. Seek a quantitative domination/transport lemma from the logarithmic induced entrance measures to the renewal-model return kernel. This is the hard open step.
5. Only after a full mathematical proof, seek a Lean formalization of the relevant analytic transfer and promote it into main.tex. The exception mass and R_X remain independent open issues.

## Genuine model-only result (proved mathematically, not Lean-checked)

For independent valuations P(g=k)=2^(-k), define xi=g log 2-log 3 and S_(n+1)=S_n+xi_(n+1). For any fixed interval of logarithmic width log 2:

- With probability P(g>=4)=1/8, the first step after a visit places the process a distance at least delta=log(8/3) above the interval.
- E exp(-xi/2)=sqrt(3)/(2^(3/2)-1)<1. The exponential process is therefore a nonnegative supermartingale.
- Its maximal inequality bounds the probability of ever returning below the interval's upper endpoint by exp(-delta/2)=sqrt(3/8).
- By the strong Markov property, the conditional chance of **never** revisiting after any visit is at least

      p0 = (1/8)*(1-sqrt(3/8)) ≈ 0.04845345.

Hence P(N_I>=k+1) <= (1-p0)P(N_I>=k) and E N_I^2 <= (2-p0)/p0^2 < 832, uniformly in the location of I and the initial state. This gives a genuine geometric tail in the **iid renewal model only**.

The finite proof uses Ville's inequality for nonnegative supermartingales and the strong Markov property. The same logic does not transfer to an actual Collatz trajectory merely because short valuation strings are approximately iid.
