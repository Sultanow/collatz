#!/bin/bash
# Quick self-tests (about a minute). Run from the C/ folder: make test
set -e
tmp=$(mktemp -d)
echo "1/4 census_all vs independent rational reference (q=3, DMAX=50, L=14)"
./census_all 3 50 14 > "$tmp/a.txt"
python3 merge_census_all.py "$tmp/a.txt" 2>/dev/null | tail -n +2 | sort > "$tmp/c.csv"
python3 tests/ref_census.py 3 50 14 | sort > "$tmp/p.csv"
diff "$tmp/c.csv" "$tmp/p.csv"
echo "2/4 sharding: full run vs 16 shards (q=3, DMAX=1000, L=20)"
./census_all 3 1000 20 > "$tmp/full.txt"
for i in $(seq 0 15); do ./census_all 3 1000 20 1 4 "$i" > "$tmp/s$i.txt"; done
python3 merge_census_all.py "$tmp/full.txt" > "$tmp/full.csv" 2>/dev/null
python3 merge_census_all.py "$tmp"/s*.txt > "$tmp/shards.csv" 2>/dev/null
diff "$tmp/full.csv" "$tmp/shards.csv"
echo "3/4 census_all vs census for 3x+5 and 3x-1 (L=20)"
for c in 5 -1; do
  a=$(python3 cycles_for_map.py "$tmp/full.csv" "$c")
  b=$(./census 3 "$c" 20 | grep -oE "^min=[0-9]+" | cut -d= -f2 | sort -n | tr "\n" " " | sed "s/ $//")
  [ "$a" = "$b" ] || { echo "mismatch for c=$c: [$a] vs [$b]"; exit 1; }
done
echo "4/4 basin: the cache size does not change the counts (3x-1, n < 2^20)"
./basin -w 3 -1 1 1048576 1000 1 5 17 | grep -v "^start" > "$tmp/b1.txt"
./basin -w 3 -1 1 1048576 100000 1 5 17 | grep -v "^start" > "$tmp/b2.txt"
diff "$tmp/b1.txt" "$tmp/b2.txt"
rm -rf "$tmp"
echo "all tests passed"
