#!/usr/bin/env python3
"""Exact finite check for the one-step dangerous Syracuse transfer cylinder.

Uses only Python integers to verify each claimed short orbit; runs on Python 3.8+.
"""
import math

def shortcut(n):
    return n // 2 if n % 2 == 0 else (3 * n + 1) // 2

beta = 1 / 200
for k in (20, 24, 28, 32):
    X = 2**k
    B = math.floor(X ** (1 - 2 * beta))
    Y = X ** (1 - beta)
    L = math.floor(math.log(X))
    modulus = 2**L
    s = (-5 * pow(3, -1, modulus)) % modulus
    first = s + ((B - s) // modulus + 1) * modulus
    candidates = range(first, 2 * B + 1, modulus)
    total = 0
    direct = 0
    for z in candidates:
        total += 1
        if 3 * z + 1 <= 4 * B:
            assert (3 * z + 1) % 4 == 0
            y = (3 * z + 1) // 4
            assert 3 * B < 4 * y <= 4 * B
            assert (y + 1) % (2 ** (L - 2)) == 0
            n = y
            for _ in range(L - 2):
                assert n % 2 == 1
                n = shortcut(n)
            assert n > Y
            direct += 1
    assert total > 0
    print(f'2^{k}: B={B}, L={L}, s={s}, '
          f'class={total}, dangerous_direct={direct}, '
          f'ratio={direct/total:.5f}')
