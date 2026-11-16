> **Status in the paper (v6e):** the aggregate dyadic criterion (Theorem 2 of the note, in its
> polynomial form) now replaces the per-shell survival condition in Problem 3; everything else stays
> here. See `STATUS.md`.

# Dyadic Block Leakage — independent Collatz research note

**Deliberately NOT integrated into `main.tex`.**

Files:
- `Collatz_Dyadic_Block_Leakage_Standalone.tex`: self-contained, 5-page LaTeX research draft.
- `Collatz_Dyadic_Block_Leakage_Standalone.pdf`: compilation preview.
- `PROOF_AUDIT.md`: mathematical status, imported assumptions, tests, missing proof.
- `verify_dyadic_identities.py`: 4,000 exact-rational abstract path tests.
- `exhaustive_finite_window.py`: all 49,152 odd starts in the documented finite interval for each map.
- `finite_orbit_probe.py`: finite-window actual-orbit simulation, with non-paper beta chosen for diagnostic power.
- `exhaustive_finite_window_results.txt`, `finite_orbit_results.txt`: recorded outputs.

Run `python verify_dyadic_identities.py`, `python exhaustive_finite_window.py`, or `python finite_orbit_probe.py`.

**Status:** The proved new contribution is a project-specific dyadic layer-cake equivalence and conditional aggregate leakage reduction. The polynomial/aggregate leakage estimate for the limiting actual Syracuse first-excess law is **not proven**. The experiments do not change that status. No Lean claim is made.
