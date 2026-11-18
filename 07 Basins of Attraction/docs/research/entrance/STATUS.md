# Early returns and the first-passage entrance law: status (2026-11-18)

Two notes on the early part of the window excess `J_X` (Remark 7): visits to `W_X` during the first
`L = floor(log X)` steps after the first passage below `X_1`. Neither controls the limiting
logarithmic entrance law or later returns; `J_X = o(X)` stays open. What entered the paper is the
paragraph "Early returns" of Remark 7 (draft after version 3).

## Audit

| Note | Statement | Verdict | In the paper |
|---|---|---|---|
| `Syracuse_Local_Entrance_Progression_Theorem`, Lemma 1 | starts with a fixed Syracuse valuation word form one class mod `2^{A+1}`; the entrance points form an arithmetic progression with difference `2*3^k`; the admissible starts in `(B, 2B]` are consecutive, so every odd class mod `2^L` gets `N/m +- 1` | correct; checked exhaustively for all 62,900 (X = 2^20) and 205,532 (X = 2^22) words: progression exact, deviation at most 1 | Remark 7 |
| same, Lemmas 2-4 | residue balance on the truncated set; Haar comparison; Chernoff (`rho_+ = 1+sqrt2`, `rho_- = 1/(2 sqrt2 - 1)`, `lambda_- = 0.05415`); long passages need few divisions | correct | Remark 7 ("Chernoff bounds dispose of ...") |
| same, Thm 5 / Cor 6 | odd starts uniform (or harmonic) in `(B, 2B]`, `B = floor(X^{1-2 beta})`: first passage below `B` followed by `L` steps reaches `X^{1-beta}` with probability `O(X^{-beta})` for `beta = 1/200` (`gamma = 1/4`, `epsilon = 1/10`; smallest exponent `beta`) | correct, unconditional, local (one source shell) | Remark 7 |
| same, Prop 7 | logarithmic entrance law = Cesaro mean over the source shells `(2^j B, 2^{j+1} B]` with harmonic weights | correct | Remark 7 |
| same, Prop 8 / Cor 9 | renewal identity across one threshold; transfer under a pointwise density bound on `(B, 2B]` and a small direct overshoot | correct as an implication; both hypotheses unproved (the overshoot is the same problem one level up) | not used |
| `Syracuse_2Adic_Entrance_Collision_Criterion`, Thm 1 | early returns `<= (L+1) sqrt(3 Q_X) X^{1-beta/2}`, `Q_X = m sum_r q(r)^2` on odd classes mod `2^L` | correct (Doob with `Z_1 = 3/2`, Cauchy-Schwarz) | Remark 7 |
| same, Prop 2, Section 4, Lemma 3 | collision and Parseval forms; total variation insufficient (`Q ~ 1 + m eps^2`); long progressions have `Q <= 1 + m/N` | correct | Remark 7 (TV remark) |
| same, Section 5 | sampled collision energies near 1 | reproduced: 1.0120, 1.0094, 1.0049 for X = 2^20, 2^22, 2^24 (2,000,000 samples); local shell exactly 1.01799, 1.01299, 1.00592; `P(tau <= 5)` and early-excursion frequencies agree to all printed digits | Remark 7 ("within 2 % of 1") |

The early-excursion frequency at the paper's beta = 1/200 is about 0.55 at these scales
(`X^beta` is only about 1.07 there); the power saving `X^{-1/200}` is asymptotic only.

## Conclusion

The early returns are reduced to one quantity of the actual entrance law, its collision energy on the
odd classes mod `2^{floor(log X)}`; it is bounded for a single source shell (unconditionally) and
numerically close to 1, but the transfer from one shell to the Cesaro limit over all shells is open,
and total-variation estimates cannot give it. Later returns are not addressed.

## Files

- the two notes (`.tex`, compiled `.pdf`; `\usepackage{lmodern}` added to the progression note so that
  microtype can compile it, content unchanged)
- `syracuse_local_shell_verify.cpp`, `syracuse_entrance_residue.cpp`: the programs of the notes
- `local_shell_check.cpp`, `local_shell_check_output.txt`: independent check (exhaustive local shell,
  progression lemma word by word, sampled collision energy)
