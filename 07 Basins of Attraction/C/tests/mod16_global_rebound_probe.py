#!/usr/bin/env python3
"""Diagnostic: probability of MOD16 good-entry + NO later rebound from shell/4.

Samples approximately log-uniform integers in [4X,16X). Global cutoff is
X/cutoff_divisor, smaller than the local shell/4. Finite computation only.
"""
import argparse,bisect,collections,json,math,random
from pathlib import Path

def step(n,c): return n//2 if n%2==0 else (3*n+c)//2

def good(y,c): return y%4==0 or (3*y+c)%16==0

def run(X,c,n,seed,cutoff_divisor,max_steps):
    rng=random.Random(seed)
    low=X/2; local_exit=X/4; global_exit=X/cutoff_divisor
    if global_exit>=local_exit: raise ValueError('cutoff_divisor must exceed 4')
    numer=collections.Counter(); den=collections.Counter(); reb=collections.Counter()
    first_states=collections.Counter(); completed=0; unresolved=0
    maximum=0
    for _ in range(n):
        y=int(math.exp(rng.uniform(math.log(4*X),math.log(16*X))))
        path=[]
        for t in range(max_steps):
            if y<=global_exit:break
            path.append(y)
            y=step(y,c)
        else:
            unresolved+=1
            continue
        completed+=1
        ids=[i for i,v in enumerate(path) if low<v<=X]
        maximum=max(maximum,len(ids))
        for k,i in enumerate(ids,1):
            if k>15:break
            v=path[i]
            den[k]+=1
            if good(v,c):
                numer[(k,'good')]+=1
                j=next((j for j in range(i+1,len(path)) if path[j]<=local_exit),len(path))
                if j>=len(path):
                    raise AssertionError(('no local exit',X,c,v))
                remaining=bisect.bisect_right(ids,j)
                rebounds=remaining<len(ids)
                if rebounds:reb[k]+=1
                else:numer[(k,'good_and_no_rebound')]+=1
    ktab={}
    for k in range(1,min(12,max(den,default=0))+1):
        p=den[k]
        if not p: continue
        g=numer[(k,'good')]; gno=numer[(k,'good_and_no_rebound')]
        ktab[str(k)]=dict(count=p,good=g,good_fraction=round(g/p,6),
                         good_no_rebound=gno,good_no_rebound_fraction=round(gno/p,6),
                         rebound_given_good=round(reb[k]/g,6) if g else None,
                         two_step_survival=round(den.get(k+2,0)/p,6))
    return dict(X=X,c=c,n=n,seed=seed,global_cutoff_divisor=cutoff_divisor,
                completed=completed,unresolved=unresolved,max_visits=maximum,
                visits_by_index=ktab,
                note='Finite sampled distributions, not asymptotic or fully induced logarithmic law')

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--X',type=int,default=100000)
    ap.add_argument('--c',type=int,default=1)
    ap.add_argument('--samples',type=int,default=10000)
    ap.add_argument('--seed',type=int,default=20261116)
    ap.add_argument('--cutoff-divisor',type=int,default=64)
    ap.add_argument('--max-steps',type=int,default=20000)
    ap.add_argument('--output',default='')
    a=ap.parse_args()
    r=run(a.X,a.c,a.samples,a.seed,a.cutoff_divisor,a.max_steps)
    s=json.dumps(r,indent=2,sort_keys=True)+'\n'
    if a.output:Path(a.output).write_text(s)
    else:print(s)
if __name__=='__main__':main()
