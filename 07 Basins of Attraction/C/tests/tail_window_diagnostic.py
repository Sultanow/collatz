#!/usr/bin/env python3
"""Finite-window diagnostic of Syracuse first-passage masses.

Not an estimator of the logarithmic limiting law in the theorem.
Uses exact integers for trajectories and a bounded iteration count.
Unresolved starts are reported separately, NEVER silently assigned zero.

Example:
  python C/tests/tail_window_diagnostic.py --X 10000 --c 1 --u 11000 --v 18000
"""
from __future__ import annotations

import argparse
import json
import math
from collections import Counter
from typing import Optional


def first_passage_mass(start: int, c: int, lower_target: int,
                       upper_target: int, passage: int,
                       max_blocks: int) -> tuple[Optional[int], str, int, int]:
    """Return (weighted target sum, outcome, blocks, number of visits).

    The first-passage block is included. A known detected cycle above
    `passage` proves the first-passage time is infinite, so Phi=0.
    """
    if start % 2 != 1 or start <= 0:
        raise ValueError("start must be a positive odd integer")
    m = start
    seen = set()
    total = 0
    hits = 0
    for j in range(max_blocks + 1):
        if m <= passage:
            return total, "passed", j, hits
        if m in seen:
            return 0, "cycle_above_passage", j, 0
        if j == max_blocks:
            return None, "unresolved", j, hits
        seen.add(m)
        a = 3*m + c
        if a <= 0 or a % 2:
            raise ValueError("Requires odd c>=-1 and positive odd trajectory")
        nu = (a & -a).bit_length() - 1
        m = a >> nu
        # i=0,...,nu-1: the new Syracuse block, including its odd member.
        for i in range(nu):
            target = m << i
            if lower_target < target <= upper_target:
                total += target
                hits += 1
    raise AssertionError("loop bound logic")


def run(X: int, c: int, u: int, v: int, max_blocks: int) -> dict:
    beta = 1 / 200
    window_lower = X ** (1-beta)
    passage = X ** (1-2*beta)
    passage_int = math.floor(passage)
    starts = range(u | 1, v, 2)
    W = 0.0
    mean_phi = 0.0
    valid = 0
    status = Counter()
    max_hits = 0
    max_mass = 0
    distribution = []
    for m in starts:
        mass, state, blocks, hits = first_passage_mass(
            m, c, math.floor(window_lower), X, passage_int, max_blocks
        )
        status[state] += 1
        if mass is None:
            continue
        weight = 1.0/m
        distribution.append((mass/X, weight))
        W += weight
        mean_phi += weight*mass/X
        valid += 1
        max_hits = max(max_hits, hits)
        max_mass = max(max_mass, mass)
    probs = {}
    for cutoff in (1,2,4,8,16,32):
        probs[str(cutoff)] = {
            "fraction_above": sum(w for y,w in distribution if y>cutoff)/W if W else None,
            "weighted_excess_over_X": sum(w*max(0,y-cutoff) for y,w in distribution)/W if W else None,
            "weighted_mass_above_over_X": sum(w*y for y,w in distribution if y>cutoff)/W if W else None,
        }
    return {
        "label":"FINITE LOG-WEIGHTED START WINDOW ONLY; NOT E_log; NOT A PROOF",
        "parameters":{"X":X,"c":c,"u":u,"v":v,"beta":beta,
                      "W_lower_real":window_lower,"passage_real":passage,
                      "max_blocks":max_blocks},
        "starts_processed":sum(status.values()),
        "outcomes":dict(status),
        "unresolved_excluded_from_normalization":status['unresolved'],
        "mean_mass_over_X_on_resolved":mean_phi/W if W else None,
        "max_mass_over_X":max_mass/X,
        "max_visits":max_hits,
        "tail_statistics":probs,
    }


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--X',type=int,default=10000)
    p.add_argument('--c',type=int,default=1)
    p.add_argument('--u',type=int,default=11000)
    p.add_argument('--v',type=int,default=18000)
    p.add_argument('--max-blocks',type=int,default=10000)
    a=p.parse_args()
    if a.X<10 or a.u<=a.X or a.v<=a.u or a.c<-1 or a.c%2==0:
        p.error('Require X>=10, v>u>X, and odd c>=-1')
    print(json.dumps(run(a.X,a.c,a.u,a.v,a.max_blocks),indent=2))

if __name__=='__main__':
    main()
