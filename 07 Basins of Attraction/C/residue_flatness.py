#!/usr/bin/env python3
"""Basin shares conditional on residue classes (Run E).

Usage:  python3 residue_flatness.py out_residue/passage_*.csv [--csv classes.csv]

Reads the output of passage.c in residue-sweep mode (lines with the extra fields m,r).
For every map 3x+c and modulus m it compares the basin shares of the residue classes
r mod m with each other: a chi-square test of homogeneity across the classes, and the
largest deviation of a class share from the pooled share, also in standard errors.
Large p-values mean that the residue class carries no information about the basin
beyond the global shares. The moduli 5 (for 3x+5) and 13 (for 3x+13) are positive
controls: the multiples of 5 and 13 form a basin of their own (Proposition 5).
"""
import csv
import glob
import importlib.util
import math
import os
import sys
from collections import defaultdict

here = os.path.dirname(os.path.abspath(__file__))
spec = importlib.util.spec_from_file_location("merge_passage", os.path.join(here, "merge_passage.py"))
mp = importlib.util.module_from_spec(spec)
spec.loader.exec_module(mp)


def label(m):
    for p in (2, 3):
        k, x = 0, m
        while x % p == 0:
            x //= p
            k += 1
        if x == 1 and k:
            return f"{p}^{k}"
    return str(m)


def main():
    argv = sys.argv[1:]
    out_csv = None
    if "--csv" in argv:
        i = argv.index("--csv"); out_csv = argv[i + 1]; del argv[i:i + 2]
    files = []
    for a in argv:
        files += sorted(glob.glob(os.path.join(a, "*.csv"))) if os.path.isdir(a) else sorted(glob.glob(a))
    if not files:
        print(__doc__)
        return
    counts = defaultdict(lambda: defaultdict(dict))   # (c, m, mode) -> r -> cyc -> count
    nper = defaultdict(dict)                           # (c, m, mode) -> r -> samples
    for f in files:
        with open(f) as fh:
            for row in csv.reader(fh):
                if len(row) != 11:
                    continue
                c, mode, n, cyc, cnt, m, r = int(row[0]), row[3], int(row[5]), int(row[6]), int(row[7]), int(row[9]), int(row[10])
                key = (c, m, mode)
                counts[key][r][cyc] = counts[key][r].get(cyc, 0) + cnt
                if cyc == -1:
                    nper[key][r] = nper[key].get(r, 0) + n
    rows_out = []
    print(f"{'map':>6} {'m':>7} {'mode':>4} {'classes':>7} {'n/class':>9} {'chi2':>10} {'dof':>5} {'p':>7} "
          f"{'max dev':>8} {'max |z|':>7}  unresolved")
    for key in sorted(counts, key=lambda k: (k[0], k[1] % 3 == 0, k[1], k[2])):
        c, m, mode = key
        classes = sorted(counts[key])
        cycles = sorted({cy for r in classes for cy in counts[key][r]})
        table = [[counts[key][r].get(cy, 0) for cy in cycles] for r in classes]
        chi, dof, p = mp.homogeneity(table)
        ntot = sum(nper[key].values())
        pooled = {cy: sum(counts[key][r].get(cy, 0) for r in classes) / ntot for cy in cycles if cy != -1}
        maxdev = maxz = 0.0
        for r in classes:
            n = nper[key][r]
            for cy, p0 in pooled.items():
                s = counts[key][r].get(cy, 0) / n
                se = math.sqrt(max(p0 * (1 - p0), 1e-300) / n)
                maxdev = max(maxdev, abs(s - p0))
                maxz = max(maxz, abs(s - p0) / se)
                rows_out.append([c, m, mode, r, cy, n, counts[key][r].get(cy, 0), s, p0])
        unres = sum(counts[key][r].get(-1, 0) for r in classes)
        nclass = ntot // len(classes)
        print(f"3x{c:+d}".rjust(6) + f" {label(m):>7} {mode:>4} {len(classes):>7} {nclass:>9,} {chi:>10.1f} {dof:>5} "
              f"{p:>7.3f} {maxdev:>8.5f} {maxz:>7.2f}  {unres}")
    if out_csv:
        with open(out_csv, "w", newline="") as fh:
            w = csv.writer(fh)
            w.writerow(["c", "m", "mode", "r", "cycle_min", "n", "count", "share", "pooled_share"])
            w.writerows(rows_out)
        print(f"\nper-class shares written to {out_csv}")


if __name__ == "__main__":
    main()
