#!/usr/bin/env python3
"""Exact rational sanity tests of the dyadic layer-cake and flux identities.
Not a formal proof and NOT a test of the asymptotic Syracuse hypothesis.
"""
from fractions import Fraction
from random import Random
rng=Random(20261116)
checked=0
for trial in range(4000):
    num_shells=rng.randint(1,6)
    K=[rng.randint(1,9) for _ in range(num_shells)]
    # Sampled deterministic futures with rational entrance weights.
    futures=[([rng.randrange(0,150) for j in range(num_shells)],Fraction(rng.randint(1,9),rng.randint(10,50))) for _ in range(rng.randint(1,25))]
    # No need to normalize; all inequalities homogeneous in entrance mass.
    def surv(j,n):
        return sum((w for a,w in futures if a[j]>=n),Fraction())
    Esum=sum((Fraction(1,2**j)*sum((w*max(a[j]-K[j],0) for a,w in futures),Fraction()) for j in range(num_shells)),Fraction())
    B=[]
    for ell in range(12):
        B.append(sum((Fraction(K[j],2**j)*surv(j,2**ell*K[j]) for j in range(num_shells)),Fraction()))
    D=sum((2**ell*B[ell] for ell in range(1,len(B))),Fraction())
    assert D/2<=Esum<=B[0]+D,(D,Esum,B[0])
    for ell in range(len(B)-1):
        # Difference in (weighted) surviving dyadic paths equals the escape flux.
        flux=sum((Fraction(K[j],2**j)*(surv(j,2**ell*K[j])-surv(j,2**(ell+1)*K[j])) for j in range(num_shells)),Fraction())
        assert B[ell]-B[ell+1]==flux
        assert flux>=0
    checked+=1
print(f'PASS: {checked} independent rational-weighted finite-path tests of layer-cake and flux identities')
