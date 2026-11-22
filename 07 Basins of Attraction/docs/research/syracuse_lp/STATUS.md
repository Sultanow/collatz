# L^p regularity and Fourier decay of the 3-adic Syracuse law: status (2026-11-20)

Two tracks on the Syracuse law itself, the reference object of Theorem 3, Remark 12(d) and Table 13.
Nothing here enters the paper yet.

## Track 1: critical L^p exponent (note `syracuse_lp_note.tex`/`.pdf`)

Proved in the note:

- **Kahane–Peyrière inequality.** `H_p(n) <= rho(p) H_p(n-1) + E_n^{p/2}` for `1 < p <= 2`, with
  `rho(p) = 3^{p-1}/(2^p - 1) < 1` on `(1, 2)` and `E_n = H_2(n) - H_2(n-1)`. This is the energy of
  the unit frequencies, `sum over units xi of |chi_n(xi)|^2`.
- **Conditional theorem.** If `sup_n E_n < infinity`, the limiting density is in `L^p` for every
  `p < 2`. Together with `H_2 -> infinity` (`transfer/`, Remark 12(d)), 2 is then the critical exponent.
- **Exact identity.** `E_n = 2 sum_{d even >= 2} 2^{-d} R_{n-1}(g_d)`, where
  `R(h) = mean f(y) f(h(y))` and `g_d(y) = 2^d y + (2^d - 1)/3`.
- **Twisted-collision recursion.** An exact recursion for `R(h)` over all affine isometries `h`.
- **Singular set.** `R_n(y -> 2^e y) >= 2^{-|e|} H_2(n-1) -> infinity`. This is the overlap
  principle in correlation form.

Open: **Conjecture 7**, `sup E_n < infinity`. A stronger form is the decorrelation **Conjecture 8**,
`R(h) <= A + B delta(h)`, where `delta` is the weighted 3-adic distance to the singular set. The note
sketches a route via a no-resonance estimate for the weighted affine walk.

Numerics (exact, n <= 16, `results_local/`):

- `E_n`: 0.4642 (n=4), 0.4659 (8), 0.4689 (12), 0.4714 (16). The increments fall from 8e-4 to 5e-4.
- The increments of `H_p` shrink geometrically for `p = 1.25, 1.5, 1.75` (factors 0.90, 0.92, 0.96).
- `R(g_d)` is stable in `n` and grows like 0.47 per unit of `v_3(d/2)`.
- Tail of `f_16`: `Haar{f > M}` falls like `M^{-2}` (local exponents 2.0 and 2.3 for `M = 10..100`).

## Track 2: decay of the Fourier sup (Tao, Prop. 1.17 and the remark after it)

Tao proves `M_n = max_unit |chi_n| << n^{-A}` and says the bound might improve "perhaps to as far as
O(exp(-cn))". Toguchi (preprint, May 2026) computed up to n = 21: the maximiser is `xi = 2^k` with
`k ~ 4(n-1)/3`, and a power law `n^{-2.1}` fits better than an exponential. Job B pushes the
maximising family to n = 80 by exact Monte Carlo.

- **Validation.** Against the exact values for n = 6..15: 90 cells, all within 1.71 standard errors
  (`results_local/mc_validation.csv`).
- **Rough first run.** 2e6 samples (`results_local/mc_rough_2e6.csv`): too noisy beyond n ~ 28. Up to
  there the decay looks more like `n^{-2}` than `0.89^n`. Not a result yet.

## Cluster runs (std partition; submit from this folder)

    sbatch slurm/slp_exact.sbatch          # job A: exact law n <= 20, about 2-3 h, about 210 GB
    sbatch slurm/slp_fourier_mc.sbatch     # job B: 4 x 2.5e10 samples, n = 16..80, 2-3 h per task
    # when both are done (login node is fine):
    python3 mc_merge.py results_mc results_mc/mc_task*.csv --exact results_exact/fourier.csv

What to send back or check in:
- `results_exact/*.csv`;
- `results_mc/mc_task*.csv`, `mc_merged.csv`, `mc_summary.txt`;
- the two job logs from `/home/esul01/data/outputs/logs/`.

Job A writes after every level, so partial results are usable if the job is cut off.

## Files

- `syracuse_lp.py`: exact law and all statistics (`selftest`, `exact`)
- `syrac_fourier_mc.c`: Monte Carlo of `chi_n(2^k)`
- `mc_merge.py`: merge and decay fits
- `slurm/`: the two job scripts
- `syracuse_lp_note.tex`/`.pdf`: the note
- `results_local/`: exact results for n <= 16, MC validation and rough run
