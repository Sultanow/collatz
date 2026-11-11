#!/bin/bash
# Self-tests for potential (a few seconds). Run from the C/ folder: make test-potential
# 1. The counts of all targets agree with a brute-force reference (tests/ref_potential.py)
#    for five maps, including maps with several cycles and with numbers below a cycle
#    whose orbits never return below themselves.
# 2. The exact identities of the predecessor potential hold (potential_report.py).
set -e
tmp=$(mktemp -d)
for c in -1 1 5 7 13; do
  ./potential $c 20000 500 "$tmp/pot_$c.csv" 2>/dev/null
  python3 tests/ref_potential.py $c 20000 500 > "$tmp/ref_$c.csv"
  grep -v '^#' "$tmp/pot_$c.csv" > "$tmp/pot_$c.body"
  cmp -s "$tmp/pot_$c.body" "$tmp/ref_$c.csv" || { echo "FAILED: counts differ from the reference for c=$c"; exit 1; }
done
echo "1/2 counts agree with the brute-force reference (c = -1, 1, 5, 7, 13)"
./potential 5 2000000 20000 "$tmp/big.csv" 2>/dev/null
python3 potential_report.py "$tmp/big.csv" > "$tmp/report.txt"
if grep -q "VIOLATED\|[1-9][0-9]* violations" "$tmp/report.txt"; then echo "FAILED: identity violated"; cat "$tmp/report.txt"; exit 1; fi
grep -q "(all starting values)" "$tmp/report.txt" || { echo "FAILED: basins do not cover all starting values"; exit 1; }
echo "2/2 flow identity, doubling chains and level identity hold exactly (3x+5, N = 2*10^6)"
echo "all potential tests passed"
rm -rf "$tmp"
