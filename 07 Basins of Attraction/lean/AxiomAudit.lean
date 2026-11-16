import CollatzBasins

-- Dependency reports for the theorems that this small project actually proves.
#print axioms CollatzBasins.step_two_mul
#print axioms CollatzBasins.step_of_odd
#print axioms CollatzBasins.step_eq_iff
#print axioms CollatzBasins.mem_predecessors_iff
#print axioms CollatzBasins.step_pos
#print axioms CollatzBasins.reachesWithin_succ_iff
#print axioms CollatzBasins.finite_hitting_mass

-- Finite clipping algebra (new candidate; CI must certify).
#print axioms CollatzBasins.clipping_identity
#print axioms CollatzBasins.clipping_excess_le_quadratic

#print axioms CollatzBasins.returnPairCount_succ
#print axioms CollatzBasins.visit_square_eq_visits_add_double_pairs

#print axioms CollatzBasins.supported_event_discrepancy
#print axioms CollatzBasins.escape_mass_from_discrepancy
#print axioms CollatzBasins.survival_contraction_of_escape

#print axioms CollatzBasins.return_square_eq_odd_sum
#print axioms CollatzBasins.return_weighted_square_eq_odd_sum
#print axioms CollatzBasins.return_occupation_cannot_contract
#print axioms CollatzBasins.return_discounted_potential_sub_eigen

#print axioms CollatzBasins.return_lyapunov_path
#print axioms CollatzBasins.return_lyapunov_weighted
#print axioms CollatzBasins.return_same_residue_arithmetic
#print axioms CollatzBasins.return_same_residue_divisor
#print axioms CollatzBasins.return_same_residue_dyadic

#print axioms CollatzBasins.mod16_good_odd_class
#print axioms CollatzBasins.mod16_good_even_classes
#print axioms CollatzBasins.mod16_good_periodic