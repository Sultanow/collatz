#!/usr/bin/env python3
"""Fail unless every `#print axioms` line of AxiomAudit.lean is reported and depends at most on
the standard axioms propext, Classical.choice and Quot.sound.

usage: check_axioms.py LEAN_OUTPUT.txt AxiomAudit.lean
"""
import re
import sys

ALLOWED = {"propext", "Classical.choice", "Quot.sound"}

out = open(sys.argv[1]).read()
audit = open(sys.argv[2]).read()
wanted = re.findall(r"^\s*#print\s+axioms\s+(\S+)", audit, re.M)

errors = []
if re.search(r"(^|\s)error[:\s]", out):
    errors.append("Lean reported an error")
reported = {}
for name, axs in re.findall(r"'([^']+)' depends on axioms:\s*\[([^\]]*)\]", out, re.S):
    reported[name] = {a.strip() for a in axs.replace("\n", " ").split(",") if a.strip()}
for name in re.findall(r"'([^']+)' does not depend on any axioms", out):
    reported[name] = set()

for name in wanted:
    if name not in reported:
        errors.append(f"no axiom report for {name}")
    elif not reported[name] <= ALLOWED:
        errors.append(f"{name} depends on {sorted(reported[name] - ALLOWED)}")

if errors:
    print("axiom audit FAILED:")
    for e in errors:
        print("  " + e)
    sys.exit(1)
used = sorted(set().union(*reported.values())) if reported else []
print(f"axiom audit OK: {len(wanted)} theorems, axioms used: {', '.join(used) or 'none'}")
