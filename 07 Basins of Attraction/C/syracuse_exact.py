#!/usr/bin/env python3
"""Exact Syracuse 3-adic law, small n.

P_n(r) = P(Syrac(Z/3^n Z) = r). Computation uses the distributional
fixed-point recursion S_n = (1 + 3 S_{n-1}) 2^{-G} (mod 3^n)
where G is geometric with P(G=k)=2^{-k} for k>=1.
Exact integer numerators and a common denominator. Suitable for small n;
integer size grows quickly with n. No random sampling.
"""
from math import gcd
from functools import reduce
from argparse import ArgumentParser


def compute(max_n=4):
    p = [1]  # n=0, state r=0
    d = 1
    for n in range(1, max_n + 1):
        mod = 3**n
        order = 2 * 3**(n-1)  # multiplicative order of 2 modulo 3^n
        orbit = []
        x = 1
        for _ in range(order):
            orbit.append(x)
            x = (2*x) % mod
        assert x == 1 and len(set(orbit)) == order
        # Before multiplication by 2^{-G}, 1 + 3 S_{n-1}.
        B = [p[(z-1)//3] if z % 3 == 1 else 0 for z in orbit]
        assert sum(B) == d
        m = (1 << order) - 1
        den = d*m
        # P_j/den = sum_{t=1}^order 2^{order-t} B_{j+t} / den.
        a = 0
        for t in range(1, order+1):
            a += (1 << (order-t)) * B[t % order]
        nums = [0]*mod
        for j, z in enumerate(orbit):
            nums[z] = a
            a = 2*a - m*B[(j+1) % order]
        assert a == nums[orbit[0]]
        assert sum(nums) == den
        factor = reduce(gcd, nums + [den])
        p = [value//factor for value in nums]
        d = den//factor
        yield n, p, d


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--max-n', type=int, default=4)
    parser.add_argument('--print-distribution', action='store_true')
    args = parser.parse_args()
    if not 1 <= args.max_n <= 7:
        parser.error('Recommended 1 <= max-n <= 7: arbitrary precision denominators grow exponentially')
    for n, numerators, denominator in compute(args.max_n):
        support = sum(x > 0 for x in numerators)
        print(f'n={n} modulus={3**n} support={support} denominator_bits={denominator.bit_length()}')
        if args.print_distribution and n <= 3:
            print('  denominator:', denominator)
            print('  numerator by residue:', numerators)
        if n == 1:
            assert denominator == 3 and numerators == [0,1,2]
        if n == 2:
            assert denominator == 63 and numerators == [0,8,16,0,11,4,0,2,22]
    print('Exact checks passed (including Tao n=1,2 reference distributions).')
