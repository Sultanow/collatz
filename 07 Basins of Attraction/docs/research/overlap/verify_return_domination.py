#!/usr/bin/env python3
"""Pathwise check of the counting in Proposition (return-domination), on sampled orbits.
For each sampled odd start w: follow the orbit up to its first repeated point; after the first value
<= X1, split the points above X1 into excursions (start u = the point before the run).  For every
excursion and every shell I_j = (X 2^-j-1, X 2^-j] cap (Y, X]:
  (1) #points of the excursion in I_j  <=  O = #odd points of J_j = ((U-c)/3, U] in the excursion,
  (2) for b in BS, every odd point of J_j in the excursion that is followed by b further odd points of
      J_j in the same excursion lies in C_{X,b} (its parity word of length t_X has a prefix of length
      s in [b, t_X] with at least zeta0*s odd entries); C_{X,b} shrinks as b grows, and for small b it
      contains every odd number, so only b >= 8 is tested,
  (3) the summand (sum of the distinct post-passage points of W_X) is at most
      X sum_j 2^-j ( b * #excursions meeting J_j + #odd points of J_j cap C_{X,b} on the orbit ).
beta is taken larger than in the paper (1/200) so that W_X has several shells and returns are frequent.
usage: verify_return_domination.py [samples]"""
import math
import random
import sys

ZETA = math.log(2) / math.log(3)
ZETA0 = 0.55
BS = (8, 12, 16, 20, 24, 28, 34)
B_SUM = 16


def T(x, c):
    return (3 * x + c) >> 1 if x & 1 else x >> 1


def in_C(a, c, b, tX):
    x, odd = a, 0
    for s in range(1, tX + 1):
        odd += x & 1
        x = T(x, c)
        if s >= b and odd >= ZETA0 * s:
            return True
    return False


def run(c, k, beta, samples, rng):
    X = 1 << k
    X1 = X ** (1 - 2 * beta)
    Y = X ** (1 - beta)
    tX = math.ceil(3 * beta * math.log(X) / ((ZETA - ZETA0) * math.log(3)))
    shells = []
    j = 0
    while X / 2 ** j > Y:
        U = X >> j
        shells.append((j, U, max(U // 2, math.floor(Y)), (U - c) / 3))
        j += 1
    stats = dict(w=0, exc=0, ret=0, viol1=0, viol3=0, maxratio=0.0, cand={b: 0 for b in BS}, viol2={b: 0 for b in BS})
    for _ in range(samples):
        w = int(math.exp(rng.uniform((k + 4) * math.log(2), (k + 20) * math.log(2)))) | 1
        orbit, seen, x = [], set(), w
        while x not in seen:
            seen.add(x)
            orbit.append(x)
            x = T(x, c)
        p = next((i for i, v in enumerate(orbit) if v <= X1), None)
        if p is None:
            continue
        stats["w"] += 1
        post = orbit[p:]
        excs, cur, start = [], None, None
        for i, v in enumerate(post):
            if v > X1:
                if cur is None:
                    cur, start = [], post[i - 1]
                cur.append(v)
            elif cur is not None:
                excs.append((start, cur))
                cur = None
        if cur is not None:
            excs.append((start, cur))
        summand = sum(v for v in set(post) if Y < v <= X)
        if summand:
            stats["ret"] += 1
        meet = [0] * len(shells)
        codd = [set() for _ in shells]
        for u, E in excs:
            stats["exc"] += 1
            for si, (j, U, lo, Jlo) in enumerate(shells):
                P = sum(1 for v in E if lo < v <= U)
                Jodd = [v for v in E if (v & 1) and Jlo < v <= U]
                if P > len(Jodd):
                    stats["viol1"] += 1
                if Jodd:
                    meet[si] += 1
                for b in BS:
                    for a in Jodd[:max(len(Jodd) - b, 0)]:
                        stats["cand"][b] += 1
                        if not in_C(a, c, b, tX):
                            stats["viol2"][b] += 1
                codd[si].update(Jodd)
                stats.setdefault("_rows", []).append((summand, meet, codd))
    b = B_SUM
    for summand, meet, codd in stats.pop("_rows"):
        bound = sum((X >> shells[si][0]) * (b * meet[si] + sum(1 for a in codd[si] if in_C(a, c, b, tX)))
                    for si in range(len(shells)))
        if summand > bound:
            stats["viol3"] += 1
        if summand:
            stats["maxratio"] = max(stats["maxratio"], summand / bound)
    two = " ".join(f"{bb}:{stats['cand'][bb]}/{stats['viol2'][bb]}" for bb in BS)
    print(f"c={c:+d} X=2^{k} beta={beta} t_X={tX} shells={len(shells)}: starts={stats['w']} with returns={stats['ret']} "
          f"excursions={stats['exc']} | (1) violations={stats['viol1']} | (2) b:points/violations {two} "
          f"| (3) violations with b={b}: {stats['viol3']}, max summand/bound={stats['maxratio']:.3f}")


if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 20000
    rng = random.Random(20261119)
    for c in (1, -1, 5, 13):
        for k, beta in ((36, 0.04), (48, 0.03)):
            run(c, k, beta, n, rng)
