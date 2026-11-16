import CollatzBasins.ReturnLyapunov

/-!
# A fixed-modulus return inside every sufficiently large real dyadic shell

This module targets Proposition `prop:residue-obstruction` of the manuscript,
not a statistical return-moment statement.  In particular the quantifier
`∀ X : ℝ, X₀ ≤ X` is essential: checking isolated shell sizes is insufficient.

The elementary interval lemma below is also useful independently.
-/

namespace CollatzBasins

/-- An open-left, closed-right real interval of length greater than a
positive integer `m` contains a multiple of `m`. -/
theorem multiple_in_real_interval
    (m : ℤ) (hm : 0 < m) (L U : ℝ)
    (hwidth : L + (m : ℝ) < U) :
    ∃ A : ℤ, m ∣ A ∧ L < (A : ℝ) ∧ (A : ℝ) ≤ U := by
  have hmR : (0 : ℝ) < (m : ℝ) := by exact_mod_cast hm
  let w : ℝ := U / (m : ℝ)
  let n : ℤ := ⌊w⌋
  let A : ℤ := m * n
  have hfl : (n : ℝ) ≤ w := by
    simpa only [n] using (Int.floor_le w)
  have hfu : w < (n : ℝ) + 1 := by
    simpa only [n] using (Int.lt_floor_add_one w)
  have hw : w * (m : ℝ) = U := by
    dsimp [w]
    field_simp [ne_of_gt hmR]
  have hAcast : (A : ℝ) = (m : ℝ) * (n : ℝ) := by
    simp only [A, Int.cast_mul]
  have hAle : (A : ℝ) ≤ U := by
    have h := mul_le_mul_of_nonneg_left hfl (le_of_lt hmR)
    rw [hAcast]
    nlinarith [hw]
  have hAgt : U - (m : ℝ) < (A : ℝ) := by
    have h := mul_lt_mul_of_pos_left hfu hmR
    rw [hAcast]
    nlinarith [hw]
  refine ⟨A, ?_, ?_, hAle⟩
  · exact ⟨n, rfl⟩
  · linarith

/-- The precise all-large-real-shell statement of the fixed-modulus
obstruction (Proposition 11).  The final divisibility assertion expresses
congruence modulo `2^k`.  The parameter `k = 0` is also allowed. -/
theorem fixed_modulus_obstruction_all_large
    (c : ℤ) (hc : c % 2 = 1) (k : ℕ) :
    ∃ X₀ : ℝ, ∀ X : ℝ, X₀ ≤ X →
      ∃ y z : ℤ,
        0 < y ∧
        X / 2 < (y : ℝ) ∧ (y : ℝ) ≤ X ∧
        X / 2 < (z : ℝ) ∧ (z : ℝ) ≤ X ∧
        step c y = z ∧ (2 : ℤ) ^ k ∣ z - y := by
  let m : ℤ := (2 : ℤ) ^ k
  have hm : 0 < m := by
    dsimp [m]
    positivity
  have hmR : (0 : ℝ) < (m : ℝ) := by exact_mod_cast hm
  refine ⟨24 * (m : ℝ) + 12 * (|c| : ℝ) + 24, ?_⟩
  intro X hX
  have hcplus : (c : ℝ) ≤ (|c| : ℝ) := by
    exact_mod_cast (le_abs_self c)
  have hcminus : -(|c| : ℝ) ≤ (c : ℝ) := by
    exact_mod_cast (neg_abs_le c)
  have hXpos : (0 : ℝ) < X := by
    linarith
  have hwidth : X / 4 + (c : ℝ) / 2 + (m : ℝ) <
      (X + (c : ℝ)) / 3 := by
    linarith
  obtain ⟨A, hdiv, hlow, hhigh⟩ :=
    multiple_in_real_interval m hm
      (X / 4 + (c : ℝ) / 2) ((X + (c : ℝ)) / 3) hwidth
  have hAposR : (0 : ℝ) < (A : ℝ) := by
    linarith
  have hApos : 0 < A := by exact_mod_cast hAposR
  let y : ℤ := 2 * A - c
  let z : ℤ := 3 * A - c
  have hycast : (y : ℝ) = 2 * (A : ℝ) - (c : ℝ) := by
    dsimp [y]
    push_cast
    ring
  have hzcast : (z : ℝ) = 3 * (A : ℝ) - (c : ℝ) := by
    dsimp [z]
    push_cast
    ring
  have hylo : X / 2 < (y : ℝ) := by
    rw [hycast]
    linarith
  have hzup : (z : ℝ) ≤ X := by
    rw [hzcast]
    linarith
  have hyz : y < z := by
    dsimp [y, z]
    omega
  have hyzR : (y : ℝ) < (z : ℝ) := by exact_mod_cast hyz
  have hypos : 0 < y := by
    have hr : (0 : ℝ) < (y : ℝ) := by linarith
    exact_mod_cast hr
  have hyup : (y : ℝ) ≤ X := by linarith
  have hzlo : X / 2 < (z : ℝ) := by linarith
  have hstep : step c y = z := by
    dsimp [y, z]
    exact return_same_residue_arithmetic c A hc
  have hcong : (2 : ℤ) ^ k ∣ z - y := by
    change m ∣ z - y
    dsimp [y, z]
    exact return_same_residue_divisor c A m hdiv
  exact ⟨y, z, hypos, hylo, hyup, hzlo, hzup, hstep, hcong⟩

/-- A real all-large-shell impossibility theorem for potentials that only
see a fixed binary residue: strict descent across every one-step shell edge
contradicts residue invariance. -/
theorem no_fixed_residue_strict_descent_all_large
    (c : ℤ) (hc : c % 2 = 1) (k : ℕ) :
    ∃ X₀ : ℝ, ∀ X : ℝ, X₀ ≤ X → ∀ U : ℤ → ℚ,
      (∀ y z : ℤ, (2 : ℤ) ^ k ∣ z - y → U y = U z) →
      ¬ (∀ y z : ℤ,
        X / 2 < (y : ℝ) → (y : ℝ) ≤ X →
        X / 2 < (z : ℝ) → (z : ℝ) ≤ X →
        step c y = z → 1 + U z ≤ U y) := by
  obtain ⟨X₀, hlarge⟩ := fixed_modulus_obstruction_all_large c hc k
  refine ⟨X₀, ?_⟩
  intro X hX U hperiod hdescent
  obtain ⟨y, z, _, hylo, hyup, hzlo, hzup, hstep, hmod⟩ := hlarge X hX
  have heq : U y = U z := hperiod y z hmod
  have hdec : 1 + U z ≤ U y := hdescent y z hylo hyup hzlo hzup hstep
  linarith

end CollatzBasins
