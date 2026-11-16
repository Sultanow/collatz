#!/usr/bin/env python3
"""Independent big-integer reference for Run I (tail_check, mode actual).
Usage: ref_tail.py C LOG10X BETA S SEED
Draws S odd starts logarithmically from [X^(1+b), X^((1+b)^2)] and prints the mean of Phi_X/X with its
standard error. Only the leading 45 bits of a start come from exp(); all lower bits are uniformly random.
(Taking all bits from a double would leave a band of zero bits and visibly bias the 2-adic valuations.)"""
import random, math, sys
c=int(sys.argv[1]); E=float(sys.argv[2]); b=float(sys.argv[3]); S=int(sys.argv[4]); random.seed(int(sys.argv[5]))
lnX=E*math.log(10); X=10**int(round(E))
Wbot=math.exp((1-b)*lnX); X1=math.exp((1-2*b)*lnX)
lnu=(1+b)*lnX; lnv=(1+b)**2*lnX
s1=s2=0.0
for _ in range(S):
    L=random.uniform(lnu,lnv)
    e=int(L/math.log(2)); sh=max(e-45,0)
    m=(int(math.exp(L-sh*math.log(2)))<<sh)|random.getrandbits(sh)|1
    phi=0
    while True:
        t=(3*m+c)//2
        while True:
            if Wbot<t<=X: phi+=t
            if t&1: break
            t//=2
        m=t
        if m<=X1: break
    v=phi/X; s1+=v; s2+=v*v
mean=s1/S
print("%.5f %.5f"%(mean, math.sqrt(max(s2/S-mean*mean,0)/S)))
