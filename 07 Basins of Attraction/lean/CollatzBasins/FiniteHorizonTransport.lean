import CollatzBasins.Mod16Escape

/-!
# Elementary finite-horizon transport ingredients

Only elementary congruence facts are formalized here: the escape residues
modulo 16 (one odd class and the multiples of four). They are auxiliary and
not used in the paper. The finite-horizon residue balance of the paper
(lemma "Finite-horizon residue balance") is proved on paper and is not
formalized.
-/
namespace CollatzBasins

/-- The exceptional (odd) escape class is exactly `5*c` modulo 16. -/
theorem mod16_good_odd_class (c y : ℤ)
    (h : (3 * y + c) % 16 = 0) :
    y % 16 = (5 * c) % 16 := by
  omega

/-- The four even escape residues are precisely the multiples of four. -/
theorem mod16_good_even_classes (y : ℤ) :
    y % 4 = 0 ↔
    (y % 16 = 0 ∨ y % 16 = 4 ∨ y % 16 = 8 ∨ y % 16 = 12) := by
  omega

/-- The escape predicate is periodic modulo 16. -/
theorem mod16_good_periodic (c y : ℤ) :
    (y % 4 = 0 ∨ (3 * y + c) % 16 = 0) ↔
    ((y + 16) % 4 = 0 ∨ (3 * (y + 16) + c) % 16 = 0) := by
  omega

end CollatzBasins
