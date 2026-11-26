# 07 Basins of Attraction

[![Preprint](https://img.shields.io/badge/Preprints.org-202610.2552-orange)](https://www.preprints.org/manuscript/202610.2552)
[![DOI](https://img.shields.io/badge/DOI-10.20944%2Fpreprints202610.2552.v1-blue)](https://doi.org/10.20944/preprints202610.2552.v1)
[![License: CC BY 4.0](https://img.shields.io/badge/Paper-CC%20BY%204.0-lightgrey)](https://creativecommons.org/licenses/by/4.0/)

> **Paper:** E. Sultanow, *Same Shadows, Different Forests: Local Predecessor Structure and Global Dynamics of Collatz-Type Maps*. Preprints (2026), [doi:10.20944/preprints202610.2552.v1](https://doi.org/10.20944/preprints202610.2552.v1).
> **Read it on [Preprints.org](https://www.preprints.org/manuscript/202610.2552).** The code in this folder reproduces all computations of the paper.

Same local trees, different forests. This section studies maps of the form

$$T_{q,c}(x)=\begin{cases}(qx+c)/2 & x \text{ odd},\\ x/2 & x \text{ even},\end{cases}\qquad q,c \text{ odd},$$

which include the Collatz map ($q=3$, $c=1$), its mirror $3x-1$ ($c=-1$, i.e. $3x+1$ on the negative integers), the maps $3x+d$ and Crandall's $qx+1$. Locally, all of them generate predecessor trees with the same congruence-class statistics. Globally, they differ: $3x+1$ is conjectured to have a single tree (basin), $3x-1$ has three, and $3x+5$ has six.

Four programs make the global side computable:

* `census_all` lists, in **one pass** over all parity words, the primitive cycles of **all** maps $3x+c$ with $|c| \le$ `DMAX` (Lagarias' rational cycles of $3x+1$ with denominator at most `DMAX`).
* `census` does the same for a single map $T_{q,c}$.
* `basin` measures how the positive integers split into the **basins of attraction** of these cycles.
* `passage` samples the basin shares **far beyond enumeration** (windows up to $10^{220}$). It follows each orbit to its first passage below a table bound; a basin is not changed by first passage (Theorem 1 of the paper).

The code accompanies the paper [*Same Shadows, Different Forests*](https://www.preprints.org/manuscript/202610.2552) (LaTeX sources in [docs](docs)). The rotating-binaries background is in [04 Cycles](../04%20Cycles) (notebooks `eldar_parityvector_k1.ipynb` and `eldar_rotate.ipynb`); a brute-force search over many $q$ is Arty's program in [04 Cycles/CPP](../04%20Cycles/CPP).

## Paper

**Same Shadows, Different Forests: Cycles and Basin Densities of Collatz-Type Maps**
Eldar Sultanow · Preprints.org, version 3, November 2026 (not peer-reviewed)

**[Preprints.org](https://www.preprints.org/manuscript/202610.2552)** · [DOI 10.20944/preprints202610.2552.v3](https://doi.org/10.20944/preprints202610.2552.v3) · version 1 (26 October 2026, subtitle *Local Predecessor Structure and Global Dynamics of Collatz-Type Maps*): [DOI …v1](https://doi.org/10.20944/preprints202610.2552.v1) · version 2: [DOI …v2](https://doi.org/10.20944/preprints202610.2552.v2)

Current draft after version 3: [PDF](docs/main.pdf) · [LaTeX sources](docs) · [changes](docs/CHANGES_v5.md) · [research notes](docs/research) (working material, not part of the paper). Table 11 of the paper lists the computational runs A–K with links to their data in `results/`; Table 12 and the [reproduction map](#reproduction-map) below list programs and files.

Build: `cd docs && pdflatex main && bibtex main && pdflatex main && pdflatex main`

#### Citation

```bibtex
@article{sultanow2026shadows,
  title   = {Same Shadows, Different Forests: Cycles and Basin Densities
             of Collatz-Type Maps},
  author  = {Sultanow, Eldar},
  journal = {Preprints},
  year    = {2026},
  month   = nov,
  note    = {Version 3},
  doi     = {10.20944/preprints202610.2552.v3},
  url     = {https://www.preprints.org/manuscript/202610.2552}
}
```

## Structure

```
07 Basins of Attraction/
├── README.md
├── docs/                      working paper (LaTeX sources and PDF)
├── figures/                   scripts for the two figures of the paper
├── oeis/                      cross-check of the census with OEIS A226607
├── lean/                      Lean 4 proofs of the finite statements (see "Lean" below)
├── C/
│   ├── census_all.c           all maps 3x+c, |c| <= DMAX, in one pass (Run A)
│   ├── mitm_census.cpp        the same census by meet-in-the-middle, one type (l,k) at a time (Run P)
│   ├── census.c               cycle census for a single map
│   ├── basin.c                basin counts for given cycles (Run B)
│   ├── passage.c              basin shares far beyond enumeration, via first passage (Run C)
│   ├── merge_census_all.py    merge census_all shards into one CSV
│   ├── run_P_tasks.py         task list of Run P, the most expensive types first
│   ├── run_P_report.py        checks and merges Run P, census against the heuristic
│   ├── heuristic_P.py         the cycle heuristic up to l = 66: boundary effect, birthday factor (Run P, Problem 1)
│   ├── cycles_for_map.py      cycles of one map from that CSV (input for basin)
│   ├── merge_census.py        merge census outputs
│   ├── merge_basin.py         merge basin outputs, window shares
│   ├── merge_passage.py       merge passage outputs, pooled shares, replicate check
│   ├── local_structure.py     residue classes of 3x+1 and 3x-1 (Tables 2 and 3)
│   ├── landscape.py           basin landscape of all maps 3x+c, c <= 400 (Run D)
│   ├── residue_flatness.py    basin shares conditional on residue classes (Run E)
│   ├── backward_mass.c        harmonic mass of the backward tree of a cycle (Run F)
│   ├── predictor_eval.py      backward-mass predictor vs. measured basin shares (Run F)
│   ├── cosets.py              cosets of <2,3> mod c against the measured shares (Prop. 6)
│   ├── potential.c            predecessor counts for all targets a <= A at once (Run G)
│   ├── potential_report.py    identities of the predecessor potential, mean value, Syracuse profile
│   ├── orbit_profile.c        the same means at large scales from random orbits (Run H)
│   ├── orbit_profile_report.py  decade means and Syracuse profile of Run H
│   ├── tail_check.c           first-passage mass Phi_X/X: actual orbits vs. random-walk model (Run I)
│   ├── tail_fallback.py       exact big-integer values for the starts that leave 128 bits (Run I)
│   ├── tail_grid.sh           all configurations of Run I and the report
│   ├── tail_report.py         actual vs. model: moments, tail probabilities, z-scores (Run I)
│   ├── word_census.c          all parity words up to length 30: reduced denominators (Run J)
│   ├── necklace_counts.py     primitive-cycle counts per type from word_census, checked against the census (Run J)
│   ├── algebraic_factor.c     divisibility of aperiodic words by 2^{l/2}+3^{k/2} (Run J)
│   ├── heuristic_tests.py     where the equidistribution heuristic for cycles fails (Run J)
│   ├── run_J.sh               all of Run J and the report
│   ├── ui_report.py           tails of the potential a*h(a) against the Syracuse profile (Run K)
│   ├── run_K.sh               submits Run K (counts with potential, then ui_report.py)
│   ├── Makefile               make (incl. backward_mass, potential, tail_check), make test, make passage,
│   │                          make test-passage, make test-potential, make test-tail, make test-ui,
│   │                          make mitm_census, make test-mitm
│   ├── tests/                 independent reference and self-tests
│   └── slurm/
│       ├── census_all_array.sbatch
│       ├── run_P.sbatch
│       ├── census_array.sbatch
│       ├── basin_array.sbatch
│       ├── submit_basins.sh
│       ├── passage_array.sbatch
│       ├── passage_configs.txt
│       ├── landscape_configs.txt
│       ├── residue_configs.txt
│       ├── residue9_configs.txt
│       ├── backward_mass_array.sbatch
│       ├── potential_array.sbatch
│       ├── potential_configs.txt
│       ├── run_K_counts.sbatch
│       └── run_K_report.sbatch
└── results/                   census outputs, rational cycle list, basin counts
    ├── passage/               sampled basin shares (Run C)
    ├── tail/                  first-passage mass Phi_X/X, actual vs. model (Run I)
    ├── heuristic/             diagnostics of the cycle-count heuristic (Run J)
    ├── run_P/                 census for word lengths 41..66 by meet-in-the-middle (Run P): report, merged list l <= 66, raw outputs
    ├── run_P_probe/           local probe of Run P, word lengths 41..48
    └── potential_ui/          tails of the potential a*h(a), Syracuse profile, dangerous classes (Run K)
```

## Background

A parity word $B=b_0b_1\cdots b_{l-1}$ with $k$ ones has *type* $(l,k)$. With $D=2^l-q^k$ and

$$z_q(B)=\sum_{j=1}^{k} q^{\,k-j}\,2^{p_j}\qquad(p_1<\dots<p_k \text{ the positions of the ones, position } 0 = \text{first step}),$$

the number $x_B=c\,z_q(B)/D$ is the unique periodic point with parity sequence $BBB\cdots$. It is a positive integer if and only if $D \mid c\,z_q(B)$ and $c\,D>0$, and the rotations of $B$ give the other members of the cycle (Böhm–Sontacchi 1978, Lagarias 1990). Enumerating all words therefore yields a complete list of cycles up to a given word length.

#### compile

```console
cd "07 Basins of Attraction/C"
make
```

or directly `gcc -O2 -o census census.c -lm` and `gcc -O2 -o basin basin.c`. Both programs need `__int128` (GCC or Clang on 64 bit). `passage` also needs GMP and MPFR and has its own targets: `make passage` (with `GMP_PREFIX=/path/to/env` if GMP and MPFR come from a conda env with `gmp` and `mpfr`) and `make test-passage`.

## census

#### run

```console
./census q c Lmax [Lmin [s i]]
```

#### parameters

* `q`, `c`: odd integers; `c` may be negative (`3 -1` is the $3x-1$ map).
* `Lmax`, `Lmin`: range of word lengths (default `Lmin = 1`, at most 62).
* `s`, `i`: sharding. Only words whose first `s` bits equal `i` are tested ($0 \le i < 2^s$); lengths `l <= s` are handled by shard 0. The shards $0,\dots,2^s-1$ together cover every word exactly once.

#### output

One header line, then one line per cycle: smallest member, type $(l,k)$, $D$ and the parity word starting at the smallest member.

```console
$ ./census 3 5 22
q=3 c=5 Lmin=1 Lmax=22 shard=0/1 words=7286490 cycles=4
min=5 l=2 k=1 D=1 word=10
min=1 l=3 k=1 D=5 word=100
min=19 l=5 k=3 D=5 word=11100
min=23 l=5 k=3 D=5 word=11010
```

A cycle with primitive word length $l_0$ reappears at $2l_0, 3l_0,\dots$; `merge_census.py` keeps the smallest length per cycle:

```console
python3 merge_census.py out/census_q3_c5_*.txt
```

#### cost

For $3x+c$ with $c>0$, all words up to $L=30$ are about $2\cdot10^9$ words and take about 2.5 minutes on one core. Every further unit of $L$ doubles the work. With 512 shards on a cluster, $L\approx 40$ takes minutes and $L\approx 45$ hours per map. For $c<0$ the 128-bit arithmetic limits $L$ to about 48 (the program stops with a message).

## census_all

#### run

```console
./census_all q DMAX Lmax [Lmin [s i]]
```

#### parameters

* `q`: odd, at least 3.
* `DMAX`: largest $|c_0|$ to report.
* `Lmax`, `Lmin`, `s`, `i`: word lengths and sharding as for `census`.

#### how it works

For a word $B$ the periodic point of $T_{q,c}$ is $c\,z_q(B)/D$. Let $d_0$ be the denominator of $z_q(B)/D$ in lowest terms. Then $B$ gives a positive integer cycle exactly for $c=\mathrm{sign}(D)\,m\,d_0$ with $m$ odd, and these cycles are $m$ times the *primitive* cycle of $c_0=\mathrm{sign}(D)\,d_0$. The denominator is found by a continued-fraction expansion that stops as soon as it exceeds `DMAX`. Each primitive cycle is printed exactly once, by the rotation of its word that starts at the smallest member.

#### output

```console
$ ./census_all 3 1000 28 > a.txt && python3 merge_census_all.py a.txt > cycles.csv
files=1 words=536870882 cycles=900 mismatches=0
$ head -6 cycles.csv
c,min,l,k,D,word
1,1,2,1,1,10
-1,1,1,1,-1,1
-1,5,3,2,-1,110
-1,17,11,7,-139,11110111000
5,1,3,1,5,100
```

The cycles of one map, e.g. $3x+5$ (input for `basin`):

```console
$ python3 cycles_for_map.py cycles.csv 5
1 5 19 23 187 347
```

#### cost

About 130 ns per word on one core. All words up to $L=28$ take 71 seconds. On 384 cores, $L=40$ takes about 15 minutes and $L=44$ about 3.5 hours.

#### verification

`make test` compares `census_all` with an independent implementation in exact rational arithmetic (`tests/ref_census.py`), checks that 16 shards reproduce a full run, compares with `census` for single maps, and checks that the cache size of `basin` does not change any count. The output field `mismatches` (parity check of every printed cycle) must be 0.

## mitm_census (Run P)

#### run

```console
./mitm_census q DMAX Lmin Lmax [kmin kmax] [--mem GB] [--cand F] [--lcap L]
```

#### parameters

* `q`, `DMAX`: as for `census_all`.
* `Lmin`, `Lmax`, `kmin`, `kmax`: the types $(l,k)$ to search; $l \le 70$ for $q=3$ (128-bit arithmetic).
* `--mem GB`: memory for the two half-lists of one split and their sorted copies (default 4). Larger lists are processed in passes over the top bits of their residues.
* `--cand F`, `--lcap L`: see below (defaults 0.1 and no limit; `--lcap 1` gives one search per divisor).

#### how it works

A word of type $(l,k)$ is in the census iff $d_0 \le$ `DMAX`, that is iff $z_q(B) \equiv 0 \pmod{|D|/d^*}$ for a maximal divisor $d^* \le$ `DMAX` of $|D|$. Splitting $B=B_1B_2$ into halves of lengths $h_1=\lfloor l/2\rfloor$ and $h_2$ with $k_1$ and $k_2$ ones gives
$z_q(B)=q^{k_2}z_q(B_1)+2^{h_1}z_q(B_2)$ (Lemma 2(a) of the paper), so the congruence is an equality of residues $u(B_1)=v(B_2)$. Both lists are generated by a depth-first search (one modular addition per one-bit), joined on their residues, and every match is checked exactly as in `census_all` (exact $z_q(B)$, $d_0$ by continued fractions, one walk around the cycle, only the rotation that starts at the minimum). The maximal divisors are searched together modulo $|D|/L$, $L$ their lcm, as long as the expected number of exact checks $\binom lk L/|D|$ stays below `F` times the number of residues of one search. The work per type is about $2^{l/2}$ residues instead of $\binom lk$ words.

#### output

The cycle lines are exactly those of `census_all`, so `merge_census_all.py` and `cycles_for_map.py` work unchanged. One summary line per type:

```console
$ ./mitm_census 3 1000 40 40 20 20 | tail -1
# q=3 DMAX=1000 l=40 k=20 D=1096024843375 dstar=905,875,781,497,385,355,275 groups=2 pairs=38230 words=780 cycles=6 mismatches=0 passes=1 maxlist=184756 rss_mb=15 seconds=0.357
```

#### cost

About 90 ns per residue on one core. All types with $l \le 40$ take 7 seconds (Run A: $2.2\cdot10^{12}$ words, 512 array tasks); the type $(56,28)$ takes 49 seconds and 2.6 GB. The cluster run for $l = 41,\dots,66$ took 46 core-hours (longest type $(66,33)$: 41 minutes, 11 GB); beyond $l = 66$ the half-lists outgrow a node and the passes dominate. A process stays about 0.2 GB above `--mem`.

#### verification

`make test-mitm` compares `mitm_census` with `census_all` for $q = 3, 5, 7$, checks that the passes and the separate searches per divisor give the same cycles, and compares with the stored census of Run A. For $l \le 40$ the program reproduces all 1097 cycles of Run A exactly.

## basin

#### run

```console
./basin [-w] q c start end cache m1 [m2 ...]
```

#### parameters

* `-w`: additionally report the counts per dyadic window $[2^j, 2^{j+1})$.

* `q`, `c`: as above.
* `start`, `end`: range $[start, end)$ of starting values.
* `cache`: size of the label cache $1,\dots,cache$ (one byte per number). Orbits of $n > cache$ are followed until they drop to at most `cache`. The cache is enlarged automatically to the largest cycle member.
* `m1 m2 ...`: smallest members of the known cycles, e.g. from `cycles_for_map.py`.

Orbits that exceed $2^{120}$ or $10^6$ steps are counted as `unlabeled` (candidates for divergence or for an unknown cycle; for $5x+1$ this is the majority). For $c \le -3$ orbits can leave the positive integers ($3x-5$ maps $1$ to $-1$); such starting values are counted as `escaped`.

#### output

```console
$ ./basin 3 -1 1 100000000 1000000 1 5 17
start=1 end=100000000 cache=1000000 unlabeled=0 escaped=0
m=1 count=32697317 share=0.326973
m=5 count=32470805 share=0.324708
m=17 count=34831877 share=0.348319
```

Counts of several ranges add up; `merge_basin.py` merges them. With `-w` it also prints the share of every basin per dyadic window and their mean over the complete windows with $j \ge 10$ (an estimate of the logarithmic density), and `--csv` writes the window counts:

```console
python3 merge_basin.py out/basin_q3_c-1_*.txt --csv ../results/basin_q3_c-1_windows.csv
```

#### cost

About 23 seconds for $n < 10^8$ with a cache of $10^6$ on one core. A run up to $10^{12}$ with 384 array tasks and a cache of $10^9$ (1 GB per task) is a matter of minutes to hours.

## passage

#### run

```console
./passage c log10y alpha mode nsamples seed [B]
```

#### parameters

* `c`: odd, at least $-1$ (the map $3x+c$).
* `log10y`, `alpha`: the window $[y, y^\alpha]$ with $y=10^{\texttt{log10y}}$, e.g. `100 1.1` for $[10^{100}, 10^{110}]$.
* `mode`: `log` draws odd starting values with probability proportional to $1/N$, `unif` draws them uniformly (natural counting), `exact` enumerates a small window completely (for validation).
* `nsamples`, `seed`: number of starting values and seed of the random generator.
* `B`: table bound (default $10^7$). All odd $m<B$ are labelled with their cycle first.
* `m K` (optional): residue sweep. The starting values are drawn from $K$ residue classes $r \bmod m$, `nsamples` in each (all admissible classes if $K=0$ or $K$ is at least their number; for even $m$ only odd $r$). Two fields `m,r` are appended to every output line.

#### how it works

Each orbit is followed with GMP integers until it first drops below `B`; the basin of that point is read from the table. By basin invariance under first passage (Step 2 of the proof of Theorem 1 of the paper), this is the basin of the starting value. Cycles whose members all exceed `B` are detected on the fly with Brent's algorithm, reported on stderr and counted like the others. The logarithmic shares estimate the window probabilities of Remark 3, which converge to the logarithmic basin densities; the uniform shares estimate natural counting on the same window.

#### output

One CSV line per cycle: `c,log10y,alpha,mode,seed,n,cycle_min,count,share` (`cycle_min = -1`: orbits not resolved within $10^7$ steps). For example:

```console
$ ./passage 5 100 1.1 log 100000 1
5,100,1.1,log,1,100000,1,14000,0.1400000000
5,100,1.1,log,1,100000,19,49847,0.4984700000
5,100,1.1,log,1,100000,5,19945,0.1994500000
5,100,1.1,log,1,100000,23,9182,0.0918200000
5,100,1.1,log,1,100000,187,3299,0.0329900000
5,100,1.1,log,1,100000,347,3727,0.0372700000
5,100,1.1,log,1,100000,-1,0,0.0000000000
```

`merge_passage.py` adds the counts of all replicates and prints shares, standard errors, $z$-scores against Table 7, homogeneity tests across the scales, and the comparison of both counting modes. `--pool 20` prints the shares pooled over the windows from $10^{20}$ on (Table 8 of the paper) together with the statistics quoted with it in the paper (deviations from $d_{10^{10}}$, between the counting modes and of single windows, homogeneity across the windows); `--replicates` checks that the replicates of each configuration scatter like independent binomial samples. It also reads its own summary CSV, so Table 8 can be reproduced from `results/passage/passage_shares.csv`:

```console
python3 merge_passage.py ../results/passage/passage_shares.csv --pool 20
```

#### cost

About 3 µs per starting value at $10^{10}$, 25–35 µs at $10^{100}$ and 75 µs at $10^{200}$ on one core.

#### verification

`make test-passage` compares the sampled shares with an exact enumeration of the window $[10^6, 10^{6.6}]$ for both counting modes, repeats the test with a small table bound (so that every orbit has to pass), and compares the GMP path with an independent Python implementation at $[10^{40}, 10^{44}]$. The cycles through 5 of $3x+5$ and through 13 of $3x+13$ consist of the multiples of 5 and 13 and serve as controls of the sampler (shares $1/5$ and $1/13$).

## potential

Predecessor counts $\pi_a(N)=\#\{n\le N:\ a \text{ lies on the orbit of } n\}$ for **all** targets $a\le A$ at once (Run G). They estimate the predecessor potential $h(a)$ of Proposition 7 of the paper, the density of the starting values whose orbit reaches $a$.

#### run

```console
make potential
OMP_NUM_THREADS=32 ./potential 5 10000000000 1000000 potential_c5.csv
python3 potential_report.py potential_c5.csv
```

#### parameters

`potential C N A OUT.csv`: map $3x+c$ ($c$ odd, $c\ge-1$), starting values $1\le n\le N$, targets $1\le a\le A$.

#### how it works

For $n$ let $f(n)$ be the first iterate below $n$. The orbit of a starting value is the concatenation of the orbit segments between the successive stopping points $n>f(n)>f(f(n))>\dots$, and a segment that starts at $n$ only contains values $\ge n$. One descending pass computes $W(n)=1+\sum_{f(n')=n}W(n')$, the number of starting values whose chain of stopping points passes through $n$; then $\pi_a(N)$ is the sum of $W(n)$ over the $n\le a$ whose segment contains $a$. Periodic targets are counted once, at the end of the chain. No orbit is followed twice.

#### output

One line `a,count,cycle_min` per target; `cycle_min` is the minimum of the cycle if `a` is periodic, else 0. `potential_report.py` verifies, as exact integer identities, the flow identity $\pi_a=[a\le N]+\pi_{2a}+\pi_{(2a-c)/3}$ for all non-periodic $a\le A/2$, the doubling chains of the multiples of 3, and the level identity (basin = cycle + predecessor sets of its entry points). It prints the basin shares, the mean value of $a\,h(a)$ against $2/\log(4/3)$, and the means over the residue classes modulo 3, 9, 27 and 81 against the law of Tao's Syracuse random variable, $\frac{2}{\log(4/3)}\,3^n\,\mathbb P(c\cdot\mathrm{Syrac}(\mathbb Z/3^n\mathbb Z)=r)$.

#### cost

8 bytes per starting value (80 GB for $N=10^{10}$); about 2 s per $10^8$ starting values on one core.

#### verification

`make test-potential` compares the counts of all targets with a brute-force reference (`tests/ref_potential.py`) for $3x-1$, $3x+1$, $3x+5$, $3x+7$ and $3x+13$, and checks the identities for $3x+5$ up to $2\cdot10^6$. The basin shares of the report reproduce those of `basin` (Table 7) by an independent method.

## orbit_profile

The mean value and the Syracuse profile of the potential for large targets (Run H). The sum of $a\,h(a)$ over a set of targets equals the mean, over the starting values, of the sum of the orbit points in that set. `orbit_profile` therefore follows the orbits of random starting values and collects their points by decade and by the residue class of $a/c$ modulo 81; no counts are stored.

```console
make orbit_profile
./orbit_profile 1 10000000 28 1 > orbit_profile_c1.txt     # C S E SEED [KMIN [KPOOL]]
python3 orbit_profile_report.py orbit_profile_c*.txt
```

`S` starting values are drawn uniformly from $[10^E, 2\cdot10^E)$ ($E\le30$, 128-bit arithmetic, deterministic generator), the targets lie in the decades $10^{\text{KMIN}}$ to $10^{E-3}$ (default from $10^6$), and the residue classes are pooled over the decades from $10^{\text{KPOOL}}$ (default $10^9$). About one minute per $10^7$ starting values on one core. The report compares the decade means with $2/\log(4/3)$ and the class means modulo 3, 9, 27 and 81 with the law of Tao's Syracuse random variable (Conjecture 1 of the paper). The results in `results/potential/` use $S=10^7$, $E=28$ and the seeds 1, 2, 3, 4 for $c=1,-1,5,13$.

## tail_check

The distribution of the first-passage mass $\Phi_X/X$ (Run I, new in version 3: the exponential-tightness proposition and the remark "What remains, and what the numerics can see" in the appendix on first-passage tails). For an odd start $m$, $\Phi_X(m)$ is the sum of the orbit points in the window $W_X=(X^{1-\beta},X]$ in the Syracuse blocks up to the first odd iterate $\le X_1=X^{1-2\beta}$. The tightness proposition proves, conditional on the coupling lemmas of the appendix, that $\Phi_X/X$ has an exponential tail up to an exceptional set of logarithmic density $O((\log X)^{-\varepsilon_0})$; Run I compares the actual distribution with the random-walk model of the paper.

```console
make tail_check
./tail_check 1 30 0.05 4000000 1 actual > a.raw 2> a.ovf    # C LOG10X BETA S SEED actual|model
python3 tail_fallback.py a.raw a.ovf > a.txt
./tail_check 1 30 0.05 4000000 2 model > m.txt
bash tail_grid.sh                                            # all of Run I -> ../results/tail/report_I.txt
```

Starts are odd and logarithmically distributed in $[X^{1+\beta},X^{(1+\beta)^2}]$, the window used in the proofs. `actual` follows the orbit in 128-bit arithmetic; starts whose orbit exceeds $2^{125}$ are written to stderr (`OVF n`) and `tail_fallback.py` recomputes them exactly with Python integers. `model` runs the random walk $D_j=\sum_{t\le j}(g_t\log2-\log3)$ with geometric $g_t$. Output: mean, second moment and maximum of $\Phi_X/X$, and the counts of $\Phi_X/X>s$ for $s=1,\dots,60$. `tail_report.py DIR` compares actual and model (moments, fitted exponential tail rate, $z$-scores of the tail probabilities). `make test-tail` checks `actual`, `model` and the independent big-integer reference `tests/ref_tail.py` against each other.

The results in `results/tail/` cover $c\in\{1,-1,5\}$, $X\in\{10^{12},10^{20},10^{30}\}$, $\beta\in\{0.05,0.005\}$ with $4\cdot10^6$ starts each ($2\cdot10^7$ for $c=1$, $X=10^{30}$, $\beta=0.05$). Actual and model agree in all 18 configurations ($|z|\le2.1$). For $\beta=0.05$ and $X\ge10^{20}$ the sampled tails are consistent with exponential decay at an empirically fitted rate of about $0.21$ (a finite-sample fit, not an asymptotic estimate); the largest value is $\Phi_X/X=87.9$. Samples of this size cannot see the exceptional set of the tightness proposition, which is what remains open.

## heuristic tests (Run J)

Where the equidistribution heuristic for the number of primitive cycles (Section 5, Problem 1) fails, new in version 3.

```console
bash run_J.sh            # about 2 minutes; needs python3 with sympy; writes ../results/heuristic/
```

`word_census L C` enumerates all parity words of length at most `L` and records, per type $(l,k)$, how many words have reduced denominator $e=|D|/\gcd(z_3(B),|D|)\le C$. `necklace_counts.py` removes the periodic words by Möbius inversion and checks the result against `rational_cycles_q3_d1000_L40.csv` (960 cycles for $l\le30$, no mismatch). `algebraic_factor l k` counts the aperiodic words of an even type with $P\mid z_3(B)$, $P=2^{l/2}+3^{k/2}$. `heuristic_tests.py` writes `report_J.txt`:

1. Census $l\le40$, $|c_0|\le1000$: the deficit of the heuristic (839 observed, 879.8 expected) comes from the 46 types with $l,k$ even and $D>0$ (329 against 379.8); the 49 coprime types give 348 against 338.2.
2. Exact counts for $l\le30$: the dispersion index of the counts is 0.55 for $C=1000$ and 1.09 for $C=2\cdot10^4$ (0.61 and 0.85 for the coprime types).
3. For even $l,k$ every periodic word $B'B'$ satisfies $P\mid z_3(B)$. Among the aperiodic words, divisibility by $P$ occurs 0.61 times as often as $1/P$ predicts for $D>0$ (38 types) and 1.40 times for $D<0$ (9 types).

## potential tails (Run K)

Tests the hypothesis of the proposition "Domination by the stopped potential of odd targets" (Appendix on first-passage tails): the clipping tail vanishes, $T_X=o(X)$, if the stopped profile $a\,h(a)$ of the odd targets is uniformly integrable on average. Run K recomputes the predecessor counts of Run G with `potential` for more targets and evaluates the distribution of $v(a)=a\,h(a)$ on the dyadic windows $(X/2,X]$.

```console
make potential test-potential test-ui      # test-ui: ui_report.py on brute-force counts, potential vs. brute force
bash run_K.sh                              # SLURM: 4 maps x (N = 3e10, A = 2^22), then the report
# quick smoke test first:
N=100000000 A=262144 MEM=8G RUNK_DIR=$HOME/data/runK_smoke RES=$HOME/data/runK_smoke/results bash run_K.sh
```

`slurm/run_K_counts.sbatch` runs `potential C N A` for $c=1,-1,5,13$ (array job, 32 threads, 240 GB: 8 bytes per starting value) and converts the CSV to a compact `counts_c*.npz` (`ui_report.py convert`). The scheduler of the cluster does not account memory, so `--mem` alone packs several tasks onto one node; the script requests `--exclusive`. The working directory is `RUNK_DIR` (default `~/data/runK`; not `WORK`, which the cluster sets). `slurm/run_K_report.sbatch` runs `ui_report.py report` on the four files and writes `../results/potential_ui/`; `ui_report.py figures DIR` rebuilds `table_K.tex` and the figure from the CSV files alone. Only numpy is required (pandas speeds up reading, matplotlib draws the figure).

#### output

- `report_K.txt`: per map and window $(2^{k-1},2^k]$: mean, quantiles and maximum of $v$; the Cesàro tails $\tau_M=|{\rm window}|^{-1}\sum_{v(a)>M}v(a)$ for $M$ from 10 to 1000 and a fitted tail exponent ($\tau_M/{\rm mean}\approx M^{-\gamma}$); the number of targets whose orbit does not reach $[1,X^{1-2\beta}]$ (stopped profile, $\beta=1/200$); the $L^1$ distance $|v-v_n|$ to the Syracuse profile $v_n(a)=\frac{2}{\log(4/3)}3^n\,\mathbb P(c\cdot\mathrm{Syrac}(\mathbb Z/3^n\mathbb Z)=a)$ for $n\le14$; and the mass of $v$ on the dangerous 2-adic classes of the propositions "Single-depth concentration criterion" and "Domination by the stopped potential" ($\zeta_0=0.55$), relative to their density. It also gives the tails of the profile $v_n$ itself for $n=8,12,16$ (the same for every $c$) and the steps $\|f_n-f_{n-1}\|_1$ of its densities, which Tao's fine-scale mixing (1.26) makes summable.
- `ui_windows.csv`, `ui_tails.csv`, `ui_profile_l1.csv`, `ui_dangerous.csv`, `ui_syracuse_tails.csv`: the same numbers; `ui_odd.csv`: the tails for odd targets only, and in `ui_dangerous.csv` the odd mass ratio.
- `table_K.tex` (rows for the table in the paper) and `fig_ui_tails.pdf`/`.png`.
- `counts_c*.npz`: the raw counts $\pi_a(N)$, $a\le A$ (uint64 up to the last target with a count of at least $2^{32}$, uint32 above; periodic targets with their cycle minimum).

The Syracuse law modulo $3^n$ is computed exactly: $S_n=2^{-a}(1+3S_{n-1})$ with geometric $a$ becomes a cyclic correlation in discrete-log coordinates of the units modulo $3^n$ (2 is a primitive root), done by FFT.

#### results

`results/potential_ui/` holds the run with $N=3\cdot10^{10}$ and $A=2^{22}$ for $c=1,-1,5,13$ (about 6 minutes and 225 GB per map on one node of the std partition). The flow identity holds exactly for the $2.1\cdot10^6$ non-periodic targets of every map, and every target reaches $[1,X^{1-2\beta}]$. The distribution of $a\,h(a)$ is the same for the four maps and stable from $X=2^{14}$ to $2^{22}$. At $X=2^{22}$ the median is 2.3, the 99th percentile 68 and the mean 6.91–6.98 (against $2/\log(4/3)=6.952$). About half of the mean comes from $a\,h(a)>20$, 15 % from $a\,h(a)>100$ and 1.4–2.4 % from $a\,h(a)>1000$; the fitted tail exponent is about 0.8. The exact Syracuse profile has the same tail (12.6 % above 100 at depth 16), and its far tail grows with the depth as the measured one grows with $X$. The mass on the dangerous 2-adic classes is within 2 % of their density for $X\ge2^{18}$. Restricted to odd targets (the hypothesis of the proposition "Domination by the stopped potential of odd targets"), the shares change by at most 0.4 percentage points, and the odd mass on the dangerous classes is within 0.4 % of their density at $X=2^{22}$ (`ui_odd.csv`, rows "odd" of `table_K.tex`). The profile predicts the distribution, not yet the individual values: the mean distance $|a\,h(a)-v_{14}(a)|$ is still about 2.0. In the paper: the paragraph "Run K", Table 13 and Figure 5 in the appendix on first-passage tails.

#### verification

`ui_report.py selftest` checks the Syracuse law modulo 9 against Section 6 of the paper and modulo 81 against an exact enumeration, and the densities of the dangerous parity words against brute force. `make test-ui` runs the whole report on brute-force counts (`tests/ref_counts.c`) for four maps and checks the flow identity $\pi_a=[a\le N]+\pi_{2a}+\pi_{(2a-c)/3}$ exactly; if `potential` is built, it compares its counts with the brute force. The report checks the flow identity on the cluster counts as well.

#### cost

The counts take 8 bytes per starting value (240 GB for $N=3\cdot10^{10}$) and, as in Run G, about 2 s per $10^8$ starting values on one core. The report takes a few minutes.

## Lean

`lean/` is a Lean 4 + Mathlib project (`lakefile`, committed `lake-manifest.json`) with the finite, elementary statements of the paper. Nothing about densities, the Syracuse profile or actual-orbit return estimates is formalised.

| module | content | in the paper |
|---|---|---|
| `Basic.lean` | accelerated map $T_{3,c}$ on $\mathbb Z$, exact immediate predecessors for odd $c$, positivity for $c\ge-1$ | Table 2, last row ("predecessors") |
| `FiniteFlow.lean` | finite-horizon hitting recursion, weighted hitting mass for finite samples | Table 2 ("hits", "weights") |
| `Clipping.lean` | clipping identity, quadratic excess bound | Table 2 ("clipping") |
| `ReturnPairs.lean`, `ReturnGreen.lean` | $N^2=N+2\,$pairs, odd-sum and Green-type occupation identities | Table 2, Remark on Lean verification |
| `EntranceTransport.lean` | finite entrance-measure inequalities | Remark on Lean verification |
| `ReturnLyapunov.lean` | two-level return certificate on terminating return graphs, one-step return arithmetic | Proposition "Two-level return certificate" |
| `ReturnLargeShell.lean` | fixed-modulus obstruction on every real shell with $X\ge24\cdot2^k+12|c|+24$ | Proposition "Fixed-modulus obstruction" |
| `Mod16Escape.lean`, `FiniteHorizonTransport.lean` | auxiliary escape residues modulo 16 | not used |

```console
cd lean
lake exe cache get                 # pinned Mathlib build cache
lake build CollatzBasins           # every declaration passes the kernel
lake env lean AxiomAudit.lean | tee axioms.txt
python3 tests/check_axioms.py axioms.txt AxiomAudit.lean
```

The workflow `.github/workflows/collatz-lean.yml` (repository root) runs these steps on every change of `lean/`. It rejects `sorry`, `admit`, `native_decide` and `axiom` declarations, requires every module to be imported by `CollatzBasins.lean`, and fails unless every theorem listed in `AxiomAudit.lean` depends at most on `propext`, `Classical.choice` and `Quot.sound`. A manually triggered job (`workflow_dispatch` with `deep_check`) replays the build with `lake check`. The paper links to the source at a fixed commit with a green run (`docs/pin_lean_links.py`).

## Pipeline example: 3x+5

```console
./census 3 5 30                                   # cycles with word length <= 30
./basin 3 5 1 1000000000 100000000 1 5 19 23 187 347
```

## Cluster (SLURM)

The templates in `C/slurm` run one shard or range per array task; adjust partition, time and memory to your cluster.

**Run A** – all primitive cycles with $|c_0| \le 1000$, word length up to 40 (then 44):

```console
cd "07 Basins of Attraction/C" && make && make test && mkdir -p logs out
Q=3 DMAX=1000 LMAX=40 sbatch --export=ALL slurm/census_all_array.sbatch
python3 merge_census_all.py out/census_all_q3_d1000_L1-40_*.txt > ../results/rational_cycles_q3_d1000_L40.csv
```

**Run P** – the same census for word lengths 41 to 64 (or 66) by meet-in-the-middle, one process per type $(l,k)$ on one exclusive node:

```console
cd "07 Basins of Attraction/C" && make census_all mitm_census && make test-mitm
LMIN=41 LMAX=64 sbatch slurm/run_P.sbatch                                   # any std node, under 1 h
LMIN=41 LMAX=66 PAR=48 MEMGB=12 sbatch --mem=700G slurm/run_P.sbatch        # 768 GB node, about 2 h
python3 run_P_report.py out_P ../results/rational_cycles_q3_d1000_L40.csv ../results/run_P
python3 heuristic_P.py ../results/run_P/rational_cycles_q3_d1000_L66.csv > ../results/run_P/heuristic_P.txt     # 1 s
(cd ../oeis && python3 compare_oeis.py ../results/run_P/rational_cycles_q3_d1000_L66.csv 66 > ../results/run_P/oeis_comparison_L66.txt)
(cd ../figures && python3 fig1_cycles_vs_heuristic.py ../results/run_P/rational_cycles_q3_d1000_L66.csv ../docs/fig1_cycles_vs_heuristic.pdf)
```

The last three steps run locally: the heuristic with the birthday factor (Section 5 of the paper, equation (10)), the comparison with OEIS A226607 up to 66 elements and Figure 1 (also written as PNG).

**Run B** – basins of $3x-1$ and all $3x+c$, $0 < c \le 50$, $\gcd(c,6)=1$ (18 maps) up to $N=10^{11}$, with window output. The cycle lists come from Run A:

```console
CSV=../results/rational_cycles_q3_d1000_L40.csv N=100000000000 slurm/submit_basins.sh
python3 merge_basin.py out/basin_q3_c5_N100000000000_*.txt --csv ../results/basin_q3_c5_windows.csv
```

`unlabeled` must be 0 for every map; otherwise the map has a cycle with a longer word, which is a result in itself. The columns $n\le10^6$ and $n\le10^8$ of Table 7 come from single runs, e.g. for $3x+5$:

```console
./basin 3 5 1 1000001 1000000 1 5 19 23 187 347 > out/b6.txt && python3 merge_basin.py out/b6.txt > ../results/basin_q3_c5_N1e6.txt
./basin 3 5 1 100000001 1000000 1 5 19 23 187 347 > out/b8.txt && python3 merge_basin.py out/b8.txt > ../results/basin_q3_c5_N1e8.txt
```

**Run C** – basin shares of $3x-1$, $3x+5$ and $3x+13$ in the windows $[10^y, 10^{1.1y}]$, $y = 10, 20, 50, 100, 200$, with $10^8$ logarithmic and $10^8$ uniform starting values per window (30 configurations in `slurm/passage_configs.txt`, 10 replicates each):

```console
make passage && make test-passage && mkdir -p logs out
sbatch --array=0-299 --export=ALL slurm/passage_array.sbatch
python3 merge_passage.py out/passage_*.csv --csv ../results/passage/passage_shares.csv --pool 20 --replicates > ../results/passage/report.txt
tar czf ../results/passage/runC_raw.tar.gz out/passage_*.csv     # raw outputs, needed for --replicates
```

**Run D** – basin landscape: all 133 maps $3x+c$ with $1 \le c \le 400$ and $\gcd(c,6)=1$, window $[10^{30}, 10^{33}]$, $5\cdot 10^7$ logarithmic starting values per map. `landscape.py` describes every cycle by its parity word, checks the gcd classes against Proposition 4 and cross-checks the cycles with $l \le 66$ against the census of Runs A and P (the default; with `--census` any other census and its bound); `fig3_landscape.py` draws the landscape and prints rank correlations between the shares and simple features of the cycles:

```console
CONFIGS=slurm/landscape_configs.txt REPS=5 OUTDIR=out_landscape sbatch --array=0-664 --export=ALL slurm/passage_array.sbatch
mkdir -p ../results/landscape
python3 landscape.py out_landscape/passage_*.csv --csv ../results/landscape/landscape_shares.csv > ../results/landscape/report.txt
python3 ../figures/fig3_landscape.py ../results/landscape/landscape_shares.csv ../docs/fig3_landscape.pdf
tar czf ../results/landscape/runD_raw.tar.gz out_landscape/passage_*.csv
```

**Run E** – basin shares conditional on residue classes $r \bmod 2^k$ ($k = 4, \dots, 30$) and $r \bmod 3^j$ ($j = 1, \dots, 12$) for $3x-1$, $3x+5$ and $3x+13$, $10^6$ starting values in each of up to 64 classes; the classes mod 5 for $3x+5$ and mod 13 for $3x+13$ are positive controls (the multiples of 5 and 13 form a basin of their own). `residue_flatness.py` tests per map and modulus whether the class shares agree:

```console
CONFIGS=slurm/residue_configs.txt REPS=1 OUTDIR=out_residue sbatch --array=0-37 --export=ALL slurm/passage_array.sbatch
mkdir -p ../results/residue
python3 residue_flatness.py out_residue/passage_*.csv --csv ../results/residue/residue_classes.csv > ../results/residue/report.txt
tar czf ../results/residue/runE_raw.tar.gz out_residue/passage_*.csv
```

**Run E2** – targeted repetition of the classes mod 9 (and mod 27 for $3x+13$) with $10^7$ starting values per class, as 10 replicates:

```console
CONFIGS=slurm/residue9_configs.txt REPS=10 OUTDIR=out_residue9 sbatch --array=0-39 --export=ALL slurm/passage_array.sbatch
python3 residue_flatness.py out_residue9/passage_*.csv --csv ../results/residue/residue9_classes.csv > ../results/residue/report_mod9.txt
tar czf ../results/residue/runE2_raw.tar.gz out_residue9/passage_*.csv
```

**Run F** – a predictor of the basin shares from the cycle alone. `backward_mass` walks the backward tree of a cycle depth first (predecessors $2y$ and $(2y-c)/3$; multiples of 3 are not expanded) and records the harmonic mass $\sum 1/x$ of every level, counting only nodes $x \ge c/2$ (below $c/2$ a node has no odd predecessor). `predictor_eval.py` normalises the mean mass of the deepest levels within every map, compares it with the measured shares of Run D and with simple baselines, and with `--sweep` shows the convergence in depth; `fig4_backward_mass.py` plots predicted against measured shares:

```console
make backward_mass && mkdir -p logs
python3 predictor_eval.py --list ../results/landscape/landscape_shares.csv > ../results/landscape/primitive_cycles.txt
sbatch --array=0-409 --export=ALL slurm/backward_mass_array.sbatch
cat out_mass/bm_*.csv > ../results/landscape/backward_mass_D70.csv
python3 predictor_eval.py ../results/landscape/landscape_shares.csv ../results/landscape/backward_mass_D70.csv --sweep 10 --csv ../results/landscape/predictions_D70.csv > ../results/landscape/predictor_D70.txt
python3 ../figures/fig4_backward_mass.py ../results/landscape/predictions_D70.csv ../results/landscape/fig4_backward_mass.pdf
tar czf ../results/landscape/runF_raw.tar.gz out_mass/bm_*.csv
python3 cosets.py ../results/landscape/landscape_shares.csv --predictions ../results/landscape/predictions_D70.csv > ../results/landscape/cosets.txt
```

`cosets.py` checks the multiplicative filtration (Proposition 5 of the paper): for $c$ coprime to 6, $T(n)$ is congruent to $n/2$ or $3n/2$ modulo $c$, so the coset of $n$ modulo the subgroup $\langle 2,3\rangle$ of $(\mathbb Z/c\mathbb Z)^\times$ is constant along every orbit. The script prints the measured share of every coset (it must be $1/[G:H]$), the cycles that are alone in their coset (their share is then exact), and the predictor before and after normalising it within the cosets.

Run G (predecessor potential, Proposition 7): four maps, $N=10^{10}$, targets up to $10^6$; 100 GB and a few minutes per task.

```console
make potential && mkdir -p logs ../results/potential
sbatch --array=0-3 --export=ALL slurm/potential_array.sbatch
python3 potential_report.py out_potential/potential_c*.csv > ../results/potential/report.txt
tar czf ../results/potential/runG_raw.tar.gz out_potential/potential_c*.csv
```

## Reproduction map

Which artifact produces which result of the paper (numbered as in version 3; the runs are listed in Table 11 of the paper):

| Result | Artifacts (in `C/` unless noted) | Data |
|---|---|---|
| Residue classes mod 18 and 288 of $3x\pm1$, forest density (Table 3, upper rows); members of one class in different trees of $3x-1$ (Table 4); residues mod 13 of the near miss $(8,5)$ | `local_structure.py` | printed |
| Cycles by binary rotation (Table 5) | `04 Cycles`: `eldar_parityvector_k1.ipynb`, `eldar_rotate.ipynb` | – |
| Complete cycle lists of single maps (Table 6) | `census.c`, `slurm/census_array.sbatch`, `merge_census.py` | `results/census_q*_L*.txt` |
| All primitive cycles of $3x+c$, $\lvert c_0\rvert \le 1000$, $l \le 40$ (Run A, Proposition 3) | `census_all.c`, `slurm/census_all_array.sbatch`, `merge_census_all.py` | `results/rational_cycles_q3_d1000_L40.csv` |
| The same census by meet-in-the-middle for word lengths 41 to 66 (Run P, roadmap C2; Proposition 3 for $l \le 66$); census vs. heuristic (Figure 1) | `mitm_census.cpp`, `run_P_tasks.py`, `run_P_report.py`, `slurm/run_P.sbatch`, `tests/run_mitm_tests.sh`; `figures/fig1_cycles_vs_heuristic.py` | `results/run_P/` (local probe 41..48: `results/run_P_probe/`) |
| Birthday factor of the cycle heuristic, fitted on $l \le 40$ and tested on $41 \le l \le 66$ (Run P, equation (10), Problem 1) | `heuristic_P.py` | `results/run_P/heuristic_P.txt` |
| Agreement with Belaga, Mignotte and Morley (OEIS A226607), up to 66 elements | `oeis/compare_oeis.py` | `oeis/a226607_first1740.txt`, `results/run_P/oeis_comparison_L66.txt` |
| Cycles with longer words ($3x+23$ through 41; $3x+29$ through 3811 and 7055) | `cycles_for_map.py --search 100000` | printed |
| Basin shares up to $10^6$, $10^8$, $10^{10}$ (Run B, Table 7); every $n \le 10^{10}$ reaches a known cycle; dyadic windows and the split of 18 forests (Figure 2) | `basin.c`, `slurm/submit_basins.sh`, `slurm/basin_array.sbatch`, `merge_basin.py`; `figures/fig2_basins.py` | `results/basin_q3_c*_N1e{6,8,10}.txt`, `results/basin_q3_c*_windows_N1e10.csv`, `results/runB_N1e10.tar.gz` (raw) |
| Basin shares far beyond enumeration, natural vs. logarithmic counting (Run C, Table 8) | `passage.c`, `slurm/passage_array.sbatch`, `slurm/passage_configs.txt`, `merge_passage.py --pool 20 --replicates` | `results/passage/passage_shares.csv`, `results/passage/report.txt`, `results/passage/runC_raw.tar.gz` (raw) |
| Multiplicative filtration (Proposition 5); survey of all maps $3x+c$ with $c \le 400$ (Run D, Figure 3) | `passage.c`, `slurm/landscape_configs.txt`, `landscape.py`, `cosets.py`; `figures/fig3_landscape.py` | `results/landscape/landscape_shares.csv`, `cosets.txt`, `correlations.txt`, `runD_raw.tar.gz` (raw) |
| Residue classes carry no further information (Runs E, E2; Section 6) | `passage.c`, `slurm/residue_configs.txt`, `slurm/residue9_configs.txt`, `residue_flatness.py` | `results/residue/` |
| Backward-mass predictor, normalised and absolute with the constant $3/\log(4/3)$ (Run F, Table 10, Figure 4, Conjecture 2) | `backward_mass.c`, `slurm/backward_mass_array.sbatch`, `predictor_eval.py`, `cosets.py`; `figures/fig4_backward_mass.py` | `results/landscape/backward_mass_D70.csv`, `predictions_D70.csv`, `predictor_D70.txt`, `runF_raw.tar.gz` (raw) |
| Predecessor potential: exact flow and level identities, mean value and Syracuse profile of $a\,h(a)$ (Runs G, H; Proposition 7, Table 9, Conjecture 1) | `potential.c`, `slurm/potential_array.sbatch`, `slurm/potential_configs.txt`, `potential_report.py`; `orbit_profile.c`, `orbit_profile_report.py` (large targets) | `results/potential/report.txt`, `results/potential/runG_raw.tar.gz`; `results/potential/orbit_profile_c*.txt`, `results/potential/report_H.txt` |
| First-passage mass $\Phi_X/X$: actual orbits vs. random-walk model, exponential tails (Run I, new in version 3; Proposition 9, Remark 8) | `tail_check.c`, `tail_fallback.py`, `tail_grid.sh`, `tail_report.py` | `results/tail/report_I.txt`, `results/tail/{actual,model}_*.txt` |
| Heuristic for primitive cycles: deficit from the algebraic factors of $2^l-3^k$, dispersion against the bound (Run J, new in version 3; Problem 1) | `word_census.c`, `necklace_counts.py`, `algebraic_factor.c`, `heuristic_tests.py`, `run_J.sh` | `results/heuristic/report_J.txt` |
| Tails of the potential $a\,h(a)$ against the Syracuse profile, dangerous classes (Run K, new in version 3; Proposition 14, Table 13, Figure 5) | `potential.c`, `ui_report.py`, `run_K.sh`, `slurm/run_K_counts.sbatch`, `slurm/run_K_report.sbatch` | `results/potential_ui/` |
| Lean 4 proofs of the finite statements (Table 2, last row; Propositions 10 and 11) | `lean/`, `.github/workflows/collatz-lean.yml` (repository root) | links pinned to commit `ee5a5b4` |
| Tests | `tests/run_tests.sh` (census vs. exact rational reference `tests/ref_census.py`, shards, cache); `tests/run_mitm_tests.sh` (Run P against `census_all` and Run A); `tests/run_potential_tests.sh` (predecessor counts vs. brute force `tests/ref_potential.py`, exact identities); `tests/run_passage_tests.sh` (sampling vs. exact enumeration and an independent Python reference); `tests/run_tail_tests.sh` (Run I: actual vs. model vs. big-integer reference `tests/ref_tail.py`); `tests/run_ui_tests.sh` (Run K report on brute-force counts `tests/ref_counts.c`, `potential` vs. brute force) | – |

## Reference results

**Complete cycle lists** (all positive integer cycles with word length $l \le L$; for the maps $3x+c$ from Runs A and P, for $5x+1$ and $181x+1$ from `census.c`):

| map | $L$ | cycle minima | type $(l,k)$ | $D=2^l-q^k$ |
|---|---|---|---|---|
| $3x+1$ | 66 | 1 | (2,1) | 1 |
| $3x-1$ | 66 | 1 / 5 / 17 | (1,1) / (3,2) / (11,7) | −1 / −1 / −139 |
| $3x+5$ | 66 | 5 / 1 / 19, 23 / 187, 347 | (2,1) / (3,1) / (5,3) / (27,17) | 1 / 5 / 5 / 5 077 565 |
| $3x+13$ | 66 | 13 / 1 / 211, 227, 251, 259, 283, 287, 319 / 131 | (2,1) / (4,1) / (8,5) / (24,15) | 1 / 13 / 13 / 2 428 309 |
| $5x+1$ | 28 | 1 / 13, 17 | (5,2) / (7,3) | 7 / 3 |
| $181x+1$ | 40 | 27, 35 | (15,2) | 7 |

Cycles sit where $2^l$ and $q^k$ nearly coincide, e.g. $2^{15}-181^2=7$ and $2^7-5^3=3$. The seven cycles of $3x+13$ of type $(8,5)$ are exactly the seven candidates for a $3x+1$ cycle of that type; they fail because none of their members is divisible by $13 = 2^8-3^5$.

**Rational cycles** (`results/rational_cycles_q3_d1000_L40.csv`, Run A): all primitive cycles of the maps $3x+c$ with $|c_0| \le 1000$ and word length $l \le 40$, from all $2.2\cdot 10^{12}$ parity words. There are 1097 of them, 763 with $c_0>0$ and 334 with $c_0<0$, for 199 different values of $|c_0|$. The heuristic $\sum L(l,k)\,S(D)/|D|$ predicts 1149 (Figure 1 of the paper). A start-value search adds three cycles with longer words: $3x+23$ through 41 ($l=43$) and $3x+29$ through 3811 and 7055 ($l=65$, the convergent $65/41$ of $\log_2 3$). Run P (`mitm_census.cpp`) reproduces this census exactly and extends it to $l \le 66$ (`results/run_P/rational_cycles_q3_d1000_L66.csv`): 199 further primitive cycles, 1296 in total, among them all three cycles of the start-value search, against 274 expected by the heuristic for $41 \le l \le 66$. The growing deficit (observed/expected 0.95 for $l \le 40$, 0.71 for $41 \le l \le 66$, types with $E \ge 1$) is mostly a birthday effect: the $k$ odd members of a cycle must be distinct, and their typical size is $M_e = e/|2^{l/k}-3|$ for $|c_0| = e$. The factor $\exp(-k^2|2^{l/k}-3|/(\alpha e))$ per divisor $e$, with $\alpha = 6$ fitted on $l \le 40$, predicts 163 instead of 206 cycles for $41 \le l \le 66$ (146 observed) and 1296.7 in total for $l \le 66$ (`results/run_P/heuristic_P.txt`, Figure 1 dashed). All 947 primitive cycles of OEIS A226607 with at most 66 elements coincide with the census (`results/run_P/oeis_comparison_L66.txt`). There is no further cycle of $3x\pm1$, $3x+5$ or $3x+13$, so the complete cycle lists of these four maps below hold for $L = 66$.

**Basin shares** for $n \le 10^{10}$ (Run B; all 18 maps in Figure 2 of the paper, every $n \le 10^{10}$ reaches a known cycle; exact counts in `results/basin_q3_c*_N1e10.txt`, for $3x-1$, $3x+5$, $3x+13$ also up to $10^6$ and $10^8$ in `*_N1e6.txt`, `*_N1e8.txt`):

| map | basins (cycle minimum: share) |
|---|---|
| $3x-1$ | 1: 0.32701, 5: 0.32498, 17: 0.34801 |
| $3x+5$ | 1: 0.14035, 5: 0.20000, 19: 0.49792, 23: 0.09223, 187: 0.03253, 347: 0.03697 |
| $3x+13$ | 1: 0.47753, 13: 0.07692, 131: 0.25167, 211: 0.03345, 227: 0.01892, 251: 0.01984, 259: 0.03861, 283: 0.02528, 287: 0.04329, 319: 0.01449 |

**Predecessor potential** (Run G, `results/potential/`, Table 9 of the paper): for $3x+1$, $3x-1$, $3x+5$ and $3x+13$, all starting values $n\le10^{10}$ and all targets $a\le10^6$. The flow identity (about $5\cdot10^5$ targets per map), the doubling chains of the multiples of 3 and the level identity of every cycle hold as exact integer identities, and the basin counts reproduce Table 7. The mean of $a\,h(a)$ is 6.85, 6.88, 6.88 and 6.97 against $2/\log(4/3)=6.95$; the class means modulo 9 follow the law of Tao's Syracuse random variable within 2.4 %, with the same six values for all four maps after relabelling the classes by $a/c$, and modulo 81 the median deviation is below 1 % (Conjecture 1). The deviations are an effect of the small targets: for the targets of the decades $10^9$ to $10^{25}$ (Run H, orbits of $10^7$ random starting values near $10^{28}$ per map) the mean is 6.9515, 6.9518, 6.9524 and 6.9518 against 6.9521, the class means modulo 9 agree to four digits, and modulo 81 the largest deviation is 0.12 %.

**Absolute backward-mass predictor** (Run F data, `results/landscape/predictor_D70.txt`, Table 10): over the 410 primitive cycles, measured density / mean level mass has the median 10.41 for the levels 60 to 69 (quartiles 10.15 and 10.86) against the constant $3/\log(4/3)=10.43$ of Conjecture 2; the absolute prediction deviates by 3.0 % in the median, without any normalisation within the maps.

**Basin shares at large scales** (Run C, `results/passage/`, Table 8 of the paper): pooled over the windows $[10^y, 10^{1.1y}]$ with $y = 20, 50, 100, 200$ and both counting modes, $8\cdot 10^8$ starting values per map (standard errors at most $2\cdot 10^{-5}$):

| map | basins (cycle minimum: share) |
|---|---|
| $3x-1$ | 1: 0.32691, 5: 0.32503, 17: 0.34807 |
| $3x+5$ | 1: 0.14042, 5: 0.20000, 19: 0.49790, 23: 0.09218, 187: 0.03255, 347: 0.03696 |
| $3x+13$ | 1: 0.47751, 13: 0.07693, 131: 0.25167, 211: 0.03344, 227: 0.01892, 251: 0.01985, 259: 0.03862, 283: 0.02527, 287: 0.04333, 319: 0.01447 |

Natural and logarithmic counting agree to within $7\cdot 10^{-5}$. The cumulative shares up to $10^{10}$ still differ from these by up to $10^{-4}$, the size of their own drift between $10^8$ and $10^{10}$.

Basins of multiples of a divisor $e>1$ of $c$ have exactly predictable shares (divisibility filtration, see the paper). Predicting the shares of the cycles coprime to $c$ is an open question.

## References

* C. Böhm, G. Sontacchi: On the existence of cycles of given length in integer sequences like $x_{n+1}=x_n/2$ if $x_n$ even, and $x_{n+1}=3x_n+1$ otherwise. Atti Accad. Naz. Lincei Rend. 64 (1978), 260–264.
* J. C. Lagarias: The set of rational cycles for the 3x+1 problem. Acta Arithmetica 56 (1990), 33–53.
* R. P. Steiner: A theorem on the Syracuse problem. Congressus Numerantium 20 (1977), 553–559.
* J. Simons, B. de Weger: Theoretical and computational bounds for m-cycles of the 3n+1 problem. Acta Arithmetica 117 (2005), 51–70.
* C. Hercher: There are no Collatz m-cycles with m ≤ 91. Journal of Integer Sequences 26 (2023), Article 23.3.5.
* A. Gupta, I. J. Aberkane, S. Ghosh, A. Abold, A. Rahn, E. Sultanow: Rotating binaries. AppliedMath 2 (2022), 104–117.
