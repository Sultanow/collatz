#!/usr/bin/env python3
"""Log chart of the 3-adic Syracuse law and the renormalisation walk (research notes, syracuse_lp).

Checks, exactly up to floating point, for n <= NMAX:
  1. the log chart  L(x) = log_4(1 + 3x)  maps the Syracuse law nu_n (law of L(S_n) mod 3^n) to the
     log-IFS  nu_n = (2/3) phi_1*(nu_{n-1} * gamma) + (1/3) phi_2*(nu_{n-1} * gamma),
     phi_c(z) = log_4(1 + 3 2^-c 4^z),  gamma = law of -j,  P(j) = (3/4) 4^-j  (j >= 0);
  2. the spectral energy identity  E_n = sum_eta kappa(eta / 3^{n-1}) |nu_hat_{n-1}(eta)|^2,
     kappa(x) = (5/3)|gamma_hat(x)|^2 - 1 = (8 cos 2 pi x - 2)/(17 - 8 cos 2 pi x);
  3. Q_l = E_{l+1} - E_l = sum over the unit frequencies omega mod 3^l of kappa(omega/3^l)|nu_hat_l(omega)|^2,
     split by the angle |x| = |omega|/3^l (energy shares against the Haar shares of the units);
  4. D_l(k) = 3^l P(L - L' = k mod 3^l) = R_l(g_{2k})  for k = 1..6;
  5. the extended singular set: for the Syracuse intertwiners h = Psi_B^-1 Psi_A (pairs of words of
     equal length k), R_n(h) >= sum over minimal pairs of 3^k 2^-(|A|+|B|) H_2(n-k); R_n is computed
     for intertwiners (among them the translation y -> y - 1/4) and for control maps.
usage: log_coordinates.py NMAX OUTDIR      (NMAX <= 15 needs about 3 GB)
"""
import math
import os
import sys
from fractions import Fraction
from itertools import product

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from syracuse_lp import discrete_log, next_law  # noqa: E402


def kappa(x):
    c = np.cos(2 * np.pi * x)
    return (8 * c - 2) / (17 - 8 * c)


def lam_table(n, dlog_next):
    """Lambda(x) = log_4(1 + 3x) mod 3^n for x mod 3^n, from the discrete log base 2 mod 3^{n+1}."""
    M = 3 ** n
    y = (1 + 3 * np.arange(M, dtype=np.int64)) % (3 * M)
    e = dlog_next[y]
    assert np.all(e % 2 == 0)
    return (e // 2) % M


def pow4_table(m, M):
    """4^u mod M for u = 0..m-1."""
    out = np.empty(m, dtype=np.int64)
    v = 1
    for i in range(m):
        out[i] = v
        v = (v * 4) % M
    return out


def smear(nu):
    """law of Z - j for Z ~ nu on Z/m, j geometric (3/4) 4^-j, wrapped."""
    m = nu.size
    s = np.arange(m, dtype=np.float64)
    g = 0.75 * np.exp2(-2 * s) / (1 - 4.0 ** (-m)) if m < 200 else 0.75 * np.exp2(-2 * np.minimum(s, 600))
    # P(U = u) = sum_j g(j) nu(u + j): circular cross-correlation
    return np.real(np.fft.ifft(np.fft.fft(nu) * np.conj(np.fft.fft(g))))


# ---------- Syracuse words ----------
def word_data(A):
    """(|A|, c_A) with Psi_A(x) = (2^|A| x - c_A)/3^k, Psi_A = psi_{a_k} o ... o psi_{a_1}, psi_a(x) = (2^a x - 1)/3."""
    c, s, k = 0, 0, 0
    for a in A:
        c = 2 ** a * c + 3 ** k
        s += a
        k += 1
    return s, c


def intertwiner(A, B):
    """Psi_B^-1 Psi_A as (e, t): y -> 2^e y + t with t rational."""
    sA, cA = word_data(A)
    sB, cB = word_data(B)
    return sA - sB, Fraction(cB - cA, 2 ** sB)


def minimal_pairs(e, t, kmax, letter_max):
    """All pairs (A, B) of equal length k <= kmax with Psi_B^-1 Psi_A = (e, t) and no proper prefix pair
    doing the same (first visit of the identity at step k)."""
    out = []
    for k in range(1, kmax + 1):
        table = {}
        for B in product(range(1, letter_max + 1), repeat=k):
            sB, cB = word_data(B)
            table.setdefault((sB, cB), []).append(B)
        for A in product(range(1, letter_max + 1), repeat=k):
            sA, cA = word_data(A)
            sB = sA - e
            if sB < k:
                continue
            cB = t * 2 ** sB + cA
            if cB.denominator != 1:
                continue
            for B in table.get((sB, int(cB)), []):
                if all(intertwiner(A[:i], B[:i]) != (e, t) for i in range(1, k)):
                    out.append((A, B))
    return out


def corr_affine(f, u, t, M):
    y = np.arange(M, dtype=np.int64)
    hy = (y * (u % M) + t) % M
    return float(np.dot(f, f[hy])) / M


def frac_mod(t, M):
    return (t.numerator * pow(t.denominator, -1, M)) % M


def main(nmax, outdir):
    os.makedirs(outdir, exist_ok=True)
    rep = open(os.path.join(outdir, "log_chart_summary.txt"), "w")

    def P(s=""):
        print(s)
        rep.write(s + "\n")

    laws, nus, H2 = {}, {}, {0: 1.0}
    D = np.array([1.0])
    for n in range(1, nmax + 1):
        D, _ = next_law(D, n, 1)
        laws[n] = D
        H2[n] = float(3 ** n * np.dot(D, D))
    nus[0] = np.array([1.0])
    for n in range(1, nmax + 1):
        dl = discrete_log(n + 1)
        lam = lam_table(n, dl)
        nus[n] = np.bincount(lam, weights=laws[n], minlength=3 ** n)
        del dl
    E = {n: H2[n] - H2[n - 1] for n in range(1, nmax + 1)}

    P("1. log-IFS check  max |nu_n - (2/3) phi_1*(nu_{n-1}*gamma) - (1/3) phi_2*(nu_{n-1}*gamma)|")
    for n in range(2, nmax + 1):
        M, m = 3 ** n, 3 ** (n - 1)
        dl = discrete_log(n + 1)
        lam = lam_table(n, dl)
        del dl
        U = smear(nus[n - 1])
        p4 = pow4_table(m, M)
        rhs = np.zeros(M)
        for c, pc in ((1, 2 / 3), (2, 1 / 3)):
            x = (p4 * pow(2, -c, M)) % M
            rhs += pc * np.bincount(lam[x], weights=U, minlength=M)
        P(f"  n={n:2d}: {np.abs(nus[n] - rhs).max():.2e}   (H_2 via log chart {M * np.dot(nus[n], nus[n]):.10f}, direct {H2[n]:.10f})")

    P("\n2. spectral identity  E_n = sum_eta kappa(eta/3^{n-1}) |nu_hat_{n-1}(eta)|^2")
    for n in range(1, nmax + 1):
        m = 3 ** (n - 1)
        F = np.fft.fft(nus[n - 1])
        val = float(np.sum(kappa(np.arange(m) / m) * np.abs(F) ** 2))
        P(f"  n={n:2d}: E_n = {E[n]:.10f}   spectral = {val:.10f}")

    P("\n3. Q_l = E_{l+1} - E_l by angle |x| = |omega|/3^l of the unit frequencies omega")
    edges = [0, 1 / 27, 1 / 9, 0.2, 1 / 3, 0.5000001]
    P("   bins: " + ", ".join(f"[{edges[i]:.3f},{min(edges[i + 1], .5):.3f})" for i in range(len(edges) - 1)))
    P("   l   E_l        Q_l         energy share / Haar share per bin            kappa-weighted part per bin")
    with open(os.path.join(outdir, "angular.csv"), "w") as fa:
        fa.write("l,E_l,Q_l," + ",".join(f"share_{i},haar_{i},kpart_{i}" for i in range(len(edges) - 1)) + "\n")
        for l in range(1, nmax + 1):
            M = 3 ** l
            F = np.fft.fft(nus[l])
            Pw = np.abs(F) ** 2
            w = np.arange(M)
            unit = (w % 3) != 0
            x = np.minimum(w, M - w) / M
            kq = kappa(x)
            El = float(Pw[unit].sum())
            Ql = float((kq * Pw)[unit].sum())
            sh, hs, kp = [], [], []
            for i in range(len(edges) - 1):
                b = unit & (x >= edges[i]) & (x < edges[i + 1])
                sh.append(float(Pw[b].sum()) / El)
                hs.append(float(b.sum()) / float(unit.sum()))
                kp.append(float((kq * Pw)[b].sum()))
            P(f"  {l:2d} {El:.6f} {Ql:+.4e}  " + " ".join(f"{s:.3f}/{h:.3f}" for s, h in zip(sh, hs)) +
              "   " + " ".join(f"{k:+.1e}" for k in kp))
            fa.write(f"{l},{El:.10e},{Ql:.10e}," + ",".join(f"{s:.6e},{h:.6e},{k:.6e}" for s, h, k in zip(sh, hs, kp)) + "\n")
            if l < nmax:
                assert abs(Ql - (E[l + 1] - E[l])) < 1e-9, (l, Ql, E[l + 1] - E[l])

    P("\n4. D_l(k) = 3^l P(L - L' = k)  (= R_l(g_2k))")
    with open(os.path.join(outdir, "D_lk.csv"), "w") as fd:
        fd.write("l,k,D\n")
        for l in range(1, nmax + 1):
            M = 3 ** l
            Pw = np.abs(np.fft.fft(nus[l])) ** 2
            Dl = np.real(np.fft.ifft(Pw)) * M       # 3^l P(L - L' = k)
            row = []
            for k in range(1, 7):
                fd.write(f"{l},{k},{Dl[k % M]:.10f}\n")
                row.append(Dl[k % M])
            P(f"  l={l:2d}: " + " ".join(f"{v:.6f}" for v in row))

    P("\n5. extended singular set: R_n(h) for intertwiners h = Psi_B^-1 Psi_A and controls")
    targets = [("intertwiner A=(1,2), B=(2,1)",) + intertwiner((1, 2), (2, 1)),
               ("intertwiner A=(2,1), B=(1,2)",) + intertwiner((2, 1), (1, 2)),
               ("intertwiner A=(1,3), B=(2,1)",) + intertwiner((1, 3), (2, 1)),
               ("intertwiner A=(1,3), B=(3,1)",) + intertwiner((1, 3), (3, 1)),
               ("intertwiner A=(2,3), B=(3,2)",) + intertwiner((2, 3), (3, 2)),
               ("intertwiner A=(1,1,2), B=(1,2,1)",) + intertwiner((1, 1, 2), (1, 2, 1)),
               ("control", 0, Fraction(1)),
               ("control", 0, Fraction(-1, 5)),
               ("control (fixes -1)", 1, Fraction(1)),
               ("control g_2", 2, Fraction(1))]
    with open(os.path.join(outdir, "singular_ext.csv"), "w") as fs:
        fs.write("map,e,t,n,R,lower_bound\n")
        for name, e, t in targets:
            pairs = minimal_pairs(e, t, 3, 7)
            wts = [(len(A), 3 ** len(A) * 2.0 ** (-sum(A) - sum(B))) for A, B in pairs]
            P(f"  {name}: h(y) = 2^{e} y + ({t}); minimal pairs (k<=3, letters<=7): {len(pairs)}"
              + (f", e.g. {pairs[0]}" if pairs else "") + f", total weight {sum(w for _, w in wts):.4f}")
            line = []
            for n in range(2, nmax + 1):
                M = 3 ** n
                f = laws[n] * M
                R = corr_affine(f, pow(2, e, M), frac_mod(t, M), M)
                lb = sum(w * H2[n - k] for k, w in wts if k <= n)
                fs.write(f"\"{name}\",{e},{t},{n},{R:.10f},{lb:.10f}\n")
                line.append(f"{R:.3f}({lb:.3f})")
            P("     R_n (lower bound), n=2..: " + " ".join(line))
    P("\n   H_2(n): " + " ".join(f"{H2[n]:.4f}" for n in range(1, nmax + 1)))

    P("\n6. non-resonance: minimal pairs (k <= 4, letters <= 8) representing g_d and integer translations")
    for d in range(2, 21, 2):
        pairs = minimal_pairs(d, Fraction(2 ** d - 1, 3), 4, 8)
        P(f"  g_{d}: {len(pairs)} pairs")
    for t in (Fraction(1), Fraction(2), Fraction(-1), Fraction(1, 2), Fraction(-1, 5)):
        pairs = minimal_pairs(0, t, 4, 8)
        P(f"  y + ({t}): {len(pairs)} pairs" + (f", e.g. {pairs[0]}" if pairs else ""))

    P("\n7. the fixed point -1 of x -> (3x+1)/2 and the dilation identity R_n(2y+1) = 2 R_n(4y+1)")
    for n in range(1, nmax + 1):
        M = 3 ** n
        f = laws[n] * M
        r1 = corr_affine(f, 2, 1, M)
        r2 = corr_affine(f, 4, 1, M)
        P(f"  n={n:2d}: f_n(-1) (2/3)^n = {f[M - 1] * (2 / 3) ** n:.6f}   max f_n = {f.max():9.3f} at "
          f"{int(np.argmax(f))} (= -1: {int(np.argmax(f)) == M - 1})   R(2y+1) - 2 R(4y+1) = {r1 - 2 * r2:+.1e}")
    rep.close()


if __name__ == "__main__":
    main(int(sys.argv[1]), sys.argv[2])
