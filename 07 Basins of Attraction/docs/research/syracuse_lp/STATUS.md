# L^p regularity and Fourier decay of the 3-adic Syracuse law: status (2026-11-23)

Two tracks on the Syracuse law itself, the reference object of Theorem 3, Remark 12(d) and Table 13.
In the paper since draft v4i: Appendix F (Lemmas 8, 9, Propositions 16, 17, Corollary 10, Conjecture 3; Runs L and M), a pointer at the end of Remark 12(d) and Outlook item (7). Draft v4j adds the logarithmic chart (Lemmas 10, 11, Propositions 18, 19, Conjecture 4, Run N, Table 17). Note: `syracuse_lp_note.tex`/`.pdf` (cluster runs and Section 8, logarithmic chart).

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

Open: Conjecture 7 (`sup E_n < infinity`) and the decorrelation conjecture (corrected form: Conjecture 12 of the note, Conjecture 4 of the paper; see Track 1b).

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

## Track 1b: first attack on the bounded energy (logarithmic chart, note Section 8)

No proof of `sup E_n < infinity`, but the following is proved (note Lemmas 8, 13, Propositions 9, 11;
paper Lemmas 10, 11, Propositions 18, 19):

- **Logarithmic chart:** `Lambda(x) = log_4(1 + 3x)` is an isometry of Z_3 with `Lambda o g_d = Lambda + d/2`.
  The law of `L = Lambda(S)` is an iterated function system of two nonlinear 1/3-contractions
  `phi_c(z) = log_4(1 + 3 2^-c 4^z)` (c = 1, 2 with weights 2/3, 1/3), smeared by integer translations
  (`P(j) = (3/4) 4^-j`). In the discrete log: `X_n = log_2(1 + 3 2^{X_{n-1}}) - a_n`.
- **Spectral form:** `E_{n+1} = sum_{k != 0} 4^-|k| D_n(k) = sum_eta kappa(eta/3^n) |nu_hat_n(eta)|^2`, with
  `D_n(k) = R_n(g_2k)` and `kappa(x) = (8 cos 2 pi x - 2)/(17 - 8 cos 2 pi x) = (5/3)|gamma_hat|^2 - 1` (mean zero:
  the L^2 similarity dimension is exactly 1). So `E_{n+1} - E_n = Q_n`, the kappa-weighted unit energy of level n.
- **Singular set:** the twisted recursion is `h -> psi_b h psi_a^-1`. Its singular set (maps from which the identity is
  reached with positive weight) is the set of intertwiners `Psi_B^-1 Psi_A` of pairs of inverse Syracuse words of the
  same length, with `R_n >= 3^k 2^-(|A|+|B|) H_2(n-k)`. Example: `y -> y - 1/4`. **This refutes the old decorrelation
  conjecture** (distance delta to the maps `2^e y` only): `delta(y - 1/4) = 0` but `R_n -> infinity`.
- **Exact non-resonance:** an intertwiner with integral translation part is a map `2^e y` (2-adic digit argument).
  So no `g_d` (d != 0) and no integer translation is singular. Height bound: a close approach of the walk from `g_d`
  to the identity (precision 3^-r after k steps) needs `max(|A|, |B|) > k + r log_2 3`.
- **Fixed point -1** of `x -> (3x+1)/2`: `f_n(-1) >= (3/2)^n`; `f_n(2x) = 2 f_n(x)` for `x != 2 mod 3`, hence
  `R_n(2y+1) = 2 R_n(g_2)` exactly.
- **Corrected conjecture:** `limsup R_m(h) <= A + B delta(h)` for affine isometries `h(y) = uy + t` with **t in Z**.
  It implies Conjecture 7. Missing piece: the count of close approaches, i.e. equidistribution of the truncated
  Syracuse sums `S(A) = sum_{j<=k} 3^{j-1} 2^{-A_j}` modulo `3^{k+r}` beyond their natural precision `3^k`.

Numerics (`log_coordinates.py 15 results_logchart`, about 30 s, exact for n <= 15; plus job A):

- log-IFS identity to 1e-17, spectral identity to ten digits.
- Angle shares of the unit energy at level 15 agree with the Haar shares to within 0.0012; `Q_15 = 5.2e-4` is the
  residual of kappa-weighted parts up to 5.7e-2.
- Intertwiners grow with slope `w(h) E_n` (w = total weight of the minimal pairs; 0.1875 for `y - 1/4`); controls
  (`y + 1`, `y - 1/5`, `g_2`) converge. No pair of length <= 4 (letters <= 8) represents any `g_d`, d <= 20.
- `f_n(-1) (2/3)^n -> 1.4620`; `-1` is the argmax of f_n for all n <= 15; `max f_20 = 4862 = 1.462 (3/2)^20`.
- `Q_n = (1/2)(R_n(g_2) - R_{n-1}(g_2)) (1 +- 0.03)` for 11 <= n <= 19: the rotation `y -> 2y+1` about -1 carries the
  convergence of E_n. Local ratios of these increments fall from 0.911 (n = 13) to 0.866 (n = 20); fit
  `C n^b theta^n` on 14..20: theta = 0.78, b = 2.1; heuristic rate 3/4 (weight of the branch (1,1)).
  Extrapolated `E_infinity` 0.4746 (fitted model) to 0.4749 (constant factor 0.866).

## Track 1c: roadmap tasks A0-A2 and Run O (local), note Section 8 (end)

Proved (note Lemmas 14, 17, Proposition 15, Corollary 16):

- **Dilation rules (A0):** `R(h) = 2 R(h o m_1)` if `h(1) = 0 mod 3`; `R(m_1 o h) = 2 R(h)` if `h(1), h(2) != 2 mod 3`.
  Hence `R_n(g_2) = R_n(y + 1/2) = R_n(2y+1)/2` and `f_n(-1/2) = f_n(-1)/2`; `y +- 1/2` is not an intertwiner.
  Pilot target: **the law of S' - S has a bounded density at 1/2**; the slow part is the singular pair -1, -1/2.
- **First-hit decomposition (A1):** with the hit weights `w_d(k, r) = 3^k P(|A|-|B| = d mod 2*3^{r-1},
  S(B) = g_d(S(A)) mod 3^{k+r})` (truncated Syracuse sums of independent words of length k) and the identity kernel
  `K = w_0`: `w_d(k, N-k) = sum_{k'} F_d(k', N-k') K(k-k', N-k)`, so
  `R_N(g_d) = F_d(N, 0) [regular part] + sum_{r>=1} F_d(N-r, r) H_2(r) [near-resonance part]`.
- **Bootstrap (weighted form):** with `rho_d(k, r) = w_d(k, r) |Aff(Z/3^r)| / R_k(g_d)` (density of the depth-k
  states at the identity class) and `c_d = (3/2) sum_r sup_k rho_d(k, r) (5/27)^r`:
  `sup_N R_N(g_d) <= max(1, F*) / (1 - c_d)` if `c_d < 1` (v_3(d/2) = 0). Uses only `H_2(r) <= (5/3)^r`.
  The weighted form is needed because of the length congruence: `rho = rho_t * rho_len`, and `rho_len` could grow
  like `L / sqrt(8 pi k)` (L = 2*3^{r-1}); measured: `rho_len <= 1.4` for k + r <= 13. Remaining difficulty: the
  regular part F_d(N, 0).
- **Large deviations (A2):** `w_d(k, r) <= 2 (3/(2-q))^k (q^{-r log_2 3} + q^{|d| - 2*3^{r-1}})`; exponentially small for
  `r >= beta k`, `beta > beta* = 2.7022` (q = 1.6214); `beta** = 6.4188` with the a priori bound on H_2.

Run O, local (`congruence_counts.py`, N <= 13, 4 min; `results_runO_local/`; selftest against brute force):

- d = 4: rho(k, 1) -> 0.999 by k = 12, rho(k, 2) in [0.96, 1.01]; regular 0.338, near-resonance 0.135.
- d = 2: rho(k, 1) = 0.16 ... 0.957 (k = 2..12), slow (factor about 0.74). Regular part decreases 0.694 -> 0.529,
  near-resonance part increases to 0.198; their increments shrink by about 0.72 and 0.76 per level but have
  opposite signs, so the sum R_N(g_2) converges with local factor 0.91: the slow rate of Run L is a near-cancellation.
- rho <= 1.15 for d = 2, 4 (<= 1.57 for d = 6).
- Cluster: `slurm/slp_runO.sbatch` (N <= 17 by default, about 50 min; N = 18 on n12/n13).

## Track 1d: the regular part, class distribution (open)

- Exact: the one-step weights depend only on the class c = (u mod 3, t mod 3): W = 5/3, 4/3, 2/3, 2/3, 4/3, 1/3 for
  (1,0), (2,0), (1,1), (1,2), (2,1), (2,2). So `R_{k+1}/R_k - 1 = sum_c W(c) (nu_k(c) - 1/6)`: R bounded iff the
  drifts have bounded partial products (class distribution -> uniform, summably).
- `class_distribution.py` (k <= 13, `results_classes/`): d = 4 equidistributes fast (|nu - 1/6| <= 1e-3 from k = 10).
  d = 2: excess in class (2,1) (the class of 2y+1) decays by **0.749-0.752** for k = 8..13, the resonance at -1
  seen directly; other classes 0.70-0.77, class (2,0) 0.82-0.86 (small). Weighted deviations up to 1.6e-2 cancel to
  a drift of 1.6e-3 (factor 0.91 per level).
- `ulam_spectrum.py`: the mean-field (Ulam) discretisation on classes mod 3^s has, besides 1, exactly the
  eigenvalues 1/(5 - 4 cos(2 pi j/L)), L = 2*3^{s-1}, of the random walk of the exponent e (diffusive, gap ~ 8 pi^2/L^2),
  and is nilpotent in t. It misses the resonance at -1. Certification (A6) must work on the correlations.
- No closing recursion for the regular part found; its monotonicity is special to d = 2.

## Track 1e: Run O on the cluster (N <= 17), roadmap A3 and decision point E1

Job 169344 (n11, 42 min, 82 processes, 216 GB; `results_runO/`). Agrees with job A to 5e-11 (Cor_N(g_d) and H_2;
the precision of the stored values).

- **d = 2 split:** regular part 0.52937 -> 0.52469 (N = 13..17), increments shrinking faster (local factors
  0.72 -> 0.655); near-resonance part 0.2061, increments factor about 0.77 (slowly rising); sum: 0.883 at N = 17.
- **Slow components at higher precision:** near-resonance terms F(N-r, r) H_2(r) have increment factors 0.70 (r=1),
  0.83 (r=2), 0.88-0.92 (r=3), 0.93-1.0 (r=4); r = 5 still grows; amplitudes ~ 9^{-r}. Diffusive relaxation of the
  length congruence mod 2*3^{r-1} (time scale ~ 9^r). Two-component extrapolation predicts local factors 0.863,
  0.846, 0.832 at N = 18-20; job A has 0.874, 0.868, 0.866. So the final convergence of Cor_N(g_2) and E_n is
  probably slower than geometric; the expectation O(n^B (3/4)^n) in Appendix F of v4j has to be revised (D4).
  Boundedness is not affected (weights 9^{-r}).
- **Length part grows:** d = 4: rho_len up to 1.6 / 3.4 / 5.7 / 7.2 for r = 4..7; rho_t <= 1.004 for k >= 7.
  max rho_4 = 4.96 at (k, r) = (10, 7): the unweighted bootstrap hypothesis fails, the weighted one holds:
  c_2 = 0.33, c_4 = 0.38, c_6 = 0.34 on the computed range (0.31, 0.36, 0.32 with the actual H_2).
- **Regular part:** d = 4 constant 0.33777 (N = 13..17); d = 2 decreasing.
- **E1:** the structure (renewal, large deviations, weighted anti-concentration) is confirmed; the open core is the
  regular part = summable equidistribution of the class distribution, a mixing statement of Tao's type.

## Track 1f: all d and the energy walk (roadmap A7, easy part; note Section 8, end)

`energy_renewal.py` (`run`, `merge`, `scan`, `selftest`; `results_energy/`). In the log chart every g_d is a
translation, so one cyclic autocorrelation per length phase gives the coincidence weights of all d at once.
N <= 13, |d| <= 80, a few minutes on 2 cores. Agrees with Run O to 1e-16 and with E_{k+1} = H_2(k+1) - H_2(k)
to 1e-14; the selftest checks brute-force first hits for d = 2, 6, 18, 54. The raw phases (2 x 10 MB) are not
stored (rerun: `run 13 results_energy --theta-slice I/2`, then `merge`).

- **Start near the identity (exact, Lemma "nearid"):** for d = 2*3^s*m, Cor_N(g_d) = H_2(N) for N <= s; for N > s
  Cor_N(g_d) = H_2(s) - E_{s+1}/2 + (equidistribution defects of the unit energy at levels s+2..N), checked to
  1e-12. Main term 0.67, 1.43, 1.91, 2.37, 2.84 for s = 0..4: this is the "0.47 per unit of v_3(d/2)" of job A.
  Defects at n = 20: -0.24..+0.20.
- **Corollary 11 for all d:** sup over k >= 1 with k + r > s (for N > s the diagonal never enters); bound
  max(H_2(s), F*_d/(1 - c_d)). c_d in [0.31, 0.68] for 37 of the 40 values d <= 80. Exceptions c_18 = 0.82,
  c_62 = 0.98, c_70 = 1.10: spikes at depth k <= 5 (up to rho_18(4,3) = 27), where a small length difference
  satisfies the length congruence and brings the walk near g_{d'} with v_3(d'/2) >= 1 (d = 70: |A|-|B| = -2, d' = 72).
- **Energy walk (Corollary 12):** start at sum_{d != 0} 2^{-|d|} g_d; E_{N+1} = F_E(N,0) + sum_r F_E(N-r,r) H_2(r).
  rho_E <= 0.97 throughout, c_E = 0.317. Regular part F_E(N,0) decreases monotonically 0.4448 (N = 2) -> 0.3409
  (N = 13), increments shrinking by about 0.7; near-resonance part -> 0.1293. Bound on the computed range:
  max(2/3, 0.651) = 2/3, against E_14 = 0.4703. **Conjecture 3 is reduced to c_E < 1 and a bounded regular part
  of the energy walk; no case distinction in v_3(d/2) is needed.**
- **All d at once (`scan 15`):** Cor_N(g_d) = D_N(d/2) depends only on d/2 mod 3^N. For |d| <= 80,
  Cor_N(g_d) < H_2(v_3(d/2)) for N <= 15 (max ratio 0.991 at d = 18, N = 8). Over all d the max ratio exceeds 1 from
  N = 6 on (large |d|), reaches 1.080 at N = 15, increments x0.8 per level (extrapolated about 1.09). So the
  pointwise bound is false, but a uniform bound C H_2(v_3(d/2)) (Conjecture 4 for the g_d) fits the data; it
  would give E_n <= 0.688 C.
- Paper: v4m (Corollary 11 for all d, Lemma 13, Corollary 12, paragraph "Run O, all d").

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
  - Exponential and quasi-polynomial decay could not be told apart from job B alone (settled by B2 below).

Job B2 (window recentred at `round(n log2 3) - 6`, `W = 8`, n = 36..80, 1e11 samples, 4 x 1 h 51 min
on n11; `results_mc2/`):

- **Maximiser:** never at the window edge; `k* - n log2 3` stays in [-7.5, -5.2], so `xi ~ 3^n/64`.
- **Decay:** jobs B (16..35) and B2 (36..80) together give 65 levels with `M >= 20 se`
  (`decay_analysis.py` -> `results_mc2/decay_fits.txt`). Model fits of `log M_n`:

  | model | RMS residual |
  |---|---|
  | exponential | 0.087 |
  | power | 0.226 |
  | quasi-power | 0.138 |
  | stretched exponential (g = 0.71) | 0.015 |
  | **exponential x power `A n^c rho^n`** | **0.0099** (max 0.029) |

  The winning model has `rho = 0.942`, `c = -0.97`. On subranges: rho = 0.9446 / 0.9414 / 0.9449 and
  c = -1.05 / -0.93 / -1.17 for n = 16..50 / 30..80 / 50..80.
- **Conclusion:** numerically `M_n ~ A n^{-1} rho^n` with `rho ~ 0.943`. This supports Tao's suggested
  `O(exp(-cn))` with `c ~ 0.058`, and rules out every fixed power, including Toguchi's `n^{-2.1}`.
- **Local ratio per level:** rises 0.892 -> 0.919 -> 0.930 (n ~ 20, 40, 76), as `rho (1 - 1/n)`
  predicts. The unbiased window energy gives the same ratios.
- **Open:** a proof. Route: transfer operator on the relative frequency `theta = 2^k/3^n`
  (`theta -> 3 theta / 2^a`), see the note.

## Cluster runs (std partition; submit from this folder)

    sbatch slurm/slp_exact.sbatch          # job A (done 2026-11-22)
    sbatch slurm/slp_fourier_mc.sbatch     # job B (done 2026-11-22)
    sbatch slurm/slp_fourier_mc2.sbatch    # job B2 (done 2026-11-22)
    sbatch slurm/slp_runO.sbatch           # Run O (roadmap A3), N <= 17 (done 2026-11-23, job 169344)
    python3 mc_merge.py results_mc2 results_mc2/mc_task*.csv
    python3 decay_analysis.py results_mc/mc_merged.csv results_mc2/mc_merged.csv

`results_mc/mc_summary.txt` was regenerated with the current `mc_merge.py`, which adds the `EDGE` flags
and the column `k* - n log2(3)`. `mc_merged.csv` is identical to the cluster output.

## Files

- `syracuse_lp.py`: exact law and all statistics (`selftest`, `exact`)
- `syrac_fourier_mc.c`: Monte Carlo of `chi_n(2^k)`; optional `SHIFT` centres the window at
  `round(n log2 3) - SHIFT`
- `mc_merge.py`: merges the Monte Carlo runs and fits the decay
- `decay_analysis.py`: decay models on the combined runs (`results_mc2/decay_fits.txt`)
- `congruence_counts.py`: Run O, hit weights w_d(k, r), first-hit decomposition (`run`, `merge`, `selftest`)
- `energy_renewal.py`: Run O for all d and the energy walk, scan over all d (`results_energy/`)
- `class_distribution.py`: class distribution mod 3 of the walk from g_d (`results_classes/`)
- `ulam_spectrum.py`: spectrum of the mean-field (Ulam) discretisation of the walk
- `log_coordinates.py`: logarithmic chart, spectral identity, angle shares, intertwiners, fixed point -1
  (`python3 log_coordinates.py 15 results_logchart`; imports `syracuse_lp.py`)
- `slurm/`: job scripts A, B, B2
- `results_exact/`: job A output (n <= 20)
- `results_mc/`: job B output
- `results_mc2/`: job B2 output and the decay fits
- `results_logchart/`: output of `log_coordinates.py` for n <= 15 (summary, angle shares, D_l(k), intertwiners)
- `results_runO_local/`: Run O for N <= 13 (summary, hit weights, first hits, raw phases)
- `results_energy/`: Run O for all |d| <= 80 and the energy walk (N <= 13), scan over all d (N <= 15)
- `results_runO/`: Run O on the cluster, N <= 17 (job 169344: phases, summary, hit weights, first hits, rho split, log)
- `results_local/`: first local runs. Exact statistics for n <= 16 (superseded by `results_exact/`),
  the Monte Carlo validation against the exact values for n = 6..15 (`mc_validation.csv`, 2e7 samples)
  and a rough 2e6-sample Monte Carlo run (`mc_rough_2e6.csv`)

Run times on n11: job A 1 h 33 min (peak 205 GB); job B 4 x 2 h; job B2 4 x 1 h 51 min.
