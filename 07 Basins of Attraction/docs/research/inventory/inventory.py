#!/usr/bin/env python3
"""Roadmap D1: inventory of all statements of the paper with their page costs.

Builds an instrumented copy of docs/main.tex (positions of every statement, proof, float, longtable and
sectioning command via \\pdfsavepos), measures the vertical extent of each item in units of the text height,
parses labels and cross-references, merges the curated status file and writes
    inventory.csv   one row per statement (measured columns + curated columns)
    sections.csv    one row per section, subsection and paragraph heading
    floats.csv      one row per table, figure and longtable
    INVENTAR.md     the report (German), see ROADMAP.md, task D1

usage:  python3 inventory.py DOCS_DIR OUT_DIR [--status status.csv] [--keep-build]
        (from docs/research/inventory:  python3 inventory.py ../.. .)
The hand-written findings in befunde.md (same folder as this script) are inserted into INVENTAR.md.
Costs are in pages of text height; a float that is placed inside a proof is subtracted from the proof.
Needs pdflatex and bibtex (TeX Live); about one minute.
"""
import argparse
import csv
import os
import re
import shutil
import subprocess
import sys
import tempfile
from collections import defaultdict

STMT = ["theorem", "lemma", "corollary", "proposition", "problem", "conjecture", "definition", "example", "remark"]
NAME_DE = {"theorem": "Satz", "lemma": "Lemma", "corollary": "Korollar", "proposition": "Proposition",
           "problem": "Problem", "conjecture": "Vermutung", "definition": "Definition", "example": "Beispiel",
           "remark": "Bemerkung"}
NAME_EN = {"theorem": "Theorem", "lemma": "Lemma", "corollary": "Corollary", "proposition": "Proposition",
           "problem": "Problem", "conjecture": "Conjecture", "definition": "Definition", "example": "Example",
           "remark": "Remark"}
SECCMDS = ["section", "subsection", "subsubsection", "paragraph"]

HOOK = r"""
% --- inventory instrumentation (docs/research/inventory/inventory.py) ---
\usepackage{etoolbox}
\makeatletter
\newwrite\inv@out
\immediate\openout\inv@out=\jobname.inv\relax
\newcommand\inv@raw[1]{\pdfsavepos\protected@edef\inv@tmp{#1}\expandafter\inv@w\expandafter{\inv@tmp}}
\newskip\inv@sk
% invisible mark: the last skip is taken out and put back, so \addvspace and \unskip see the same list
\newcommand\inv@put[1]{%
  \ifhmode\ifnum\lastnodetype=-1 \par\fi\fi
  \ifvmode\inv@sk\lastskip\ifdim\inv@sk=\z@\inv@raw{#1}\else\vskip-\inv@sk\inv@raw{#1}\vskip\inv@sk\fi
  \else\ifhmode\inv@sk\lastskip\unskip\inv@raw{#1}\ifdim\inv@sk=\z@\else\hskip\inv@sk\fi
  \else\inv@raw{#1}\fi\fi}
\newcommand\inv@w[1]{\write\inv@out{#1;\thepage;\the\pdflastypos}}
\newcommand\inv@stmt[1]{%
  \AtBeginEnvironment{#1}{\inv@put{B;#1;}}%
  \AfterEndEnvironment{#1}{\inv@put{E;#1;\csname the#1\endcsname}}}
""" + "".join(r"\inv@stmt{%s}" % e for e in STMT) + r"""
\AtBeginEnvironment{proof}{\inv@put{B;proof;}}
\AfterEndEnvironment{proof}{\inv@put{E;proof;}}
\AtBeginEnvironment{longtable}{\inv@put{B;longtable;}}
\AfterEndEnvironment{longtable}{\inv@put{E;longtable;}}
\g@addto@macro\@floatboxreset{\inv@put{B;float;\@captype}}
\AtEndEnvironment{table}{\inv@put{E;float;table}}
\AtEndEnvironment{figure}{\inv@put{E;float;figure}}
\pretocmd{\@startsection}{\inv@put{S;#1;}}{}{\errmessage{inventory: patching startsection failed}}
\AtBeginDocument{\immediate\write\inv@out{H;\the\paperheight;\the\textheight;\the\dimexpr1in+\voffset+\topmargin+\headheight+\headsep\relax}}
\AtEndDocument{\inv@put{END;;}}
\makeatother
% --- end of instrumentation ---
"""


# ---------------------------------------------------------------- parsing the source
def strip_comments(s):
    return "\n".join(re.sub(r"(?<!\\)%.*", "", line) for line in s.split("\n"))


def braced(s, i):
    """s[i] == '{': return (content, index after the closing brace)."""
    assert s[i] == "{"
    depth, j = 0, i
    while j < len(s):
        c = s[j]
        if c == "\\":
            j += 2
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return s[i + 1:j], j + 1
        j += 1
    raise ValueError("unbalanced braces")


def bracketed(s, i):
    """s[i] == '[': optional argument (brace-aware)."""
    depth, j = 0, i + 1
    while j < len(s):
        c = s[j]
        if c == "\\":
            j += 2
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
        elif c == "]" and depth == 0:
            return s[i + 1:j], j + 1
        j += 1
    raise ValueError("unbalanced brackets")


def plain(t):
    """rough plain-text rendering of a TeX title"""
    while "\\texorpdfstring{" in t:
        k = t.index("\\texorpdfstring{")
        a1, j = braced(t, k + len("\\texorpdfstring"))
        _, j = braced(t, j)
        t = t[:k] + a1 + t[j:]
    t = re.sub(r"\\cite(\[[^\]]*\])?\{[^}]*\}", "", t)
    t = re.sub(r"~?\\(?:ref|eqref)\{([^}]*)\}", r" [\1]", t)
    for k, v in {'\\"o': "ö", '\\"a': "ä", '\\"u': "ü", "\\`e": "è", "\\'e": "é", "\\c{c}": "ç"}.items():
        t = t.replace(k, v)
    t = t.replace("~", " ").replace("\\'", "").replace('\\"', "").replace("--", "–")
    t = re.sub(r"\\(emph|textbf|textit|mathrm|operatorname)\{([^}]*)\}", r"\2", t)
    t = re.sub(r"\s+", " ", t)
    return t.strip()


def parse(src):
    s = strip_comments(src)
    line_of = lambda o: s.count("\n", 0, o) + 1  # noqa: E731
    items = []
    app = s.find("\\appendix")
    begdoc = s.find("\\begin{document}")
    # sectioning
    for m in re.finditer(r"\\(section|subsection|subsubsection|paragraph)(\*?)\s*(?=[\[{])", s):
        if m.start() < begdoc:
            continue
        j = m.end()
        if s[j] == "[":
            _, j = bracketed(s, j)
        title, j = braced(s, j)
        items.append(dict(kind="sec", cmd=m.group(1), star=bool(m.group(2)), title=plain(title), start=m.start(),
                          end=j, line=line_of(m.start())))
    # environments
    for m in re.finditer(r"\\begin\{(" + "|".join(STMT + ["proof", "table", "figure", "longtable"]) + r")\}", s):
        env = m.group(1)
        j = m.end()
        opt = ""
        while j < len(s) and s[j] in " \t":
            j += 1
        if j < len(s) and s[j] == "[" and env not in ("table", "figure"):
            opt, j = bracketed(s, j)
        e = s.find("\\end{%s}" % env, j)
        body = s[j:e]
        lab = re.match(r"\s*\\label\{([^}]*)\}", body)
        cap = re.search(r"\\caption\{", body)
        caption = plain(braced(body, cap.end() - 1)[0]) if cap else ""
        labels = re.findall(r"\\label\{([^}]*)\}", body)
        items.append(dict(kind="env", env=env, opt=opt, title=plain(opt), labels=labels, label=lab.group(1) if lab else
                          (labels[0] if labels and env in ("table", "figure", "longtable") else ""),
                          start=m.start(), end=e + len("\\end{%s}" % env), line=line_of(m.start()),
                          caption=caption))
    items.sort(key=lambda x: x["start"])
    refs = [(m.start(), m.group(1)) for m in re.finditer(r"\\(?:ref|eqref|autoref|cref|Cref)\{([^}]*)\}", s)]
    return s, items, refs, app


# ---------------------------------------------------------------- build and positions
def build(docs, keep):
    tmp = tempfile.mkdtemp(prefix="inventory_")
    for f in os.listdir(docs):
        p = os.path.join(docs, f)
        if os.path.isfile(p) and not f.startswith("main.") or f in ("main.tex", "main.bib"):
            shutil.copy(p, tmp)
    src = open(os.path.join(tmp, "main.tex")).read()
    k = src.find("\\begin{document}")
    open(os.path.join(tmp, "main.tex"), "w").write(src[:k] + HOOK + src[k:])
    run = lambda c: subprocess.run(c, cwd=tmp, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)  # noqa
    run(["pdflatex", "-interaction=nonstopmode", "main.tex"])
    run(["bibtex", "main"])
    for _ in range(2):
        run(["pdflatex", "-interaction=nonstopmode", "main.tex"])
    inv = open(os.path.join(tmp, "main.inv")).read().splitlines()
    aux = open(os.path.join(tmp, "main.aux"), errors="replace").read()
    pages = int(re.search(r"Output written on main.pdf \((\d+) page", open(os.path.join(tmp, "main.log"),
                                                                          errors="replace").read()).group(1))
    if keep:
        print("build kept in", tmp)
    else:
        shutil.rmtree(tmp)
    return inv, aux, pages


def parse_inv(inv):
    head = None
    marks = []
    for line in inv:
        f = line.split(";")
        if f[0] == "H":
            head = [int(x.rstrip("pt").split(".")[0]) if False else float(x.rstrip("pt")) for x in f[1:4]]
            continue
        marks.append(dict(t=f[0], what=f[1], arg=f[2], page=int(f[3]), y=int(f[4])))
    ph, th, top = head            # in pt
    sp = 65536.0
    geo = dict(top=(ph - top) * sp, th=th * sp)
    geo["bot"] = geo["top"] - geo["th"]
    return marks, geo


def span(geo, a, b):
    """extent from position a=(page, y) to b in units of the text height"""
    (p1, y1), (p2, y2) = a, b
    if (p2, -y2) < (p1, -y1):
        return 0.0
    if p1 == p2:
        return max(0.0, (y1 - y2) / geo["th"])
    first = max(0.0, (y1 - geo["bot"]) / geo["th"])
    last = max(0.0, (geo["top"] - y2) / geo["th"])
    return min(first, 1.0) + (p2 - p1 - 1) + min(last, 1.0)


def aux_labels(aux):
    out = {}
    for m in re.finditer(r"\\newlabel\{([^}]*)\}\{\{([^}]*)\}\{([^}]*)\}", aux):
        out[m.group(1)] = (m.group(2), m.group(3))
    return out


# ---------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("docs")
    ap.add_argument("out")
    ap.add_argument("--status", default=os.path.join(os.path.dirname(os.path.abspath(__file__)), "status.csv"))
    ap.add_argument("--keep-build", action="store_true")
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    src = open(os.path.join(a.docs, "main.tex")).read()
    s, items, refs, app = parse(src)
    inv, aux, npages = build(a.docs, a.keep_build)
    marks, geo = parse_inv(inv)
    labels = aux_labels(aux)

    # match marks to source items (per kind, in order)
    bq, eq = defaultdict(list), defaultdict(list)
    for mk in marks:
        if mk["t"] == "B":
            key = mk["arg"] if mk["what"] == "float" else mk["what"]
            bq[key].append(mk)
        elif mk["t"] == "E":
            key = mk["arg"] if mk["what"] == "float" else mk["what"]
            eq[key].append(mk)
        elif mk["t"] == "S":
            bq["sec:" + mk["what"]].append(mk)
    endmark = [m for m in marks if m["t"] == "END"][0]
    cnt = defaultdict(int)
    problems = []
    for it in items:
        key = ("sec:" + it["cmd"]) if it["kind"] == "sec" else it["env"]
        i = cnt[key]
        cnt[key] += 1
        if i >= len(bq[key]):
            problems.append(f"no begin mark for {key} #{i + 1} (line {it['line']})")
            continue
        it["b"] = (bq[key][i]["page"], bq[key][i]["y"])
        if it["kind"] == "env":
            if i >= len(eq[key]):
                problems.append(f"no end mark for {key} #{i + 1}")
                continue
            it["e"] = (eq[key][i]["page"], eq[key][i]["y"])
            if it["env"] in STMT:
                it["num"] = eq[key][i]["arg"]
    for key in bq:
        n_src = cnt[key]
        if key.startswith("sec:") and len(bq[key]) > n_src:
            continue                                  # bibliography heading etc.
        if len(bq[key]) != n_src:
            problems.append(f"{key}: {len(bq[key])} marks, {n_src} in the source")
    if problems:
        print("WARNING:\n  " + "\n  ".join(problems))
    bib = [m for m in bq["sec:section"][cnt["sec:section"]:]]
    docend = (bib[0]["page"], bib[0]["y"]) if bib else (endmark["page"], endmark["y"])
    refstart = bib[0]["page"] if bib else npages

    # floats and their extents
    floats = [it for it in items if it["kind"] == "env" and it["env"] in ("table", "figure", "longtable")
              and "b" in it and "e" in it]
    for f in floats:
        f["cost"] = span(geo, f["b"], f["e"])
        if f["env"] != "longtable":
            f["cost"] += 0.02                          # float separation (\textfloatsep), rough

    def float_inside(a, b):
        tot = 0.0
        for f in floats:
            if f["env"] == "longtable":
                continue
            if (a[0], -a[1]) <= (f["b"][0], -f["b"][1]) and (f["e"][0], -f["e"][1]) <= (b[0], -b[1]):
                tot += f["cost"]
        return tot

    # section tree
    secs = [it for it in items if it["kind"] == "sec" and "b" in it]
    level = {"section": 1, "subsection": 2, "subsubsection": 3, "paragraph": 4}
    nsec, letter = 0, 0
    cur = {1: None, 2: None}
    for k, it in enumerate(secs):
        if it["cmd"] == "section" and not it["star"]:
            if app >= 0 and it["start"] > app:
                letter += 1
                it["num"] = chr(64 + letter)
            else:
                nsec += 1
                it["num"] = str(nsec)
        else:
            it["num"] = ""
        nxt = docend
        for jt in secs[k + 1:]:
            if level[jt["cmd"]] <= level[it["cmd"]]:
                nxt = jt["b"]
                break
        it["cost"] = span(geo, it["b"], nxt)
        it["floats"] = float_inside(it["b"], nxt)
        if it["cmd"] == "section":
            cur[1] = it
        it["parent"] = cur[1]

    def section_of(off):
        best = None
        for it in secs:
            if it["start"] <= off and it["cmd"] == "section":
                best = it
        return best

    def para_of(off):
        best = None
        for it in secs:
            if it["start"] <= off:
                best = it
        return best

    # statements, proofs
    stmts = [it for it in items if it["kind"] == "env" and it["env"] in STMT]
    proofs = [it for it in items if it["kind"] == "env" and it["env"] == "proof"]
    bylabel = {st["label"]: st for st in stmts if st["label"]}
    for st in stmts:
        st["proofs"] = []
    for pr in proofs:
        target = None
        m = re.search(r"\\ref\{([^}]*)\}", pr["opt"])
        if m and m.group(1) in bylabel:
            target = bylabel[m.group(1)]
        else:
            prev = [st for st in stmts if st["end"] <= pr["start"]]
            if prev:
                cand = prev[-1]
                between = s[cand["end"]:pr["start"]]
                if len(between.strip()) < 600 and not any(it["start"] > cand["end"] and it["start"] < pr["start"]
                                                          for it in stmts):
                    target = cand
        pr["target"] = target
        if target is not None:
            target["proofs"].append(pr)
        else:
            problems.append(f"proof at line {pr['line']} not attached")
    for st in stmts:
        st["cost"] = span(geo, st["b"], st["e"]) + 0.03 if "e" in st else float("nan")   # + head spacing
        st["pcost"] = sum(max(0.0, span(geo, p["b"], p["e"]) - float_inside(p["b"], p["e"])) + 0.02
                          for p in st["proofs"] if "e" in p)
        st["page"] = st["b"][0] if "b" in st else ""
        st["sec"] = section_of(st["start"])
        st["par"] = para_of(st["start"])
        st["name"] = f"{NAME_DE[st['env']]} {st.get('num', '?')}"

    # references: who uses which label
    units = sorted([(st["start"], st["end"], st) for st in stmts] +
                   [(pr["start"], pr["end"], pr["target"]) for pr in proofs if pr.get("target")],
                   key=lambda t: t[0])
    used_by = defaultdict(list)
    for off, lab in refs:
        for x in lab.split(","):
            x = x.strip()
            host = None
            for a0, a1, st in units:
                if a0 <= off < a1:
                    host = st
            if host is not None:
                name = host["name"]
            else:
                sec = section_of(off)
                name = ("Abschn. " + sec["num"]) if sec is not None and sec["num"] else "Vorspann"
            if host is not None and host.get("label") == x:
                continue
            if name not in used_by[x]:
                used_by[x].append(name)

    # a statement is used wherever one of its labels (or a label inside its proofs) is referenced
    for st in stmts:
        own = set(st.get("labels", [])) | {l for pr in st["proofs"] for l in pr.get("labels", [])}
        if st["label"]:
            own.add(st["label"])
        ub = []
        for l in sorted(own, key=lambda x: (x != st["label"], x)):
            for n in used_by.get(l, []):
                if n != st["name"] and n not in ub:
                    ub.append(n)
        st["ub"] = ub

    # curated status
    status = {}
    if os.path.exists(a.status):
        with open(a.status, newline="") as f:
            for row in csv.DictReader(f):
                status[row["key"]] = row
    missing = []
    for st in stmts:
        key = st["label"] or f"{st['env']}:{st.get('num', '?')}"
        st["key"] = key
        st["cur"] = status.get(key, {})
        if not st["cur"]:
            missing.append(key)
    if missing:
        print("not in the status file:", ", ".join(missing))

    # ---- write CSVs
    with open(os.path.join(a.out, "inventory.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["key", "name", "title", "section", "page", "line", "cost_statement", "cost_proof", "used_by",
                    "status", "role", "d2"])
        for st in stmts:
            w.writerow([st["key"], st["name"], st["title"], st["sec"]["num"] if st["sec"] else "", st["page"],
                        st["line"], f"{st['cost']:.3f}", f"{st['pcost']:.3f}", "; ".join(st["ub"]),
                        st["cur"].get("status", ""), st["cur"].get("role", ""), st["cur"].get("d2", "")])
    with open(os.path.join(a.out, "sections.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["cmd", "num", "title", "page", "line", "cost", "floats_inside"])
        for it in secs:
            w.writerow([it["cmd"] + ("*" if it["star"] else ""), it["num"], it["title"], it["b"][0], it["line"],
                        f"{it['cost']:.3f}", f"{it['floats']:.3f}"])
    with open(os.path.join(a.out, "floats.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["env", "label", "number", "page", "line", "cost", "caption"])
        for fl in floats:
            num = labels.get(fl["label"], ("", ""))[0] if fl["label"] else ""
            w.writerow([fl["env"], fl["label"], num, fl["b"][0], fl["line"], f"{fl['cost']:.3f}", fl["caption"][:160]])

    write_report(a.out, stmts, secs, floats, used_by, labels, npages, refstart, geo, draft_name(a.docs))
    if problems:
        print("problems:", *problems, sep="\n  ")


# ---------------------------------------------------------------- report
STATUS_ORDER = ["bewiesen", "bewiesen, Lean", "bewiesen (Literatur)", "bewiesen mit Preprint", "bedingt", "Rechnung", "numerisch",
                "Vermutung", "offen", "Bemerkung"]


def fmt(x):
    return f"{x:.2f}"


def draft_name(docs):
    """name of the draft: the last '## v...' heading of docs/CHANGES_v5.md (or '?' if there is none)"""
    try:
        heads = re.findall(r"^## (v\w+)", open(os.path.join(docs, "CHANGES_v5.md"), encoding="utf-8").read(), re.M)
    except OSError:
        heads = []
    return heads[-1] if heads else "?"


def write_report(out, stmts, secs, floats, used_by, labels, npages, refstart, geo, draft="?"):
    L = []
    P = L.append
    P("# D1: Inventar aller Aussagen mit Status und Seitenkosten")
    P("")
    P(f"Stand: Entwurf {draft}, {npages} Seiten (davon Literatur ab S. {refstart}). Erzeugt mit "
      "`inventory/inventory.py` aus `docs/main.tex`; der Status stammt aus `inventory/status.csv` (von Hand gepflegt).")
    P("")
    P("**Seitenkosten** sind in Seiten Satzspiegel gemessen (Position jedes Elements im PDF via `\\pdfsavepos`). "
      "Eine Aussage kostet Kopf und Text, der Beweis wird getrennt gezählt; Gleitobjekte, die in einen Beweis fallen, "
      "sind abgezogen. Abschnittskosten reichen von der Überschrift bis zur nächsten Überschrift gleicher oder "
      "höherer Ebene und enthalten die dort platzierten Gleitobjekte.")
    P("")
    P("@@SUMMARY@@")
    P("")
    P("@@BEFUNDE@@")
    P("")
    P("## Legende")
    P("")
    P("- **Status:** *bewiesen* (vollständiger Beweis im Paper, gegebenenfalls mit begutachteter Literatur); "
      "*bewiesen, Lean* (zusätzlich in Lean 4 geprüft, endlicher Kern); *bewiesen (Literatur)* (zitiert, ohne Beweis "
      "im Paper); *bewiesen mit Preprint* (der Beweis stützt sich auf nicht begutachtete Preprints); *bedingt* (die "
      "Implikation ist bewiesen, ihre Voraussetzung ist offen; die Voraussetzung steht in der Anmerkung); *Rechnung* "
      "(vollständige Rechnung innerhalb angegebener Schranken); *Vermutung*; *offen* (Problem); *Bemerkung*.")
    P("- **Rolle:** *Faden* (Glied der zentralen Kette von Satz 3 über die obere Schranke und Korollar 10 zu den "
      "Vermutungen 3 und 4); *Hauptergebnis* (eigenständiges Ergebnis); *Stütze* (wird für ein anderes Ergebnis "
      "gebraucht); *Ergänzung* (für keine andere Aussage nötig); *Kontext* (Einordnung, Beispiel, Frage).")
    P("- **D2:** erster Vorschlag für die Auswahl in D2, noch keine Entscheidung. *Haupttext*, *Anhang*, "
      "*Anhang (Kern F)* (der Kern von Anhang F laut ROADMAP D2), *Note* (Forschungsnotizen unter `docs/research/`).")
    P("")
    # sections overview
    P("## Seitenbilanz nach Abschnitten")
    P("")
    P("| Abschn. | Titel | ab S. | Seiten | davon Gleitobj. | Aussagen | Beweise (S.) |")
    P("|---|---|---|---|---|---|---|")
    tops = [it for it in secs if it["cmd"] == "section" and it["num"]]
    for it in tops:
        inside = [st for st in stmts if st["sec"] is it]
        pc = sum(st["pcost"] for st in inside)
        P(f"| {it['num']} | {it['title']} | {it['b'][0]} | {fmt(it['cost'])} | {fmt(it['floats'])} | {len(inside)} | "
          f"{fmt(pc)} |")
    main_cost = sum(it["cost"] for it in tops if it["num"].isdigit())
    app_cost = sum(it["cost"] for it in tops if not it["num"].isdigit())
    P(f"| | **Haupttext (1–7)** | | **{fmt(main_cost)}** | | | |")
    P(f"| | **Anhänge (A–F)** | | **{fmt(app_cost)}** | | | |")
    P("")
    # per section statements
    P("## Aussagen")
    P("")
    P("Spalten: **S.** Seite; **Aussage/Beweis** Seitenkosten; **genutzt von** Stellen, die das Label referenzieren "
      "(Aussagen und Abschnitte); **Status** siehe Legende; **Rolle** im roten Faden; **D2** erster Vorschlag für die "
      "Auswahl (Haupttext / Anhang / Note), noch keine Entscheidung.")
    P("")
    for it in tops:
        inside = [st for st in stmts if st["sec"] is it]
        if not inside:
            continue
        P(f"### {it['num']}. {it['title']}")
        P("")
        P("| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |")
        P("|---|---|---|---|---|---|---|---|---|")
        for st in inside:
            cur = st["cur"]
            title = st["title"] or cur.get("short", "")
            ub = st["ub"]
            ubs = ", ".join(ub[:8]) + (f" (+{len(ub) - 8})" if len(ub) > 8 else "")
            P(f"| {st['name']} | {title} | {st['page']} | {fmt(st['cost'])} | "
              f"{fmt(st['pcost']) if st['proofs'] else '–'} | {cur.get('status', '?')} | {cur.get('role', '')} | "
              f"{cur.get('d2', '')} | {ubs or '–'} |")
        notes = [(st["name"], st["cur"].get("note", "")) for st in inside if st["cur"].get("note")]
        if notes:
            P("")
            for n, t in notes:
                P(f"- *{n}:* {t}")
        P("")
    # floats
    P("## Tabellen und Abbildungen")
    P("")
    P("| Objekt | Inhalt | S. | Seiten |")
    P("|---|---|---|---|")
    for fl in floats:
        num = labels.get(fl["label"], ("", ""))[0] if fl["label"] else ""
        kind = {"table": "Tabelle", "figure": "Abbildung", "longtable": "Tabelle (lang)"}[fl["env"]]
        cap = fl["caption"]
        cap = cap if len(cap) <= 110 else cap[:107] + "…"
        P(f"| {kind} {num or '(ohne Nr.)'} | {cap or '–'} | {fl['b'][0]} | {fmt(fl['cost'])} |")
    P(f"| | **Summe** | | **{fmt(sum(f['cost'] for f in floats))}** |")
    P("")
    # paragraphs of the big appendices
    P("## Feinstruktur (Unterabschnitte und Absatzüberschriften; im Haupttext ab 0,5 Seiten)")
    P("")
    P("| Abschn. | Überschrift | S. | Seiten |")
    P("|---|---|---|---|")
    for it in secs:
        if it["cmd"] in ("subsection", "paragraph", "subsubsection") and it["parent"] is not None and \
                (not it["parent"]["num"].isdigit() or it["cost"] >= 0.5):
            P(f"| {it['parent']['num']} | {it['title']} | {it['b'][0]} | {fmt(it['cost'])} |")
    P("")
    text = "\n".join(L)
    # summary by status
    agg = defaultdict(lambda: [0, 0.0, 0.0])
    for st in stmts:
        k = st["cur"].get("status", "?")
        agg[k][0] += 1
        agg[k][1] += st["cost"]
        agg[k][2] += st["pcost"]
    S = ["## Übersicht nach Status", "", "| Status | Anzahl | Aussagen (S.) | Beweise (S.) |", "|---|---|---|---|"]
    for k in STATUS_ORDER + sorted(set(agg) - set(STATUS_ORDER)):
        if k in agg:
            n, c, p = agg[k]
            S.append(f"| {k} | {n} | {fmt(c)} | {fmt(p)} |")
    tot = [sum(v[i] for v in agg.values()) for i in range(3)]
    S.append(f"| **alle** | **{tot[0]}** | **{fmt(tot[1])}** | **{fmt(tot[2])}** |")
    # D2 suggestion totals, with a rough prose factor per section
    S += ["", "## Übersicht nach D2-Vorschlag", "",
          "Aussagen und Beweise allein sowie hochgerechnet mit dem Faktor Abschnittsseiten / (Aussagen + Beweise) des "
          "jeweiligen Abschnitts (grobe Schätzung des Umfangs samt Fließtext, Runs und Tabellen).", "",
          "| D2-Vorschlag | Anzahl | Aussagen + Beweise (S.) | hochgerechnet (S.) |", "|---|---|---|---|"]
    fac = {}
    for it in secs:
        if it["cmd"] == "section" and it["num"]:
            inside = [st for st in stmts if st["sec"] is it]
            core = sum(st["cost"] + st["pcost"] for st in inside)
            fac[id(it)] = it["cost"] / core if core > 0 else 0.0
    d2agg = defaultdict(lambda: [0, 0.0, 0.0])
    for st in stmts:
        k = st["cur"].get("d2", "?")
        c = st["cost"] + st["pcost"]
        d2agg[k][0] += 1
        d2agg[k][1] += c
        d2agg[k][2] += c * fac.get(id(st["sec"]), 1.0)
    for k in sorted(d2agg):
        n, c, h = d2agg[k]
        S.append(f"| {k} | {n} | {fmt(c)} | {fmt(h)} |")
    text = text.replace("@@SUMMARY@@", "\n".join(S))
    bf = os.path.join(os.path.dirname(os.path.abspath(__file__)), "befunde.md")
    text = text.replace("@@BEFUNDE@@", open(bf).read().strip() if os.path.exists(bf) else "")
    open(os.path.join(out, "INVENTAR.md"), "w").write(text + "\n")


if __name__ == "__main__":
    main()
