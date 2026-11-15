import Mathlib

/-!
# Finite return-pair counting

For a trajectory with `n` recorded visits to a shell, the number of
chronologically ordered pairs of distinct visits is `0 + ... + (n-1)`.
These are finite combinatorial statements only.  They do not imply any
probabilistic tail estimate for a Collatz orbit.
-/

namespace CollatzBasins

/-- Count pairs of visits by charging each later visit for all earlier ones. -/
def returnPairCount (visits : ℕ) : ℕ :=
  (Finset.range visits).sum (fun i => i)

/-- Appending one visit creates precisely one pair with each earlier visit. -/
theorem returnPairCount_succ (n : ℕ) :
    returnPairCount (n + 1) = returnPairCount n + n := by
  simp [returnPairCount, Finset.sum_range_succ]

/-- The exact finite identity behind second-moment return-pair arguments. -/
theorem visit_square_eq_visits_add_double_pairs (n : ℕ) :
    n * n = n + 2 * returnPairCount n := by
  induction n with
  | zero => decide
  | succ n ih =>
      change (n + 1) * (n + 1) = (n + 1) + 2 * returnPairCount (n + 1)
      rw [returnPairCount_succ]
      nlinarith [ih]

end CollatzBasins
