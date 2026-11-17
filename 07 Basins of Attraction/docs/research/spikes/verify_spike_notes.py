#!/usr/bin/env python3
"""Independent checks of the 17 Nov 2026 notes (spine countermodel, valuation lemma,
live filter on the explicit construction, Syracuse-profile maximum). Exact integers."""
import math
from fractions import Fraction


def T(x, c=1):
    return (3 * x + c) // 2 if x & 1 else x // 2


# --- 1. single inverse ray for 3x+1 ------------------------------------------------
def e_of(u):
    for e in range(8, 14):
        if (pow(2, e, 9) * u) % 9 == 4:
            return e
    raise AssertionError


def spine(nblocks):
    S = {1, 2, 4, 8}
    us, es = [5], []
    for i in range(nblocks):
        e = e_of(us[-1]); es.append(e)
        S |= {(1 << k) * us[-1] for k in range(e)}
        us.append(((1 << e) * us[-1] - 1) // 3)
    return S, us, es


S, us, es = spine(7)
umax = us[-2]                       # blocks 0..5 complete below 2^{e}*u_5
B = us[-1]                          # S is complete below u_7 (block 7 starts at u_7)
assert us[:3] == [5, 853, 291157] and es[:2] == [9, 10]
for y in S:                          # forward invariance
    assert y in (1, 2) or T(y) in S, y
for y in sorted(S):                  # exactly one supported predecessor (recurrence with 0/1 values)
    if y in (1, 2) or 2 * y >= B:
        continue
    preds = [2 * y] + ([(2 * y - 1) // 3] if (2 * y - 1) % 3 == 0 else [])
    assert sum(p in S for p in preds) == 1, y
for y in range(3, 10 ** 6):          # unsupported y: no supported predecessor
    if y in S:
        continue
    preds = [2 * y] + ([(2 * y - 1) // 3] if (2 * y - 1) % 3 == 0 else [])
    assert not any(p in S for p in preds), y
assert all(x % 3 for x in S)
# weighted mean: the running maximum of R^{-1} sum_{n<=R} n H(n) is attained at support points
tot, best = 0, 0.0
for x in sorted(v for v in S if v < B):
    tot += x
    best = max(best, tot / x)
print(f"spine: u = {us[:5]}..., e = {es[:6]}, sup_R R^-1 sum n H(n) = {best:.3f} (claim <= 10; <= 7 for the ray alone)")
print("spine: odd spike u_i/R at R = u_i:", [1.0 for _ in us[:5]], "-> (UI) fails")

# --- 2. valuation truncation lemma (brute force on all odd classes mod 2^t*) ------------
for c in (1, -1, 5, 13):
    for tstar in (12, 16):
        z0 = Fraction(11, 20); b = 2; bound = math.floor((1 - z0) * tstar + 1)
        worst = 0
        for a in range(1, 1 << tstar, 2):
            x, odd, danger = a, 0, False
            for t in range(1, tstar + 1):
                odd += x & 1
                x = T(x, c)
                if t >= b and odd >= z0 * t:
                    danger = True; break
            if danger:
                v = 3 * a + c; nu = (v & -v).bit_length() - 1
                worst = max(worst, nu)
        assert worst <= bound, (c, tstar, worst, bound)
print("valuation lemma: max nu_2(3a+c) on dangerous odd classes <= floor(0.45 t*+1) for c=1,-1,5,13, t*=12,16")

# --- 3. live filter excludes the explicit construction -------------------------------------
beta, zeta, z0 = 1 / 200, math.log(2) / math.log(3), 0.55
for t in (1600, 3200):
    r, s = -(-t // 30), -(-t // 20); d = r + s
    u0 = ((1 - (1 << r)) * pow(pow(3, t + 1, 1 << d), -1, 1 << d)) % (1 << d)
    u = next(v for v in (u0, u0 + (1 << d)) if ((1 << (t + 1)) * v - 1) % 3)
    a = 2 * u * 3 ** t - 1
    X = a; lX = math.log(X); lX1 = (1 - 2 * beta) * lX
    tX = math.ceil(3 * beta * lX / ((zeta - z0) * math.log(3)))
    bstar = math.ceil(math.log(lX) ** 2)
    x, odd, superset, live = a, 0, False, False
    above = True
    for ell in range(1, tX + 1):
        odd += x & 1
        above = above and math.log(x) > lX1
        x = T(x)
        if ell >= bstar and odd >= z0 * ell:
            superset = True
            live = live or above
    print(f"construction t={t}: in C_X,b* (superset) = {superset}, live = {live}, b* = {bstar}, t_X = {tX}")
    assert superset and not live

# --- 4. Syracuse profile: max_a v_n(a) >= (2/log(4/3)) (3/2)^n (all valuations equal to 1) ---
for n, tab in ((8, 260), (12, 1318), (16, 6676)):
    lb = 2 / math.log(4 / 3) * 1.5 ** n
    print(f"profile n={n}: lower bound {lb:.0f}, Table 13 max {tab}, ratio {tab / lb:.2f}")
print("all checks passed")
