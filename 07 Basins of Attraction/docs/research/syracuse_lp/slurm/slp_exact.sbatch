#!/bin/bash
#SBATCH --job-name=slp-exact
#SBATCH --partition=std
#SBATCH --cpus-per-task=128
#SBATCH --exclusive
#SBATCH --mem=360G
#SBATCH --time=12:00:00
#SBATCH --output=/home/esul01/data/outputs/logs/slp_exact_%j.out
# Job A (syracuse_lp): exact 3-adic Syracuse law for n = 1..NMAX (default 20) with L^p moments,
# unit-frequency energies E_n, twisted correlations, tails and the Fourier sup over the units.
# Submit from docs/research/syracuse_lp/:   sbatch slurm/slp_exact.sbatch
# Peak memory about 210 GB at n = 20 (2.6 GB measured at n = 16, times 81; fits every std node);
# --exclusive because the scheduler does not account memory. Expected run time: two to three hours.
# Output: CSV files in $OUT (default results_exact/).
set -euo pipefail
NMAX=${NMAX:-20}
OUT=${OUT:-$SLURM_SUBMIT_DIR/results_exact}
cd "$SLURM_SUBMIT_DIR"
set +u
source /home/jupyter/anaconda3/etc/profile.d/conda.sh
conda activate "${CONDA_ENV:-base}"
set -u
echo "slp-exact: NMAX=$NMAX host=$(hostname) cpus=$SLURM_CPUS_PER_TASK start=$(date -Is)"
python3 syracuse_lp.py selftest
python3 syracuse_lp.py exact "$NMAX" "$OUT" --fourier-max 20 --dmax 20 --workers "$SLURM_CPUS_PER_TASK"
echo "done $(date -Is): results in $OUT"
