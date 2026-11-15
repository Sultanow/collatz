# Lean certification boundary and research roadmap

Date: 2026-11-15

## Foundational theorems targeted by the first CI build

- `step_two_mul`: universal even predecessor.
- `step_of_odd`: accelerated odd branch.
- `step_eq_iff`: algebraically exact inverse classification for odd c.
- `mem_predecessors_iff`: finite inverse enumerator covers all cases.
- `step_pos`: positivity of every iterate for c >= -1 and positive input.
- `reachesWithin_succ_iff`: exact finite-horizon hitting identity.
- `finite_hitting_mass`: weighted finite-sample identity.

**Status before CI:** proofs written but NOT compiled in the present environment.
Do not use the word **Lean-certified** until GitHub Actions is green.

## Theorems not covered by this foundation

- existence of predecessor potential `h(a)`;
- logarithmic basin density and finite/infinite-level conservation of `h`;
- probabilistic coupling (Lemma 3), renewal/Lorden analysis (Lemma 4);
- Theorem 2's Syracuse-profile lower bound;
- exceptional-tail and return-count moment bounds;
- statements relying on the Mazur and El Jaouhari preprints.

## Milestones after green CI

1. Express `P(a)` and prove the one-step decomposition; handle
   periodic targets separately before claiming disjointness.
2. Formalise finite backward levels with no duplicated nodes and exact
   conservation of weights on each level, conditional on a harmonic weight
   function satisfying the predecessor-flow identity.
3. Introduce a clearly parameterised interface for the analytical limit
   and its hypotheses; do not assert convergence as an axiom.
4. Return to the probabilistic research goal: establish or disprove a
   uniform moment bound for actual Syracuse return visits `V_X`.

## Independent checks

`python tests/smoke_check.py` is an arithmetic smoke test; it is not a
proof, even if all of its finite cases pass. The proper proof condition is
`lake build CollatzBasins` plus `lake check` with the pinned dependencies.
