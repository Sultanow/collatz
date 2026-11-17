# Stopped predecessor potential and the first-excess tail: status (2026-11-17)

## Audit

All statements of the note are correct.

| Note | Content | Verdict |
|---|---|---|
| Lemma 1 | sum_n nu_{X,j,n}({y}) <= h(y) 1[tau_{X_1}(y) < oo] | correct: a terminating orbit visits no state above X_1 twice; the same argument as Step 1 of the proof of Theorem 3 |
| Thm 2 | E_X <= 2b mu_X + (2/X) sum_{y in W_X cap C_{X,b}} y h_stop(y) | correct; no recursion and no 1/(1-q) needed |
| (sparse) | #(W_X cap C_{X,b})/X <= alpha_b + 2^{t_X}/X | correct; needs zeta_0 <= 0.6 so that 2^{t_X} = X^{delta}, delta < 1 (0.55 gives 0.117) |
| Thm 3 | Cesaro uniform integrability of y h_stop(y) => T_X = o(X) | correct |
| Cor 4 | L^p bound, p > 1 => T_X = o(X), with a rate | correct (Hoelder) |

## Assessment

This is the first of the recent notes that is not a restatement.
- Proposition 13 (v6g) is an equivalence: it bounds T_X by conditioned return laws whose defect is itself
  bounded by T_X.
- Here the bound is by the **unconditioned** potential on an arithmetically thin set. The criterion is
  one-way: sufficient, not necessary.
- The absolute-vs-relative barrier (Remark 8) does not enter the bound. The dangerous set is compared with
  the whole potential mass, not with the rare first-excess mass.

Not a breakthrough: the hypothesis (UI) is open. It contains a bounded mean of the stopped profile over
W_X, a constant-factor form of the upper bound (16) for targets whose orbits come down. But it says
nothing about unbounded orbits (all their points have h_stop = 0), unlike (16). So it is not at least
as hard as the divergence problem.

Heuristic support (added to the paper): Tao's fine-scale mixing (1.26) is an L^1 bound
||f_n - f_m||_{L^1(Z_3)} = Osc_{m,n} << m^{-A} for the densities f_n = 3^n P(Syrac(Z/3^n) = .).
So f_n converges in L^1, the Syracuse law on Z_3 has an integrable density, and the profile is
uniformly integrable. (UI) is what Conjecture 1 suggests for individual targets. An L^p bound with
p > 1 would need more than L^1; it is not suggested by (1.26).

## What went into the paper (v6h)

- Proposition 14 (Domination by the stopped potential), Appendix D.3, after Proposition 13.
  It states the bound and the consequence "(UI) => T_X = o(X)", with a short proof.
- One paragraph after it:
  - one-way, not an equivalence;
  - a weak form of (16) for stopped targets, but silent on unbounded orbits;
  - the Syracuse profile is uniformly integrable by Tao (1.26);
  - an L^p bound suffices;
  - (UI) is open.
- Contributions table, row "First-passage tails": one clause added.
- Outlook item (2): one clause added.

## Suggested test

Run G (potential.c, all targets a <= 10^6 from n <= 10^10) produces exactly the per-target counts needed
to test (UI): compute X^{-1} sum_{a in (X/2, X]} a h(a) 1[a h(a) > M] for M = 10, 20, 50 and several X,
and the share of the mass on the dangerous 2-adic classes. The raw counts are not in the repository.

The note refers to the manuscript as `main(8).tex`; that is v6g.
