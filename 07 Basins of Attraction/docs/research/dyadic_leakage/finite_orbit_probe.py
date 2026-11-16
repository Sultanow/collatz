#!/usr/bin/env python3
"""Reproducible FINITE-WINDOW experiments on actual accelerated 3n+c trajectories.
Not a test of the limiting logarithmic entrance measure and not an asymptotic proof.
"""
import argparse
import collections
import math
import random


def trajectory(m, c, low, top, cutoff, limit):
    x=m
    seen=set()
    visits=[]
    for _ in range(limit):
        if x <= cutoff:
            return visits, True
        if x in seen:
            return visits, False
        seen.add(x)
        x=x//2 if x%2==0 else (3*x+c)//2
        if low < x <= top:
            visits.append(x)
    return visits, False


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--X',type=int,default=1_000_000)
    parser.add_argument('--beta',type=float,default=.13)
    parser.add_argument('--level',type=float,default=10.0)
    parser.add_argument('--samples',type=int,default=40000)
    parser.add_argument('--c',type=int,nargs='*',default=[-1,1,5,13])
    parser.add_argument('--seed',type=int,default=202611161827)
    args=parser.parse_args()
    X=args.X;low=int(X**(1-args.beta));cutoff=int(X**(1-2*args.beta))
    assert 0<cutoff<low<X
    random.seed(args.seed)
    print(f'X={X} beta={args.beta} lower_W={low} cutoff={cutoff} level={args.level} samples={args.samples}')
    print('WARNING: finite log-uniform input window [4X,16X); beta is deliberately larger than manuscript beta=1/200')
    for c in args.c:
        success=0;fail=0; excess=[]
        for _ in range(args.samples):
            m=(round(math.exp(math.log(4*X)+random.random()*math.log(4)))|1)
            visits,ok=trajectory(m,c,low,X,cutoff,limit=20000)
            if not ok:
                fail+=1
                continue
            success+=1
            cum=0.0
            for i,z in enumerate(visits):
                cum+=z/X
                if cum>args.level:
                    shell_counts=collections.Counter()
                    for y in visits[i+1:]:
                        j=0
                        while y*2**(j+1)<=X:
                            j+=1
                        shell_counts[j]+=1
                    excess.append(shell_counts)
                    break
        print(f'c={c} completed={success} censored={fail} first_excess={len(excess)} p_hat={len(excess)/args.samples:.7f}')
        K={j:math.ceil(4*2**(.3*j)) for j in range(30) if X/2**j>low}
        B=[]
        for z in range(12):
            val=sum(sum((2**(-j))*K[j]*(counts[j]>=2**z*K[j]) for j in K) for counts in excess)/args.samples
            B.append(val)
        for k in range(7):
            if B[k]>0:
                print(f' B[{k}]={B[k]:.7f} B[{k+1}]/B[{k}]={B[k+1]/B[k]:.4f}')
        for j in K:
            q4=sum(counts[j]>=4 for counts in excess)
            q8=sum(counts[j]>=8 for counts in excess)
            if q4:
                print(f' shell={j}, K={K[j]}, P(N>=4)={q4/args.samples:.6f}, ratio_P8_P4={q8/q4:.4f}')

if __name__=='__main__':main()
