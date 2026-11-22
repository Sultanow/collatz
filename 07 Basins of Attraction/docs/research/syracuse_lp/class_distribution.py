#!/usr/bin/env python3
"""Class distribution modulo 3 of the renormalisation walk started at g_d (roadmap: regular part).

For independent geometric words A, B of length k with S(B) = g_d(S(A)) (mod 3^k), the depth-k state is
h(y) = 2^e y + t with e = |B| + d - |A| and t = 2^{|B|} (g_d(S(A)) - S(B)) / 3^k.  Its class modulo 3 is
c = (2^e mod 3, t mod 3), one of six classes, and the one-step weights W(c) of the recursion are
    W(1,0) = 5/3, W(2,0) = 4/3, W(1,1) = W(1,2) = 2/3, W(2,1) = 4/3, W(2,2) = 1/3   (mean 1).
With m_k(c) the weight of the depth-k states in class c (sum_c m_k(c) = Cor_k(g_d)), exactly
    Cor_{k+1}(g_d) = sum_c W(c) m_k(c),   so   Cor_{k+1}/Cor_k - 1 = sum_c W(c) (nu_k(c) - 1/6),
with nu_k(c) = m_k(c)/Cor_k(g_d).  Cor_N(g_d) is bounded if the class distribution nu_k converges to the uniform
distribution summably fast.  This script computes nu_k for k <= KMAX (laws of S(A) mod 3^{k+1} with the parity of
|A| via the length phases theta = 0 and 1/2 of congruence_counts.py).

usage: class_distribution.py KMAX OUTDIR [--d 2,4]
"""
import argparse
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from congruence_counts import Chain, gd_index  # noqa: E402

CLASSES = [(1, 0), (2, 0), (1, 1), (1, 2), (2, 1), (2, 2)]
W = {(1, 0): 5 / 3, (2, 0): 4 / 3, (1, 1): 2 / 3, (1, 2): 2 / 3, (2, 1): 4 / 3, (2, 2): 1 / 3}


def class_weights(ch, d, k):
    N = k + 1
    M = 3 ** N
    F0 = ch.law(N, k, 0.0).real
    F1 = ch.law(N, k, 0.5).real                  # e(theta(|A|-k)) = (-1)^{|A|-k}
    P = {0: (F0 + F1) / 2, 1: (F0 - F1) / 2}      # parity of |A| - k
    g = gd_index(d, N)
    m = {c: 0.0 for c in CLASSES}
    for pA in (0, 1):
        for pB in (0, 1):
            u = 1 if (pB - pA) % 2 == 0 else 2
            sign = 1 if (pB + k) % 2 == 0 else -1  # (-1)^{|B|}, 2^{|B|} = (-1)^{|B|} mod 3
            for delta in range(3):
                y = (g - (3 ** k) * delta) % M
                val = 3 ** k * float(np.dot(P[pA], P[pB][y]))
                m[(u, (sign * delta) % 3)] += val
    return m


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("kmax", type=int)
    ap.add_argument("outdir")
    ap.add_argument("--d", default="2,4")
    a = ap.parse_args()
    os.makedirs(a.outdir, exist_ok=True)
    ch = Chain(a.kmax + 1)
    rep = open(os.path.join(a.outdir, "class_distribution.txt"), "w")

    def P(s=""):
        print(s)
        rep.write(s + "\n")

    with open(os.path.join(a.outdir, "class_distribution.csv"), "w") as fo:
        fo.write("d,k,cor_k," + ",".join(f"nu_{u}{t}" for u, t in CLASSES) + ",drift\n")
        for d in (int(x) for x in a.d.split(",")):
            P(f"d = {d}: class distribution nu_k(c) of the depth-k states; drift = Cor_(k+1)/Cor_k - 1")
            P("  k   Cor_k(g_d)   " + "  ".join(f"({u},{t})" for u, t in CLASSES) + "   drift      check")
            prev = None
            rows = []
            for k in range(1, a.kmax + 1):
                m = class_weights(ch, d, k)
                cor = sum(m.values())
                nu = {c: m[c] / cor for c in CLASSES}
                nxt = sum(W[c] * m[c] for c in CLASSES)
                drift = nxt / cor - 1
                chk = "" if prev is None else f"{abs(prev - cor):.1e}"   # Cor_k from step k-1 vs direct
                prev = nxt
                rows.append((k, cor, nu, drift))
                P(f" {k:2d}  {cor:.8f}  " + " ".join(f"{nu[c]:.4f}" for c in CLASSES) + f"  {drift:+.2e}  {chk}")
                fo.write(f"{d},{k},{cor:.12f}," + ",".join(f"{nu[c]:.10f}" for c in CLASSES) + f",{drift:.6e}\n")
            P("  deviation nu_k(c) - 1/6 and its local ratio (k -> k+1):")
            for c in CLASSES:
                dev = [r[2][c] - 1 / 6 for r in rows]
                rat = [dev[i] / dev[i - 1] if abs(dev[i - 1]) > 1e-15 else float("nan") for i in range(1, len(dev))]
                P(f"  {c}: " + " ".join(f"{x:+.4f}" for x in dev[-6:]) + "   ratios " + " ".join(f"{x:.3f}" for x in rat[-5:]))
            P()
    rep.close()


if __name__ == "__main__":
    main()
