# Collatz Sequences: Manuscripts and Computational Explorations

[![License: CC BY 4.0](https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey)](https://creativecommons.org/licenses/by/4.0/) [![Latest preprint](https://img.shields.io/badge/Latest-Preprints.org%202026-orange)](https://www.preprints.org/manuscript/202610.2552)

This repository collects the manuscripts (TeX) and the computational investigations (C/C++, Python, Mathematica, Blender) of our work on the Collatz problem and its generalizations. Every numbered folder is a self-contained subproject; where a subproject has its own README, it contains the paper reference, build instructions and results.

## Subprojects

| #  | subproject | topic | paper | sources |
|----|------------|-------|-------|---------|
| 01 | [**Graph Theory**](01%20Graph%20Theory) | The inverted Collatz tree: reachability, cycles, binary and ternary trees. The open book where the project started. | [publish.UP 2020](https://doi.org/10.25932/publishup-48214) | TeX · C · Python · Mathematica |
| 02 | [**Divisions by Two**](02%20Divisions%20by%20Two) | Bounds for the number of divisions by two in a sequence, with a data-science approach | [OPUS 2020](https://doi.org/10.34646/thn/ohmdok-620) · [IJPMS 2020 (PDF)](02%20Divisions%20by%20Two/IJPMS_21_1.pdf) | TeX |
| 03 | [**Engel Expansion**](03%20Engel%20Expansion) | Engel expansions as worst-case sequences that maximize the product $\prod_i (1+\frac{1}{3v_i})$ | working draft | TeX · Python · Mathematica |
| 04 | [**Cycles**](04%20Cycles) | Parity vectors, rotating binaries and $3n+c$ cycles; Arty's cycle search, Darrell Cox's experiments | [AppliedMath 2022](https://doi.org/10.3390/appliedmath2010005) | C · C++ · Python · Mathematica |
| 05 | [**Feather**](05%20Feather) | 3D renderings of the Collatz tree, the "Collatz feather" | – | Python · Blender |
| 06 | [**Collatz L-System**](06%20Collatz%20L-System) | Visualizing the Collatz feather as an L-system (master's thesis of Max Henkel) | thesis | Python · Blender · Mathematica |
| 07 | [**Basins of Attraction**](07%20Basins%20of%20Attraction) | Cycles and basins of attraction of the maps $T_{q,c}$: $3x+1$ is conjectured to have one basin, $3x-1$ has three, $3x+5$ six | [Preprints.org 2026](https://doi.org/10.20944/preprints202610.2552.v1) | C · Python · SLURM · TeX |
| 99 | [**Construction Zone**](99%20Construction%20Zone) | Early drafts, e.g. a 2-adic (binary) view of Collatz sequences | – | TeX |

## Publications

Newest first. The folder column shows where the sources and the code live.

| date    | publication | folder |
|---------|-------------|--------|
| 10/2026 | E. Sultanow: *Same Shadows, Different Forests: Local Predecessor Structure and Global Dynamics of Collatz-Type Maps*. Preprints (2026), [doi:10.20944/preprints202610.2552.v1](https://doi.org/10.20944/preprints202610.2552.v1) | [07](07%20Basins%20of%20Attraction) |
| 02/2022 | A. Gupta, I. J. Aberkane, S. Ghosh, A. Abold, A. Rahn, E. Sultanow: *Rotating Binaries*. AppliedMath 2 (2022), 104–117, [doi:10.3390/appliedmath2010005](https://doi.org/10.3390/appliedmath2010005) | [04](04%20Cycles) |
| 08/2021 | A. Rahn, E. Sultanow, M. Henkel, S. Ghosh, I. J. Aberkane: *An Algorithm for Linearizing the Collatz Convergence*. Mathematics 9 (2021), 1898, [doi:10.3390/math9161898](https://doi.org/10.3390/math9161898) | [01](01%20Graph%20Theory) |
| 01/2021 | D. Cox, S. Ghosh, E. Sultanow: *Collatz Cycles and 3n+c Cycles*. [arXiv:2101.04067](https://arxiv.org/abs/2101.04067) | [04](04%20Cycles) |
| 11/2020 | E. Sultanow, C. Koch, S. Cox: *Collatz Sequences in the Light of Graph Theory* (Fifth Version). University of Potsdam, [doi:10.25932/publishup-48214](https://doi.org/10.25932/publishup-48214) | [01](01%20Graph%20Theory) |
| 11/2020 | C. Koch, E. Sultanow, S. Cox: *Divisions by Two in Collatz Sequences: A Data Science Approach*. International Journal of Pure Mathematical Sciences 21 (2020), 1–13, doi:10.18052/www.scipress.com/IJPMS.21.1, [PDF](02%20Divisions%20by%20Two/IJPMS.21.1.pdf) | [02](02%20Divisions%20by%20Two) |
| 05/2020 | C. Koch, E. Sultanow, S. Cox: *Divisions by Two in Collatz Sequences: A Data Science Approach* (2nd ed.). Working Paper, TH Nürnberg, [doi:10.34646/thn/ohmdok-620](https://doi.org/10.34646/thn/ohmdok-620) | [02](02%20Divisions%20by%20Two) |
| 12/2017 | E. Sultanow, D. Volkov, S. Cox: *Introducing a Finite State Machine for Processing Collatz Sequences*. International Journal of Pure Mathematical Sciences 19 (2017), 10–19, doi:10.18052/www.scipress.com/IJPMS.19.10, [PDF](01%20Graph%20Theory/IJPMS.19.10.pdf) | predecessor of [01](01%20Graph%20Theory) |

## Layout

Within a subproject, the sources are sorted by language: `TeX/` for manuscripts, `C/`, `CPP/`, `Python/` (scripts and `notebook/`), `Mathematica/` and `blender/`. Subprojects with a reproducible pipeline (currently 07) also have `docs/`, `results/` and SLURM templates.

## Connect with us

We are on [ResearchGate](https://www.researchgate.net/project/Collatz-Conjecture-4) and open for any question and collaboration. Issues and pull requests are welcome.

## License

[![CC BY 4.0](https://i.creativecommons.org/l/by/4.0/88x31.png)](http://creativecommons.org/licenses/by/4.0/)

This work is licensed under a [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/).
