# Research notes — Same Shadows, Different Forests

This folder contains **working research material**, not claims that have all been
independently verified or accepted for publication.

| File | Purpose | Current status |
|---|---|---|
| `PROOF_AUDIT.md` | Review notes for the reduction, coupling, renewal model and potential bounds | Targeted audit only; full Theorem 2 chain remains to be independently verified |
| `RESEARCH_NOTE.md` | Truncated first-passage calibration and the open tails `T_X`, `R_X` | Conditional research result relying on Lemmas 2–4 |
| `candidate_residual_section.tex` | Proposed first-passage/residual decomposition | **Candidate only**, not included in the paper |

## Connection to the paper

The publication manuscript lives one directory up (`../main.tex` and
`../main.bib`), with the auxiliary LaTeX file `../truncated_calibration.tex`.
Do **not** add `\input{research/candidate_residual_section.tex}` without a
separate mathematical review. The current paper uses
`\input{truncated_calibration.tex}` when that proposition is enabled.

The numerical reference implementation and its exact small-n output are at:

- [`../../C/syracuse_exact.py`](../../C/syracuse_exact.py)
- [`../../results/syracuse/`](../../results/syracuse/)

## Research checklist

- [ ] Audit the GGM parameter uniformities and quantitative first-passage bound.
- [ ] Independently verify the full proof of Theorem 2, especially the model-to-orbit comparison.
- [ ] Prove uniform integrability / bound `T_X=o(X)` or find a counterexample to the proposed moment bound.
- [ ] Bound or characterize the residual mass `R_X`.
- [ ] Compare 3-adic Syracuse profiles with predecessor-potential data.

Cite proved results and conjectures separately in the manuscript. Reference
outputs here are computational checks, not proofs of asymptotic statements.
