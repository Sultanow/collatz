#!/bin/bash
# Run A: census of all primitive cycles of 3x+c with |c| <= DMAX in ONE pass over all
# parity words, split into 2^S shards (one array task per shard).
# Submit from the C/ folder, e.g.:
#   make && mkdir -p logs out
#   Q=3 DMAX=1000 LMAX=40 sbatch --export=ALL slurm/census_all_array.sbatch
# Merge afterwards (mismatches must be 0):
#   python3 merge_census_all.py out/census_all_q3_d1000_L1-40_*.txt > ../results/rational_cycles_q3_d1000_L40.csv
#SBATCH --job-name=census_all
#SBATCH --partition=std              # adjust to your cluster
#SBATCH --array=0-511                # must match 2^S - 1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=200M
#SBATCH --time=12:00:00              # L=40: well below 1 h per shard; L=44: a few hours
#SBATCH --output=logs/census_all_%A_%a.log

Q=${Q:-3}; DMAX=${DMAX:-1000}; LMAX=${LMAX:-40}; LMIN=${LMIN:-1}; S=${S:-9}
./census_all "$Q" "$DMAX" "$LMAX" "$LMIN" "$S" "$SLURM_ARRAY_TASK_ID" \
  > "out/census_all_q${Q}_d${DMAX}_L${LMIN}-${LMAX}_${SLURM_ARRAY_TASK_ID}.txt"
