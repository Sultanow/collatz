#!/usr/bin/env python3
"""Independent checks: return ladders for T_{3,c} (c = 1, -1, 5, 13) with the paper's beta = 1/200,
and the 'live but no return' family for c = 1. Exact integers; the parity word is realised bit by bit
(Terras lift), independently of the supplied scripts."""
import math
from fractions import Fraction

BETA = Fraction(1, 200)


def T(x, c):
    return (3 * x + c) // 2 if x & 1 else x // 2


def realise(word, c):
    """smallest a in [2^d, 2^{d+1}) whose first d parities are `word` (lift one bit at a time)."""
    r, mod = 0, 1
    for i, bit in enumerate(word):
        for cand in (r, r + mod):          # the two lifts modulo 2*mod
            x = cand + 2 * mod * 4         # any positive representative of the class
            for _ in range(i):
                x = T(x, c)
            if (x & 1) == bit:
                r = cand
                break
        else:
            raise AssertionError("no lift")
        mod *= 2
    return mod + r


def below_X1(x, X):                       # x <= X^{1-2 beta}  <=>  x^200 <= X^198
    return x ** 200 <= X ** 198


def ladder(b, c):
    ratio, word = Fraction(1), []
    for _ in range(b):
        if ratio <= 1:
            word += [1, 1, 0]; ratio *= Fraction(9, 8)
        else:
            word += [1, 0]; ratio *= Fraction(3, 4)
    m = len(word)
    r = -(-(m + 2) // 99) + 2
    a = realise(word + [0] * r, c)
    X = 6 * a // 5
    x, visits, steps = a, 0, 0
    while not below_X1(x, X):
        if X < 2 * x <= 2 * X:
            visits += 1
        x = T(x, c); steps += 1
    assert X ** 199 < (X // 2 + 1) ** 200          # (X/2, X] lies in W_X
    return X.bit_length(), visits, steps


for c in (1, -1, 5, 13):
    for b in (100, 300):
        bits, visits, steps = ladder(b, c)
        assert visits >= b + 1 and bits < 100 * b / 33 + 7, (c, b, bits, visits)
        print(f"ladder c={c:>3} b={b}: log2 X = {bits}, visits to (X/2,X] before X^0.99: {visits}, ratio {visits / bits:.3f}")

# live but no return (c = 1): a = 2^s v 3^t - 1
for t in (2000, 5000):
    s = math.ceil((math.log(t) + 3) ** 2); r = math.ceil(t / 45); mod = 1 << (r + 1)
    v0 = ((1 + (1 << r)) * pow(pow(3, t + s, mod), -1, mod)) % mod
    v = next(w for w in (v0, v0 + mod) if ((1 << (t + s)) * w - 1) % 3)
    a = (1 << s) * v * 3 ** t - 1
    X = a
    x, odd_in_J, visits, first = a, 0, 0, True
    while not below_X1(x, X):
        if X < 2 * x <= 2 * X:
            visits += 1
        if not first and (x & 1) and 3 * x > X - 1 and x <= X:
            odd_in_J += 1
        first = False
        x = T(x, 1)
    print(f"live family t={t}: s={s} odd steps above X, visits to (X/2,X] = {visits}, later odd points in J = {odd_in_J}")
    assert visits == 2 and odd_in_J == 0
print("all checks passed")
