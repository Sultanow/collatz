import Mathlib

/-!
# Algebraic foundations of the accelerated 3x+c map

This is a deliberately small, integer-valued model.  For the maps considered in
our paper (odd c >= -1) positive inputs remain positive; the integer definition
also makes the mirror symmetry useful later.

No density theorem, stochastic approximation, or Collatz conjecture is assumed.
-/

namespace CollatzBasins

/-- Accelerated 3x+c map on integers. -/
def step (c n : ℤ) : ℤ :=
  if n % 2 = 0 then n / 2 else (3 * n + c) / 2

/-- The even predecessor of a target is always `2*a`. -/
theorem step_two_mul (c a : ℤ) : step c (2 * a) = a := by
  unfold step
  have h : (2 * a) % 2 = 0 := by omega
  simp only [ite_eq_left h]
  omega

/-- An odd input uses the odd branch of the accelerated map. -/
theorem step_of_odd (c n : ℤ) (hn : n % 2 = 1) :
    step c n = (3 * n + c) / 2 := by
  unfold step
  have h : ¬ n % 2 = 0 := by omega
  simp [h]

/-- Complete algebraic classification of the immediate predecessors of a target.
For odd c the second branch has an even numerator, so integer division is exact. -/
theorem step_eq_iff (c a n : ℤ) (hc : c % 2 = 1) :
    step c n = a ↔
      n = 2 * a ∨ (n % 2 = 1 ∧ 3 * n + c = 2 * a) := by
  unfold step
  by_cases hn : n % 2 = 0
  · simp only [ite_eq_left hn]
    constructor
    · intro h
      left
      omega
    · intro h
      rcases h with he | ⟨ho, _⟩
      · omega
      · omega
  · have ho : n % 2 = 1 := by omega
    simp only [ite_eq_right hn]
    constructor
    · intro h
      right
      constructor
      · exact ho
      · omega
    · intro h
      rcases h with he | ⟨_, hs⟩
      · omega
      · omega

/-- The only possible odd predecessor as an integer candidate. -/
def oddCandidate (c a : ℤ) : ℤ := (2 * a - c) / 3

/-- At most two immediate predecessors (filtered to ensure correctness). -/
def predecessors (c a : ℤ) : Finset ℤ :=
  ({2 * a, oddCandidate c a} : Finset ℤ).filter (fun n => step c n = a)

/-- The finite predecessor set is *exact*, not just an overapproximation. -/
theorem mem_predecessors_iff (c a n : ℤ) (hc : c % 2 = 1) :
    n ∈ predecessors c a ↔ step c n = a := by
  constructor
  · intro h
    exact (Finset.mem_filter.mp h).2
  · intro hs
    apply Finset.mem_filter.mpr
    constructor
    · have hcases := (step_eq_iff c a n hc).mp hs
      rcases hcases with he | ⟨_, hlin⟩
      · simp [he]
      · have ho : n = oddCandidate c a := by
          unfold oddCandidate
          omega
        simp [ho]
    · exact hs

/-- The map preserves positive inputs for the paper's parameter range. -/
theorem step_pos (c n : ℤ) (hc : -1 ≤ c) (hn : 0 < n) :
    0 < step c n := by
  unfold step
  by_cases he : n % 2 = 0
  · simp only [ite_eq_left he]
    omega
  · simp only [ite_eq_right he]
    omega

-- Small arithmetic regression checks (not substitutes for universal proofs).
example : step 1 1 = 2 := by decide
example : step 1 2 = 1 := by decide
example : step (-1) 5 = 7 := by decide
example : step (-1) 7 = 10 := by decide
example : step (-1) 10 = 5 := by decide

end CollatzBasins
