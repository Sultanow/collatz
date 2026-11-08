#!/usr/bin/env python3
"""Basin landscape of the maps 3x+c, 1 <= c <= 400, gcd(c,6) = 1 (Run D).

Usage:  python3 landscape.py out_landscape/passage_*.csv [--census FILE] [--csv FILE]

Adds the counts of all replicates of passage.c and describes every cycle by its parity
word, computed from the cycle itself: word length l, number k of odd steps, and the
multiplier m = gcd(cycle, c), so that the cycle is m times a primitive cycle of 3x+c/m
(Proposition 5). The relative share of a cycle is its share divided by the density
phi(c/m)/c of its gcd class, i.e. its share among the integers with the same gcd with c.
The script checks that the shares of every map add up to 1 and that every gcd class has
the density phi(c/m)/c (as a z-score), and it cross-checks the cycles with word length at most 40 against
the census of primitive cycles (default ../results/rational_cycles_q3_d1000_L40.csv).
"""
import csv
import glob
import math
import os
import sys
from collections import defaultdict


def phi(n):
    r, x, p = n, n, 2
    while p * p <= x:
        if x % p == 0:
            while x % p == 0:
                x //= p
            r -= r // p
        p += 1
    if x > 1:
        r -= r // x
    return r


def main():
    argv = sys.argv[1:]
    here = os.path.dirname(os.path.abspath(__file__))
    census = os.path.join(here, "..", "results", "rational_cycles_q3_d1000_L40.csv")
    out_csv = None
    for opt in ("--census", "--csv"):
        if opt in argv:
            i = argv.index(opt)
            if opt == "--census":
                census = argv[i + 1]
            else:
                out_csv = argv[i + 1]
            del argv[i:i + 2]
    files = []
    for a in argv:
        files += sorted(glob.glob(os.path.join(a, "*.csv"))) if os.path.isdir(a) else sorted(glob.glob(a))
    if not files:
        print(__doc__)
        return

    counts = defaultdict(lambda: defaultdict(int))     # c -> cycle_min -> count
    total = defaultdict(int)                           # c -> samples
    for f in files:
        with open(f) as fh:
            for r in csv.reader(fh):
                if len(r) != 9 or r[3] not in ("log", "unif"):
                    continue
                c, n, cyc, cnt = int(r[0]), int(r[5]), int(r[6]), int(r[7])
                counts[c][cyc] += cnt
                if cyc == -1:
                    total[c] += n                       # one line per task carries its n

    prim = {}                                          # (c0, min) -> (l, k, D, word)
    with open(census) as fh:
        for r in csv.DictReader(fh):
            prim[(int(r["c"]), int(r["min"]))] = (int(r["l"]), int(r["k"]), int(r["D"]), r["word"])

    rows, mismatch, missing, sum_err, class_err, unresolved = [], [], [], 0.0, 0.0, 0
    nprim = nlong = 0
    for c in sorted(counts):
        n = total[c]
        unresolved += counts[c].get(-1, 0)
        cycles = sorted(cy for cy in counts[c] if cy != -1)
        sum_err = max(sum_err, abs(sum(counts[c][cy] for cy in cycles) / n - 1))
        by_class = defaultdict(float)
        seen = set()
        for cy in cycles:
            share = counts[c][cy] / n
            se = math.sqrt(share * (1 - share) / n)
            word, x = [], cy                        # parity word of the cycle, from its minimum
            while True:
                word.append("1" if x % 2 else "0")
                x = (3 * x + c) // 2 if x % 2 else x // 2
                if x == cy:
                    break
            l, k = len(word), word.count("1")
            m = math.gcd(cy, c)
            c0 = c // m
            rel = share * c / phi(c0)
            by_class[m] += share
            nprim += m == 1
            if l <= 40:
                seen.add((c0, cy // m))
                if (c0, cy // m) not in prim:
                    mismatch.append((c, cy))
            else:
                nlong += 1
            D = 2 ** l - 3 ** k
            rows.append([c, cy, f"{share:.6f}", f"{se:.6f}", m, c0, l, k,
                         D if l <= 60 else "", f"{rel:.6f}", "".join(word) if l <= 200 else ""])
        for m, f in by_class.items():               # gcd class m has density phi(c/m)/c (Prop. 5)
            rho = phi(c // m) / c
            class_err = max(class_err, abs(f - rho) / math.sqrt(rho * (1 - rho) / n) if rho < 1 else 0.0)
        for (c0, mn) in prim:
            if c % c0 == 0 and c0 > 0 and (c0, mn) not in seen:
                missing.append((c, mn * (c // c0)))

    print(f"maps: {len(counts)}, cycles: {len(rows)} ({nprim} primitive, {nlong} with word length > 40), "
          f"samples per map: {min(total.values()):,} to {max(total.values()):,}")
    print(f"unresolved orbits: {unresolved}")
    print(f"max |sum of shares - 1| per map: {sum_err:.2e}")
    print(f"gcd classes: share against density phi(c/m)/c, max |z| = {class_err:.2f}")
    print(f"cycles with l <= 40 that are not in the census: {mismatch if mismatch else 'none'}")
    print(f"census cycles of these maps that were not observed: {missing if missing else 'none'}")
    if out_csv:
        with open(out_csv, "w", newline="") as fh:
            w = csv.writer(fh)
            w.writerow(["c", "cycle_min", "share", "se", "m", "c0", "l", "k", "D", "rel_share", "word"])
            w.writerows(rows)
        print(f"landscape written to {out_csv}")


if __name__ == "__main__":
    main()
