#!/usr/bin/env python3
"""Exact finite-integer verification of the finite-horizon mod-16 entrance bound.
This does NOT certify any infinite-time or all-return-index estimate.
"""
from collections import defaultdict
import json
from pathlib import Path


def step(n, c):
    return n // 2 if n % 2 == 0 else (3 * n + c) // 2


def good(y, c):
    return y % 4 == 0 or (3 * y + c) % 16 == 0


def run(L, c, T, cutoff_den=64):
    A, B = 4 * L, 16 * L
    cutoff = L // cutoff_den
    assert cutoff >= 1
    count = defaultdict(lambda: [0, 0])
    for n in range(A, B):
        y, visited = n, 0
        for t in range(T + 1):
            if y <= cutoff:
                break
            if L // 2 < y <= L:
                visited += 1
                e = count[(visited, t)]
                e[0] += 1
                if good(y, c):
                    e[1] += 1
            y = step(y, c)
    per_k = []
    for k in range(1, 9):
        a = sum(v[0] for (idx, t), v in count.items() if idx == k)
        g = sum(v[1] for (idx, t), v in count.items() if idx == k)
        if not a:
            continue
        gap_num = abs(16 * g - 5 * a)  # divide by 16 for count discrepancy
        # Very conservative analytic bound: 40*(T+1)*2**T
        assert gap_num <= 16 * 40 * (T + 1) * (2 ** T)
        per_k.append(dict(k=k, observed=a, good=g, good_fraction=round(g / a, 6),
                          count_discrepancy_numerator=gap_num, count_discrepancy_denominator=16))
    return dict(L=L, c=c, T=T, start=[A, B], cutoff=cutoff,
                theoretical_count_error_bound=40*(T+1)*2**T,
                per_k=per_k,
                weighted_invariant='weighted extension is established by summation by parts, not tested here')

if __name__ == '__main__':
    data = [run(L,c,T) for L,T in [(1000,8), (10000,12), (20000,14)] for c in [-1,1,5,13]]
    path=Path(__file__).resolve().parents[2]/'results'/'syracuse'/'finite_transport'/'finite_horizon_results.json'
    path.parent.mkdir(parents=True,exist_ok=True)
    path.write_text(json.dumps(data,indent=2)+'\n')
    print('Configurations:',len(data))
    for row in data:
        print(f"c={row['c']:>3}, L={row['L']:>6}, T={row['T']:>2}, k entries:", [(p['k'],p['good_fraction'],p['observed']) for p in row['per_k'][:5]])
    print('Saved',path)
