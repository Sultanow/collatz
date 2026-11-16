> **Status in the paper (v6g):** the single-depth defect inequality is Proposition 13 in Appendix D.3,
> stated as an equivalence; the support correction (few states force a large defect, not conversely)
> is applied there. Note: the summed defects D_X and C_X are *equivalent* to T_X = o(X), not merely
> sufficient. See `STATUS.md`.

# Collatz — Relative Fourier Energy / Single-Depth Block Leakage

Standalone research deliverable (16 November 2026). NOT integrated into `main.tex`.

Files:
- `relative_energy_transport_standalone.tex` — six-page independent LaTeX proof note.
- `relative_energy_transport_standalone.pdf` — compiled and visually checked preview.
- `PROOF_AUDIT.md` — theorem-by-theorem certification boundary and a proposed prose correction to consolidated `main(6).tex`.
- `check_energy_lemmas.py` — exact finite-arithmetic checks of the capacity and recurrence inequalities and Fourier Parseval tests (requires Python 3; NumPy optional).

Compile: `pdflatex relative_energy_transport_standalone.tex` (twice for references).
Test: `python check_energy_lemmas.py`.

**Main improvement over the earlier all-depth approach:** One parity-prefix union at depth `t_X` yields a single defect `d_{t_X}`. It is not necessary to sum defects at every intermediate modulus, and no actual-orbit parity independence is assumed.

**Still open:** Showing the summed **event-specific correlation defect** is `o(1)` for the actual first-excess return laws. A stronger sufficient condition is the summed relative capacity defect. Full untruncated chi-square/Fourier energy is too strong to be a necessary target (explicit abstract counterexample in the note). A complete proof of `T_X=o(X)` is not supplied; `R_X=o(X)` remains separate.
