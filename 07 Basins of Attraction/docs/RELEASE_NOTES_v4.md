# Release notes: version 4 (Preprints.org, November 2026)

**Same Shadows, Different Forests: Cycles and Basin Densities of Collatz-Type Maps**
Eldar Sultanow · Preprints.org [202610.2552](https://www.preprints.org/manuscript/202610.2552), version 4 · not peer-reviewed

Version 4 is the draft v4r of this repository (`docs/main.tex`, `docs/main.pdf`). It has 85 pages, against 71 pages
for version 3 (November 2026). The title, the main theorems of version 3 and their proofs are unchanged. The
detailed log of the intermediate drafts v4a–v4r is in [`CHANGES_v5.md`](CHANGES_v5.md).

## In short

- The exhaustive cycle census now reaches parity-word length 66: there are 1296 primitive cycles of the maps
  3x+c with |c| ≤ 1000, found with a meet-in-the-middle search. Before, it reached word length 40 with 1097 cycles.
- A birthday factor corrects the cycle-count heuristic for long words. Fitted on word lengths up to 40, it predicts
  the counts for lengths 41–66 out of sample.
- A new appendix treats the 3-adic Syracuse law. It shows L^p regularity for every p < 2 under a bounded
  unit-frequency energy and reports exact and Monte Carlo computations. It also reduces the bounded energy to a
  mixing statement and explains why a certificate at finite precision cannot replace that statement.
- The return term of the upper bound for the predecessor potential is now dominated by the potential itself. Both
  tails therefore rest on one uniform-integrability hypothesis.
- Appendices D and F were reduced to their lines of argument for readability. The side results moved verbatim to
  research notes in this repository, and the reproduction table moved to the README.

## New results

### Cycles (Sections 4 and 5)

- **Census to word length 66** (Propositions 2 and 3, Run P):
  - There are 1296 primitive cycles of 3x+c₀ with |c₀| ≤ 1000 (947 with c₀ > 0, 349 with c₀ < 0, 257 distinct |c₀|).
  - Every cycle of 3x+c with |c| ≤ 1000 and word length at most 66 is an odd multiple of one of them.
  - The complete cycle lists of 3x±1, 3x+5 and 3x+13 hold up to word length 66.
  - The search split each parity word into halves and matched the halves modulo |D|/L. It covered all 1391 types
    with 41 ≤ l ≤ 66 in 46 core-hours, and every match was checked in exact arithmetic.
  - The type (65,41), at the convergent 65/41 of log₂3, has 2 cycles of 3x+29 and 7 of 3x+551. The heuristic
    expects 8.0 for this type.
- **Published tables:** the cycle lists of Belaga, Mignotte and Morley (OEIS A226607) agree exactly with the census
  for all 947 cycles with at most 66 elements. For 91 of the maps, every known cycle is longer than 66.
- **Lemma 2 (half-word collisions):** for even types, P = 2^{l/2}+3^{k/2} divides z₃ exactly when the two half-word
  values collide modulo P. This explains the deficit of the heuristic for even types with D > 0: the zero slot is
  empty, and the densities at 2 and 3 are not uniform. For D < 0 it gives counts that do not depend on the length,
  which is an S-unit phenomenon.
- **Birthday factor** (equation (10)):
  - Each divisor e gets the factor exp(−k²|2^{l/k}−3|/(αe)), because the k odd members of a cycle must be distinct.
  - With α = 6 fitted on l ≤ 40, the corrected heuristic predicts 163 cycles for 41 ≤ l ≤ 66; 146 are observed and
    the uncorrected heuristic gives 206.
  - Over all l ≤ 66 the corrected heuristic gives 1296.7 against 1296 observed.
  - Problem 1 and Figure 1 are updated.

### Basin densities and the predecessor potential (Section 6, Appendix D)

- **Lemma 7 (return term):** R_X = S_Y + J_X, and the upper bound for the mean of a·h(a) holds exactly when
  T_X + J_X = o(X). If an orbit of positive potential is unbounded, the return term grows linearly.
- **Proposition 12 (returns dominated by the potential):** the window part J_X is bounded by the potential at the
  points where the excursions start, and on the dangerous classes. If the potential of the odd targets is uniformly
  integrable, both tails vanish and the upper bound follows. This holds conditionally on the coupling estimates of
  Theorem 3.
- **Remark 3 (averages, not maxima)** covers three points:
  - Pointwise bounds are too strong, because the maxima of a·h(a) grow polynomially.
  - An inverse ray of 3x+1 shows that uniform integrability does not follow from the structural identities.
  - Moments of order p ≥ 2 diverge, so moment routes need 1 < p < 2.
- **Related work:** Inselmann (mean total stopping time) and Shaik (polylogarithmic descent in natural density) are
  now discussed.

### New Appendix F: the 3-adic Syracuse law

- **Lemma 11 (Kahane–Peyrière inequality) and Corollary 8:** the Syracuse profile lies in L^p for every p < 2 if the
  unit-frequency energy E_n stays bounded (Conjecture 3). The exponent 2 is critical. An exact second-moment identity
  (Proposition 13) writes E_n through twisted self-correlations.
- **Run L:** exact laws up to n = 20, with E_n → about 0.475.
- **Run M:** Monte Carlo up to n = 80. The largest Fourier coefficient behaves like n⁻¹·0.943ⁿ, which is consistent
  with Tao's suggested exponential rate. Every power law is excluded numerically.
- **Logarithmic chart and singular set** (Lemma 12, Propositions 15 and 16, Run N):
  - In the logarithmic chart the correlation maps become translations, and the Syracuse law is an iterated function
    system of two nonlinear 1/3-contractions.
  - The singular set of the correlation recursion consists of the intertwiners of inverse Syracuse words. The maps
    that enter E_n avoid it.
  - The slowest part of the convergence sits at the fixed point −1.
- **First-hit decomposition** (Proposition 17, Corollary 9, Run O on the cluster up to N = 17): bounded energy follows
  from two conditions. The first is an anti-concentration at the identity, which holds numerically with margin. The
  second is a bounded regular part, which is a mixing statement.
- **What a proof needs:**
  - The intertwiners are dense; this is proved by an explicit construction. So no certificate at a fixed finite
    precision can bound the correlations.
  - Summable rates suffice, but they are needed for pairs and pointwise. The missing piece is a pair version of Tao's
    fine-scale mixing in which the spikes at −1 are removed.

## Corrections and updates to version 3

- **Tao's paper** (Forum Math. Pi 10 (2022), e12) is now cited throughout with the equation numbers of the published
  version. Two citations in Appendix E had used the arXiv numbering: the no-hit bound is (1.19) and the stabilisation
  of first passages is (1.20).
- **Section 6 of Tao's published version** contains a numerical slip in its proof, which L. Mazur reported on Tao's
  blog in July 2026. arXiv version 7 (16 July 2026) corrects the proof and leaves the statements unchanged. This paper
  uses only the statements, and the corresponding proof of Gonçalves, Greenfeld and Madrid already uses the uncritical
  window.
- **Preprint status:**
  - Allikvere is cited in version 2 (22 July 2026).
  - The other preprints are unchanged.
  - None of them has appeared in a journal yet, so the statements that rest on them stay marked as such (Corollaries
    4 and 5, Theorem 2, Lemma 9).

## Structure and readability

- **Appendix D** now follows one chain: calibration (Proposition 8), the two tails (Corollary 7), tightness
  (Proposition 9, Run I), first excess (Proposition 10), arithmetic rigidity (Lemma 8) and domination by the potential
  (Propositions 11 and 12).
  - Moved verbatim to the research note [`docs/research/tail_programme`](research/tail_programme), with references
    to this version:
    - the moment criteria and Corollary 8 of version 3;
    - the transfer problem for the early returns;
    - the two-level return certificate and the fixed-modulus obstruction (Propositions 10 and 11 of version 3, both
      formalised in Lean 4);
    - Problem 3 and Proposition 13 of version 3.
- **Appendix F** keeps its core. The dilation rules at −1, the bound for single correlation maps g_d, the start near
  the identity and the large deviations are in [`docs/research/syracuse_lp`](research/syracuse_lp).
- **Remarks:** remarks that are not referenced became plain text. Two load-bearing remarks became lemmas (Lemmas 7
  and 8).
- **Tables:** the reproduction table of version 3 (Table 12) was replaced by the reproduction map in the
  [README](../README.md). The runs table (Table 11) now lists Runs A–P.

### Renumbering (version 3 → version 4)

| version 3 | version 4 |
|---|---|
| Lemmas 2, 3, 4, 5 (reduction, valuations, model, escape ladder) | Lemmas 3, 4, 5, 6 |
| Lemma 7 (one block, two laws) | Lemma 9 |
| Remark 3 (first-passage formula), Remark 4 (two routes to natural density) | Remarks 1, 2 |
| Remark 7 (unbounded orbits and the return term) | Lemma 7 |
| Remark 11 (arithmetic rigidity of returns) | Lemma 8 |
| Proposition 12 (first-excess transport) | Proposition 10 |
| Proposition 14 (domination by the stopped potential) | Proposition 11 |
| Table 13 (Run K) | Table 12 |
| Corollaries 8, 9; Propositions 10, 11, 13; Problem 3; Remarks 6, 8–10 | research note `tail_programme` (Remark 8: its numerical part is the paragraph "Run I") |
| Remarks 1, 2, 5, Example 1 | plain text |
| Table 12 (reproduction) | README, reproduction map |

New in version 4:
- Lemmas 2 and 10–12;
- Propositions 12–17;
- Corollaries 8 and 9;
- Conjectures 3 and 4;
- Remark 3;
- Tables 14–17.

## Code and data

All programs, SLURM templates, raw outputs and the research notes are in this folder of the repository
[Sultanow/collatz](https://github.com/Sultanow/collatz/tree/master/07%20Basins%20of%20Attraction). The runs that are
new since version 3:

| Run | Content | Data |
|---|---|---|
| L | exact Syracuse laws up to n = 20 | `docs/research/syracuse_lp/results_exact/` |
| M | Monte Carlo of the Fourier maxima up to n = 80 | `results_mc/`, `results_mc2/` |
| N | logarithmic chart up to n = 15 | `results_logchart/` |
| O | first-hit decomposition, cluster up to N = 17 | `results_runO*/`, `results_energy/`, `results_classes/` |
| P | cycle census for word lengths 41–66 | `results/run_P/` |
