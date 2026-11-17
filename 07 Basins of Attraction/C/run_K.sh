#!/bin/bash
# Run K: submit the counts (array job, four maps) and the report (runs after all counts succeeded).
# Overrides: N, A, MEM, WORK, RES, CONDA_ENV, e.g. a quick smoke test:
#   N=100000000 A=262144 MEM=8G WORK=$HOME/data/runK_smoke RES=$HOME/data/runK_smoke/results bash run_K.sh
set -euo pipefail
cd "$(dirname "$0")"
make potential
mkdir -p /home/esul01/data/outputs/logs
jid=$(sbatch --parsable --mem="${MEM:-300G}" slurm/run_K_counts.sbatch)
rid=$(sbatch --parsable --dependency=afterok:"${jid}" slurm/run_K_report.sbatch)
echo "counts: job ${jid} (array 0-3)   report: job ${rid} (starts after the counts)"
echo "status: squeue -u \$USER"
echo "logs:   /home/esul01/data/outputs/logs/runK_counts_${jid}_*.out, runK_report_${rid}.out"
