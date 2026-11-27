#!/usr/bin/env python3
"""Roadmap A6, scoping (27.11.2026): the intertwiners are dense in Aff(Z_3) (Appendix F, "What a proof needs").

An intertwiner of length k is y -> 2^{|A|-|B|} y + (c_B - c_A)/2^{|B|} for words A, B of length k, with
c_A = sum_j 3^(j-1) 2^(alpha_j), alpha_j = a_(j+1) + ... + a_k; Cor_n is unbounded at every intertwiner.

1. Short intertwiners: which classes (e mod 2*3^(m-1), t mod 3^m) of Aff(Z/3^m) contain an intertwiner of length
   <= k (letters <= 8)? For m = 1, 2, 3 all of them, already for k = max(2, m).
2. The construction of the paper for every m: B equal to A except b_1 = a_1 + eps and b_k = a_k + delta gives
   Psi_B^{-1} Psi_A(y) = 2^{-eps-delta} y + (2^delta - 1) 2^{a_k - |B|} c', c' = sum_{j<k} 3^(j-1) 2^(gamma_j),
   gamma_j = a_(j+1) + ... + a_(k-1). Checked for all classes with m <= 3 and 3000 random classes for m = 4, 5, 6.

usage: intertwiner_density.py [MMAX_SHORT] [MMAX_CONSTRUCT]      (defaults 3 and 6; about a second)
"""
import random
import sys
from itertools import product


def c_word(w):
    return sum(3 ** (j - 1) * 2 ** sum(w[j:]) for j in range(1, len(w) + 1))


def intertwiner(A, B, m):
    M, L = 3 ** m, 2 * 3 ** (m - 1)
    return (sum(A) - sum(B)) % L, (c_word(B) - c_word(A)) * pow(2, -sum(B), M) % M


def short_cover(m):
    size = 2 * 3 ** (m - 1) * 3 ** m
    got = set()
    for k in range(1, m + 2):
        for A in product(range(1, 9), repeat=k):
            for B in product(range(1, 9), repeat=k):
                got.add(intertwiner(A, B, m))
        print(f"m={m}: |Aff(Z/3^m)| = {size}, classes with an intertwiner of length <= {k}: {len(got)}")
        if len(got) == size:
            return


def construct(m, e_t, t_t):
    """words A, B with Psi_B^{-1} Psi_A = (2^e_t, t_t) modulo 3^m"""
    M, L = 3 ** m, 2 * 3 ** (m - 1)
    t_t %= M
    if t_t == 0:
        v, delta = m, 0
    else:
        v = 0
        while t_t % 3 ** (v + 1) == 0:
            v += 1
        delta = 1 if v == 0 else 2 * 3 ** (v - 1)          # v_3(2^delta - 1) = v
    eps = (-e_t - delta) % L + L                           # linear part 2^{-eps-delta} = 2^{e_t}
    k, a_k, beta = m + 3, 1, 0                             # beta: target of |B| mod L
    if delta == 0:
        gam = list(range(k - 2, -1, -1))
    else:
        w = (2 ** delta - 1) // 3 ** v
        tau = (t_t // 3 ** v) * pow(2, beta - a_k, M) * pow(w, -1, M) % M    # required unit c' mod 3^m
        gam = []
        for j in range(1, m + 1):                          # gamma_j digit by digit, each chosen mod 6
            W = 6 * (m + 1 - j) + (k - 1 - m)
            for g in range(W, W + 6):
                rest = tau - pow(2, g, 3 ** (m - j + 1))
                if rest % 3 == 0 and (j == m or (rest // 3) % 3 != 0):
                    gam.append(g)
                    tau = (rest // 3) % 3 ** (m - j)
                    break
            else:
                raise RuntimeError("no gamma")
        gam += list(range(k - 2 - m, -1, -1))
    mid = [gam[i] - gam[i + 1] for i in range(k - 2)]      # a_2 .. a_{k-1}
    a1 = ((beta - (sum(mid) + a_k + delta + eps)) % L) or L  # fixes |B| mod L, does not change c'
    A = [a1] + mid + [a_k]
    B = [a1 + eps] + mid + [a_k + delta]
    return intertwiner(A, B, m) == (e_t % L, t_t)


def main():
    m1 = int(sys.argv[1]) if len(sys.argv) > 1 else 3
    m2 = int(sys.argv[2]) if len(sys.argv) > 2 else 6
    for m in range(1, m1 + 1):
        short_cover(m)
    rng = random.Random(1)
    for m in range(1, m2 + 1):
        M, L = 3 ** m, 2 * 3 ** (m - 1)
        cases = ([(e, t) for e in range(L) for t in range(M)] if m <= 3 else
                 [(rng.randrange(L), rng.randrange(M)) for _ in range(3000)])
        ok = sum(construct(m, e, t) for e, t in cases)
        print(f"construction, m={m}: {ok}/{len(cases)} classes realised")


if __name__ == "__main__":
    main()
