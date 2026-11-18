# Live-parity filter / exact return-certificate audit

Date: 2026-11-17. Internal only. `main(9).tex` has not been edited.

## Unconditional result

The paper proves an explicit infinite family of *actual* accelerated `3x+1` trajectories with

- a prefix of `s=ceil((ln t+3)^2)` consecutive odd steps, all before first passage below `X^0.99`;
- `a=X` and `a` belongs to the manuscript's live dangerous set for `b_X=ceil((ln ln X)^2)`;
- `nu_2(3a+1)=1`;
- exactly **two visits** to its own shell `(X/2,X]` before first passage (the initial state and one crossing on a pure halving descent);
- a known predecessor `n` with `T^t(n)=a` and `3 ∤ n`.

This proves that **survival-gated parity danger is not sufficient for many shell returns**, not that all live-dangerous states are harmless.

## Conditional result (external hypothesis)

Only with the additional unverified claim `h(n)>=kappa/n` for all `3∤n`, the same family has `a h_X^downarrow(a)>= X^delta` along a sequence for every `delta < delta_* = ln(3/2)/(ln 3 + (ln 2)/45) ~=0.363967`. In turn this rules out an L^p bound on *the live dangerous set* for `p>1/delta_* ~= 2.7475`. This is not an unconditional claim about `h`.

## Exact identity (all finite deterministic processes)

`nu_n(y)` is the law of the nth visit to a shell, and `A_{I,b}` comprises states with `>=b` actual subsequent visits to the same shell. Then

`s(n+b) = nu_n(A_{I,b})`.

The corresponding sum and stopped-potential bounds follow by indexing and disjointness of occurrences of a fixed state along first-passage trajectories. The `h_X^downarrow(y)` domination requires the entrance law be the manuscript's logarithmic first-excess law and the source-to-target relationship used there.

## Independent finite checks

- Congruence and size identities, exact integers for `t=25000,30000,40000,50000,75000,100000,125000`.
- Direct deterministic forward simulation until first passage for `t=25000,30000,50000`: each has exactly two shell visits; the path has the advertised initial all-odd and subsequent all-even phases.
- 1,060 exact rational checks of the return-certificate identity for `3x+1` at different shells, visits and block sizes; associated tail identities passed.
- LaTeX compiled using pdflatex; 4 pages and no overfull boxes.

Numerical checks are not substitutes for proofs.

## Novelty and scope

These are an explicit arithmetic no-go family and a deterministic identity, not a proof of `T_X=o(X)` and not a new Lean formalization. The return-certified set is dynamic, so obtaining its *weighted* potential-mass decay remains an open problem. The finite return identity is elementary; no historical originality claim is made.
