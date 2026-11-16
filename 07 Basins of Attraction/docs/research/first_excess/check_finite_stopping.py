#!/usr/bin/env python3
"""Finite exact-rational sanity checks; not an asymptotic or Lean proof."""
import math, random
from fractions import Fraction


def T(n, c):
    return n//2 if n % 2 == 0 else (3*n+c)//2


def path(m, c, lower, threshold, max_steps=25000):
    # If starting odd value is at or below the Syracuse threshold,
    # no blocks before the first passage exist.
    if m <= threshold:
        return []
    visited={m}
    n=m
    out=[]
    for _ in range(max_steps):
        n=T(n,c)
        if n <= threshold:
            return out
        if n in visited:
            return None  # no finite stopping time
        visited.add(n)
        if lower < n <= upper:
            out.append(n)
    return None

rng=random.Random(20261116)
tests=0
crossings=0
max_err=Fraction(0)
for c in (-1,1,5,13):
    for upper in (1200, 2800, 8000):
        lower=round(upper*.8)
        threshold=round(upper*.7)
        for _ in range(300):
            m=rng.randrange(threshold+2, upper*6)|1
            li=path(m,c,lower,threshold)
            if li is None:
                continue
            for L in (Fraction(1,2), Fraction(3,2), Fraction(5,1)):
                vals=[Fraction(v,upper) for v in li]
                Y=sum(vals)
                if Y<=L:
                    assert max(Y-L,0)==0
                    continue
                crossings+=1
                prev=Fraction(0)
                for idx,val in enumerate(vals):
                    if prev+val > L:
                        overshoot=prev+val-L
                        future=sum(vals[idx+1:])
                        assert 0<overshoot<=1
                        assert Y-L==overshoot+future
                        # Define dyadic shells based on max height
                        counts={}
                        for z in li[idx+1:]:
                            j=0
                            while Fraction(z,upper)<=Fraction(1,2**(j+1)):
                                j+=1
                            assert Fraction(1,2**(j+1)) < Fraction(z,upper)<=Fraction(1,2**j)
                            counts[j]=counts.get(j,0)+1
                        K={j:2 for j in counts}
                        E=sum(Fraction(1,2**j)*max(n-K[j],0) for j,n in counts.items())
                        cost=sum(Fraction(K[j],2**j) for j in counts)
                        assert E/2 <= Y-L <= 1+cost+E
                        break
                    prev+=val
                else:
                    raise AssertionError('no first crossing')
            tests+=1
print('successful terminated sampled paths:',tests)
print('exact first-excess and dyadic sandwich checks:',crossings)
