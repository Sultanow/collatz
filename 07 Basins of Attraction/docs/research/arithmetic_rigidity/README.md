> **Status in the paper (v6f):** the pathwise strip and its consequence are Remark 11 in Appendix D.3
> ("Arithmetic rigidity of returns"); everything else stays here. See `STATUS.md`.

# Arithmetic Rigidity of Late Returns (independent research note)

Files:
- `Parity_Strip_Late_Returns_Standalone.tex`: independently compilable LaTeX, nothing added to main manuscript.
- `Parity_Strip_Late_Returns_Standalone.pdf`: compiled independent preview.
- `PROOF_AUDIT.md`: statement status and reviewer cautions.
- `multi_altitude.cpp`: C++17 exact-integer orbit experiment at multiple starting altitudes.
- `multi_altitude_results.txt`: observed finite-window results with fixed random seed and 20,000 samples per combination.
- `verify_parity_strip.py`: independent finite arithmetic check of the cocycle and rigidity bound.

Build PDF: `pdflatex -interaction=nonstopmode -halt-on-error Parity_Strip_Late_Returns_Standalone.tex` (twice).

Run sample:
```
g++ -std=c++17 -O3 multi_altitude.cpp -o multi_altitude
./multi_altitude 20000
python3 verify_parity_strip.py
```

**Status:** deterministic theorems proved on paper but not Lean-certified; true stopped-measure polynomial decay still open.
