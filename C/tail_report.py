#!/usr/bin/env python3
"""Compare the distribution of Phi_X/X for actual orbits with the random-walk model (Run I).
Usage: tail_report.py OUTDIR > report.txt"""
import sys, os, re, math, glob
def load(f):
    L=open(f).read().split('\n')
    m=re.search(r'c=(-?\d+) log10X=([\d.]+) beta=([\d.]+) S=(\d+)',L[0])
    ov=re.search(r'overflow\w*=(\d+)',L[0])
    mean,sec,mx=[float(v) for v in L[1].split()[1::2]]
    tail=[int(v) for v in L[2].split()[1:]]
    return dict(c=int(m.group(1)),E=float(m.group(2)),b=float(m.group(3)),S=int(m.group(4)),mean=mean,sec=sec,mx=mx,tail=tail,ovf=int(ov.group(1)) if ov else 0)
def rate(t,S):
    pts=[(s+1,math.log(n/S)) for s,n in enumerate(t) if n>=30]
    if len(pts)<4: return float('nan')
    pts=pts[len(pts)//3:]
    n=len(pts); mx=sum(p[0] for p in pts)/n; my=sum(p[1] for p in pts)/n
    return -sum((x-mx)*(y-my) for x,y in pts)/sum((x-mx)**2 for x,_ in pts)
d=sys.argv[1]
models={}
for f in glob.glob(os.path.join(d,'model_*.txt')):
    r=load(f); models[(r['E'],r['b'],r['S'])]=r
print("Run I: Phi_X/X for actual orbits against the random-walk model of Lemma 5 (logarithmic starts in [X^(1+b),X^((1+b)^2)])")
print("%5s %6s %4s %9s | %8s %8s | %9s %9s | %7s %7s | %s"%("beta","log10X","c","samples","mean","model","2nd mom","model","decay","model","P(Phi/X > L): actual / model (z)"))
for f in sorted(glob.glob(os.path.join(d,'actual_*.txt')), key=lambda f:(-load(f)['b'],load(f)['E'],load(f)['c'],load(f)['S'])):
    r=load(f)
    mods=[m for k,m in models.items() if k[0]==r['E'] and k[1]==r['b'] and k[2]==r['S']] or [m for k,m in models.items() if k[0]==r['E'] and k[1]==r['b']]
    m=mods[0]
    Ls=[10,20,30,40] if r['b']>0.01 else [2,4,6,8]
    parts=[]
    for L in Ls:
        pa=r['tail'][L-1]/r['S']; pm=m['tail'][L-1]/m['S']
        se=math.sqrt(pa*(1-pa)/r['S']+pm*(1-pm)/m['S']) or 1
        parts.append("L=%d: %.2e/%.2e (%+.1f)"%(L,pa,pm,(pa-pm)/se))
    print("%5.3f %6.0f %4d %9d | %8.4f %8.4f | %9.3f %9.3f | %7.3f %7.3f | %s   max %.1f, >2^125: %d"%(r['b'],r['E'],r['c'],r['S'],r['mean'],m['mean'],r['sec'],m['sec'],rate(r['tail'],r['S']),rate(m['tail'],m['S']),"; ".join(parts),r['mx'],r['ovf']))
