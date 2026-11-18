# The correct next research target

Instead of the known sparse parity superset or the survival-gated sparse parity superset, study the actual *return-certified* states

    A_{I,b}={y in I: N_I^+(y) >= b}

under the stopped logarithmic predecessor profile. The new bound asks for

    X^{-1} sum_j sum_{y in A_{I_{X,j},b_X}} y h_X^downarrow(y) = o(1),

with `b_X` increasing but `b_X * mu_X(L_X) -> 0`.

This is sufficient, **not proved**, and may itself be too strong. The exact return-certificate identity makes clear that merely high odd-step density or initial survival cannot supply the bound. Any next positive attempt must use information about the actual distribution of subsequent visits, or an independent quantitative bound for the weighted count of multi-return states. Avoid promoting this reduction into the main paper as a new asymptotic theorem before that input is secured.
