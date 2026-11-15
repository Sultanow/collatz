# Exact finite return-potential experiments

All listed JSON files are **finite exhaustive computations**, not asymptotic claims and not Lean certificates. They were produced by `C/tests/return_potential_exact.py` using integer-exact enumeration of the induced first-return graph and integer-exact verification of Green identities. Reported decimal moments are display-only normalizations of exact integer totals.

## Parameters

`dyadic`: `I=(X/2,X]` (integer endpoints), first-passage cutoff `floor(X/4)`; X=10,000, 100,000, 1,000,000.

`narrow`: `I=(floor(X^(199/200)),X]`, cutoff `floor(X^(99/100))`; X=100,000, 300,000.

Maps: c=-1, 1, 5, 13 under `T(n)=n/2` if even and `(3n+c)/2` if odd.

Initial laws: (1) one unit of first-entry mass on each state of I (artificial uniform shell entry); (2) actual first-hit states induced by uniformly spaced outside starts in `(X,2X]` with stride 31 (a *deterministic finite sample*, not a random asymptotic entrance law).

The main integer checks, including per-node `nu-Qnu=q1` and `sigma-Qsigma=nu`, and two methods of computing the squared visit total, passed in all 20 reported cases. Independent raw accelerated-trajectory spot checks were also performed and counted in each result file. In particular, no output assumes a uniform tail bound in X.

## Dyadic second moments for uniform shell entry

| c | X=10^4 | X=10^5 | X=10^6 |
|---:|---:|---:|---:|
| -1 | 19.61020 | 18.98152 | 19.224834 |
| 1 | 19.18880 | 18.67078 | 18.947698 |
| 5 | 19.27880 | 18.62690 | 19.033318 |
| 13 | 19.68160 | 18.52676 | 18.923398 |

The smoothness is evidence about these **small finite artificial laws**, not a proof of `sup_{X,j} E_log N^2 < infinity` for the actual first-passage setting.

## Pointwise obstruction

For actual first entries from outside at X=100,000, the numbers of visited shell states with positive occupation but *zero first-entry mass* were:

| c | occupied-but-not-first-entry states |
|---:|---:|
| -1 | 5,384 |
| 1 | 4,967 |
| 5 | 4,477 |
| 13 | 4,538 |

On each such state, `(Qnu)(y)=nu(y)` because `nu-Qnu=q1` and `q1(y)=0`. Thus this exact occupation potential cannot satisfy `Qnu <= rho nu` for any `rho<1`. That is a finite mathematical counterexample to the **naive weight choice**, not to other potential-inspired weights or to the geometric tail conjecture.

## Discounted reference

For fixed `0<rho<1`, the discounted potential `nu_rho = sum_j rho^(-j) Q^j q1` satisfies the strict sub-eigenmeasure inequality identically on each resolved finite return graph. The reported `discounted_occupation_mass_per_start` at rho=9/10 and rho=19/20 is numerical bookkeeping of the **exact finite sums**. Proving their uniform bound over all X would require a stronger exponential return-tail result.
