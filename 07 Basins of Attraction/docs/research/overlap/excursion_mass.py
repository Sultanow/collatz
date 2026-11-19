#!/usr/bin/env python3
"""Excursion starts u (odd, (2X1-c)/3 < u <= X1, T(u) > X1): share of those whose excursion above X1
climbs to s*u or higher, uniformly counted and weighted by the Run K potential h(u) = count/N.
usage: excursion_mass.py counts_c1.npz [...]"""
import sys
import numpy as np


def load(path):
    z = np.load(path)
    cnt = np.concatenate([z["head"].astype(np.int64), z["tail"].astype(np.int64)])
    return int(z["c"]), int(z["N"]), cnt


for path in sys.argv[1:]:
    c, N, cnt = load(path)
    for X1 in (1 << 20, 1 << 22):
        lo = (2 * X1 - c) // 3
        u = np.arange(lo + 1, X1 + 1, dtype=np.int64)
        u = u[(u & 1) == 1]
        u = u[(3 * u + c) // 2 > X1]
        h = cnt[u - 1] / N
        x = (3 * u + c) >> 1
        top = x.copy()
        length = np.ones(u.size, dtype=np.int64)
        act = np.arange(u.size)
        while act.size:
            y = x[act]
            odd = (y & 1).astype(bool)
            y2 = y >> 1
            y2[odd] = (3 * y[odd] + c) >> 1
            x[act] = y2
            top[act] = np.maximum(top[act], y2)
            up = y2 > X1
            length[act[up]] += 1
            act = act[up]
        g = top / u
        print(f"c={c:+d} X1=2^{int(np.log2(X1))}: {u.size} excursion starts, mean length {length.mean():.2f}, "
              f"P(length>40) uniform {np.mean(length > 40):.2e} weighted {h[length > 40].sum() / h.sum():.2e}")
        for s in (2, 4, 8, 16, 32, 64):
            G = g >= s
            fu, fh = G.mean(), h[G].sum() / h.sum()
            print(f"   climb >= {s:3d} u: uniform {fu:.5f}  h-weighted {fh:.5f}  ratio {fh / fu:.3f}  (Doob bound 1.5/s = {1.5 / s:.4f})")
