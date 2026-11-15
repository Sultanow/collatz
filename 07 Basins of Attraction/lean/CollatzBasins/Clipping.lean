import Mathlib

/-!
# Elementary clipping algebra (candidate for Lean CI review)

This file is NOT certified until `lake env lean CollatzBasins/Clipping.lean`
passes on the repository's pinned toolchain and the CI dependency audit runs.
It proves only finite algebraic inequalities; nothing about actual Collatz
orbits, logarithmic density, or the open local-time moment bound.
-/

namespace CollatzBasins

/-- Every mass equals its clipped part plus its nonnegative excess. -/
theorem clipping_identity (mass cutoff : ℚ) :
    mass = min mass cutoff + max (mass - cutoff) 0 := by
  rcases le_total mass cutoff with h | h
  · have hn : mass - cutoff ≤ (0 : ℚ) := by linarith
    rw [min_eq_left h, max_eq_right hn]
    ring
  · have hn : (0 : ℚ) ≤ mass - cutoff := by linarith
    rw [min_eq_right h, max_eq_left hn]
    ring

/-- A finite mass's clipping excess is bounded by a quadratic moment. -/
theorem clipping_excess_le_quadratic (mass cutoff : ℚ)
    (hm : 0 ≤ mass) (hc : 0 < cutoff) :
    max (mass - cutoff) 0 ≤ (mass * mass) / cutoff := by
  by_cases h : mass ≤ cutoff
  · have hn : mass - cutoff ≤ (0 : ℚ) := by linarith
    rw [max_eq_right hn]
    exact div_nonneg (mul_nonneg hm hm) (le_of_lt hc)
  · have hlarge : cutoff ≤ mass := le_of_lt (lt_of_not_ge h)
    have hmult : cutoff * mass ≤ mass * mass :=
      mul_le_mul_of_nonneg_right hlarge hm
    have hmass : mass ≤ (mass * mass) / cutoff := by
      apply (le_div_iff₀ hc).2
      nlinarith
    have hpos : (0 : ℚ) ≤ mass - cutoff := by linarith
    rw [max_eq_left hpos]
    linarith

end CollatzBasins
