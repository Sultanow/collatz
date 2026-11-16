# Arithmetic rigidity of late returns: status (2026-11-16)

## Audit of the note

All statements are correct.

| Note | Content | Verdict |
|---|---|---|
| Lemma 1 | exact log-cocycle of T_{3,c}, error eps_a t with eps_a = \|c\|/((3a-\|c\|) log 3) | correct, elementary |
| Thm 2 | return to the same dyadic shell => \|H_t - rho t\| <= rho + eps_a t, at every time t | correct, elementary; new as a stated tool here |
| Lemma 3 | parities <-> residues mod 2^t, bijectively, for odd c | classical (Terras 1976, Everett 1977) |
| Prop 4 | return mass <= d_nu(t) exp(-t D(theta\|\|1/2)) | correct; Terras counting plus Chernoff |
| Prop 5 | finite support => d_nu(t) >= 2^t/M | correct, trivial; already Remark 9 of the paper |
| Prop 6 | survival <= strip local time + long excursions | correct; a union bound |

The paper already has the parity-cylinder balance (Lemma 6, cf. Terras) and the finite-support
obstruction (Remark 9). It also has the absolute-versus-relative barrier (Remark 8).

## Exact arithmetic condition?

No new one. The strip is a *necessary* condition for a return. The exact condition is classical.
A parity word w of length t with k odd entries fixes z mod 2^t. A return then means
T^t(z) = (3^k z + c F_w)/2^t lies in the shell, with all intermediate values above the cutoff
(Boehm-Sontacchi/Lagarias form, as used in Section 4 and in the proof of Lemma 6).

## What went into the paper (Remark 11, after Problem 3)

- The pathwise strip, at every time t.
- New in this form: survival above X_1 for t steps from W_X forces h_t >= zeta t - 2 beta log X/log 3.
  So for t >= t_X = ceil(3 beta log X/((zeta - zeta_0) log 3)), survival is suppressed like a return.
- Consequence: a law nu on a shell with relative 2-adic concentration <= A on the scales 2^t,
  t <= t_X, returns at time t, or survives t >= t_X steps, with mass <= A exp(-t Delta_0)|nu|.
- If the return laws eta E Q^m keep this bound, their masses halve within every block of
  b = b(A, zeta_0) returns. This is far more than Problem 3 asks.

## Where new mathematics is needed (our analysis, stated in Remark 11)

1. **Relative non-concentration.** The bound must hold relative to the mass of laws of total mass
   O((log X)^{-eps_0}). Tao/GGM and Lemma 2 control only absolute errors of that size, which is
   Remark 8 again. It is needed only on the 2-adic scales up to 2^{t_X} = X^{O(beta)}. This is far
   below the support size, so Remark 9 does not exclude it.
2. **The last few orbits.** The bound cannot persist. A law with fewer than 2^{t_X}/A atoms
   violates it on the scale 2^{t_X}. Once the surviving mass sits on at most X^{O(beta)} states,
   their later visits to W_X need a separate bound. No residue count can provide it, because
   (2^t/s) exp(-t Delta_0) < 1 needs s > exp(t(log 2 - Delta_0)). This is a statement about
   individual orbits.

So the route "residues + strip" reduces Problem 3 to (1) and (2). (1) is a statistical estimate
with relative errors. (2) is pathwise. Neither follows from Terras, Tao or GGM.

## Numerics

The diagnostics are not evidence for the asymptotic question. They use beta = 0.13 and only
2 shells. The first excess is not rare: about 51 % of the samples (10215 of 20000).
At most 25 later visits per shell occur. Stability of B_1/B_0 across starting altitudes X^1..X^4
says nothing about X -> infinity at fixed altitude ratio.

Reproduced on 2026-11-16:

    python3 verify_parity_strip.py                 # PASS for c = -1, 1, 5, 13 (0.2 s)
    g++ -std=c++17 -O3 multi_altitude.cpp -o multi_altitude
    ./multi_altitude 20000                         # identical to multi_altitude_results.txt (0.3 s)

The source was delivered twice under two names (Parity_Strip_Late_Returns_Standalone.tex and
Collatz_Arithmetic_Rigidity_Late_Returns.tex, byte-identical); it is kept once, under the name used
in README.md.
