import CollatzBasins.Basic

/-!
Finite certificates for moment bounds in a terminating deterministic return graph.
They do not establish any uniform estimate for Collatz/Syracuse return visits.
-/

namespace CollatzBasins

universe u

/-- The next-return transfer operator, acting on a function. -/
def returnNextValue {α : Type u} (R : α → Option α) (F : α → ℚ)
    (x : α) : ℚ :=
  match R x with
  | none => 0
  | some y => F y

/-- A witness that the next-return chain terminates after precisely n visits. -/
inductive FinishedReturnPath {α : Type u} (R : α → Option α) : α → ℕ → Prop where
  | terminal (x : α) (hx : R x = none) : FinishedReturnPath R x 1
  | next (x y : α) (n : ℕ) (hx : R x = some y)
      (htail : FinishedReturnPath R y n) : FinishedReturnPath R x (n + 1)

/-- Two local inequalities certify the first and second moment along every
    terminating return path, without a geometric-return assumption. -/
theorem return_lyapunov_path {α : Type u}
    (R : α → Option α) (U V : α → ℚ)
    (hU : ∀ x, 1 + returnNextValue R U x ≤ U x)
    (hV : ∀ x, 1 + 2 * returnNextValue R U x +
      returnNextValue R V x ≤ V x)
    {x : α} {n : ℕ} (hp : FinishedReturnPath R x n) :
    (n : ℚ) ≤ U x ∧ (n : ℚ) ^ 2 ≤ V x := by
  induction hp with
  | terminal x hx =>
      constructor
      · simpa [returnNextValue, hx] using hU x
      · simpa [returnNextValue, hx] using hV x
  | next x y n hx htail ih =>
      have hUx : 1 + U y ≤ U x := by
        simpa [returnNextValue, hx] using hU x
      have hVx : 1 + 2 * U y + V y ≤ V x := by
        simpa [returnNextValue, hx] using hV x
      have hcast : (↑(n + 1) : ℚ) = (n : ℚ) + 1 := by push_cast; ring
      constructor
      · rw [hcast]
        linarith [ih.1]
      · rw [hcast]
        nlinarith [ih.1, ih.2]

/-- A weighted finite law: the second return-count moment is bounded by
    the initial law's average of the second Lyapunov certificate. -/
theorem return_lyapunov_weighted {α : Type u}
    (R : α → Option α) (U V : α → ℚ)
    (hU : ∀ x, 1 + returnNextValue R U x ≤ U x)
    (hV : ∀ x, 1 + 2 * returnNextValue R U x +
      returnNextValue R V x ≤ V x)
    (sample : Finset α) (w : α → ℚ)
    (hw : ∀ x ∈ sample, 0 ≤ w x)
    (N : α → ℕ)
    (hpaths : ∀ x ∈ sample, FinishedReturnPath R x (N x)) :
    (∑ x ∈ sample, w x * (N x : ℚ) ^ 2) ≤
      ∑ x ∈ sample, w x * V x := by
  apply Finset.sum_le_sum
  intro x hx
  have hb : (N x : ℚ) ^ 2 ≤ V x :=
    (return_lyapunov_path R U V hU hV (hpaths x hx)).2
  exact mul_le_mul_of_nonneg_left hb (hw x hx)

/-- Exact arithmetic one-step return for odd c. -/
theorem return_same_residue_arithmetic (c A : ℤ) (hc : c % 2 = 1) :
    step c (2 * A - c) = 3 * A - c := by
  have ho : (2 * A - c) % 2 = 1 := by omega
  rw [step_of_odd c (2 * A - c) ho]
  omega

/-- The two explicit consecutive values agree modulo every divisor of A. -/
theorem return_same_residue_divisor (c A m : ℤ) (h : m ∣ A) :
    m ∣ (3 * A - c) - (2 * A - c) := by
  have heq : (3 * A - c) - (2 * A - c) = A := by ring
  rw [heq]
  exact h

/-- A convenient interval certificate for the return pair in
    the dyadic shell (X/2,X] where X = 3*A + |c|. -/
theorem return_same_residue_dyadic (c A : ℤ) (hA : 3 * |c| < A) :
    let X : ℤ := 3 * A + |c|
    X < 2 * (2 * A - c) ∧
    2 * (2 * A - c) ≤ 2 * X ∧
    X < 2 * (3 * A - c) ∧
    2 * (3 * A - c) ≤ 2 * X := by
  dsimp
  have hp : c ≤ |c| := le_abs_self c
  have hn : -|c| ≤ c := neg_abs_le c
  omega

end CollatzBasins
