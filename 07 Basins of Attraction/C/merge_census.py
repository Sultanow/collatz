#!/usr/bin/env python3
"""Merge census.c outputs (several runs or shards): keep the smallest word length per cycle."""
import re, sys

cycles, words = {}, 0
for path in sys.argv[1:]:
    for line in open(path):
        m = re.match(r"q=(-?\d+) c=(-?\d+) .*words=(\d+)", line)
        if m:
            words += int(m.group(3))
            continue
        m = re.match(r"min=(\d+) l=(\d+) k=(\d+) D=(-?\d+)(?: word=([01]+))?", line)
        if m:
            mn, l = int(m.group(1)), int(m.group(2))
            if mn not in cycles or l < cycles[mn][0]:
                cycles[mn] = (l, int(m.group(3)), int(m.group(4)), m.group(5) or "")
print(f"files={len(sys.argv) - 1} words={words} cycles={len(cycles)}")
for mn in sorted(cycles):
    l, k, D, w = cycles[mn]
    print(f"min={mn} l={l} k={k} D={D}" + (f" word={w}" if w else ""))
