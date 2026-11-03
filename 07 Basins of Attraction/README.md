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
* `passage` samples the basin shares **far beyond enumeration** (windows up to $10^{220}$). It follows each orbit to its first passage below a table bound; a basin is not changed by first passage (Theorem 1 of the paper, version 2).

The code accompanies the paper [*Same Shadows, Different Forests*](https://www.preprints.org/manuscript/202610.2552) (LaTeX sources in [docs](docs)). The rotating-binaries background is in [04 Cycles](../04%20Cycles) (notebooks `eldar_parityvector_k1.ipynb` and `eldar_rotate.ipynb`); a brute-force search over many $q$ is Arty's program in [04 Cycles/CPP](../04%20Cycles/CPP).

## Paper

**Same Shadows, Different Forests: Local Predecessor Structure and Global Dynamics of Collatz-Type Maps**
Eldar Sultanow · Preprints.org, version 1, posted 26 October 2026 (not peer-reviewed)

**[Preprints.org](https://www.preprints.org/manuscript/202610.2552)** · [DOI 10.20944/preprints202610.2552.v1](https://doi.org/10.20944/preprints202610.2552.v1)

Local working draft v4: [PDF](docs/main.pdf) · [LaTeX sources](docs)

Build: `cd docs && pdflatex main && bibtex main && pdflatex main && pdflatex main`

#### Citation

```bibtex
@article{sultanow2026shadows,
  title   = {Same Shadows, Different Forests: Local Predecessor Structure
             and Global Dynamics of Collatz-Type Maps},
  author  = {Sultanow, Eldar},
  journal = {Preprints},
  year    = {2026},
  month   = oct,
  doi     = {10.20944/preprints202610.2552.v1},
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
├── C/
│   ├── census_all.c           all maps 3x+c, |c| <= DMAX, in one pass (Run A)
│   ├── census.c               cycle census for a single map
│   ├── basin.c                basin counts for given cycles (Run B)
│   ├── passage.c              basin shares far beyond enumeration, via first passage (Run C)
│   ├── merge_census_all.py    merge census_all shards into one CSV
│   ├── cycles_for_map.py      cycles of one map from that CSV (input for basin)
│   ├── merge_census.py        merge census outputs
│   ├── merge_basin.py         merge basin outputs, window shares
│   ├── merge_passage.py       merge passage outputs, pooled shares, replicate check
│   ├── local_structure.py     residue classes of 3x+1 and 3x-1 (Tables 2 and 3)
│   ├── Makefile               make, make test, make passage, make test-passage
│   ├── tests/                 independent reference and self-tests
│   └── slurm/
│       ├── census_all_array.sbatch
│       ├── census_array.sbatch
│       ├── basin_array.sbatch
│       ├── submit_basins.sh
│       ├── passage_array.sbatch
│       └── passage_configs.txt
└── results/                   census outputs, rational cycle list, basin counts
    └── passage/               sampled basin shares (Run C)
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

#### how it works

Each orbit is followed with GMP integers until it first drops below `B`; the basin of that point is read from the table. By basin invariance under first passage (Step 2 of the proof of Theorem 1 of the paper), this is the basin of the starting value. The logarithmic shares estimate the window probabilities of Remark 5, which converge to the logarithmic basin densities; the uniform shares estimate natural counting on the same window.

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

`merge_passage.py` adds the counts of all replicates and prints shares, standard errors, $z$-scores against Table 6, homogeneity tests across the scales, and the comparison of both counting modes. `--pool 20` prints the shares pooled over the windows from $10^{20}$ on (Table 7 of the paper), `--replicates` checks that the replicates of each configuration scatter like independent binomial samples. It also reads its own summary CSV, so Table 7 can be reproduced from `results/passage/passage_shares.csv`:

```console
python3 merge_passage.py ../results/passage/passage_shares.csv --pool 20
```

#### cost

About 3 µs per starting value at $10^{10}$, 25–35 µs at $10^{100}$ and 75 µs at $10^{200}$ on one core.

#### verification

`make test-passage` compares the sampled shares with an exact enumeration of the window $[10^6, 10^{6.6}]$ for both counting modes, repeats the test with a small table bound (so that every orbit has to pass), and compares the GMP path with an independent Python implementation at $[10^{40}, 10^{44}]$. The cycles through 5 of $3x+5$ and through 13 of $3x+13$ consist of the multiples of 5 and 13 and serve as controls of the sampler (shares $1/5$ and $1/13$).

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

**Run B** – basins of $3x-1$ and all $3x+c$, $0 < c \le 50$, $\gcd(c,6)=1$ (18 maps) up to $N=10^{11}$, with window output. The cycle lists come from Run A:

```console
CSV=../results/rational_cycles_q3_d1000_L40.csv N=100000000000 slurm/submit_basins.sh
python3 merge_basin.py out/basin_q3_c5_N100000000000_*.txt --csv ../results/basin_q3_c5_windows.csv
```

`unlabeled` must be 0 for every map; otherwise the map has a cycle with a longer word, which is a result in itself. The columns $n\le10^6$ and $n\le10^8$ of Table 6 come from single runs, e.g. for $3x+5$:

```console
./basin 3 5 1 1000001 1000000 1 5 19 23 187 347 > out/b6.txt && python3 merge_basin.py out/b6.txt > ../results/basin_q3_c5_N1e6.txt
./basin 3 5 1 100000001 1000000 1 5 19 23 187 347 > out/b8.txt && python3 merge_basin.py out/b8.txt > ../results/basin_q3_c5_N1e8.txt
```

**Run C** – basin shares of $3x-1$, $3x+5$ and $3x+13$ in the windows $[10^y, 10^{1.1y}]$, $y = 10, 20, 50, 100, 200$, with $10^8$ logarithmic and $10^8$ uniform starting values per window (30 configurations in `slurm/passage_configs.txt`, 10 replicates each):

```console
make passage && make test-passage && mkdir -p logs out
sbatch --array=0-299 --export=ALL slurm/passage_array.sbatch
python3 merge_passage.py out/passage_*.csv --csv ../results/passage/passage_shares.csv --pool 20 --replicates > ../results/passage/report.txt
```

## Reproduction map

Which artifact produces which result of the paper (tables and figures as numbered in version 2):

| Result | Artifacts (in `C/` unless noted) | Data |
|---|---|---|
| Non-trivial $qx+1$ cycles for odd $q \le 1000$ (Table 1) | brute-force search over $q$: `04 Cycles/CPP` | – |
| Residue classes mod 18 and 288 of $3x\pm1$, forest density (Table 2, upper rows); members of one class in different trees of $3x-1$ (Table 3); residues mod 13 of the near miss $(8,5)$ | `local_structure.py` | printed |
| Cycles by binary rotation (Table 4) | `04 Cycles`: `eldar_parityvector_k1.ipynb`, `eldar_rotate.ipynb` | – |
| Complete cycle lists of single maps (Table 5) | `census.c`, `slurm/census_array.sbatch`, `merge_census.py` | `results/census_q*_L*.txt` |
| All primitive cycles of $3x+c$, $\lvert c_0\rvert \le 1000$, $l \le 40$ (Run A); census vs. heuristic (Figure 1) | `census_all.c`, `slurm/census_all_array.sbatch`, `merge_census_all.py`; `figures/fig1_cycles_vs_heuristic.py` | `results/rational_cycles_q3_d1000_L40.csv` |
| Agreement with Belaga, Mignotte and Morley (OEIS A226607) | `oeis/compare_oeis.py` | `oeis/a226607_first1740.txt` |
| Cycles with longer words ($3x+23$ through 41; $3x+29$ through 3811 and 7055) | `cycles_for_map.py --search 100000` | printed |
| Basin shares up to $10^6$, $10^8$, $10^{10}$ (Table 6); every $n \le 10^{10}$ reaches a known cycle; dyadic windows and the split of 18 forests (Figure 2) | `basin.c`, `slurm/submit_basins.sh`, `slurm/basin_array.sbatch`, `merge_basin.py`; `figures/fig2_basins.py` | `results/basin_q3_c*_N1e{6,8,10}.txt`, `results/basin_q3_c*_windows_N1e10.csv` |
| Basin shares far beyond enumeration, natural vs. logarithmic counting (Table 7) | `passage.c`, `slurm/passage_array.sbatch`, `slurm/passage_configs.txt`, `merge_passage.py --pool 20 --replicates` | `results/passage/passage_shares.csv`, `results/passage/report.txt` |
| Tests | `tests/run_tests.sh` (census vs. exact rational reference `tests/ref_census.py`, shards, cache); `tests/run_passage_tests.sh` (sampling vs. exact enumeration and an independent Python reference) | – |

## Reference results

**Complete cycle lists** (all positive integer cycles with word length $l \le L$; for the maps $3x+c$ from Run A, for $5x+1$ and $181x+1$ from `census.c`):

| map | $L$ | cycle minima | type $(l,k)$ | $D=2^l-q^k$ |
|---|---|---|---|---|
| $3x+1$ | 40 | 1 | (2,1) | 1 |
| $3x-1$ | 40 | 1 / 5 / 17 | (1,1) / (3,2) / (11,7) | −1 / −1 / −139 |
| $3x+5$ | 40 | 5 / 1 / 19, 23 / 187, 347 | (2,1) / (3,1) / (5,3) / (27,17) | 1 / 5 / 5 / 5 077 565 |
| $3x+13$ | 40 | 13 / 1 / 211, 227, 251, 259, 283, 287, 319 / 131 | (2,1) / (4,1) / (8,5) / (24,15) | 1 / 13 / 13 / 2 428 309 |
| $5x+1$ | 28 | 1 / 13, 17 | (5,2) / (7,3) | 7 / 3 |
| $181x+1$ | 40 | 27, 35 | (15,2) | 7 |

Cycles sit where $2^l$ and $q^k$ nearly coincide, e.g. $2^{15}-181^2=7$ and $2^7-5^3=3$. The seven cycles of $3x+13$ of type $(8,5)$ are exactly the seven candidates for a $3x+1$ cycle of that type; they fail because none of their members is divisible by $13 = 2^8-3^5$.

**Rational cycles** (`results/rational_cycles_q3_d1000_L40.csv`, Run A): all primitive cycles of the maps $3x+c$ with $|c_0| \le 1000$ and word length $l \le 40$, from all $2.2\cdot 10^{12}$ parity words. There are 1097 of them, 763 with $c_0>0$ and 334 with $c_0<0$, for 199 different values of $|c_0|$. The heuristic $\sum L(l,k)\,S(D)/|D|$ predicts 1149 (Figure 1 of the paper). A start-value search adds three cycles with longer words: $3x+23$ through 41 ($l=43$) and $3x+29$ through 3811 and 7055 ($l=65$, the convergent $65/41$ of $\log_2 3$).

**Basin shares** for $n \le 10^{10}$ (Run B; all 18 maps in Figure 2 of the paper, every $n \le 10^{10}$ reaches a known cycle; exact counts in `results/basin_q3_c*_N1e10.txt`, for $3x-1$, $3x+5$, $3x+13$ also up to $10^6$ and $10^8$ in `*_N1e6.txt`, `*_N1e8.txt`):

| map | basins (cycle minimum: share) |
|---|---|
| $3x-1$ | 1: 0.32701, 5: 0.32498, 17: 0.34801 |
| $3x+5$ | 1: 0.14035, 5: 0.20000, 19: 0.49792, 23: 0.09223, 187: 0.03253, 347: 0.03697 |
| $3x+13$ | 1: 0.47753, 13: 0.07692, 131: 0.25167, 211: 0.03345, 227: 0.01892, 251: 0.01984, 259: 0.03861, 283: 0.02528, 287: 0.04329, 319: 0.01449 |

**Basin shares at large scales** (Run C, `results/passage/`, Table 7 of the paper): pooled over the windows $[10^y, 10^{1.1y}]$ with $y = 20, 50, 100, 200$ and both counting modes, $8\cdot 10^8$ starting values per map (standard errors at most $2\cdot 10^{-5}$):

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
