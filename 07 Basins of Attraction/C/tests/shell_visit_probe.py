#!/usr/bin/env python3
"""Finite-window diagnostics for dyadic shell local times of Syracuse orbits.

This is NOT an estimator of the limiting logarithmic law E_log.  Use it to
validate the definitions, the deterministic dyadic mass bound, and the
finite-window second-moment statistics.  Any unresolved starting values
are reported and omitted from moment summaries rather than silently set to 0.

Example:
  python shell_visit_probe.py --X 100000 --c -1 --u 2000000 --v 2400000 --samples 2500
"""
from __future__ import annotations
import argparse
import json
from collections import Counter, defaultdict

BETA = 1/200


def integer_nthroot_floor(value: int, degree: int) -> int:
    """Exact floor of a positive integer's degree-th root (no floating point)."""
    lo = 0
    hi = 1 << ((value.bit_length() + degree - 1) // degree)
    while lo + 1 < hi:
        mid = (lo + hi) // 2
        if pow(mid, degree) <= value:
            lo = mid
        else:
            hi = mid
    return lo


def trajectory(m: int, c: int, X: int, lower: int, passage: int, max_blocks: int):
    if m <= 0 or not m & 1:
        raise ValueError("odd positive start required")
    current = m
    seen = set()
    mass = 0
    counts = Counter()
    for block in range(max_blocks + 1):
        if current <= passage:
            return mass, counts, "passed", block
        if current in seen:
            return 0, Counter(), "cycle_above_passage", block
        if block == max_blocks:
            return mass, counts, "unresolved", block
        seen.add(current)
        t = 3*current+c
        if t <= 0 or t % 2:
            raise ValueError("requires odd c >= -1, and positive Syracuse orbit")
        nu = (t & -t).bit_length()-1
        current = t >> nu
        for i in range(nu):
            a = current << i
            if lower < a <= X:
                mass += a
                j = (X // a).bit_length()-1
                # Check strict lower and nonstrict upper dyadic-shell bounds
                assert a*(1 << j) <= X < a*(1 << (j+1))
                counts[j] += 1
    raise AssertionError("unexpected loop exit")


def probe(X: int, c: int, u: int, v: int, samples: int, max_blocks: int):
    if X < 16 or c < -1 or c % 2 == 0 or v <= u or u <= X:
        raise ValueError("requires X>=16, odd c>=-1 and X<u<v")
    # beta = 1/200 exactly: avoid huge absolute errors from float powers.
    lower = integer_nthroot_floor(X**199, 200)
    passage = integer_nthroot_floor(X**99, 100)
    first = u | 1
    n_odds = max(0, (v - first + 1)//2)
    count = min(samples, n_odds)
    chosen = [first + 2*((i*(n_odds-1))//max(1,count-1)) for i in range(count)]
    status = Counter()
    moments = defaultdict(float)
    numerator_mass = numerator_mass2 = numerator_visits = numerator_visits2 = totalw = 0.0
    max_v = 0
    max_shell = 0
    shell_bound_checks = 0
    for m in chosen:
        mass, counts, state, blocks = trajectory(m,c,X,lower,passage,max_blocks)
        status[state]+=1
        if state == 'unresolved':
            continue
        weighted_shells = sum(count * (2.0**(-j)) for j,count in counts.items())
        assert mass/X <= weighted_shells + 1e-10, (m,mass,counts,weighted_shells)
        shell_bound_checks += 1
        visits = sum(counts.values())
        w=1.0/m
        totalw += w
        numerator_mass+= w*mass/X
        numerator_mass2+=w*(mass/X)**2
        numerator_visits+=w*visits
        numerator_visits2+=w*visits*visits
        max_v=max(max_v,visits)
        for j,k in counts.items():
            moments[j]+=w*k*k
            max_shell=max(max_shell,j)
    if totalw == 0:
        raise ValueError("All trajectories unresolved")
    moments={str(j):moments[j]/totalw for j in range(max_shell+1)}
    B=max(moments.values(), default=0)
    return {
        'warning':'FINITE log-weighted starting window only; NOT the limiting E_log; NOT a proof',
        'parameters':dict(X=X,c=c,u=u,v=v,samples=len(chosen),beta=BETA,
                          lower_integer=lower,passage_integer=passage,max_blocks=max_blocks),
        'outcomes':dict(status),
        'pointwise_shell_inequality_checked_for':shell_bound_checks,
        'finite_log_weighted_mean_visits':numerator_visits/totalw,
        'finite_log_weighted_mean_visits_squared':numerator_visits2/totalw,
        'finite_log_weighted_mean_mass_over_X':numerator_mass/totalw,
        'finite_log_weighted_mean_mass_over_X_squared':numerator_mass2/totalw,
        'finite_shell_second_moments':moments,
        'max_finite_shell_second_moment':B,
        'checks':{'empirical_E_mass_squared_le_4B':numerator_mass2/totalw <= 4*B +1e-10},
        'max_visit_count':max_v,
    }


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--X',type=int,default=100000)
    p.add_argument('--c',type=int,default=1)
    p.add_argument('--u',type=int,default=2000000)
    p.add_argument('--v',type=int,default=2400000)
    p.add_argument('--samples',type=int,default=2500)
    p.add_argument('--max-blocks',type=int,default=10000)
    a=p.parse_args()
    print(json.dumps(probe(a.X,a.c,a.u,a.v,a.samples,a.max_blocks),indent=2))

if __name__=='__main__':
    main()
