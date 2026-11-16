#!/usr/bin/env python3
"""Finite, seeded diagnostics for dyadic first-entrance and mod-16 escape classes.

NOT a proof of uniform-in-X or return-index distribution. The random starts are
approximately log-uniform (rounding creates a tiny discretization effect).
Only trajectories whose passage below X/4 was observed are called resolved.
"""
import argparse, collections, json, math, random
from pathlib import Path


def step(n,c):
    return n//2 if n % 2 == 0 else (3*n+c)//2


def good(n,c):
    return n%4 == 0 or (3*n+c)%16 == 0


def run(X,c,samples,seed,max_steps,upper_factor=16.,lower_factor=4.):
    rng=random.Random(seed)
    logs=(math.log(lower_factor*X),math.log(upper_factor*X))
    enter=collections.Counter(); good_enter=collections.Counter()
    first_mod16=collections.Counter(); visited_mod16=collections.Counter()
    first_states=0; processed=0; unresolved=0; no_enter=0
    max_visits=0; maximum_steps=0; verified_escape=0
    for _ in range(samples):
        w=max(1,int(math.exp(rng.uniform(*logs))))
        x=w; visited=[]; hit_threshold=False
        for j in range(max_steps):
            if x<=X/4:
                hit_threshold=True; maximum_steps=max(maximum_steps,j)
                break
            if X/2 < x <= X:
                visited.append(x)
            x=step(x,c)
        if not hit_threshold:
            unresolved+=1
            continue
        processed+=1
        if not visited:
            no_enter+=1
        else:
            first_states+=1
            first_mod16[visited[0]%16]+=1
        max_visits=max(max_visits,len(visited))
        for k,y in enumerate(visited,1):
            enter[k]+=1
            visited_mod16[y%16]+=1
            if good(y,c):
                good_enter[k]+=1
                # Deterministic check: from good position, at most 1 further
                # shell visit before reaching <=X/4.
                v=y; future=0; crossed=False
                for _ in range(8):
                    v=step(v,c)
                    if v<=X/4: crossed=True; break
                    if X/2<v<=X:future+=1
                if crossed and future<=1:
                    verified_escape+=1
                else:
                    raise AssertionError((X,c,y,future,crossed))
    reps={}
    for k in range(1,min(15,max(enter,default=0))+1):
        reps[str(k)]={
            'survivors':enter[k],
            'good_count':good_enter[k],
            'good_fraction':round(good_enter[k]/enter[k],6) if enter[k] else None,
            'survival_2_return_ratio':round(enter.get(k+2,0)/enter[k],6) if enter[k] else None,
        }
    return dict(X=X,c=c,samples=samples,seed=seed,draw='approx log uniform [4X,16X)',
                threshold='X/4',shell='(X/2,X]',complete_orbits=processed,
                unresolved=unresolved,no_shell_entry=no_enter,first_entries=first_states,
                first_entry_mod16={str(k):first_mod16.get(k,0) for k in range(16)},
                max_shell_visits=max_visits,max_steps_to_passage=maximum_steps,
                good_class_residues=[r for r in range(16) if good(r,c)],
                verified_good_visits=verified_escape,
                visits_by_index=reps,
                statistical_status='finite numerical evidence, NOT asymptotic proof')


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--X',type=int,default=100000)
    ap.add_argument('--c',type=int,default=1)
    ap.add_argument('--samples',type=int,default=10000)
    ap.add_argument('--max-steps',type=int,default=10000)
    ap.add_argument('--seed',type=int,default=20261116)
    ap.add_argument('--output',default='')
    args=ap.parse_args()
    if args.c%2==0 or args.c < -1 or args.X<=64*max(1,args.c):
        ap.error('Need c odd and >= -1, and X comfortably greater than c')
    r=run(args.X,args.c,args.samples,args.seed,args.max_steps)
    text=json.dumps(r,indent=2,sort_keys=True)+'\n'
    if args.output:Path(args.output).write_text(text)
    else:print(text)

if __name__=='__main__':main()
