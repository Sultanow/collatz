#!/usr/bin/env python3
"""Independent check of the return ladders for T_{3,c}, c = 1, -1, 5, 13 (exact integers).
Counts visits to (X/2, X] and the orbit mass in W_X = (X^0.995, X] before the first value <= X^0.99."""
from fractions import Fraction
import math


C = 1


def T(x):
    return (3 * x + C) // 2 if x & 1 else x // 2


def residue_for_word(word):
    """Terras: the integers whose first len(word) parities are `word` form one class mod 2^len."""
    r, mod = 0, 1
    for k, bit in enumerate(word):
        # r realises word[:k]; choose the lift mod 2^(k+1) whose k-th parity is bit
        for cand in (r, r + mod):
            x = cand + 2 * mod * 4          # any positive member of the class (shift keeps parities)
            for _ in range(k):
                x = T(x)
            if (x & 1) == bit:
                r = cand
                break
        mod *= 2
    return r, mod


def below_cut(x, X99):          # x <= X^0.99  <=>  x^100 <= X^99
    return x ** 100 <= X99


def in_W(x, X, X199):           # x in (X^0.995, X]  <=>  x^200 > X^199
    return x <= X and x ** 200 > X199


def ladder(b):
    ratio, word = Fraction(1), []
    for _ in range(b):
        if ratio <= 1:
            word += [1, 1, 0]; ratio *= Fraction(9, 8)
        else:
            word += [1, 0]; ratio *= Fraction(3, 4)
        assert Fraction(3, 4) <= ratio <= Fraction(9, 8)
    m = len(word)
    r = -(-(m + 2) // 99) + 2
    res, mod = residue_for_word(word + [0] * r)
    a = mod + res                       # in [2^(m+r), 2^(m+r+1))
    X = 6 * a // 5
    X99, X199 = X ** 99, X ** 199
    x, visits, mass, steps = a, 0, 0, 0
    while not below_cut(x, X99):
        if X // 2 < x <= X:
            visits += 1
        if in_W(x, X, X199):
            mass += x
        x = T(x); steps += 1
    return dict(b=b, log2X=round(math.log2(X), 2), visits=visits,
                visits_over_log2X=round(visits / math.log2(X), 4),
                mass_over_X=round(mass / X, 1), odd_start=bool(a & 1),
                bound_log2X=(100 / 33) * b + 6 > math.log2(X), visits_ge_b1=visits >= b + 1)


for C in (1, -1, 5, 13):
    for b in (100, 300, 1000):
        d = ladder(b)
        print("c =", C, d)
        assert d["odd_start"] and d["bound_log2X"] and d["visits_ge_b1"] and d["mass_over_X"] >= 5 / 8 * b
print("ladders: all checks passed")
