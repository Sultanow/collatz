#!/usr/bin/env python3
"""Exhaustively check the finite-instance mod-16 local-escape proposition.

This is a regression test only. The proof is in candidate_mod16_escape_transport.tex.
"""
import json
from pathlib import Path

def step(y,c):return y//2 if y%2==0 else (3*y+c)//2

def good(y,c):return y%4==0 or (3*y+c)%16==0

def check(L,c):
    states=list(range(L//2+1,L+1))
    good_states=[y for y in states if good(y,c)]
    for y in good_states:
        x=y; visits=0; success=False
        for k in range(5):
            if x <= L/4:
                success=True;break
            if L/2<x<=L:visits+=1
            if k==4:break
            x=step(x,c)
        assert success and visits<=2,(L,c,y,visits,x)
    return {'L':L,'c':c,'states':len(states),'good':len(good_states),
            'fraction':round(len(good_states)/len(states),8)}

def main():
    reps=[]
    for c in (-1,1,5,13,47):
        for L in (128,256,1024,4096,10000,50000):
            if L < 2*abs(c) or L<16:continue
            reps.append(check(L,c))
    result={'tested':len(reps),'total_states':sum(t['states'] for t in reps),
            'all_checks_passed':True,'examples':reps}
    output=Path(__file__).resolve().parents[2]/'results/syracuse/mod16_escape/local_escape_exact_grid.json'
    output.parent.mkdir(parents=True,exist_ok=True)
    output.write_text(json.dumps(result,indent=2)+'\n')
    print('Exhaustive finite grid passed:',result['tested'],'configurations,',result['total_states'],'shell states')
if __name__=='__main__':main()
