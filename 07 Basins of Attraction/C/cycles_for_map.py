#!/usr/bin/env python3
"""Cycles of one map T_{q,c} from the merged CSV of census_all.c.

All primitive cycles with c0 | c and the same sign, scaled by c/c0.
Prints the smallest members (input for basin.c); with --table the details.

Usage:  python3 cycles_for_map.py cycles.csv c [--table]
"""
import csv
import sys

path, c = sys.argv[1], int(sys.argv[2])
out = []
for r in csv.DictReader(open(path)):
    c0 = int(r["c"])
    if (c0 > 0) != (c > 0) or c % c0:
        continue
    m = c // c0
    out.append((int(r["min"]) * m, c0, int(r["l"]), int(r["k"]), int(r["D"]), r["word"]))
out.sort()
if "--table" in sys.argv:
    print("min,c0,l,k,D,word")
    for o in out:
        print(",".join(map(str, o)))
else:
    print(" ".join(str(o[0]) for o in out))
