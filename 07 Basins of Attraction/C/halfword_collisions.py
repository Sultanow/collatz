#!/usr/bin/env python3
"""Run J (continued): half-word collisions for the algebraic factor P = 2^h + 3^kappa of D = 2^l - 3^k (l = 2h, k = 2 kappa).

Lemma (Section 4): for B = B1 B2 with half-words of length h,
    z_3(B) = 3^{k2} z_3(B1) + 2^h z_3(B2)  ==  3^{k2} z_3(B1) - 3^kappa z_3(B2)   (mod P);
P | z_3 is rotation invariant, every word has a balanced rotation (k1 = k2 = kappa), and for balanced words
    P | z_3(B)  <=>  Delta = z_3(B2) - z_3(B1) = m P,   m = 0  <=>  B = B'B' (periodic).
This script
  1. checks the formula and the direct counts of algebraic_factor.c for small types,
  2. counts the balanced solutions m != 0 (ordered pairs of distinct balanced half-words) against the heuristic
     (n^2 - n)/P, n = C(h, kappa), and splits the heuristic into the zero slot |Delta| < P/2 and the rest,
  3. predicts the count from the rest with the exact local densities of Delta modulo 2^6 and 3^4,
  4. shows that for D < 0 the counts depend only on the number of zeros per half-word.
usage: halfword_collisions.py LMAX > halfword_report.txt
"""
import sys
from collections import Counter
from itertools import combinations
from math import comb

import numpy as np


def z3(pos, k):
    return sum(3 ** (k - 1 - j) * (1 << p) for j, p in enumerate(pos))


def runs_form(pos, h, kap):
    """z_3 of a half-word as a signed sum over its runs of ones: sum 3^U 2^a (3^L - 2^L)."""
    s, tot, p, U = set(pos), 0, 0, kap
    while p < h:
        if p in s:
            a = p
            while p < h and p in s:
                p += 1
            L = p - a
            U -= L
            tot += 3 ** U * 2 ** a * (3 ** L - 2 ** L)
        else:
            p += 1
    return tot


def is_aperiodic(w, l):
    full, r = (1 << l) - 1, w
    for _ in range(1, l):
        r = ((r >> 1) | ((r & 1) << (l - 1))) & full
        if r == w:
            return False
    return True


def direct_count(l, k, P):
    na = nP = 0
    for c in combinations(range(l), k):
        if is_aperiodic(sum(1 << p for p in c), l):
            na += 1
            nP += z3(c, k) % P == 0
    return na, nP


def balanced(h, kap, a2=6, a3=4):
    P = 2 ** h + 3 ** kap
    vals = [z3(c, kap) for c in combinations(range(h), kap)]
    n, s = len(vals), set(vals)
    M = (max(vals) - min(vals)) // P
    arr = np.array(vals, dtype=np.float64)
    D = (arr[None, :] - arr[:, None]).ravel()
    off = D != 0
    bins = Counter(np.round(D[off] / P).astype(np.int64).tolist())
    loc = {}
    for p, a in ((2, a2), (3, a3)):
        q = p ** a
        cz = Counter(v % q for v in vals)
        dist = Counter()
        for x, cx in cz.items():
            for y, cy in cz.items():
                dist[(y - x) % q] += cx * cy
        loc[p] = (q, dist)
    obs = smooth = pred = 0.0
    for m in range(-M - 1, M + 2):
        if m == 0:
            continue
        o = sum(1 for v in vals if v + m * P in s)
        b = bins.get(m, 0) / P
        lam = 1.0
        for q, dist in loc.values():
            lam *= q * dist[(m * P) % q] / (n * n)
        obs, smooth, pred = obs + o, smooth + b, pred + b * lam
    return P, n, obs, (n * n - n) / P, smooth, pred


def main(lmax):
    print("1. checks")
    ok = all(runs_form(c, 10, 6) == z3(c, 6) for c in combinations(range(10), 6))
    ok2 = True
    for h in range(2, 7):
        for k1 in range(h + 1):
            for k2 in range(h + 1):
                for c1 in combinations(range(h), k1):
                    for c2 in combinations(range(h), k2):
                        full = list(c1) + [h + p for p in c2]
                        ok2 &= z3(full, k1 + k2) == 3 ** k2 * z3(c1, k1) + 2 ** h * z3(c2, k2)
    print(f"   z_3(B1 B2) = 3^k2 z_3(B1) + 2^h z_3(B2) for all words with l <= 12: {ok2}; run decomposition: {ok}")
    for l, k in ((12, 4), (14, 6), (16, 8), (18, 8), (20, 10)):
        P = 2 ** (l // 2) + 3 ** (k // 2)
        print(f"   (l,k) = ({l},{k}), P = {P}: aperiodic words with P | z_3 (direct) = {direct_count(l, k, P)[1]}")
    print("\n2./3. balanced solutions m != 0 against the heuristic (n^2-n)/P (types with heuristic >= 5)")
    print("  l  k  D     obs   heuristic  zero-slot  rest   local-pred  obs/heur  obs/pred")
    tot = {"D>0": [0.0] * 5, "D<0": [0.0] * 5}
    for h in range(4, lmax // 2 + 1):
        for kap in range(1, h + 1):
            P, n, obs, exp, smooth, pred = balanced(h, kap)
            if exp < 5:
                continue
            sg = "D>0" if 4 ** h > 9 ** kap else "D<0"
            t = tot[sg]
            for i, v in enumerate((obs, exp, smooth, pred, 1)):
                t[i] += v
            print(f" {2*h:2d} {2*kap:2d} {sg} {obs:6.0f} {exp:10.2f} {exp - smooth:9.2f} {smooth:7.2f} {pred:10.2f} {obs/exp:9.3f} {obs/pred:9.3f}")
    for sg, (o, e, sm, pr, nt) in tot.items():
        print(f"  pooled {sg} ({nt:.0f} types): observed/heuristic {o/e:.3f}; zero slot {1 - sm/e:.3f} of the heuristic; "
              f"local prediction/heuristic {pr/e:.3f}; observed/prediction {o/pr:.3f}")
    print("\n4. D < 0: counts by the number of zeros per half-word (observed / heuristic)")
    for zeros in (3, 4, 5, 6):
        row = []
        for h in range(zeros + 4, 22):
            kap = h - zeros
            if 9 ** kap <= 4 ** h:
                continue
            P = 2 ** h + 3 ** kap
            vals = set(z3(c, kap) for c in combinations(range(h), kap))
            n = len(vals)
            M = (max(vals) - min(vals)) // P
            obs = sum(1 for v in vals for m in range(-M - 1, M + 2) if m and v + m * P in vals)
            row.append(f"h={h}: {obs}/{(n*n-n)/P:.2f}")
        print(f"  {zeros} zeros: " + ", ".join(row))


if __name__ == "__main__":
    main(int(sys.argv[1]))
