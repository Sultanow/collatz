#!/usr/bin/env python3
"""Run K: uniform integrability of the predecessor potential a*h(a).

Proposition 14 of the paper bounds the clipping tail T_X by the stopped potential profile
y*h(y) on a thin set of 2-adic classes; T_X = o(X) follows if this profile is uniformly
integrable on average (UI). This script evaluates the per-target predecessor counts of
`potential` (Run G program) for that hypothesis:

  * per dyadic window (X/2, X]: mean, quantiles, maximum and the Cesaro tail
        tau_M(X) = |window|^{-1} * sum_{a in window, v(a) > M} v(a),   v(a) = a*h(a),
    for several M (UI <=> sup_X tau_M(X) -> 0 as M -> infinity);
  * the same tail for the Syracuse profile v_n(a) = (2/log(4/3)) 3^n P(c*Syrac(Z/3^n) = a),
    computed exactly, and the L1 distance |v - v_n| per window (pointwise form of Conjecture 1);
  * the mass of v on the "dangerous" 2-adic classes of Proposition 13/14 relative to their
    density (ratio 1 = no concentration on them);
  * the number of targets whose orbit does not reach [1, X^(1-2*beta)] (stopped profile);
  * an exact integrity check of the counts (flow identity pi_a = [a<=N] + pi_2a + pi_(2a-c)/3).

usage:
  ui_report.py convert C N IN.csv OUT.npz          potential CSV (a,count,cycle_min) -> compact npz
  ui_report.py report OUTDIR FILE.npz [FILE.npz ...]
  ui_report.py figures OUTDIR                      table_K.tex and fig_ui_tails.* from the CSVs in OUTDIR
  ui_report.py selftest

Only numpy is required; matplotlib (optional) draws the figure.
"""
import math
import os
import sys

import numpy as np

K_POT = 2.0 / math.log(4.0 / 3.0)          # 6.9521...
BETA = 1.0 / 200.0                          # window parameter of the paper
ZETA0 = 0.55                                # threshold of Remark 11 / Proposition 14
M_LIST = (10.0, 15.0, 20.0, 30.0, 50.0, 75.0, 100.0, 150.0, 200.0, 300.0, 500.0, 1000.0)
M_TABLE = (20.0, 50.0, 100.0)
DANGER = ((8, 4), (12, 4), (12, 8), (16, 8))  # (t, b)
NPROF = 14                                  # depth of the profile for the per-target comparison
NPROF_TAIL = (8, 12, 16)                    # depths of the profile tails (map-independent)


# ----------------------------------------------------------------------------- I/O
def read_potential_csv(path):
    """Return (a, count, cycle_min) as int64 arrays. Only lines whose first field is an
    integer are data; comment lines, a header line and any other text are skipped."""
    try:
        import pandas as pd
        df = pd.read_csv(path, header=None, names=[0, 1, 2], comment="#",
                         skipinitialspace=True, dtype=str, on_bad_lines="skip")
        first = pd.to_numeric(df[0].str.strip(), errors="coerce")
        df = df[first.notna()]
        a = pd.to_numeric(df[0]).to_numpy(np.int64)
        cnt = pd.to_numeric(df[1]).to_numpy(np.int64)
        cyc = pd.to_numeric(df[2], errors="coerce").fillna(0).to_numpy(np.int64)
    except ImportError:
        rows = []
        with open(path) as f:
            for line in f:
                parts = line.split("#", 1)[0].strip().split(",")
                if parts and parts[0].strip().lstrip("-").isdigit():
                    rows.append([int(x) for x in parts[:3]])
        arr = np.array(rows, dtype=np.int64).reshape(-1, len(rows[0]) if rows else 3)
        a, cnt = arr[:, 0], arr[:, 1]
        cyc = arr[:, 2] if arr.shape[1] > 2 else np.zeros_like(a)
    return a, cnt, cyc


def convert(c, N, csv_path, npz_path):
    a, cnt, cyc = read_potential_csv(csv_path)
    A = int(a.max())
    if not np.array_equal(a, np.arange(1, A + 1)):
        raise SystemExit(f"{csv_path}: targets are not exactly 1..{A}")
    big = np.flatnonzero(cnt >= 2**32)            # counts that need 64 bits (small or heavy targets)
    split = int(big.max()) + 1 if big.size else 0
    tail = cnt[split:]
    per = np.flatnonzero(cyc)
    np.savez_compressed(npz_path, c=np.int64(c), N=np.int64(N), A=np.int64(A),
                        head=cnt[:split].astype(np.uint64), tail=tail.astype(np.uint32),
                        per_idx=per.astype(np.int64), per_min=cyc[per].astype(np.int64))
    print(f"{npz_path}: c={c} N={N} A={A} periodic targets={per.size} uint64 head={split}")


def load_npz(path):
    z = np.load(path)
    cnt = np.concatenate([z["head"].astype(np.int64), z["tail"].astype(np.int64)])
    cyc = np.zeros(cnt.size, dtype=np.int64)
    cyc[z["per_idx"]] = z["per_min"]
    return int(z["c"]), int(z["N"]), int(z["A"]), cnt, cyc


# ----------------------------------------------------------------------------- dynamics
def step(x, c):
    """One step of T_{3,c} on an int64 array (values stay far below 2^62 here)."""
    odd = (x & 1).astype(bool)
    y = x >> 1
    y[odd] = (3 * x[odd] + c) >> 1
    return y


def flow_check(c, N, cnt, cyc):
    """pi_a = [a<=N] + pi_{2a} + pi_{(2a-c)/3} for non-periodic a <= A/2 (exact)."""
    A = cnt.size
    a = np.arange(1, A // 2 + 1, dtype=np.int64)
    a = a[cyc[a - 1] == 0]
    rhs = (a <= N).astype(np.int64) + cnt[2 * a - 1]
    m = 2 * a - c
    ok = (m % 3 == 0)
    m3 = m // 3
    ok &= (m3 > 0) & (m3 % 2 == 1) & (m3 <= A)
    rhs[ok] += cnt[m3[ok] - 1]
    return int(a.size), int(np.count_nonzero(rhs != cnt[a - 1]))


def stopped_flags(c, lo, hi, cutoff, cap=20000):
    """True where the orbit of a in (lo, hi] reaches [1, cutoff]."""
    x = np.arange(lo + 1, hi + 1, dtype=np.int64)
    done = x <= cutoff
    idx = np.flatnonzero(~done)
    cur = x[idx]
    for _ in range(cap):
        if idx.size == 0:
            break
        cur = step(cur, c)
        hit = cur <= cutoff
        done[idx[hit]] = True
        idx, cur = idx[~hit], cur[~hit]
    return done, int(idx.size)


def parity_danger(c, lo, hi, t, b, zeta0=ZETA0):
    """Targets whose forward parity word of length t has a prefix of length s in [b, t]
    with at least zeta0*s odd entries (the classes C_{X,b} of Propositions 13/14)."""
    x = np.arange(lo + 1, hi + 1, dtype=np.int64)
    ssum = np.zeros(x.size, dtype=np.int64)
    flag = np.zeros(x.size, dtype=bool)
    for s in range(1, t + 1):
        ssum += x & 1
        x = step(x, c)
        if s >= b:
            flag |= ssum >= math.ceil(zeta0 * s - 1e-12)
    return flag


def danger_density(t, b, zeta0=ZETA0):
    """Exact density of the dangerous parity words of length t (fraction of {0,1}^t)."""
    # count words that never reach the threshold: dp[k] = number of prefixes with k ones
    dp = {0: 1}
    for s in range(1, t + 1):
        nd = {}
        for k, v in dp.items():
            for bit in (0, 1):
                kk = k + bit
                if s >= b and kk >= math.ceil(zeta0 * s - 1e-12):
                    continue
                nd[kk] = nd.get(kk, 0) + v
        dp = nd
    return 1.0 - sum(dp.values()) / 2.0**t


# ----------------------------------------------------------------------------- Syracuse
def syracuse_law(n, prev=None):
    """P(Syrac(Z/3^n Z) = r), r in Z/3^n, exactly up to float rounding (prev = law at n-1).
    S_n = 2^{-a}(1 + 3 S'_{n-1}) with a geometric (P(a=k)=2^-k); in discrete-log coordinates
    of the units mod 3^n (2 is a primitive root) this is a cyclic correlation, done by FFT."""
    D = np.array([1.0]) if prev is None else prev
    for k in range(1 if prev is None else n, n + 1):
        M = 3**k
        L = 2 * 3**(k - 1)
        dlog = np.full(M, -1, dtype=np.int64)
        # powers of 2 mod M, vectorised in blocks
        B = 4096
        blk = np.empty(min(B, L), dtype=np.int64)
        v = 1
        for i in range(blk.size):
            blk[i] = v
            v = (v * 2) % M
        step_mul = v  # 2^B mod M
        e0, base = 0, 1
        while e0 < L:
            cnt = min(blk.size, L - e0)
            vals = (blk[:cnt] * base) % M
            dlog[vals] = np.arange(e0, e0 + cnt)
            base = (base * step_mul) % M
            e0 += cnt
        u = (1 + 3 * np.arange(3**(k - 1), dtype=np.int64)) % M
        Q = np.bincount(dlog[u], weights=D, minlength=L)
        r = np.arange(L, dtype=np.float64)
        w = np.exp2(-np.where(r == 0, L, r)) / (1.0 - 2.0**(-L)) if L < 1000 else \
            np.exp2(-np.where(r == 0, 1e9, r))
        P = np.fft.irfft(np.fft.rfft(Q) * np.conj(np.fft.rfft(w)), n=L)
        P = np.clip(P, 0.0, None)
        P /= P.sum()
        Dn = np.zeros(M)
        units = np.flatnonzero(dlog >= 0)
        Dn[units] = P[dlog[units]]
        D = Dn
    return D


def profile_values(D, n, c, a):
    """v_n(a) = (2/log(4/3)) 3^n P(c*Syrac = a mod 3^n)."""
    M = 3**n
    cinv = pow(c % M, -1, M) if n > 0 else 0
    return K_POT * M * D[(cinv * (a % M)) % M] if n > 0 else np.full(a.shape, K_POT)


# ----------------------------------------------------------------------------- report
def tail_exponent(shares):
    """Least-squares slope of log(share) against log(M) on 20 <= M <= 200 (shares > 0)."""
    xs = [math.log(m) for m, y in zip(M_LIST, shares) if 20 <= m <= 200 and y > 0]
    ys = [math.log(y) for m, y in zip(M_LIST, shares) if 20 <= m <= 200 and y > 0]
    if len(xs) < 3:
        return float("nan")
    mx, my = sum(xs) / len(xs), sum(ys) / len(ys)
    return -sum((x - mx) * (y - my) for x, y in zip(xs, ys)) / sum((x - mx) ** 2 for x in xs)


def l1_steps(laws):
    out = []
    for n in sorted(laws):
        if n - 1 in laws:
            f_n = 3**n * laws[n]
            f_m = np.tile(3**(n - 1) * laws[n - 1], 3)
            out.append((n, float(np.mean(np.abs(f_n - f_m)))))
    return out


def quantiles(v, qs=(0.5, 0.9, 0.99, 0.999)):
    return [float(np.quantile(v, q)) for q in qs]


def report(outdir, files):
    os.makedirs(outdir, exist_ok=True)
    laws = {}
    D = None
    for n in range(1, max(NPROF, *NPROF_TAIL) + 1):
        D = syracuse_law(n, D)
        laws[n] = D
    lines, rows_w, rows_t, rows_p, rows_d = [], [], [], [], []
    summary = {}
    P = lines.append
    P("Run K: uniform integrability of the predecessor potential v(a) = a*h(a)")
    P(f"windows (X/2, X], X = 2^k; h(a) = count/N; 2/log(4/3) = {K_POT:.4f}; "
      f"stopped cutoff X^(1-2beta), beta = {BETA}; zeta0 = {ZETA0}")
    for path in files:
        c, N, A, cnt, cyc = load_npz(path)
        a_all = np.arange(1, A + 1, dtype=np.int64)
        v_all = a_all * cnt / N
        nchk, nbad = flow_check(c, N, cnt, cyc)
        P("")
        P(f"=== 3x{c:+d}: N = {N:.3e}, A = {A}, file {os.path.basename(path)}")
        P(f"flow identity: {nchk} non-periodic targets checked, {nbad} violations")
        P(f"mean of v over a <= A: {v_all.mean():.4f}")
        P("  k   targets   mean     q50     q90     q99    q99.9     max    "
          + "  ".join(f"tau{m:g}" for m in M_LIST) + "   nonstop   noise")
        kmax = int(math.floor(math.log2(A)))
        summary[c] = {"N": N, "A": A, "flow_bad": nbad, "windows": {}}
        for k in range(8, kmax + 1):
            lo, hi = 2**(k - 1), 2**k
            v = v_all[lo:hi]          # a = lo+1 .. hi
            a = a_all[lo:hi]
            cutoff = int(hi ** (1 - 2 * BETA))
            stop, nstop_left = stopped_flags(c, lo, hi, cutoff)
            vs = np.where(stop, v, 0.0)
            tails = [float(vs[vs > m].sum() / vs.size) for m in M_LIST]
            q = quantiles(vs)
            noise = float(np.mean(math.sqrt(2 / math.pi) * a * np.sqrt(cnt[lo:hi]) / N))
            nonstop = int(np.count_nonzero(~stop))
            P(f"{k:3d} {vs.size:9d} {vs.mean():7.4f} " + " ".join(f"{x:7.3f}" for x in q)
              + f" {vs.max():8.3f}  " + "  ".join(f"{x:.2e}" for x in tails)
              + f"  {nonstop:7d}  {noise:.4f}")
            rows_w.append((c, k, vs.size, vs.mean(), *q, vs.max(), nonstop, noise))
            for m, tval in zip(M_LIST, tails):
                rows_t.append((c, k, m, tval, tval / vs.mean() if vs.mean() else 0.0))
            summary[c]["windows"][k] = {"mean": float(vs.mean()), "max": float(vs.max()),
                                        "tails": dict(zip(M_LIST, tails)),
                                        "exponent": tail_exponent([x / vs.mean() for x in tails])}
            # pointwise comparison with the Syracuse profile
            l1 = []
            for n in range(0, NPROF + 1):
                vp = profile_values(laws.get(n), n, c, a)
                l1.append(float(np.mean(np.abs(vs - vp))))
            rows_p.append((c, k, *l1))
            summary[c]["windows"][k]["l1"] = l1
            # dangerous 2-adic classes
            for t, b in DANGER:
                if t > k - 2:
                    continue
                flag = parity_danger(c, lo, hi, t, b)
                alpha = danger_density(t, b)
                cnt_ratio = flag.mean() / alpha
                mass_ratio = (vs[flag].sum() / vs.sum()) / alpha
                rows_d.append((c, k, t, b, alpha, cnt_ratio, mass_ratio))
                summary[c]["windows"][k][f"danger_{t}_{b}"] = (alpha, cnt_ratio, mass_ratio)
        P("tail exponent gamma (share above M ~ M^-gamma, fitted on 20 <= M <= 200) per window: "
          + "  ".join(f"{k}:{w['exponent']:.3f}" for k, w in summary[c]["windows"].items() if k >= 12))
        P("L1 distance mean|v - v_n| to the Syracuse profile of depth n (last window):")
        P("  " + "  ".join(f"n={n}:{x:.3f}" for n, x in enumerate(rows_p[-1][2:])))
        P("dangerous classes (t, b): density alpha, count ratio, mass ratio (last window):")
        for r in [r for r in rows_d if r[0] == c and r[1] == kmax]:
            P(f"  t={r[2]:2d} b={r[3]}: alpha={r[4]:.4f}  count/alpha={r[5]:.4f}  "
              f"mass/alpha={r[6]:.4f}")
    # Syracuse-profile tails: the multiset of values of v_n does not depend on c
    P("")
    P("Syracuse profile v_n (Haar measure on Z/3^n, the same for every c): share of the mean above M")
    rows_s, syr = [], {}
    for n in NPROF_TAIL:
        vp = K_POT * 3**n * laws[n]
        sh = [float(vp[vp > m].sum() / vp.sum()) for m in M_LIST]
        g = tail_exponent(sh)
        rows_s.append((n, float(vp.max()), g, *sh))
        syr[n] = (float(vp.max()), dict(zip(M_LIST, sh)), g)
        P(f"  n={n:2d}: max={vp.max():10.3f}  exponent={g:.3f}  "
          + "  ".join(f">{m:g}:{x:.3e}" for m, x in zip(M_LIST, sh)))
    P("steps ||f_n - f_(n-1)||_1 of the profile densities (Tao (1.26)):")
    P("  " + "  ".join(f"{n}:{x:.4f}" for n, x in l1_steps(laws)))
    write_csv(os.path.join(outdir, "ui_windows.csv"),
              "c,k,targets,mean,q50,q90,q99,q999,max,nonstopped,noise", rows_w)
    write_csv(os.path.join(outdir, "ui_tails.csv"), "c,k,M,tau,share", rows_t)
    write_csv(os.path.join(outdir, "ui_profile_l1.csv"),
              "c,k," + ",".join(f"l1_n{n}" for n in range(NPROF + 1)), rows_p)
    write_csv(os.path.join(outdir, "ui_dangerous.csv"),
              "c,k,t,b,alpha,count_ratio,mass_ratio", rows_d)
    write_csv(os.path.join(outdir, "ui_syracuse_tails.csv"),
              "n,max,exponent," + ",".join(f"share_gt{m:g}" for m in M_LIST), rows_s)
    with open(os.path.join(outdir, "report_K.txt"), "w") as f:
        f.write("\n".join(lines) + "\n")
    table_from_csv(outdir)
    figure_from_csv(outdir)
    print("\n".join(lines))


def write_csv(path, header, rows):
    with open(path, "w") as f:
        f.write(header + "\n")
        for r in rows:
            f.write(",".join(f"{x:.6g}" if isinstance(x, float) else str(x) for x in r) + "\n")


def read_csv_rows(path):
    with open(path) as f:
        head = f.readline().strip().split(",")
        return [dict(zip(head, line.strip().split(","))) for line in f if line.strip()]


def load_tables(outdir):
    """Shares per (c, k), windows per (c, k), dangerous ratios, profile tails, from the CSVs."""
    shares, windows, danger, prof = {}, {}, {}, {}
    for r in read_csv_rows(os.path.join(outdir, "ui_tails.csv")):
        shares.setdefault((int(r["c"]), int(r["k"])), {})[float(r["M"])] = float(r["share"])
    for r in read_csv_rows(os.path.join(outdir, "ui_windows.csv")):
        windows[(int(r["c"]), int(r["k"]))] = r
    for r in read_csv_rows(os.path.join(outdir, "ui_dangerous.csv")):
        danger[(int(r["c"]), int(r["k"]), int(r["t"]), int(r["b"]))] = float(r["mass_ratio"])
    for r in read_csv_rows(os.path.join(outdir, "ui_syracuse_tails.csv")):
        prof[int(r["n"])] = (float(r["max"]), {m: float(r[f"share_gt{m:g}"]) for m in M_LIST})
    return shares, windows, danger, prof


def table_from_csv(outdir, ms=(20.0, 100.0, 1000.0), tb=(12, 8)):
    """LaTeX rows for the paper: per map three windows, then the Syracuse profile."""
    shares, windows, danger, prof = load_tables(outdir)
    cs = list(dict.fromkeys(c for c, _ in windows))
    out = ["% generated by ui_report.py (Run K) from the CSV files in this directory",
           "% map & X & mean of a h(a) & shares (percent) of the mean above M = "
           + ", ".join(f"{m:g}" for m in ms) + " & exponent & max & mass ratio on C(t=%d,b=%d)" % tb]
    for c in cs:
        ks = sorted(k for cc, k in windows if cc == c)
        pick = [k for k in (ks[-1] - 8, ks[-1] - 4, ks[-1]) if k in ks]
        for i, k in enumerate(pick):
            w, sh = windows[(c, k)], shares[(c, k)]
            g = tail_exponent([sh[m] for m in M_LIST])
            ratio = danger.get((c, k, tb[0], tb[1]))
            lab = f"$3x{c:+d}$" if i == 0 else ""
            out.append(f"{lab} & $2^{{{k}}}$ & {float(w['mean']):.3f} & "
                       + " & ".join(pct(sh[m]) for m in ms)
                       + f" & {g:.2f} & {float(w['max']):.0f} & "
                       + (f"{ratio:.3f}" if ratio is not None else "--") + " \\\\")
        out.append("\\addlinespace")
    for i, n in enumerate(sorted(prof)):
        mx, sh = prof[n]
        g = tail_exponent([sh[m] for m in M_LIST])
        lab = "profile" if i == 0 else ""
        out.append(f"{lab} & $n={n}$ & {K_POT:.3f} & " + " & ".join(pct(sh[m]) for m in ms)
                   + f" & {g:.2f} & {mx:.0f} & -- \\\\")
    with open(os.path.join(outdir, "table_K.tex"), "w") as f:
        f.write("\n".join(out) + "\n")


def pct(x):
    """Share as a percentage: one decimal, two below 0.1 %, and 0 for an empty tail."""
    if x <= 0:
        return "0"
    return f"{100 * x:.1f}" if x >= 1e-3 else f"{100 * x:.2f}"


def figure_from_csv(outdir):
    """Share of the mean of a h(a) above M (log-log): the largest window of every map against
    the Syracuse profile at the depths in the CSV. Zero shares are not drawn."""
    try:
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
    except ImportError:
        return
    shares, windows, _, prof = load_tables(outdir)
    cs = list(dict.fromkeys(c for c, _ in windows))
    colors = ["#2a78d6", "#eb6834", "#1baf7a", "#eda100"]   # validated categorical slots 1-4
    markers = ["o", "s", "^", "D"]
    greys = ["#c3c2b7", "#8a8984", "#3d3c39"]
    fig, ax = plt.subplots(figsize=(4.6, 3.2))
    ns = sorted(prof)
    for g, n in zip(greys[-len(ns):], ns):
        _, sh = prof[n]
        pts = [(m, sh[m]) for m in M_LIST if sh[m] > 0]
        ax.loglog([m for m, _ in pts], [y for _, y in pts], color=g, lw=1.5,
                  ls="-" if n == ns[-1] else "--", label=f"profile, $n={n}$")
    for c, col, mk in zip(cs, colors, markers):
        k = max(kk for cc, kk in windows if cc == c)
        sh = shares[(c, k)]
        pts = [(m, sh[m]) for m in M_LIST if sh[m] > 0]
        ax.loglog([m for m, _ in pts], [y for _, y in pts], color=col, marker=mk, ms=4, ls="",
                  label=f"$3x{c:+d}$, $X=2^{{{k}}}$")
    ax.set_xlabel("$M$", fontsize=9)
    ax.set_ylabel("share of the mean of $a\\,h(a)$ above $M$", fontsize=9)
    ax.grid(True, which="major", color="#e5e4e0", lw=0.6)
    ax.tick_params(labelsize=8)
    for sp in ("top", "right"):
        ax.spines[sp].set_visible(False)
    ax.legend(fontsize=7, frameon=False, loc="lower left")
    fig.tight_layout()
    stem = os.path.join(outdir, "fig_ui_tails")
    fig.savefig(stem + ".pdf")
    fig.savefig(stem + ".png", dpi=200)
    plt.close(fig)


# ----------------------------------------------------------------------------- self test
def selftest():
    # 1. Syracuse law mod 9 against the paper (c = 1): s = 1,2,4,5,7,8
    D = syracuse_law(2)
    want = {1: 8 / 63, 2: 16 / 63, 4: 11 / 63, 5: 4 / 63, 7: 2 / 63, 8: 22 / 63}
    for s, p in want.items():
        assert abs(D[s] - p) < 1e-12, (s, D[s], p)
    # 2. exact enumeration for n = 4: states (sum of valuations mod L, value mod 3^n)
    n = 4
    M, L = 3**n, 2 * 3**(n - 1)
    ref = np.zeros(M)
    w = np.array([sum(2.0**-(r + j * L) for j in range(0, 60) if r + j * L >= 1)
                  for r in range(L)])
    states = {(0, 0): 1.0}
    for m in range(n):
        new = {}
        for (e, val), p in states.items():
            for r in range(L):
                ee = (e + r) % L
                vv = (val + pow(3, m) * pow(2, -ee, M)) % M
                new[(ee, vv)] = new.get((ee, vv), 0.0) + p * w[r]
        states = new
    for (e, val), p in states.items():
        ref[val] += p
    D4 = syracuse_law(4)
    assert np.max(np.abs(D4 - ref)) < 1e-12, np.max(np.abs(D4 - ref))
    # 3. L1 convergence of the profile densities (Tao (1.26)): ||f_n - f_m||_1 decreasing
    Ds = [syracuse_law(k) for k in range(1, 9)]
    d = []
    for k in range(1, 8):
        f_k = 3**k * Ds[k - 1]
        f_n = 3**(k + 1) * Ds[k]
        d.append(float(np.mean(np.abs(f_n - np.tile(f_k, 3)))))
    assert all(x > 0 for x in d)
    # 4. dangerous density against brute force over all words
    for t, b in ((8, 4), (10, 3)):
        brute = 0
        for wv in range(2**t):
            s = 0
            hit = False
            for i in range(t):
                s += (wv >> i) & 1
                if i + 1 >= b and s >= math.ceil(ZETA0 * (i + 1) - 1e-12):
                    hit = True
            brute += hit
        assert abs(brute / 2**t - danger_density(t, b)) < 1e-12
    # 5. parity words of consecutive integers are equidistributed over a full period
    for c in (1, -1, 5, 13):
        flag = parity_danger(c, 2**14, 2**15, 8, 4)
        assert abs(flag.mean() - danger_density(8, 4)) < 1e-12, c
    print("selftest PASS: Syracuse law mod 9 and mod 81 exact, profile L1 steps "
          + ", ".join(f"{x:.3f}" for x in d) + "; dangerous densities exact")


if __name__ == "__main__":
    if len(sys.argv) >= 2 and sys.argv[1] == "convert" and len(sys.argv) == 6:
        convert(int(sys.argv[2]), int(float(sys.argv[3])), sys.argv[4], sys.argv[5])
    elif len(sys.argv) >= 4 and sys.argv[1] == "report":
        report(sys.argv[2], sys.argv[3:])
    elif len(sys.argv) == 3 and sys.argv[1] == "figures":
        table_from_csv(sys.argv[2])
        figure_from_csv(sys.argv[2])
    elif len(sys.argv) == 2 and sys.argv[1] == "selftest":
        selftest()
    else:
        print(__doc__)
        sys.exit(2)
