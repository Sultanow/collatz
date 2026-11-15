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