#!/usr/bin/env python3
"""Decay of the Fourier maximum M_n = max_k |chi_n(2^k)| from the Monte Carlo runs (jobs B and B2).

usage: decay_analysis.py mc_merged.csv [mc_merged2.csv ...]

Per file: the levels without window-edge effect and with M_n >= 20 standard errors, and local ratios
per level on windows of 8 levels for
  * M_n (maximum over the k-window; biased upward by about 2.5 standard errors), and
  * the window energy P_n = sqrt(sum_k (|chi_hat_k|^2 - se_k^2)), whose square is unbiased.
Combined: the files are merged (a later file takes precedence where levels overlap), and five models
for log M_n are fitted by least squares:
  exponential a + b n, power a + b log n, quasi-power a + b (log n)^2,
  exponential times power a + b n + c log n, stretched exponential a - c n^g (g on a grid).
"""
import csv
import math
import sys
from collections import defaultdict

import numpy as np


def load(path):
    cells = defaultdict(list)
    for r in csv.DictReader(open(path)):
        cells[int(r["n"])].append((int(r["k"]), float(r["abs"]), float(r["stderr"])))
    out = {}
    for n, cs in cells.items():
        cs.sort()
        k_star, m, se = max(cs, key=lambda c: c[1])
        if k_star in (cs[0][0], cs[-1][0]) or m < 20 * se:
            continue
        energy = sum(a * a - s * s for _, a, s in cs)   # s^2 = variance of the complex mean = bias of |chi_hat|^2
        out[n] = (m, math.sqrt(max(energy, 0.0)), k_star, se)
    return out


def slope(xs, ys):
    xs, ys = np.asarray(xs, float), np.asarray(ys, float)
    X = np.c_[np.ones_like(xs), xs]
    c, *_ = np.linalg.lstsq(X, ys, rcond=None)
    r = ys - X @ c
    sb = math.sqrt((r @ r) / (len(xs) - 2) / ((xs - xs.mean()) @ (xs - xs.mean())))
    return c[1], sb


def main(paths):
    data = {}
    for p in paths:
        d = load(p)
        ns = sorted(d)
        print(f"== {p}: {len(ns)} levels {ns[0]}..{ns[-1]} (edge-free, M >= 20 se)")
        for name, idx in (("max M_n", 0), ("window energy P_n", 1)):
            print(f"  local ratio per level, {name}:")
            for i in range(0, len(ns) - 7, 4):
                w = ns[i:i + 8]
                b, sb = slope(w, [math.log(d[n][idx]) for n in w])
                print(f"    n = {w[0]}..{w[-1]}: {math.exp(b):.4f} +- {math.exp(b) * sb:.4f}")
        data.update(d)
    ns = np.array(sorted(data), dtype=float)
    y = np.log([data[int(n)][0] for n in ns])
    one = np.ones_like(ns)
    print(f"== combined: {len(ns)} levels {int(ns[0])}..{int(ns[-1])}, model fits of log M_n")
    models = {
        "exponential        a + b n": np.c_[one, ns],
        "power              a + b log n": np.c_[one, np.log(ns)],
        "quasi-power        a + b (log n)^2": np.c_[one, np.log(ns) ** 2],
        "exponential*power  a + b n + c log n": np.c_[one, ns, np.log(ns)],
    }
    for name, X in models.items():
        c, *_ = np.linalg.lstsq(X, y, rcond=None)
        r = y - X @ c
        extra = ""
        if "b n" in name:
            extra += f"  ratio e^b = {math.exp(c[1]):.4f}"
        if "c log n" in name:
            extra += f"  power c = {c[2]:.3f}"
        print(f"  {name:38s} RMS {math.sqrt(np.mean(r ** 2)):.4f}  max|res| {np.abs(r).max():.3f}{extra}")
    best = None
    for g in np.linspace(0.3, 1.0, 71):
        X = np.c_[one, ns ** g]
        c, *_ = np.linalg.lstsq(X, y, rcond=None)
        rms = math.sqrt(np.mean((y - X @ c) ** 2))
        if best is None or rms < best[0]:
            best = (rms, g)
    print(f"  {'stretched exp.     a - c n^g':38s} RMS {best[0]:.4f}  (best g = {best[1]:.2f})")
    print("  exponential*power on subranges:")
    for lo, hi in ((16, 50), (30, 80), (50, 80)):
        m = (ns >= lo) & (ns <= hi)
        X = np.c_[one, ns, np.log(ns)][m]
        c, *_ = np.linalg.lstsq(X, y[m], rcond=None)
        print(f"    n = {lo}..{hi}: ratio {math.exp(c[1]):.4f}, power {c[2]:.3f}")


if __name__ == "__main__":
    main(sys.argv[1:])
