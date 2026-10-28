# 02 Divisions by Two

[![OPUS](https://img.shields.io/badge/OPUS-TH%20N%C3%BCrnberg%202nd%20ed.-orange)](https://opus4.kobv.de/opus4-ohm/frontdoor/index/index/docId/620) [![DOI](https://img.shields.io/badge/DOI-10.34646%2Fthn%2Fohmdok--620-blue)](https://doi.org/10.34646/thn/ohmdok-620) [![Journal](https://img.shields.io/badge/IJPMS-Vol.%2021%2C%202020-yellowgreen)](IJPMS_21_1.pdf) [![License: CC BY 4.0](https://img.shields.io/badge/Paper-CC%20BY%204.0-lightgrey)](https://creativecommons.org/licenses/by/4.0/)

> **Paper:** C. Koch, E. Sultanow, S. Cox, *Divisions by Two in Collatz Sequences: A Data Science Approach*. Working Paper (2nd ed.), Technische Hochschule Nürnberg (2020), [doi:10.34646/thn/ohmdok-620](https://doi.org/10.34646/thn/ohmdok-620). **Read it on [OPUS](https://opus4.kobv.de/opus4-ohm/frontdoor/index/index/docId/620).** The journal version appeared in the International Journal of Pure Mathematical Sciences 21 (2020), 1–13. The publisher's website is no longer available, so the published article is stored in this folder: **[PDF](IJPMS_21_1.pdf)**.

How often can a Collatz sequence divide by two? The paper writes the odd members of a sequence of the map $kx+1$ as

$$v_{n+1}=k^n\,v_1\prod_{i=1}^{n}\beta_i\prod_{i=1}^{n}2^{-\alpha_i},\qquad \beta_i=1+\frac{1}{kv_i},$$

where $\alpha_i$ counts the divisions by two in step $i$ and $\alpha=\sum_i\alpha_i$. Reading the operations in binary (the multiplication by $k$ adds digits, every division by two removes one), it derives

- the maximum number of divisions by two in a sequence with $n$ odd members, $\hat\alpha=\lfloor n\log_2 k+\log_2 v_1\rfloor+1$; a sequence that reaches this maximum ends in 1,
- the bounds $1<\beta\le 2$ for the product $\beta=\prod_i\beta_i$,
- the number of divisions by two that a cycle of length $n$ requires, $\bar\alpha=\lfloor n\log_2 k\rfloor+1$.

Every statement was checked against an empirical data set of 10,000 sequences ($v_1<4000$, $k\in\{1,3,5,7,9\}$) and a cycle search over $v_1<10^4$, $k<1000$. The search finds, among others, the cycles $(13,33,83)$ and $(27,17,43)$ of $5x+1$ and $(27,611)$ and $(35,99)$ of $181x+1$.

## Paper

**Divisions by Two in Collatz Sequences: A Data Science Approach**
Christian Koch, Eldar Sultanow, Sean Cox

| version                | published                                                                               | links                                                                                                                                                   |
|------------------------|-----------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------|
| journal article        | International Journal of Pure Mathematical Sciences 21 (2020), 1–13, SciPress; online 13 November 2020 | doi:10.18052/www.scipress.com/IJPMS.21.1 · [PDF in this folder](IJPMS_21_1.pdf)                                                                         |
| working paper, 2nd ed. | Technische Hochschule Nürnberg, 5 May 2020                                              | [OPUS](https://opus4.kobv.de/opus4-ohm/frontdoor/index/index/docId/620) · [DOI 10.34646/thn/ohmdok-620](https://doi.org/10.34646/thn/ohmdok-620)        |
| working paper, 1st ed. | Technische Hochschule Nürnberg, 17 April 2020 (title without subtitle)                  | [DOI 10.34646/thn/ohmdok-617](https://doi.org/10.34646/thn/ohmdok-617)                                                                                  |

All versions are licensed under CC BY 4.0; this is what allows us to keep the publisher's PDF here.

Local sources of the journal article: [LaTeX sources](TeX/sp) (SciPress template) · [author PDF](TeX/sp/collatz_tree.pdf)

Build: `cd TeX/sp && pdflatex collatz_tree && pdflatex collatz_tree`

#### Citation

```bibtex
@techreport{koch2020divisions,
  title       = {Divisions by Two in Collatz Sequences: A Data Science Approach},
  author      = {Koch, Christian and Sultanow, Eldar and Cox, Sean},
  institution = {Technische Hochschule N{\"u}rnberg Georg Simon Ohm},
  type        = {Working Paper},
  edition     = {2nd},
  year        = {2020},
  month       = may,
  doi         = {10.34646/thn/ohmdok-620},
  url         = {https://opus4.kobv.de/opus4-ohm/frontdoor/index/index/docId/620}
}

@article{koch2020divisions_ijpms,
  title   = {Divisions by Two in Collatz Sequences: A Data Science Approach},
  author  = {Koch, Christian and Sultanow, Eldar and Cox, Sean},
  journal = {International Journal of Pure Mathematical Sciences},
  volume  = {21},
  pages   = {1--13},
  year    = {2020},
  doi     = {10.18052/www.scipress.com/IJPMS.21.1}
}
```

## Code and data

The data set and the cycle finder of the paper are Python scripts by Christian Koch in [c4ristian/collatz](https://github.com/c4ristian/collatz/tree/v1.3), tag `v1.3`:

- [`run_alpha_export.py`](https://github.com/c4ristian/collatz/blob/v1.3/run_alpha_export.py) generates the data set (odd $v_1<4000$, $k\in\{1,3,5,7,9\}$, at most 100 odd members per sequence).
- [`run_cycle_finder.py`](https://github.com/c4ristian/collatz/blob/v1.3/run_cycle_finder.py) searches cycles for odd $v_1<10^4$ and odd $k<1000$ up to length 100.

## Structure

```
02 Divisions by Two/
├── README.md
├── IJPMS_21_1.pdf                 published article (IJPMS 21, 2020)
└── TeX/
    ├── sp/                        author sources of the journal article
    │   ├── collatz_tree.tex
    │   ├── collatz_tree.pdf
    │   └── figures/               maximum of beta for different k (Figure 1)
    └── supplement/                draft of a supplementary note on the upper bound
                                   of the binary growth (not published)
```

## Related

- The representation of the odd members by products of $\beta_i$ comes from [01 Graph Theory](../01%20Graph%20Theory) (Sultanow, Koch, Cox, *Collatz Sequences in the Light of Graph Theory*).
- Worst-case sequences that maximize $\prod_i\beta_i$ via Engel expansions are studied in [03 Engel Expansion](../03%20Engel%20Expansion).
- Cycles of $kx+c$ and their basins of attraction are computed systematically in [07 Basins of Attraction](../07%20Basins%20of%20Attraction).
