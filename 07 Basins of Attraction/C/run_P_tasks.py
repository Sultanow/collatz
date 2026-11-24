#!/usr/bin/env python3
"""Run P: task list for mitm_census, one task per type (l, k), the most expensive first.

The cost of a type is the number of half-word residues the meet-in-the-middle search generates,
sum_{k1} C(floor(l/2), k1) + C(ceil(l/2), k - k1), for one group of maximal divisors (most types
have one). Printing the most expensive tasks first keeps the parallel run balanced.

usage:  run_P_tasks.py LMIN LMAX > tasks.txt        (lines "l k cost")
"""
import sys
from math import comb


def cost(l, k):
    h1, h2 = l // 2, l - l // 2
    return sum(comb(h1, k1) + comb(h2, k - k1) for k1 in range(max(0, k - h2), min(k, h1) + 1))


def main():
    lmin, lmax = int(sys.argv[1]), int(sys.argv[2])
    tasks = [(cost(l, k), l, k) for l in range(lmin, lmax + 1) for k in range(1, l + 1)]
    tasks.sort(reverse=True)
    for c, l, k in tasks:
        print(l, k, c)
    tot = sum(t[0] for t in tasks)
    print(f"{len(tasks)} tasks, {tot:.3e} residues in total, largest {tasks[0][0]:.3e} (l={tasks[0][1]}, k={tasks[0][2]})",
          file=sys.stderr)


if __name__ == "__main__":
    main()
