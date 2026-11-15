import CollatzBasins.Basic

-- Scoped instance `Classical.propDecidable` is needed when elaborating
-- the `if` expressions in the statement of `finite_hitting_mass`.
open Classical

/-!
# Finite-horizon predecessor identity

This result concerns *finite hitting events*, and is independent of all
asymptotic/logarithmic density arguments.  It is valid even for periodic targets.
In contrast, decompositions of unbounded predecessor sets into disjoint branches
need an additional non-periodicity condition or special handling of cycles.
-/

namespace CollatzBasins

/-- Iterate the accelerated map exactly `k` times. -/
def iter (c : ℤ) : ℕ → ℤ → ℤ
  | 0, n => n
  | k + 1, n => iter c k (step c n)

/-- The starting point reaches `a` within at most `N` steps. -/
def reachesWithin (c n a : ℤ) (N : ℕ) : Prop :=
  ∃ k : ℕ, k ≤ N ∧ iter c k n = a

/-- Exact finite-horizon recursion: hit at time 0, or hit after one step. -/
theorem reachesWithin_succ_iff (c n a : ℤ) (N : ℕ) :
    reachesWithin c n a (N + 1) ↔
      n = a ∨ reachesWithin c (step c n) a N := by
  constructor
  · rintro ⟨k, hk, heq⟩
    cases k with
    | zero =>
      left
      simpa [iter] using heq
    | succ j =>
      right
      refine ⟨j, by omega, ?_⟩
      simpa [iter] using heq
  · intro h
    rcases h with h0 | ⟨k, hk, heq⟩
    · refine ⟨0, by omega, ?_⟩
      simpa [iter] using h0
    · refine ⟨k + 1, by omega, ?_⟩
      simpa [iter] using heq

/-- Finite weighted hitting-mass identity, for any finite sample and weights.
This is not an assertion that a logarithmic-density potential exists. -/
theorem finite_hitting_mass (c a : ℤ) (N : ℕ)
    (sample : Finset ℤ) (w : ℤ → ℚ) :
    (∑ n ∈ sample, if reachesWithin c n a (N + 1) then w n else 0) =
      ∑ n ∈ sample, if n = a ∨ reachesWithin c (step c n) a N then w n else 0 := by
  classical
  apply Finset.sum_congr rfl
  intro n _
  rw [reachesWithin_succ_iff]

end CollatzBasins
