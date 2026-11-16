# Dyadic block leakage: status (2026-11-16)

## What went into the paper (v6e, Problem 3 in Appendix D.3)

- The survival s_{X,j}(n) = eta E_{X,j} Q_{X,j}^{n-1} 1 is nonincreasing in n. Grouping the return counts
  into the dyadic blocks (2^l K_{X,j}, 2^{l+1} K_{X,j}] gives
  D_X/2 <= E_X(L_X;K_X) <= B_{X,0} + D_X, with B_{X,l} = sum_j 2^{-j} K_{X,j} s_{X,j}(2^l K_{X,j}) and
  D_X = sum_{l>=1} 2^l B_{X,l}. So T_X = o(X) iff D_X -> 0.
- The sufficient condition of Problem 3 is now aggregate over the shells:
  B_{X,l} <= A 2^{-(1+kappa) l} mu_X(L_X) S_X gives T_X = O(X (log X)^{sigma - eps_0}).
  It follows from the ratio condition B_{X,l+1} <= q B_{X,l} with q < 1/2, and from the old per-shell
  bound, but it does not need the per-shell bound.
- One sentence on errors: an additive error e_{X,l} is admissible if sum_l 2^l e_{X,l} -> 0. An error of
  the size (log X)^{-eps_0} is not admissible; this is the barrier of Remark 8 in dyadic form.

## What stays here, and why

- The reduction is elementary (Cauchy condensation and a geometric series). It sharpens the sufficient
  condition but does not prove it. T_X = o(X) remains open, exactly as before.
- The flux identity B_l - B_{l+1} = sum_j 2^{-j} K_j eta E Q^{a-1}(1 - Q^a 1) is a one-line telescoping.
  It is correct but adds no information.
- The finite-window experiments are **not evidence** for the uniform condition:
  * beta = 0.13 instead of 1/200. With beta = 1/200 the window W_X = (X^{1-beta}, X] is a single shell
    for every X < 2^200, so the multi-shell condition cannot be tested at the paper's parameters at all.
  * The first excess is not a rare event in these windows. It occurs in 51 % of the starts at X = 8192
    with L = 3, and in 13 % at X = 10^6 with L = 10. In the theorem it has mass O((log X)^{-eps_0}).
  * Only 2 or 3 shells occur. At most 25 later visits per shell are observed, so B_l = 0 as soon as
    2^l K_j > 25. "Ratio < 1/2" is then a finite-size effect.
  * Reproduced: at beta = 1/200, X = 10^6 and L = 2, the first excess occurs 1 to 3 times in 40000 starts.

## Reproduction (2026-11-16)

    python3 verify_dyadic_identities.py      # PASS, 4000 rational tests, ~3 s
    python3 exhaustive_finite_window.py      # identical to exhaustive_finite_window_results.txt, ~3 s
    python3 finite_orbit_probe.py            # identical to finite_orbit_results.txt, ~2 s
    python3 finite_orbit_probe.py --beta 0.005 --level 2   # first excess 3/1/1/2 of 40000
