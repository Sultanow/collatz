#!/usr/bin/env python3
"""Figure 1: primitive cycles of all maps 3x+c, |c0| <= 1000, per word length l
(census, exact) versus the heuristic sum_k L(l,k) * S(D) / |D|, where L(l,k) is the
number of aperiodic necklaces of type (l,k), D = 2^l - 3^k and S(D) = sum of phi(e)
over the divisors e <= DMAX of |D|; dashed: the same with the birthday factor
exp(-k^2 |2^(l/k) - 3| / (ALPHA e)) for every divisor e (types with |D| > DMAX; ALPHA fitted on
l <= 40 by C/heuristic_P.py).

Usage:  python3 fig1_cycles_vs_heuristic.py ../results/run_P/rational_cycles_q3_d1000_L66.csv ../docs/fig1_cycles_vs_heuristic.pdf [ALPHA]
        (Run A alone: ../results/rational_cycles_q3_d1000_L40.csv)
"""
import csv
import math
import sys
from collections import Counter

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
DMAX = 1000
src = sys.argv[1] if len(sys.argv) > 1 else "../results/run_P/rational_cycles_q3_d1000_L66.csv"
dst = sys.argv[2] if len(sys.argv) > 2 else "../docs/fig1_cycles_vs_heuristic.pdf"
ALPHA = float(sys.argv[3]) if len(sys.argv) > 3 else 6.0
rows = list(csv.DictReader(open(src)))
LMAX = max(int(r["l"]) for r in rows)


def phi(n):
    r, m, p = n, n, 2
    while p * p <= m:
        if m % p == 0:
            while m % p == 0:
                m //= p
            r -= r // p
        p += 1
    return r - r // m if m > 1 else r


def mobius(n):
    r, m, p = 1, n, 2
    while p * p <= m:
        if m % p == 0:
            m //= p
            if m % p == 0:
                return 0
            r = -r
        p += 1
    return -r if m > 1 else r


def lyndon(l, k):
    g = math.gcd(l, k)
    return sum(mobius(d) * math.comb(l // d, k // d) for d in range(1, g + 1) if g % d == 0) // l


PHI = {e: phi(e) for e in range(1, DMAX + 1)}
obs = Counter(int(r["l"]) for r in rows)
exp, bday = Counter(), Counter()
for l in range(1, LMAX + 1):
    for k in range(1, l + 1):
        A = abs(2**l - 3**k)
        divs = [e for e in range(1, DMAX + 1) if A % e == 0]      # only divisors <= DMAX count
        L = lyndon(l, k)
        exp[l] += L * sum(PHI[e] for e in divs) / A
        g = abs(2 ** (l / k) - 3)
        bday[l] += L * sum(PHI[e] * (math.exp(-k * k * g / (ALPHA * e)) if A > DMAX else 1.0) for e in divs) / A

ls = list(range(1, LMAX + 1))
fig, ax = plt.subplots(figsize=(7.2, 3.6))
ax.bar(ls, [obs[l] for l in ls], color="#9fb7d6", edgecolor="#4a6a94", linewidth=0.6,
       label="census (exact, all parity words)")
ax.plot(ls, [exp[l] for l in ls], "o-", color="#c8553d", markersize=3.0, linewidth=1.1,
        label=r"heuristic $\sum_k L(l,k)\,S(D)/|D|$")
ax.plot(ls, [bday[l] for l in ls], "--", color="#2f6f4f", linewidth=1.1,
        label=rf"with birthday factor ($\alpha={ALPHA:g}$, fitted on $l\leq40$)")
if LMAX > 40:
    ax.axvline(40.5, color="0.5", linestyle=":", linewidth=0.8)
    ax.text(40.0, 0.42, "Run A", transform=ax.get_xaxis_transform(), fontsize=7, ha="right", va="top", color="0.35")
    ax.text(41.0, 0.42, "Run P", transform=ax.get_xaxis_transform(), fontsize=7, ha="left", va="top", color="0.35")
ax.annotate(r"$(19,12)$: $D=-23\cdot 311$", xy=(19, obs[19]), xytext=(4.0, 118),
            fontsize=8, arrowprops=dict(arrowstyle="->", lw=0.7))
ax.annotate(r"$(22,14)$: $D=-5\cdot7\cdot11^2\cdot139$", xy=(22, obs[22]), xytext=(24.5, 108),
            fontsize=8, arrowprops=dict(arrowstyle="->", lw=0.7))
ax.text(0.985, 0.97, f"total: {sum(obs.values())} observed, {sum(exp.values()):.0f} expected, "
        f"{sum(bday.values()):.0f} with birthday factor", transform=ax.transAxes, fontsize=8, va="top", ha="right")
ax.set_xlabel(r"word length $l$")
ax.set_ylabel(r"primitive cycles, $|c_0|\leq 1000$")
ax.set_xlim(0.3, LMAX + 0.7)
ax.legend(fontsize=8, frameon=False, loc="upper right", bbox_to_anchor=(1.0, 0.74))
ax.spines[["top", "right"]].set_visible(False)
fig.tight_layout()
fig.savefig(dst)
fig.savefig(dst.replace(".pdf", ".png"), dpi=160)
print(f"written {dst}: observed {sum(obs.values())}, expected {sum(exp.values()):.1f}, birthday {sum(bday.values()):.1f}")
for lo, hi in ((1, 40), (41, LMAX)):
    print(f"  l = {lo}..{hi}: observed {sum(obs[l] for l in range(lo, hi + 1))}, "
          f"expected {sum(exp[l] for l in range(lo, hi + 1)):.1f}, birthday {sum(bday[l] for l in range(lo, hi + 1)):.1f}")
