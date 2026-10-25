#!/usr/bin/env python3
"""Cross-check of the census (results/rational_cycles_q3_d1000_L40.csv) with OEIS A226607
(primitive cycles of 3x+k found by Belaga, Mignotte and Morley), for all k < 1000.
Input: the first 1740 terms of A226607 (a226607_first1740.txt, transcribed from the b-file).
Run from this folder: python3 compare_oeis.py"""
import csv, math, sys, time
sys.path.insert(0, '../C')
from cycles_for_map import from_search

stream = [int(v) for v in open('a226607_first1740.txt').read().split()]
def T(x, k): return x // 2 if x % 2 == 0 else (3 * x + k) // 2
def cycle_info(x, k, limit=100000):
    """(is x the minimum of a cycle of T_k coprime to k, word length)"""
    if math.gcd(x, k) != 1: return (False, 0)
    y, l, mn = x, 0, x
    while True:
        y = T(y, k); l += 1
        if y == x: return (mn == x, l)
        if y < mn: mn = y
        if l > limit or y > 10**30: return (False, 0)

ks = [k for k in range(1, 1001) if k % 6 in (1, 5)]
rows, ki, pos = {k: [] for k in ks}, 0, 0
while pos < len(stream) and ki < len(ks):
    x = stream[pos]
    if rows[ks[ki]] and x <= rows[ks[ki]][-1][0]:      # rows are strictly ascending
        ki += 1; continue
    ok, l = cycle_info(x, ks[ki])
    if ok:
        rows[ks[ki]].append((x, l)); pos += 1
    else:
        ki += 1
print(f"stream used up to position {pos} of {len(stream)}; rows assigned for k=1..{ks[-1]}; empty rows: {sum(1 for k in ks if not rows[k])}")
if ki < len(ks) - 1: print("WARNING: stream too short for all k <= 1000")

census = {k: set() for k in ks}
for r in csv.DictReader(open('../results/rational_cycles_q3_d1000_L40.csv')):
    c0 = int(r['c'])
    if c0 > 0 and c0 in census: census[c0].add(int(r['min']))

t0 = time.time(); n_rows = 0
only_oeis_long, only_oeis_short, only_ours, agree_short, oeis_total = [], [], [], 0, 0
for k in ks:
    pass
    n_rows += 1
    R = {x for x, _ in rows[k]}; Rl = dict(rows[k]); oeis_total += len(R)
    S = {m for m in from_search(k, 100000) if math.gcd(m, k) == 1}
    U = census[k] | S
    short = {x for x in R if Rl[x] <= 40}
    if short == census[k]: agree_short += 1
    for x in short - census[k]: only_oeis_short.append((k, x, Rl[x]))
    for x in census[k] - R: only_ours.append((k, x, 'census'))
    for x in S - R: only_ours.append((k, x, 'search'))
    for x in R - U: only_oeis_long.append((k, x, Rl[x]))
print(f"rows compared: {n_rows} (k <= {ks[n_rows-1]}), OEIS cycles: {oeis_total}, census cycles (c0>0): {sum(len(census[k]) for k in ks[:n_rows])}  [{time.time()-t0:.0f} s]")
print(f"rows where OEIS cycles with word length <= 40 coincide exactly with the census: {agree_short}/{n_rows}")
print("OEIS cycles with l <= 40 missing in census:", only_oeis_short[:10], len(only_oeis_short))
print("our cycles missing in OEIS:", only_ours[:10], len(only_ours))
print("OEIS cycles not found by census or search n<1e5:", only_oeis_long)
import collections
print("OEIS cycles per word length > 40:", sorted(collections.Counter(Rl for k in ks for x, Rl in rows[k] if Rl > 40).items()))
print("rows with OEIS cycles only beyond l=40 (census empty):", sum(1 for k in ks if rows[k] and not census[k]))
