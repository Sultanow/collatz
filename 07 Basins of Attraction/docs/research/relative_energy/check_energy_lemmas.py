#!/usr/bin/env python3
"""Independent exact checks of finite probability identities only, not Collatz asymptotics."""
from fractions import Fraction
from random import Random
import math

rng=Random(20261116)
cap_checks=0
for _ in range(10000):
    t=rng.randrange(1,10)
    m=2**t
    vals=[rng.randrange(0,20) for _ in range(m)]
    M=sum(vals)
    if M==0: continue
    A=Fraction(rng.randrange(2,12),rng.randrange(1,2)) # integer >=2
    u=A*Fraction(M,m)
    defect=sum((max(Fraction(v)-u,0) for v in vals),Fraction(0))
    selected=set(rng.sample(range(m),rng.randrange(m+1)))
    lhs=sum(vals[r] for r in selected)
    rhs=A*Fraction(len(selected)*M,m)+defect
    assert lhs<=rhs,(lhs,rhs)
    chi=Fraction(m*sum(v*v for v in vals),M*M)-1
    assert chi>=0
    assert defect<=Fraction(M,1)*chi/(A-1),(defect,chi)
    support=sum(v>0 for v in vals)
    assert defect>=Fraction(M,1)*max(Fraction(1)-A*Fraction(support,m),0)
    cap_checks+=1

recurrence_checks=0
for _ in range(10000):
    length=rng.randrange(5,70)
    b=rng.randrange(1,length)
    q=Fraction(rng.randrange(1,9),10)
    # arbitrary nonincreasing mass profile with terminating tail
    weights=sorted([rng.randrange(0,100) for _ in range(length)],reverse=True)+[0]*b
    d=[max(Fraction(weights[i+b])-q*weights[i],0) for i in range(length)]
    K=rng.randrange(0,length)
    total=sum(weights[K:],0)
    target=(b*max(weights,default=0)+sum(d[K:],Fraction(0)))/(1-q)
    assert total<=target,(total,target)
    recurrence_checks+=1

try:
 import numpy as np
 fourier_checks=0
 for _ in range(500):
    m=2**rng.randrange(2,11)
    weights=np.array([rng.randrange(0,20) for _ in range(m)],dtype=float)
    if weights.sum()==0:continue
    p=weights/weights.sum()
    chi=m*np.sum((p-1/m)**2)
    fourier=np.fft.fft(p)
    assert abs(chi-np.sum(np.abs(fourier[1:])**2))<1e-9
    fourier_checks+=1
except ImportError:
 fourier_checks='skipped (numpy unavailable)'
target_checks=0
for _ in range(5000):
    t=rng.randrange(1,9); m=2**t
    vals=[rng.randrange(0,20) for _ in range(m)]; M=sum(vals)
    if not M: continue
    chosen=set(rng.sample(range(m),rng.randrange(0,m+1)))
    alpha=Fraction(len(chosen),m)
    v=sum(vals[r] for r in chosen)
    e=max(Fraction(v)-alpha*M,0)
    assert Fraction(v)<=alpha*M+e
    # The event-specific defect can never exceed total mass and is at
    # most the complete TV-from-uniform defect (A=1).
    tv=sum((max(Fraction(w)-Fraction(M,m),0) for w in vals),Fraction(0))
    assert e<=tv and e<=M
    target_checks+=1
try:
 import numpy as np
 target_fourier=0
 for _ in range(300):
    m=2**rng.randrange(1,8)
    p=np.array([rng.randrange(0,30) for _ in range(m)],dtype=float)
    mask=np.array([rng.randrange(0,2) for _ in range(m)],dtype=float)
    left=(p*mask).sum()-mask.mean()*p.sum()
    # np.fft.fft uses the negative-exponent convention;
    # conjugate pairing is invariant with the definitions in LaTeX.
    right=(np.fft.fft(mask)*np.conjugate(np.fft.fft(p)))[1:].sum()/m
    assert abs(left-right.real)<1e-8 and abs(right.imag)<1e-8,(left,right)
    target_fourier+=1
except ImportError:
 target_fourier='skipped (numpy unavailable)'
print('PASS',cap_checks,'exact cap/chi/support checks,',recurrence_checks,
      'exact recurrence/tail checks,',fourier_checks,'Parseval checks,',
      target_checks,'event-defect checks,',target_fourier,'event Fourier checks')
