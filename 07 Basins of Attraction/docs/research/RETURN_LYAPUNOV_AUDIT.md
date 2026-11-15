# Two-level Lyapunov return certificate: proof audit (15 November 2026)

## Status and scope

- **Mathematical proof given here:** the finite two-level certificate and the fixed-residue obstruction for dyadic shells.
- **Finite computation:** reproducible exact first-return graphs on (X/2,X], for X=10,000 and c=-1,1,5,13; only fully resolved, acyclic graphs are accepted.
- **Lean:** `ReturnLyapunov.lean` is a NEW candidate. It has not been compiled by us. Only call its declarations kernel-checked after GitHub CI succeeds on this new version.
- **Still open:** existence of scale-uniform low-complexity U,V for the actual logarithmically induced entrance laws, the uniform shell second moment, T_X=o(X), and R_X=o(X).

## 1. Exact two-level certificate

Let R:I→I∪{†} be the partial first-return map before first passage, on a finite return graph where every relevant starting state terminates. Define PF(x)=F(R(x)) when R(x)∈I and zero otherwise. Suppose

    U(x) ≥ 1 + (PU)(x),
    V(x) ≥ 1 + 2(PU)(x) + (PV)(x).

If the path from x has n visits, then n≤U(x) and n²≤V(x), by backward induction on n. At a terminal state n=1 the lower bound for both certificates is 1. Otherwise the next state z has n−1 visits and

    V(x) ≥ 1 + 2U(z) + V(z) ≥ 1+2(n−1)+(n−1)² = n².

For a finite first-entry subprobability q1, E[N²]≤∑ q1(x)V(x). NO independence or uniform contraction is needed.

**Crucial limitation:** For a finite resolved graph the exact functions U(x)=N(x), V(x)=N(x)² always work, so *existence alone is trivial*. The hard step is finding useful explicit certificates with ∑q1 V bounded uniformly in shell size, for the actual induced entrance laws. No such estimate is claimed.

## 2. A new unconditional obstruction to fixed-residue certificates

Fix any odd integer c and k≥1. For every sufficiently large positive integer X, there exist x,z in (X/2,X] such that

    T_{3,c}(x)=z,   x≡z (mod 2^k).

**Proof.** Put m=2^k. Choose an integer multiple A of m in

    (X/4+c/2, (X+c)/3].

The interval length equals X/12−c/6, which exceeds m when X>12m+2c. Also X>2|c| ensures the chosen A is positive. The interval therefore contains a multiple A of m. Put x=2A−c and z=3A−c. Since c is odd, x is odd. The inequalities imply X/2<x<z≤X, and the accelerated odd step equals (3x+c)/2=z. Finally z−x=A is divisible by m. Therefore z is immediately the next shell visit and remains above any cutoff below X/2.

For a candidate U_X(x)=f_X(x mod 2^k), the local superharmonic inequality would imply

    f_X(x mod 2^k) ≥ 1+f_X(z mod 2^k)

at this pair, yet the residues coincide. Contradiction. This excludes **all** fixed 2^k-residue-only U, even if f_X is allowed to change with X, from being a global strict additive certificate for every sufficiently large shell.

This does not exclude weights depending jointly on x/X, the exact return trajectory, or a *growing* modulus.

**Explicit one-step witness.** Let c=1, k=11, A=2048 and X=6145. Then

    x=4095 → z=6143,   x≡z≡2047 (mod 2048),

and both lie in (6145/2,6145].

## 3. Computations

`C/tests/lyapunov_return_probe.py` checks arithmetic witnesses and uses the existing `finite_return_graph.py` to enumerate actual accelerated first returns. It refuses to infer completed trajectories from unresolved or cyclic return-graph states. For the four maps at X=10,000, all finite return graphs in this run were resolved and acyclic; the program checks the two local inequalities for U=N and V=N² **at every node**. It reports exact mean-square numerators and denominators rather than interpreting the values as limiting densities.

The modulo-only obstruction is additionally checked directly on the enumerated graph (self-edge in the quotient by modulo 2^k). The universal theorem in §2 is arithmetic, not an inference from these finite tests.

## 4. Relation to the previous return-potential package

The Green operator gives the *exact* second-moment identity but a simple pointwise contraction Qν≤ρν fails for the undiscounted occupation potential at some visited nodes. The present two-level dual certificate does not require a strict contraction at any point. Its second inequality contains the first-level potential U, and only ∑q1 V needs to be controlled.

A workable next candidate must use scale information or stronger orbit-dependent features, since finite fixed-modulus weights alone are impossible by §2. Suggested next experiment: search for U,V in families depending on both x/X and arithmetic data, and measure ∑q1 V under **actual** first-entry distributions, with unresolved orbits explicitly reported. A successful finite fit is not an asymptotic proof.

## 5. Lean proof boundary

The Lean candidate contains:
- `return_lyapunov_path`: abstract finite terminating return-path inequalities;
- `return_lyapunov_weighted`: the finite weighted sum bound;
- `return_same_residue_arithmetic`: explicit odd-step equality;
- `return_same_residue_divisor`: congruence of the pair;
- `return_same_residue_dyadic`: explicit dyadic shell inequalities.

Its `return_same_residue_dyadic` lemma proves a parameterized explicit infinite subsequence of shell witnesses. The stronger claim for **all sufficiently large X** uses an additional elementary multiple-in-an-interval argument, supplied above, which is *not yet formalized in Lean*.
