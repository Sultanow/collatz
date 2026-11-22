#!/usr/bin/env python3
"""Ulam (mean-field) discretisation of the forward renormalisation walk on affine maps (roadmap A4).

States: classes c = (u mod 3^s, t mod 3^s) of affine isometries h(y) = u y + t (u a unit), 2*3^(2s-1) classes.
One step: (a, b) with weight 3*2^(-a-b) and indicator 2^(b-a) u + 2^b t = 1 (mod 3), new state
h_{a,b} = (2^(b-a) u, (2^(b-a) u + 2^b t - 1)/3); the new class mod 3^s needs (u, t) mod 3^(s+1), and the
mean-field operator T_s averages over the 9 lifts.  Column sums are the one-step weights W(c mod 3); the uniform
vector is fixed (eigenvalue 1, Haar invariance).  The subdominant eigenvalues predict the decay rates of the class
deviations of the walk.
usage: ulam_spectrum.py SMAX
"""
import sys

import numpy as np

AMAX = 24


def T_matrix(s):
    M, M1 = 3 ** s, 3 ** (s + 1)
    units = np.array([u for u in range(M) if u % 3], dtype=np.int64)
    n = units.size * M
    uu = np.repeat(units, M)                       # class index i = (position of u) * M + t
    tt = np.tile(np.arange(M, dtype=np.int64), units.size)
    pos = np.full(M, -1, dtype=np.int64)
    pos[units] = np.arange(units.size)
    src = np.repeat(np.arange(n), 9)
    U = np.repeat(uu, 9) + np.tile(np.repeat(np.arange(3), 3), n) * M
    Tt = np.repeat(tt, 9) + np.tile(np.tile(np.arange(3), 3), n) * M
    T = np.zeros((n, n))
    for a in range(1, AMAX + 1):
        for b in range(1, AMAX + 1):
            v = (pow(2, b - a, M1) * U + pow(2, b, M1) * Tt) % M1
            ok = v % 3 == 1
            u2 = (pow(2, b - a, M1) * U[ok]) % M
            t2 = ((v[ok] - 1) // 3) % M
            dst = pos[u2] * M + t2
            np.add.at(T, (dst, src[ok]), 3 * 2.0 ** (-a - b) / 9)
    return T, n


def main(smax):
    for s in range(1, smax + 1):
        T, n = T_matrix(s)
        cs = T.sum(axis=0)
        rs = T.sum(axis=1)
        ev = np.linalg.eigvals(T)
        ev = ev[np.argsort(-np.abs(ev))]
        print(f"s = {s}: {n} classes; column sums (= W) in [{cs.min():.4f}, {cs.max():.4f}], "
              f"row sums in [{rs.min():.6f}, {rs.max():.6f}]")
        print("   largest |eigenvalues|: " + " ".join(f"{abs(x):.4f}" for x in ev[:12]))
        print("   leading eigenvalues:   " + " ".join(f"{x.real:+.4f}{x.imag:+.4f}i" for x in ev[:8]))


if __name__ == "__main__":
    main(int(sys.argv[1]))


def sparse_spectrum(s, k=10):
    """sparse version for larger s (scipy)."""
    import scipy.sparse as sp
    import scipy.sparse.linalg as sla
    M, M1 = 3 ** s, 3 ** (s + 1)
    units = np.array([u for u in range(M) if u % 3], dtype=np.int64)
    n = units.size * M
    uu = np.repeat(units, M)
    tt = np.tile(np.arange(M, dtype=np.int64), units.size)
    pos = np.full(M, -1, dtype=np.int64)
    pos[units] = np.arange(units.size)
    src = np.repeat(np.arange(n), 9)
    U = np.repeat(uu, 9) + np.tile(np.repeat(np.arange(3), 3), n) * M
    Tt = np.repeat(tt, 9) + np.tile(np.tile(np.arange(3), 3), n) * M
    rows, cols, vals = [], [], []
    for a in range(1, AMAX + 1):
        for b in range(1, AMAX + 1):
            v = (pow(2, b - a, M1) * U + pow(2, b, M1) * Tt) % M1
            ok = v % 3 == 1
            u2 = (pow(2, b - a, M1) * U[ok]) % M
            t2 = ((v[ok] - 1) // 3) % M
            rows.append(pos[u2] * M + t2)
            cols.append(src[ok])
            vals.append(np.full(ok.sum(), 3 * 2.0 ** (-a - b) / 9))
    T = sp.csr_matrix((np.concatenate(vals), (np.concatenate(rows), np.concatenate(cols))), shape=(n, n))
    ev = sla.eigs(T, k=k, which="LM", return_eigenvectors=False, tol=1e-10, maxiter=5000)
    return n, ev[np.argsort(-np.abs(ev))]
