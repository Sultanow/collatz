# Proof audit — Extreme-Spike Barrier (2026-11-17)

Status: **mathematical consequence, conditional on the consolidated manuscript's first-passage and odd-core propositions. NOT a proof of T_X=o(X).** Not merged into main.tex as a solution.

## Exact inputs

1. `main(9).tex`: β=1/200, ε₀ in (0,1), X₁=X^(1−2β), `μ_X(L_X) ≤ C_μ (log X)^(-ε₀)`.
2. `Odd_Core_Compression_Standalone.tex`: for all b₀≤b≤t_X,
   `T_X/X ≤ (5+6b) μ_X + (36/X) Σ_{odd a in (X₁/4,X] ∩ C_{X,b}} a h_X↓(a)`.
3. `Dyadic_Odd_Flux_Spine_Standalone.tex`: odd elements of C_{X,b}, b≥2, satisfy `ν₂(3a+c) ≤ floor((1−ζ₀)t_X+1)`.
4. `main(9).tex` and companion: C_{X,b} occupies ≤ X exp(−Δ b)/(1−exp(−Δ)) + 2^{t_X} integers in [1,X], with `2^{t_X} ≤ 2X^0.12` for the chosen constants.
5. By definition `0 ≤ h_X↓(a) ≤ 1`; hence `G_X ≤ X`.

## Proof core

Substitute the maximum `G_X` on the eligible dangerous odd states into the odd-core inequality, using the integer count in (4):

`T_X/X ≤ (5+6b) C_μ / L + 36 G_X [exp(−Δb)/(1−exp(−Δ))+2 X^(−0.88)]`,
with `L=(log X)^ε₀`.

- For `log(2+G_X)=o(L)`, choose b∼[log(2+G_X)+log log X]/Δ. Then b=o(L), b<t_X, and all error terms vanish.
- For `G_X≤(log X)^A`, choose b∼(A+2) log log X/Δ. Obtain an explicit log-log / log^ε bound.
- If `T_X/X≥δ` infinitely often, choose b∼δ L/(24 C_μ), valid because ε₀<1. The first term is ≤δ/3, while the `2 X^−0.88` remainder is smaller than exp(−Δb). Thus `G_X ≥ exp(κ L)` for some κ>0 on that subsequence.

## Scope

This is **not** another two-sided equivalence to a dangerous-cylinder correlation defect. It is a one-sided *pointwise sufficient criterion* and a strong necessary extreme-spike condition for failure. However, the missing pointwise inequality for the real potential has NOT been proved; it may be difficult and is not assumed true.

The arithmetic and Chernoff inputs are classical, so no independent novelty/priority claim is made. No Lean check has been performed. The generated three-page TeX was compiled twice without missing references or overfull boxes.

## Concrete research direction

For actual logarithmic predecessor densities, seek a target-uniform endpoint bound on **odd** a in `(X₁/4,X]` satisfying `ν₂(3a+c)≤0.45t_X+1` and whose forward orbit reaches the cut-off, of the form

`a h(a) ≤ exp(o((log X)^ε₀))`.

Pointwise estimates that hold only for a density-one subset of these targets will *not* automatically suffice; the very rare high-profile exceptions are the potential obstruction. No source currently reviewed here provides such a uniform upper bound.
