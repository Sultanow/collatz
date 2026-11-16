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
