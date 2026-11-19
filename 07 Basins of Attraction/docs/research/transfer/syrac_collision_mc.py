"""Independent Monte Carlo check of H_2(n) = 3^n * P(V = V' mod 3^n) for Tao's Syracuse variable
Syrac(Z/3^n) = sum_{m=1}^n 3^{m-1} 2^{-(a_1+...+a_m)} mod 3^n, a_i iid geometric P(a)=2^-a.
Computed from the definition (forward sum), not from the self-similar recursion."""
import numpy as np
rng = np.random.default_rng(20261119)
for n, S in ((5, 4_000_000), (8, 8_000_000), (11, 8_000_000)):
    q = 3 ** n
    inv2 = pow(2, -1, q)
    acc = np.zeros(S, dtype=np.int64)          # partial sum mod q
    pw = np.ones(S, dtype=np.int64)            # 2^{-S_m} mod q
    for m in range(1, n + 1):
        a = rng.geometric(0.5, size=S)
        pw = pw * np.array([pow(inv2, int(k), q) for k in range(0, 64)], dtype=np.int64)[np.minimum(a, 63)] % q
        acc = (acc + pow(3, m - 1, q) * pw) % q
    cnt = np.bincount(acc, minlength=q).astype(np.float64)
    H2 = q * (cnt * (cnt - 1)).sum() / (S * (S - 1.0))
    print(f"n={n}: H_2 Monte Carlo = {H2:.3f}  (S={S})")
