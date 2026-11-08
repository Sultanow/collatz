#!/bin/bash
# Self-tests for passage (about 25 seconds). Run from the C/ folder: make test-passage
set -e
tmp=$(mktemp -d)
echo "1/5 Monte Carlo vs exact window shares, both counting modes (3x+5, [10^6, 10^6.6])"
./passage 5 6 1.1 exact 0 0 > "$tmp/exact.csv" 2>/dev/null
./passage 5 6 1.1 log 1000000 1 > "$tmp/log.csv" 2>/dev/null
./passage 5 6 1.1 unif 1000000 2 > "$tmp/unif.csv" 2>/dev/null
echo "2/5 first passage below a small table bound gives the same shares (B = 10^4)"
./passage 5 6 1.1 log 1000000 3 10000 > "$tmp/logB.csv" 2>/dev/null
echo "3/5 GMP path vs an independent Python reference (3x+5, uniform on [10^40, 10^44])"
./passage 5 40 1.1 unif 200000 4 > "$tmp/big.csv" 2>/dev/null
echo "4/5 cycles above the table bound are detected (B = 100: the cycles through 187 and 347)"
./passage 5 6 1.1 exact 0 0 100 > "$tmp/exactB.csv" 2>/dev/null
echo "5/5 residue sweep: the multiples of 5 form the basin of the cycle through 5 (3x+5, classes mod 5)"
./passage 5 20 1.1 log 2000 5 10000000 5 0 > "$tmp/sweep.csv" 2>/dev/null
python3 - "$tmp" << 'PY'
import csv, math, random, sys
tmp = sys.argv[1]
def rows(name):
    return [r for r in csv.reader(open(f"{tmp}/{name}")) if len(r) == 9]
def fail(msg):
    print("FAILED:", msg); sys.exit(1)
exact = {(r[3], int(r[6])): float(r[8]) for r in rows("exact.csv")}
for name, ref in (("log.csv", "exact_log"), ("unif.csv", "exact_unif"), ("logB.csv", "exact_log")):
    for r in rows(name):
        cyc, n, p = int(r[6]), int(r[5]), float(r[8])
        if cyc == -1:
            if int(r[7]): fail(f"{name}: {r[7]} unresolved orbits")
            continue
        e = exact[(ref, cyc)]
        z = (p - e) / math.sqrt(e * (1 - e) / n)
        if abs(z) > 4: fail(f"{name}: cycle {cyc} share {p:.5f} vs exact {e:.5f} (z={z:.2f})")
# independent reference: follow the orbits with Python integers
def syr(n):
    t = 3 * n + 5
    return t >> ((t & -t).bit_length() - 1)
def cycle_min(n):
    seen = set()
    while n not in seen:
        seen.add(n); n = syr(n)
    m, w = n, syr(n)
    while w != n:
        m, w = min(m, w), syr(w)
    return m
random.seed(11)
nref, cnt = 20000, {}
for _ in range(nref):
    m = cycle_min(random.randrange(10**40, 10**44) | 1)
    cnt[m] = cnt.get(m, 0) + 1
for r in rows("big.csv"):
    cyc, n, p = int(r[6]), int(r[5]), float(r[8])
    if cyc == -1:
        if int(r[7]): fail(f"big.csv: {r[7]} unresolved orbits")
        continue
    q = cnt.get(cyc, 0) / nref
    se = math.sqrt(p * (1 - p) / n + q * (1 - q) / nref)
    if se > 0 and abs(p - q) / se > 4: fail(f"big.csv: cycle {cyc} C {p:.4f} vs Python {q:.4f}")
# 4/5: identical exact counts with B = 100 and with the default table bound
a = sorted((r[3], r[6], r[7]) for r in rows("exact.csv"))
b = sorted((r[3], r[6], r[7]) for r in rows("exactB.csv"))
if a != b: fail("exact counts differ between B = 100 and B = 10^7")
# 5/5: class 0 mod 5 lies entirely in the basin of the cycle through 5, the other classes not at all
for r in csv.reader(open(f"{tmp}/sweep.csv")):
    if len(r) != 11 or r[6] != "5":
        continue
    share, cls = float(r[8]), int(r[10])
    if (cls == 0) != (share == 1.0) or (cls != 0 and share != 0.0):
        fail(f"sweep: class {cls} mod 5 has share {share} in the basin of 5")
PY
rm -rf "$tmp"
echo "all passage tests passed"
