#!/bin/bash
# Self-tests of mitm_census (Run P), about half a minute. Run from the C/ folder: make test-mitm
set -e
tmp=$(mktemp -d)
echo "1/4 mitm_census vs census_all (q=3, DMAX=1000, L=24)"
./census_all 3 1000 24 > "$tmp/a.txt"
./mitm_census 3 1000 1 24 > "$tmp/m.txt"
python3 merge_census_all.py "$tmp/a.txt" > "$tmp/a.csv" 2>/dev/null
python3 merge_census_all.py "$tmp/m.txt" > "$tmp/m.csv" 2>/dev/null
diff "$tmp/a.csv" "$tmp/m.csv"
echo "2/4 other q: mitm_census vs census_all (q=5 and q=7, DMAX=300, L=20)"
for q in 5 7; do
  ./census_all "$q" 300 20 > "$tmp/a$q.txt"
  ./mitm_census "$q" 300 1 20 > "$tmp/m$q.txt"
  python3 merge_census_all.py "$tmp/a$q.txt" > "$tmp/a$q.csv" 2>/dev/null
  python3 merge_census_all.py "$tmp/m$q.txt" > "$tmp/m$q.csv" 2>/dev/null
  diff "$tmp/a$q.csv" "$tmp/m$q.csv"
done
echo "3/4 code paths: memory passes (--mem 0.0001) and one search per divisor (--lcap 1), q=3, L=30..32"
./mitm_census 3 1000 30 32 > "$tmp/p0.txt"
./mitm_census 3 1000 30 32 --mem 0.0001 > "$tmp/p1.txt"
./mitm_census 3 1000 30 32 --lcap 1 > "$tmp/p2.txt"
for f in p0 p1 p2; do grep "^c=" "$tmp/$f.txt" | sort > "$tmp/$f.s"; done
diff "$tmp/p0.s" "$tmp/p1.s"
diff "$tmp/p0.s" "$tmp/p2.s"
grep -q "passes=[2-9]" "$tmp/p1.txt" || { echo "the memory test did not split into passes"; exit 1; }
echo "4/4 the stored census of Run A for l <= 32"
python3 - "$tmp/p0.txt" "$tmp/m.txt" ../results/rational_cycles_q3_d1000_L40.csv <<'EOF'
import csv, re, sys
pat = re.compile(r"c=(-?\d+) min=(\d+) l=(\d+) k=(\d+) D=(-?\d+) word=([01]+)")
got = set()
for p in sys.argv[1:3]:
    for line in open(p):
        m = pat.match(line)
        if m:
            got.add((int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4)), int(m.group(5)), m.group(6)))
ref = {(int(r["c"]), int(r["min"]), int(r["l"]), int(r["k"]), int(r["D"]), r["word"])
       for r in csv.DictReader(open(sys.argv[3])) if int(r["l"]) <= 24 or 30 <= int(r["l"]) <= 32}
if got != ref:
    sys.exit(f"differs from Run A: {len(got)} vs {len(ref)} cycles")
EOF
if grep -h "^#" "$tmp"/*.txt | grep -qE "mismatches=[1-9]"; then echo "parity mismatches"; exit 1; fi
rm -rf "$tmp"
echo "all mitm tests passed"
