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
