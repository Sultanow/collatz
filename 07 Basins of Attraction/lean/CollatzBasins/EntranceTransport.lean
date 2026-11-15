import Mathlib

/-!
Finite entrance-measure algebra. This file is a CI CANDIDATE until the
repository's pinned Lean/Mathlib build and kernel check are green.
It proves no uniform tail bound for deterministic Collatz trajectories.
-/

namespace CollatzBasins

/-- A normalized mass supported on `support` assigns that event full mass;
    the discrepancy against a uniform reference is explicit. -/
theorem supported_event_discrepancy
    (M : ℕ) (support : Finset (Fin M)) (p : Fin M → ℚ)
    (hnorm : (∑ x : Fin M, p x) = 1)
    (hzero : ∀ x : Fin M, x ∉ support → p x = 0) :
    (∑ x ∈ support, p x) - (support.card : ℚ) / (M : ℚ) =
      1 - (support.card : ℚ) / (M : ℚ) := by
  classical
  have hsum : (∑ x ∈ support, p x) = (∑ x : Fin M, p x) := by
    apply Finset.sum_subset (Finset.subset_univ support)
    intro x _ hx
    exact hzero x hx
  rw [hsum, hnorm]

/-- An event-level discrepancy bound transfers the reference escape mass. -/
theorem escape_mass_from_discrepancy
    (muGood nuGood eta epsilon : ℚ)
    (hreference : eta + epsilon ≤ nuGood)
    (hdiscrepancy : nuGood - muGood ≤ epsilon) :
    eta ≤ muGood := by
  linarith

/-- A positive escaped fraction yields contraction of a return-survival mass. -/
theorem survival_contraction_of_escape
    (pNow pLater escaped eta : ℚ)
    (hsubset : pLater ≤ pNow - escaped)
    (hescape : eta * pNow ≤ escaped) :
    pLater ≤ (1 - eta) * pNow := by
  nlinarith

end CollatzBasins
