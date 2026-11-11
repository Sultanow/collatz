#!/usr/bin/env python3
"""Brute-force reference for potential.c:  ref_potential.py C N A  prints a,count,cycle_min."""
import sys
c, N, A = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
T = lambda n: n // 2 if n % 2 == 0 else (3 * n + c) // 2
cnt = [0] * (A + 1); cyc = [0] * (A + 1)
for n in range(1, N + 1):
    seen = []; s = set(); x = n
    while x not in s:
        seen.append(x); s.add(x); x = T(x)
    i = seen.index(x); mn = min(seen[i:])
    for v in seen:
        if v <= A: cnt[v] += 1
    for v in seen[i:]:
        if v <= A: cyc[v] = mn
print("a,count,cycle_min")
for a in range(1, A + 1): print(f"{a},{cnt[a]},{cyc[a]}")
