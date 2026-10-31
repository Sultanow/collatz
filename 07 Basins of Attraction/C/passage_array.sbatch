#!/bin/bash
# Basin shares far beyond brute force (Theorem 1 and Remark 5 of the paper). Every line
# "c log10y alpha mode n" of CONFIGS is run by REPS array tasks; each task draws n odd
# starting values in [10^log10y, 10^(alpha*log10y)], logarithmically (log) or uniformly
# (unif), and records the basin of the first passage below the table bound B.
# Submit from the C/ folder, e.g.:
#   make passage && mkdir -p logs out
#   sbatch --array=0-299 --export=ALL slurm/passage_array.sbatch   # 30 config lines x REPS=10
# The array size must be (number of config lines) x REPS. Merge afterwards:
#   python3 merge_passage.py out/passage_*.csv --csv ../results/passage_shares.csv
#SBATCH --job-name=passage
#SBATCH --partition=std              # adjust to your cluster
#SBATCH --array=0-299                # (config lines) x REPS - 1 (can be overridden with sbatch --array)
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=500M
#SBATCH --time=04:00:00
#SBATCH --output=logs/passage_%A_%a.log

CONFIGS=${CONFIGS:-slurm/passage_configs.txt}; REPS=${REPS:-10}; B=${B:-10000000}
LINE=$(( SLURM_ARRAY_TASK_ID / REPS + 1 ))
REP=$(( SLURM_ARRAY_TASK_ID % REPS ))
read -r C S A M N < <(grep -v '^#' "$CONFIGS" | grep -v '^[[:space:]]*$' | sed -n "${LINE}p")
if [ -z "$C" ]; then exit 0; fi
SEED=$(( 1000003 * LINE + REP + 1 ))
./passage "$C" "$S" "$A" "$M" "$N" "$SEED" "$B" \
  > "out/passage_c${C}_y${S}_a${A}_${M}_${REP}.csv"
