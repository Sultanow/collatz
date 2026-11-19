#!/usr/bin/env python3
"""L^p shell means of v(a) = a h(a) from the Run K counts, and the weighted mass of the
inputs z in (B, 2B] whose first passage below B lands in a dangerous class.
usage: lp_shell_means.py counts_c1.npz [...]"""
import sys
import numpy as np

PS = (1.25, 1.5, 1.75, 2.0)


def load(path):
    z = np.load(path)
    cnt = np.concatenate([z["head"].astype(np.int64), z["tail"].astype(np.int64)])
    return int(z["c"]), int(z["N"]), cnt          # cnt[a-1] = number of starts n <= N visiting a


def pass_below(z, B, c):
    """first T_{3,c} value <= B for every entry of z (> B)."""
    x = z.copy()
    act = np.flatnonzero(x > B)
    while act.size:
        y = x[act]
        odd = (y & 1).astype(bool)
        y2 = y >> 1
        y2[odd] = (3 * y[odd] + c) >> 1
        x[act] = y2
        act = act[y2 > B]
    return x


def danger_classes(L, s, c):
    """classes r mod 2^L whose first L parities give max_t 3^q/2^t >= 2^s (Doob: density <= 2^-s)."""
    r = np.arange(1 << L, dtype=np.int64)
    x = r + (1 << (L + 4)) * 7          # a representative far from 0, same class; parities depend only on r mod 2^L
    lg = np.zeros(r.size)
    best = np.zeros(r.size)
    for _ in range(L):
        odd = (x & 1).astype(bool)
        lg += np.where(odd, np.log2(3.0) - 1.0, -1.0)
        best = np.maximum(best, lg)
        x = np.where(odd, (3 * x + c) >> 1, x >> 1)
    return best >= s


for path in sys.argv[1:]:
    c, N, cnt = load(path)
    A = cnt.size
    a = np.arange(1, A + 1, dtype=np.int64)
    v = a * cnt / N
    print(f"c={c:+d}  N={N:.1e}  targets <= 2^{int(np.log2(A))}")
    print("  window (X/2,X]   set   mean(v)   " + "  ".join(f"V_{p:<4}" for p in PS))
    for k in range(12, int(np.log2(A)) + 1):
        lo, hi = 1 << (k - 1), 1 << k
        for lab, sl in (("all", slice(lo, hi)), ("odd", slice(lo, hi, 2))):   # a = lo+1.. ; odd: lo+1, lo+3, ...
            w = v[sl]
            row = "  ".join(f"{np.mean(w ** p) ** (1 / p):7.2f}" for p in PS)
            print(f"  X=2^{k:<2}          {lab}   {w.mean():7.3f}   {row}")
    # dangerous first-passage inputs from the shell (B, 2B]
    B = 1 << 20
    z = np.arange(B + 1, 2 * B + 1, dtype=np.int64)
    hz = cnt[z - 1] / N
    e = pass_below(z, B, c)
    L = 14
    print(f"  shell (B,2B], B=2^20: inputs z whose first value e <= B lies in a class mod 2^{L} with growth >= 2^s within {L} steps")
    print("     s   uniform share   h-weighted share   ratio   Hoelder bound (p=3/2)")
    Vp = np.mean((z * hz) ** 1.5) ** (1 / 1.5) / np.mean(z * hz)
    for s in (1, 2, 3, 4, 6):
        D = danger_classes(L, s, c)
        E = D[e & ((1 << L) - 1)]
        fu = E.mean()
        fh = hz[E].sum() / hz.sum()
        print(f"    {s:2d}   {fu:12.5f}   {fh:15.5f}   {fh / fu:6.3f}   {2 * Vp * fu ** (1 / 3):10.4f}")
    print()
