#!/bin/bash
# Backward-mass predictor for the primitive cycles of Run D (Problem 2(c) of the paper), Run F.
# One array task per line "c cycle_min" of CYCLES (written by predictor_eval.py --list: the 410
# primitive cycles of the maps with at least two of them). Each task writes the harmonic mass
# of every level 0..D-1 of the backward tree to OUTDIR/bm_c<c>_m<cycle_min>.csv.
# Submit from the C/ folder:
#   make backward_mass && mkdir -p logs
#   python3 predictor_eval.py --list ../results/landscape/landscape_shares.csv > ../results/landscape/primitive_cycles.txt
#   sbatch --array=0-409 --export=ALL slurm/backward_mass_array.sbatch
# Merge and evaluate afterwards (windows of 10 levels show the convergence in depth):
#   cat out_mass/bm_*.csv > ../results/landscape/backward_mass_D70.csv
#   python3 predictor_eval.py ../results/landscape/landscape_shares.csv ../results/landscape/backward_mass_D70.csv \
#     --sweep 10 --csv ../results/landscape/predictions_D70.csv > ../results/landscape/predictor_D70.txt
#SBATCH --job-name=bmass
#SBATCH --partition=std              # adjust to your cluster
#SBATCH --array=0-409                # (lines of CYCLES) - 1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=200M
#SBATCH --time=12:00:00
#SBATCH --output=logs/bmass_%A_%a.log

CYCLES=${CYCLES:-../results/landscape/primitive_cycles.txt}; D=${D:-70}; K=${K:-0.5}; OUTDIR=${OUTDIR:-out_mass}
mkdir -p "$OUTDIR"
LINE=$(( SLURM_ARRAY_TASK_ID + 1 ))
read -r C M < <(sed -n "${LINE}p" "$CYCLES")
if [ -z "$C" ]; then exit 0; fi
echo "$C $M" | ./backward_mass "$D" 0 0 "$K" levels > "$OUTDIR/bm_c${C}_m${M}.csv"
