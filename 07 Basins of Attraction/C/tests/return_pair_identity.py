#!/usr/bin/env python3
"""Independent finite combinatorial check; not a Lean proof."""
import random


def explicit_pairs(visits):
    return sum(1 for i in range(len(visits))
               for j in range(i+1, len(visits))
               if visits[i] and visits[j])


def compressed_pairs(visits):
    n=sum(visits)
    return sum(range(n))


def run():
    rng=random.Random(20261115)
    checks=0
    for length in range(60):
        for _ in range(120):
            v=[bool(rng.randrange(2)) for _ in range(length)]
            n=sum(v)
            p=explicit_pairs(v)
            assert p==compressed_pairs(v)
            assert n*n==n+2*p
            checks+=1
    print(f'PASS: {checks} finite chronological return-pair identities')

if __name__=='__main__': run()
