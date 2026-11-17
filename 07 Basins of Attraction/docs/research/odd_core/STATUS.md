# Odd-core compression: status (2026-11-17)

## Audit

All statements of the fragment are correct.

| Fragment | Content | Verdict |
|---|---|---|
| Lemma (even-run compression) | visits to (U/2,U] weigh at most U(2+3 O_U), O_U = odd points in ((U-c)/3,U] | correct; in fact at most U(1+O_U), since every visit is the first point, follows an odd point of the band, or ends an excursion above U that began with an odd step from ((2U-c)/3,U]; checked on 16 million trajectory prefixes |
| odd delayed-visit classification | an odd point of the band with b later odd points of the band lies in C_{X,b} | correct; the band has endpoint ratio < 4, so the strip of Remark 11 holds with a constant b_0 |
| Theorem (odd-core bound) | T_X/X <= (5+6b) mu + (36/X) sum over odd a in C of a h(a) | correct; with the sharper lemma and "at most two bands, 2^{-j} < 4a/X" the constants become 3+2b and 8 |
| Corollary (L^p) | Hoelder | correct |
| Proposition (dyadic harmonic spine) | H = indicator of powers of two solves the recurrence for c = 1 and fails all-target UI | correct, but formal: H is not the actual potential (actual 2^k h(2^k) <= 20 in the test data) |
| Proposition (dyadic flux) | h(a) = h(y) - h(2y) for odd a, y = T(a) not periodic; telescoping along dyadic rays | correct (Proposition 7(b) of the paper); the telescoping keeps the term h(2^{M+1} u) |

## Assessment

The task "control the full potential, including all doubling chains" is formally reduced to
"control the odd targets", that is, the dyadic differences h(y) - h(2y). This is a real but modest
improvement of Proposition 14:
- the hypothesis needs odd targets only;
- the bound is on T_X directly, without the allowances K_j;
- the constants are smaller.

It does not make the problem easier for the actual potential. The flux identity writes h(y) as
h(2^K y) plus the potentials of the odd predecessors along the dyadic ray, so heavy even targets come
from heavy odd targets. Run K confirms this. At X = 2^22, the shares of the mean above M = 20, 100 and
1000 differ between odd and all targets by at most 0.4 percentage points. The odd mass on the dangerous
classes equals their density to within 0.4 %. The open core is unchanged: a power-law tail (exponent
about 0.8) of the odd potential, which the Syracuse profile also has.

## In the paper (v6k)

- Proposition 14 is replaced by the odd-target version, with constants 3+2b and 8 and the charging
  argument in the proof.
- The discussion after it covers the dyadic difference, why doubling chains drop out, and why this
  changes little for the actual potential.
- Run K reports the odd-target shares and the odd mass on the dangerous classes; Table 12 has a row
  "odd" per map.
- Contributions table and Outlook say "of the odd targets".

Not in the paper: the spine countermodel (formal) and the telescoping bound.

## Reproduction

    python3 verify_odd_core.py     # PASSED, as in TEST_RESULTS.txt (about 3 s)

The fragment calls the manuscript `main(9).tex`; that is v6j.
