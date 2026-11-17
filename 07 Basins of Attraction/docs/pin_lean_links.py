#!/usr/bin/env python3
"""Pin the Lean source links of the paper to one commit.

usage: python3 pin_lean_links.py SHA [main.tex]

Replaces .../collatz/blob/<branch-or-sha>/07%20Basins%20of%20Attraction/lean/... by .../blob/SHA/...
and the marker line '% lean-pin: ...' by a sentence that names the commit. Use the SHA of a commit
whose run of the "Collatz Lean foundation" workflow is green; the line anchors (#L70 etc.) must
match that commit.
"""
import re
import sys

if len(sys.argv) not in (2, 3) or not re.fullmatch(r"[0-9a-f]{7,40}", sys.argv[1]):
    sys.exit(__doc__)
sha = sys.argv[1]
path = sys.argv[2] if len(sys.argv) == 3 else "main.tex"
s = open(path).read()
pat = re.compile(r"(github\.com/Sultanow/collatz/blob/)[^/]+(/07\\%20Basins\\%20of\\%20Attraction/lean/)")
s, n = pat.subn(lambda m: m.group(1) + sha + m.group(2), s)
sentence = (f"The links point to commit \\texttt{{{sha[:7]}}}, for which this workflow ran "
            "successfully.\n")
s, k = re.subn(r"^% lean-pin:.*\n", lambda m: sentence, s, flags=re.M)
open(path, "w").write(s)
print(f"{path}: {n} Lean links pinned to {sha}" + ("" if k else " (marker line not found)"))
