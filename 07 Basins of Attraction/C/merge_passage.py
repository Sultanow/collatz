#!/usr/bin/env python3
"""Merge passage.c outputs: add the counts and print the basin shares.

Usage:  python3 merge_passage.py FILES... [--csv shares.csv] [--pool Y] [--replicates]

FILES are raw outputs (out/passage_*.csv) or the summary CSV written by --csv
(e.g. ../results/passage/passage_shares.csv). For every map 3x+c the script
prints, per cycle, the brute-force share d_{10^10} (Table 6 of the paper, four
digits) and, per window [10^y, 10^(alpha*y)], the logarithmic and uniform shares
with standard errors. Two z-scores are reported per cell: log share vs. d_{10^10},
and uniform vs. log share (natural against logarithmic counting, Problem 2(a)).
Chi-square tests check that the shares do not depend on the scale, and that both
counting modes agree.

  --pool Y       also print the shares pooled over all windows with log10y >= Y,
                 per counting mode (Table 7 of the paper: --pool 20)
  --replicates   chi-square test per configuration that the replicates (seeds)
                 scatter like independent binomial samples (raw outputs only)
"""
import csv
import glob
import math
import os
import sys
from collections import defaultdict

# Table 6 of the paper: d_{10^10} per cycle minimum, rounded to four digits.
# The shares 1/5 (cycle through 5 of 3x+5) and 1/13 (cycle through 13 of
# 3x+13) are exact (Proposition 6) and serve as controls of the sampler.
REF = {
    -1: {1: 0.3270, 5: 0.3250, 17: 0.3480},
    5: {1: 0.1403, 5: 0.2, 19: 0.4979, 23: 0.0922, 187: 0.0325, 347: 0.0370},
    13: {1: 0.4775, 13: 1 / 13, 131: 0.2517, 211: 0.0334, 227: 0.0189,
         251: 0.0198, 259: 0.0386, 283: 0.0253, 287: 0.0433, 319: 0.0145},
}
EXACT = {(5, 5), (13, 13)}
ROUND_SD = 0.00005 / math.sqrt(3)   # rounding of the four-digit references


def gammq(a, x):
    """Regularized upper incomplete gamma Q(a, x)."""
    if x <= 0:
        return 1.0
    gln = math.lgamma(a)
    if x < a + 1:                     # series for P, then Q = 1 - P
        ap, s, d = a, 1.0 / a, 1.0 / a
        for _ in range(1000):
            ap += 1
            d *= x / ap
            s += d
            if abs(d) < abs(s) * 1e-15:
                break
        return max(0.0, 1.0 - s * math.exp(-x + a * math.log(x) - gln))
    b, c, d = x + 1 - a, 1e300, 1.0 / (x + 1 - a)   # continued fraction for Q
    h = d
    for i in range(1, 1000):
        an = -i * (i - a)
        b += 2
        d = an * d + b
        d = 1e-300 if abs(d) < 1e-300 else d
        c = b + an / c
        c = 1e-300 if abs(c) < 1e-300 else c
        d = 1.0 / d
        h *= d * c
        if abs(d * c - 1) < 1e-15:
            break
    return math.exp(-x + a * math.log(x) - gln) * h


def homogeneity(table):
    """Chi-square test of homogeneity for a list of count rows."""
    rows = [r for r in table if sum(r) > 0]
    cols = [sum(r[j] for r in rows) for j in range(len(rows[0]))] if rows else []
    keep = [j for j, t in enumerate(cols) if t > 0]
    if len(rows) < 2 or len(keep) < 2:
        return float("nan"), 0, float("nan")
    tot = sum(cols)
    chi = 0.0
    for r in rows:
        rs = sum(r)
        for j in keep:
            e = rs * cols[j] / tot
            chi += (r[j] - e) ** 2 / e
    dof = (len(rows) - 1) * (len(keep) - 1)
    return chi, dof, gammq(dof / 2, chi / 2)


def read_rows(paths):
    files = []
    for p in paths:
        if os.path.isdir(p):
            files += sorted(glob.glob(os.path.join(p, "*.csv")))
        else:
            files += sorted(glob.glob(p))
    for f in files:
        with open(f) as fh:
            summary = False
            for r in csv.reader(fh):
                if r and r[0] == "c" and "cycle_min" in r:
                    summary = True           # summary CSV written by --csv
                    continue
                if summary and len(r) == 11:
                    c, sc, al, cyc = r[0], r[1], r[2], r[3]
                    for mode, n, share in (("log", r[5], r[6]), ("unif", r[8], r[9])):
                        cnt = round(float(share) * int(n))
                        yield [c, sc, al, mode, "summary", n, cyc, str(cnt), share]
                elif len(r) == 9 and r[0].lstrip("-").isdigit():
                    yield r


def main():
    argv = sys.argv[1:]
    out_csv = pool_min = None
    if "--csv" in argv:
        i = argv.index("--csv"); out_csv = argv[i + 1]; del argv[i:i + 2]
    if "--pool" in argv:
        i = argv.index("--pool"); pool_min = float(argv[i + 1]); del argv[i:i + 2]
    replicates = "--replicates" in argv
    args = [a for a in argv if not a.startswith("--")]
    if not args:
        print(__doc__)
        return

    counts = defaultdict(int)                 # (c, scale, alpha, mode, cyc) -> count
    totals = defaultdict(int)                 # (c, scale, alpha, mode) -> samples
    seen = set()                              # (c, scale, alpha, mode, seed)
    exact = {}                                # (c, scale, alpha, mode, cyc) -> share
    cycles = defaultdict(set)
    per_seed = defaultdict(lambda: defaultdict(dict))   # (c, scale, alpha, mode) -> seed -> cyc -> count
    for c, sc, al, mode, seed, n, cyc, cnt, share in read_rows(args):
        c, cyc = int(c), int(cyc)
        key = (c, sc, al, mode)
        if mode.startswith("exact"):
            exact[key + (cyc,)] = float(share)
            cycles[c].add(cyc)
            continue
        if (key + (seed,)) not in seen:
            seen.add(key + (seed,))
            totals[key] += int(n)
        counts[key + (cyc,)] += int(cnt)
        per_seed[key][seed][cyc] = per_seed[key][seed].get(cyc, 0) + int(cnt)
        if cyc != -1:
            cycles[c].add(cyc)

    rows_out = []
    for c in sorted(cycles):
        cfg = sorted({(sc, al) for (cc, sc, al, m) in totals if cc == c},
                     key=lambda t: (float(t[0]), float(t[1])))
        print(f"\n=== map 3x{c:+d} ===")
        for sc, al in cfg:
            nl = totals.get((c, sc, al, "log"), 0)
            nu = totals.get((c, sc, al, "unif"), 0)
            print(f"\nwindow [10^{sc}, 10^({sc}*{al})]   samples: log {nl:,}   unif {nu:,}")
            print(f"{'cycle':>6} {'d_1e10':>8} | {'log share':>10} {'SE':>8} {'z(ref)':>7} | "
                  f"{'unif share':>10} {'SE':>8} {'z(u-l)':>7}")
            for cyc in sorted(cycles[c]):
                ref = REF.get(c, {}).get(cyc)
                pl = counts[(c, sc, al, "log", cyc)] / nl if nl else float("nan")
                pu = counts[(c, sc, al, "unif", cyc)] / nu if nu else float("nan")
                sl = math.sqrt(pl * (1 - pl) / nl) if nl else float("nan")
                su = math.sqrt(pu * (1 - pu) / nu) if nu else float("nan")
                rsd = 0.0 if (c, cyc) in EXACT else ROUND_SD
                zr = (pl - ref) / math.hypot(sl, rsd) if (ref is not None and nl and sl > 0) else float("nan")
                zu = (pu - pl) / math.hypot(sl, su) if (nl and nu and math.hypot(sl, su) > 0) else float("nan")
                refs = f"{ref:.4f}" if ref is not None else "   -   "
                print(f"{cyc:>6} {refs:>8} | {pl:>10.5f} {sl:>8.5f} {zr:>7.2f} | "
                      f"{pu:>10.5f} {su:>8.5f} {zu:>7.2f}")
                rows_out.append([c, sc, al, cyc, ref if ref is not None else "",
                                 nl, pl, sl, nu, pu, su])
            unl = counts[(c, sc, al, "log", -1)]
            unu = counts[(c, sc, al, "unif", -1)]
            if unl or unu:
                print(f"  unresolved orbits: log {unl}, unif {unu}")
        cyc_list = sorted(cycles[c]) + [-1]
        for mode in ("log", "unif"):
            tab = [[counts[(c, sc, al, mode, cy)] for cy in cyc_list] for sc, al in cfg]
            chi, dof, pv = homogeneity(tab)
            if dof:
                print(f"\nhomogeneity across scales ({mode}): chi2={chi:.1f}, dof={dof}, p={pv:.3f}")
        for sc, al in cfg:
            tab = [[counts[(c, sc, al, mode, cy)] for cy in cyc_list] for mode in ("log", "unif")]
            chi, dof, pv = homogeneity(tab)
            if dof:
                print(f"log vs unif at 10^{sc}: chi2={chi:.1f}, dof={dof}, p={pv:.3f}")
        ex = sorted({(sc, al) for (cc, sc, al, m, cy) in exact if cc == c})
        for sc, al in ex:
            print(f"\nexact window [10^{sc}, 10^({sc}*{al})]")
            for cyc in sorted(cycles[c]):
                el = exact.get((c, sc, al, "exact_log", cyc), float("nan"))
                eu = exact.get((c, sc, al, "exact_unif", cyc), float("nan"))
                print(f"{cyc:>6}  log {el:.6f}  unif {eu:.6f}")

    if pool_min is not None:
        print(f"\n=== shares pooled over the windows with log10y >= {pool_min:g} ===")
        for c in sorted(cycles):
            keys = [k for k in totals if k[0] == c and float(k[1]) >= pool_min]
            print(f"\nmap 3x{c:+d}")
            print(f"{'cycle':>6} | {'log share':>10} {'SE':>8} | {'unif share':>10} {'SE':>8}")
            for cyc in sorted(cycles[c]):
                cells = []
                for mode in ("log", "unif"):
                    n = sum(totals[k] for k in keys if k[3] == mode)
                    k_ = sum(counts[k + (cyc,)] for k in keys if k[3] == mode)
                    p = k_ / n if n else float("nan")
                    se = math.sqrt(p * (1 - p) / n) if n else float("nan")
                    cells.append(f"{p:>10.5f} {se:>8.6f}")
                print(f"{cyc:>6} | {cells[0]} | {cells[1]}")

    if replicates:
        print("\n=== replicate check: do the seeds of each configuration scatter binomially? ===")
        pv = []
        for key in sorted(per_seed, key=lambda k: (k[0], float(k[1]), k[3])):
            reps = {sd: v for sd, v in per_seed[key].items() if sd != "summary"}
            if len(reps) < 2:
                continue
            cyc = sorted({cy for d in reps.values() for cy in d})
            chi, dof, p = homogeneity([[reps[sd].get(cy, 0) for cy in cyc] for sd in sorted(reps)])
            pv.append(p)
            print(f"3x{key[0]:+d} 10^{key[1]:>3} {key[3]:4s}: {len(reps)} replicates, chi2={chi:6.1f}, dof={dof:3d}, p={p:.3f}")
        if pv:
            print(f"{len(pv)} configurations: {sum(p < 0.05 for p in pv)} with p<0.05, "
                  f"{sum(p < 0.01 for p in pv)} with p<0.01 (expected about {0.05 * len(pv):.1f} and {0.01 * len(pv):.1f})")
        else:
            print("no raw outputs with several seeds found")

    if out_csv:
        with open(out_csv, "w", newline="") as fh:
            w = csv.writer(fh)
            w.writerow(["c", "log10y", "alpha", "cycle_min", "d_1e10",
                        "n_log", "share_log", "se_log", "n_unif", "share_unif", "se_unif"])
            w.writerows(rows_out)
        print(f"\nsummary written to {out_csv}")


if __name__ == "__main__":
    main()
