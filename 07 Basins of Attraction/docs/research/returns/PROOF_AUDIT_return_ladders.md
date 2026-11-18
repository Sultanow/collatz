# Internal proof audit — Genuine Return Ladders

Date: 2026-11-17. **No edits to main.tex. No claim of T_X=o(X).**

## New, unconditional result

For every b >= 100, construct positive integers a, X such that:

* a is in I_X = (X/2,X] and X > 2^200;
* the accelerated 3x+1 orbit of a has at least b+1 distinct visits to I_X **before** first passage below X^(99/100);
* every certified visit also lies in W_X = (X^(199/200),X];
* log_2 X < (100/33)b+6 and Phi_X(a)/X >= (5/8)(b+1).

Therefore sup of individual return count among finite excursions at scale X cannot be o(log X) uniformly. These are points, not probabilities. No claim about h(a), first-excess entrance probability, or aggregate return-tail asymptotic is implied.

## Mathematical proof checkpoints

1. On parity blocks P=OOE and M=OE, the affine maps are (9x+5)/8 and (3x+1)/4. Check exact arithmetic.
2. Multiplier balancing: use P when the current product is <=1 and M otherwise. Every product stays in [3/4,9/8].
3. Offset bound: e_i = r_i sum_{k<i} d_k/r_{k+1}, hence 0 <= e_i <= 15i/16, since r_i <=9/8, d_k <=5/8, r_{k+1}>=3/4.
4. Unique residue class modulo 2^d for every parity word (finite Terras-type parity-cylinder bijection). Construct a=2^d+residue, which forces the entire finite word.
5. Append r=ceil((m+2)/99)+2 even steps to a b-block word of length m<=3b. Because X=floor(6a/5)<2^(m+r+2) and 99r>=m+2, after the suffix the state is <X^(99/100).
6. For b>=100, a>=2^(m+r)>=2^(2b)>=2^200, and the endpoints are all within (X/2,X], which is a subset of W_X for X>2^200.
7. The points are distinct: repeats before the first passage would entail a cycle that never crosses below cutoff.
8. Since z_i >= 3a/4 and X<=6a/5, the weighted first-passage occupation has Phi_X(a)/X >=(5/8)(b+1).
9. r<= (m+2)/99+3 yields log_2X<100b/33+6. This proves the logarithmic visit lower bound.

## Independent calculation / limits

The attached `construct_returns.py` uses exact fractions to choose block words, exact modular inverses to compute the unique parity residue, and arbitrary-precision integers to verify every stated parity bit, all block endpoints, shell membership and final first-passage condition. Tested b=40, 100, 300 and 1000. Only b>=100 is claimed in the theorem.

No local Lean kernel verification. The mathematical assertion is presented as a research candidate pending independent review; the finite parity-bijection itself is classical and not claimed as new. This note does not establish an upper bound on the logarithmically weighted predecessor mass of the genuine long-return set.

## Strategic relevance and remaining gap

The previous return-certificate refinement removes *false* positives; it cannot eliminate **all** long-return states. The construction even produces b comparable to log X whereas the paper's threshold K_X is polylogarithmic with exponent <1. The needed new input must be a relative bound on the mass that the true, rare first-excess entrance law assigns to these actual multi-return states, or an independent finite-source estimate yielding the same weighted control.

This is a **negative structural test** of possible proof strategies, not an asymptotic breakthrough.
