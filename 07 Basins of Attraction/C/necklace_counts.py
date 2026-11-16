#!/usr/bin/env python3
"""Run J: primitive-cycle counts per type (l,k) and reduced denominator e from the exhaustive word census.
Usage: necklace_counts.py WORDS.txt LMAX OUT.json [CENSUS.csv]
WORDS.txt is the output of word_census (l k |D| e words). Periodic words are removed by Moebius inversion over
the divisors of gcd(l,k); a word (B')^t has the reduced denominator of B'. With CENSUS.csv the counts for e<=1000
are checked against rational_cycles_q3_d1000_L40.csv."""
import sys, math, csv, json
from collections import defaultdict, Counter
def divisors(n): return [d for d in range(1,n+1) if n%d==0]
W=defaultdict(dict)
for line in open(sys.argv[1]):
    l,k,a,e,c=map(int,line.split()); W[(l,k)][e]=c
LMAX=int(sys.argv[2]); O={}
for l in range(1,LMAX+1):
    for k in range(1,l+1):
        if 2**l==3**k: continue
        words=dict(W.get((l,k),{}))
        for t in divisors(math.gcd(l,k))[1:]:
            for e,c in O.get((l//t,k//t),{}).items(): words[e]=words.get(e,0)-(l//t)*c
        for e,c in words.items(): assert c%l==0,(l,k,e,c)
        O[(l,k)]={e:c//l for e,c in words.items() if c}
json.dump({"%d,%d"%t:d for t,d in O.items()},open(sys.argv[3],'w'))
if len(sys.argv)>4:
    cen=Counter((int(r['l']),int(r['k'])) for r in csv.DictReader(open(sys.argv[4])) if int(r['l'])<=LMAX)
    mine={t:sum(c for e,c in d.items() if e<=1000) for t,d in O.items()}
    bad=[t for t in set(cen)|set(mine) if cen.get(t,0)!=mine.get(t,0)]
    print("check against census (l<=%d, |c0|<=1000): %d cycles, %d mismatching types"%(LMAX,sum(mine.values()),len(bad)))
