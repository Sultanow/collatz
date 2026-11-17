#!/usr/bin/env python3
"""Check exact special finite orbit construction; no claims about density h.
All congruence and orbit assertions are checked with integers.
"""
import math, json

def shortcut(n):
    return n // 2 if n % 2 == 0 else (3 * n + 1)//2

def valuation_two(n):
    assert n != 0
    return ((n & -n).bit_length() - 1)

def example(t):
    r=(t+29)//30
    s=(t+19)//20
    mod=1<<(r+s)
    inv=pow(pow(3,t+1,mod),-1,mod)
    u0=((1-(1<<r))*inv)%mod
    u=next(u0+k*mod for k in range(3) if ( (1<<(t+1))*(u0+k*mod)-1)%3!=0)
    n=(1<<(t+1))*u-1
    a=2*u*pow(3,t)-1
    v=u*pow(3,t+1)-1
    b=v>>r
    assert n%3!=0 and a%2==1
    assert valuation_two(v)==r
    assert b>0 and b% (1<<s)==((1<<s)-1)
    assert valuation_two(3*a+1)==r+1
    z=n
    for j in range(t):
        assert z%2 == 1
        z=shortcut(z)
    assert z==a
    z=shortcut(a)
    assert z==v
    for j in range(r):
        assert z%2==0
        z=shortcut(z)
    assert z==b
    for j in range(s):
        assert z%2==1
        z=shortcut(z)
    X=a
    beta=1/200
    zeta=math.log(2)/math.log(3)
    zeta0=0.55
    tX=math.ceil(3*beta*math.log(X)/((zeta-zeta0)*math.log(3)))
    L=1+r+s
    assert (1+s)/L>zeta0, ((1+s)/L,t)
    assert L<=tX, (L,tX,t)
    assert r+1<=math.floor((1-zeta0)*tX+1), (r,tX,t)
    assert math.log(b)<.99*math.log(X), (t,math.log(b)/math.log(X))
    return {'t':t,'r':r,'s':s,'modulus_bits':r+s,'log_X':round(math.log(X),3),
            'prefix_length':L,'h_odd_fraction':round((1+s)/L,5),
            't_X':tX,'initial_valuation':r+1,
            'stop_exponent':round(math.log(b)/math.log(X),5),
            'log_a_over_n':round(math.log(a)-math.log(n),5),
            'a_over_n_exponent':round((math.log(a)-math.log(n))/math.log(a),5),
            'n_unit_mod_3':True}

def main():
    data=[example(t) for t in (100,150,200,400,800,1600,3200)]
    for row in data: print(row)
    with open('construction_checks.json','w') as f:json.dump(data,f,indent=2)
if __name__=='__main__': main()
