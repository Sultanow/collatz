# Live parity, return certificates and return ladders: status (2026-11-17)

Two notes that test the refinements of Proposition 14 against actual orbits. Neither proves
`T_X = o(X)`; both are unconditional constructions plus one elementary identity.

## Audit

| Note | Statement | Verdict | In the paper (draft after version 3) |
|---|---|---|---|
| `Genuine_Return_Ladders_Standalone`, Thm | for every b >= 100 a start a in (X/2, X] with at least b+1 visits to (X/2, X] before its first value <= X^0.99, and log2 X < (100/33) b + 6 | correct (blocks OOE = (9x+5)/8 and OE = (3x+1)/4, balanced product in [3/4, 9/8], Terras realisation, halving suffix); reproduced for b = 100, 300, 1000; independent check for c = 1, -1, 5, 13 | one paragraph after Proposition 12: return counts of order log X exist, Problem 3 is about mass |
| `Live_Parity_False_Positives_...`, Thm 1 | a = 2^s v 3^t - 1, s = ceil((log t + 3)^2): s odd steps above X = a (live for b = ceil((log log X)^2)), then halvings below X^0.99; two visits to its shell | correct; reproduced; independent check: no later odd point in the band J | Remark 12(c): live points need not return |
| same, Cor 2 | polynomial spikes on the live set under (UF) | conditional on an unrefereed online manuscript | not used |
| same, Prop 3 / Cor 4 | exact return-certificate identity s(n+b) = nu_n(A_{I,b}) and the bound over actual multi-return states | correct, elementary; it is the identity behind Propositions 12 and 13 (return counts N^+) | Remark 12(c), as the sharpest admissible set |

## Conclusion

The chain of sets in Proposition 14 is now explicit: actual return states ⊂ live points ⊂ parity
classes C_{X,b}, both inclusions proper. Actual return states with order log X returns exist at
arbitrarily large scales, so no deterministic cap of the return counts can close Problem 3; what is
missing is the weighted mass of the entrance law (equivalently, of the stopped potential) on them.
`README_NEXT_EXPERIMENT.md` states this target; it is open and sufficient, not proved.

## Files

- the two notes (`.tex`, compiled `.pdf`), audits `PROOF_AUDIT_*.md`, `README_live_parity.md`,
  `README_NEXT_EXPERIMENT.md`
- `construct_returns.py` → `return_ladders_results.json`; `test_construction.py` → `test_results.json`,
  `construction_test_stdout.txt`; `test_return_certificates.py` → `return_test_stdout.txt` (all reproduced)
- `verify_returns.py`, `verify_returns_output.txt`: independent checks (ladders for four maps with the
  paper's beta = 1/200; the live family has two shell visits and no later odd point in J)
- `verify_ladders.py`, `verify_ladders_output.txt`: second independent check of the ladders for
  c = 1, -1, 5, 13 and b = 100, 300, 1000 (visits to (X/2, X] and orbit mass in W_X before X^0.99;
  the mass is about 0.92 b X, above the (5/8) b X stated in the paper)
