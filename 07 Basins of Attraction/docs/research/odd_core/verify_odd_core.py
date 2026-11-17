"""Exact-integer finite checks for the deterministic odd-core compression lemma.
This script is not a proof of asymptotic occupation or potential bounds.
"""
import random
from collections import defaultdict


def T(n, c):
    return n // 2 if n % 2 == 0 else (3 * n + c) // 2


def certify_prefix(seq, c, U):
    # Exact rational comparison via multiplication by 3U.
    shell = sum(z for z in seq if U < 2*z <= 2*U)
    odd = sum((z % 2 == 1 and 3*z > U-c and z <= U) for z in seq)
    if shell > U*(2+3*odd):
        raise AssertionError((c, U, seq[0], len(seq), shell, odd))


def test_spine():
    def H(n):
        return n > 0 and n & (n-1) == 0
    for n in range(3, 100_001):
        odd = (2*n-1) // 3 if (2*n-1) % 3 == 0 else 0
        assert int(H(n)) == int(H(2*n)) + (int(H(odd)) if odd>0 and odd%2 else 0)
    for m in range(5, 30):
        X=1<<m
        total=(1<<(m+1))-1
        assert total/X<2
        assert X*H(X)==X
    return 100_000 - 3


def test_charge():
    rng=random.Random(20261117)
    sets=[(c,U) for c in (-1,1,5,13,31) for U in (128,1024,16384,131072)]
    counts=defaultdict(int)
    largest=0
    for c,U in sets:
        for _ in range(900):
            n=rng.randint(1, 8*U)
            z=n
            seq=[]
            steps=rng.randint(0, 320)
            for t in range(steps+1):
                seq.append(z)
                z=T(z,c)
                if z<=0:
                    break
            certify_prefix(seq,c,U)
            counts[c]+=1
            largest=max(largest,len(seq))
    # Exhaustive shorter prefixes, varied shell sizes.
    for c in (-1,1,5,13):
        for U in (32,64,128,256):
            for start in range(1,6*U+1):
                z=start; seq=[]
                for t in range(50):
                    seq.append(z)
                    certify_prefix(seq,c,U)
                    z=T(z,c)
                    if z<=0:break
                counts[c]+=1
    return counts,largest

if __name__=='__main__':
    n=test_spine()
    counts, largest=test_charge()
    print('PASSED: harmonic spine recurrence for',n,'nonperiodic n in [3,100000]')
    print('PASSED: charging inequality exact arithmetic, prefix cases by c:',dict(sorted(counts.items())))
    print('Maximum random prefix length:',largest)
    print('NOTE: no asymptotic claim and no Lean kernel formalisation is certified by these tests.')
