#!/bin/bash
#SBATCH --job-name=runP
#SBATCH --partition=std
#SBATCH --cpus-per-task=128
#SBATCH --exclusive
#SBATCH --time=12:00:00
#SBATCH --output=/home/esul01/data/outputs/logs/runP_%j.out
# Run P (roadmap C2): meet-in-the-middle census of the primitive cycles of all maps 3x+c with
# |c0| <= DMAX for the word lengths LMIN..LMAX (Run A covered l <= 40 by brute force).
# One process per type (l, k), the most expensive first, PAR at a time; each process keeps
# its two half-lists below MEMGB (larger lists are split into passes).
#
# Cost (local probe: 120-170 ns per half-word residue; l = 41..48 in 5 core-minutes, type (56,28) in
# 92 s and 2.6 GB): 41..64 about 60 core-hours, longest type (64,32) about 30-40 min, wall about 1-2 h;
# 41..66 about 120 core-hours, wall about 3-4 h. Beyond l = 66 the half-lists outgrow the node memory
# and the passes over the residues dominate (not recommended with this program).
#   41..64 on any std node (n11: PAR=64, MEMGB=5, at most 320 GB):
#     make census_all mitm_census && make test-mitm && LMIN=41 LMAX=64 sbatch slurm/run_P.sbatch
#   41..66 on n12/n13 (768 GB; PAR=48, MEMGB=14, at most 672 GB):
#     LMIN=41 LMAX=66 PAR=48 MEMGB=14 sbatch --nodelist=n12 slurm/run_P.sbatch
# Afterwards (any machine):
#   python3 run_P_report.py out_P ../results/rational_cycles_q3_d1000_L40.csv ../results/run_P
# Submit from the C/ folder.
set -euo pipefail
Q=${Q:-3}
DMAX=${DMAX:-1000}
LMIN=${LMIN:-41}
LMAX=${LMAX:-64}
PAR=${PAR:-64}
MEMGB=${MEMGB:-5}
OUT=${OUT:-$SLURM_SUBMIT_DIR/out_P}
cd "$SLURM_SUBMIT_DIR"
set +u
source /home/jupyter/anaconda3/etc/profile.d/conda.sh
conda activate "${CONDA_ENV:-base}"
set -u
mkdir -p "$OUT"
echo "runP: q=$Q DMAX=$DMAX l=$LMIN..$LMAX PAR=$PAR MEMGB=$MEMGB host=$(hostname) start=$(date -Is)"
test -x ./mitm_census || { echo "build mitm_census first (make mitm_census)"; exit 1; }
./mitm_census "$Q" "$DMAX" 1 24 > "$OUT/selftest_L24.txt"     # quick sanity run, compared in the report
python3 run_P_tasks.py "$LMIN" "$LMAX" > "$OUT/tasks_${LMIN}-${LMAX}.txt"
awk -v Q="$Q" -v D="$DMAX" -v M="$MEMGB" -v O="$OUT" \
  '{printf "./mitm_census %s %s %d %d %d %d --mem %s > %s/P_q%s_l%d_k%d.txt\n", Q, D, $1, $1, $2, $2, M, O, Q, $1, $2}' \
  "$OUT/tasks_${LMIN}-${LMAX}.txt" > "$OUT/cmds_${LMIN}-${LMAX}.txt"
xargs -P "$PAR" -I{} sh -c '{}' < "$OUT/cmds_${LMIN}-${LMAX}.txt"
grep -h "^#" "$OUT"/P_q${Q}_l*_k*.txt | awk '{for(i=1;i<=NF;i++) if($i ~ /^(seconds|mismatches)=/){split($i,a,"="); s[a[1]]+=a[2]}} END{printf "types done; CPU %.1f h, mismatches %d\n", s["seconds"]/3600, s["mismatches"]}'
echo "done $(date -Is): outputs in $OUT"
