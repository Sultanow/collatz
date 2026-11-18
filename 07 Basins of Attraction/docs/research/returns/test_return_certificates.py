"""Exact finite checks of the deterministic return-certificate identity.
This is not a proof for logarithmic entrance measures or asymptotic Collatz.
"""
from fractions import Fraction

def T(n):
    return n//2 if n%2==0 else (3*n+1)//2

def visits(start, low, upper, cutoff, maxsteps=20000):
    assert cutoff<low
    z=start; seq=[]
    for _ in range(maxsteps):
        if z<=cutoff: return seq
        if low<z<=upper: seq.append(z)
        z=T(z)
    raise RuntimeError('not stopped in tested finite horizon')

checked=0
for X, X1 in [(200,20),(600,20),(1800,30),(4000,40)]:
    shells=[(X//2,X), (X//4,X//2), (X//8,X//4)]
    for low,hi in shells:
        starts=range(X,3*X,3)
        trajectories=[visits(z,low,hi,X1) for z in starts]
        size=Fraction(1,len(trajectories))
        maxn=max((len(v) for v in trajectories),default=0)
        # Measure on states occupied on the n-th visit
        nu={n:{} for n in range(1,maxn+1)}
        for vs in trajectories:
            for n,y in enumerate(vs,start=1):
                nu[n][y]=nu[n].get(y,Fraction(0))+size
        s={n:sum(nu[n].values(),Fraction(0)) for n in nu}
        for b in range(1,6):
            A=set()
            for y in set(y for vs in trajectories for y in vs):
                if len(visits(T(y),low,hi,X1))>=b: A.add(y)
            for n in range(1,maxn+1):
                lhs=s.get(n+b,Fraction(0))
                rhs=sum((weight for y,weight in nu.get(n,{}).items() if y in A),Fraction(0))
                assert lhs==rhs,(X,low,b,n,lhs,rhs)
                checked+=1
                for K in range(0,maxn+1):
                    a=sum((s.get(i,Fraction(0)) for i in range(K+1,maxn+1)),Fraction(0))
                    B=sum((s.get(i,Fraction(0)) for i in range(K+1,K+b+1)),Fraction(0))
                    C=sum((sum((w for y,w in nu.get(i,{}).items() if y in A),Fraction(0)) for i in range(K+1,maxn+1)),Fraction(0))
                    assert a==B+C,(X,low,b,n,K)
print('PASS',checked,'exact n,b shell return identities; associated tail identities also passed')
