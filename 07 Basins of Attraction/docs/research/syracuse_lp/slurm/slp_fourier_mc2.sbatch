#!/bin/bash
#SBATCH --job-name=slp-fmc2
#SBATCH --partition=std
#SBATCH --array=0-3
#SBATCH --cpus-per-task=128
#SBATCH --exclusive
#SBATCH --time=08:00:00
#SBATCH --output=/home/esul01/data/outputs/logs/slp_fmc2_%A_%a.out
# Job B2 (syracuse_lp): rerun of job B with the k-window centred at round(n log2 3) - SHIFT.
# In job B (window centred at 4(n-1)/3) the maximiser left the window from n = 41 on; there
# k* - n log2 3 stayed between -6.4 and -5.4 for 16 <= n <= 40. Defaults: n = 36..80, W = 8, SHIFT = 6,
# SAMPLES = 2.5e10 per array task (about 2 hours on 128 cores).
# Submit from docs/research/syracuse_lp/:   sbatch slurm/slp_fourier_mc2.sbatch
# Afterwards:  python3 mc_merge.py results_mc2 results_mc2/mc_task*.csv
set -euo pipefail
NMIN=${NMIN:-36}
NMAX=${NMAX:-80}
SAMPLES=${SAMPLES:-25000000000}
W=${W:-8}
SHIFT=${SHIFT:-6}
OUT=${OUT:-$SLURM_SUBMIT_DIR/results_mc2}
cd "$SLURM_SUBMIT_DIR"
mkdir -p "$OUT"
BIN=./syrac_fourier_mc2_$(hostname -s)
[ -x "$BIN" ] || gcc -O3 -march=native -fopenmp -o "$BIN" syrac_fourier_mc.c -lm
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
SEED=$((2000 + SLURM_ARRAY_TASK_ID))
echo "slp-fmc2: task=$SLURM_ARRAY_TASK_ID n=$NMIN..$NMAX samples=$SAMPLES W=$W SHIFT=$SHIFT seed=$SEED host=$(hostname) start=$(date -Is)"
"$BIN" "$NMIN" "$NMAX" "$SAMPLES" "$SEED" "$W" "$SHIFT" > "$OUT/mc_task${SLURM_ARRAY_TASK_ID}.csv"
echo "done $(date -Is)"
