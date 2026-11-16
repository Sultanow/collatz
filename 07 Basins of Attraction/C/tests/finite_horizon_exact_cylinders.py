#!/usr/bin/env python3
"""Exact fast counting of k-th shell arrivals by time T, with good mod-16 classes.

Enumerates parity cylinders (2^t) and threshold intervals, NOT individual
start values. All arithmetic is arbitrary-precision integer. The event is
k-th visit to shell before first state <= cutoff. No Collatz conjecture used.

Complexity roughly polynomial(T) * 2^T, independent of starting interval size.
"""
from collections import defaultdict
import json
from pathlib import Path


def step(x,c):
    return x//2 if x%2==0 else (3*x+c)//2


def good(y,c):
    return y%4 == 0 or (3*y+c)%16 == 0


def cylinder_count(A, B, L, cutoff, c, T):
    assert 1 <= A < B and c >= -1 and c%2 != 0 and T>=0
    # aggregate for each k over t=0,...,T: [total,good]
    entries=defaultdict(lambda: [0,0])
    branch_cells=0
    for t in range(T+1):
        M=1<<t
        for r in range(M):
            lo= -((r-A)//M)  # ceil((A-r)/M) = -floor((r-A)/M)
            hi=(B-1-r)//M +1
            if lo>=hi: continue
            # values x_s(r), slope d_s = 3^h*2^(t-s)
            vals=[r]
            h=0
            for s in range(t):
                if vals[-1]%2: h+=1
                vals.append(step(vals[-1],c))
            slopes=[(3**sum(vals[j]%2 for j in range(s))) * (1<<(t-s)) for s in range(t+1)]
            cuts={lo,hi}
            for v,d in zip(vals,slopes):
                for cp in ((cutoff-v)//d+1,
                           (L-2*v)//(2*d)+1,
                           (L-v)//d+1):
                    if lo < cp < hi:
                        cuts.add(cp)
            sorted_cuts=sorted(cuts)
            for left,right in zip(sorted_cuts,sorted_cuts[1:]):
                branch_cells+=1
                states=[v+d*left for v,d in zip(vals,slopes)]
                hits=0
                eligible=True
                for s,y in enumerate(states):
                    if y <= cutoff:
                        eligible=False;break
                    if 2*y > L and y <= L:
                        hits+=1
                    if s==t and (2*y <=L or y>L):
                        eligible=False
                if not eligible or hits<1: continue
                size=right-left
                q,rem=divmod(size,16)
                count_good=5*q
                v,d=vals[-1],slopes[-1]
                for j in range(rem):
                    if good(v+d*(left+j),c):count_good+=1
                entries[hits][0]+=size
                entries[hits][1]+=count_good
    return {'A':A,'B':B,'L':L,'cutoff':cutoff,'c':c,'T':T,'cells':branch_cells,
            'counts':{str(k):{'total':v[0],'good':v[1],
                              'ratio':round(v[1]/v[0],8)} for k,v in sorted(entries.items())},
            'max_abs_count_error':max((abs(16*v[1]-5*v[0])/16 for v in entries.values()),default=0),
            'guaranteed_count_error_bound':40*(T+1)*2**T}


def brute(A,B,L,cutoff,c,T):
    out=defaultdict(lambda:[0,0])
    for n in range(A,B):
        x=n;hits=0
        for t in range(T+1):
            if x<=cutoff:break
            if 2*x>L and x<=L:
                hits+=1
                out[hits][0]+=1
                if good(x,c):out[hits][1]+=1
            x=step(x,c)
    return {str(k):(v[0],v[1]) for k,v in out.items()}

if __name__=='__main__':
    # Cross-validate the entire exact-cylinder algorithm independently.
    for L in [100,256,1000]:
        for c in [-1,1,5,13]:
            T=9
            got=cylinder_count(4*L,16*L,L,max(1,L//64),c,T)
            ref=brute(4*L,16*L,L,max(1,L//64),c,T)
            assert {k:(v['total'],v['good']) for k,v in got['counts'].items()}==ref,(L,c,got['counts'],ref)
    print('Exact-cylinder vs brute-force: 12/12 cross-checks PASSED')
    # Extra-large integer range; no sampling, no floating point in computation.
    L=10**30
    data=[cylinder_count(4*L,16*L,L,L//64,c,12) for c in [-1,1,5,13]]
    path=Path(__file__).resolve().parents[2]/'results'/'syracuse'/'finite_transport'/'huge_cylinder_counts.json'
    path.parent.mkdir(parents=True,exist_ok=True)
    path.write_text(json.dumps(data,indent=2)+'\n')
    for z in data:
        print('c',z['c'],'L=10^30 T=12 k=1..5',[(k, v['ratio']) for k,v in list(z['counts'].items())[:5]],'cells',z['cells'])
    print('Saved',path)
