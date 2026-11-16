#!/bin/bash
# Run I self-test: tail_check (actual orbits, 128-bit), its random-walk model, and the independent
# big-integer reference tests/ref_tail.py must give the same mean of Phi_X/X within 4 standard errors.
set -e
cd "$(dirname "$0")/.."
fail=0
for cfg in "1 12 0.05" "-1 12 0.05" "5 20 0.05"; do
  set -- $cfg
  a=$(./tail_check $1 $2 $3 400000 7 actual 2>/dev/null | awk '/^mean/{print $2, $4}')
  m=$(./tail_check $1 $2 $3 400000 8 model | awk '/^mean/{print $2, $4}')
  r=$(python3 tests/ref_tail.py $1 $2 $3 20000 9)
  res=$(python3 - "$a" "$m" "$r" <<'PY'
import sys, math
(am,a2),(mm,m2)=[map(float,x.split()) for x in sys.argv[1:3]]
rm,rse=map(float,sys.argv[3].split())
se=lambda mu,sec,n: math.sqrt((sec-mu*mu)/n)
sa,sm=se(am,a2,400000),se(mm,m2,400000)
z1=(am-mm)/math.hypot(sa,sm); z2=(am-rm)/math.hypot(sa,rse)
print("%s actual %.4f  model %.4f (z=%+.1f)  reference %.4f (z=%+.1f)"%("ok  " if abs(z1)<4 and abs(z2)<4 else "FAIL",am,mm,z1,rm,z2))
PY
)
  echo "c=$1 log10X=$2 beta=$3: $res"
  case "$res" in FAIL*) fail=1;; esac
done
[ $fail = 0 ] && echo "tail tests passed" || { echo "tail tests FAILED"; exit 1; }
