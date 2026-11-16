# Relative 2-adic energy / dangerous-cylinder criterion: status (2026-11-16)

## Audit

All statements of the note are correct.

| Note | Content | Verdict |
|---|---|---|
| Lemma 1 | return / survival cylinders are thin: <= 2^t e^{-Delta_0 t} classes | correct; strip + Terras + Chernoff (Remark 11) |
| Lemma 2 | nu(union of q classes) <= A q \|nu\|/2^t + d_t(nu;A) | correct, trivial |
| Thm 3 | single depth: s(n+b) <= q s(n) + d_{t_X}(nu_n;A) | correct; the useful observation is that the prefix of length t_X decides all shorter ones |
| Thm 4 | E_X <= (2b mu + D_X)/(1-q) | correct |
| Thm 5 | same with the excess on the dangerous classes C_X only | correct |
| Prop 6 | Fourier formula for that excess | correct (Fourier inversion) |
| Prop 7 | d <= \|nu\| chi^2/(A-1), Parseval | correct; chi^2 is genuinely stronger (Remark 8 of the note) |
| Prop 9 | d_t(nu;A) >= \|nu\|(1 - A r_t/2^t) | correct; the right direction of the support argument |

## The point the note misses: these criteria are equivalences

Since d_t(nu;A) <= \|nu\| and (nu(C_X) - alpha_X \|nu\|)_+ <= \|nu\| = s(n), both budgets are bounded by
E_X = sum_j 2^{-j} sum_{n>K_j} s_{X,j}(n). Hence

    D_X(A) <= E_X <= (2b mu_X + D_X(A))/(1-q),      C_X <= E_X <= (2b mu_X + C_X)/(1-alpha_X).

With mu_X -> 0 this gives T_X = o(X) <=> D_X(A) -> 0 <=> C_X -> 0, for every fixed A > 1.
So the note's labels are misleading. "D_X is a stronger sufficient condition" and "C_X is the weakest
target" are both off: all of them are equivalent to the open problem. Only the chi-square budget F_X
is strictly stronger.

**Consequence: no breakthrough.** The criterion is exactly as hard as T_X = o(X). What it adds is an
exact restatement at a single modulus 2^{t_X} = X^{O(beta)}. Up to O(mu_X), the clipping tail equals the
relative 2-adic concentration of the repeatedly conditioned return laws at that modulus.

## What went into the paper (v6g)

- Remark 11, second paragraph: replaced by one sentence (a single modulus suffices).
- New Proposition 13 (Single-depth concentration criterion), two-sided, hence an equivalence, with a
  short proof.
- One paragraph after it: equivalence, not a weaker target; the targeted version; the two sources of
  the defect.
  - Relative concentration of laws of mass O((log X)^{-eps_0}), against the absolute errors of Lemma 2.
  - Laws of the latest visits on few states. Few states force a defect of at least half the mass; the
    converse is false (heavy atoms). This corrects the direction of the support statement in v6f, as the
    note points out.

## Reproduction (2026-11-16)

    python3 check_energy_lemmas.py
    # PASS 9999 cap/chi/support, 10000 recurrence/tail, 500 Parseval,
    #      5000 event-defect, 300 event Fourier checks (numpy 2.5.3)

The note refers to the manuscript as `main(6).tex`; that is v6f.
