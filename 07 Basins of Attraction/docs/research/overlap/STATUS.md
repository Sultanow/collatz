# Overlap principle and domination of the returns by the potential: status (2026-11-19)

Starting point: the diagram `syracuse_regularization_transfer_diagram.py` (rendered as `.pdf`/`.png`).
Its left half is the 3-adic overlap principle, its right half a potential-moment bound for the
dangerous entrance states of one source shell. Evaluating it led to Proposition 15 of the paper
(draft v4h), which dominates the whole window excess `J_X`, early and late returns, by the potential.

## The diagram

| Part | Statement | Verdict | In the paper |
|---|---|---|---|
| left | branches of the Syracuse variable overlap on 3-adic sets of positive mass; `H_p(n) >= 3^{p-1}/(2^p-1) H_p(n-1)`; `H_2(n) -> infinity` | correct; an obstruction, not a tool: moment bounds for `a h(a)` can only hold for `1 < p < 2` | Remark 12(d) (since v4g) |
| right | `mu_B(E_B) <= C_p H_p(B) X^{-beta(1-1/p)}` for the entrance mass of the inputs `z in (B, 2B]` whose first passage below `B` lands in a dangerous class; `O(X^{-1/600})` if `H_{3/2}(B) = O(1)` | correct (Hoelder; the first visit to `(B, 2B]` is dominated by `h(z)`, finitely many `z`); for `c = 1` it uses the local shell theorem of `entrance/`; only early returns; the hypothesis is open | superseded by Proposition 15 |

The left half does not touch the 2-adic collision criterion `Q` (Remark 7): that concerns residues
mod `2^L` of the entrance law, not the 3-adic profile.

## Proposition 15 (what the right half becomes)

After the first passage below `X_1`, every visit to `W_X` lies in an excursion above `X_1` that starts
with an odd step from some odd `u in ((2X_1-c)/3, X_1]`. The starting values through `u` have
logarithmic density `h(u)`, and there are finitely many `u`. Counting the points of an excursion in
the shells of `W_X` as in the proof of Proposition 14 (bands `J`, rigidity, classes `C_{X,b}`) gives

    J_X/X <= 2b sum_{u in G_X} h(u) + 2(1+b) g_X + (8/X) sum_{X_1<a<=X, a odd, a in C_{X,b}} a h(a),

where `G_X` is the set of starts whose excursion climbs above `(Y-|c|)/3`
(`|G_X| <= 6 X^{-beta} X_1`: Doob for `3^q/2^t` and a Chernoff bound for survival over
`T_0 = floor(log_2 X_1) - 2` steps, on `u mod 2^{T_0}` by Terras' bijection), and `g_X -> 0` is the
upper log density of starting values whose orbits never come below `X_1` (diagonal argument from the
theorem on almost bounded orbits). Consequences:

- (UI) with the full potential `h` instead of `h^downarrow` gives `J_X = o(X)`; with Proposition 14,
  `T_X + J_X = o(X)`, i.e. the upper bound (eq. upper), under the hypotheses of Proposition 12(c).
- The entrance law of the returns is never used, so the transfer problem of Remark 7 (Cesaro limit
  over source shells, the class `-5*3^{-1}`) is bypassed.
- `h` (not `h^downarrow`) is necessary: an unbounded orbit of positive potential forces
  `J_X >= h(a)(X - 2Y)` and violates (UI) with `h`.
- Under a moment bound with `1 < p < 2` the first term is `O(b X^{-beta(1-1/p)})`.

What stays open: (UI) itself (for `h`), and the hypotheses of Propositions 8 and 12 (the coupling
lemmas). Problem 3 is not solved; it is implied by (UI), as before.

## Checks

- `verify_return_domination.py` -> `verify_return_domination_output.txt`: pathwise check of the
  counting on 20,000 sampled orbits per map (`c = 1, -1, 5, 13`; `X = 2^36, beta = 0.04` and
  `X = 2^48, beta = 0.03`, larger `beta` than in the paper so that `W_X` has two shells and returns
  are frequent, about 28,000 excursions per run): (1) points of an excursion in a shell <= odd points
  of the band: no violation; (2) odd band points followed by `b` further band points lie in
  `C_{X,b}`: no violation for `b = 8, 12, ..., 34` (up to 3,525 points per run at `b = 8`);
  (3) the pathwise bound with `b = 16`: no violation, largest ratio summand/bound 0.45.
- `excursion_mass.py` -> `excursion_mass_output.txt` (Run K counts, `results/potential_ui/counts_c*.npz`):
  for the odd starts with `X_1 = 2^20, 2^22`, the share of the potential on the starts whose excursion
  climbs to `s u` or higher divided by their share is 0.95-1.04 (`s = 2, 4, 8, 16, 32, 64`, four maps),
  one exception 1.15 (`3x+5`, `X_1 = 2^20`, `s = 32`); the share itself is 1/2 for `s = 2` (if `T(u)` is
  even the excursion ends at once, if odd the next point exceeds `2u`) and falls like `0.6/s` for `s >= 4`.
- `lp_shell_means.py` -> `lp_shell_means_output.txt` (Run K counts): `L^p` means of `v = a h(a)` on the
  windows `(X/2, X]`, all and odd targets, `p = 1.25, 1.5, 1.75, 2`: for odd targets and
  `2^14 <= X <= 2^22` the `L^{3/2}` means are 10.1-14.7 for all four maps; from `2^16` to `2^22` they
  increase by 3-25 %, and single large values move them by up to about 30 % at `2^14`; the `L^2`
  means are larger and less stable (up to 38.6). Second part: the right half of the diagram for `B = 2^20` with
  "dangerous" = growth `>= 2^s` within 14 steps: h-weighted share / uniform share 0.97-1.13.
- Independent review of the proposition (separate agent, read-only, with its own brute-force check of
  "points in a shell <= odd band points": 56,370 cases, c = 1, -1, 5, 13, no violation). Result: main
  inequality, `|G_X|` bound (Chernoff factor `(zeta/(1-zeta))^{2-zeta} = 2.08 <= 3`,
  `1 - H(zeta) = 0.05004`), `g_X -> 0` and the consequence correct. Fixed after the review:
  (1) the paragraph after the proof claimed that `h` is *needed*; now: `h^downarrow` cannot replace `h`
  without a separate argument excluding unbounded orbits of positive potential, and (UI) with `h`
  excludes them by itself; (2) "points at or before `w^flat`" (the start `w^flat` is not in a block) and
  "all points if `tau = infinity`"; (3) the last excursion may be infinite; (4) the diagonal argument for
  `g_X -> 0` written out; (5) conditionality on Proposition 12(c) stated in the discussion and the
  Outlook, the status column of the contribution table now lists `J_X`, the end of Remark 7 mentions the
  orbits that never come below `X_1`; (6) Run K: share 1/2 at `s = 2`, `0.6/s` only for `s >= 4`;
  (7) Run K: the `L^{3/2}` range described with its variation; minor wording (definition of `G_X` as a
  property of `u`, times `1..T_0`, Hoelder for every `p > 1`).

## Diagram script

`syracuse_regularization_transfer_diagram.py` is the received script with two changes: the line
breaks in the text labels were written as `\\n` (rendered literally) and are now `\n`, and the output
directory is a command-line argument instead of `/mnt/data`. Layout unchanged.
