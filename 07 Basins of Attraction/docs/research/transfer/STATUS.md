# Threshold transfer of the entrance law and L^p regularity: status (2026-11-19)

Three notes on carrying the local early-return bound (folder `entrance/`) from one source shell to the
logarithmic entrance law, and on the L^p regularity this would need. None of them proves the
transfer, `J_X = o(X)` or `T_X = o(X)`. What entered the paper: one sentence in Remark 7 ("Early
returns": no residue-by-residue transfer) and Remark 12(d) (moments of order >= 2 diverge).

## The refuted note

`Transfer_Operator_Dangerous_Residue_Classes_WRONG.tex` proposes a transfer operator on the odd classes
mod `2^L` and targets `sup_s K_B(D_X, s) << X^{-beta} + X^{-delta}`, uniformly in the input class `s`.
Both steps are wrong:

1. The first passage does not factor through the residues mod `2^L`: `z` and `z + 2^L` with first
   valuation 2 land at points that differ by `3 * 2^{L-2}`, not 0 mod `2^L` (V2, Prop. 2). The
   recursion `q_B ~ q_{2B} + T_B q_{2B}^mid` on residues is therefore not exact.
2. The uniform bound is false: the class `s = -5 * 3^{-1} mod 2^L` sends asymptotically a third of its
   members in `(B, 2B]` in one Syracuse step (valuation 2) to `y = -1 mod 2^{L-2}`, and these climb
   above `Y` in the next `L - 2` odd steps (V2, Thm. 3; checked exactly for X = 2^20 ... 2^32:
   ratios 0.33333, 0.33333, 0.33175, 0.33415). Under harmonic weights the share is log(4/3)/log 2.
3. Its "progression lemma" states `Pass_B(z + 2^{A+1}) = Pass_B(z) + 2*3^k` without the admissibility
   condition. Correct is `Syr^k(n + 2^{A+1}) = Syr^k(n) + 2*3^k` on the class of a valuation word, and
   only the starts that pass below `B` with that word (a consecutive set) give a progression of
   entrance points. The paper (Remark 7) uses the correct form.

## Audit of the other notes

| Note | Statement | Verdict | In the paper |
|---|---|---|---|
| `Syracuse_Transfer_Operator_V2`, Prop 1 | renewal identity `nu_B = nu_{2B}|_{[1,B]} + P_B(nu_{2B}|_{(B,2B]})` | correct | not used (stated in `entrance/`) |
| same, Prop 2, Thm 3 | no residue factorisation; explicit class with dangerous share 1/3 | correct | Remark 7, one sentence (compare Prop. 11) |
| same, Thm 4, Thm 5, Cor 6 | transfer under an incoming `L^p` density bound, or under collision energy plus a within-class discrepancy | correct (Hoelder, Cauchy-Schwarz); hypotheses unproved | not used |
| `Syracuse_Regularity_Transfer_V3`, Prop 1 | `H_p(n) >= 3^{p-1}/(2^p - 1) H_p(n-1)` for the Syracuse profile (self-similarity `V = 2^{-A}(1 + 3V')`, drop cross terms) | correct; factor > 1 for p > 2, = 1 for p = 2 | Remark 12(d) |
| same, Thm 2 | the 3-adic Syracuse law has no L^2 density; `H_2(n) -> infinity` | correct (branches `a = 1` and `a = 1 + 2*3^k` share the image, `R_k -> id` since `4^{3^k} = 1 mod 3^{k+1}`, so the overlap is positive; L^2-bounded martingale otherwise) | Remark 12(d) |
| same, Prop 3 | `H_2(n) >= c n` given the L^1 convergence of the profiles (Tao (1.26)) | correct | Remark 12(d): "grows by about 0.47 per level" (numerical) |
| same, Thm 4, Lemma 5 | data processing for a deterministic map; one valuation code preserves low bits | correct, elementary | not used |
| same, Thm 6 | one-step dangerous transfer bounded by the shell moment of `R h(z)` | correct; needs a potential moment bound, open | not used |

Consequence used in the paper (Remark 12(d)): Theorem 3 bounds the class means of `a h(a)` mod `3^n`
from below by `gamma_0 f_n`; with Jensen, `liminf X^{-1} sum_{a <= X} (a h(a))^p >= gamma_0^p H_p(n)` for
every `n`, so these moments diverge for every `p >= 2`. Moment routes to (UI) need `1 < p < 2`,
consistent with Run K (`gamma ~ 0.8`) and with the profile exponent in Table 13 (1.69, 1.08, 0.94 for
n = 8, 12, 16).

## Checks

- `syracuse_transfer_v2_check.py` -> `syracuse_transfer_v2_check_output.txt` (exact; reproduced)
- `syracuse_v3_three_adic.py` -> `syracuse_v3_three_adic_output.txt` (L^p norms of the profile; reproduced)
- `syrac_collision_mc.py` -> `syrac_collision_mc_output.txt`: independent Monte Carlo of `H_2(n)` from
  the forward definition of the Syracuse variable: 3.536, 4.931, 6.338 for n = 5, 8, 11, against
  3.534, 4.932, 6.334 from the recursion
- `syracuse_v3_regularization_diagnostic.cpp`: the note's finite-source diagnostic (beta = 0.08); not
  used and not rerun

`\usepackage{lmodern}` was added to V2 and V3 so that microtype compiles them; content unchanged.
