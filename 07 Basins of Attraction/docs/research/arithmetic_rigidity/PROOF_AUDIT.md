# Independent proof audit — arithmetic rigidity of late returns

2026-11-16. This file is separate from `docs/main.tex`.

## Exact conclusions

1. For an accelerated `T_{3,c}` orbit staying strictly above cutoff `a`, the logarithmic change is the parity count times `log(3)` minus step count times `log(2)`, with explicit `c/(3n)` corrections. This holds at **all** finite times.
2. Returns to the same dyadic shell enforce parity count within `log(2)/log(3) + epsilon_a*t` of `(log(2)/log(3))*t`.
3. The parity vector is a bijective encoding of integers modulo `2^t` for odd `c`; binomial Chernoff and the exact maximal residue-class concentration give the counting upper bound. This portion is classical and **not claimed novel**.
4. Any distribution supported on M integers necessarily has parity cylinder concentration at least `2^t/M`; thus uniform Bernoulli control solely from the **initial stopped state** is impossible at `t >> log(M)`.
5. A stopped-law survival probability is bounded by parity-strip local time before a chosen horizon plus the probability of an excursion surviving beyond it. **Both terms still need actual-dynamics estimates**.

## Limits / critical qualifications

- The note does **not** prove polynomial decay of returns, `T_X=o(X)`, or the Collatz conjecture.
- The long-time *pathwise* estimate must not be mistaken for long-time *randomness* of parity bits.
- Parameters `c` odd >= -1 and `a>=max(1,2|c|)` ensure positivity and the error bound.
- Counting is for accelerated steps; use the odd-to-odd Syracuse map only after translating each odd step plus subsequent even steps.
- The earlier dyadic block-leakage equivalence does not establish its uniform block contraction assumption.
- C++ probe uses high-altitude **finite sampled bands**, *not* true logarithmic limiting entrance law. It records censored orbits separately.
- A user- or reviewer-supplied independent audit is still needed before referencing new propositions in the manuscript.

## Suggested next work

Study the **actual first-future-entry law after survival conditioning**. Find a quantitative transport statement that repairs conditional entropy after each renewal, and independently control excursions surviving past a mesoscopic horizon. Parity equidistribution of a single starting residue class cannot do this.
