#!/bin/bash
#SBATCH --job-name=runK-counts
#SBATCH --partition=std
#SBATCH --array=0-3
#SBATCH --cpus-per-task=32
#SBATCH --mem=300G
#SBATCH --exclusive
#SBATCH --time=12:00:00
#SBATCH --output=/home/esul01/data/outputs/logs/runK_counts_%A_%a.out
# Submit from C/ (run_K.sh does this). Run K, step 1: predecessor counts pi_a(N) of all targets a <= A with `potential`
# (the program of Run G), one map 3x+c per array task, then conversion to a compact npz.
# Memory: 8 bytes per starting value, i.e. 240 GB for N = 3e10. On rci the scheduler does not
# account memory (AllocMem = 0), so --mem alone would pack several tasks onto one node; --exclusive
# gives every task a whole node (n11: 384 GB, n12/n13: 768 GB).
set -euo pipefail
CS=(1 -1 5 13)
C=${CS[$SLURM_ARRAY_TASK_ID]}
N=${N:-30000000000}
A=${A:-4194304}
RUNK_DIR=${RUNK_DIR:-/home/esul01/data/runK}
cd "$SLURM_SUBMIT_DIR"
mkdir -p "$RUNK_DIR"
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
echo "Run K counts: c=$C N=$N A=$A host=$(hostname) threads=$OMP_NUM_THREADS start=$(date -Is)"
./potential "$C" "$N" "$A" "$RUNK_DIR/potential_c${C}.csv"
echo "counts written $(date -Is)"
set +u
source /home/jupyter/anaconda3/etc/profile.d/conda.sh
conda activate "${CONDA_ENV:-base}"
set -u
python3 ui_report.py convert "$C" "$N" "$RUNK_DIR/potential_c${C}.csv" "$RUNK_DIR/counts_c${C}.npz"
echo "done $(date -Is)"
