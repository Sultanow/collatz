#!/usr/bin/env python3
"""Figure 4: predicted against measured basin shares. The prediction is the harmonic mass of
the backward tree of each primitive cycle (backward_mass), normalised over the primitive
cycles of its map; the measurement is the relative share from Run D. Colour: c.

Usage:  python3 fig4_backward_mass.py ../results/landscape/predictions_D70.csv ../docs/fig4_backward_mass.pdf
        (the input is written by predictor_eval.py --csv)
"""
import csv
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

src = sys.argv[1] if len(sys.argv) > 1 else "../results/landscape/predictions_D70.csv"
dst = sys.argv[2] if len(sys.argv) > 2 else "../docs/fig4_backward_mass.pdf"
rows = [r for r in csv.DictReader(open(src)) if float(r["predicted"]) > 0 and float(r["rel_share"]) > 0]
x = [float(r["predicted"]) for r in rows]
y = [float(r["rel_share"]) for r in rows]
c = [int(r["c"]) for r in rows]

fig, ax = plt.subplots(figsize=(4.6, 3.9))
sc = ax.scatter(x, y, c=c, s=7, cmap="viridis", vmin=1, vmax=400)
lo = min(min(x), min(y)) / 1.5
ax.plot([lo, 1.2], [lo, 1.2], color="grey", lw=0.8, ls="--")
ax.set_xscale("log")
ax.set_yscale("log")
ax.set_xlim(lo, 1.2)
ax.set_ylim(lo, 1.2)
ax.set_xlabel("predicted share (backward mass)")
ax.set_ylabel("measured share (Run D)")
fig.colorbar(sc, ax=ax, label="c")
fig.tight_layout()
fig.savefig(dst)
print(f"figure written to {dst}")
