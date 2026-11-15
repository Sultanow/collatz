# Shell-local-time diagnostics

These outputs are from finite intervals of starting values, weighted by `1/m` over the selected odd integers; they are **not** estimates of the limiting logarithmic law and certify no asymptotic theorem.

The script `C/tests/shell_visit_probe.py` reproduces all files; the parameters used in each run are recorded in its JSON output. Small runs use `X=10^4,10^5` with 1800 starts in `[20X,24X)`. Larger exploratory runs use `X=10^60` with 500 starts and `X=10^120` with 300 starts, in the same relative interval. The last two windows are too small and too close to `X` to stand in for the random logarithmic-start law of Theorem 2.

All sampled trajectories in these runs reached the first-passage threshold. Each resolved sample passed the deterministic dyadic shell bound `Phi_X/X <= sum 2^(-j) N_j`, and the corresponding finite weighted second-moment inequality was checked.

Files are useful for regression testing, *not* for claiming a uniform-in-X bound on the actual Syracuse shell local time.
