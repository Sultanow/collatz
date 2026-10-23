#!/bin/bash
# Exhaustive cycle census, split into 2^S shards (one array task per shard).
# Submit from the C/ folder, e.g.:
#   mkdir -p logs out
#   Q=3 C=5 LMAX=40 LMIN=31 sbatch --export=ALL slurm/census_array.sbatch
# Merge afterwards:
#   python3 merge_census.py out/census_q3_c5_L31-40_*.txt
#SBATCH --job-name=census
#SBATCH --partition=std              # adjust to your cluster
#SBATCH --array=0-511                # must match 2^S - 1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=500M
#SBATCH --time=24:00:00
#SBATCH --output=logs/census_%A_%a.log

Q=${Q:-3}; C=${C:-5}; LMAX=${LMAX:-40}; LMIN=${LMIN:-1}; S=${S:-9}
./census "$Q" "$C" "$LMAX" "$LMIN" "$S" "$SLURM_ARRAY_TASK_ID" \
  > "out/census_q${Q}_c${C}_L${LMIN}-${LMAX}_${SLURM_ARRAY_TASK_ID}.txt"
