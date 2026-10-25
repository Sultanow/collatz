#!/usr/bin/env python3
"""Cycles of one map T_{3,c} (input for basin.c).

All primitive cycles in the CSV of census_all.c with c0 | c and the same sign, scaled
by c/c0. This list is complete for the word lengths covered by the census. With
--search N0, the cycles reached by the orbits of the starting values 1 <= n < N0 are
added as well; this finds cycles with longer words (e.g. the cycle through 41 of
3x+23, word length 43, or the cycles through 3811 and 7055 of 3x+29, length 65).

Usage:  python3 cycles_for_map.py cycles.csv c [--search N0] [--table]
Prints the smallest members (space separated); with --table one line per cycle.
"""
import csv
import sys


def T(x, c):
    return x // 2 if x % 2 == 0 else (3 * x + c) // 2


def word(m, c):
    w, x = "", m
    while True:
        w += "1" if x % 2 else "0"
        x = T(x, c)
        if x == m:
            return w


def from_census(path, c):
    out = {}
    for r in csv.DictReader(open(path)):
        c0 = int(r["c"])
        if (c0 > 0) != (c > 0) or c % c0:
            continue
        out[int(r["min"]) * (c // c0)] = "census"
    return out


def from_search(c, n0):
    label, found = {}, {}
    for n in range(1, n0):
        if n in label:
            continue
        path, pos, x = [], {}, n
        while x not in label and x not in pos:
            if x <= 0:                      # orbit left the positive integers
                break
            pos[x] = len(path)
            path.append(x)
            x = T(x, c)
        if x <= 0:
            for y in path:
                label[y] = None
            continue
        if x in pos:
            m = min(path[pos[x]:])
            found[m] = "search"
            for y in path:
                label[y] = m
        else:
            for y in path:
                label[y] = label[x]
    return found


if __name__ == "__main__":
    path, c = sys.argv[1], int(sys.argv[2])
    cycles = from_census(path, c)
    if "--search" in sys.argv:
        n0 = int(sys.argv[sys.argv.index("--search") + 1])
        for m, src in from_search(c, n0).items():
            cycles.setdefault(m, src)
    if "--table" in sys.argv:
        print("min,source,l,k,word")
        for m in sorted(cycles):
            w = word(m, c)
            print(f"{m},{cycles[m]},{len(w)},{w.count('1')},{w}")
    else:
        print(" ".join(str(m) for m in sorted(cycles)))
