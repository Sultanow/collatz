#!/usr/bin/env python3
"""Merge census_all.c outputs (shards, length ranges) into one CSV of primitive cycles.

Usage:  python3 merge_census_all.py out/census_all_*.txt > cycles.csv
The summary (files, words, cycles, mismatches) goes to stderr; mismatches must be 0.
"""
import re
import sys

PAT = re.compile(r"c=(-?\d+) min=(\d+) l=(\d+) k=(\d+) D=(-?\d+) word=([01]+)")
rows, words, mismatches = set(), 0, 0
for path in sys.argv[1:]:
    for line in open(path):
        if line.startswith("#"):
            m = re.search(r"words=(\d+).*mismatches=(\d+)", line)
            if m:
                words += int(m.group(1))
                mismatches += int(m.group(2))
            continue
        m = PAT.match(line)
        if m:
            rows.add((int(m.group(1)), int(m.group(2)), int(m.group(3)),
                      int(m.group(4)), int(m.group(5)), m.group(6)))
print("c,min,l,k,D,word")
for r in sorted(rows, key=lambda r: (abs(r[0]), r[0] < 0, r[1])):
    print(",".join(map(str, r)))
print(f"files={len(sys.argv) - 1} words={words} cycles={len(rows)} mismatches={mismatches}",
      file=sys.stderr)
