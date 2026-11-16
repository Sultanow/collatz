#!/usr/bin/env python3
"""Check finite versions of parity drift inequality, NOT asymptotic theorem."""
import math
rho=math.log(2)/math.log(3)
for c in (-1,1,5,13):
    a=max(786,2*abs(c),1)
    eps=abs(c)/((3*a-abs(c))*math.log(3))
    checks=0
    returns=0
    for start in range(2539,8193):
        x=start
        odds=0
        for t in range(1,600):
            if x<=a: break
            is_odd=x%2
            odds+=is_odd
            x=x//2 if not is_odd else (3*x+c)//2
            diff=abs(odds-rho*t-math.log(x/start)/math.log(3))
            assert diff<=eps*t+1e-10, (c,start,t,x,diff,eps*t)
            checks+=1
            if 4096<x<=8192 and 4096<start<=8192:
                returns+=1
                assert abs(odds-rho*t)<=math.log(2)/math.log(3)+eps*t+1e-10
        else:
            raise AssertionError((c,start,'censored'))
    print(f'c={c}: cocycle-checks={checks}, same-shell-return-checks={returns}: PASS')
print('All finite exact-orbit parity identities tested; this is not a proof of return probabilities.')
