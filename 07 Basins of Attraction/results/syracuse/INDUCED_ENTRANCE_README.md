# Induced entrance diagnostic – reproducibility summary

**Not a proof of an asymptotic density or uniform return bound.**

All runs use `X=10^60`, odd starting values in the exactly represented
interval `[2X,12X/5)`, 650 independent random samples per map, seed 261115,
`b=2`, finite horizon `H=20` accelerated steps, and a first-passage threshold
`floor(X^0.99)`. Each sample is `1/m`-weighted in the reported conditional
statistics (renormalised over the resolved sample). All 2,600 sampled starts
reached the threshold, but this is finite numerical evidence only.

| c | Resolved | First shell-visit sample count | Finite-H certified escape share for k=1 | P(N>=3 \| N>=1) |
|---:|---:|---:|---:|---:|
| -1 | 650 | 649 | 0.4918 | 0.5021 |
| 1 | 650 | 649 | 0.5295 | 0.4674 |
| 5 | 650 | 649 | 0.4906 | 0.4999 |
| 13 | 650 | 649 | 0.4770 | 0.5198 |

The escape event is a **deterministically checkable sufficient condition**
for fewer than `b` subsequent shell visits before first passage. The reported
fraction is conditional on the finite *sampled* first-entry law, not the
limiting logarithmic entrance law. The numbers for higher k and other H are
in the JSON files. No supremum over k or X is bounded by these measurements.

The script also performs pointwise identity checks against the full
accelerated orbit and verifies that every finite-H escape certificate implies
failure to reach k+b shell visits in each sampled resolved trajectory.
The algebraic finite-support TV inequality is exact and does not depend
on the empirical sample.

Reproduce:

```bash
cd '07 Basins of Attraction/C/tests'
python induced_entrance_probe.py --X 1000000000000000000000000000000000000000000000000000000000000 --c 1 --samples 650 --H 20 --b 2
```

The independent renewal model and the actual deterministic Collatz map are
not interchangeable. `EntranceTransport.lean` is untested until GitHub CI
passes. Only the previously green `Clipping.lean` and `ReturnPairs.lean`
modules are described as kernel checked in this package.

### Exact bounded return-graph enumeration

Reproduce with `C/tests/finite_return_graph.py --X 100000 --c 1`.
All integers in `(floor(X^0.995),X]` are enumerated, and the output marks
whether any state remained unresolved. The maximum harmonic sub-eigenmeasure
ratio > 1 shows that naive weights `1/y` do not give a pointwise strict
contraction even on this finite example. This is not an asymptotic result.

| c | States checked | Returns before passage | Maximum visits | Max `(Qnu)(z)/nu(z)` for `nu(y)=1/y` |
|---:|---:|---:|---:|---:|
| -1 | 5,594 | 47 | 3 | 1.91136129 |
| 1 | 5,594 | 46 | 2 | 1.02749681 |
| 5 | 5,594 | 42 | 2 | 1.02763453 |
| 13 | 5,594 | 46 | 3 | 2.04178340 |

These 4 outputs are in `finite_return_graph_c*_X100000.json`.
