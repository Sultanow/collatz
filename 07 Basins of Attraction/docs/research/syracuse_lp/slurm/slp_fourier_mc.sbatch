#!/bin/bash
#SBATCH --job-name=slp-fmc
#SBATCH --partition=std
#SBATCH --array=0-3
#SBATCH --cpus-per-task=128
#SBATCH --exclusive
#SBATCH --time=08:00:00
#SBATCH --output=/home/esul01/data/outputs/logs/slp_fmc_%A_%a.out
# Job B (syracuse_lp): Monte Carlo of chi_n(2^k) for NMIN <= n <= NMAX (default 16..80) and
# k = round(4(n-1)/3) - W .. + W (default W = 6), SAMPLES per array task (default 2.5e10, about
# 2-3 hours on 128 cores; four tasks give 1e11 samples and a standard error of about 3e-6).
# Submit from docs/research/syracuse_lp/:   sbatch slurm/slp_fourier_mc.sbatch
# Afterwards (login node is fine):
#   python3 mc_merge.py results_mc results_mc/mc_task*.csv --exact results_exact/fourier.csv
set -euo pipefail
NMIN=${NMIN:-16}
NMAX=${NMAX:-80}
SAMPLES=${SAMPLES:-25000000000}
W=${W:-6}
OUT=${OUT:-$SLURM_SUBMIT_DIR/results_mc}
cd "$SLURM_SUBMIT_DIR"
mkdir -p "$OUT"
BIN=./syrac_fourier_mc_$(hostname -s)
[ -x "$BIN" ] || gcc -O3 -march=native -fopenmp -o "$BIN" syrac_fourier_mc.c -lm
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
SEED=$((1000 + SLURM_ARRAY_TASK_ID))
echo "slp-fmc: task=$SLURM_ARRAY_TASK_ID n=$NMIN..$NMAX samples=$SAMPLES W=$W seed=$SEED host=$(hostname) start=$(date -Is)"
"$BIN" "$NMIN" "$NMAX" "$SAMPLES" "$SEED" "$W" > "$OUT/mc_task${SLURM_ARRAY_TASK_ID}.csv"
echo "done $(date -Is)"
