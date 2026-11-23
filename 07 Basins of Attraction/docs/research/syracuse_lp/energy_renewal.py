#!/usr/bin/env python3
"""Run O, all d (roadmap A7): hit weights and first-hit decomposition for every g_d with |d| <= 2*DMAX and for the
energy E_{N+1} = sum_{d != 0} 2^{-|d|} Cor_N(g_d) itself.

Notation as in congruence_counts.py (Run O).  In the logarithmic chart Lambda(x) = log(1+3x)/log 4 every g_d is the
translation by d/2 (Lemma "logarithmic chart"), so for each length phase theta
    Phi_theta(d) = sum_x F_theta(x) conj F_theta(g_d x) = sum_j G_theta(j) conj G_theta(j + d/2),
with G_theta the law of Lambda(S(A)) mod 3^N, is a cyclic autocorrelation: one FFT gives all d at once.  The
inverse DFT over theta gives the law of |A| - |B| on the coincidence event, as in Run O.

For d = 2 * 3^s * m (3 does not divide m), g_d is the identity modulo 3^s and not modulo 3^(s+1):
  * Cor_N(g_d) = H_2(N) for N <= s;
  * the diagonal pairs A = B reach the identity modulo 3^r at depth k only if k + r <= s, so for N > s the
    first-hit decomposition Cor_N(g_d) = F_d(N, 0) + sum_{r=1}^{N-1} F_d(N-r, r) H_2(r) only involves the hit
    weights with k >= 1 and k + r > s.  Hence the bootstrap constant
        c_d = (3/2) sum_r max{rho_d(k, r) : k >= 1, k + r > s} (5/27)^r
    and the bound  sup_N Cor_N(g_d) <= max(H_2(s), F*_d / (1 - c_d)),  F*_d = sup_{N > s} F_d(N, 0).
The energy walk starts at the mixture mu = sum_{d != 0} 2^{-|d|} delta_{g_d}; its hit weights are
w_E = sum_d 2^{-|d|} w_d, with w_E(k, 0) = E_{k+1}, and
        sup_n E_n <= max(2/3, F*_E / (1 - c_E)),  c_E = (3/2) sum_r max_{k >= 0} rho_E(k, r) (5/27)^r.
Truncation at |d/2| <= DMAX changes the mixture by a weight below 4^-DMAX.

usage:
  energy_renewal.py run NMAX OUTDIR [--dmax 40] [--theta-slice I/S]
  energy_renewal.py merge OUTDIR NMAX FILES...
  energy_renewal.py selftest
"""
import argparse
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from congruence_counts import LT, Chain, brute_first_hits, gd_index, reduce_mod, renewal  # noqa: E402
from syracuse_lp import discrete_log  # noqa: E402


def v3int(x):
    if x == 0:
        return 10 ** 9
    v = 0
    while x % 3 == 0:
        x //= 3
        v += 1
    return v


def log_chart_table(n):
    """lam[x] = Lambda(x) mod 3^n for 0 <= x < 3^n, from 1 + 3x = 2^e mod 3^(n+1), Lambda = e/2."""
    dl = discrete_log(n + 1)
    e = dl[1 + 3 * np.arange(3 ** n, dtype=np.int64)]
    assert (e >= 0).all() and (e % 2 == 0).all()
    lam = (e // 2).astype(np.int64)
    del dl
    assert np.unique(lam).size == 3 ** n
    return lam


def autocorr(F, lam, N, deltas):
    """Phi(delta) = sum_x F(x) conj F(g_{2 delta} x) for the law F mod 3^N."""
    M = 3 ** N
    G = np.empty(M, dtype=np.complex128)
    G[lam[:M] % M] = F
    Gh = np.fft.fft(G)
    a = np.fft.ifft(Gh * np.conj(Gh))            # a(t) = sum_j G(j + t) conj G(j)
    return a[(-deltas) % M]                       # Phi(delta) = a(-delta)


def run(nmax, outdir, dmax, slice_i, slice_s):
    os.makedirs(outdir, exist_ok=True)
    ch = Chain(nmax)
    lam = log_chart_table(nmax)
    js = [j for j in range(LT // 2 + 1) if j % slice_s == slice_i]
    deltas = np.arange(-dmax, dmax + 1)
    out = np.zeros((len(js), nmax + 1, nmax + 1, deltas.size), dtype=np.complex128)
    for jj, j in enumerate(js):
        theta = j / LT
        for k in range(1, nmax + 1):
            F = ch.law(nmax, k, theta)
            for N in range(nmax, k - 1, -1):
                F = reduce_mod(F, N)
                out[jj, N, k, :] = autocorr(F, lam, N, deltas)
        print(f"theta index {j} done", flush=True)
    path = os.path.join(outdir, f"phiall_{slice_i}of{slice_s}.npz")
    np.savez(path, js=np.array(js), phi=out, nmax=nmax, dmax=dmax)
    return path


def load(files):
    phi, nmax, dmax = {}, None, None
    for p in files:
        z = np.load(p)
        nmax, dmax = int(z["nmax"]), int(z["dmax"])
        for jj, j in enumerate(z["js"]):
            phi[int(j)] = z["phi"][jj]
    assert sorted(phi) == list(range(LT // 2 + 1)), "missing theta indices"
    return phi, nmax, dmax


def delta_law(phi, N, k, idx):
    full = np.empty(LT, dtype=np.complex128)
    for j in range(LT // 2 + 1):
        full[j] = phi[j][N, k, idx]
    for j in range(LT // 2 + 1, LT):
        full[j] = np.conj(phi[LT - j][N, k, idx])
    return np.real(np.fft.fft(full)) / LT          # G[Delta mod LT]


def hit_weights_all(phi, nmax, dmax):
    """w[delta][(k, r)] for d = 2 delta, |delta| <= dmax, as in congruence_counts.hit_weights."""
    w = {}
    for idx, delta in enumerate(range(-dmax, dmax + 1)):
        d = 2 * delta
        wd = {}
        for N in range(1, nmax + 1):
            for k in range(1, N + 1):
                G = delta_law(phi, N, k, idx)
                r = N - k
                if r == 0:
                    tot = G.sum()
                else:
                    L = 2 * 3 ** (r - 1)
                    if LT % L == 0:
                        tot = sum(G[i] for i in range(LT) if (i - d) % L == 0)
                    else:
                        tot = G[d % LT]              # |A| - |B| = d up to aliasing at d +- 162
                wd[(k, r)] = 3 ** k * tot
        for N in range(0, nmax + 1):
            wd[(0, N)] = 1.0 if (N == 0 or delta % 3 ** N == 0) else 0.0
        w[delta] = wd
    return w


def rho(wd, k, r):
    return wd[(k, r)] * 2 * 3 ** (2 * r - 1) / wd[(k, 0)]


def merge(outdir, nmax_arg, files):
    phi, nmax, dmax = load(files)
    assert nmax == nmax_arg
    w = hit_weights_all(phi, nmax, dmax)
    K = w[0]
    H2 = {0: 1.0, **{j: K[(j, 0)] for j in range(1, nmax + 1)}}
    keys = list(K.keys())
    rep = open(os.path.join(outdir, "energy_renewal.txt"), "w")

    def P(s=""):
        print(s)
        rep.write(s + "\n")

    P(f"Run O, all d: N <= {nmax}, |d/2| <= {dmax}, LT = {LT}")
    P("H_2(N): " + " ".join(f"{H2[j]:.6f}" for j in range(1, nmax + 1)))
    sym = max(abs(w[dl][kk] - w[-dl][kk]) for dl in range(1, dmax + 1) for kk in keys)
    P(f"check w_d = w_(-d): max difference {sym:.1e}")
    # energy mixture
    wE = {kk: sum(4.0 ** (-abs(dl)) * w[dl][kk] for dl in range(-dmax, dmax + 1) if dl != 0) for kk in keys}
    chk = max(abs(wE[(k, 0)] - (H2[k + 1] - H2[k])) for k in range(1, nmax))
    P(f"check E_(k+1) = sum_d 2^-|d| Cor_k(g_d) = H_2(k+1) - H_2(k): max difference {chk:.1e}")
    nearid = max(abs(w[dl][(N, 0)] - H2[N]) for dl in range(1, dmax + 1) for N in range(1, min(v3int(dl), nmax) + 1)) \
        if any(v3int(dl) >= 1 for dl in range(1, dmax + 1)) else 0.0
    P(f"check Cor_N(g_d) = H_2(N) for N <= v_3(d/2): max difference {nearid:.1e}")
    FE = renewal(wE, K, H2, nmax)
    P("\nEnergy walk (start mu = sum_d 2^-|d| g_d):  E_(N+1) = F_E(N, 0) [regular] + sum_r F_E(N-r, r) H_2(r)")
    P("   N   E_(N+1)      regular     near-res.   min F_E")
    with open(os.path.join(outdir, "energy_walk.csv"), "w") as fo:
        fo.write("N,E_Nplus1,regular,near_resonance\n")
        for N in range(0, nmax + 1):
            reg = FE[(N, 0)]
            res = sum(FE[(N - r, r)] * H2[r] for r in range(1, N + 1))
            mn = min(FE[(N - r, r)] for r in range(0, N + 1))
            P(f"  {N:2d}  {wE[(N, 0)]:.8f}  {reg:.8f}  {res:.8f}  {mn:+.1e}")
            fo.write(f"{N},{wE[(N, 0)]:.12f},{reg:.12f},{res:.12f}\n")
    P("  rho_E(k, r), rows k = 0.., columns r = 1..:")
    for k in range(0, nmax):
        P(f"  k={k:2d}: " + " ".join(f"{rho(wE, k, r):7.4f}" for r in range(1, nmax - k + 1)))
    rstar = {r: max(rho(wE, k, r) for k in range(0, nmax - r + 1)) for r in range(1, nmax + 1)}
    cE = 1.5 * sum(rstar[r] * (5 / 27) ** r for r in rstar)
    FsE = max(FE[(N, 0)] for N in range(1, nmax + 1))
    P("  max_k rho_E(k, r): " + " ".join(f"{rstar[r]:.3f}" for r in range(1, nmax + 1)))
    P(f"  c_E = {cE:.4f} (computed range), F*_E = max_N F_E(N, 0) = {FsE:.5f}, "
      f"bound max(2/3, F*_E/(1-c_E)) = {max(2 / 3, FsE / (1 - cE)):.4f};  E_(N+1) at N = {nmax}: {wE[(nmax, 0)]:.5f}")
    # per d
    P("\nPer d = 2 * 3^s * m:  Cor_N(g_d) against H_2(s); bootstrap with k >= 1, k + r > s")
    P("    d  s   H_2(s)    Cor_N(g_d)   ratio   max_N ratio   F*_d      c_d     bound")
    worst = (0.0, None)
    with open(os.path.join(outdir, "per_d.csv"), "w") as fo:
        fo.write("d,s,H2s,cor_nmax,ratio_nmax,max_ratio,Fstar,c_d,bound\n")
        for dl in range(1, dmax + 1):
            s = v3int(dl)
            wd = w[dl]
            H2s = H2[min(s, nmax)]
            cor = wd[(nmax, 0)]
            rats = [wd[(N, 0)] / H2s for N in range(s + 1, nmax + 1)]
            mr = max(rats) if rats else float("nan")
            if rats and mr > worst[0]:
                worst = (mr, 2 * dl)
            if s >= nmax:
                continue
            FH = renewal(wd, K, H2, nmax)
            Fs = max(FH[(N, 0)] for N in range(s + 1, nmax + 1))
            rs = {}
            for r in range(1, nmax):
                ks = [k for k in range(1, nmax - r + 1) if k + r > s]
                rs[r] = max(rho(wd, k, r) for k in ks) if ks else 0.0
            cd = 1.5 * sum(rs[r] * (5 / 27) ** r for r in rs)
            bd = max(H2s, Fs / (1 - cd)) if cd < 1 else float("inf")
            P(f"  {2 * dl:3d}  {s}  {H2s:.5f}  {cor:.6f}  {cor / H2s:.4f}   {mr:.4f}      {Fs:.5f}  {cd:.4f}  {bd:.4f}")
            fo.write(f"{2 * dl},{s},{H2s:.10f},{cor:.10f},{cor / H2s:.8f},{mr:.8f},{Fs:.10f},{cd:.8f},{bd:.8f}\n")
    P(f"\nlargest ratio Cor_N(g_d) / H_2(v_3(d/2)) over N <= {nmax}, |d| <= {2 * dmax}: {worst[0]:.4f} (d = {worst[1]})")
    with open(os.path.join(outdir, "hit_weights_all.csv"), "w") as fo:
        fo.write("d,k,r,w\n")
        for dl in range(0, dmax + 1):
            for (k, r), v in sorted(w[dl].items()):
                fo.write(f"{2 * dl},{k},{r},{v:.12e}\n")
        for (k, r), v in sorted(wE.items()):
            fo.write(f"E,{k},{r},{v:.12e}\n")
    rep.close()


def scan(nmax, outdir):
    """Exact test of  Cor_N(g_d) <= H_2(v_3(d/2))  for ALL even d at once: Cor_N(g_d) = D_N(d/2) depends only on
    d/2 mod 3^N, and D_N(k) = 3^N P(L' - L = k mod 3^N) is the autocorrelation of the law of L_N = Lambda(S_N)."""
    os.makedirs(outdir, exist_ok=True)
    ch = Chain(nmax)
    lam = log_chart_table(nmax)
    H2 = {0: 1.0}
    rep = open(os.path.join(outdir, "scan.txt"), "w")

    def P(s=""):
        print(s, flush=True)
        rep.write(s + "\n")

    P("max and min over all k with v_3(k) = s of D_N(k) / H_2(s), D_N(k) = Cor_N(g_2k); argmax as d = 2k, |k| minimal")
    with open(os.path.join(outdir, "scan.csv"), "w") as fo:
        fo.write("N,s,max_ratio,argmax_d,min_ratio,mean_ratio,max_ratio_d_le_80\n")
        for N in range(1, nmax + 1):
            M = 3 ** N
            small_all = 0.0
            F = ch.law(N, N, 0.0).real
            G = np.empty(M)
            G[lam[:M] % M] = F
            Gh = np.fft.rfft(G)
            D = M * np.fft.irfft(Gh * np.conj(Gh), n=M)           # D[k] = 3^N sum_j G(j + k) G(j)
            H2[N] = D[0]
            line = []
            for s in range(0, N):
                m = np.arange(3 ** (N - s), dtype=np.int64)
                m = m[m % 3 != 0]
                ks = 3 ** s * m
                vals = D[ks] / H2[s]
                i = int(np.argmax(vals))
                kk = int(ks[i])
                kk = kk if kk <= M // 2 else kk - M
                kmin = np.minimum(ks, M - ks)
                small = vals[kmin <= 40]                            # |d| <= 80
                sm = float(small.max()) if small.size else float("nan")
                line.append(f"s={s}: {vals.max():.4f} (d={2 * kk})")
                fo.write(f"{N},{s},{vals.max():.10f},{2 * kk},{vals.min():.10f},{vals.mean():.10f},{sm:.10f}\n")
                small_all = max(small_all, sm) if small.size else small_all
            P(f"N={N:2d} H_2={H2[N]:.6f}  max over |d|<=80: {small_all:.4f}  " + "  ".join(line))
            del D, G, Gh, F
    rep.close()


def selftest():
    import tempfile
    nmax = 5
    ch = Chain(nmax)
    lam = log_chart_table(nmax)
    deltas = np.arange(-30, 31)
    err = 0.0
    for theta in (0.0, 7 / LT, 0.5):
        for k in (1, 3, 5):
            F = ch.law(nmax, k, theta)
            for N in range(nmax, k - 1, -1):
                F = reduce_mod(F, N)
                a = autocorr(F, lam, N, deltas)
                for i, dl in enumerate(deltas):
                    direct = complex(np.vdot(F[gd_index(2 * int(dl), N)], F)) if dl >= 0 else \
                        complex(np.vdot(F, F[gd_index(-2 * int(dl), N)]))
                    err = max(err, abs(a[i] - direct))
    print(f"FFT autocorrelation against direct sums over g_d: max difference {err:.1e}")
    ok = err < 1e-12
    tmp = tempfile.mkdtemp()
    p = run(4, tmp, 30, 0, 1)
    phi, n4, dmax = load([p])
    w = hit_weights_all(phi, n4, dmax)
    K = w[0]
    H2 = {0: 1.0, **{j: K[(j, 0)] for j in range(1, n4 + 1)}}
    for dl in (1, 3, 9, 27):
        FH = renewal(w[dl], K, H2, n4)
        for N in (1, 2, 3):
            amax = 13 if N < 3 else 9
            bf = brute_first_hits(2 * dl, N, amax)
            fh = [FH[(k, N - k)] for k in range(0, N + 1)]
            e2 = max(abs(x - y) for x, y in zip(bf, fh))
            tol = 2 * N * 2.0 ** (-amax) * 4
            print(f"d={2 * dl} N={N}: FFT {np.round(fh, 6)}  brute {np.round(bf, 6)}  diff {e2:.1e} (tol {tol:.1e})")
            ok &= e2 < tol and all(x <= y + 1e-12 for x, y in zip(bf, fh))
    print("selftest", "OK" if ok else "FAILED")


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd")
    a = sub.add_parser("run")
    a.add_argument("nmax", type=int)
    a.add_argument("outdir")
    a.add_argument("--dmax", type=int, default=40)
    a.add_argument("--theta-slice", default="0/1")
    b = sub.add_parser("merge")
    b.add_argument("outdir")
    b.add_argument("nmax", type=int)
    b.add_argument("files", nargs="+")
    c = sub.add_parser("scan")
    c.add_argument("nmax", type=int)
    c.add_argument("outdir")
    sub.add_parser("selftest")
    args = ap.parse_args()
    if args.cmd == "run":
        i, s = (int(x) for x in args.theta_slice.split("/"))
        run(args.nmax, args.outdir, args.dmax, i, s)
    elif args.cmd == "merge":
        merge(args.outdir, args.nmax, args.files)
    elif args.cmd == "scan":
        scan(args.nmax, args.outdir)
    elif args.cmd == "selftest":
        selftest()
    else:
        ap.print_help()


if __name__ == "__main__":
    main()
