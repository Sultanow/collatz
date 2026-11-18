# Research checkpoint: live parity does not certify many returns

The standalone LaTeX is `Live_Parity_False_Positives_and_Return_Certificates.tex`; its PDF is the companion preview. This package does **not** modify the Collatz main paper.

The exact, unconditional result constructs infinitely many real Collatz orbits with long odd prefixes remaining above first-passage cutoff, but only two visits to the initial dyadic shell. The conditional potential amplification uses an outside **unverified** predecessor-density lower bound. The return-certificate identity is exact but its weighted asymptotic consequence remains open.

Reproduce checks with `python test_construction.py` and `python test_return_certificates.py`; compare `test_results.json`. Inspect `PROOF_AUDIT.md` before citing any claim.
