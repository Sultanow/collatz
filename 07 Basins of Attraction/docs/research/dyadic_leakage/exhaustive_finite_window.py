#!/usr/bin/env python3
"""Complete finite-window enumeration of actual 3x+c trajectories.
Outputs EXACT integer-weighted dyadic block survival ratios.
This is not the limiting logarithmic first-excess law or an asymptotic proof.
"""
from collections import Counter
from fractions import Fraction
import math

def trajectory(m,c,cut,lo,X,maxsteps=15000):
    x=m; seen=set(); visits=[]
    for _ in range(maxsteps):
        if x<=cut:return visits,True
        if x in seen:return visits,False
        seen.add(x)
        x=x//2 if x%2==0 else (3*x+c)//2
        if lo<x<=X:visits.append(x)
    return visits,False

X=8192; beta=.13; cut=int(X**(1-2*beta)); lo=int(X**(1-beta)); L=3
K={j:math.ceil(4*2**(.3*j)) for j in range(20) if X/2**j>lo}
J=max(K)
print('Full enumeration of odd starts in [4X,16X): X',X,'beta',beta,'cut',cut,'W_lower',lo,'L',L,'K',K)
for c in (-1,1,5,13):
    Bnum=[0]*12; cnt_ex=0; censored=0; tot=0; maxN=0
    for m in range(4*X+1,16*X,2):
        tot+=1
        visits,ok=trajectory(m,c,cut,lo,X)
        if not ok:
            censored+=1
            continue
        s=0
        for i,z in enumerate(visits):
            s+=z
            if s>L*X:
                cnt_ex+=1
                ctr=Counter()
                for y in visits[i+1:]:
                    j=0
                    while y*2**(j+1)<=X:j+=1
                    ctr[j]+=1
                maxN=max(maxN,max(ctr.values(),default=0))
                for ell in range(len(Bnum)):
                    Bnum[ell]+=sum(2**(J-j)*K[j]*(ctr[j]>=2**ell*K[j]) for j in K)
                break
    assert censored==0, 'Finite window includes unresolved paths, cannot certify'
    assert Bnum[-1]==0, 'Need to inspect more dyadic layers'
    assert all(2*Bnum[i+1] < Bnum[i] for i in range(len(Bnum)-1) if Bnum[i]>0), 'Block leakage q<1/2 did not hold'
    ratios=[Fraction(Bnum[i+1],Bnum[i]) if Bnum[i] else None for i in range(5)]
    ratios_fmt=[f'{float(v):.5f} ({v})' if v is not None else '-' for v in ratios]
    print('c',c,'total',tot,'censored',censored,'excess',cnt_ex,'maxN',maxN,'B0scaled_integer',Bnum[0], 'q ratios',ratios_fmt)
print('PASS: exact integer dyadic-weight calculations verify block leakage q<1/2 over the complete specified finite windows.')
