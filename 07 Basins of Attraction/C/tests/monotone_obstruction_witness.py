#!/usr/bin/env python3
"""Construct two OPPOSITELY oriented, globally terminating shell returns.

This program validates an elementary interval/congruence proof. It does not
assert a bound on return moments, on Lyapunov averages, or on Collatz convergence.
"""
import argparse,json,random
from pathlib import Path

def step(n,c):
    return n//2 if n%2==0 else (3*n+c)//2

def first_congruent_at_least(L,r,M):
    return L + ((r-L)%M)

def first_return(y,X,a,c,cap=10000):
    v=y
    for i in range(1,cap+1):
        v=step(v,c)
        if v<=a:return None,i,True
        if X/2<v<=X:return v,i,False
    raise RuntimeError('unresolved')

def validate(X,a,c):
    if c%2!=1 or c< -1 or a<64 or 4*a>X or X<16*(abs(c)+1):
        raise ValueError('Require c odd>=-1, 64<=a<=X/4, X>=16(|c|+1)')
    ratio=3*X+abs(c)
    M=1
    while M*a<ratio:M*=2
    assert M>=16 and M < X/8  # consequence of bounds on X,a,c
    inv3=pow(3,-1,M)
    r=(-c*inv3)%M
    Linc=X//2+1
    Uinc=(2*X-c)//3
    Ldec=(2*X-c)//3+1
    Udec=X
    y_up=first_congruent_at_least(Linc,r,M)
    y_down=first_congruent_at_least(Ldec,r,M)
    assert y_up<=Uinc and y_down<=Udec,(X,a,c,M,Linc,Uinc,Ldec,Udec,y_up,y_down)
    z_up=step(y_up,c)
    z_down=step(step(y_down,c),c)
    assert y_up<z_up and y_down>z_down
    assert X<2*y_up<=2*X and X<2*z_up<=2*X
    assert X<2*y_down<=2*X and X<2*z_down<=2*X
    assert first_return(y_up,X,a,c)[0]==z_up
    assert first_return(y_down,X,a,c)[0]==z_down
    for y,z in [(y_up,z_up),(y_down,z_down)]:
        v=y; orbit=[]
        for _ in range(100):
            if v<=a:break
            orbit.append(v)
            v=step(v,c)
        else:raise AssertionError('did not reach cutoff')
        assert [u for u in orbit if X<2*u<=2*X]==[y,z]
    return dict(X=X,a=a,c=c,modulus=M,residue=r,
                upward=dict(y=y_up,next_return=z_up),downward=dict(y=y_down,next_return=z_down),
                certified_global_cutoff=True,
                implication='No monotone (nondecreasing or nonincreasing) U(y/X) can satisfy U>=1+PU on entire terminating return graph')

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--X',type=int,default=100000)
    ap.add_argument('--a',type=int,default=1000)
    ap.add_argument('--c',type=int,default=1)
    ap.add_argument('--output',default='')
    v=ap.parse_args(); r=validate(v.X,v.a,v.c)
    s=json.dumps(r,indent=2,sort_keys=True)+'\n'
    if v.output:Path(v.output).write_text(s)
    else:print(s)
if __name__=='__main__':main()
