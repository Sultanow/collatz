# 04 Cycles

[![AppliedMath](https://img.shields.io/badge/AppliedMath-2%281%29%2C%202022-orange)](https://www.mdpi.com/2673-9909/2/1/5) [![DOI](https://img.shields.io/badge/DOI-10.3390%2Fappliedmath2010005-blue)](https://doi.org/10.3390/appliedmath2010005) [![arXiv](https://img.shields.io/badge/arXiv-2107.08363-b31b1b)](https://arxiv.org/abs/2107.08363) [![License: CC BY 4.0](https://img.shields.io/badge/Paper-CC%20BY%204.0-lightgrey)](https://creativecommons.org/licenses/by/4.0/)

> **Paper:** A. Gupta, I. J. Aberkane, S. Ghosh, A. Abold, A. Rahn, E. Sultanow, *Rotating Binaries*. AppliedMath 2 (2022), 104–117, [doi:10.3390/appliedmath2010005](https://doi.org/10.3390/appliedmath2010005). **Read it on [MDPI](https://www.mdpi.com/2673-9909/2/1/5)** or as preprint on [arXiv:2107.08363](https://arxiv.org/abs/2107.08363). This folder collects the computations on rotating binaries, parity vectors and $3n+c$ cycles behind the paper, together with further cycle experiments.

Rotate a binary word $B$ of length $l$ with $N_1(B)$ ones and count the left rotations $r$ that lead from its smallest rotation $B_{\min}$ to its largest rotation $B_{\max}$. In many cases the divisibility $l \mid N_1(B)\cdot r+1$ holds. For $B=10110101$ we have $l=8$, $N_1=5$, $B_{\min}=01011011$, $B_{\max}=11011010$, $r=3$ and indeed $8 \mid 5\cdot 3+1$.

The paper connects this observation with cycles. Read as a parity vector, a binary word with $2^l>3^{N_1}$ defines a cycle of the map

$$f_c(x)=\begin{cases}(3x+c)/2 & x \text{ odd},\\ x/2 & x \text{ even},\end{cases}\qquad c=2^l-3^{N_1},$$

and the rotations of the word run through the members of this cycle. In the example $c=13$, and the rotations give the $3n+13$ cycle $(319, 485, 734, 367, 557, 842, 421, 638)$. Building on this, the paper

- gives a closed formula for the smallest member of a $3n+c$ cycle with $N_1$ odd and $N_0$ even members,
- collects necessary conditions for the existence of a cycle and shows that two different primitive cycles never share a parity vector,
- relates the cycles to the bounds $M(l,n)$ of Halbeisen and Hungerbühler and $N(l,n)$ of Cox et al.,
- counts the rotation equivalence classes of binary words of length $l$, on which the sum of the cycle members is constant,
- generalizes everything from $3n+c$ to $kn+c$ cycles.

The divisibility $l \mid N_1(B)\cdot r+1$ itself is still an open question.

## Paper

**Rotating Binaries**
Anant Gupta, Idriss J. Aberkane, Sourangshu Ghosh, Adrian Abold, Alexander Rahn, Eldar Sultanow · AppliedMath 2(1) (2022), 104–117 · received 26 October 2021, accepted 10 January 2022, published 3 February 2022 (open access)

[**MDPI**](https://www.mdpi.com/2673-9909/2/1/5) · [DOI 10.3390/appliedmath2010005](https://doi.org/10.3390/appliedmath2010005) · [PDF](https://www.mdpi.com/2673-9909/2/1/5/pdf) · [arXiv:2107.08363](https://arxiv.org/abs/2107.08363) (preprint, July 2021)

#### Citation

```bibtex
@article{gupta2022rotating,
  title   = {Rotating Binaries},
  author  = {Gupta, Anant and Aberkane, Idriss J. and Ghosh, Sourangshu and
             Abold, Adrian and Rahn, Alexander and Sultanow, Eldar},
  journal = {AppliedMath},
  volume  = {2},
  number  = {1},
  pages   = {104--117},
  year    = {2022},
  doi     = {10.3390/appliedmath2010005},
  url     = {https://www.mdpi.com/2673-9909/2/1/5}
}
```

#### Related publication

The lower bound $N(l,n)$ used in the paper and Darrell Cox's programs in this folder belong to:

- D. Cox, S. Ghosh, E. Sultanow: Generalizing Halbeisen's and Hungerbühler's optimal bounds for the length of Collatz cycles to $3n+c$ cycles. Journal of Mathematics and Computer Science 24 (2021), 330–337, [doi:10.22436/jmcs.024.04.05](https://doi.org/10.22436/jmcs.024.04.05). Preprint: [arXiv:2101.04067](https://arxiv.org/abs/2101.04067).

## Code

| path | content | by |
|------|---------|----|
| `Python/notebook/eldar_rotate.ipynb`, `eldar_parityvector.ipynb` | rotations, $B_{\min}$ and $B_{\max}$, left-rotational distance, the function $z(B)$ and the divisibility for $3n+c$ cycles | E. Sultanow |
| `Python/notebook/eldar_parityvector_k.ipynb`, `eldar_parityvector_k1.ipynb` | the same for $kn+c$ cycles; `_k1` is the background for [07 Basins of Attraction](../07%20Basins%20of%20Attraction) | E. Sultanow |
| `Python/notebook/EquivalenceClasses/`, `Data_Prep.ipynb`, `*.xlsx` | rotation equivalence classes, their cardinalities and the constant sums, see `EquivalenceClasses/README.md` | A. Abold |
| `Python/notebook/playground.ipynb` | $kx+1$ sequences | A. Gupta |
| `CPP/` | brute-force search for cycles of $qx+1$ over ranges of $q$ and starting values, see `CPP/README.md` | Arty |
| `Python/notebook/cycle_verifier.ipynb`, `eldar_cycles.ipynb` | check cycle candidates of $qx+1$ | E. Sultanow |
| `C/darrell/`, `Python/notebook/darrell/` | $3n+c$ cycles of given length and number of odd members, and number-theoretic experiments | D. Cox |
| `Python/notebook/mertens_function/`, `weyl_criterion/`, `farey.ipynb` | Mertens function, Weyl's criterion, Farey series | D. Cox, E. Sultanow |

The notebooks need Python 3 with `numpy`, `pandas`, `scipy`, `sympy` and `matplotlib`.

## Structure

```
04 Cycles/
├── README.md
├── Python/notebook/
│   ├── eldar_rotate.ipynb                 binary rotations and rotational distance
│   ├── eldar_parityvector.ipynb           parity vectors and 3n+c cycles
│   ├── eldar_parityvector_k.ipynb         generalization to kn+c
│   ├── eldar_parityvector_k1.ipynb        kn+c cycles, background for 07
│   ├── EquivalenceClasses/                rotation equivalence classes (with README)
│   ├── Data_Prep.ipynb, *.xlsx            data for the equivalence classes
│   ├── playground.ipynb                   kx+1 sequences
│   ├── cycle_verifier.ipynb               verification of cycle candidates
│   ├── eldar_cycles.ipynb                 qx+1 sequences and cycles
│   ├── darrell/                           Darrell Cox's notebooks
│   ├── mertens_function/                  Mertens function
│   ├── weyl_criterion/                    Weyl's criterion
│   └── farey.ipynb                        Farey series
├── CPP/
│   ├── collatz_cycles.cpp                 cycle search for qx+1
│   ├── parallelize.py                     splits the search over all cores
│   └── README.md
├── C/darrell/                             Darrell Cox's C programs (Farey, Mertens, Möbius, integrals, ...)
└── Mathematica/
    └── generic_discrete_plot.nb
```

## References

- L. Halbeisen, N. Hungerbühler: Optimal bounds for the length of rational Collatz cycles. Acta Arithmetica 78 (1997), 227–239.
- C. Böhm, G. Sontacchi: On the existence of cycles of given length in integer sequences like $x_{n+1}=x_n/2$ if $x_n$ even, and $x_{n+1}=3x_n+1$ otherwise. Atti Accad. Naz. Lincei Rend. 64 (1978), 260–264.
- D. Cox: The $3n+1$ problem: a probabilistic approach. Journal of Integer Sequences 15 (2012).
- O. Grošek, V. Hromada: Rotation-equivalence classes of binary vectors. Tatra Mountains Mathematical Publications 70 (2016), 93–98.
- A. Gupta: On cycles of generalized Collatz sequences. [arXiv:2008.11103](https://arxiv.org/abs/2008.11103) (2020).
