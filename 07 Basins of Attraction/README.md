# 07 Basins of Attraction

Same local trees, different forests. This section studies maps of the form

$$T_{q,c}(x)=\begin{cases}(qx+c)/2 & x \text{ odd},\\ x/2 & x \text{ even},\end{cases}\qquad q,c \text{ odd},$$

which include the Collatz map ($q=3$, $c=1$), its mirror $3x-1$ ($c=-1$, i.e. $3x+1$ on the negative integers), the maps $3x+d$ and Crandall's $qx+1$. Locally, all of them generate predecessor trees with the same congruence-class statistics. Globally, they differ: $3x+1$ is conjectured to have a single tree (basin), $3x-1$ has three, and $3x+5$ has six.

Three programs make the global side computable:

* `census_all` lists, in **one pass** over all parity words, the primitive cycles of **all** maps $3x+c$ with $|c| \le$ `DMAX` (Lagarias' rational cycles of $3x+1$ with denominator at most `DMAX`).
* `census` does the same for a single map $T_{q,c}$.
* `basin` measures how the positive integers split into the **basins of attraction** of these cycles.

The code accompanies the working paper in [docs](docs). The rotating-binaries background is in [04 Cycles](../04%20Cycles) (notebooks `eldar_parityvector_k1.ipynb` and `eldar_rotate.ipynb`); a brute-force search over many $q$ is Arty's program in [04 Cycles/CPP](../04%20Cycles/CPP).

## Paper

**Same Shadows, Different Forests: Local Tree Structure versus Global Cycles in $qx+c$ Maps** (working draft, October 2026)

[PDF](docs/main.pdf) · [LaTeX sources](docs)

Build: `cd docs && pdflatex main && bibtex main && pdflatex main && pdflatex main`

## Structure

```
07 Basins of Attraction/
├── README.md
├── docs/                      working paper (LaTeX sources and PDF)
├── C/
│   ├── census_all.c           all maps 3x+c, |c| <= DMAX, in one pass (Run A)
│   ├── census.c               cycle census for a single map
│   ├── basin.c                basin counts for given cycles (Run B)
│   ├── merge_census_all.py    merge census_all shards into one CSV
│   ├── cycles_for_map.py      cycles of one map from that CSV (input for basin)
│   ├── merge_census.py        merge census outputs
│   ├── merge_basin.py         merge basin outputs, window shares
│   ├── Makefile               make, make test
│   ├── tests/                 independent reference and self-tests
│   └── slurm/
│       ├── census_all_array.sbatch
│       ├── census_array.sbatch
│       ├── basin_array.sbatch
│       └── submit_basins.sh
└── results/                   census outputs and the rational cycle list
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

or directly `gcc -O2 -o census census.c -lm` and `gcc -O2 -o basin basin.c`. Both programs need `__int128` (GCC or Clang on 64 bit).

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

`unlabeled` must be 0 for every map; otherwise the map has a cycle with a longer word, which is a result in itself.

## Reference results

**Complete cycle lists** (all positive integer cycles with word length $l \le L$; raw outputs in `results/`, produced with the first version of `census.c`, same enumeration, without the `word=` field):

| map | $L$ | cycle minima | type $(l,k)$ | $D=2^l-q^k$ |
|---|---|---|---|---|
| $3x+1$ | 30 | 1 | (2,1) | 1 |
| $3x-1$ | 30 | 1 / 5 / 17 | (1,1) / (3,2) / (11,7) | −1 / −1 / −139 |
| $3x+5$ | 30 | 5 / 1 / 19, 23 / 187, 347 | (2,1) / (3,1) / (5,3) / (27,17) | 1 / 5 / 5 / 5 077 565 |
| $3x+13$ | 30 | 13 / 1 / 211, 227, 251, 259, 283, 287, 319 / 131 | (2,1) / (4,1) / (8,5) / (24,15) | 1 / 13 / 13 / 2 428 309 |
| $5x+1$ | 28 | 1 / 13, 17 | (5,2) / (7,3) | 7 / 3 |
| $181x+1$ | 40 | 27, 35 | (15,2) | 7 |

Cycles sit where $2^l$ and $q^k$ nearly coincide, e.g. $2^{15}-181^2=7$ and $2^7-5^3=3$. The seven cycles of $3x+13$ of type $(8,5)$ are exactly the seven candidates for a $3x+1$ cycle of that type; they fail because none of their members is divisible by $13 = 2^8-3^5$.

**Rational cycles** (`results/rational_cycles_q3_d1000_L28.csv`): all primitive cycles of the maps $3x+c$ with $|c_0| \le 1000$ and word length $l \le 28$. There are 900 of them, 600 with $c_0>0$ and 300 with $c_0<0$, for 150 different values of $|c_0|$.

**Basin shares** for $n \le 10^9$:

| map | basins (cycle minimum: share) |
|---|---|
| $3x-1$ | 1: 0.3269, 5: 0.3250, 17: 0.3482 |
| $3x+5$ | 1: 0.1404, 5: 0.2000, 19: 0.4979, 23: 0.0922, 187: 0.0325, 347: 0.0369 |
| $3x+13$ | 1: 0.4776, 13: 0.0769, 131: 0.2517, 211: 0.0334, 227: 0.0189, 251: 0.0198, 259: 0.0386, 283: 0.0253, 287: 0.0433, 319: 0.0145 |

The basins of the scaled trivial cycles have share exactly $1/d$ (the multiples of $d$). Predicting the other shares from the cycle data is an open question.

## References

* C. Böhm, G. Sontacchi: On the existence of cycles of given length in integer sequences like $x_{n+1}=x_n/2$ if $x_n$ even, and $x_{n+1}=3x_n+1$ otherwise. Atti Accad. Naz. Lincei Rend. 64 (1978), 260–264.
* J. C. Lagarias: The set of rational cycles for the 3x+1 problem. Acta Arithmetica 56 (1990), 33–53.
* R. P. Steiner: A theorem on the Syracuse problem. Congressus Numerantium 20 (1977), 553–559.
* J. Simons, B. de Weger: Theoretical and computational bounds for m-cycles of the 3n+1 problem. Acta Arithmetica 117 (2005), 51–70.
* C. Hercher: There are no Collatz m-cycles with m ≤ 91. Journal of Integer Sequences 26 (2023), Article 23.3.5.
* A. Gupta, I. J. Aberkane, S. Ghosh, A. Abold, A. Rahn, E. Sultanow: Rotating binaries. AppliedMath 2 (2022), 104–117.
