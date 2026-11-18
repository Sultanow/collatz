#!/usr/bin/env python3
"""Exact witness of many real shell returns followed by controlled descent.
No convergence conjecture and no randomized approximations.
"""
from fractions import Fraction
from math import log2
from argparse import ArgumentParser
import json


def word_for_b(b):
    # P = OOE, multiplier 9/8, M = OE, multiplier 3/4
    ratio=Fraction(1)
    word=[]
    ends=[ratio]
    choices=[]
    for _ in range(b):
        if ratio <= 1:
            word.extend([1,1,0]); ratio *= Fraction(9,8); choices.append('+')
        else:
            word.extend([1,0]); ratio *= Fraction(3,4); choices.append('-')
        assert Fraction(3,4) <= ratio <= Fraction(9,8)
        ends.append(ratio)
    return word, ends, ''.join(choices)


def parity_residue(word):
    A=1; B=0; two=1
    for odd in word:
        if odd:
            B=3*B+two
            A=3*A
        two*=2
    r=(-B*pow(A,-1,two)) % two
    return r,two


def T(x): return (3*x+1)//2 if x&1 else x//2


def test(b, verbose=False):
    assert b>=10
    word, ratios, choices=word_for_b(b)
    m=len(word)
    r=(m+2+98)//99+2 # ~ m/99 + 3
    w=word+[0]*r
    residue, modulus=parity_residue(w)
    a=modulus+residue
    U=(6*a)//5
    x=a
    endpoint_values=[a]
    pos=0
    assert U>2**100
    assert a>=16*b
    # show U cutoff relation exact: U <= 2^(100*r), so U/2^r <= U^(.99)
    assert U <= (1 << (100*r))
    assert 100*r >= m+r+2
    assert (U//2)<3*a//4
    for k in range(b):
        par=word[pos:pos+(3 if choices[k]=='+' else 2)]
        for e in par:
            assert (x&1)==e
            assert (x*2)>U  # x>U/2 for all intermediate values
            x=T(x)
            pos+=1
        assert U < 2*x and x<=U
        endpoint_values.append(x)
    assert pos==m
    before_suffix=x
    for _ in range(r):
        assert x%2==0
        x//=2
    # exact cutoff criterion x^100 <= U^99 (a few 10000-bit exponents for b~1000)
    assert x**100 <= U**99
    # boundary points all before first passage since U/2 > U^.99 for U>=2^100
    assert U>=2**100
    assert 2**100<=U  # implies U^.01 >=2
    lower_tracked=min(endpoint_values)
    upper_tracked=max(endpoint_values)
    assert lower_tracked*2>U>=upper_tracked
    assert U <= Fraction(6*a,5)
    out={"b":b, "parity_length":m,"even_descent":r,"bits_start":a.bit_length(),"bits_shell_top":U.bit_length(),"visits_certified":b+1,"visit_to_bits_ratio":round((b+1)/U.bit_length(),6),"min_endpoint_relative_start":round(float(Fraction(lower_tracked,a)),6),"max_endpoint_relative_start":round(float(Fraction(upper_tracked,a)),6),"first_passage_reached":True,"cutoff_exponent":"99/100", "block_prefix":choices[:80]}
    if verbose: print(json.dumps(out,ensure_ascii=False,indent=2))
    return out


def main():
    ap=ArgumentParser()
    ap.add_argument('--b', nargs='+',type=int,default=[40,100,300,1000])
    args=ap.parse_args()
    print(json.dumps([test(b) for b in args.b],ensure_ascii=False,indent=2))

if __name__=='__main__':main()
