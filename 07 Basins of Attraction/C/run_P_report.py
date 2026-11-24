#!/usr/bin/env python3
"""Run P: check and merge the meet-in-the-middle census, compare it with the heuristic.

usage:  run_P_report.py OUT_DIR CENSUS_L40.csv RESULTS_DIR [DMAX]

OUT_DIR       outputs of mitm_census: P_*.txt (one file per type); selftest_L24.txt (l <= 24) if present
CENSUS_L40    Run A census (l <= 40) for the merged complete census
RESULTS_DIR   receives rational_cycles_q3_d<DMAX>_L<LMAX>.csv (Run A plus Run P) and run_P_report.txt

Checks: every type (l, k) of the covered lengths has exactly one summary line, no type was skipped,
mismatches = 0. Heuristic as in Run J: a type (l, k) is expected to give
E = N(l, k) S(A) / A primitive cycles with |c0| <= DMAX, where N is the number of aperiodic
necklaces of type (l, k), A = |2^l - 3^k| and S(A) = sum of phi(e) over the divisors e <= DMAX of A.
"""
import csv
import glob
import math
import os
import re
import sys
from collections import defaultdict

SUM = re.compile(r"# q=(\d+) DMAX=(\d+) l=(\d+) k=(\d+) (.*)")
CYC = re.compile(r"c=(-?\d+) min=(\d+) l=(\d+) k=(\d+) D=(-?\d+) word=([01]+)")


def phi(n):
    r, m, p = n, n, 2
    while p * p <= m:
        if m % p == 0:
            while m % p == 0:
                m //= p
            r -= r // p
        p += 1
    if m > 1:
        r -= r // m
    return r


def mobius(n):
    r, m, p = 1, n, 2
    while p * p <= m:
        if m % p == 0:
            m //= p
            if m % p == 0:
                return 0
            r = -r
        p += 1
    return -r if m > 1 else r


def lyndon(l, k):
    g = math.gcd(l, k)
    return sum(mobius(d) * math.comb(l // d, k // d) for d in range(1, g + 1) if g % d == 0) // l


PHI = {}


def expected(l, k, dmax):
    A = abs(2 ** l - 3 ** k)
    S = 0
    for e in range(1, dmax + 1):
        if A % e == 0:
            if e not in PHI:
                PHI[e] = phi(e)
            S += PHI[e]
    return lyndon(l, k) * S / A


def cls(l, k):
    g = math.gcd(l, k)
    return "gcd=1" if g == 1 else ("even l,k" if l % 2 == 0 and k % 2 == 0 else "other gcd>1")


def main():
    out_dir, l40, res_dir = sys.argv[1], sys.argv[2], sys.argv[3]
    dmax = int(sys.argv[4]) if len(sys.argv) > 4 else 1000
    os.makedirs(res_dir, exist_ok=True)
    summ, cycles, skipped = {}, set(), []
    for path in sorted(glob.glob(os.path.join(out_dir, "P_*.txt"))):
        for line in open(path):
            m = SUM.match(line)
            if m:
                l, k = int(m.group(3)), int(m.group(4))
                if "skipped" in line:
                    skipped.append((l, k))
                    continue
                d = dict(x.split("=", 1) for x in m.group(5).split() if "=" in x)
                if (l, k) in summ:
                    print(f"WARNING: type ({l},{k}) twice", file=sys.stderr)
                summ[(l, k)] = d
                continue
            m = CYC.match(line)
            if m:
                cycles.add((int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4)), int(m.group(5)),
                            m.group(6)))
    ls = sorted({l for l, _ in summ})
    lmin, lmax = ls[0], ls[-1]
    missing = [(l, k) for l in range(lmin, lmax + 1) for k in range(1, l + 1) if (l, k) not in summ]
    mism = sum(int(d["mismatches"]) for d in summ.values())
    rep = open(os.path.join(res_dir, "run_P_report.txt"), "w")

    def P(s=""):
        print(s)
        rep.write(s + "\n")

    P(f"Run P: meet-in-the-middle census of the primitive cycles of 3x+c, |c0| <= {dmax}, word lengths {lmin}..{lmax}")
    P(f"types: {len(summ)} done, {len(missing)} missing, {len(skipped)} skipped; mismatches = {mism}")
    if missing:
        P("missing types: " + " ".join(f"({l},{k})" for l, k in missing[:40]))
    secs = [float(d["seconds"]) for d in summ.values()]
    P(f"CPU time {sum(secs) / 3600:.2f} h in total, longest type {max(secs) / 60:.1f} min; "
      f"max passes {max(int(d['passes']) for d in summ.values())}, "
      f"largest half-list {max(float(d['maxlist']) for d in summ.values()):.3e}")
    # merged complete census
    base = set()
    with open(l40) as f:
        for r in csv.DictReader(f):
            base.add((int(r["c"]), int(r["min"]), int(r["l"]), int(r["k"]), int(r["D"]), r["word"]))
    lbase = max(r[2] for r in base)
    new = {r for r in cycles if r[2] > lbase}
    allc = base | new
    path = os.path.join(res_dir, f"rational_cycles_q3_d{dmax}_L{lmax}.csv")
    with open(path, "w") as f:
        f.write("c,min,l,k,D,word\n")
        for r in sorted(allc, key=lambda r: (abs(r[0]), r[0] < 0, r[1])):
            f.write(",".join(map(str, r)) + "\n")
    P(f"merged census: {len(base)} cycles with l <= {lbase} (Run A) + {len(new)} with {lbase} < l <= {lmax} -> {path}")
    st = os.path.join(out_dir, "selftest_L24.txt")
    if os.path.exists(st):
        got = set()
        for line in open(st):
            m = CYC.match(line)
            if m:
                got.add((int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4)), int(m.group(5)),
                         m.group(6)))
        ref = {r for r in base if r[2] <= 24}
        P(f"selftest l <= 24 (selftest_L24.txt): {len(got)} cycles, Run A {len(ref)}; identical: {got == ref}")
    if lmin <= lbase:
        old = {r for r in cycles if r[2] <= lbase}
        ref = {r for r in base if lmin <= r[2]}
        P(f"overlap l = {lmin}..{lbase}: Run P {len(old)} cycles, Run A {len(ref)}; identical: {old == ref}")
    # per length against the heuristic
    obs = defaultdict(int)
    for r in allc:
        obs[(r[2], r[3])] += 1
    P("")
    P("primitive cycles per word length against the heuristic sum_k E(l,k):")
    P("   l   observed   expected   distinct |c0|")
    for l in range(max(lbase + 1, lmin), lmax + 1):
        E = sum(expected(l, k, dmax) for k in range(1, l + 1))
        o = sum(v for (ll, _), v in obs.items() if ll == l)
        nc = len({abs(r[0]) for r in allc if r[2] == l})
        P(f"  {l:2d}   {o:8d}   {E:8.1f}   {nc:6d}")
    # classes as in Run J (types with |D| > DMAX and E >= 1)
    P("")
    P("O/E by class (types with |D| > DMAX and E >= 1), as in Run J:")
    for lo, hi in ((1, lbase), (lbase + 1, lmax)):
        groups = defaultdict(lambda: [0, 0.0, 0])
        for l in range(lo, hi + 1):
            for k in range(1, l + 1):
                if abs(2 ** l - 3 ** k) <= dmax:
                    continue
                E = expected(l, k, dmax)
                if E < 1:
                    continue
                o = obs.get((l, k), 0)
                sg = "D>0" if 2 ** l > 3 ** k else "D<0"
                for key in (cls(l, k), cls(l, k) + " " + sg, "all"):
                    g = groups[key]
                    g[0] += 1
                    g[1] += E
                    g[2] += o
        P(f"  l = {lo}..{hi}:")
        for key in sorted(groups):
            n, E, o = groups[key]
            P(f"    {key:18s} types {n:4d}   E {E:8.1f}   O {o:6d}   O/E {o / E:5.2f}")
    # the maps of Proposition 'census'
    P("")
    P("cycles of 3x+1, 3x-1, 3x+5, 3x+13 with word length in the new range (c0 = +-1, 5, 13):")
    sel = sorted(r for r in new if r[0] in (1, -1, 5, 13))
    P("  none" if not sel else "\n".join(f"  c0={r[0]} min={r[1]} l={r[2]} k={r[3]}" for r in sel))
    P("")
    P("new cycles with the smallest |c0|:")
    for r in sorted(new, key=lambda r: (abs(r[0]), r[1]))[:25]:
        P(f"  c0={r[0]:5d}  min={r[1]:>12d}  l={r[2]}  k={r[3]}")
    rep.close()


if __name__ == "__main__":
    main()
