#!/usr/bin/env python3
"""Exact finite first-return diagnostic and explicit residue-only obstruction.

This is not a proof of any uniform shell moment estimate. The finite graph
portion accepts only resolved, acyclic first-return graphs.
"""
from __future__ import annotations
import argparse
import json
import sys
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
from finite_return_graph import first_return
from induced_entrance_probe import next_step


def arithmetic_witness(X: int, c: int, k: int):
    m = 1 << k
    if not (k >= 1 and c % 2 and X > 12 * m + 2 * abs(c) + 2):
        raise ValueError("Need k>=1, odd c, and a sufficiently large X")
    # Least multiple of m with 4*A > X + 2*c.
    A = ((X + 2*c) // (4*m) + 1)*m
    y, z = 2*A-c, 3*A-c
    assert X < 2*y <= 2*X
    assert X < 2*z <= 2*X
    assert next_step(y,c) == z
    assert (y-z) % m == 0
    assert y > 0 and z > 0
    return dict(X=X,c=c,k=k,modulus=m,A=A,y=y,z=z,
                shell="(X/2,X]",one_step_return=True,
                same_residue=True)


def finite_graph(X: int, c: int, max_steps: int=20000):
    lower, cutoff = X//2, X//4
    states = range(lower+1,X+1)
    R = {}
    status = Counter()
    for y in states:
        state,z,_ = first_return(y,c,lower,cutoff,X,max_steps)
        status[state] += 1
        if state == "return":
            R[y]=z
        elif state == "passage":
            R[y]=None
        else:
            return dict(status="INCOMPLETE",X=X,c=c,counts=dict(status))
    # Functional graph cycle detection and complete return depth.
    marks = {}
    depth = {}
    def visit(y):
        if marks.get(y) == 1:
            raise ValueError("RETURN_CYCLE")
        if marks.get(y) == 2:
            return depth[y]
        marks[y]=1
        z=R[y]
        depth[y]=1+(visit(z) if z is not None else 0)
        marks[y]=2
        return depth[y]
    try:
        for y in R:
            visit(y)
    except ValueError:
        return dict(status="RETURN_CYCLE",X=X,c=c,counts=dict(status))
    # Minimal exact barriers U(y)=depth(y), V(y)=depth(y)^2.
    for y,z in R.items():
        uz=depth[z] if z is not None else 0
        vz=depth[z]**2 if z is not None else 0
        assert depth[y] >= 1+uz
        assert depth[y]**2 >= 1+2*uz+vz
    self_loops = {}
    witness = {}
    for k in [1,2,3,4,5,6,8,10,11]:
        m=1<<k
        hits=[(y,z) for y,z in R.items() if z is not None and (z-y)%m==0]
        self_loops[str(m)]=len(hits)
        if hits:
            witness[str(m)]={'y':hits[0][0],'z':hits[0][1]}
    return dict(status="FINITE_COMPLETE_ONLY",X=X,c=c,cutoff=cutoff,
                state_count=len(R),first_return_events=dict(status),
                exact_barrier_verified=True,
                mean_squared_visits_numerator=sum(t*t for t in depth.values()),
                mean_squared_visits_denominator=len(depth),
                max_visits=max(depth.values()),
                residue_self_edges=self_loops,residue_examples=witness)


def main():
    p=argparse.ArgumentParser()
    p.add_argument('--X',type=int,default=10000)
    p.add_argument('--c',type=int,default=1)
    p.add_argument('--k',type=int,default=8)
    p.add_argument('--skip-graph',action='store_true')
    p.add_argument('--output',type=str,default='')
    a=p.parse_args()
    rep={'arithmetic':arithmetic_witness(max(a.X,12*(1<<a.k)+2*abs(a.c)+3),a.c,a.k)}
    if not a.skip_graph: rep['graph']=finite_graph(a.X,a.c)
    data=json.dumps(rep,indent=2,sort_keys=True)+'\n'
    if a.output: Path(a.output).write_text(data,encoding='utf8')
    else: print(data)

if __name__=='__main__': main()
