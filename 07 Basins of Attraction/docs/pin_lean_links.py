#!/usr/bin/env python3
"""Pin the Lean source links of the paper to one commit.

usage: python3 pin_lean_links.py SHA [--deep] [main.tex]

Replaces .../collatz/blob/<branch-or-sha>/07%20Basins%20of%20Attraction/lean/... by .../blob/SHA/...,
and writes the sentence that names the commit: it replaces the marker line '% lean-pin: ...' or
updates the commit in an existing sentence. --deep: the run included the independent replay.
Use a commit whose run of the "Collatz Lean foundation" workflow is green; the line anchors
(#L70 etc.) must match that commit.
"""
import re
import sys

args = [a for a in sys.argv[1:] if a != "--deep"]
deep = "--deep" in sys.argv[1:]
if len(args) not in (1, 2) or not re.fullmatch(r"[0-9a-f]{7,40}", args[0]):
    sys.exit(__doc__)
sha = args[0]
path = args[1] if len(args) == 2 else "main.tex"
s = open(path).read()
pat = re.compile(r"(github\.com/Sultanow/collatz/blob/)[^/]+(/07\\%20Basins\\%20of\\%20Attraction/lean/)")
s, n = pat.subn(lambda m: m.group(1) + sha + m.group(2), s)
what = ("for which both jobs of the workflow, including the independent replay, ran successfully"
        if deep else "for which the workflow ran successfully")
sentence = ("The links point to commit \\href{https://github.com/Sultanow/collatz/commit/" + sha
            + "}{\\texttt{" + sha[:7] + "}}, " + what + ".\n")
s, k = re.subn(r"^% lean-pin:.*\n", lambda m: sentence, s, flags=re.M)
if not k:
    s, k = re.subn(r"^The links point to commit .*\n", lambda m: sentence, s, flags=re.M)
open(path, "w").write(s)
print(f"{path}: {n} Lean links pinned to {sha}" + ("" if k else " (no sentence marker found)"))
