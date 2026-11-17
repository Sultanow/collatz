import CollatzBasins.Basic

/-!
# Arithmetic ingredients for the candidate mod-16 shell escape criterion

These theorems do NOT establish any distributional entrance estimate, any
uniform return moment, or T_X = o(X). They are auxiliary and not used in
the paper.
-/

namespace CollatzBasins

/-- For odd c, the special congruence 3*y+c=0 mod 16 forces odd y. -/
theorem mod16_escape_odd (c y : ℤ) (hc : c % 2 = 1)
    (hy : (3 * y + c) % 16 = 0) : y % 2 = 1 := by
  omega

/-- The unusual good residue is 5*c modulo 16. -/
theorem mod16_escape_residue (c y : ℤ)
    (hy : (3 * y + c) % 16 = 0) :
    y % 16 = (5 * c) % 16 := by
  omega

/-- Every number divisible by four makes two ordinary halving steps. -/
theorem mod16_escape_even_two_steps (c y : ℤ) (hy : y % 4 = 0) :
    step c (step c y) = y / 4 := by
  have he : y % 2 = 0 := by omega
  have he2 : (y / 2) % 2 = 0 := by omega
  have hs1 : step c y = y / 2 := by simp [step, he]
  have hs2 : step c (y / 2) = (y / 2) / 2 := by simp [step, he2]
  rw [hs1, hs2]
  omega

/-- An increasing return edge forbids a nondecreasing additive potential. -/
theorem no_nondecreasing_return_potential
    (U : ℤ → ℚ) (y z : ℤ)
    (hyz : y < z) (hmono : ∀ x₁ x₂, x₁ ≤ x₂ → U x₁ ≤ U x₂)
    (hstep : 1 + U z ≤ U y) : False := by
  have h := hmono y z (le_of_lt hyz)
  linarith

/-- A decreasing return edge forbids a nonincreasing additive potential. -/
theorem no_nonincreasing_return_potential
    (U : ℤ → ℚ) (y z : ℤ)
    (hyz : z < y) (hmono : ∀ x₁ x₂, x₁ ≤ x₂ → U x₂ ≤ U x₁)
    (hstep : 1 + U z ≤ U y) : False := by
  have h := hmono z y (le_of_lt hyz)
  linarith

end CollatzBasins
