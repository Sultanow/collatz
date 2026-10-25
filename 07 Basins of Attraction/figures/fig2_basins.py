#!/usr/bin/env python3
"""Figure 2: how the forest splits. Basin shares of 1 <= n <= N for 3x-1 and 3x+c,
0 < c < 50, gcd(c,6) = 1 (left), and the shares of 3x-1 per dyadic window (right).
Basins of cycles coprime to c are coloured; basins of cycles inherited from 3x+c/e
(multiples of a divisor e > 1 of c) are grey.

Usage:  python3 fig2_basins.py ../results ../docs/fig2_basins.pdf [N_tag]
        (reads basin_q3_c{c}_{N_tag}.txt, default N_tag = N1e10)
"""
import math
import re
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Patch

src = sys.argv[1] if len(sys.argv) > 1 else "../results"
dst = sys.argv[2] if len(sys.argv) > 2 else "../docs/fig2_basins.pdf"
tag = sys.argv[3] if len(sys.argv) > 3 else "N1e10"
maps = [-1, 1, 5, 7, 11, 13, 17, 19, 23, 25, 29, 31, 35, 37, 41, 43, 47, 49]


def read(c):
    txt = open(f"{src}/basin_q3_c{c}_{tag}.txt").read()
    shares = [(int(m), float(s)) for m, s in re.findall(r"m=(\d+) count=\d+ share=([\d.]+)", txt)]
    win = {}
    for line in txt.splitlines():
        mm = re.match(r"(\d+),([\d.,]+)$", line)
        if mm:
            win[int(mm.group(1))] = [float(v) for v in mm.group(2).split(",")]
    return shares, win


fig, (ax, bx) = plt.subplots(1, 2, figsize=(7.4, 4.6), gridspec_kw={"width_ratios": [2.4, 1]})
cmap = plt.get_cmap("tab10")
for row, c in enumerate(maps):
    shares, _ = read(c)
    own = sorted([s for s in shares if math.gcd(s[0], abs(c)) == 1], key=lambda s: -s[1])
    inh = sorted([s for s in shares if math.gcd(s[0], abs(c)) > 1], key=lambda s: -s[1])
    left = 0.0
    for i, (m, s) in enumerate(own):
        ax.barh(row, s, left=left, height=0.72, color=cmap(i % 10), edgecolor="white", linewidth=0.5)
        if s > 0.07:
            ax.text(left + s / 2, row, str(m), ha="center", va="center", fontsize=6.5, color="white")
        left += s
    for m, s in inh:
        ax.barh(row, s, left=left, height=0.72, color="#d0d0d0", edgecolor="white", linewidth=0.5)
        left += s
ax.set_yticks(range(len(maps)))
ax.set_yticklabels([f"$3x{c:+d}$" for c in maps], fontsize=7.5)
ax.invert_yaxis()
ax.set_xlim(0, 1)
ax.set_xlabel(r"share of $1\leq n\leq 10^{10}$", fontsize=8.5)
ax.tick_params(axis="x", labelsize=7.5)
ax.spines[["top", "right"]].set_visible(False)
handles = [Patch(color=cmap(0), label="cycles coprime to $c$ (labels: minima)"),
           Patch(color="#d0d0d0", label=r"cycles of multiples of $e\,|\,c$, $e>1$")]
ax.legend(handles=handles, fontsize=7, frameon=False, loc="upper center", bbox_to_anchor=(0.5, 1.09), ncol=2)

_, win = read(-1)
js = [j for j in sorted(win) if 10 <= j <= 32]
for i, (m, col) in enumerate(zip([1, 5, 17], ["#1f77b4", "#ff7f0e", "#2ca02c"])):
    bx.plot(js, [win[j][i] for j in js], "-", color=col, linewidth=1.2, label=f"cycle through {m}")
bx.set_title(r"$3x-1$: share in $[2^j,2^{j+1})$", fontsize=8)
bx.set_xlabel(r"$j$", fontsize=8.5)
bx.tick_params(labelsize=7.5)
bx.set_ylim(0.30, 0.37)
bx.legend(fontsize=7, frameon=False, loc="upper right")
bx.spines[["top", "right"]].set_visible(False)
fig.tight_layout()
fig.savefig(dst)
fig.savefig(dst.replace(".pdf", ".png"), dpi=160)
print("written", dst)
