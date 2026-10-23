#!/usr/bin/env python3
"""Merge basin.c outputs of several ranges: add the counts and print the shares.

With window lines (basin -w) it also prints the shares per dyadic window
[2^j, 2^(j+1)) and the mean of the window shares over the complete windows with
j >= JMIN (an estimate of the logarithmic density; default JMIN = 10).

Usage:  python3 merge_basin.py out/basin_*.txt [--csv windows.csv] [--jmin JMIN]
"""
import re
import sys
from collections import defaultdict

argv = sys.argv[1:]
csv_out, jmin = None, 10
if "--csv" in argv:
    i = argv.index("--csv")
    csv_out = argv[i + 1]
    del argv[i:i + 2]
if "--jmin" in argv:
    i = argv.index("--jmin")
    jmin = int(argv[i + 1])
    del argv[i:i + 2]

order, counts = [], defaultdict(int)
total = unlabeled = escaped = 0
wn, wu, wc = defaultdict(int), defaultdict(int), {}
for path in argv:
    mins = []
    for line in open(path):
        m = re.match(r"start=(\d+) end=(\d+) cache=\d+ unlabeled=(\d+)(?: escaped=(\d+))?", line)
        if m:
            total += int(m.group(2)) - int(m.group(1))
            unlabeled += int(m.group(3))
            escaped += int(m.group(4) or 0)
            continue
        m = re.match(r"m=(\d+) count=(\d+)", line)
        if m:
            mn = int(m.group(1))
            mins.append(mn)
            if mn not in counts:
                order.append(mn)
            counts[mn] += int(m.group(2))
            continue
        m = re.match(r"w=(\d+) lo=(\d+) hi=(\d+) unlabeled=(\d+)(?: escaped=\d+)? counts=([\d,]*)", line)
        if m:
            j = int(m.group(1))
            wn[j] += int(m.group(3)) - int(m.group(2))
            wu[j] += int(m.group(4))
            acc = wc.setdefault(j, defaultdict(int))
            vals = [int(v) for v in m.group(5).split(",")] if m.group(5) else []
            for mn, v in zip(mins, vals):
                acc[mn] += v

print(f"files={len(argv)} numbers={total} unlabeled={unlabeled} escaped={escaped}")
for mn in order:
    print(f"m={mn} count={counts[mn]} share={counts[mn] / total:.6f}")
if wc:
    full = [j for j in sorted(wc) if wn[j] == (1 << j) and j >= jmin]
    print("window shares (j: window [2^j, 2^(j+1)); * = incomplete window):")
    print("j," + ",".join(f"m{mn}" for mn in order))
    for j in sorted(wc):
        mark = "" if wn[j] == (1 << j) else " *"
        print(f"{j}," + ",".join(f"{wc[j][mn] / wn[j]:.6f}" for mn in order) + mark)
    if full:
        est = ", ".join(f"m{mn}={sum(wc[j][mn] / wn[j] for j in full) / len(full):.6f}" for mn in order)
        print(f"mean window share over complete windows {full[0]}..{full[-1]}: {est}")
    if csv_out:
        with open(csv_out, "w") as f:
            f.write("j,numbers,unlabeled," + ",".join(f"m{mn}" for mn in order) + "\n")
            for j in sorted(wc):
                f.write(f"{j},{wn[j]},{wu[j]}," + ",".join(str(wc[j][mn]) for mn in order) + "\n")
