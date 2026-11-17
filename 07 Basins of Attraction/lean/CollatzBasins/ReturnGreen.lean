import Mathlib

/-!
Finite second-moment algebra for return visits.
No probabilistic or asymptotic Collatz statements are formalized here.
Checked by the "Collatz Lean foundation" workflow (build and axiom audit).
-/

namespace CollatzBasins

/-- A finite path with `n` distinct ordered visits has square visit count
    equal to the sum of the successive odd numbers. -/
theorem return_square_eq_odd_sum (n : ℕ) :
    (∑ j ∈ Finset.range n, (2 * j + 1)) = n * n := by
  induction n with
  | zero => simp
  | succ n ih =>
      rw [Finset.sum_range_succ, ih]
      ring

/-- Weighted variant, valid for an arbitrary rational weight of the path. -/
theorem return_weighted_square_eq_odd_sum (n : ℕ) (w : ℚ) :
    w * (n : ℚ) ^ 2 =
      ∑ j ∈ Finset.range n, w * (2 * (j : ℚ) + 1) := by
  induction n with
  | zero => simp
  | succ n ih =>
      rw [Finset.sum_range_succ, ← ih]
      push_cast
      ring

/-- If occupation is positive but first-entry mass vanishes, exact balance
    rules out strict one-step contraction of that occupation weight. -/
theorem return_occupation_cannot_contract
    (firstMass returnMass occupation rho : ℚ)
    (hbalance : occupation - returnMass = firstMass)
    (hfirst : firstMass = 0)
    (hpositive : 0 < occupation)
    (hrho : rho < 1) :
    ¬ returnMass ≤ rho * occupation := by
  intro hcontract
  have heq : returnMass = occupation := by
    linarith
  have hgap : 0 < (1 - rho) * occupation :=
    mul_pos (sub_pos.mpr hrho) hpositive
  nlinarith

/-- The discounted occupation identity algebraically gives a sub-eigenmeasure.
    It says nothing about a scale-uniform upper bound on its total mass. -/
theorem return_discounted_potential_sub_eigen
    (firstMass returnMass discounted rho : ℚ)
    (hbalance : returnMass = rho * (discounted - firstMass))
    (hrho : 0 ≤ rho) (hfirst : 0 ≤ firstMass) :
    returnMass ≤ rho * discounted := by
  have hnonneg : 0 ≤ rho * firstMass := mul_nonneg hrho hfirst
  nlinarith

end CollatzBasins
