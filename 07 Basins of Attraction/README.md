# 07 Basins of Attraction

Same local trees, different forests. This section studies maps of the form

$$T_{q,c}(x)=\begin{cases}(qx+c)/2 & x \text{ odd},\\ x/2 & x \text{ even},\end{cases}\qquad q,c \text{ odd},$$

which include the Collatz map ($q=3$, $c=1$), its mirror $3x-1$ ($c=-1$, i.e. $3x+1$ on the negative integers), the maps $3x+d$ and Crandall's $qx+1$. Locally, all of them generate predecessor trees with the same congruence-class statistics. Globally, they differ: $3x+1$ is conjectured to have a single tree (basin), $3x-1$ has three, and $3x+5$ has six.

Two programs make the global side computable:

* `census` finds **all** positive integer cycles whose parity word has a given length, by exhaustive enumeration of parity words.
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
├── docs/                   working paper (LaTeX sources and PDF)
│   ├── main.tex
│   ├── main.bib
│   └── main.pdf
├── C/
│   ├── census.c            exhaustive cycle census via parity words
│   ├── basin.c             basin counts for given cycles
│   ├── merge_census.py     merge census outputs (runs, shards)
│   ├── merge_basin.py      merge basin outputs (ranges)
│   ├── Makefile
│   └── slurm/
│       ├── census_array.sbatch
│       └── basin_array.sbatch
└── results/                reference outputs of census
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

## basin

#### run

```console
./basin q c start end cache m1 [m2 ...]
```

#### parameters

* `q`, `c`: as above.
* `start`, `end`: range $[start, end)$ of starting values.
* `cache`: size of the label cache $1,\dots,cache$ (one byte per number). Orbits of $n > cache$ are followed until they drop to at most `cache`. The cache is enlarged automatically to the largest cycle member.
* `m1 m2 ...`: smallest members of the known cycles, e.g. from `census`.

Orbits that exceed $2^{120}$ or $10^6$ steps are counted as `unlabeled` (candidates for divergence or for an unknown cycle; for $5x+1$ this is the majority).

#### output

```console
$ ./basin 3 -1 1 100000000 1000000 1 5 17
start=1 end=100000000 cache=1000000 unlabeled=0
m=1 count=32697317 share=0.326973
m=5 count=32470805 share=0.324708
m=17 count=34831877 share=0.348319
```

Counts of several ranges add up; `merge_basin.py` merges them:

```console
python3 merge_basin.py out/basin_q3_c-1_*.txt
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

```console
cd "07 Basins of Attraction/C" && make && mkdir -p logs out
Q=3 C=5 LMAX=40 LMIN=31 sbatch --export=ALL slurm/census_array.sbatch
Q=3 C=5 N=1000000000000 MINS="1 5 19 23 187 347" sbatch --export=ALL slurm/basin_array.sbatch
```

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
