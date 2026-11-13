#!/usr/bin/env python3
"""Evaluate the outputs of orbit_profile.c (Run H) against Conjecture 1 of the paper.

Usage:  orbit_profile_report.py orbit_profile_c*.txt > report.txt

For every map: the mean of a*h(a) in every decade of the targets against 2/log(4/3), the
mean and the scatter over the decades k >= KPOOL, and the means over the residue classes of
a/c modulo 3, 9, 27 and 81 (pooled over these decades) against
(2/log(4/3)) * 3^n * P(Syrac(Z/3^n) = s), the law of Tao's Syracuse random variable.
"""
import math
import re
import sys
from statistics import mean, median, stdev


def syracuse_law(n, amax=80):
    """Law of Syrac(Z/3^n) by the recursion Syrac' = (3*Syrac + 1) * 2^(-Geom(2)) (Tao 2022)."""
    p = [1.0]
    for k in range(n):
        mod = 3 ** (k + 1); q = [0.0] * mod
        for s in range(mod):
            if s % 3 == 0: continue
            pw = 1
            for a in range(1, amax + 1):
                pw = (pw * 2) % mod
                t = (pw * s) % mod
                if t % 3 == 1: q[s] += 0.5 ** a * p[(t - 1) // 3]
        p = q
    return p


def report(path):
    K = 2 / math.log(4 / 3)
    dec, cls, head = {}, {}, ""
    for line in open(path):
        if line.startswith("#"): head = line.strip(); continue
        p = line.split()
        if p[0] == "decade": dec[int(p[1])] = float(p[2])
        elif p[0] == "class": cls[int(p[1])] = float(p[2])
    m = re.search(r"c=(-?\d+) S=(\d+) E=(\d+).*kpool=(\d+)", head)
    c, S, E, kpool = int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4))
    print(f"== {path}: c={c}, {S:.0e} starting values in [1e{E}, 2e{E}); 2/log(4/3) = {K:.4f}")
    print("  mean of a*h(a) by decade 10^k of the targets:")
    print("    " + "  ".join(f"k={k}: {dec[k]:.4f}" for k in sorted(dec) if k < kpool))
    pooled = [dec[k] for k in sorted(dec) if k >= kpool]
    print("    " + "  ".join(f"{k}: {dec[k]:.4f}" for k in sorted(dec) if k >= kpool))
    print(f"  decades k >= {kpool}: mean {mean(pooled):.4f}, standard deviation between decades {stdev(pooled):.4f}, "
          f"relative deviation of the mean from 2/log(4/3): {mean(pooled) / K - 1:+.2e}")
    for n in (1, 2, 3, 4):
        mod = 3 ** n; law = syracuse_law(n)
        dev, rows = [], []
        for s in range(mod):
            if s % 3 == 0: continue
            meas = mean(cls[t] for t in range(81) if t % mod == s)
            pred = K * mod * law[s]
            dev.append(abs(math.log(meas / pred))); rows.append((s, meas, pred))
        print(f"  classes of a/c mod {mod}: median |log ratio| {median(dev):.4f}, max {max(dev):.4f}")
        if n == 2:
            print("    s: measured / predicted  " + "  ".join(f"{s}: {a:.3f}/{b:.3f}" for s, a, b in rows))
    print()


if __name__ == "__main__":
    if len(sys.argv) < 2: print(__doc__); sys.exit(1)
    for p in sys.argv[1:]: report(p)
