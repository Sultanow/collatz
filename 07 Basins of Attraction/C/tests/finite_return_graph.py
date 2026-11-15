#!/usr/bin/env python3
"""Exact finite first-return graph diagnostic for a SINGLE bounded shell.

Enumerates every integer y in W_X=(floor(X^0.995), X] and computes the first
later shell visit before first passage below floor(X^0.99). A result is marked
complete only if EVERY state is resolved before max_steps. This does not prove
any uniform-in-X theorem or Collatz convergence. Graph ratios are numerical
summaries of an exactly enumerated graph, not asymptotic certificates.
"""
from __future__ import annotations
from argparse import ArgumentParser
from collections import Counter, defaultdict
from fractions import Fraction
from functools import lru_cache
import json
from induced_entrance_probe import nthroot_floor, next_step


def first_return(y,c,lower,passage,X,max_steps):
    seen={y}
    z=y
    for steps in range(1,max_steps+1):
        z=next_step(z,c)
        if z<=passage:
            return 'passage',None,steps
        if lower<z<=X:
            return 'return',z,steps
        if z in seen:
            return 'cycle_outside',None,steps
        seen.add(z)
    return 'unresolved',None,max_steps


def analyze(X,c,max_steps):
    lower=nthroot_floor(X**199,200)
    passage=nthroot_floor(X**99,100)
    if X<16 or c%2==0 or c< -1:
        raise ValueError('require X>=16 and odd c>=-1')
    states=range(lower+1,X+1)
    result={}
    status=Counter()
    total_return=0
    max_stps=0
    for x in states:
        state,dest,steps=first_return(x,c,lower,passage,X,max_steps)
        result[x]=dest if state=='return' else None
        status[state]+=1
        total_return+=state=='return'
        max_stps=max(max_stps,steps)
    incomplete=status['unresolved']+status['cycle_outside']
    report={
      'status':'EXACT_FINITE_ENumeration_ONLY; no uniform asymptotic statement',
      'parameters':{'X':X,'c':c,'shell_lower_exclusive':lower,'passage_cutoff':passage,
                    'max_steps':max_steps},
      'enumerated_shell_states':len(result),
      'first_return_states':total_return,
      'termination_status':dict(status),
      'maximum_steps_to_first_event':max_stps,
      'full_graph_resolved':incomplete==0,
    }
    if incomplete:
        return report

    @lru_cache(maxsize=None)
    def depth(x):
        # In an acyclic return graph, depth counts returns beginning at x.
        y=result[x]
        if y is None:
            return 1
        return 1+depth(y)

    # Separate exact cycle detection from recursion.
    cycle=[]
    colors={}
    def has_cycle(x):
        colors[x]=1
        y=result[x]
        if y is not None:
            if colors.get(y,0)==1:
                cycle.append(y)
                return True
            if colors.get(y,0)==0 and has_cycle(y):
                return True
        colors[x]=2
        return False
    for x in result:
        if colors.get(x,0)==0 and has_cycle(x):
            break
    report['return_graph_cycle_detected']=bool(cycle)
    if cycle:
        return report
    deepest=max(depth(x) for x in result)
    report['max_visits_including_initial']=deepest
    report['mean_visits_uniform_start_in_shell']=round(sum(depth(x) for x in result)/len(result),7)
    # Exact incoming harmonic mass for every return state z:
    # (Q nu)(z)/nu(z) with nu(y)=1/y.
    weights=defaultdict(Fraction)
    for y,z in result.items():
        if z is not None:
            weights[z]+=Fraction(1,y)
    ratios={z:z*mass for z,mass in weights.items()}
    report['max_exact_harmonic_sub_eigenratio_as_fraction']=(
        str(max(ratios.values())) if ratios else '0')
    report['max_harmonic_sub_eigenratio']=round(float(max(ratios.values(),default=0)),8)
    report['positive_ratio_states']=len(ratios)
    report['states_with_ratio_ge_one']=sum(v>=1 for v in ratios.values())
    report['harmonic_nu_not_subinvariant_with_factor_less_than_one']=any(v>=1 for v in ratios.values())
    return report


def main():
    p=ArgumentParser(description=__doc__)
    p.add_argument('--X',type=int,default=100000)
    p.add_argument('--c',type=int,default=1)
    p.add_argument('--max-steps',type=int,default=20000)
    p.add_argument('--output',default='')
    a=p.parse_args()
    result=analyze(a.X,a.c,a.max_steps)
    out=json.dumps(result,indent=2,sort_keys=True)
    if a.output:
        from pathlib import Path
        Path(a.output).write_text(out+'\n',encoding='utf-8')
    else: print(out)

if __name__=='__main__':main()
