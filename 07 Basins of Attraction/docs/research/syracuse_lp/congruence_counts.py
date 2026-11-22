#!/usr/bin/env python3
"""Run O: hit weights of the renormalisation walk and the first-hit (renewal) decomposition of Cor_N(g_d).

Words A = (a_1..a_k) with independent geometric letters, S(A) = sum_{j<=k} 3^{j-1} 2^{-A_j} (truncated Syracuse
sum, computed to any precision 3^N, N >= k), |A| = a_1 + ... + a_k.  For even d (d = 0 is the identity) the walk
started at g_d(y) = 2^d y + (2^d-1)/3 is at depth k in the state
    h_{A,B}(y) = 2^{|B|+d-|A|} y + 2^{|B|} (g_d(S(A)) - S(B)) / 3^k ,
and it is the identity modulo 3^r ("hit at (k, r)") iff
    |A| - |B| = d  (mod 2*3^{r-1})   and   S(B) = g_d(S(A))  (mod 3^{k+r})      (no length condition for r = 0).
The hit weight is  w_d(k, r) = 3^k P(hit at (k, r)).  For r = 0, w_d(k, 0) = Cor_k(g_d); for d = 0,
K(j, r) = w_0(j, r) is the identity kernel (K(j, 0) = H_2(j), K(0, r) = 1).

Renewal equation (fixed N = k + r):  w_d(k, N-k) = sum_{k'=0}^{k} FH_d(k', N-k') K(k-k', N-k),
where FH_d(k', N-k') is the weight of the pairs whose walk is the identity mod 3^{N-k'} at depth k' for the first
time.  At k = N:  Cor_N(g_d) = sum_{k'} FH_d(k', N-k') H_2(N-k') = FH_d(N, 0) + (near-resonance part).

Computation: for each length phase theta = j/LT the complex "law" F_theta(x) = E[1{S(A) = x} e(theta(|A|-k))]
mod 3^N is computed by the recursion S(A) = 2^{-a}(1 + 3 S(A')) (discrete logarithms, FFT), started from the point
mass at 0 modulo 3^{N-k}.  Phi_theta = sum_x F_theta(x) conj F_theta(g_d x); an inverse DFT over theta gives the law
of |A| - |B| on the coincidence event (LT = 162, so the classes mod 2*3^{r-1} are exact for r <= 5, and for r >= 6
the condition is |A| - |B| = d up to aliasing at |A| - |B| = d +- 162, of probability below 1e-12 for k <= 17).

usage:
  congruence_counts.py run NMAX OUTDIR [--d 0,2,4,6] [--theta-slice I/S]   (slice: compute theta indices = I mod S)
  congruence_counts.py merge OUTDIR NMAX FILES...                          (combine theta slices, renewal, report)
  congruence_counts.py selftest
"""
import argparse
import math
import os
import sys
from fractions import Fraction
from itertools import product

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from syracuse_lp import discrete_log  # noqa: E402

LT = 162


def weights(L, theta):
    """w[s] = sum_{a >= 1, a = s mod L} 2^-a e(theta (a-1))  (complex), s = 0..L-1."""
    s = np.arange(L, dtype=np.float64)
    z = 0.5 * np.exp(2j * np.pi * theta)
    zL = z ** L
    w = np.empty(L, dtype=np.complex128)
    w[1:] = (z ** s[1:]) * np.exp(-2j * np.pi * theta) / (1 - zL)
    w[0] = zL * np.exp(-2j * np.pi * theta) / (1 - zL)
    return w


class Chain:
    def __init__(self, nmax):
        self.dlog = {m: discrete_log(m).astype(np.int32) for m in range(1, nmax + 1)}   # 3^m < 2^31 for m <= 19
        self.wcache = {}

    def step(self, D, m, theta):
        """law of 2^{-a}(1 + 3X) mod 3^m from the law D of X mod 3^{m-1} (complex weights)."""
        M, L = 3 ** m, 2 * 3 ** (m - 1)
        dl = self.dlog[m]
        idx = dl[1 + 3 * np.arange(3 ** (m - 1), dtype=np.int64)]
        Q = np.bincount(idx, weights=D.real, minlength=L) + 1j * np.bincount(idx, weights=D.imag, minlength=L)
        if self.wcache.get("theta") != theta:
            self.wcache = {"theta": theta}
        key = m
        if key not in self.wcache:
            w = weights(L, theta)
            wrev = np.roll(w[::-1], 1)                 # wrev[r] = w[-r mod L]
            self.wcache[key] = np.fft.fft(wrev)
        P = np.fft.ifft(np.fft.fft(Q) * self.wcache[key])   # P[s] = sum_r Q[s + r] w[r]
        out = np.zeros(M, dtype=np.complex128)
        units = dl >= 0
        out[units] = P[dl[units]]
        return out

    def law(self, N, k, theta):
        D = np.zeros(3 ** (N - k), dtype=np.complex128)
        D[0] = 1.0
        for m in range(N - k + 1, N + 1):
            D = self.step(D, m, theta)
        return D


def reduce_mod(F, N):
    M = 3 ** N
    return F.reshape(-1, M).sum(axis=0) if F.size > M else F


def gd_index(d, N):
    M = 3 ** N
    y = np.arange(M, dtype=np.int64)
    return (y * pow(2, d, M) + ((2 ** d - 1) // 3) % M) % M


def run(nmax, outdir, ds, slice_i, slice_s):
    os.makedirs(outdir, exist_ok=True)
    ch = Chain(nmax)
    js = [j for j in range(LT // 2 + 1) if j % slice_s == slice_i]
    path = os.path.join(outdir, f"phi_{slice_i}of{slice_s}.csv")
    with open(path, "w") as fo:
        fo.write("j,N,k,d,re,im\n")
        for j in js:
            theta = j / LT
            for k in range(1, nmax + 1):
                F = ch.law(nmax, k, theta)              # S^(k) mod 3^nmax
                for N in range(nmax, k - 1, -1):
                    F = reduce_mod(F, N)
                    for d in ds:
                        phi = complex(np.vdot(F[gd_index(d, N)], F))   # sum_x F(x) conj F(g_d x)
                        fo.write(f"{j},{N},{k},{d},{phi.real:.17e},{phi.imag:.17e}\n")
            print(f"theta index {j} done", flush=True)
    return path


def load_phi(files):
    phi = {}
    for p in files:
        with open(p) as f:
            next(f)
            for line in f:
                j, N, k, d, re, im = line.split(",")
                phi[(int(j), int(N), int(k), int(d))] = complex(float(re), float(im))
    return phi


def hit_weights(phi, nmax, ds):
    """w[d][(k, r)] for 1 <= k <= N <= nmax, r = N - k; plus k = 0 entries."""
    w = {d: {} for d in ds}
    jgrid = np.arange(LT)
    for d in ds:
        for N in range(1, nmax + 1):
            for k in range(1, N + 1):
                full = np.empty(LT, dtype=np.complex128)
                for j in range(LT // 2 + 1):
                    full[j] = phi[(j, N, k, d)]
                for j in range(LT // 2 + 1, LT):
                    full[j] = np.conj(phi[(LT - j, N, k, d)])
                # G[delta mod LT] = (1/LT) sum_j e(-delta j / LT) Phi_j
                G = np.real(np.fft.fft(full)) / LT
                r = N - k
                if r == 0:
                    tot = G.sum()
                else:
                    L = 2 * 3 ** (r - 1)
                    if LT % L == 0:
                        tot = sum(G[delta % LT] for delta in range(-LT, LT) if (delta - d) % L == 0 and -LT // 2 < delta <= LT // 2)
                    else:
                        tot = G[d % LT]
                w[d][(k, r)] = 3 ** k * tot
        # depth 0: g_d is the identity mod 3^N ?
        for N in range(0, nmax + 1):
            M = 3 ** N
            ok = (pow(2, d, M) == 1 % M) and (((2 ** d - 1) // 3) % M == 0) if N > 0 else True
            w[d][(0, N)] = 1.0 if ok else 0.0
    return w


def renewal(w, K, H2, nmax):
    """first-hit weights FH[(k', r')] with k' + r' = N for every N <= nmax."""
    FH = {}
    for N in range(0, nmax + 1):
        for k in range(0, N + 1):
            r = N - k
            s = w[(k, r)]
            for kp in range(0, k):
                kk = k - kp
                Kv = H2[kk] if r == 0 else K[(kk, r)]
                s -= FH[(kp, N - kp)] * Kv
            FH[(k, r)] = s
    return FH


def merge(outdir, nmax, files, ds=(0, 2, 4, 6)):
    phi = load_phi(files)
    ds = sorted({k[3] for k in phi})
    w = hit_weights(phi, nmax, ds)
    K = w[0]
    H2 = {0: 1.0}
    H2.update({j: K[(j, 0)] for j in range(1, nmax + 1)})
    rep = open(os.path.join(outdir, "run_O_summary.txt"), "w")

    def P(s=""):
        print(s)
        rep.write(s + "\n")

    with open(os.path.join(outdir, "hit_weights.csv"), "w") as fw:
        fw.write("d,k,r,w,rho\n")
        for d in ds:
            for (k, r), v in sorted(w[d].items()):
                if k >= 1:
                    fw.write(f"{d},{k},{r},{v:.12e},{v * 2 * 3 ** (2 * r - 1) / w[d][(k, 0)] if r else 1.0:.6e}\n")
    P(f"Run O: hit weights and renewal decomposition, N <= {nmax}, LT = {LT}")
    P("H_2(N) from K(N, 0): " + " ".join(f"{H2[j]:.6f}" for j in range(1, nmax + 1)))
    for d in ds:
        if d == 0:
            continue
        FH = renewal(w[d], K, H2, nmax)
        with open(os.path.join(outdir, f"first_hits_d{d}.csv"), "w") as ff:
            ff.write("N,k,r,FH,FH_times_H2r\n")
            for (k, r), v in sorted(FH.items(), key=lambda t: (t[0][0] + t[0][1], t[0][0])):
                ff.write(f"{k + r},{k},{r},{v:.12e},{v * H2[r]:.12e}\n")
        P(f"\nd = {d}:  Cor_N(g_d) = FH(N, 0) [regular part] + sum_r FH(N-r, r) H_2(r) [near-resonance part]")
        P("   N   Cor_N(g_d)    regular     near-res.   min FH")
        for N in range(1, nmax + 1):
            cor = w[d][(N, 0)]
            reg = FH[(N, 0)]
            res = sum(FH[(N - r, r)] * H2[r] for r in range(1, N + 1))
            mn = min(FH[(N - r, r)] for r in range(0, N + 1))
            P(f"  {N:2d}  {cor:.8f}  {reg:.8f}  {res:.8f}  {mn:+.1e}")
        P(f"  rho_d(k, r) = w_d(k, r) |Aff(Z/3^r)| / Cor_k(g_d), equal to 1 if the depth-k states were equidistributed")
        P(f"  modulo 3^r (|Aff(Z/3^r)| = 2*3^(2r-1)); rows k, columns r = 1..:")
        for k in range(1, nmax):
            P(f"  k={k:2d}: " + " ".join(f"{w[d][(k, r)] * 2 * 3 ** (2 * r - 1) / w[d][(k, 0)]:7.4f}" for r in range(1, nmax - k + 1)))
        P(f"  9^r FH_d(k, r):")
        for k in range(1, nmax):
            P(f"  k={k:2d}: " + " ".join(f"{FH[(k, r)] * 9 ** r:7.4f}" for r in range(1, nmax - k + 1)))
    P("\n  9^r K(j, r) - 9^r (identity kernel, diagonal removed: off-diagonal coincidences):")
    for j in range(1, nmax):
        P(f"  j={j:2d}: " + " ".join(f"{(K[(j, r)] - 1) * 9 ** r:7.4f}" for r in range(1, nmax - j + 1)))
    rep.close()


# ---------------- brute force (selftest) ----------------
def v3(fr):
    if fr == 0:
        return 10 ** 9
    n, dd = fr.numerator, fr.denominator
    v = 0
    while n % 3 == 0:
        n //= 3
        v += 1
    while dd % 3 == 0:
        dd //= 3
        v -= 1
    return v


def brute_first_hits(d, N, amax):
    """FH_d(k, N-k) by walking pairs of letters <= amax (states as exact rationals)."""
    FH = [0.0] * (N + 1)

    def is_id(e, t, s):
        if s <= 0:
            return True
        return e % (2 * 3 ** (s - 1)) == 0 and v3(t) >= s

    e0, t0 = d, Fraction(2 ** d - 1, 3)
    if is_id(e0, t0, N):
        FH[0] = 1.0
        return FH
    stack = [(e0, t0, 0, 1.0)]
    while stack:
        e, t, k, wt = stack.pop()
        for a in range(1, amax + 1):
            for b in range(1, amax + 1):
                # h_{a,b}(y) = 2^{b-a} u y + (2^{b-a} u + 2^b t - 1)/3 with u = 2^e
                e2 = e + b - a
                t2 = (Fraction(2) ** e2 + 2 ** b * t - 1) / 3
                if v3(t2) < 0:
                    continue
                w2 = wt * 3 * 2.0 ** (-a - b)
                if is_id(e2, t2, N - k - 1):
                    FH[k + 1] += w2
                elif k + 1 < N:
                    stack.append((e2, t2, k + 1, w2))
    return FH


def selftest():
    nmax = 4
    import tempfile
    tmp = tempfile.mkdtemp()
    p = run(nmax, tmp, [0, 2, 6], 0, 1)
    phi = load_phi([p])
    w = hit_weights(phi, nmax, [0, 2, 6])
    K = w[0]
    H2 = {0: 1.0, **{j: K[(j, 0)] for j in range(1, nmax + 1)}}
    print("H_2:", [round(H2[j], 10) for j in range(1, nmax + 1)], "(expected 1.6666666667 2.1428571429 2.6044318232 3.0686462316)")
    ok = True
    for d in (2, 6):
        FH = renewal(w[d], K, H2, nmax)
        for N in (1, 2, 3):
            amax = 14 if N < 3 else 10
            bf = brute_first_hits(d, N, amax)
            fh = [FH[(k, N - k)] for k in range(0, N + 1)]
            err = max(abs(x - y) for x, y in zip(bf, fh))
            tol = 2 * N * 2.0 ** (-amax) * 4      # weight lost by truncating the letters at amax
            print(f"d={d} N={N}: FFT {np.round(fh, 6)}  brute(a,b<={amax}) {np.round(bf, 6)}  max diff {err:.1e} (tol {tol:.1e})")
            ok &= err < tol and all(x <= y + 1e-12 for x, y in zip(bf, fh))
        cor = [w[d][(N, 0)] for N in range(1, nmax + 1)]
        print(f"d={d}: Cor_N(g_d) =", np.round(cor, 10))
    print("selftest", "OK" if ok else "FAILED")


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd")
    a = sub.add_parser("run")
    a.add_argument("nmax", type=int)
    a.add_argument("outdir")
    a.add_argument("--d", default="0,2,4,6")
    a.add_argument("--theta-slice", default="0/1")
    b = sub.add_parser("merge")
    b.add_argument("outdir")
    b.add_argument("nmax", type=int)
    b.add_argument("files", nargs="+")
    sub.add_parser("selftest")
    args = ap.parse_args()
    if args.cmd == "run":
        ds = [int(x) for x in args.d.split(",")]
        assert 0 in ds
        i, s = (int(x) for x in args.theta_slice.split("/"))
        run(args.nmax, args.outdir, ds, i, s)
    elif args.cmd == "merge":
        merge(args.outdir, args.nmax, args.files)
    elif args.cmd == "selftest":
        selftest()
    else:
        ap.print_help()


if __name__ == "__main__":
    main()
