#!/bin/bash
# Basin census for 1 <= n <= N, split into TASKS ranges (one array task per range).
# Submit from the C/ folder, e.g.:
#   make && mkdir -p logs out
#   Q=3 C=5 N=1000000000000 MINS="1 5 19 23 187 347" WINDOWS=1 sbatch --export=ALL slurm/basin_array.sbatch
# For many maps at once see slurm/submit_basins.sh. Merge afterwards:
#   python3 merge_basin.py out/basin_q3_c5_N1000000000000_*.txt --csv ../results/basin_q3_c5_windows.csv
#SBATCH --job-name=basin
#SBATCH --partition=std              # adjust to your cluster
#SBATCH --array=0-383                # TASKS - 1 (can be overridden with sbatch --array)
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=2G             # the label cache needs CACHE bytes
#SBATCH --time=24:00:00
#SBATCH --output=logs/basin_%A_%a.log

Q=${Q:-3}; C=${C:-5}; N=${N:-1000000000000}; CACHE=${CACHE:-1000000000}
MINS=${MINS:-"1 5 19 23 187 347"}
TASKS=${TASKS:-${SLURM_ARRAY_TASK_COUNT:-384}}
FLAG=""; if [ "${WINDOWS:-0}" = "1" ]; then FLAG="-w"; fi
CHUNK=$(( (N + TASKS - 1) / TASKS ))
START=$(( 1 + SLURM_ARRAY_TASK_ID * CHUNK ))
END=$(( START + CHUNK ))
if [ "$END" -gt $(( N + 1 )) ]; then END=$(( N + 1 )); fi
if [ "$START" -ge "$END" ]; then exit 0; fi
./basin $FLAG "$Q" "$C" "$START" "$END" "$CACHE" $MINS \
  > "out/basin_q${Q}_c${C}_N${N}_${SLURM_ARRAY_TASK_ID}.txt"
