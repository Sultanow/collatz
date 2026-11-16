#!/usr/bin/env python3
"""Run J: where the equidistribution heuristic for primitive cycles fails.
Usage: heuristic_tests.py CENSUS.csv NECKLACES.json LMAX_J ALGEBRAIC.txt
Part 1 (paper setting): census l<=40, |c0|<=1000, the 116 types with |D|>1000 and E>=1, split by gcd(l,k) and sign of D.
Part 2: dispersion index against the bound C for l<=LMAX_J (exact necklace counts).
Part 3: divisibility of aperiodic words by P=2^{l/2}+3^{k/2} (output of algebraic_factor).
Requires sympy. Monte Carlo p-values: probability that independent Poisson counts give an index <= the observed one (fixed seeds)."""
import sys, csv, json, math, random, statistics
from collections import Counter, defaultdict
from sympy import divisors, mobius, totient as phi   # needs sympy
def lyndon(l,k):
    g=math.gcd(l,k); return sum(int(mobius(d))*math.comb(l//d,k//d) for d in divisors(g))//l
def poisson(lam,rng):
    if lam<30:
        L=math.exp(-lam);k=0;p=1.0
        while True:
            p*=rng.random()
            if p<L: return k
            k+=1
    return max(0,int(round(rng.gauss(lam,math.sqrt(lam)))))
def index_p(rows,seed,sims=4000):
    rng=random.Random(seed); st=sum((o-E)**2/E for E,o in rows); n=len(rows)
    le=sum(1 for _ in range(sims) if sum((poisson(E,rng)-E)**2/E for E,o in rows)<=st)
    return st/n, le/sims
def cls(l,k):
    g=math.gcd(l,k); return 'gcd=1' if g==1 else ('even l,k' if l%2==0 and k%2==0 else 'other g>1')
census,neck,LJ,alg=sys.argv[1],sys.argv[2],int(sys.argv[3]),sys.argv[4]
# ---- part 1
rows=list(csv.DictReader(open(census))); obs=Counter((int(r['l']),int(r['k'])) for r in rows)
LMAX=max(int(r['l']) for r in rows); C=1000; groups=defaultdict(list)
for l in range(1,LMAX+1):
    for k in range(1,l+1):
        A=abs(2**l-3**k)
        if A<=C: continue
        S=sum(int(phi(e)) for e in divisors(A) if e<=C); E=lyndon(l,k)*S/A
        if E<1: continue
        o=obs.get((l,k),0); sg='D>0' if 2**l>3**k else 'D<0'
        for key in [cls(l,k),cls(l,k)+' '+sg,'all']: groups[key].append((E,o))
print("Part 1. Census l<=%d, |c0|<=1000, types with |D|>1000 and E>=1"%LMAX)
print("  %-15s types   E      O     O/E     z    index   P(index<=obs)"%"class")
for i,key in enumerate(['gcd=1','gcd=1 D>0','gcd=1 D<0','even l,k','even l,k D>0','even l,k D<0','other g>1','all']):
    r=groups.get(key,[])
    if not r: continue
    tE=sum(E for E,o in r); tO=sum(o for E,o in r); idx,p=index_p(r,100+i)
    print("  %-15s %4d %7.1f %5d  %.3f  %+5.2f  %.3f   %.4f"%(key,len(r),tE,tO,tO/tE,(tO-tE)/math.sqrt(tE),idx,p))
# ---- part 2
O={tuple(map(int,k.split(','))):{int(e):c for e,c in d.items()} for k,d in json.load(open(neck)).items()}
info={}
for l in range(1,LJ+1):
    for k in range(1,l+1):
        if 2**l==3**k: continue
        A=abs(2**l-3**k); info[(l,k)]=(A,lyndon(l,k),[(e,int(phi(e))) for e in divisors(A) if e<=20000])
print("\nPart 2. Exact necklace counts, l<=%d: dispersion index against the bound C"%LJ)
print("  C       all types (n, O/E, index, P)          gcd=1 types (n, O/E, index, P)")
for j,C in enumerate([300,1000,3000,10000,20000]):
    g=defaultdict(list)
    for (l,k),(A,L,divs) in info.items():
        if A<=C: continue
        E=L*sum(ph for e,ph in divs if e<=C)/A
        if E<1: continue
        o=sum(c for e,c in O.get((l,k),{}).items() if e<=C)
        g['all'].append((E,o)); g[cls(l,k)].append((E,o))
    out=[]
    for key in ['all','gcd=1']:
        r=g[key]; idx,p=index_p(r,200+10*j+(key=='all'),sims=2000)
        out.append("%3d  %.3f  %.3f  %.3f"%(len(r),sum(o for E,o in r)/sum(E for E,o in r),idx,p))
    print("  %-6d  %s        %s"%(C,out[0],out[1]))
# ---- part 3
print("\nPart 3. Aperiodic words with P | z_3(B), P=2^{l/2}+3^{k/2}, l,k even, l<=%d, expected count >= 5"%max(int(x.split()[0]) for x in open(alg)))
rows=[x.split() for x in open(alg)]
for sg in ['D>0','D<0']:
    r=[(int(a[0]),int(a[1]),float(a[-1].split('=')[1]),float(a[4].split('=')[1]),int(a[3].split('=')[1])) for a in rows if a[2]==sg]
    r=[x for x in r if x[3]/x[4]>=5]
    tot=sum(x[2]*x[3]/x[4] for x in r)/sum(x[3]/x[4] for x in r)
    print("  %s: %d types, pooled observed/expected %.3f, median %.3f, range %.2f-%.2f"%(sg,len(r),tot,statistics.median(x[2] for x in r),min(x[2] for x in r),max(x[2] for x in r)))
