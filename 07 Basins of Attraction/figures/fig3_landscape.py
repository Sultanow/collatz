#!/usr/bin/env python3
"""Figure 3: the basin landscape of the maps 3x+c, 1 <= c <= 400, gcd(c,6) = 1 (Run D):
for every map, the relative shares of its primitive cycles (cycles coprime to c), largest
first (darkest at the bottom). The script also prints Spearman
rank correlations between the relative share and simple features of the cycles,
pooled and within maps (the within-map version removes the effect of the number of cycles).

Usage:  python3 fig3_landscape.py ../results/landscape/landscape_shares.csv ../docs/fig3_landscape.pdf
"""
import csv
import math
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

src = sys.argv[1] if len(sys.argv) > 1 else "../results/landscape/landscape_shares.csv"
dst = sys.argv[2] if len(sys.argv) > 2 else "../docs/fig3_landscape.pdf"

rows = [r for r in csv.DictReader(open(src)) if int(r["m"]) == 1]
maps = sorted({int(r["c"]) for r in rows})
by_map = {c: sorted((float(r["rel_share"]) for r in rows if int(r["c"]) == c), reverse=True) for c in maps}


def ranks(v):
    order = sorted(range(len(v)), key=lambda i: v[i])
    r = [0.0] * len(v)
    i = 0
    while i < len(v):
        j = i
        while j + 1 < len(v) and v[order[j + 1]] == v[order[i]]:
            j += 1
        for t in range(i, j + 1):
            r[order[t]] = (i + j) / 2
        i = j + 1
    return r


def spearman(x, y):
    rx, ry = ranks(x), ranks(y)
    mx, my = sum(rx) / len(rx), sum(ry) / len(ry)
    sxy = sum((a - mx) * (b - my) for a, b in zip(rx, ry))
    sxx = sum((a - mx) ** 2 for a in rx)
    syy = sum((b - my) ** 2 for b in ry)
    return sxy / math.sqrt(sxx * syy)


rel = [float(r["rel_share"]) for r in rows]
feats = {
    "log(min/c)": [math.log(int(r["cycle_min"]) / int(r["c"])) for r in rows],
    "word length l": [int(r["l"]) for r in rows],
    "k/l": [int(r["k"]) / int(r["l"]) for r in rows],
    "cycles of the map": [len(by_map[int(r["c"])]) for r in rows],
}
print(f"{len(rows)} primitive cycles of {len(maps)} maps; Spearman correlation with the relative share, pooled:")
for name, v in feats.items():
    print(f"  {name:>18}: {spearman(v, rel):+.3f}")

# Pooled correlations mix maps with few and many cycles (the shares of a map add up to 1).
# Within-map correlations remove this confounder; a sign test counts the maps.
print("within-map Spearman correlation with the relative share (maps with at least two primitive cycles):")
groups = {}
for r in rows:
    groups.setdefault(int(r["c"]), []).append(r)
for name, f in (("cycle minimum", lambda r: int(r["cycle_min"])),
                ("word length l", lambda r: int(r["l"])),
                ("k/l", lambda r: int(r["k"]) / int(r["l"]))):
    vals = []
    for g in groups.values():
        x = [f(r) for r in g]
        if len(g) >= 2 and len(set(x)) > 1:
            vals.append(spearman(x, [float(r["rel_share"]) for r in g]))
    pos, neg = sum(v > 0 for v in vals), sum(v < 0 for v in vals)
    n = pos + neg
    tail = sum(math.comb(n, i) for i in range(0, min(pos, neg) + 1)) / 2 ** n
    print(f"  {name:>18}: mean {sum(vals) / len(vals):+.3f}; positive in {pos}, negative in {neg} of "
          f"{len(vals)} maps; sign test p = {min(1.0, 2 * tail):.4f}")
multi = [g for g in groups.values() if len(g) >= 2]
top = sum(max(g, key=lambda r: float(r["rel_share"]))["cycle_min"] == min(g, key=lambda r: int(r["cycle_min"]))["cycle_min"]
          for g in multi)
print(f"  the cycle with the smallest minimum has the largest basin in {top} of {len(multi)} maps")

fig, ax = plt.subplots(figsize=(7.4, 3.2))
cmap = plt.get_cmap("viridis")
for i, c in enumerate(maps):
    bottom = 0.0
    for j, s_ in enumerate(by_map[c]):
        ax.bar(i, s_, bottom=bottom, width=1.0, color=cmap(min(j, 7) / 7), linewidth=0)
        bottom += s_
step = max(1, len(maps) // 14)
ax.set_xticks(range(0, len(maps), step))
ax.set_xticklabels([str(maps[i]) for i in range(0, len(maps), step)], fontsize=7)
ax.set_xlim(-0.5, len(maps) - 0.5)
ax.set_ylim(0, 1)
ax.set_xlabel("c (maps 3x+c with gcd(c,6) = 1)")
ax.set_ylabel("share among integers prime to c")
fig.tight_layout()
fig.savefig(dst)
print(f"figure written to {dst}")
