#!/usr/bin/env python3
# tail_fallback.py -- exact big-integer Phi_X/X for the starts that overflowed 128 bits in tail_check;
# merges them into the C output.  Usage: tail_fallback.py RUN.raw RUN.ovf > RUN.txt
import sys, math
from decimal import Decimal, getcontext
getcontext().prec=60
out, ovf = sys.argv[1], sys.argv[2]
head=open(out).read().split('\n')
m=__import__('re').search(r'c=(-?\d+) log10X=([\d.]+) beta=([\d.]+) S=(\d+)', head[0])
c=int(m.group(1)); E=float(m.group(2)); b=float(m.group(3)); S=int(m.group(4))
X=Decimal(10)**Decimal(str(E)); Wbot=X**(Decimal(1)-Decimal(str(b))); X1=X**(Decimal(1)-2*Decimal(str(b)))
mean,second,mx=[float(v) for v in head[1].split()[1::2]]
tail=[int(v) for v in head[2].split()[1:]]
starts=[int(l.split()[1]) for l in open(ovf) if l.startswith('OVF')]
sum1=mean*S; sum2=second*S   # overflow starts contributed 0 to the C sums; add their exact values below
vals=[]
for x in starts:
    phi=Decimal(0); steps=0
    while True:
        t=(3*x+c)//2
        while True:
            if Wbot < t <= X: phi+=Decimal(t)/X
            if t&1: break
            t//=2
        x=t; steps+=1
        if x<=X1: break
    v=float(phi); vals.append(v); sum1+=v; sum2+=v*v; mx=max(mx,v)
    for s in range(1,61):
        if v>s: tail[s-1]+=1
        else: break
print(head[0].replace('overflow=','overflow_resolved_exactly='))
print("mean %.5f second %.5f max %.3f"%(sum1/S,sum2/S,mx))
print("tail "+" ".join(map(str,tail)))
print("# overflow starts: %d, their Phi/X: %s"%(len(vals), ", ".join("%.2f"%v for v in sorted(vals,reverse=True)[:12])))
