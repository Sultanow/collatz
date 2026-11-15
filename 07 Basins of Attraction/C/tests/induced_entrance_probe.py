#!/usr/bin/env python3
"""FINITE-SAMPLE induced entrance diagnostic for the accelerated 3x+c map.

This is NOT a proof of geometric returns or a computation of asymptotic
logarithmic density. It measures the k-th shell *entry-state* law, finite-time
escape certificates and conditional survival on RESOLVED sampled trajectories.

Examples:
  python induced_entrance_probe.py --X '1000000000000000000000000000000000000000000000000000000000000' \
      --c 1 --start-factor-low 2 --start-factor-high 2.4 --samples 600 --H 180
"""
from __future__ import annotations
import argparse
from collections import Counter, defaultdict
from functools import lru_cache
import json
import random
from fractions import Fraction


def nthroot_floor(value: int, degree: int) -> int:
    lo = 0
    hi = 1 << ((value.bit_length() + degree - 1) // degree)
    while lo + 1 < hi:
        mid = (lo + hi) // 2
        if pow(mid, degree) <= value:
            lo = mid
        else:
            hi = mid
    return lo


def next_step(n: int, c: int) -> int:
    return n // 2 if n % 2 == 0 else (3 * n + c) // 2


def shell_id(X: int, a: int) -> int:
    j = (X // a).bit_length() - 1
    assert (a << j) <= X < (a << (j + 1))
    return j


def odd_part(a: int) -> int:
    return a >> ((a & -a).bit_length() - 1)


def entries_for_odd_start(m: int, c: int, X: int, lower: int,
                          passage: int, max_blocks: int):
    """List (j, shell number, oddpart, Syracuse-block index) in time order."""
    current = m
    seen = set()
    entries = []
    for block in range(max_blocks):
        if current <= passage:
            return "passed", entries, block
        if current in seen:
            return "cycle_above_passage", entries, block
        seen.add(current)
        t = 3 * current + c
        if t <= 0 or (t & 1):
            raise ValueError("requires odd c>=-1 and positive odd current")
        v = (t & -t).bit_length() - 1
        z = t >> v
        visited_shells = set()
        # Chronological halving chain: z*2^(v-1), ..., z.
        for i in range(v - 1, -1, -1):
            a = z << i
            if lower < a <= X:
                j = shell_id(X, a)
                if j in visited_shells:
                    raise AssertionError("one block has two visits to a dyadic shell")
                visited_shells.add(j)
                entries.append((j, a, z, block))
        current = z
    return "unresolved", entries, max_blocks


@lru_cache(maxsize=100000)
def finite_escape_good(a: int, c: int, lower_passage: int,
                       window_lower: int, shell_j: int, X: int, H: int, b: int) -> bool:
    """Certified in finite time: <=b-1 further shell visits before passage."""
    y = a
    future = 0
    for t in range(H + 1):
        if y <= lower_passage:
            return future < b
        if t == H:
            return False
        y = next_step(y, c)
        if window_lower < y <= X and shell_j == shell_id(X, y):
            future += 1
            if future >= b:
                return False
    return False


def sample_odds(u: int, v: int, n: int, seed: int):
    first = u | 1
    size = max(0, (v - first + 1) // 2)
    if size == 0:
        raise ValueError("starting interval contains no odd integers")
    rng = random.Random(seed)
    return [first + 2 * rng.randrange(size) for _ in range(n)]


def estimate(X, c, u, v, samples, seed, H, b, max_blocks, max_k):
    if X < 16 or not (0 < X < u < v) or c < -1 or (c & 1) == 0:
        raise ValueError("require X>=16, 0<X<u<v, odd c>=-1")
    lower = nthroot_floor(X**199, 200)  # beta=1/200
    passage = nthroot_floor(X**99, 100)
    rows = []
    states = Counter()
    for m in sample_odds(u, v, samples, seed):
        status, es, blocks = entries_for_odd_start(m,c,X,lower,passage,max_blocks)
        states[status] += 1
        if status != 'passed':
            continue
        per_shell = defaultdict(list)
        for j,a,z,block in es:
            per_shell[j].append((a,z,block))
        rows.append((1.0/m, per_shell))
    if not rows:
        raise RuntimeError("no resolved trajectories")
    wtot = sum(w for w,_ in rows)
    shell_ids = sorted({j for _,sh in rows for j in sh})
    all_shells={}
    for j in shell_ids:
        x_low = max(lower, X // (1 << (j+1)))
        x_high = X // (1 << j)
        size = max(0, x_high - x_low)
        # Mathematical support bound for the TRUE conditional law (not just data)
        B = X.bit_length() + 16
        support_tv_floor = 1.0 - min(size, 1 << (B - 1)) / (1 << (B - 1))
        stats=[]
        for k in range(1, max_k + 1):
            selected=[(w,sh[j][k-1][0],sh[j][k-1][1],len(sh[j]))
                      for w,sh in rows if len(sh.get(j,[])) >= k]
            if not selected:
                break
            mass=sum(w for w,*_ in selected)
            p_ge_k=mass/wtot
            continuation=sum(w for w,a,z,num in selected if num>=k+b)/mass
            good_cases=[]
            for w,a,z,num in selected:
                good=finite_escape_good(a,c,passage,lower,j,X,H,b)
                if good and num>=k+b:
                    raise AssertionError("a finite escape certificate incorrectly predicted a tail")
                good_cases.append((w,good))
            goodmass=sum(w for w,good in good_cases if good)/mass
            if continuation > 1-goodmass + 1e-10:
                raise AssertionError("finite conditional transport inequality failed")
            entry_distinct=len({z for _,a,z,num in selected})
            # Coarse residues are only diagnostics: they do NOT bound escape TV.
            residues={}
            for modpow in (4,6,8):
                mask=(1<<modpow)-1
                bins=Counter()
                for w,a,z,num in selected:
                    bins[z&mask]+=w/mass
                uniform=1/(1<<(modpow-1))
                tv=0.5*sum(abs(bins.get(r,0)-uniform)
                           for r in range(1,1<<modpow,2))
                residues[str(modpow)] = round(tv,7)
            stats.append({
                'k':k,'n_sampled_kth_visits':len(selected),
                'p_ge_k_in_resolved_sample':round(p_ge_k,7),
                'p_ge_k_plus_b_given_k':round(continuation,7),
                'finite_H_certified_escape_given_k':round(goodmass,7),
                'escape_plus_continuation_le_one':bool(continuation+goodmass<=1+1e-10),
                'unique_sampled_odd_part_states':entry_distinct,
                'coarse_oddpart_mod2B_TV_vs_uniform_DIAGNOSTIC_ONLY':residues
            })
        all_shells[str(j)]={
            'lower_exclusive':str(x_low),'upper_inclusive':str(x_high),
            'max_true_state_support':size,
            'reference_modulus_power_B':B,
            'THEORETICAL_lower_bound_on_true_conditional_TV':round(support_tv_floor,8),
            'conditional_return_stats':stats
        }
    return {
        'status':'FINITE_SAMPLE_DIAGNOSTIC_ONLY; NOT a theorem about logarithmic entrance laws',
        'parameters':{'X':str(X),'c':c,'u':str(u),'v':str(v),'samples':samples,
                      'seed':seed,'H':H,'b':b,'max_blocks':max_blocks,
                      'first_passage_cutoff':str(passage),
                      'window_lower_floor':str(lower)},
        'counts':dict(states),
        'n_resolved':len(rows),
        'unresolved_or_cycling_excluded_from_conditional_statistics':
            states['unresolved']+states['cycle_above_passage'],
        'shells':all_shells,
        'notes':[
          'Uniform odd samples from [u,v); 1/m reweighting only in the resolved sample.',
          'The future from a shell state is deterministic: no iid restart is used.',
          'Finite-time good event implies no b further visits before passage.',
          'TV over a coarse residue histogram cannot bound the full-state escape event.',
          'The support-capacity TV lower bound is mathematical and holds for any conditional law.',
          'No conclusion for rare/unresolved trajectories or asymptotic X,k.'
        ]
    }


def self_test():
    # Sampled shell visit counts agree with an independently expanded accelerated path.
    for c in (-1,1,5,13):
        for m in (101,109,371,519,1001):
            X=100; lower=45; passage=35
            status, entries, blocks=entries_for_odd_start(m,c,X,lower,passage,20000)
            if status!='passed':
                continue
            actual=[]
            x=m
            seen=set()
            while x>passage and x not in seen:
                seen.add(x)
                x=next_step(x,c)
                if lower<x<=X:
                    actual.append((shell_id(X,x),x,odd_part(x)))
            assert [(j,a,z) for j,a,z,_ in entries]==actual,(c,m,entries,actual)
    print('self-test: shell entry order and state identities match full accelerated T trajectories')


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--X',type=int,default=10**60)
    ap.add_argument('--c',type=int,default=1)
    ap.add_argument('--start-factor-low',default='2')
    ap.add_argument('--start-factor-high',default='12/5')
    ap.add_argument('--samples',type=int,default=700)
    ap.add_argument('--seed',type=int,default=261115)
    ap.add_argument('--H',type=int,default=180)
    ap.add_argument('--b',type=int,default=2)
    ap.add_argument('--max-blocks',type=int,default=10000)
    ap.add_argument('--max-k',type=int,default=6)
    ap.add_argument('--output',default='')
    args=ap.parse_args()
    self_test()
    u=(args.X * Fraction(args.start_factor_low).numerator) // Fraction(args.start_factor_low).denominator
    v=(args.X * Fraction(args.start_factor_high).numerator) // Fraction(args.start_factor_high).denominator
    result=estimate(args.X,args.c,u,v,args.samples,args.seed,args.H,args.b,
                    args.max_blocks,args.max_k)
    s=json.dumps(result,sort_keys=True,indent=2)
    if args.output:
        from pathlib import Path
        Path(args.output).write_text(s+'\n',encoding='utf-8')
    else:
        print(s)

if __name__=='__main__':
    main()
