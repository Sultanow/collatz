# Natural densities for all maps 3x+c: transfer audit (working note, 2026-11-16)

Target statement (would replace Remark 6 and settle Problem 2(a)):

> **Theorem.** Let c ≥ −1 be odd. Every set A ⊆ ℕ that is completely invariant under T_{3,c}
> has a natural density, and it equals δ(A). In particular every basin and every predecessor set
> of every map 3x+c has a natural density.

Status (v6): **integrated as Theorem 2 of the paper, conditional on Allikvere's Section 6.** The mapping
of Allikvere's imported Tao inputs to T_{3,c} is done (Tao §5 with the offset bound |cF_n| <= |c|3^n;
GGM §3–4 are the general-map versions with other parameters). The comparison of the uniform and the
logarithmic profile is Lemma 7 in Appendix E (fine-scale mixing between the scales m_1 and m_0, and the
bound C(N) << 1 from GGM, Step 3 of the proof of Prop. 3.5). Remaining risk: hidden uses of c = 1 in the
details of Allikvere §6.2–6.3 that are not visible at the level of statements; a referee should check
the window replacement (Lemma 6.4) in its two-sided form.

## 1. Route

1. Reduce to 3 ∤ c by Step 0 of the proof of Theorem 1 (unchanged).
2. Same block, two laws: for x large and y ∈ {x^α, x^{α²}}, compare Pass_x under
   U_y = Unif(odd ∩ [y, y^α]) and L_y = Log(odd ∩ [y, y^α]).
   * Allikvere, Thm 6.1 and §6.4: P(Pass_x(U_y) ∈ E) = (1+O(log^{−c} x)) Q(E) + O(log^{−c} x),
     Q(E) = Z^{−1} Σ_{M∈E′(E)} p_{m₁}(M)/M, with Z independent of y and E.
   * Tao (5.21) (GGM analogue for c): the same statement for L_y with p_{m₀} in place of p_{m₁};
     fine-scale mixing (Tao Prop. 1.14 / Allikvere Cor. 4.4) makes the two normalised profiles agree
     up to O(log^{−c} x).
   * Hence sup_E |P(Pass_x(U_y) ∈ E) − P(Pass_x(L_y) ∈ E)| ≪ log^{−c} x, plus the no-hit bound
     P(T_x(U_y) = ∞) ≪ x^{−c} (Allikvere (D.1)).
3. Conclude exactly as in the proof of Corollary 4 (natural density for 3x+1), with this comparison in
   place of Mazur's Theorem 3.1: P(U_y ∈ A₁) = P(L_y ∈ A₁) + o(1) → δ(A), and the block
   [N^{1/α}, N] carries all but O(N^{1/α−1}) of the uniform mass below N.

Side effect: for c = 1 this gives a second, independent proof of Corollary 4 (Mazur's Theorem 3.1
and Allikvere's Theorem 6.1 are different arguments).

## 2. Where c enters (Allikvere, section by section)

For T_{3,c}: Syr^n(N) = 3^n 2^{−|a|} N + c·F_n(a). The valuation law is the same for every odd c.

| Allikvere | content | dependence on c | verdict |
|---|---|---|---|
| §2, (1) | Syr^n = Aff_a, X_n = Syrac mod 3^n | offset c·F_n; X_n ↦ c·X_n | replace X_n by cX_n |
| §2.1 | Chernoff, local limit for s_n = \|a\| | none (valuation law) | unchanged |
| §3, Thm 3.1 | sum-conditioned Fourier decay of X_n, 3 ∤ ξ | E[χ_ξ(cX_n)\|s] = E[χ_{cξ}(X_n)\|s], 3 ∤ cξ | invariant (c a unit mod 3^n) |
| §4, Thm 4.1, Prop 4.2, Lemma 4.3, Cor 4.4 | oscillation of conditional laws mod 3^m | multiplication by c commutes with reduction mod 3^m and permutes residues | invariant; p_m ↦ p_m(c^{−1}·) |
| §5, Lemmas 5.1–5.2 | irrationality measure of log₂3, Kronecker orbit | none | unchanged |
| §6.1 | Tao Prop. 1.9 (valuations of 2-adically uniform inputs), (5.1)–(5.2), (5.10)–(5.15) | valuations: none; drift (5.13)–(5.15): offset c·F_n is a relative error O(\|c\| 3^{n} 2^{−\|a\|}/N) | **needs GGM analogues** of Tao (5.12)–(5.15) |
| §6.1, (D.1) | no-hit for U_y | Tao (1.13) with offset c·F_n | **check against GGM** |
| Prop. 6.2, (14) | deterministic first-passage identity; Tao Lemma 2.1 | Aff_a with offset cF; positivity of iterates needs c ≥ −1 | holds for odd c ≥ −1; **check GGM's Lemma 2.1 analogue** |
| §6.2 | fibre count: N = 2^s(M − cF)/3^{n′}, M ≡ cF (mod 3^{n′}) | congruence with cF; count = C(s−1,n′−1)·P(cX_{n′} ≡ M \| s) | invariant |
| §6.2, window replacement M − F → M (p. 21: "0 < F ≤ 3^n") | uses the sign F > 0 | for c = −1 the shift 2^s cF/3^n changes sign; the bound only needs \|cF\| ≤ \|c\|3^n | **rewrite with \|c\|, two-sided** |
| Lemma 6.3 | E′ equidistributed in residue classes; "Aff_b(M) > 0 always" | positivity holds for c ≥ −1 | holds |
| §6.3–6.4, Lemmas 6.4–6.5 | Kronecker-orbit evaluation, profile Q(E) | none | unchanged |

Nothing in §§3–6 uses c = 1 beyond the offset, the positivity of the iterates and one sign.

## 3. Open checks before this goes into the paper

1. Tao (5.12)–(5.15), (5.21), Prop. 1.9, Lemma 2.1, (1.13): locate the GGM analogues for
   T_{3,c} (GGM §§2–4) and confirm the constants used by Allikvere are available
   (needs both PDFs).
2. Write the two-sided window replacement (§6.2 of Allikvere) with |c|.
3. Confirm that Allikvere's coarse profile p_{m₁} and Tao's p_{m₀} give the same normalised Q(E)
   (Tao Prop. 1.14 for cX_n; for c = 1 this is implicit in Allikvere's remark that his profile is
   "the exact analogue of [9, (5.21)]").
4. Decide the presentation: a theorem "conditional on [Allikvere, Thm 6.1] and its proof, which
   transfers to T_{3,c} as listed in Table ...", with this audit as an appendix.

## 4. Mazur's route (for comparison)

Mazur's Theorem 3.1 compares flat and harmonic band profiles directly; c enters through the affine law
p_m (recursion b(a) = 3^{m−1} + 2^{a₁} b(…), for c: c·b) and through Tao's Section 5 schedule
(n₀ = ⌊log₂B/10⌋, scheduled no-hit 10B^{−1/32000}). The phase input (Rhin) is c-independent. The
transfer looks equally plausible, but the argument is longer and its Lean development is specific to
3x+1. Allikvere's route is the shorter one to audit.
