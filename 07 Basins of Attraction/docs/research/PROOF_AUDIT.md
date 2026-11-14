# Proof audit and next research steps

**Source**: `/mnt/data/main_revised.tex` (the latest revised manuscript from this conversation, immediately before the audit).

**Scope**: Lemma 2 (`lem:reduction`), Lemma 3 (`lem:valuations`), Lemma 4 (`lem:model`), Theorem 2 (`thm:lower`), Corollary 6 (`cor:remains`), and a proposed next route towards the missing upper bound.

**Status**: Targeted independent checks and a constructive proof expansion, *not* a claim of complete independent verification of Theorem 2.

## Findings by source location

- **Lemma 2, around source lines 1149–1168**: The reduction uses the uniform adjacent-window comparison for a bounded test function and the only needed properties of the indicator in Steps 4–6 are boundedness and the window-average identity. The argument is consistent *provided* the quantitative first-passage estimate inherited from GGM is uniform in the chosen window parameters and the bounded test function. Verify those uniformities in the exact GGM statement. The lemma's function may depend on the target cutoff X; this is why uniformity matters.

- **Lemma 3, lines 1170–1182**: The block sampling argument does produce the correct geometric valuation law and the stated O(2^k/u) total variation order. The important probabilistic construction requirement is that the final U is obtained from U' using a coupling depending on U' only, with independent auxiliary randomness; do *not* couple U using N or the residue rho, or its independence from the geometric sequence will be lost. The N marginal can be corrected independently using N'.

- **Lemma 4(i), line 1201**: The increments \(\xi=g\log 2-\log 3\) have positive mean \(\mu_0=\log(4/3)\), bounded below, and exponential moments. Wald and the overshoot bound of Lorden yield \(\mathbb E T_t=t/\mu_0+O(1)\) after verifying integrability (Chernoff implies exponential tail for the hitting time at fixed t). Lorden (1970), *On Excess Over the Boundary* is the primary source.

- **Lemma 4(ii) and (b), around lines 1202 and 1234**: The original manuscript compresses an important expected-visit / second-moment argument into one sentence. `lemma4_visit_L2.patch` gives a more explicit proof. For a fixed unit interval, begin at its first visit J; all subsequent visits imply \(D'_i<1\), and \(\mathbb P(D'_i<1)\le e^{1/2}\rho^i\). This bounds both L1 and L2 counts. Cover longer intervals by units. The first-crossing valuation has uniformly bounded L2 via \(\sum v^2 2^{-v}C(1+v)\). Finally apply Minkowski and the Chernoff series to obtain \(\sup_q\mathbb E N_q^2<\infty\). This expands rather than changes the proposed lemma.

- **Lemma 4(a), lines 1205–1232**: The identity \(\mathbb E m_j=2p_r\) is consistent with geometric memorylessness and the 3-adic Syracuse law. The marked renewal computation requires careful attention to the n-1 boundary terms, but its order O(1) follows from the independence of \(m_j\) from \(\{T\ge j-n+1\}\). The supplied patch also expands the **first-return term**: the downward crossing has g=1 (the only negative increment), so its overshoot is bounded by log(3/2); the remaining return blocks are handled by strong Markov and the uniform visit bound. This is a proposed expansion for reviewer scrutiny, not a separately published theorem.

- **Lemma 4(c), line 1236**: \(j_0=\lceil 8\Delta\rceil\) works because \(8\mu_0>2\), and Chernoff controls both the lower deviation of D and the upper deviation of the geometric sum. The numerical inequality \(3j_0\le24\Delta+3\) holds.

- **Theorem 2, lines 1240–1282**: The lower-bound direction relies only on the *good coupling* event. This is a structural advantage: it does not require estimating the actual Collatz counting functional on the coupling failure event. The potential upper bound would require more (see below). For a complete audit, confirm every $X$-dependent parameter is in the same admissible quantitative GGM regime, particularly the uniform constants in Lemma 2 and the first-passage estimate (16). Independently inspect the model-to-orbit comparison at \(\tau^*,\tau^{**}\), not just the probabilistic lemmas.

- **Corollary 6(b), lines 1287 onwards**: The additional inference for c=+1 and c=-1 is conditional on the published/preprint predecessor-positivity results invoked through Corollary 5. It is not a standalone new proof of the Collatz conjecture. If an unbounded orbit begins at \(m_0\) and \(h(m_0)>0\), its forward orbit eventually contains an element \(p\in(X/3,X]\) for each sufficiently large X; the orbit sum receives an extra contribution on all starting values in \(P(m_0)\). This supplies the qualitative contradiction with a sharp mean upper bound.

## Exact residual decomposition – new candidate

`candidate_residual_section.tex` gives a rigorous **conditional-on-the-current-lemmas** proposition. With \(W_X=(X^{1-\beta},X]\) and \(\Phi_X\) the first-passage block contribution in Theorem 2, define

\[
F_X=\lim_{N\to\infty}\frac1{\log N}\sum_{w<N}\frac{\Phi_X(w^\flat)}w,
\]

\[
R_X=\lim_{N\to\infty}\frac1{\log N}\sum_{w<N}\frac{\sum_{a\le X,\,a\in\mathrm{orb}(w)}a-\Phi_X(w^\flat)}w.
\]

Then the potential and reduction lemma yield

\[
\boxed{\quad S_X:=\sum_{a\le X}a h(a)=F_X+R_X,\quad R_X\ge0.\quad}
\]

Theorem 2's existing proof gives \(F_X\ge\gamma_0 X-o(X)\), where \(\gamma_0=2/\log(4/3)\). An upper bound needs **two** new inputs:

1. **First-passage calibration:** \(F_X\le\gamma_0 X+o(X)\). This is a *new unproved* upper bound on the full, untruncated functional.
2. **Negligible residual mass:** \(R_X=o(X)\). This rules out a macroscopic mass from small values, later returns, and long excursions.

The exact identity is conceptually useful even without either new bound. If the first calibration is obtained, \(R_X=o(X)\) is equivalent to the paper's missing upper-mean bound.

**Important technical difficulty:** The available coupling has exceptional-event probability \(X^{-\delta}\), but the untruncated first-passage functional can be as large as \(O(X^2)\). This cannot be bounded by simply multiplying the exceptional probability by a deterministic maximum. An additional *uniform integrability* or direct exceptional-orbit estimate is necessary.

## Small exact reference computation

`python syracuse_exact.py --max-n 5 --print-distribution`

The calculation implements the identity in law

\[
S_n \stackrel d=2^{-G}(1+3S_{n-1})\pmod {3^n}
\]

with geometrically distributed G, and uses that 2 has multiplicative order \(2\cdot 3^{n-1}\) modulo \(3^n\). It verifies Tao's reference results without simulation:

- n=1: denominator 3, numerator vector `[0, 1, 2]`.
- n=2: denominator 63, numerator vector `[0, 8, 16, 0, 11, 4, 0, 2, 22]`.

The exact-arithmetic routine is intended for *small* n, not a claim that n=22 is feasible with this representation. Scaling to large moduli will require numerical or compact representation and independent error checks.

## Do next

1. Review the supplied Lemma 4 patch, now including the first-return, visitation and second-moment expansions, line by line with an external mathematical referee.
2. Audit the GGM constants and parameter ranges against the exact published statement, including estimate (16), rather than citing the proof informally.
3. Attempt a **two-sided** comparison for truncated first-passage functionals and quantify the truncation tail. Use the new F_X + R_X decomposition to state exactly what remains.
4. Validate the small 3-adic law with Tao, then engineer a higher-n implementation to compare predicted residue profiles. Maintain independent checks against deterministic orbit counts.

## Literature checked

- G. Lorden, *On Excess Over the Boundary*, Annals of Mathematical Statistics 41 (1970), 520–527, DOI: 10.1214/aoms/1177697092.
- T. Tao, *Almost all orbits of the Collatz map attain almost bounded values*, Forum of Mathematics, Pi 10 (2022), e12, especially the 3-adic Syracuse distribution and Lemma 1.12.
- Gonçalves–Greenfeld–Madrid: the manuscript's cited first-passage proposition and its proof; the exact parameter-uniformity check remains listed as a separate verification item.
