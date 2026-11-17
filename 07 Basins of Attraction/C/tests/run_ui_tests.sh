#!/bin/bash
# Run K tests (make test-ui): profile/selftest, brute-force counts through the whole report,
# and, if ./potential is built, its counts against the brute force on a small instance.
set -euo pipefail
cd "$(dirname "$0")/.."
PY=${PY:-python3}
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
$PY ui_report.py selftest
for c in 1 -1 5 13; do
  tests/ref_counts "$c" 300000 16384 > "$TMP/ref_c$c.csv"
  $PY ui_report.py convert "$c" 300000 "$TMP/ref_c$c.csv" "$TMP/counts_c$c.npz" > /dev/null
done
$PY ui_report.py report "$TMP/out" "$TMP"/counts_c1.npz "$TMP"/counts_c-1.npz \
    "$TMP"/counts_c5.npz "$TMP"/counts_c13.npz > "$TMP/report.txt"
bad=$(grep -c 'flow identity: .* [1-9][0-9]* violations' "$TMP/report.txt" || true)
if [ "$bad" != "0" ]; then echo "FAIL: flow identity violated"; exit 1; fi
for f in report_K.txt table_K.tex ui_windows.csv ui_tails.csv ui_dangerous.csv; do
  [ -s "$TMP/out/$f" ] || { echo "FAIL: $f missing"; exit 1; }
done
echo "report on brute-force counts: OK (flow identity exact for all four maps)"
if [ -x ./potential ]; then
  for c in 1 -1 5 13; do
    ./potential "$c" 300000 16384 "$TMP/pot_c$c.csv" > /dev/null
    $PY - "$TMP/pot_c$c.csv" "$TMP/ref_c$c.csv" <<'PYEOF'
import sys
sys.path.insert(0, ".")
from ui_report import read_potential_csv
import numpy as np
a1, n1, c1 = read_potential_csv(sys.argv[1])
a2, n2, c2 = read_potential_csv(sys.argv[2])
assert np.array_equal(a1, a2) and np.array_equal(n1, n2), "counts differ"
assert np.array_equal(c1 > 0, c2 > 0), "periodic targets differ"
PYEOF
  done
  echo "potential agrees with the brute force for c = 1, -1, 5, 13 (N = 3e5, A = 16384)"
else
  echo "(./potential not built: cross-check skipped; run 'make potential' first)"
fi
echo "PASS"
