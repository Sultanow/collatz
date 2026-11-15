#!/usr/bin/env python3
"""Reproducible finite-window diagnostic of successive shell return probabilities.

Requires shell_visit_probe.py in the same directory (from the earlier package).
This script does NOT prove logarithmic limit laws, geometric tails or any
Collatz convergence result. Unresolved trajectories are explicitly reported.

Example:
  python shell_return_probe.py --X 100000 --c -1 --u 2000000 --v 2400000 \
      --samples 3000 --seed 20261115 --block 2
"""
from __future__ import annotations
import argparse
import json
import random
from collections import Counter, defaultdict

from shell_visit_probe import integer_nthroot_floor, trajectory


def sample_odds_uniform(u: int, v: int, n: int, seed: int) -> list[int]:
    # Integer-uniform draws from all odd integers in [u,v); no floating-point
    # bias and no equally-spaced residue lattice.
    first = u if u % 2 else u + 1
    n_odds = max(0, (v - first + 1) // 2)
    if not n_odds:
        raise ValueError("No odd starting integers in [u,v)")
    rng = random.Random(seed)
    return [first + 2 * rng.randrange(n_odds) for _ in range(n)]


def analyze(X: int, c: int, u: int, v: int, samples: int,
            seed: int, block: int, max_blocks: int) -> dict:
    if X < 16 or X >= u or v <= u or c < -1 or c % 2 == 0:
        raise ValueError("Requires X>=16, X<u<v, odd c>=-1")
    if block < 1 or samples < 1:
        raise ValueError("block and samples must be positive")

    lower = integer_nthroot_floor(X**199, 200)
    passage = integer_nthroot_floor(X**99, 100)
    starts = sample_odds_uniform(u,v,samples,seed)

    statuses = Counter()
    trajectories = []
    for m in starts:
        mass, shell_counts, state, steps = trajectory(
            m, c, X, lower, passage, max_blocks)
        statuses[state] += 1
        if state == 'unresolved':
            continue
        # Exact enumeration identity: each later visit pairs with all earlier.
        n = sum(shell_counts.values())
        pair_count = sum(nj*(nj-1)//2 for nj in shell_counts.values())
        assert 2*pair_count + sum(shell_counts.values()) == sum(
            nj*nj for nj in shell_counts.values())
        assert mass <= X * sum(nj/(2**j) for j,nj in shell_counts.items()) + 1e-6
        trajectories.append((m, shell_counts, steps))

    if not trajectories:
        raise ValueError("No resolved trajectories; no statistic can be computed")

    shell_indices = sorted({j for _,counts,_ in trajectories for j in counts})
    weight_total = sum(1.0/m for m,_,_ in trajectories)
    report_shells = {}
    for j in shell_indices:
        counts = [s.get(j,0) for _,s,_ in trajectories]
        weights = [1.0/m for m,_,_ in trajectories]
        max_n = max(counts)
        pn = {k: sum(w for w,nj in zip(weights,counts) if nj >= k) / weight_total
              for k in range(1, max_n+1)}
        entries=[]
        for k in range(1,max_n+1):
            pk=pn[k]
            pnext=pn.get(k+block,0)
            entries.append({
                'k':k,
                'p_ge_k':pk,
                'p_ge_k_plus_b':pnext,
                'conditional_survival_empirical':(pnext/pk if pk>0 else None),
                'support_count_ge_k':sum(nj>=k for nj in counts)
            })
        en=sum(w*n for w,n in zip(weights,counts))/weight_total
        en2=sum(w*n*n for w,n in zip(weights,counts))/weight_total
        epairs=sum(w*(n*(n-1)//2) for w,n in zip(weights,counts))/weight_total
        assert abs(en2 - (en+2*epairs)) <= 1e-9*max(1,en2)
        report_shells[str(j)] = {
            'p_any_visit':pn.get(1,0),
            'mean_count':en,
            'second_moment':en2,
            'mean_chronological_return_pairs':epairs,
            'max_observed_visits':max_n,
            'weighted_survival_probabilities':entries
        }

    return {
        'disclaimer':'Finite-window random sample; weighted only within sampled resolved trajectories. NOT E_log; NOT a geometric-tail proof.',
        'parameters':{'X':str(X),'c':c,'u':str(u),'v':str(v),'samples':samples,
                      'seed':seed,'block':block,'max_blocks':max_blocks,
                      'shell_lower_floor':str(lower),'passage_floor':str(passage)},
        'outcomes':dict(statuses),
        'n_resolved':len(trajectories),
        'unresolved_excluded_from_statistics':statuses['unresolved'],
        'sampling':'Independent uniform random odd start values in [u,v), with 1/m reweighting over resolved records',
        'per_shell':report_shells,
    }


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--X',type=int,default=100000)
    p.add_argument('--c',type=int,default=-1)
    p.add_argument('--u',type=int,default=2000000)
    p.add_argument('--v',type=int,default=2400000)
    p.add_argument('--samples',type=int,default=3000)
    p.add_argument('--seed',type=int,default=20261115)
    p.add_argument('--block',type=int,default=2)
    p.add_argument('--max-blocks',type=int,default=20000)
    p.add_argument('--output',type=str,default='')
    a=p.parse_args()
    result=analyze(a.X,a.c,a.u,a.v,a.samples,a.seed,a.block,a.max_blocks)
    output=json.dumps(result, indent=2,sort_keys=True)
    if a.output:
        from pathlib import Path
        Path(a.output).write_text(output+'\n',encoding='utf-8')
    else:
        print(output)


if __name__=='__main__': main()
