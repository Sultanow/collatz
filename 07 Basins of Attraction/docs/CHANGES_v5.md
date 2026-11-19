# v5: one file, two streamlining passes

`docs/main.tex` now contains everything; `truncated_calibration.tex` and `tail_tightness.tex`
are merged into it and can be deleted from the repository. 60 pages (before: 62), main text up to p. 38 (before: p. 44).

## Pass 1: literature instead of re-derivation
- Unique parent of branching numbers: proposition and proof replaced by one sentence with citations
  (Sultanow et al. 2020, Kleinnijenhuis et al.).
- Parity words and cycles (Böhm–Sontacchi, Lagarias): statement kept, proof sketch replaced by the one identity it rests on.
- Divisibility filtration (Lagarias, Belaga–Mignotte (19), Wegner): attributed in the header, proof replaced by one sentence.
- Remark "Predecessor sets" removed: it repeated Proposition 7(a) with a weaker argument;
  its natural-density sentence moved after Proposition 7.
- Inside the tail part: Proposition 9 (old), the paragraph "A concrete next target" and the
  "remaining gap" paragraph repeated each other and Corollary 7; merged into Remark 6 (moment criteria).

## Pass 2: secondary blocks to the appendix
- Appendix B (new): the case q = 1 and the product identity for cycles of the same type.
- Appendix C: only the proofs of Theorem 2 and Corollary 6.
- Appendix D (new, formerly Section 7 plus the research parts of Appendix B):
  D.1 calibration and decomposition (Prop. 8, Cor. 7, Remark 6),
  D.2 exponential tightness (Prop. 9, Cor. 8, Remark 7, Run I),
  D.3 return certificates and their limits (Lemma 5, Remark 8, Prop. 10, Cor. 9, Prop. 11, Remark 9),
  Problem 3 (new): weighted occupation of long finite excursions.
- Main text keeps one paragraph pointing to Appendix D (after Corollary 6) and Outlook item (2).
- Contribution table: the three Lean rows are merged into one, the tail rows into one ("First-passage tails").
- Machine-checked core: states that the workflow runs `lake check` (fails on non-standard axioms).

Abstract: the three closing sentences on Lean and return certificates are shortened to one.

Unchanged: title, keywords, all theorems of the main text, all numerical results.

## v5c: Run J (heuristic for primitive cycles)
- Section 4, paragraph "Heuristic": the deficit of the heuristic sits in the types with l, k even and D > 0;
  mechanism: P = 2^{l/2}+3^{k/2} divides z_3 of every periodic word B'B', and aperiodic words are depleted modulo P
  (factor 0.61 for D > 0, 1.40 for D < 0). The underdispersion is weaker than stated before and depends on the bound C.
- Problem 1 and Outlook (3) reformulated accordingly; reproduction table: row for Run J.
- `alt_if_lean_green/`: the same paper plus the Lean statement for Proposition 11, to be used only after the
  CI has built ReturnLargeShell.lean (see alt_if_lean_green/README_FIRST.md).

## v6: natural densities for all 3x+c (conditional), escape ladder
- Theorem 2 (new): every completely invariant set of T_{3,c}, c >= -1 odd, has a natural density equal to
  its logarithmic density, conditional on Allikvere's Section 6. Appendix E: transfer table (where the map
  enters Tao §5 and Allikvere §§2–6) and Lemma 7 (uniform vs. logarithmic first passage on one block).
  Predecessor sets: h(a) is a natural density (same proof). Remark 6 rewritten (two routes: Mazur, Allikvere).
  Abstract, introduction, contribution table, Problem 2(a), Outlook (1) updated accordingly.
- Escape ladder (Lemma 5 in Appendix C, colleague's idea): an unbounded orbit carries at least (2-o(1))X
  below X. Corollary 6(b): the excess caused by an unbounded orbit of potential h is now 2h (was h/3).
  Remark 7 in Appendix D: R_X = o(X) alone excludes unbounded orbits of positive potential.
- Barina 2021 cited next to Barina 2025.
- Renumbering: the lower-bound theorem is now Theorem 3 (was 2).

## v6b
- Contribution table, row "First-passage tails": the old wording ("which alone decides T_X = o(X)") could be read as
  if exponential tightness proved T_X = o(X). Now: whether T_X = o(X) holds depends only on the mass carried by
  the exceptional set, which is not controlled.
- Appendix D.3: Proposition 12 (first-excess transport): T_X(L)/X = transported mass of the first-excess
  entrance law + overshoot; sandwich with free per-shell allowances (log X)^sigma 2^{omega j};
  T_X = o(X) iff the excess of late shell returns tends to 0. Kernel form via first-entry/first-return kernels.
- Problem 3 restated in these terms, with the polynomial survival bound as a sufficient condition.
- docs/research/first_excess/: the full research fragment and its finite exact-rational check
  (3600 terminated paths, 1862 first-excess decompositions, all passed).

## v6c
- Table 2 (contributions): caption shortened to one sentence. Its caveats moved into the text:
  the meaning of "exact bounded computation" into the paragraph before the table; the scope of the
  Lean links into the subsection "Machine-checked finite core and its scope", whose list of what is
  not certified now also names the first-passage calibration and tightness (Propositions 8 and 9)
  and the all-large-shell obstruction of Proposition 11 (the latter only in the main variant; in
  alt_if_lean_green it is formalised).

## v6d
- Figure 4: caption rewritten ("one point per cycle", which 98 maps, what the dashed line and the colour
  mean). The definition of the predicted share (mean mass of the levels 64 to 69, normalised so that the
  predicted shares of each map add up to 1) moved into the text before Table 10.

## v6e
- Problem 3 (Appendix D.3): the sufficient survival condition is now aggregate over the shells, taken at
  the dyadic return counts 2^l K_{X,j}. Dyadic grouping gives D_X/2 <= E_X <= B_{X,0} + D_X, so
  T_X = o(X) iff D_X -> 0. The bound B_{X,l} <= A 2^{-(1+kappa)l} mu_X S_X gives
  T_X = O(X (log X)^{sigma-eps_0}). It follows from a fixed ratio q < 1/2 and from the old per-shell bound,
  without needing the latter. Errors are admissible only if they are 2^l-summable.
- docs/research/dyadic_leakage/: the full research note (with PDF), its proof audit, scripts and outputs
  (all reproduced), and STATUS.md. STATUS.md explains why the finite-window ratios are not evidence for
  the uniform condition.

## v6f
- Appendix D.3, new Remark 11 ("Arithmetic rigidity of returns"), after Problem 3:
  - The exact log-cocycle of T_{3,c} forces every return to a shell into the parity strip
    |h_t - zeta t| <= zeta + O(t/X_1), with zeta = log 2/log 3, at every time t. Survival above X_1
    for t >= t_X ~ beta log X steps forces h_t >= zeta_0 t.
  - With Terras' bijection, a law with relative 2-adic concentration <= A on the scales 2^t
    (t <= t_X) returns or survives with mass <= A exp(-t Delta_0).
  - Two inputs are missing, and they mark where new mathematics is needed on this route:
    (1) relative instead of absolute errors for laws of mass O((log X)^{-eps_0});
    (2) a bound on the later visits of the last X^{O(beta)} states, which no residue count gives.
- docs/research/arithmetic_rigidity/: the note (tex + pdf), its audit, scripts and outputs
  (reproduced), and STATUS.md.
- 66 pages.

## v6g
- Appendix D.3, new Proposition 13 (Single-depth concentration criterion):
  D_X(A) <= E_X <= (2b mu_X + D_X(A))/(1-q). D_X(A) is the summed excess of the return laws over the
  relative cap A 2^{-t_X}|nu| at the single modulus 2^{t_X} = X^{O(beta)}. Hence T_X = o(X) iff
  D_X(A) -> 0, and the same holds for the excess on the dangerous classes alone.
- Remark 11: its second paragraph is replaced by this proposition and a paragraph on the two sources of
  the defect. The support statement now runs in the right direction: few states force a large defect,
  while a large defect does not imply few states (heavy atoms).
- docs/research/relative_energy/: the note (tex + pdf), its audit, the check script (reproduced), and
  STATUS.md. STATUS.md explains why the criteria are equivalences, not weaker targets.
- 67 pages.

## v6h
- Appendix D.3, new Proposition 14 (Domination by the stopped potential):
  E_X <= 2b mu_X + (2/X) sum over the dangerous classes C_{X,b} of y h(y), restricted to targets whose
  orbit reaches [1, X_1]. The proof uses the fact that a terminating orbit never revisits a state.
  Consequence: T_X = o(X) if the stopped profile y h(y) is uniformly integrable on average.
  This is a one-way, static criterion: the hypothesis concerns the unconditioned potential, not
  conditioned return laws, and says nothing about unbounded orbits. It is supported by Tao (1.26),
  which makes the Syracuse densities converge in L^1, so the Syracuse profile is uniformly integrable.
- Contributions table (row "First-passage tails") and Outlook item (2): one clause each.
- docs/research/stopped_potential/: the note (tex + pdf) and STATUS.md.
- 68 pages.

## v6i (Lean CI green)
- docs/: the Lean variant is now the main version (formerly alt_if_lean_green/docs). Proposition 11 has the
  explicit threshold X0(c,k) = 24*2^k + 12|c| + 24. The Lean remark cites ReturnLargeShell.lean, the
  contributions table links "large shells", and the Outlook no longer lists the all-large-shell
  obstruction as future work.
- Section 2, "Machine-checked finite core": the description of the CI now matches the workflow. It
  builds every module (kernel), rejects sorry/admit/native_decide/axiom, and enforces the axiom audit
  (propext, Quot.sound, Classical.choice). An optional manual job runs lake check. The old sentence said
  that the workflow runs lake check and fails on non-standard axioms, which was not what the workflow did.
- docs/pin_lean_links.py: pins all Lean links to one commit SHA and inserts the sentence naming it
  (marker line '% lean-pin').
- lean/: docstrings updated after the green run (no more "candidate / not yet checked"). The
  FiniteHorizonTransport docstring no longer quotes an outdated error bound. AxiomAudit also prints the
  five Mod16Escape theorems. The root module imports Mod16Escape explicitly. New: tests/check_axioms.py.
- .github/workflows/collatz-lean.yml (repository root, in _repo_root/): native_decide added to the
  forbidden list, AxiomAudit.lean added to the grep; new step "every module is imported by the root";
  the axiom audit now fails on any axiom outside the standard three (and on missing reports).
- README: new section "Lean" (modules, what the paper cites, how to check locally, the CI); Run K section.
- C/: Run K scripts (ui_report.py, run_K.sh, slurm/run_K_*.sbatch, tests); results follow.

## v6j (Run K, Lean links pinned)
- Appendix D.3, after Proposition 14: paragraph "Run K", Table 12 and Figure 5. The predecessor counts
  use N = 3e10 for all targets a <= 2^22 of 3x+1, 3x-1, 3x+5 and 3x+13. The flow identity holds exactly,
  and every target is stopped. The distribution of a h(a) is the same for the four maps and stable from
  2^14 to 2^22: median 2.3, q99 68, mean 6.91-6.98 vs 6.952. It is heavy-tailed: half of the mean comes
  from a h(a) > 20, 15 % from > 100, 1.4-2.4 % from > 1000, fitted exponent about 0.8. The exact Syracuse
  profile has the same tail; it converges by Tao (1.26) but its far tail still grows with the depth. The
  data are consistent with (UI) but cannot decide uniformity in X, and an L^p bound is possible at most for
  p < 1 + gamma. The dangerous classes carry their density share of the mass to within 2 %.
- Reproduction table: row for Run K; tests row with run_ui_tests.sh / ref_counts.c.
- Lean links pinned to commit ee5a5b4 (both CI jobs green, including the independent lake check replay);
  the sentence in "Machine-checked finite core" names the commit. docs/pin_lean_links.py: --deep option,
  re-pinning updates the existing sentence.
- C/: ui_report.py has a robust CSV reader, chooses the uint64 split automatically, and has a new
  subcommand `figures` (table and figure from the CSVs); slurm/run_K_counts.sbatch uses --exclusive and
  RUNK_DIR (the cluster sets WORK).
- results/potential_ui/: report, CSVs, table and figure. Copy counts_c*.npz from the cluster archive.
- README: Run K results, notes on --exclusive/RUNK_DIR, reproduction map rows for Run K and Lean.
- 70 pages.

## v6k (odd targets suffice)
- Proposition 14 now uses odd targets: T_X/X <= (3+2b) mu_X + (8/X) * sum over odd a in C_{X,b},
  X_1 < a <= X, of a h(a). Every visit to a shell is its first point, follows an odd point of the band
  J = ((U-c)/3, U], or ends an excursion above U started by such a point. The odd points of J with b
  later odd points of J lie in C_{X,b} (strip of Remark 11 on a band of ratio < 4). The bound is on T_X
  directly, without the allowances K_j. (UI) is now required for odd targets only.
- Discussion: for odd a, h(a) = h(T(a)) - h(2T(a)), so the hypothesis concerns the dyadic differences of
  the potential and doubling chains drop out. Conversely, even targets are sums along dyadic rays, so
  for the actual potential the restriction changes little.
- Run K: odd-target rows in Table 12. The shares change by at most 0.4 percentage points, and the odd
  mass on the dangerous classes is within 0.4 % of their density at 2^22. The float barrier before
  Appendix E is gone; Table 12/Figure 5 form a float page.
- Contributions table and Outlook: "of the odd targets".
- C/ui_report.py: ui_odd.csv, odd mass ratios in ui_dangerous.csv, table row "odd".
- results/potential_ui/: report rerun (18 s) with the odd-target output.
- docs/research/odd_core/: the fragment, its audit, check script and outputs (reproduced), STATUS.md.
- 70 pages.

## v3 (Preprints.org version 3)
- New Table 11 "computational runs": Runs A–K with computation, scale, place in the paper and a link
  to the data folder in results/. The text labels every run where it is described (Runs A–F added),
  the contributions section points to the table, and the reproduction table (now Table 12) carries
  the run labels. Numbering: reproduction 11 -> 12, Run K table 12 -> 13, transfer table 13 -> 14.
- Abstract: one sentence on Appendix D (clipping tail reduced to uniform integrability of the stopped
  potential of the odd targets, conditional on the coupling estimates; Run K consistent, not decisive).
- Contributions: the same pointer to Appendix D, Proposition 14 and Run K.
- Introduction: the predictor accuracy now matches abstract and Section 6 (median about 5 % after
  normalisation within each map, 3 % absolute with the constant 3/log(4/3)).
- Run K: Table 13 and Figure 5 are separate floats again (the joint float was 72 pt too high and
  drifted into Appendix E); a float barrier keeps both in Appendix D.
- README: paper section and citation for version 3, numbering of the reproduction map and of all
  references updated to version 3. docs/research/README.md: index of the notes and where they
  entered the paper.
- 71 pages.

## v4a (draft after Preprints.org version 3)
- New Remark 12 "Averages, not maxima" at the end of Appendix D (after Run K):
  (a) bounding the sum of Proposition 14 by its maximum would give T_X = o(X) under
  log G_X = o((log X)^eps0), but maxima grow polynomially: the Syracuse profile has
  max v_n >= (2/log(4/3)) (3/2)^n (Table 13 shows 1.46 times this), and the Run K maxima over odd
  targets grow like X^0.37 to X^0.62; this is compatible with (UI), which is needed in averaged form.
  (b) the indicator of one inverse ray of 3x+1 (u_0 = 5, e_i in {8..13}) satisfies Proposition 7(b), (c)
  and has a bounded mean, but violates (UI); convex combinations are positive on the basin. The ray is
  invisible to the finite counts h_N. (c) Proposition 14 holds with the live dangerous points; the
  subset is proper (explicit Chinese-remainder construction).
- Outlook item 2: pointer to Remark 12.
- docs/research/spikes/: the four notes, both audits (wording adjusted), checks (reproduced), STATUS.md.
  The conditional spike bound under (UF) is not used: (UF) rests on an unrefereed online manuscript.
- README: docs/main.pdf is the draft after version 3; research index row for spikes/.
- 72 pages.

## v4b
- After Proposition 12: return ladders. Blocks (odd, odd, even) and (odd, even) with a balanced
  product in [3/4, 9/8], followed by halvings and realised by Terras' bijection, give starts with at
  least b+1 visits to (X/2, X] before X^0.99 and log2 X < (100/33) b + 6. Return counts of order log X
  exist; Problem 3 is about the mass of the entrance law on such states.
- Remark 12(c): the proof of Proposition 14 counts only odd points with b actual later odd points in
  J; actual return states ⊂ live points ⊂ C_{X,b}, both inclusions proper (second example:
  a = 2^s v 3^t - 1 with s odd steps above X, then halvings).
- docs/research/returns/: the two notes, audits, scripts and results (reproduced), independent checks
  for c = 1, -1, 5, 13, STATUS.md. The (UF)-conditional corollary is not used.
- 73 pages.
- v4b review: the ladder paragraph defines X = floor(6a/5), states that the construction carries over
  to every odd c (checked exactly for c = -1, 5, 13), and adds Phi_X(a) >= (5/8) b X, so the finite
  excursions of Remark 8 exist at arbitrarily large scales. research/returns/verify_ladders.py checks
  b = 100, 300, 1000 for c = 1, -1, 5, 13, including the orbit mass in W_X.

## v4c (related work: Inselmann, Shaik)
- Related work: Inselmann (arXiv:2402.03276v3): descent below m^eps in natural density, and the mean
  total stopping time of T_{3,1} is at least (2/log(4/3)) log x, with equality if tau(m) = O(log m).
  Shaik (Zenodo preprint 4.0.2): height (log m)^9.99 f(m) in natural density by counting parity words,
  after (2/log(4/3)) log m + O(sqrt(log m log log m)) steps.
- Contributions table, row "Syracuse potential profile": Inselmann added to the previous foundation.
- Heuristic before Conjecture 1: the typical time c* log m is proved for 3x+1 (Shaik); almost-all
  statements miss the exceptional set, which the mean value counts (Corollary 6).
- After Corollary 6: Theorem 3 and Corollary 6 as the analogue of Inselmann's lower bound and
  conditional equality; Theorem 3 refines it to residue classes mod 3^n and holds for all 3x+c.
- main.bib: Inselmann_2024, Shaik_2026. 73 pages.

## v4d (window form of the return term)
- Remark 7, second part: R_X = S_Y + J_X with the window excess J_X (points of W_X outside the
  first-passage blocks). Given Proposition 8, (upper) holds iff T_X + J_X = o(X) (iteration over
  the scales X^{(1-beta)^j}); S_Y, the ordinary descent, drops out. Single-scale ladder:
  J_X >= h(a)(X - 2X^{1-beta}) if a <= X_1 is not periodic and its orbit reaches X. A power saving
  for J_X would bound heights (polynomially under a uniform lower bound for h, which is not known).
  Model prediction J_X = O(X^{1-beta}) (martingale e^{-D_j}); rigorous for uniform starts over the
  first log X steps (Doob). Entrance-law density bound and later returns are open.
- Appendix D intro and Outlook item 2: the return problem concerns only J_X.
- docs/research/return_excess/: both notes, their programs, an independent check showing that the
  entrance escape excess is parity (odd states), not 2-adic bias; STATUS.md.
- 73 pages.

## v4e (early returns after first passage)
- Remark 7, new paragraph "Early returns": besides uniform starts below X_1, the bound O(X^{-beta})
  for reaching Y within floor(log X) steps also holds after an actual first passage from one source
  shell (c = 1, beta = 1/200, odd starts uniform in (X_1, 2X_1]): valuation words give residue classes,
  entrance points form progressions with difference 2*3^k, Chernoff for the rest. The logarithmic
  entrance law is the Cesaro mean over all source shells; the transfer is open. Collision energy
  Q = m sum_r q(r)^2 on the odd classes mod 2^{floor(log X)}: early returns O(sqrt(Q) X^{1-beta/2} log X);
  total variation does not bound Q (1 + m eps^2). Experiments: Q within 2 % of 1.
- docs/research/entrance/: both notes, their programs, independent exhaustive check, STATUS.md.
- 74 pages.

## v4f
- Remark 7, "Early returns": the progression identity Syr^k(n + 2^{A+1}) = Syr^k(n) + 2*3^k is displayed,
  derived from the affine form Syr^k(n) = 3^k 2^{-A} n + c F_k (Remark 4) on the residue class mod 2^{A+1}
  of a valuation word (proof of Lemma 3); it holds for every odd c. "Early returns" starts a new paragraph.

## v4g (no residue-wise transfer; moments of order >= 2)
- Remark 7, "Early returns": the transfer cannot be organised residue by residue mod 2^{floor(log X)};
  the first passage does not factor through these residues, and for c = 1 the class -5*3^{-1} sends a
  third of its members in (X_1, 2X_1] in one step to y = -1 mod 2^{L-2}, which climb above Y.
- Remark 12(d): the Syracuse profile densities f_n satisfy H_p(n) >= 3^{p-1}/(2^p-1) H_p(n-1); the
  limiting profile has no L^2 density, so H_p(n) -> infinity for p >= 2. With Theorem 3 and Jensen,
  X^{-1} sum_{a <= X} (a h(a))^p -> infinity for every p >= 2; moment routes to (UI) need 1 < p < 2.
- docs/research/transfer/: notes V2, V3 and the refuted transfer-operator note, scripts and outputs,
  independent Monte Carlo of H_2(n), STATUS.md with the three errors of the refuted note.
- 74 pages.
