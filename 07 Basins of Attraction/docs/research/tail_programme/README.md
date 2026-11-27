# First-passage tails: criteria, certificates and obstructions

`tail_programme_note.tex` / `.pdf` (7 pages) keeps the parts of Appendix D that left the paper in draft v4p
(roadmap D2, decision of 27 November 2026: move out only what improves readability):

- moment criteria and the location of the clipping tail (former Remark 6, Corollary 8, Remark 8 except Run I),
- early returns and the entrance law (former paragraph of Remark 7),
- return certificates and their limits: finite-horizon residue balance, finite-support obstruction,
  two-level return certificate and fixed-modulus obstruction (both in Lean 4: `lean/CollatzBasins/ReturnLyapunov.lean`,
  `ReturnLargeShell.lean`), conditional removal of the clipping tail, Lean remark,
- long finite excursions: kernel form, return ladders, the weighted-occupation problem (former Problem 3,
  equivalent to `T_X = o(X)`) and the single-depth concentration criterion (former Proposition 13).

The text is that of draft v4o. References with the prefix P point to draft v4p of the paper; they were resolved
from its `main.aux` when the note was generated. Build: `pdflatex tail_programme_note && bibtex tail_programme_note
&& pdflatex tail_programme_note && pdflatex tail_programme_note` (uses `../../main.bib`).

The paper keeps the chain calibration (Proposition 8), two tails (Corollary 7), Lemma 7 (return term), tightness
(Proposition 9, Run I), first excess (Proposition 10), Lemma 8 (arithmetic rigidity) and the domination by the
potential (Propositions 11 and 12), which avoids the open points of this note.
