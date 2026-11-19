# Research notes — Same Shadows, Different Forests

This folder contains **working research material** from the development of the appendices of the
paper ([`../main.tex`](../main.tex)). It is not part of the paper: only what the paper states, with
its stated hypotheses, is claimed there. Numbers below refer to version 3 of the paper and to the
draft after it (Remark 12).

## Notes that entered the paper

| Folder | Content | In the paper (version 3) |
|---|---|---|
| [`first_excess/`](first_excess) | weighted first-excess transport beyond the coupling horizon | Proposition 12, Problem 3 (Appendix D.3) |
| [`dyadic_leakage/`](dyadic_leakage) | aggregate dyadic block-leakage criterion | Problem 3 (aggregate survival bound) |
| [`arithmetic_rigidity/`](arithmetic_rigidity) | parity strip of late returns | Remark 11 |
| [`relative_energy/`](relative_energy) | single-depth relative-energy defect | Proposition 13 (stated as an equivalence) |
| [`stopped_potential/`](stopped_potential) | domination of the first-excess tail by the stopped predecessor potential | Proposition 14 (first form) |
| [`odd_core/`](odd_core) | odd-core compression: only odd targets enter | Proposition 14 (odd targets), Run K odd rows |
| [`spikes/`](spikes) | pointwise criterion and its failure, inverse-ray countermodel, live dangerous points | Remark 12 (draft after version 3) |
| [`returns/`](returns) | return ladders; live points without returns; return-certificate identity | paragraph before Problem 3 (return ladders, Phi_X >= (5/8) b X), Remark 12(c) (draft after version 3) |
| [`return_excess/`](return_excess) | window excess J_X of the return term; single-scale ladder; conditional polynomial heights; finite-horizon escapes | Remark 7, second part (draft after version 3) |
| [`entrance/`](entrance) | early returns: local first-passage progression theorem, Cesaro form of the entrance law, collision criterion | Remark 7, paragraph "Early returns" (draft after version 3) |
| [`transfer/`](transfer) | threshold transfer of the entrance law; residue-by-residue transfer refuted (incl. the refuted note); no L^2 density of the Syracuse profile | Remark 7 (one sentence), Remark 12(d) (draft after version 3) |

Each folder has a `STATUS.md` (or `README.md`) with the audit and what was taken over. The other
statements of these notes stay here.

## Earlier audits and candidate sections

`PROOF_AUDIT.md`, `RESEARCH_NOTE.md` (clipped calibration, now Proposition 8), `TAIL_AUDIT.md`,
`TAIL_RESEARCH_AUDIT.md`, `EXCEPTIONAL_MASS_AUDIT.md`, `ENTRANCE_TRANSPORT_AUDIT.md`,
`RETURN_*_AUDIT.md`, `SHELL_LOCAL_TIME_AUDIT.md`, `MOD16_ESCAPE_RESEARCH_REPORT.md`,
`natural_density_3xc.md` (transfer audit, now Theorem 2 and Appendix E) and the
`candidate_*.tex` sections are working drafts from 15–16 November 2026. Their lemma and theorem
numbers refer to the manuscript of that day. They are conditional and **not** included in the paper
unless the table above or the paper says so; do not `\input` them into `main.tex`.

## Open problems (as in the paper)

- `T_X = o(X)` for the clipping tail (Problem 3; equivalent to Proposition 13's defect condition;
  implied by uniform integrability of the stopped potential of the odd targets, Proposition 14;
  needed in averaged form and not implied by the structural properties of `h`, Remark 12).
- `R_X = o(X)` for the return term (Corollary 7).
- The audit of the GGM parameter uniformities used in Lemmas 2–4 and Theorem 3.

Reference outputs here are computational checks, not proofs of asymptotic statements.
