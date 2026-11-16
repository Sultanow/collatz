#!/bin/bash
# Run J: diagnostics of the equidistribution heuristic for primitive cycles (Section 4, Problem 1).
# Usage (from C/): bash run_J.sh [OUTDIR]   (default ../results/heuristic; about 3 minutes on one core; needs python3 with sympy)
set -e
cd "$(dirname "$0")"
OUT=${1:-../results/heuristic}; mkdir -p "$OUT"
gcc -O2 -o word_census word_census.c
gcc -O2 -o algebraic_factor algebraic_factor.c
./word_census 30 20000 > "$OUT/words_L30_C20000.txt"
python3 necklace_counts.py "$OUT/words_L30_C20000.txt" 30 "$OUT/necklaces_L30.json" ../results/rational_cycles_q3_d1000_L40.csv | tee "$OUT/check.txt"
: > "$OUT/algebraic_factor.txt"
for l in $(seq 4 2 28); do for k in $(seq 2 2 $l); do ./algebraic_factor $l $k >> "$OUT/algebraic_factor.txt"; done; done
python3 heuristic_tests.py ../results/rational_cycles_q3_d1000_L40.csv "$OUT/necklaces_L30.json" 30 "$OUT/algebraic_factor.txt" | tee "$OUT/report_J.txt"
