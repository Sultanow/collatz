#!/usr/bin/env python3
"""Figure 1: primitive cycles of all maps 3x+c, |c0| <= 1000, per word length l
(census, exact) versus the heuristic sum_k L(l,k) * S(D) / |D|, where L(l,k) is the
number of aperiodic necklaces of type (l,k), D = 2^l - 3^k and S(D) = sum of phi(e)
over the divisors e <= DMAX of |D|.

Usage:  python3 fig1_cycles_vs_heuristic.py ../results/rational_cycles_q3_d1000_L40.csv ../docs/fig1_cycles_vs_heuristic.pdf
"""
import csv
import math
import sys
from collections import Counter

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from sympy import divisors, mobius, totient

DMAX = 1000
src = sys.argv[1] if len(sys.argv) > 1 else "../results/rational_cycles_q3_d1000_L40.csv"
dst = sys.argv[2] if len(sys.argv) > 2 else "../docs/fig1_cycles_vs_heuristic.pdf"
rows = list(csv.DictReader(open(src)))
LMAX = max(int(r["l"]) for r in rows)


def lyndon(l, k):
    g = math.gcd(l, k)
    return sum(int(mobius(d)) * math.comb(l // d, k // d) for d in divisors(g)) // l


obs = Counter(int(r["l"]) for r in rows)
exp = Counter()
for l in range(1, LMAX + 1):
    for k in range(1, l + 1):
        A = abs(2**l - 3**k)
        S = sum(int(totient(e)) for e in divisors(A) if e <= DMAX)
        exp[l] += lyndon(l, k) * S / A

ls = list(range(1, LMAX + 1))
fig, ax = plt.subplots(figsize=(7.2, 3.6))
ax.bar(ls, [obs[l] for l in ls], color="#9fb7d6", edgecolor="#4a6a94", linewidth=0.6,
       label="census (exact, all parity words)")
ax.plot(ls, [exp[l] for l in ls], "o-", color="#c8553d", markersize=3.5, linewidth=1.2,
        label=r"heuristic $\sum_k L(l,k)\,S(D)/|D|$")
ax.annotate(r"$l/k=19/12$: $D=-7153=-23\cdot 311$", xy=(19, obs[19]), xytext=(22.5, 124),
            fontsize=8, arrowprops=dict(arrowstyle="->", lw=0.7))
ax.annotate(r"$l/k=22/14$: $D=-5\cdot7\cdot11^2\cdot139$", xy=(22, obs[22]), xytext=(24.5, 104),
            fontsize=8, arrowprops=dict(arrowstyle="->", lw=0.7))
ax.text(0.015, 0.95, f"total: {sum(obs.values())} observed, {sum(exp.values()):.0f} expected",
        transform=ax.transAxes, fontsize=8, va="top")
ax.set_xlabel(r"word length $l$")
ax.set_ylabel(r"primitive cycles, $|c_0|\leq 1000$")
ax.set_xlim(0.3, LMAX + 0.7)
ax.legend(fontsize=8, frameon=False, loc="upper right", bbox_to_anchor=(1.0, 0.66))
ax.spines[["top", "right"]].set_visible(False)
fig.tight_layout()
fig.savefig(dst)
fig.savefig(dst.replace(".pdf", ".png"), dpi=160)
print(f"written {dst}: observed {sum(obs.values())}, expected {sum(exp.values()):.1f}")
