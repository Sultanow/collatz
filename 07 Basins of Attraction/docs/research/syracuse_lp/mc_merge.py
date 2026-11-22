#!/usr/bin/env python3
"""Merge the Monte Carlo files of syrac_fourier_mc and test the decay of M_n = max_k |chi_n(2^k)|.

usage: mc_merge.py OUTDIR mc_task0.csv [mc_task1.csv ...] [--exact fourier.csv]

Writes OUTDIR/mc_merged.csv (sample-weighted means per (n, k)) and OUTDIR/mc_summary.txt:
  * per n: M_n, the maximising k, the standard error, and (if --exact is given, n <= 20) the exact values;
  * least-squares fits of log M_n over the n with M_n >= 20 standard errors against
      exponential   log M = a + b n
      power         log M = a + b log n
      quasi-power   log M = a + b (log n)^2
    with RMS residuals, and the local power exponent d log M / d log n on windows of 8 levels.
The maximum over k of noisy values is biased upward by about 2.3 standard errors (13 values of k);
only levels with M_n >= 20 standard errors enter the fits. A maximum at the edge of the k-window is
only a lower bound for M_n (the maximiser may lie outside the window); such levels are flagged EDGE
and left out of the fits.
"""
import csv
import math
import sys
from collections import defaultdict


def lsq(xs, ys):
    n = len(xs)
    mx, my = sum(xs) / n, sum(ys) / n
    sxx = sum((x - mx) ** 2 for x in xs)
    b = sum((x - mx) * (y - my) for x, y in zip(xs, ys)) / sxx
    a = my - b * mx
    rms = math.sqrt(sum((y - a - b * x) ** 2 for x, y in zip(xs, ys)) / n)
    return a, b, rms


def main(argv):
    exact_path = None
    if "--exact" in argv:
        i = argv.index("--exact")
        exact_path = argv[i + 1]
        argv = argv[:i] + argv[i + 2:]
    outdir, files = argv[0], argv[1:]
    acc = defaultdict(lambda: [0.0, 0.0, 0.0, 0])        # sum N*re, sum N*im, sum N^2 se^2 , N
    for p in files:
        for r in csv.DictReader(open(p)):
            key = (int(r["n"]), int(r["k"]))
            N = int(r["samples"])
            a = acc[key]
            a[0] += N * float(r["re"])
            a[1] += N * float(r["im"])
            a[2] += (N * float(r["stderr"])) ** 2
            a[3] += N
    exact = {}
    if exact_path:
        for r in csv.DictReader(open(exact_path)):
            exact[(int(r["n"]), int(r["k"]))] = float(r["abs_chi"])
    rows = []
    with open(f"{outdir}/mc_merged.csv", "w") as f:
        f.write("n,k,re,im,abs,stderr,samples\n")
        for (n, k), (sr, si, sv, N) in sorted(acc.items()):
            re, im, se = sr / N, si / N, math.sqrt(sv) / N
            f.write(f"{n},{k},{re:.10e},{im:.10e},{math.hypot(re, im):.10e},{se:.3e},{N}\n")
            rows.append((n, k, math.hypot(re, im), se))
    best, kr = {}, {}
    for n, k, a, se in rows:
        if n not in best or a > best[n][0]:
            best[n] = (a, k, se)
        lo, hi = kr.get(n, (k, k))
        kr[n] = (min(lo, k), max(hi, k))
    edge = {n: best[n][1] in kr[n] for n in best}
    out = open(f"{outdir}/mc_summary.txt", "w")

    def P(s=""):
        print(s)
        out.write(s + "\n")

    P(f"files: {len(files)}; samples per cell: {next(iter(acc.values()))[3]}")
    P(" n   M_n          k*  k*-n*log2(3)  stderr     M/se    exact(n<=20)")
    for n in sorted(best):
        a, k, se = best[n]
        ex = exact.get((n, k))
        P(f"{n:3d} {a:.6e} {k:4d} {k - n * math.log2(3):+8.2f}   {se:.2e} {a / se:8.1f}   " +
          (f"{ex:.6e}" if ex is not None else "") + ("  EDGE" if edge[n] else ""))
    good = [n for n in sorted(best) if best[n][0] >= 20 * best[n][2] and not edge[n]]
    if len(good) >= 5:
        ys = [math.log(best[n][0]) for n in good]
        for name, xs in (("exponential  log M = a + b n", [float(n) for n in good]),
                         ("power        log M = a + b log n", [math.log(n) for n in good]),
                         ("quasi-power  log M = a + b (log n)^2", [math.log(n) ** 2 for n in good])):
            a, b, rms = lsq(xs, ys)
            P(f"fit {name}: b = {b:.4f}, RMS residual = {rms:.4f}  (n = {good[0]}..{good[-1]}, {len(good)} levels)")
        P("local power exponent d log M / d log n on windows of 8 levels:")
        for i in range(0, len(good) - 7, 4):
            w = good[i:i + 8]
            _, b, _ = lsq([math.log(n) for n in w], [math.log(best[n][0]) for n in w])
            _, be, _ = lsq([float(n) for n in w], [math.log(best[n][0]) for n in w])
            P(f"  n = {w[0]}..{w[-1]}: power exponent {b:.3f}, ratio per level {math.exp(be):.4f}")
    out.close()


if __name__ == "__main__":
    main(sys.argv[1:])
