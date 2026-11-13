#!/usr/bin/env python3
"""Compare the backward-mass predictor (backward_mass.c) with measured basin shares (Run D).

Usage:
  python3 predictor_eval.py --list LANDSCAPE.csv [--all] > cycles.txt
  python3 predictor_eval.py LANDSCAPE.csv MASS_FILES... [--levels A:B] [--sweep W] [--csv FILE]

--list writes the primitive cycles ("c cycle_min") of the maps with at least two primitive
cycles (with --all: of all maps) as input for backward_mass. MASS_FILES are outputs of
backward_mass, either summaries (c,cycle_min,l,mass_0,avg_d1,avg_d3) or per-level masses
(c,cycle_min,d,mass_d, written with the option "levels"). With per-level masses the predictor
is the mean mass of the levels A..B-1 (--levels; default the last 6 levels, compared with the
last 12), and --sweep W also evaluates windows of W levels at increasing depth. The masses
are normalized within the primitive cycles of every map and compared with the measured
relative shares, for the maps with at least two primitive cycles. Baselines: equal shares,
shares proportional to the word length l, and the mass of level 0 alone. Reported: log-log
Pearson correlation, median and 90% quantile of |log(predicted/measured)|, mean absolute
share error, the number of maps whose largest basin is predicted correctly, the mean
within-map Spearman correlation, the stability of the predictor between two level windows,
and the errors by basin size.

With per-level masses the script also evaluates the absolute prediction
    density of the basin = KAPPA * (mean mass of a level window),  KAPPA = 3/log(4/3),
for all primitive cycles, without any normalization within the maps: the median and the
quartiles of (measured share)/(mean mass), which should equal KAPPA, and the median and the
90% quantile of |log(KAPPA * mass / share)|, for the same level windows.
"""
import csv
import math
import sys
from collections import defaultdict


def ranks(v):
    o = sorted(range(len(v)), key=lambda i: v[i])
    rk, i = [0.0] * len(v), 0
    while i < len(v):
        j = i
        while j + 1 < len(v) and v[o[j + 1]] == v[o[i]]:
            j += 1
        for t in range(i, j + 1):
            rk[o[t]] = (i + j) / 2
        i = j + 1
    return rk


def corr(x, y):
    n = len(x)
    mx, my = sum(x) / n, sum(y) / n
    a = sum((p - mx) * (q - my) for p, q in zip(x, y))
    b = math.sqrt(sum((p - mx) ** 2 for p in x) * sum((q - my) ** 2 for q in y))
    return a / b if b else float("nan")


def quant(v, q):
    v = sorted(v)
    return v[min(len(v) - 1, int(q * len(v)))]


def main():
    argv = sys.argv[1:]
    if not argv:
        print(__doc__)
        return
    meas, absolute = {}, {}
    src = argv[1] if argv[0] == "--list" else argv[0]
    for r in csv.DictReader(open(src)):
        if r["m"] == "1":
            meas[(int(r["c"]), int(r["cycle_min"]))] = (float(r["rel_share"]), int(r["l"]))
            absolute[(int(r["c"]), int(r["cycle_min"]))] = float(r["share"])
    if argv[0] == "--list":
        count = defaultdict(int)
        for c, mn in meas:
            count[c] += 1
        for c, mn in sorted(meas):
            if count[c] >= 2 or "--all" in argv:
                print(c, mn)
        return
    out_csv = levels = sweep = None
    if "--csv" in argv:
        i = argv.index("--csv"); out_csv = argv[i + 1]; del argv[i:i + 2]
    if "--levels" in argv:
        i = argv.index("--levels"); levels = tuple(int(x) for x in argv[i + 1].split(":")); del argv[i:i + 2]
    if "--sweep" in argv:
        i = argv.index("--sweep"); sweep = int(argv[i + 1]); del argv[i:i + 2]
    mass, per_level = {}, defaultdict(dict)
    for f in argv[1:]:
        for line in open(f):
            p = line.strip().split(",")
            if len(p) == 6:
                mass[(int(p[0]), int(p[1]))] = (float(p[3]), float(p[4]), float(p[5]))
            elif len(p) == 4:
                per_level[(int(p[0]), int(p[1]))][int(p[2])] = float(p[3])
    depth = None
    if per_level:
        depth = min(len(v) for v in per_level.values())
        A, B = levels if levels else (depth - 6, depth)
        def window(v, a, b):
            return sum(v[d] for d in range(a, b)) / (b - a)
        for k, v in per_level.items():
            mass[k] = (v[0], window(v, max(0, depth - 12), depth), window(v, A, B))
        print(f"per-level masses for {len(per_level)} cycles, depth {depth}; predictor: mean of the levels "
              f"{A}..{B - 1}, compared with {max(0, depth - 12)}..{depth - 1}")
    maps = defaultdict(list)
    for k in meas:
        if k in mass:
            maps[k[0]].append(k)
    multi = {c: ks for c, ks in maps.items() if len(ks) >= 2}
    missing = [k for k in meas if k not in mass]
    print(f"{len(meas)} primitive cycles, masses for {len(meas) - len(missing)}; "
          f"{len(multi)} maps with at least two primitive cycles\n")

    def evaluate(name, weight):
        P, R, absdev, logdev, top, within = [], [], [], [], 0, []
        for ks in multi.values():
            w = [weight(k) for k in ks]
            s = sum(w)
            if s <= 0:
                continue
            p = [x / s for x in w]
            r = [meas[k][0] for k in ks]
            P += p; R += r
            absdev += [abs(a - b) for a, b in zip(p, r)]
            logdev += [abs(math.log(a / b)) for a, b in zip(p, r) if a > 0 and b > 0]
            top += max(range(len(ks)), key=lambda i: p[i]) == max(range(len(ks)), key=lambda i: r[i])
            if len(set(p)) > 1:
                within.append(corr(ranks(p), ranks(r)))
        pairs = [(math.log(a), math.log(b)) for a, b in zip(P, R) if a > 0 and b > 0]
        wsp = sum(within) / len(within) if within else float("nan")
        print(f"{name:>30} | log-log r {corr([a for a, _ in pairs], [b for _, b in pairs]):.3f} "
              f"| median |log ratio| {quant(logdev, 0.5):.3f} | 90% {quant(logdev, 0.9):.3f} "
              f"| mean |share error| {sum(absdev) / len(absdev):.4f} | largest basin right {top}/{len(multi)} "
              f"| within-map Spearman {wsp:+.3f}")
        return P, R

    evaluate("equal shares", lambda k: 1.0)
    evaluate("proportional to l", lambda k: meas[k][1])
    evaluate("level-0 mass", lambda k: mass[k][0])
    evaluate("backward mass, window d1", lambda k: mass[k][1])
    evaluate("backward mass, window d3", lambda k: mass[k][2])
    if sweep and per_level:
        print(f"\nwindows of {sweep} levels at increasing depth:")
        for a in range(0, depth - sweep + 1, sweep):
            evaluate(f"levels {a}..{a + sweep - 1}",
                     lambda k, a=a: sum(per_level[k][d] for d in range(a, a + sweep)) / sweep)

    if per_level:
        kappa = 3 / math.log(4 / 3)
        keys = [k for k in per_level if absolute.get(k, 0) > 0]
        print(f"\nabsolute prediction, density = kappa * mean level mass, kappa = 3/log(4/3) = {kappa:.4f}, "
              f"{len(keys)} primitive cycles, no normalization:")
        def absolute_eval(a, b):
            ratio, dev = [], []
            for k in keys:
                m = sum(per_level[k][d] for d in range(a, b)) / (b - a)
                if m > 0:
                    ratio.append(absolute[k] / m); dev.append(abs(math.log(kappa * m / absolute[k])))
            print(f"{'levels %d..%d' % (a, b - 1):>30} | share/mass: median {quant(ratio, 0.5):.3f}, quartiles "
                  f"{quant(ratio, 0.25):.3f}..{quant(ratio, 0.75):.3f} | median |log ratio| {quant(dev, 0.5):.3f} "
                  f"| 90% {quant(dev, 0.9):.3f}")
        if sweep:
            for a in range(0, depth - sweep + 1, sweep):
                absolute_eval(a, a + sweep)
        absolute_eval(A, B)
        tot = defaultdict(lambda: [0.0, 0.0])
        for k in keys:
            tot[k[0]][0] += kappa * sum(per_level[k][d] for d in range(A, B)) / (B - A); tot[k[0]][1] += absolute[k]
        r = [p / q for p, q in tot.values()]
        print(f"  per map, sum of the predicted densities / sum of the measured shares: median {quant(r, 0.5):.3f}, "
              f"quartiles {quant(r, 0.25):.3f}..{quant(r, 0.75):.3f} ({len(r)} maps)")

    d = []
    for ks in multi.values():
        w1 = [mass[k][1] for k in ks]; w3 = [mass[k][2] for k in ks]
        d += [abs(math.log((a / sum(w1)) / (b / sum(w3)))) for a, b in zip(w1, w3) if a > 0 and b > 0]
    print(f"\npredictor stability between the windows: median |log ratio| {quant(d, 0.5):.3f}, 90% {quant(d, 0.9):.3f}")
    rows = []
    for ks in multi.values():
        w = [mass[k][2] for k in ks]; s = sum(w)
        for k, x in zip(ks, w):
            rows.append((k[0], k[1], meas[k][1], meas[k][0], x / s))
    print("errors by measured relative share (window d3):")
    for lo, hi in ((0.2, 1.01), (0.05, 0.2), (0.01, 0.05), (0.0, 0.01)):
        sel = [abs(math.log(p / r)) for c, mn, l, r, p in rows if lo <= r < hi and r > 0 and p > 0]
        if sel:
            print(f"  [{lo:.2f}, {hi:.2f}): {len(sel):3d} cycles, median |log ratio| {quant(sel, 0.5):.3f}, 90% {quant(sel, 0.9):.3f}")
    print("largest deviations (window d3):")
    for c, mn, l, r, p in sorted(rows, key=lambda t: -abs(math.log(t[4] / t[3])) if t[3] > 0 and t[4] > 0 else 0)[:6]:
        print(f"  3x+{c}, cycle {mn} (l={l}): measured {r:.4f}, predicted {p:.4f}")
    if out_csv:
        with open(out_csv, "w", newline="") as fh:
            w = csv.writer(fh)
            w.writerow(["c", "cycle_min", "l", "rel_share", "predicted"])
            w.writerows(rows)
        print(f"predictions written to {out_csv}")


if __name__ == "__main__":
    main()
