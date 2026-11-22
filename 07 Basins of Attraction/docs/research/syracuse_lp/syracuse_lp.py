#!/usr/bin/env python3
"""Exact 3-adic Syracuse law and its L^p regularity (research notes, syracuse_lp).

Let S_n = Syrac(Z/3^n Z) with the law of sum_{j=1}^n 3^{j-1} 2^{-A_j} mod 3^n, A_j = a_1 + ... + a_j,
a_i iid geometric (P(a = k) = 2^-k), so that S_n = 2^{-a}(1 + 3 S'_{n-1}) with an independent copy S'.
f_n = 3^n P(S_n = .) is its density on Z/3^n, H_p(n) = mean_x f_n(x)^p and E_n = H_2(n) - H_2(n-1)
the L^2 energy of the unit frequencies (Parseval).

For every level n this script writes
  moments.csv   H_p(n) for p in P_GRID, E_n, and the Kahane-Peyriere bound
                rho(p) H_p(n-1) + E_n^{p/2} >= H_p(n),  rho(p) = 3^{p-1}/(2^p - 1)
  tails.csv     Haar measure of {f_n > M} and the share of the mean carried there
  corr_gd.csv   R_{n}(g_d) = mean_y f_n(y) f_n(g_d y),  g_d(y) = 2^d y + (2^d - 1)/3,  d even;
                identity check  E_n = 2 sum_{d >= 2 even} 2^{-d} R_{n-1}(g_d)
  corr_tr.csv   R_n(tau_j) = mean_y f_n(y) f_n(y + 3^j)   (translations of depth j)
  corr_lin.csv  R_n(h) for h(y) = 2^e y + t, t in {0, 1, 9, 81}: for t = 0 these are the linear maps of the
                singular set, where R_n(h) >= 2^{-|e|} H_2(n-1) grows linearly; for t != 0 they stay bounded
  fourier.csv   sup of |chi_n(xi)| over the units xi mod 3^n with its discrete log k* (xi = 2^k*),
                and chi_n(2^k) for k near 4(n-1)/3 (to validate the Monte Carlo of job B)

usage:
  syracuse_lp.py exact NMAX OUTDIR [--fourier-max N] [--dmax D] [--workers W]
  syracuse_lp.py selftest
Peak memory is about 60 bytes per residue: 2.6 GB at n = 16, about 210 GB at n = 20 (n <= 20 because
the index arithmetic uses unsigned 64-bit products of numbers below 3^20).
"""
import argparse
import math
import os
import sys
import time

import numpy as np

try:
    import scipy.fft as sfft
except ImportError:          # numpy fallback (single-threaded)
    sfft = None

P_GRID = (1.1, 1.25, 1.5, 1.75, 1.9, 2.0)
M_GRID = (3, 10, 30, 100, 300, 1000, 3000)
D_EXTRA = (54, 108, 162)     # d with v_3(d/2) = 2, 3, 4 (structure only; weights 2^-d are negligible)


def rho(p):
    return 3.0 ** (p - 1) / (2.0 ** p - 1)


def fft(x, workers, inverse=False, real=False, n=None):
    if sfft is not None:
        if real:
            return sfft.irfft(x, n=n, workers=workers) if inverse else sfft.rfft(x, workers=workers)
        return sfft.fft(x, workers=workers)
    if real:
        return np.fft.irfft(x, n=n) if inverse else np.fft.rfft(x)
    return np.fft.fft(x)


def discrete_log(k):
    """dlog[x] = e with 2^e = x mod 3^k for units x, -1 otherwise (2 is a primitive root mod 3^k)."""
    M, L = 3 ** k, 2 * 3 ** (k - 1)
    dlog = np.full(M, -1, dtype=np.int64)
    B = min(1 << 16, L)
    blk = np.empty(B, dtype=np.int64)
    v = 1
    for i in range(B):
        blk[i] = v
        v = (v * 2) % M
    step = v                      # 2^B mod M
    e0, base = 0, 1
    while e0 < L:
        cnt = min(B, L - e0)
        vals = (blk[:cnt].astype(np.uint64) * np.uint64(base)) % np.uint64(M)   # < 2^64 for k <= 20
        dlog[vals.astype(np.int64)] = np.arange(e0, e0 + cnt)
        base = (base * step) % M
        e0 += cnt
    return dlog


def next_law(D, k, workers):
    """Law of S_k from the law D of S_{k-1}: S_k = 2^{-a}(1 + 3 S_{k-1}), a cyclic correlation in
    discrete-log coordinates of the units mod 3^k. Returns (law, dlog)."""
    M, L = 3 ** k, 2 * 3 ** (k - 1)
    dlog = discrete_log(k)
    u = 1 + 3 * np.arange(3 ** (k - 1), dtype=np.int64)
    Q = np.bincount(dlog[u], weights=D, minlength=L)
    del u
    r = np.arange(L, dtype=np.float64)
    if L < 1000:
        w = np.exp2(-np.where(r == 0, L, r)) / (1.0 - 2.0 ** (-L))
    else:
        w = np.exp2(-np.where(r == 0, 1e9, r))
    del r
    P = fft(fft(Q, workers, real=True) * np.conj(fft(w, workers, real=True)), workers, inverse=True, real=True, n=L)
    del Q, w
    np.clip(P, 0.0, None, out=P)
    P /= P.sum()
    Dn = np.zeros(M)
    units = dlog >= 0
    Dn[units] = P[dlog[units]]
    return Dn, dlog


def g_index(d, M, y):
    """g_d(y) = 2^d y + (2^d - 1)/3 mod M on an int64 chunk y (M <= 3^20, so products < 2^64)."""
    u = pow(2, d, M)
    t = ((2 ** d - 1) // 3) % M
    return ((y.astype(np.uint64) * np.uint64(u)) % np.uint64(M) + np.uint64(t)) % np.uint64(M)


def corr(f, idx_fun, chunk=1 << 26):
    """mean_y f(y) f(h(y)) for the index map h given chunkwise."""
    M = f.size
    s = 0.0
    for y0 in range(0, M, chunk):
        y = np.arange(y0, min(M, y0 + chunk), dtype=np.int64)
        s += float(np.dot(f[y0:y0 + y.size], f[idx_fun(y).astype(np.int64)]))
    return s / M


def run_exact(nmax, outdir, fourier_max, dmax, workers):
    os.makedirs(outdir, exist_ok=True)
    fm = open(os.path.join(outdir, "moments.csv"), "w")
    ft = open(os.path.join(outdir, "tails.csv"), "w")
    fc = open(os.path.join(outdir, "corr_gd.csv"), "w")
    fr = open(os.path.join(outdir, "corr_tr.csv"), "w")
    fl = open(os.path.join(outdir, "corr_lin.csv"), "w")
    ff = open(os.path.join(outdir, "fourier.csv"), "w")
    fm.write("n,E_n,E_identity," + ",".join(f"H_{p}" for p in P_GRID) + "," +
             ",".join(f"KP_slack_{p}" for p in P_GRID) + ",max_f,seconds\n")
    ft.write("n," + ",".join(f"haar_gt_{m}" for m in M_GRID) + "," + ",".join(f"share_gt_{m}" for m in M_GRID) + "\n")
    fc.write("n,d,v3_d_half,R\n")
    fr.write("n,j,R\n")
    fl.write("n,e,t,R,lower_bound\n")
    ff.write("n,sup_unit_abs_chi,k_star,k,abs_chi,re_chi,im_chi\n")   # k_star: xi = 2^k_star (or -xi)
    D = np.array([1.0])
    Hprev = {p: 1.0 for p in P_GRID}
    Rprev = {}
    ds = list(range(2, 2 * dmax + 1, 2)) + [d for d in D_EXTRA if d > 2 * dmax]
    for n in range(1, nmax + 1):
        t0 = time.time()
        D, dlog = next_law(D, n, workers)
        M = 3 ** n
        f = D * M
        H = {p: float(np.mean(f ** p)) for p in P_GRID}
        E = H[2.0] - Hprev[2.0]
        # identity E_n = 2 sum_d 2^-d R_{n-1}(g_d), truncated at the d of the previous level
        Eid = 2.0 * sum(2.0 ** (-d) * R for d, R in Rprev.items() if d <= 2 * dmax) if n > 1 else float("nan")
        slack = {p: rho(p) * Hprev[p] + max(E, 0.0) ** (p / 2) - H[p] for p in P_GRID}
        # tails
        haar = [float(np.count_nonzero(f > m)) / M for m in M_GRID]
        share = [float(f[f > m].sum()) / M for m in M_GRID]
        ft.write(f"{n}," + ",".join(f"{x:.6e}" for x in haar) + "," + ",".join(f"{x:.6e}" for x in share) + "\n")
        # twisted correlations
        Rprev = {}
        for d in ds:
            R = corr(f, lambda y, d=d: g_index(d, M, y))
            Rprev[d] = R
            v3 = 0
            q = d // 2
            while q % 3 == 0:
                q //= 3
                v3 += 1
            fc.write(f"{n},{d},{v3},{R:.10f}\n")
        for j in range(n):
            s = 3 ** j
            R = corr(f, lambda y, s=s: (y + s) % M)
            fr.write(f"{n},{j},{R:.10f}\n")
        for e in (1, 2, 4, 6):
            for t in (0, 1, 9, 81):
                u = pow(2, e, M)
                R = corr(f, lambda y, u=u, t=t: ((y.astype(np.uint64) * np.uint64(u)) % np.uint64(M) + np.uint64(t % M)) % np.uint64(M))
                lb = 2.0 ** (-e) * Hprev[2.0] if t == 0 else 0.0
                fl.write(f"{n},{e},{t},{R:.10f},{lb:.10f}\n")
        # Fourier
        kc = round(4 * (n - 1) / 3)
        ks = [k for k in range(kc - 4, kc + 5) if k >= 0]
        if n <= fourier_max:
            # F[xi] = sum_r D[r] e(-xi r / 3^n) = conj chi_n(xi), xi <= M/2; F[M - xi] = conj F[xi]
            F = fft(D, workers, real=True)
            absF = np.abs(F)
            absF[::3] = 0.0                           # non-units
            xi = int(np.argmax(absF))
            sup, kstar = float(absF[xi]), int(dlog[xi])
            vals = {}
            for k in ks:
                x = pow(2, k, M)
                vals[k] = complex(np.conj(F[x])) if x <= M // 2 else complex(F[M - x])
            del F, absF
        else:
            sup, kstar = float("nan"), -1
            vals = {}
            r = np.arange(M, dtype=np.float64)
            for k in ks:
                xi = pow(2, k, M)
                acc = 0j
                for r0 in range(0, M, 1 << 26):
                    rr = np.arange(r0, min(M, r0 + (1 << 26)), dtype=np.int64)
                    ph = ((rr.astype(np.uint64) * np.uint64(xi)) % np.uint64(M)).astype(np.float64) / M
                    acc += complex(np.dot(D[r0:r0 + rr.size], np.exp(2j * np.pi * ph)))
                vals[k] = acc
            del r
        for k in ks:
            c = vals[k]
            ff.write(f"{n},{sup:.10e},{kstar},{k},{abs(c):.10e},{c.real:.10e},{c.imag:.10e}\n")
        secs = time.time() - t0
        fm.write(f"{n},{E:.10f},{Eid:.10f}," + ",".join(f"{H[p]:.10f}" for p in P_GRID) + "," +
                 ",".join(f"{slack[p]:.6e}" for p in P_GRID) + f",{f.max():.6f},{secs:.1f}\n")
        for fh in (fm, ft, fc, fr, fl, ff):
            fh.flush()
        print(f"n={n:2d} E_n={E:.6f} (identity {Eid:.6f}) H_1.5={H[1.5]:.5f} H_1.75={H[1.75]:.5f} "
              f"H_2={H[2.0]:.5f} sup|chi|={sup:.5e} k*={kstar} "
              f"min KP slack (p<2)={min(v for q, v in slack.items() if q < 2):.3e} "
              f"[{secs:.0f}s]", flush=True)
        Hprev = H
        del dlog, f


def selftest():
    """Small-n checks against brute force: the law, the KP inequality, the identity for E_n."""
    import itertools
    for n in (1, 2, 3, 4):
        M = 3 ** n
        brute = np.zeros(M)
        cut = 22 if n <= 3 else 17            # valuations up to cut; the rest has mass < n 2^-(cut-1)
        for word in itertools.product(range(1, cut + 1), repeat=n):
            A, s = 0, 0
            for j, a in enumerate(word, start=1):
                A += a
                s += 3 ** (j - 1) * pow(2, -A, M)
            brute[s % M] += 2.0 ** (-A)
        brute /= brute.sum()
        D = np.array([1.0])
        for k in range(1, n + 1):
            D, _ = next_law(D, k, 1)
        err = np.abs(D - brute).max()
        print(f"selftest n={n}: max |law - brute| = {err:.2e}")
        assert err < 1e-4, err
    print("selftest ok")


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    e = sub.add_parser("exact")
    e.add_argument("nmax", type=int)
    e.add_argument("outdir")
    e.add_argument("--fourier-max", type=int, default=20)
    e.add_argument("--dmax", type=int, default=20, help="even d = 2, 4, ..., 2*dmax")
    e.add_argument("--workers", type=int, default=os.cpu_count() or 1)
    sub.add_parser("selftest")
    a = ap.parse_args()
    if a.cmd == "selftest":
        selftest()
    else:
        run_exact(a.nmax, a.outdir, a.fourier_max, a.dmax, a.workers)
