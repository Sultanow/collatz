# Return-count diagnostics, November 2026

Script: `../../C/tests/shell_return_probe.py`, importing the existing `shell_visit_probe.py`.
All outputs are finite-window sampled estimates, not logarithmic limits and not a proof.

## Parameters

- `beta=1/200` (from the paper), `b=2`, random seed `20261115`.
- Short scale: X=100000; random odd starts in [2000000,2400000); 3000 draws each for c=-1,+1.
- Large scale: X=10^60; random odd starts in [2*10^60,2.4*10^60); 1800 draws each for c=-1,+1,+5,+13.
- Weighting within resolved samples is proportional to `1/m`.
- All reported sampled trajectories hit the first-passage threshold within 15,000 blocks (large scale) or 20,000 blocks (small scale); this is not a general convergence claim.
- These particular configurations have only the nonempty dyadic shell `j=0`.

## Example output (large scale)

| c | E_sample[N] | E_sample[N^2] | maximum observed N | P_sample(N>=3)/P_sample(N>=1) |
|---:|---:|---:|---:|---:|
| -1 | 3.411 | 19.619 | 25 | 0.505 |
| +1 | 3.298 | 18.396 | 20 | 0.489 |
| +5 | 3.355 | 19.336 | 28 | 0.489 |
| +13 | 3.340 | 18.783 | 22 | 0.493 |

The last column is one estimated block-2 survival ratio, not a rigorous uniform bound. Higher-k ratios can be volatile because the number of surviving sampled records becomes small. Variation across X, j, c, and initial intervals must be studied; even extensive numerics cannot establish the required sup over all k and X.

## Run reproducibly

```bash
python C/tests/return_pair_identity.py
python C/tests/shell_return_probe.py --X 100000 --c -1 \
 --u 2000000 --v 2400000 --samples 3000 --seed 20261115 --block 2
```

For 10^60 use the full integer values in `local_return_c-1_X1e60.json` and set `--samples 1800`.
