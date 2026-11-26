#!/usr/bin/env python3
"""Run P, analysis of the cycle heuristic up to word length 66 (Problem 1 of the paper).

The equidistribution heuristic expects E(l,k) = L(l,k) sum_e phi(e)/A primitive cycles of type (l,k) with
|c0| <= C (A = |2^l - 3^k|, e over the divisors e <= C of A, L = aperiodic necklaces). For l <= 40 it fits
(O/E = 0.95); for 41 <= l <= 66 the observed counts fall short (O/E = 0.71). This script

 1. tests a boundary effect: a cycle needs z_min >= A/e for the smallest rotation value z_min; the probability
    P(z_min >= A/(2e)) is estimated by sampling words of each type (it turns out to be about 1);
 2. compares one-parameter corrections of E by their Poisson deviance, fitted on l <= 40 and tested on
    41 <= l <= 66:
      birthday:  each e contributes phi(e)/A exp(-k^2 / (alpha M_e)), M_e = e / |2^(l/k) - 3|;
                 M_e is the common size of the odd members if they were all equal ((3 + e/x)^k = 2^l),
                 and exp(-k^2 / 2N) is the probability that a random map on N states has no repeated state
                 in k steps (birthday problem), with N = alpha M_e / 2 odd states of the right size;
      length:    E exp(-b l);   odd steps: E exp(-b k);   size only: exp(-k |2^(l/k) - 3| / (a e)) per e;
 3. prints observed against expected for the classes of Run J and for bins of k/l.

usage:  heuristic_P.py CENSUS_L66.csv [SAMPLES] [SEED] > heuristic_P.txt     (about 1 s)
"""
import csv
import math
import random
import sys
from collections import defaultdict

C = 1000


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


def zmin(pos, l, k, D):
    """smallest z_3 over the rotations of the word with ones at pos (z' = z/2 or (3z + D)/2)"""
    z = sum(3 ** (k - 1 - j) << p for j, p in enumerate(pos))
    bits = [0] * l
    for p in pos:
        bits[p] = 1
    zm = z
    for i in range(l - 1):
        z = (3 * z + D) // 2 if bits[i] else z // 2
        zm = min(zm, z)
    return zm


def cls(l, k):
    g = math.gcd(l, k)
    return "gcd=1" if g == 1 else ("even l,k" if l % 2 == 0 and k % 2 == 0 else "other gcd>1")


def main():
    census = sys.argv[1]
    W = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    rng = random.Random(int(sys.argv[3]) if len(sys.argv) > 3 else 1)
    obs = defaultdict(int)
    lmax = 0
    for r in csv.DictReader(open(census)):
        obs[(int(r["l"]), int(r["k"]))] += 1
        lmax = max(lmax, int(r["l"]))
    PH = {e: phi(e) for e in range(1, C + 1)}
    types = []                     # types with |D| > C and E >= 1, as in Run J
    for l in range(2, lmax + 1):
        for k in range(1, l + 1):
            D = 2 ** l - 3 ** k
            A = abs(D)
            if A <= C:
                continue
            divs = [e for e in range(1, C + 1) if A % e == 0]
            L = lyndon(l, k)
            E = L * sum(PH[e] for e in divs) / A
            if E < 1:
                continue
            zs = [zmin(sorted(rng.sample(range(l), k)), l, k, D) for _ in range(W)]
            Eb = L * sum(PH[e] * sum(1 for z in zs if 2 * z * e >= A) / W for e in divs) / A
            types.append(dict(l=l, k=k, L=L, A=A, D=D, divs=divs, E=E, Eb=Eb, o=obs.get((l, k), 0),
                              g=abs(2 ** (l / k) - 3), cls=cls(l, k)))

    def birthday(t, a):
        return t["L"] * sum(PH[e] * math.exp(-t["k"] ** 2 * t["g"] / (a * e)) for e in t["divs"]) / t["A"]

    models = {
        "plain": (lambda t, p: t["E"], [0]),
        "birthday": (birthday, [x / 10 for x in range(5, 600, 5)]),
        "length exp(-b l)": (lambda t, p: t["E"] * math.exp(-p * t["l"]), [x / 4000 for x in range(0, 400)]),
        "odd steps exp(-b k)": (lambda t, p: t["E"] * math.exp(-p * t["k"]), [x / 4000 for x in range(0, 600)]),
        "size only": (lambda t, p: t["L"] * sum(PH[e] * math.exp(-t["k"] * t["g"] / (p * e)) for e in t["divs"]) / t["A"],
                      [x / 100 for x in range(1, 400)]),
    }

    def dev(sel, f, p):
        s = 0.0
        for t in sel:
            m = max(f(t, p), 1e-300)
            o = t["o"]
            s += 2 * ((o * math.log(o / m) if o else 0.0) - (o - m))
        return s

    tr = [t for t in types if t["l"] <= 40]
    te = [t for t in types if t["l"] > 40]
    print(f"Run P, cycle heuristic up to l = {lmax} (|c0| <= {C}); types with |D| > {C} and E >= 1: "
          f"{len(tr)} with l <= 40, {len(te)} with 41 <= l <= {lmax}")
    print()
    print("1. boundary effect: E_b uses P(z_min >= A/(2e)) per divisor e, estimated from "
          f"{W} random words per type")
    for name, sel in (("l <= 40", tr), (f"41 <= l <= {lmax}", te)):
        O = sum(t["o"] for t in sel)
        E = sum(t["E"] for t in sel)
        Eb = sum(t["Eb"] for t in sel)
        print(f"   {name:14s} O {O:5d}   E {E:7.1f}   E_b {Eb:7.1f}   O/E {O / E:.2f}   O/E_b {O / Eb:.2f}")
    print()
    print("2. one-parameter corrections, Poisson deviance (fit on l <= 40, test on the longer words)")
    print("   model                 parameter   dev(l<=40)   dev(test)   O/M(test)   | fit on all: parameter  deviance")
    fitted = {}
    for name, (f, grid) in models.items():
        p = min(grid, key=lambda q: dev(tr, f, q))
        pa = min(grid, key=lambda q: dev(types, f, q))
        fitted[name] = (p, pa)
        O = sum(t["o"] for t in te)
        M = sum(f(t, p) for t in te)
        print(f"   {name:20s}  {p:9.4g}   {dev(tr, f, p):9.1f}   {dev(te, f, p):9.1f}   {O / M:9.2f}   | "
              f"{pa:9.4g}  {dev(types, f, pa):8.1f}")
    print()
    a40, aall = fitted["birthday"]
    f = models["birthday"][0]
    print(f"3. observed O, heuristic E and birthday-corrected E* (alpha = {a40:g} from l <= 40; {aall:g} from all)")
    print("   group                          types     O       E      E*    O/E   O/E*")

    def line(name, sel):
        if not sel:
            return
        O = sum(t["o"] for t in sel)
        E = sum(t["E"] for t in sel)
        Es = sum(f(t, a40) for t in sel)
        print(f"   {name:30s} {len(sel):5d}  {O:5d}  {E:7.1f} {Es:7.1f}  {O / E:5.2f}  {O / Es:5.2f}")

    for lo, hi in ((2, 20), (21, 30), (31, 40), (41, 48), (49, 56), (57, lmax), (2, 40), (41, lmax)):
        line(f"l = {lo}..{hi}", [t for t in types if lo <= t["l"] <= hi])
    for c in ("gcd=1", "even l,k", "other gcd>1"):
        for sg, test in (("D>0", lambda t: t["D"] > 0), ("D<0", lambda t: t["D"] < 0)):
            for rng_name, lo, hi in (("l<=40", 2, 40), (f"41..{lmax}", 41, lmax)):
                line(f"{c} {sg} {rng_name}", [t for t in types if t["cls"] == c and test(t) and lo <= t["l"] <= hi])
    for a, b in ((0, 0.45), (0.45, 0.55), (0.55, 0.62), (0.62, 0.66), (0.66, 1.01)):
        for rng_name, lo, hi in (("l<=40", 2, 40), (f"41..{lmax}", 41, lmax)):
            line(f"k/l in [{a:.2f},{b:.2f}) {rng_name}", [t for t in types if a <= t["k"] / t["l"] < b and lo <= t["l"] <= hi])


if __name__ == "__main__":
    main()
