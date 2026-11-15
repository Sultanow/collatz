# Lean 4 foundation for Same Shadows, Different Forests

## Scope (intentionally narrow)

The Lean files formalise the **accelerated integer map** `step c n` and prove:

1. `step_two_mul`: every target `a` has even predecessor `2*a`;
2. `step_eq_iff`: for odd `c`, every immediate predecessor is either `2*a`
   or an odd integer `n` satisfying `3*n+c=2*a`;
3. `mem_predecessors_iff`: the finite predecessor enumerator is complete;
4. `step_pos`: the accelerated map preserves positive inputs for `c >= -1`;
5. `reachesWithin_succ_iff`: finite-horizon hitting events obey an exact recursion;
6. `finite_hitting_mass`: weighted counts over any finite sample obey that recursion.

**Not proved here:** existence or positivity of logarithmic/natural densities,
`h(a)` potential existence, infinite-level basin conservation, GGM/Mazur/El
Jaouhari results, coupling/renewal lemmas, Theorem 2, or tail estimates.
The finite weighted identity must **not** be described as a formalisation of
those analytic claims.

`step` has domain `Int`; the positive-input forward invariance of the range
`c >= -1` is made explicit by `step_pos`.

## Version pinning and local verification

The toolchain is Lean **v4.35.0**; Mathlib uses its corresponding **v4.35.0**
release commit `56a005855ea17e0b0958f73ec801ae140b0a1679`. Once dependencies resolve, commit the generated `lake-manifest.json`
so the exact Mathlib Git revision and transitive dependencies are locked.

Run in this directory:

```bash
lake update
lake exe cache get
lake build CollatzBasins
lake env lean AxiomAudit.lean
lake check
python tests/smoke_check.py
```

On Windows PowerShell, use the same `lake` commands after installing elan
through the official Lean 4 / VS Code extension.

GitHub Actions checks the build and runs `lake check` on push/PR. **Do not
call a result Lean-certified until these checks are green.** Protect `master`
if possible and require the `kernel-check` workflow before merging.

## Current verification status

Source prepared. **A Lean runtime was not available in the authoring
container**, so the `.lean` files have not been type-checked here. The GitHub
workflow is provided to make this status actionable. The Python cross-check
is only an independent arithmetic smoke test, not a Lean certificate.

The next formalisation milestones are: disjoint predecessor-basin
partition for nonperiodic targets, finite backward-level flow, and then
an explicitly axiom-tracked interface for logarithmic-density existence.
