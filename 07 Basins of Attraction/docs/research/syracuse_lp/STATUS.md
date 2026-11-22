# L^p regularity and Fourier decay of the 3-adic Syracuse law: status (2026-11-22)

Two tracks on the Syracuse law itself, the reference object of Theorem 3, Remark 12(d) and Table 13.
Nothing here enters the paper yet. Note: `syracuse_lp_note.tex`/`.pdf` (updated with the cluster runs).

## Track 1: critical L^p exponent

Proved in the note:

- **Kahane–Peyrière inequality:** `H_p(n) <= rho(p) H_p(n-1) + E_n^{p/2}` for `1 < p <= 2`, where
  `rho(p) = 3^{p-1}/(2^p - 1) < 1` on `(1, 2)` and `E_n = H_2(n) - H_2(n-1)` is the energy of the unit
  frequencies.
- **Conditional theorem:** `sup E_n < infinity` implies that the limiting density is in `L^p` for every
  `p < 2`. With `H_2 -> infinity` (`transfer/`, Remark 12(d)), 2 is then the critical exponent.
- **Exact identity** `E_n = 2 sum_{d even >= 2} 2^{-d} R_{n-1}(g_d)`.
- **Twisted-collision recursion** for the correlations.
- **Singular set:** `y -> 2^e y`, where the correlations diverge.

Open: Conjecture 7 (`sup E_n < infinity`) and the decorrelation Conjecture 8.

Cluster job A (exact, n <= 20; 1 h 33 min, 205 GB on n11; `results_exact/`):

- **`E_n`:** 0.47135 (n=16), 0.47223 (18), 0.47291 (20). The increments shrink by about 0.87 per
  level, so the extrapolated limit is 0.4751. E_n is numerically bounded.
- **`H_p`:** the increments shrink at n = 20 by 0.909 (p=1.25), 0.930 (p=1.5), 0.959 (p=1.75); `H_2`
  grows by `E_n` per level.
- **Kahane–Peyrière bound** holds at every level; for p = 1.5 the slack is at least 0.42.
- **Exact identity for `E_n`** holds to all printed digits.
- **`R(g_d)`:** identical at n = 8, 16 and 20 to three decimals; grows about 0.47 per unit of
  `v_3(d/2)`.
- **Linear maps `2^e y`:** grow by `2^{-e} E_n` per level, as predicted; with a translation added they
  stay constant.
- **Tail of `f_20`:** local exponents 1.9, 2.2, 2.5 for M = 10..300. They steepen towards the cut-off and
  decrease with n at fixed M, consistent with `M^{-2}`.

## Track 2: decay of the Fourier sup (Tao, Prop. 1.17: "perhaps to as far as O(exp(-cn))")

Job B (Monte Carlo of `chi_n(2^k)`, n = 16..80, 1e11 samples, standard error 3.2e-6; `results_mc/`):

- **Validation:** agrees with the exact values for n = 16..20 within 1.4 standard errors.
- **Maximiser:** `k* - n log2 3` stays in [-6.6, -5.3] for all edge-free n <= 42, so `xi ~ 3^n/64`.
  Toguchi's `k ~ 4(n-1)/3` holds only for small n. The window of job B was centred at `4(n-1)/3`, so
  from n = 41 on the maximum sits at the window edge. Those values are lower bounds only; `mc_merge.py`
  now flags them `EDGE` and leaves them out of the fits.
- **Decay on the 26 edge-free levels 16 <= n <= 42:** `M_n` falls from 1.44e-2 to 1.16e-3.
  - The local power exponent steepens from -2.2 to -3.1. This is faster than any fixed power, and
    Toguchi's `n^{-2.1}` was pre-asymptotic.
  - The ratio per level rises from 0.892 to 0.917.
  - Exponential and quasi-polynomial decay cannot be told apart yet.

Next: **job B2** (`slurm/slp_fourier_mc2.sbatch`) with the window centred at `round(n log2 3) - 6`,
`W = 8`, `n = 36..80`, 4 x 2.5e10 samples (about 2 h per task). If the ratio levels off below 1, that
supports exponential decay; if it keeps rising, decay is subexponential.

## Cluster runs (std partition; submit from this folder)

    sbatch slurm/slp_exact.sbatch          # job A (done 2026-11-22)
    sbatch slurm/slp_fourier_mc.sbatch     # job B (done 2026-11-22)
    sbatch slurm/slp_fourier_mc2.sbatch    # job B2 (next)
    python3 mc_merge.py results_mc2 results_mc2/mc_task*.csv

`results_mc/mc_summary.txt` was regenerated with the current `mc_merge.py`, which adds the `EDGE` flags
and the column `k* - n log2(3)`. `mc_merged.csv` is identical to the cluster output.

## Files

- `syracuse_lp.py`: exact law and all statistics (`selftest`, `exact`)
- `syrac_fourier_mc.c`: Monte Carlo of `chi_n(2^k)`; optional `SHIFT` centres the window at
  `round(n log2 3) - SHIFT`
- `mc_merge.py`: merges the Monte Carlo runs and fits the decay
- `slurm/`: job scripts A, B, B2
- `results_exact/`: job A output (n <= 20)
- `results_mc/`: job B output
- `logs/`: job logs
- `results_local/mc_validation.csv`: Monte Carlo against the exact values for n = 6..15 (local, 2e7 samples)
