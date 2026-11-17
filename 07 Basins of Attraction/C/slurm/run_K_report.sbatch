#!/bin/bash
#SBATCH --job-name=runK-report
#SBATCH --partition=std
#SBATCH --cpus-per-task=4
#SBATCH --mem=32G
#SBATCH --time=02:00:00
#SBATCH --output=/home/esul01/data/outputs/logs/runK_report_%j.out
# Submit from C/ (run_K.sh does this). Run K, step 2: evaluation of the four count files -> ../results/potential_ui/
set -euo pipefail
WORK=${WORK:-/home/esul01/data/runK}
RES=${RES:-../results/potential_ui}
cd "$SLURM_SUBMIT_DIR"
set +u
source /home/jupyter/anaconda3/etc/profile.d/conda.sh
conda activate "${CONDA_ENV:-base}"
set -u
echo "Run K report: start $(date -Is) on $(hostname)"
python3 ui_report.py selftest
python3 ui_report.py report "$RES" "$WORK/counts_c1.npz" "$WORK/counts_c-1.npz" \
    "$WORK/counts_c5.npz" "$WORK/counts_c13.npz"
cp "$WORK"/counts_c*.npz "$RES"/
echo "done $(date -Is): results in $RES"
