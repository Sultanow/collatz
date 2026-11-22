#!/bin/bash
#SBATCH --job-name=slp-runO
#SBATCH --partition=std
#SBATCH --cpus-per-task=128
#SBATCH --exclusive
#SBATCH --time=12:00:00
#SBATCH --output=/home/esul01/data/outputs/logs/slp_runO_%j.out
# Run O (syracuse_lp, roadmap task A3): hit weights w_d(k, r) of the renormalisation walk started at g_d
# (d = 0, 2, 4, 6) for all k + r <= NMAX, and the first-hit (renewal) decomposition of Cor_N(g_d).
# The 82 length phases theta = j/162 (j = 0..81) run as 82 independent single-threaded processes, PAR of
# them at a time, followed by one merge.
# Measured per process (one phase): NMAX = 13: 4 s, 0.23 GB; 14: 14 s, 0.56 GB. Extrapolated (x3.5 time,
# x3 memory per level): 17: about 10 min and 15 GB; 18: about 35 min and 45 GB.
# Defaults NMAX = 17, PAR = 20 (about 300 GB, fits every std node; about 50 minutes).
# NMAX = 18 only on n12/n13 (768 GB) with PAR = 14 (about 4 hours):
#   NMAX=18 PAR=14 sbatch --nodelist=n12 slurm/slp_runO.sbatch
# Submit from docs/research/syracuse_lp/:   sbatch slurm/slp_runO.sbatch
set -euo pipefail
NMAX=${NMAX:-17}
PAR=${PAR:-20}
OUT=${OUT:-$SLURM_SUBMIT_DIR/results_runO}
cd "$SLURM_SUBMIT_DIR"
set +u
source /home/jupyter/anaconda3/etc/profile.d/conda.sh
conda activate "${CONDA_ENV:-base}"
set -u
export OMP_NUM_THREADS=1 OPENBLAS_NUM_THREADS=1 MKL_NUM_THREADS=1
mkdir -p "$OUT"
echo "slp-runO: NMAX=$NMAX PAR=$PAR host=$(hostname) start=$(date -Is)"
python3 congruence_counts.py selftest
seq 0 81 | xargs -P "$PAR" -I{} \
  sh -c "python3 congruence_counts.py run $NMAX '$OUT' --theta-slice {}/82 > '$OUT/slice_{}.log' 2>&1"
python3 congruence_counts.py merge "$OUT" "$NMAX" "$OUT"/phi_*of82.csv
echo "done $(date -Is): results in $OUT"
