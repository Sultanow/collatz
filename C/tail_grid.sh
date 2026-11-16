#!/bin/bash
# tail_grid.sh -- Run I: Phi_X/X for actual orbits against the random-walk model (Proposition 13, Remark 9).
# Runs all 18 configurations (c = 1, -1, 5; log10 X = 12, 20, 30; beta = 0.05, 0.005; 4e6 starts each)
# plus 2e7 starts for c = 1, X = 1e30, beta = 0.05, then writes the report.
# Usage (from C/):  make tail_check && bash tail_grid.sh [OUTDIR]      (default ../results/tail)
# About ten minutes on two cores.
set -e
cd "$(dirname "$0")"
OUT=${1:-../results/tail}
mkdir -p "$OUT"
run(){ c=$1; E=$2; b=$3; N=$4; seed=$5; mode=$6
  f=$OUT/${mode}_c${c}_E${E}_b${b}
  ./tail_check $c $E $b $N $seed $mode > $f.raw 2> $f.ovf
  if [ "$mode" = actual ]; then python3 tail_fallback.py $f.raw $f.ovf > $f.txt; else cp $f.raw $f.txt; fi
  rm -f $f.raw; [ -s $f.ovf ] || rm -f $f.ovf; }
seed=10
for b in 0.05 0.005; do for E in 12 20 30; do
  run 1 $E $b 4000000 $((seed++)) model &
  for c in 1 -1 5; do run $c $E $b 4000000 $((seed++)) actual &
    while [ $(jobs -r | wc -l) -ge 2 ]; do sleep 1; done; done
  wait
done; done
# the larger run replaces the 4e6-sample files of this configuration
run 1 30 0.05 20000000 99 actual &
run 1 30 0.05 20000000 98 model &
wait
python3 tail_report.py "$OUT" > "$OUT/report_I.txt"
cat "$OUT/report_I.txt"
