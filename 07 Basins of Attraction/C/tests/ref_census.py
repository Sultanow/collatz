#!/usr/bin/env python3
"""Independent reference for census_all.c (exact rational arithmetic, small L only).

Usage:  python3 tests/ref_census.py q DMAX L    -> CSV lines c,min,l,k,D,word (no header)
"""
import sys
from fractions import Fraction
from itertools import product

q, dmax, L = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
found = set()
for l in range(1, L + 1):
    for bits in product((0, 1), repeat=l):
        k = sum(bits)
        if k == 0:
            continue
        a, b = Fraction(1), Fraction(0)          # composition x -> a*x + b, for c = 1
        for bit in bits:
            if bit:
                a, b = q * a / 2, (q * b + 1) / 2
            else:
                a, b = a / 2, b / 2
        x1 = b / (1 - a)                          # periodic point of 3x+1 with word bits
        d0 = x1.denominator
        if d0 > dmax:
            continue
        c0 = d0 if x1 > 0 else -d0
        x = abs(x1.numerator)
        y, members = x, []
        for bit in bits:
            if (y & 1) != bit:
                raise SystemExit(f"parity mismatch for {bits}")
            members.append(y)
            y = (q * y + c0) // 2 if y & 1 else y // 2
        if y != x:
            raise SystemExit(f"not periodic: {bits}")
        if len(set(members)) != l or x != min(members):
            continue                              # not primitive or not the minimal rotation
        found.add((c0, x, l, k, 2 ** l - q ** k, "".join(map(str, bits))))
for r in sorted(found, key=lambda r: (abs(r[0]), r[0] < 0, r[1])):
    print(",".join(map(str, r)))
