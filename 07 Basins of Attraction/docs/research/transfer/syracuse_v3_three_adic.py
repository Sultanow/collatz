"""Finite 3-adic Syracuse-profile densities. Geometric(a)=2^-a truncated at a=45.
Purely a model diagnostic, NOT a computation of h(a) or of the entrance measure.
"""
import numpy as np
MAX_A = 45
P = np.array([1.0]); print('depth,haar_L1_density,haar_L1p5,haar_L2,haar_L2p5,haar_L3,p_mass_at_-1,all_ones_lower')
for n in range(1,12):
    q=3**n; x=np.arange(3**(n-1),dtype=np.int64)
    V=1+3*x
    nextP=np.zeros(q)
    for a in range(1,MAX_A+1):
        inverse=pow(pow(2,a,q),-1,q)
        indices=(V*inverse)%q
        nextP[indices]+=2.0**(-a)*P
    P=nextP
    f=q*P
    norms=[float(np.mean(f**p)**(1/p)) for p in (1,1.5,2,2.5,3)]
    print(n,*[f'{z:.5f}' for z in norms], f'{P[q-1]:.8e}',f'{2**(-n):.8e}',sep=',')
