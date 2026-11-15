# Conditional entrance transport: audited research note (2026-11-15)

**Status.** New elementary implications are proved below. The uniform
conditional entrance distribution estimates for actual Collatz orbits are
**open**. This note does not assert T_X=o(X), R_X=o(X), or Collatz convergence.

## 1. Correct state space: return points and their odd parts

In `shell_visit_probe.py`, the observable Phi_X records the halving chain
inside each Syracuse block. An observed shell value `y` can be even. The
future accelerated orbit is determined by `y`; its next odd-to-odd Syracuse
step begins at `oddpart(y)`, after deterministic halving. For a half-open
dyadic shell I=(L,2L], at most one term of a single halving chain lies in I.
Do not treat every observed even term as an independent geometric-valuation
restart. The induced law of the *kth return state* is history-dependent.

## 2. Exact finite-support obstruction (unconditional)

Let I be a finite set of positive integers, and let mu be *any* probability
measure supported on the odd parts of elements of I. For B>=1 let U_B
be uniform on the 2^(B-1) odd residues modulo 2^B. With s=|I|,

    d_TV(mu mod 2^B, U_B) >= 1 - s/2^(B-1).

Proof: let A be the set of odd residue classes containing the support.
Then mu(A)=1 and U_B(A)=|A|/2^(B-1) <= s/2^(B-1). The variational
definition of total variation gives the claim. A sharper version uses
the actual number of distinct occupied residues instead of s.

Therefore an iid-geometric valuation theorem predicated on near-uniformity
mod 2^B cannot be applied uniformly at arbitrary lengths B after conditioning
on shell entrances when the current state belongs to a finite shell.
This is a methodological limitation, not a proof that geometric shell-return
tails are false.

## 3. A *finite-time* escape certificate (elementary exact theorem)

Fix X and a shell I contained in the target window W_X. Let a>0 be the
first-passage cutoff. For an integer b>=1 and H>=1 define

  G_{I,a}^{(b,H)} = { y in I : under the actual accelerated map T_{3,c},
      the trajectory hits <=a in at most H subsequent steps, before
      recording b further visits to I }.

This is a *decidable* condition for each concrete y, b, H, c, X.
Every y in G has strictly fewer than b further shell visits in the
first-passage trajectory. It is stronger than merely observing one large
2-adic valuation.

Take any probability law P on starting values for which the first-passage
visit count N_I is well-defined and finite almost surely. For k with
p(k)=P(N_I>=k)>0, let mu_k be the induced conditional law of the actual
kth *visited shell state*. Then

    p(k+b) <= p(k) * (1 - mu_k(G)).                       (E1)

Proof: on {N_I>=k, Y_k in G}, the b subsequent visits cannot occur
before first passage. This event has probability p(k)*mu_k(G), and is
disjoint from {N_I>=k+b}. Subtract it from {N_I>=k}.

In particular, if mu_k(G)>=eta>0 uniformly in X,I,k, then

    p(k+b) <= (1-eta) p(k),
    E[N_I^2] <= b^2 * (2-eta)/eta^2.

The last implication is exactly the previously proved block-geometric
criterion. The uniform lower bound for **real conditioned entrance laws**
is not proved.

## 4. A transport criterion separating two hard tasks

Let nu_{X,I,k} be any reference probability law on actual shell states.
For every event G,

    mu_k(G) >= nu_{X,I,k}(G) - d_TV(mu_k,nu_{X,I,k}).

Thus if uniformly in X,I,k

    nu_{X,I,k}(G_{I,a}^{(b,H_X)}) >= eta + epsilon,
    d_TV(mu_k,nu_{X,I,k}) <= epsilon,

then (E1) yields block-geometric returns with factor 1-eta.
There is *no* independence assumption. The two unproved tasks are now
(i) finite-horizon escape under a tractable reference law and
(ii) conditional transport to the true kth entrance law, uniformly in k.

Important: total variation measured only **after reducing states modulo 2^r**
does NOT control the escape event unless that event is measurable with
respect to these residue classes. Coarse residue histograms are a
*diagnostic*, not an admissible TV certificate for G.

## 5. Why this is a better target than repeated iid restarts

Tao's finite valuation-vector estimate applies under approximate
uniformity of the *initial law* modulo a sufficiently large 2-power;
it does not provide an iid future after a random stopping/return time.
The elementary finite-support obstruction makes unconditional repeat
application at fixed shell scale impossible for arbitrarily many digits.
The finite-time certificate E1 asks for a measurable condition on actual
entry states, and the conditional law needed to transfer it is explicit.

The iid random-walk model's p0=(1/8)(1-sqrt(3/8)) lower bound is kept as a
model-only benchmark; this does not establish either of the two required
uniform conditions above.

## 6. Proposed Lean work

`EntranceTransport.lean` provides finite algebraic statements:
- exact support-event discrepancy against a uniform reference law;
- transferring a lower bound from a reference event mass with a discrepancy error;
- converting an escape-mass estimate to a survival contraction.

The new Lean file is a **candidate** until a green GitHub `lake build` and
`lake check` are confirmed. Do not label (E1)'s asymptotic application
kernel-checked. `Clipping.lean` and `ReturnPairs.lean` were reported green
by the user, but the newly added paper remark is not formalised by them.

## 7. Empirical programme and limitations

Run `C/tests/induced_entrance_probe.py`: it uses independent uniform odd
starting values from a finite interval, records each shell return and the
exact even/odd return state, measures conditional survival for successive
visits, and tests the finite-time escape event against the *same* trajectory.
Unresolved paths are counted and excluded from conditional statistics;
all output is explicitly labelled finite-sample diagnostic only.

For future research, try `H_X` proportional to log X, b=1,2,3 and vary
X, c, shell j, initial window and sample method. A positive empirical
escape share is not a proof of uniformity over all k or X. The first
return law may be extremely biased toward rare trajectories.

## 8. Relation to the paper

The Contributions table may now link the green Clipping and ReturnPairs
modules. Section 7 gains the unconditional finite-support warning.
Do not merge the unproved conditional-transport criterion into the list
of proved dynamical results or label the Syracuse potential theorem
Lean-certified.

## 9. A second, potentially stronger path: deterministic return transfer operator

On the finite shell I, define the *partial first-return map* R_I(y): the
first subsequent visit of the accelerated orbit from y to I before the
first descent through the passage threshold; use a cemetery state if it
does not return. This is a deterministic function, not a stochastic kernel.

For a starting-value probability law supported on trajectories having finite
first passage, set

    q_k(A) = P(the kth shell visit exists and its state belongs to A), A subset I.

For every subset A of I, a direct partition by kth visit state gives

    q_(k+1)(A) = q_k({y in I : R_I(y) in A}) = (Q_I q_k)(A).

Thus q_k=Q_I^(k-1) q_1 **exactly**, with Q_I the positive sub-Markov
transfer operator induced by deterministic first returns. There is no
conditional independence assumption.

**Finite-operator supermeasure certificate (proved abstractly).** If there
exist constants 0<=rho<1, C>0 and a finite nonnegative reference measure
nu_I on I such that

    q_1 <= C nu_I                  (measure domination),
    Q_I nu_I <= rho nu_I           (sub-eigenmeasure inequality),

then positivity of Q_I implies

    q_k <= C rho^(k-1) nu_I,
    P(N_I>=k) <= C nu_I(I) rho^(k-1),
    E[N_I^2] <= C nu_I(I) (1+rho)/(1-rho)^2.

This reduces the **entire chain of conditional estimates** to one weighted
return-operator inequality. The unresolved problem is to find such a nu_I
with C nu_I(I) and rho uniformly bounded for all X,j. The paper's exact
predecessor potential h(a) suggests a *candidate source for weights*, but
no domination or spectral inequality from h is proved.

A functional graph for R_I can contain cycles. Any strict sub-eigenmeasure
with positive mass on such a cycle is impossible. Conversely, a finite
acyclic return graph need not have a uniform rho or uniform comparison C
as the shell size grows. The obstruction is quantitative, not the finite
algebra above.

The companion `candidate_entrance_transfer_section.tex` records both
the finite-time event criterion and this exact transfer-operator route.
Its conclusions are stated conditionally, and it must remain in
`docs/research/` until the uniform dynamical estimates are proved.

## 10. Exact bounded return graphs and a failed naive weight

`C/tests/finite_return_graph.py` enumerates **every** integer in the
finite window `(floor(X^0.995),X]` for a selected integer X. From each state
it follows the real accelerated map until (i) passage below
`floor(X^0.99)`, (ii) the next window visit, or (iii) an explicitly marked
unresolved/cyclic event. The partial first-return graph is declared complete
only if every state has reached (i) or (ii). Cycles of the finite return
graph are checked independently.

At X=100000, 5594 states were enumerated for each of c=-1,1,5,13.
Every first event resolved before the step cap 20000 and each return graph
was acyclic. The maximum number of shell visits, initial visit included,
was 3,2,2,3 respectively. These are **exact finite computational checks**,
not formal Lean proofs and not bounds uniform in X.

We also evaluated the elementary candidate `nu(y)=1/y` as a positive
reference weight. For each visited state z, the exact ratio is

   (Q nu)(z)/nu(z) = z * sum_{y : R(y)=z} (1/y).

The maximal ratios were 1.91136129 (c=-1), 1.02749681 (c=1),
1.02763453 (c=5), and 2.04178340 (c=13). Therefore **this naive harmonic
weight is not a strict sub-eigenmeasure** for these finite return maps:
its maximum ratio exceeds one. This does not refute the broader harmonic
backward-mass conjecture or rule out corrected weights based on h(x).

A viable next task is to design a *nontrivial* reference weight
`nu_{X,I}` (possibly an entrance-flow correction of the predecessor
potential) and to estimate both `Q nu/nu` and the comparison `q_1/nu`
without constants growing with X. No such construction is proved here.
