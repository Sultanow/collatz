#!/usr/bin/env python3
"""Merge basin.c outputs of several ranges: add the counts and print the shares."""
import re, sys
from collections import defaultdict

counts, total, unlabeled, order = defaultdict(int), 0, 0, []
for path in sys.argv[1:]:
    for line in open(path):
        m = re.match(r"start=(\d+) end=(\d+) cache=\d+ unlabeled=(\d+)", line)
        if m:
            total += int(m.group(2)) - int(m.group(1))
            unlabeled += int(m.group(3))
            continue
        m = re.match(r"m=(\d+) count=(\d+)", line)
        if m:
            mn = int(m.group(1))
            if mn not in counts:
                order.append(mn)
            counts[mn] += int(m.group(2))
print(f"files={len(sys.argv) - 1} numbers={total} unlabeled={unlabeled}")
for mn in order:
    print(f"m={mn} count={counts[mn]} share={counts[mn] / total:.6f}")
