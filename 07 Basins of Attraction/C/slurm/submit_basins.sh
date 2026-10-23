#!/bin/bash
# Run B: basin census for 3x-1 and all 3x+c with 0 < c <= CMAX, gcd(c,6) = 1, using the cycle
# list of Run A. (For c <= -3 orbits can leave the positive integers; set CMIN to include them,
# such starting values are then reported as "escaped".)
# Usage (from the C/ folder):
#   CSV=../results/rational_cycles_q3_d1000_L40.csv N=100000000000 slurm/submit_basins.sh
CSV=${CSV:-../results/rational_cycles_q3_d1000_L40.csv}
N=${N:-100000000000}; CMIN=${CMIN:--1}; CMAX=${CMAX:-50}; TASKS=${TASKS:-64}; CACHE=${CACHE:-100000000}
mkdir -p logs out
for c in $(seq "$CMIN" "$CMAX"); do
  if [ $(( c % 2 )) -eq 0 ] || [ $(( c % 3 )) -eq 0 ]; then continue; fi
  MINS=$(python3 cycles_for_map.py "$CSV" "$c")
  echo "3x+($c): cycle minima $MINS"
  Q=3 C=$c N=$N CACHE=$CACHE TASKS=$TASKS MINS="$MINS" WINDOWS=1 \
    sbatch --array=0-$(( TASKS - 1 )) --export=ALL slurm/basin_array.sbatch
done
