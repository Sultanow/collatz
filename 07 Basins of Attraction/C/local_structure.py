#!/usr/bin/env python3
"""Local structure of the forests of 3x+1 and 3x-1 (Section 2 of the paper).

Reproduces the local rows of Table 2 (branching classes mod 18, Leftward and
Upward classes mod 288, forest density), the residue-class examples of Table 3
(members of the same class in different trees of 3x-1), and the residues mod 13
in the proof of Corollary 2 (the near miss of type (8,5)).

Usage:  python3 local_structure.py
"""
from fractions import Fraction


def T(x, c):
    return x // 2 if x % 2 == 0 else (3 * x + c) // 2


def is_branching(n, c):
    """even n such that (n - c)/3 is an odd integer not divisible by 3"""
    return n % 2 == 0 and (n - c) % 3 == 0 and ((n - c) // 3) % 2 == 1 and ((n - c) // 3) % 3 != 0


def child_type(n, c):
    """'U' if n is an Upward child (n/2^r is a branching number before an odd number
    appears), 'L' if n is a Leftward child (Proposition 1)"""
    m = n // 2
    while True:
        if is_branching(m, c):
            return "U"
        if m % 2 == 1:
            return "L"
        m //= 2


def cycle_min(n, c):
    seen = set()
    while n not in seen:
        seen.add(n)
        n = T(n, c)
    m, x = n, T(n, c)
    while x != n:
        m, x = min(m, x), T(x, c)
    return m


print("Table 2, local rows")
for c in (1, -1):
    classes18 = sorted(r for r in range(18) if is_branching(r + 18 * 6, c))
    res288 = [r for r in range(288) if is_branching(r + 288 * 4, c)]
    kinds = {}
    for r in res288:                       # the type depends only on n mod 288
        t = {child_type(r + 288 * k, c) for k in range(1, 9)}
        assert len(t) == 1, (c, r, t)
        kinds[r] = t.pop()
    left = [r for r in res288 if kinds[r] == "L"]
    up = [r for r in res288 if kinds[r] == "U"]
    dens = Fraction(len(left), 288) + Fraction(len(up), 288)
    print(f"  3x{c:+d}: branching classes mod 18 {classes18}; Leftward classes mod 288: {len(left)}; "
          f"Upward classes mod 288 {up}; forest density {len(left)}/288 + {len(up)}/288 = 2/18 = {dens}")

print("\nTable 3 (3x-1): members of the same residue class in different trees")
for M, r in ((18, 14), (18, 2), (288, 2), (4608, 2)):
    inside, outside = [], None
    l = 0
    while outside is None:
        n = r + M * l
        if cycle_min(n, -1) == 1:
            inside.append((n, l))
        else:
            outside = (n, l, cycle_min(n, -1))
        l += 1
    if not inside:                         # first member in the tree of 2 after the outsider
        while True:
            n = r + M * l
            if cycle_min(n, -1) == 1:
                inside.append((n, l))
                break
            l += 1
    print(f"  {M}l+{r}: in the tree of 2: {', '.join(f'{n} (l={k})' for n, k in inside)}; "
          f"first member outside: {outside[0]} (l={outside[1]}), cycle through {outside[2]}")

print("\nCorollary 2 (near miss): minima of the cycles of 3x+13 of type (8,5) mod 13")
minima = [211, 227, 251, 259, 283, 287, 319]
print("  " + ", ".join(f"{m} = {m % 13} mod 13" for m in minima))
assert all(m % 13 for m in minima)
