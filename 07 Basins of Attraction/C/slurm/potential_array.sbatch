#!/bin/bash
# Predecessor counts pi_a(N) for all targets a <= A (Run G: the predecessor potential,
# Proposition 8 of the paper). One array task per line "c N A" of CONFIGS.
# Memory: 8 bytes per starting value, 80 GB for N = 1e10. Time: a few minutes per task.
# Submit from the C/ folder:
#   make potential && mkdir -p logs
#   sbatch --array=0-3 --export=ALL slurm/potential_array.sbatch
# Evaluate afterwards (exact identities, mean value and Syracuse profile of a*h(a)):
#   python3 potential_report.py out_potential/potential_c*.csv > ../results/potential/report.txt
#SBATCH --job-name=potential
#SBATCH --partition=std              # adjust to your cluster
#SBATCH --array=0-3                  # (configuration lines) - 1
#SBATCH --cpus-per-task=32
#SBATCH --mem=100G
#SBATCH --time=04:00:00
#SBATCH --output=logs/potential_%A_%a.log

CONFIGS=${CONFIGS:-slurm/potential_configs.txt}; OUTDIR=${OUTDIR:-out_potential}
mkdir -p "$OUTDIR"
LINE=$(( SLURM_ARRAY_TASK_ID + 1 ))
read -r C N A < <(grep -v '^#' "$CONFIGS" | sed -n "${LINE}p")
if [ -z "$C" ]; then exit 0; fi
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
./potential "$C" "$N" "$A" "$OUTDIR/potential_c${C}_N${N}.csv"
