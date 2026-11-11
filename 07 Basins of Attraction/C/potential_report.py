#!/usr/bin/env python3
"""Check the identities of the predecessor potential on the output of potential.c and
summarise x*h(x).

Usage:  potential_report.py potential_c*.csv [...]  > report.txt

For every file (one map, one N) the script verifies, as exact integer identities,
  (1) the flow identity  pi_a = [a<=N] + pi_{2a} + pi_{(2a-c)/3}   for all non-periodic a <= A/2,
  (2) pi_a = #{2^i a <= N}  for all a divisible by 3 (if 3 does not divide c),
  (3) the level identity  #basin = #(C in [1,N]) + sum of pi_x over the entry points of C,
and prints the basin shares, the mean value of a*h(a) = a*pi_a(N)/N, and its means over the
residue classes modulo 3, 9, 27 and 81 against the law of Tao's Syracuse random variable:
(2/log(4/3)) * 3^n * P(c * Syrac(Z/3^n) = r).
"""
import sys, re, math
from statistics import mean, median

def T(n, c): return n // 2 if n % 2 == 0 else (3 * n + c) // 2

def syracuse_law(n, amax=80):
    """Law of Tao's Syracuse random variable modulo 3^n, by the recursion
    Syrac(Z/3^(k+1)) = (3*Syrac(Z/3^k) + 1) * 2^(-Geom(2))  (Tao 2022, Lemma 1.12)."""
    p = [1.0]
    for k in range(n):
        mod = 3 ** (k + 1); q = [0.0] * mod
        for s in range(mod):
            if s % 3 == 0: continue
            pw = 1
            for a in range(1, amax + 1):
                pw = (pw * 2) % mod
                t = (pw * s) % mod
                if t % 3 == 1: q[s] += 0.5 ** a * p[(t - 1) // 3]
        p = q
    return p

def report(path):
    with open(path) as fh:
        head = fh.readline()
        m = re.search(r"c=(-?\d+) N=(\d+) A=(\d+) roots=(\d+) unresolved=(\d+)", head)
        c, N, A, roots, unres = map(int, m.groups())
        fh.readline()
        cnt = [0] * (A + 1); cyc = [0] * (A + 1)
        for line in fh:
            a, k, mn = line.split(",")
            cnt[int(a)] = int(k); cyc[int(a)] = int(mn)
    print(f"== {path}: c={c}, N={N:.0e}, targets a <= {A}, roots={roots}, unresolved={unres}")
    # (1) flow identity
    bad = tested = 0
    for a in range(1, A // 2 + 1):
        if cyc[a]: continue
        rhs = (1 if a <= N else 0) + cnt[2 * a]
        if (2 * a - c) % 3 == 0 and (2 * a - c) // 3 > 0: rhs += cnt[(2 * a - c) // 3]
        tested += 1; bad += (cnt[a] != rhs)
    print(f"  flow identity: {tested} targets, {bad} violations")
    # (2) multiples of 3
    if c % 3:
        bad = tested = 0
        for a in range(3, A + 1, 3):
            k = 0; x = a
            while x <= N: k += 1; x *= 2
            tested += 1; bad += (cnt[a] != k)
        print(f"  targets divisible by 3 (doubling chains only): {tested} targets, {bad} violations")
    # (3) cycles, basins, level identity
    mins = sorted(set(v for v in cyc if v))
    total = 0
    for mn in mins:
        C = [mn]; x = T(mn, c)
        while x != mn: C.append(x); x = T(x, c)
        Cs = set(C)
        entry = set()
        for z in C:
            for b in [2 * z] + ([(2 * z - c) // 3] if (2 * z - c) % 3 == 0 and (2 * z - c) // 3 > 0 else []):
                if b not in Cs: entry.add(b)
        basin = cnt[mn]; total += basin
        if max(entry) <= A:
            lhs = sum(1 for z in C if z <= N) + sum(cnt[x] for x in entry)
            res = "ok" if lhs == basin else f"VIOLATED ({lhs} != {basin})"
        else:
            res = "not tested (entry point above A)"
        print(f"  cycle {mn} (length {len(C)}): basin {basin} = {basin / N:.6f} N, level identity {res}")
    print(f"  sum of the basins: {total} of {N}" + ("  (all starting values)" if total == N else ""))
    # x*h(x): mean value and profile modulo powers of 3, against the Syracuse law
    amax = min(A, N // 1000)                                   # keep a << N
    K = 2 / math.log(4 / 3)
    allv = [a * cnt[a] / N for a in range(1, amax + 1) if not cyc[a]]
    if allv:
        print(f"  mean of a*h(a) over the {len(allv)} non-periodic targets a <= {amax}: {mean(allv):.4f}"
              f"   (2/log(4/3) = {K:.4f})")
    if c % 3 and allv:
        for n in (1, 2, 3, 4):
            mod = 3 ** n
            law = syracuse_law(n)
            cinv = pow(c % mod, -1, mod)
            sums = [0.0] * mod; num = [0] * mod
            for a in range(1, amax + 1):
                if cyc[a]: continue
                sums[a % mod] += a * cnt[a] / N; num[a % mod] += 1
            dev = []
            for r in range(mod):
                if r % 3 == 0 or not num[r]: continue
                pred = K * mod * law[(r * cinv) % mod]
                dev.append(abs(math.log((sums[r] / num[r]) / pred)))
            line = f"  classes mod {mod}: mean of a*h(a) against (2/log(4/3))*{mod}*P(c*Syrac = r):"
            print(line + f" median |log ratio| {median(dev):.4f}, max {max(dev):.4f}")
            if n == 2:
                print("    r: measured / predicted  " + "  ".join(
                    f"{r}: {sums[r] / num[r]:.3f}/{K * mod * law[(r * cinv) % mod]:.3f}" for r in range(mod) if r % 3))
    print()

if __name__ == "__main__":
    if len(sys.argv) < 2: print(__doc__); sys.exit(1)
    for p in sys.argv[1:]: report(p)
