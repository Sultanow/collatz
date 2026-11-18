# Return excess and polynomial heights: status (2026-11-18)

Two notes on the return term of Corollary 7 and its link to orbit heights. Neither proves
`T_X = o(X)`, `J_X = o(X)` or a height bound; what entered the paper is the second part of Remark 7
in the draft after version 3.

## Audit

| Note | Statement | Verdict | In the paper |
|---|---|---|---|
| `Syracuse_Return_Excess_Research_Note`, (1) | `J_X = sum_{a in W_X} a h(a) - F_X = R_X - S_Y >= 0` (window points outside the first-passage blocks) | correct; with the paper's definitions `S_X = C_X + T_X + R_X`, so `R_X = S_Y + J_X` | Remark 7 |
| `Syracuse_Polynomial_Height_Criterion`, Lemma 1 | with `C_X = gamma_0 X + o(X)`: window excess `E_X = T_X + J_X` is `o(X)` iff `S_X = gamma_0 X + o(X)` (iteration over the scales `X^{(1-beta)^j}`) | correct | Remark 7: (upper) holds iff `T_X + J_X = o(X)`; `S_Y` drops out |
| both, ladder lemma | if `a <= X_1` is not periodic and its orbit reaches `X`, then `J_X >= h(a)(X - 2X^{1-beta})` | correct (first points after `a` above `X/2^{j+1}` are distinct, in `W_X`, outside the first-passage blocks) | Remark 7, single-scale form of the existing bound |
| both, height theorem | `J_X <= K X^{1-delta}` and `h(a) >= c a^{-kappa}` imply heights `<= C n^{max(1/(1-2 beta), kappa/delta)}` | correct as an implication; the uniform lower bound for `h` is not known (the notes refer to unrefereed online claims) | one sentence, with "no such uniform bound is known" |
| Return excess note, Thm 2 | uniform `n <= X^{1-2 beta}`: proportion exceeding `X^{1-beta}` within `floor(log X)` steps is `<= 2X^{-beta} + 2^{L}/B` (parity bijection, Doob for `3^q/2^t`), `beta < (1 - log 2)/3` | correct, elementary | one sentence |
| Return excess note, Cor 3 | transfer to an entrance law with `L^p` density `M_p = O(X^eta)` (Hoelder) | correct; the `L^p` bound for the actual entrance law is not proved | mentioned as the missing density bound |
| Return excess note, diagnostics | entrance escape 0.106 vs uniform 0.063 "illustrates" a non-uniform entrance law | the numbers reproduce, but the excess is parity, not 2-adic bias: entrance states are odd, and uniform **odd** starts escape with 0.1066, 0.0812, 0.0632 against 0.1063, 0.0825, 0.0641 for the entrance law (X = 2^20, 2^24, 2^28; `entrance_vs_scale_output.txt`). The sampled `M_{3/2}` is dominated by parity (2^{1/3} = 1.26 for uniform odd) and Poisson noise (0.8 to 7.6 samples per state) | Remark 7: "agrees ... to about 1 %" |

## Conclusion

The return term of Corollary 7 contained the ordinary descent `S_Y`; the genuinely open part is the
window excess `J_X`, and (upper) holds iff `T_X + J_X = o(X)`. The model predicts `J_X = O(X^{1-beta})`
(martingale `e^{-D_j}`), and for uniform starts the first `log X` steps are rigorous. Missing: a density
bound for the actual first-passage entrance law, and later returns. The finite-horizon experiments do
not show a 2-adic bias of the entrance law.

## Files

- the two notes (`.tex`, compiled `.pdf`)
- `syracuse_short_escapes.cpp`, `syracuse_entrance_diagnostic.cpp`: the programs of the notes
- `entrance_vs_scale.cpp`, `entrance_vs_scale_output.txt`: independent check (uniform, uniform odd,
  entrance law, shell-matched uniform odd; beta = 0.1, 2,000,000 entrance samples per scale)
