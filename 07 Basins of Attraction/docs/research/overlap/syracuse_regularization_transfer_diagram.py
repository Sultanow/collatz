import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle, FancyArrowPatch

fig, ax = plt.subplots(figsize=(14, 8))
ax.set_xlim(0, 14)
ax.set_ylim(0, 10)
ax.axis('off')

ax.text(7, 9.6, "Syracuse regularity and transfer picture",
        ha='center', va='center', fontsize=18, fontweight='bold')
ax.text(
    7, 9.15,
    "Left: 3-adic self-similar branches overlap -> no naive L^2 regularity transfer.   "
    "Right: control dangerous 2-adic inputs E_B via the potential h(z).",
    ha='center', va='center', fontsize=10
)

left_x, left_y, left_w, left_h = 0.6, 1.0, 5.8, 7.6
ax.add_patch(Rectangle((left_x, left_y), left_w, left_h, fill=False, linewidth=1.5))
ax.text(left_x + left_w/2, left_y + left_h - 0.35,
        "3-adic reference distribution", ha='center', va='center',
        fontsize=14, fontweight='bold')

ax.add_patch(Rectangle((1.8, 7.1), 3.4, 0.8, fill=False, linewidth=1.2))
ax.text(3.5, 7.5, "V = sum_{j>=1} 3^(j-1) 2^(-(A1+...+Aj))",
        ha='center', va='center', fontsize=12)

coords_lvl1 = [(1.2, 5.7), (3.0, 5.7), (4.8, 5.7)]
labels_lvl1 = [
    "F1(x) = (1+3x)/2",
    "F2(x) = (1+3x)/4",
    "F3(x) = (1+3x)/8"
]
for (x, y), lab in zip(coords_lvl1, labels_lvl1):
    ax.add_patch(Rectangle((x, y), 1.2, 0.7, fill=False, linewidth=1.0))
    ax.text(x + 0.6, y + 0.35, lab, ha='center', va='center', fontsize=10)
    ax.add_patch(FancyArrowPatch((3.5, 7.1), (x + 0.6, y + 0.7),
                                 arrowstyle='->', mutation_scale=12, linewidth=1.0))
    ax.add_patch(FancyArrowPatch((x + 0.6, y), (3.5, 4.7),
                                 arrowstyle='->', mutation_scale=12, linewidth=1.0))

ax.add_patch(Rectangle((2.0, 3.8), 3.0, 0.9, fill=False, linewidth=1.2))
ax.text(3.5, 4.25, "overlap of branch images\non positive-mass 3-adic sets",
        ha='center', va='center', fontsize=11)

left_formula = (
    "Integral f_n^p dm_3  >=  (3^(p-1)/(2^p-1)) Integral f_(n-1)^p dm_3\n"
    "p = 2: critical factor = 1  +  overlaps  =>  ||f_n||_2 -> infinity"
)
ax.text(3.5, 2.7, left_formula, ha='center', va='center', fontsize=10.2,
        bbox=dict(boxstyle='round,pad=0.35', fill=False))

ax.text(3.5, 1.55,
        "Takeaway:\n3-adic fine-scale mixing alone does not give the\n"
        "L^2 regularity needed for the 2-adic entrance problem.",
        ha='center', va='center', fontsize=10)

ax.add_patch(FancyArrowPatch((6.55, 5.0), (7.35, 5.0),
                             arrowstyle='simple', mutation_scale=18, linewidth=1.0))
ax.text(6.95, 5.35, "therefore", ha='center', va='bottom', fontsize=11, fontweight='bold')

right_x, right_y, right_w, right_h = 7.6, 1.0, 5.8, 7.6
ax.add_patch(Rectangle((right_x, right_y), right_w, right_h, fill=False, linewidth=1.5))
ax.text(right_x + right_w/2, right_y + right_h - 0.35,
        "2-adic dangerous-input transfer",
        ha='center', va='center', fontsize=14, fontweight='bold')

ax.add_patch(Rectangle((8.2, 7.0), 4.6, 0.85, fill=False, linewidth=1.2))
ax.text(10.5, 7.42, "odd states z in (B, 2B]", ha='center', va='center', fontsize=12)
ax.text(10.5, 7.08, "weighted by actual entrance mass mu_B(z)", ha='center', va='center', fontsize=10)

ax.add_patch(Rectangle((8.8, 5.25), 3.4, 0.95, fill=False, linewidth=1.2))
ax.text(10.5, 5.73, "local first-passage map  z -> Pass_B(z)",
        ha='center', va='center', fontsize=11)
ax.text(10.5, 5.38, "P_B mu(y) = sum_{Pass_B(z)=y} mu(z)",
        ha='center', va='center', fontsize=10)

ax.add_patch(FancyArrowPatch((10.5, 7.0), (10.5, 6.2),
                             arrowstyle='->', mutation_scale=12, linewidth=1.0))
ax.add_patch(FancyArrowPatch((10.5, 5.25), (10.5, 4.45),
                             arrowstyle='->', mutation_scale=12, linewidth=1.0))

ax.add_patch(Rectangle((8.2, 3.55), 4.6, 0.85, fill=False, linewidth=1.2))
ax.text(10.5, 3.98, "first odd entrance values y <= B", ha='center', va='center', fontsize=12)

ax.add_patch(Rectangle((9.3, 2.3), 2.4, 0.75, fill=False, linewidth=1.2))
ax.text(10.5, 2.68, "dangerous inputs E_B", ha='center', va='center', fontsize=12)
ax.text(10.5, 2.38, "Pass_B(z) in D_X", ha='center', va='center', fontsize=10)

ax.add_patch(FancyArrowPatch((10.5, 3.55), (10.5, 3.05),
                             arrowstyle='->', mutation_scale=12, linewidth=1.0))

potential_text = (
    "mu_B(E_B) <= C_p H_p(B) X^(-beta(1-1/p))\n"
    "H_p(B) = ((1/R) sum_{B<z<=2B} (R h(z))^p)^(1/p)\n"
    "If H_(3/2)(B) = O(1), then  mu_B(E_B) = O(X^(-1/600))."
)
ax.text(10.5, 1.55, potential_text, ha='center', va='center', fontsize=10.2,
        bbox=dict(boxstyle='round,pad=0.35', fill=False))

ax.text(10.5, 6.55,
        "Need regularity of actual incoming weights,\nnot uniform smallness for each single residue class.",
        ha='center', va='center', fontsize=10)

ax.text(7, 0.35,
        "Research message: replace naive 3-adic L^2 regularity by a targeted estimate on the mass of "
        "dangerous 2-adic entrance states.",
        ha='center', va='center', fontsize=10)

import sys
out = sys.argv[1] if len(sys.argv) > 1 else "."
fig.savefig(f"{out}/syracuse_regularization_transfer_diagram.png", dpi=200, bbox_inches='tight')
fig.savefig(f"{out}/syracuse_regularization_transfer_diagram.pdf", bbox_inches='tight')
