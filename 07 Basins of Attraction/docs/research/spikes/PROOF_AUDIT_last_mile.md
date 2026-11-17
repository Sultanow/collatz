# Collatz last-mile internal proof audit (2026-11-17)

**Status: no proof of `T_X=o(X)`.** No change to `main(9).tex`, no Lean claim.

## 1. Unconditional arithmetic theorem (proved on paper)

For each sufficiently large integer `t`, set `r=ceil(t/30)`, `s=ceil(t/20)`, `d=r+s`, and choose odd `u<2^(d+1)` satisfying

`3^(t+1)u = 1 - 2^r (mod 2^d)` and `3 ∤ (2^(t+1)u-1)`.

The targets `n=2^(t+1)u-1` and `a=2u3^t-1` satisfy `T^t(n)=a`, `nu_2(3a+1)=r+1`, and `T^(r+1)(a)=m=(u3^(t+1)-1)/2^r`, with `m=-1 (mod 2^s)`. For `X=a`, the target `a` lies in the dangerous parity-cylinder set of the manuscript, satisfies its short-initial-valuation filter, and nevertheless reaches the cutoff `X^0.99`. This is an explicit proof of false positives in the dangerous cylinder superset; no Collatz conjecture is required.

## 2. Conditional no-go (requires external input, not verified)

Assumption UF: `h(n) >= kappa/n` for all `n` with `3∤n`.

An online 2026 preprint by **fourier_wren**, *A uniform linear lower bound for Collatz predecessor counts, from joint Syracuse atoms at fixed valuation*, `https://rstation.ai/problems/collatz/papers/10/v2`, claims a finite-count bound implying UF. The preprint's proof has NOT been independently audited here and its theorem is NOT an imported fact from the user's manuscript.

With UF, `P(n)⊂P(a)` implies `g_X(a)=a h(a)>=kappa a/n`, and `a/n >= (1/2)(3/2)^t`. As `X=a <= C 3^t 2^(t/12)`, this gives `g_X(a)>=c X^delta` with

`delta=log(3/2)/(log 3 + log 2/12)=0.3506347548...`.

Thus the previously proposed target `log(2+G_X)=o((log X)^epsilon)` **fails** under UF. So do the odd-target `L^p` moments for `p>=3`. These are conditional obstructions, not unconditional statements about the actual potential.

## 3. Unconditional survival-gated set (proved by modifying the previous return argument)

Replace the purely parity-based cylinder set by the subset requiring the orbit to remain above `X_1` during the very prefix witnessing the excess number of odd steps. Every state with `b` further shell returns before first passage remains inside this *live dangerous set*, so the old stopped predecessor domination estimate remains valid with its sum restricted to live states.

The constructed examples are excluded: they become parity-dangerous only after descending below the cutoff. This is a genuine tighter deterministic inequality, **not** an asymptotic upper bound on the live mass.

## 4. Numerical checks

`test_spike_construction.py` verifies all orbit and congruence identities using **exact integers**, then checks analytic scale comparisons numerically. Values tested: `t=100,150,200,400,800,1600,3200`, plus separate checks at `t=6400,10000`.

For `t=10000`: `r=334`, `s=500`, first dangerous prefix length `835`, odd fraction `0.6`, `t_X=1952`, initial 2-adic valuation `335`, and `log(m)/log(X)≈0.98002<0.99`.

The tests corroborate algebraic computations but are **not proofs** of the asymptotic statements.

## 5. What not to claim

- Do not say the real `h(n)` has been proven uniformly `>= kappa/n` as part of our paper. It is an **outside preprint claim** pending independent verification.
- Do not say `T_X=o(X)` has been proved; its core weighted live-mass estimate remains open.
- Do not claim all parity-dangerous targets have heavy first-excess contributions. Our explicit counterexamples illustrate why that implication would be wrong.
- No Lean build or kernel check was performed on these new paper proofs.

## 6. Next viable positive research path

Target a **weighted bound over the survival-gated dangerous set**, or a low-order moment `1<p<2.85` if external uniform lower floor is accepted. Such bounds are compatible with sparse polynomial spikes and avoid ruling out the genuine dynamics by an unrealistically strong pointwise hypothesis.
