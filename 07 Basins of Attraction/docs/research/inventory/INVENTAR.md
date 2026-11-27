# D1: Inventar aller Aussagen mit Status und Seitenkosten

Stand: Entwurf v4o, 96 Seiten (davon Literatur ab S. 94). Erzeugt mit `inventory/inventory.py` aus `docs/main.tex`; der Status stammt aus `inventory/status.csv` (von Hand gepflegt).

**Seitenkosten** sind in Seiten Satzspiegel gemessen (Position jedes Elements im PDF via `\pdfsavepos`). Eine Aussage kostet Kopf und Text, der Beweis wird getrennt gezählt; Gleitobjekte, die in einen Beweis fallen, sind abgezogen. Abschnittskosten reichen von der Überschrift bis zur nächsten Überschrift gleicher oder höherer Ebene und enthalten die dort platzierten Gleitobjekte.

## Übersicht nach Status

| Status | Anzahl | Aussagen (S.) | Beweise (S.) |
|---|---|---|---|
| bewiesen | 26 | 5.67 | 12.64 |
| bewiesen, Lean | 2 | 0.50 | 0.52 |
| bewiesen (Literatur) | 2 | 0.99 | 0.00 |
| bewiesen mit Preprint | 4 | 0.64 | 2.14 |
| bedingt | 12 | 3.23 | 6.69 |
| Rechnung | 3 | 0.42 | 0.00 |
| Vermutung | 4 | 0.91 | 0.00 |
| offen | 3 | 1.35 | 0.00 |
| Bemerkung | 13 | 7.13 | 0.00 |
| **alle** | **69** | **20.82** | **21.99** |

## Übersicht nach D2-Vorschlag

Aussagen und Beweise allein sowie hochgerechnet mit dem Faktor Abschnittsseiten / (Aussagen + Beweise) des jeweiligen Abschnitts (grobe Schätzung des Umfangs samt Fließtext, Runs und Tabellen).

| D2-Vorschlag | Anzahl | Aussagen + Beweise (S.) | hochgerechnet (S.) |
|---|---|---|---|
| Anhang | 15 | 17.01 | 25.64 |
| Anhang (Kern F) | 12 | 4.30 | 10.71 |
| Haupttext | 25 | 13.31 | 28.57 |
| Note | 17 | 8.19 | 12.94 |

## Befunde für D2 und D3

*Die Zahlen der Befunde beziehen sich auf v4m (94 Seiten). v4n (Run P im Abschnitt 5, Zeilen in den Tabellen 11 und 12) fügt zwei Seiten hinzu und ändert an den Befunden nichts.*

1. **Umfang.** 94 Seiten. Davon entfallen 42,4 Seiten auf den Haupttext (Abschnitte 1–7) und 47,8 Seiten auf die Anhänge A–F; die Literatur steht auf rund drei Seiten. Die Anhänge sind also länger als der Haupttext.
2. **Vier Blöcke tragen gut zwei Drittel.** Anhang D (22,4 S.), Abschnitt 6 (19,9 S.), Anhang F (14,1 S.) und Anhang C (7,8 S.) ergeben zusammen 64,1 Seiten (68 %). Jede ernsthafte Kompaktierung setzt hier an.
3. **Aussagen gegen Fließtext.** Die 69 nummerierten Aussagen kosten 21,9 Seiten, ihre Beweise 21,8 Seiten, zusammen knapp die Hälfte. Gleitobjekte und lange Tabellen kosten 15,4 Seiten. Davon sind 5,7 Seiten die drei Langtabellen (Beiträge 2,4, Runs 1,6, Reproduktion 1,7). Der Rest, rund 31 Seiten, ist Fließtext: Einleitung, Run-Absätze, Erläuterungen.
4. **Status.** 30 Aussagen sind ohne Vorbehalt bewiesen: 28 mit Beweis im Paper, davon 2 zusätzlich in Lean geprüft (Prop. 10, 11), und 2 aus der Literatur zitiert (Lemma 1, Prop. 4). Vier Aussagen stützen sich auf nicht begutachtete Preprints (Kor. 4, Kor. 5, Satz 2, Lemma 8; zusammen 3,3 S.). Zwölf sind bedingt, davon neun in Anhang D unter den Uniformitäten der Lemmata 3–5 und drei in Anhang F (Kor. 10, 11, 12). Dazu kommen drei vollständige Rechnungen, vier Vermutungen, drei offene Probleme und 13 Bemerkungen.
5. **Bemerkungen sind teuer.** Die 13 Bemerkungen kosten 7,2 Seiten, mehr als der Aussagentext aller 30 unbedingt bewiesenen Aussagen (6,4 S.). Drei davon tragen Beweislast: Bem. 7 (Zerlegung R_X = S_Y + J_X) wird von Prop. 15 benutzt, Bem. 11 (arithmetische Starrheit) von Prop. 13–15, Bem. 12 enthält den Run-K-Befund. Sie sollten als kurze Lemmata gefasst werden, der Rest gekürzt. Fünf Aussagen werden nirgends referenziert: Bem. 1, Beispiel 1, Bem. 2, Bem. 5, Bem. 10.
6. **Anhang D** (22,4 S., 20 Aussagen) ist der größte Block und fast ganz bedingt. Auf dem roten Faden liegen Kor. 7 (Zerlegung), Prop. 8 (Kalibrierung), Prop. 14 und 15 ((UI) ⇒ obere Schranke) und Bem. 12. Gestützt werden sie von Prop. 9 und 12 sowie Bem. 7 und 11. Diese Kette kostet 11,2 Seiten Aussagen und Beweise. Die übrigen Aussagen (6,1 S.: Lyapunov-Zertifikate, Lean-Bezug, Obstruktionen, Problem 3) werden von dieser Kette nicht gebraucht und können in eine Forschungsnotiz unter `docs/research/`, wie bei `syracuse_lp`.
7. **Anhang F** (14,1 S.). Der Kern laut ROADMAP D2, ergänzt um Prop. 20 und Kor. 12, umfasst 12 Aussagen mit 3,9 Seiten Aussagen und Beweisen; samt Run-Absätzen und Tabellen sind das rund 10 Seiten. Lemma 12 (Dilatationsregeln), Kor. 11, Lemma 13, Lemma 14 und die Absätze „The fixed point −1“ und „Run O, all d“ stehen schon vollständig in `syracuse_lp_note` und können ohne Inhaltsverlust dorthin. Auch Lemma 11 und Prop. 18 (logarithmische Karte, Spektralform) braucht die Kette nicht: die a-priori-Schranke H_2(r) ≤ (5/3)^r folgt schon aus Prop. 16 und Cauchy–Schwarz (E_n ≤ (2/3) H_2(n−1)). Sie tragen aber die Deutung der Energie als Gleichverteilung im Winkel und Lemma 13; ob sie bleiben, entscheidet D2. Die ROADMAP nannte für den Kern noch die v4j-Nummern; „Lemma 8, 9“ sind heute Lemma 9 und 10.
8. **Abschnitt 6** (19,9 S.) ist das Schwergewicht des Haupttexts. Größte Teile: Existenz der Dichten 4,4 S., Prädiktor 3,4 S., Potential 2,8 S., Mittelwert und Profil 2,8 S. Die Beweise im Abschnitt kosten 7,1 Seiten, darunter Satz 1 mit 1,6 S. und Prop. 7 mit 1,3 S. Sie wären Kandidaten für den Anhang, falls die Zeitschrift einen kurzen Haupttext verlangt. Das verschiebt Seiten nur, spart aber keine.
9. **Grobe Szenarien für D3.** Die Hochrechnung skaliert Aussagen und Beweise mit dem Fließtextanteil ihres Abschnitts. Die Zahlen sind Größenordnungen, keine Zusagen.
   - Nur die Note-Vorschläge auslagern (17 Aussagen): etwa −13 S., also rund 81 Seiten.
   - Zusätzlich die Reproduktionstabelle ins Repository und die Beiträge-Tabelle auf die Statusspalte verdichten: etwa −3 S., also rund 78 Seiten.
   - Zusätzlich Anhang D auf die Kette aus Punkt 6 mit gekürzten Beweisen bringen (hochgerechnet etwa 14,5 → 8 S.): rund 71 Seiten.
   - Weniger als etwa 65 Seiten ginge nur, wenn Ergebnisse herausfallen oder das Paper geteilt wird. Laut Leitplanke soll es keine Serie von Mini-Papern geben. D3 sollte deshalb eine Zeitschrift wählen, die lange Arbeiten annimmt.
10. **Für D4 notiert.** Die Tabelle in Anhang A hat weder Nummer noch Unterschrift. Vor der Einreichung ist der Status der vier Preprints (Mazur ×2, El Jaouhari, Allikvere) zu prüfen, denn von ihm hängt der Status von Kor. 4, Kor. 5, Satz 2 und Lemma 8 ab. Nur Prop. 10 und 11 entsprechen direkt einem Lean-Modul. Die übrigen Lean-Module belegen Identitäten in Beweisen und im Text; das deckt die letzte Zeile der Beiträge-Tabelle ab.

## Legende

- **Status:** *bewiesen* (vollständiger Beweis im Paper, gegebenenfalls mit begutachteter Literatur); *bewiesen, Lean* (zusätzlich in Lean 4 geprüft, endlicher Kern); *bewiesen (Literatur)* (zitiert, ohne Beweis im Paper); *bewiesen mit Preprint* (der Beweis stützt sich auf nicht begutachtete Preprints); *bedingt* (die Implikation ist bewiesen, ihre Voraussetzung ist offen; die Voraussetzung steht in der Anmerkung); *Rechnung* (vollständige Rechnung innerhalb angegebener Schranken); *Vermutung*; *offen* (Problem); *Bemerkung*.
- **Rolle:** *Faden* (Glied der zentralen Kette von Satz 3 über die obere Schranke und Korollar 10 zu den Vermutungen 3 und 4); *Hauptergebnis* (eigenständiges Ergebnis); *Stütze* (wird für ein anderes Ergebnis gebraucht); *Ergänzung* (für keine andere Aussage nötig); *Kontext* (Einordnung, Beispiel, Frage).
- **D2:** erster Vorschlag für die Auswahl in D2, noch keine Entscheidung. *Haupttext*, *Anhang*, *Anhang (Kern F)* (der Kern von Anhang F laut ROADMAP D2), *Note* (Forschungsnotizen unter `docs/research/`).

## Seitenbilanz nach Abschnitten

| Abschn. | Titel | ab S. | Seiten | davon Gleitobj. | Aussagen | Beweise (S.) |
|---|---|---|---|---|---|---|
| 1 | Introduction | 1 | 3.81 | 0.31 | 0 | 0.00 |
| 2 | Contributions | 5 | 3.76 | 0.00 | 0 | 0.00 |
| 3 | Local structure: one forest, several trees | 9 | 2.35 | 0.58 | 3 | 0.19 |
| 4 | Parity words and cycles | 11 | 1.81 | 0.26 | 2 | 0.00 |
| 5 | The decoded zoo: cycles live where $2^l\approx q^k$ | 13 | 6.03 | 0.64 | 5 | 0.46 |
| 6 | Measuring forests: basin densities | 19 | 20.10 | 4.55 | 17 | 7.19 |
| 7 | Outlook | 39 | 6.00 | 0.00 | 0 | 0.00 |
| A | Notation | 45 | 1.07 | 0.38 | 0 | 0.00 |
| B | Supplementary remarks on cycles | 46 | 0.58 | 0.00 | 1 | 0.00 |
| C | Proof of Theorem [thm:lower] and Corollary [cor:remains] | 47 | 7.87 | 0.00 | 4 | 3.72 |
| D | First-passage tails: reduction, tightness and obstructions | 55 | 22.20 | 1.15 | 20 | 7.16 |
| E | Natural densities for all maps $3x+c$ | 77 | 2.30 | 0.62 | 1 | 0.70 |
| F | The $3$-adic Syracuse law: $L^p$ regularity and Fourier decay | 79 | 14.62 | 1.33 | 16 | 2.56 |
| | **Haupttext (1–7)** | | **43.85** | | | |
| | **Anhänge (A–F)** | | **48.64** | | | |

## Aussagen

Spalten: **S.** Seite; **Aussage/Beweis** Seitenkosten; **genutzt von** Stellen, die das Label referenzieren (Aussagen und Abschnitte); **Status** siehe Legende; **Rolle** im roten Faden; **D2** erster Vorschlag für die Auswahl (Haupttext / Anhang / Note), noch keine Entscheidung.

### 3. Local structure: one forest, several trees

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Proposition 1 | Local indistinguishability | 10 | 0.12 | 0.19 | bewiesen | Hauptergebnis | Haupttext | Abschn. 1, Abschn. 2 |
| Korollar 1 | (lokale Statistik entscheidet nicht) | 10 | 0.18 | – | bewiesen | Hauptergebnis | Haupttext | Bemerkung 1, Problem 2 |
| Bemerkung 1 | Scope | 11 | 0.17 | – | Bemerkung | Kontext | Haupttext | – |

- *Korollar 1:* unmittelbar aus Prop. 1, ohne eigenen Beweis

### 4. Parity words and cycles

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Lemma 1 | Cycles from words; Böhm–Sontacchi, Lagarias | 12 | 0.26 | – | bewiesen (Literatur) | Stütze | Haupttext | Abschn. 4, Abschn. 5, Korollar 2, Lemma 2 |
| Beispiel 1 | (Zyklus von 5x+971) | 12 | 0.22 | – | Bemerkung | Kontext | Haupttext | – |

- *Lemma 1:* Böhm–Sontacchi, Lagarias; kein Beweis im Paper
- *Beispiel 1:* Kürzbar

### 5. The decoded zoo: cycles live where $2^l\approx q^k$

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Proposition 2 | Census | 13 | 0.12 | – | Rechnung | Hauptergebnis | Haupttext | Abschn. 5, Abschn. 7 |
| Korollar 2 | (kein (8,5)-Zyklus von 3x+1) | 14 | 0.09 | 0.19 | bewiesen | Ergänzung | Anhang | Abschn. 5, Abschn. 7 |
| Proposition 3 | Rational cycles | 15 | 0.17 | – | Rechnung | Hauptergebnis | Haupttext | Abschn. 2, Proposition 2, Abschn. 5, Proposition 6, Abschn. 6, Abschn. 7 |
| Lemma 2 | Half-word collisions | 17 | 0.33 | 0.27 | bewiesen | Stütze | Haupttext | Problem 1, Abschn. 7 |
| Problem 1 | (Güte der Heuristik) | 19 | 0.17 | – | offen | Kontext | Haupttext | Abschn. 7 |

- *Proposition 2:* vollständig innerhalb der Schranken (Runs A, P, I, J)
- *Korollar 2:* Beweis mit endlicher Prüfung von sieben Kandidaten
- *Proposition 3:* 1296 primitive Zyklen, |c| ≤ 1000, l ≤ 66 (Runs A, P); in v4n von l ≤ 40 erweitert
- *Lemma 2:* Alternativ Anhang; neu in v4l; erklärt das Defizit der Heuristik bis auf den Restfaktor 0,86
- *Problem 1:* v4n: Geburtstagsfaktor (10), out of sample bestätigt (Run P); offen: Herleitung von α, Block 49 ≤ l ≤ 56

### 6. Measuring forests: basin densities

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Proposition 4 | Divisibility filtration | 19 | 0.73 | – | bewiesen (Literatur) | Stütze | Haupttext | Abschn. 6, Problem 2 |
| Proposition 5 | Multiplicative filtration | 21 | 0.17 | 0.17 | bewiesen | Hauptergebnis | Haupttext | Abschn. 1, Abschn. 2, Abschn. 6, Problem 2, Abschn. 7, Abschn. A |
| Proposition 6 | Verification | 22 | 0.12 | – | Rechnung | Ergänzung | Haupttext | Abschn. 6, Abschn. 7 |
| Satz 1 | Logarithmic basin densities | 23 | 0.15 | 1.61 | bewiesen | Hauptergebnis | Haupttext | Abschn. 1, Abschn. 2, Abschn. 6, Proposition 7, Problem 2, Lemma 3, Satz 3, Satz 2 (+3) |
| Korollar 3 | The basin of $1$ for $3x+1$ | 23 | 0.12 | 0.08 | bewiesen | Hauptergebnis | Haupttext | Abschn. 1, Abschn. 6 |
| Bemerkung 2 | Consequences and limits | 26 | 0.22 | – | Bemerkung | Kontext | Haupttext | – |
| Bemerkung 3 | A first-passage formula | 26 | 0.26 | – | Bemerkung | Ergänzung | Note | Abschn. 6, Abschn. 7 |
| Korollar 4 | Natural densities for $3x+1$ | 26 | 0.12 | 0.38 | bewiesen mit Preprint | Hauptergebnis | Haupttext | Abschn. 1, Abschn. 2, Abschn. 6, Problem 2, Satz 2 |
| Bemerkung 4 | Two routes to natural density | 27 | 0.30 | – | Bemerkung | Kontext | Haupttext | Abschn. 6, Bemerkung 7 |
| Satz 2 | Natural densities for all maps $3x+c$ | 27 | 0.12 | 0.52 | bewiesen mit Preprint | Hauptergebnis | Haupttext | Abschn. 1, Abschn. 2, Bemerkung 2, Abschn. 6, Problem 2, Abschn. 7 |
| Proposition 7 | Predecessor potential | 27 | 0.27 | 1.28 | bewiesen | Faden | Haupttext | Abschn. 1, Abschn. 2, Abschn. 6, Korollar 5, Abschn. 7, Satz 3, Korollar 7, Abschn. D (+4) |
| Korollar 5 | No sparse invariant sets for $3x\pm1$ | 29 | 0.17 | 0.55 | bewiesen mit Preprint | Hauptergebnis | Haupttext | Abschn. 1, Abschn. 2, Korollar 6, Abschn. 6 |
| Vermutung 1 | Syracuse profile of the potential | 34 | 0.21 | – | Vermutung | Faden | Haupttext | Abschn. 2, Abschn. 6, Korollar 6, Problem 2, Abschn. 7, Abschn. D, Abschn. F |
| Satz 3 | Lower bound for the potential | 34 | 0.18 | 1.73 | bewiesen | Faden | Haupttext | Abschn. 2, Abschn. 6, Problem 2, Abschn. 7, Abschn. C, Korollar 6, Abschn. D, Proposition 8 (+7) |
| Korollar 6 | What remains of Conjecture [conj:profile] | 34 | 0.25 | 0.88 | bewiesen | Faden | Haupttext | Abschn. 2, Abschn. 6, Problem 2, Abschn. 7, Abschn. C, Bemerkung 7, Abschn. D, Proposition 15 |
| Vermutung 2 | Density formula | 37 | 0.16 | – | Vermutung | Hauptergebnis | Haupttext | Abschn. 1, Abschn. 2, Abschn. 6, Problem 2, Abschn. 7 |
| Problem 2 | (offene Fragen zu Basin-Dichten) | 39 | 0.60 | – | offen | Kontext | Haupttext | Abschn. 1, Bemerkung 3, Abschn. 6, Abschn. 7 |

- *Proposition 4:* Lagarias, Belaga–Mignotte, Wegner; kein Beweis im Paper
- *Proposition 6:* Run B, n ≤ 10^10
- *Satz 1:* Beweis eventuell in den Anhang; stützt sich auf Tao 2022 und GGM 2025 (begutachtet); Beweis 1,6 Seiten
- *Korollar 4:* Mazur 2026 (Preprint)
- *Bemerkung 4:* Kürzen
- *Satz 2:* Annahme im Satz: Allikvere 2026, Abschn. 6 (nicht begutachtet); Beweis in Anhang E
- *Proposition 7:* stützt sich auf GGM 2025; Beweis 1,3 Seiten
- *Korollar 5:* El Jaouhari 2026, Mazur (Prädezessoren) 2026
- *Vermutung 1:* untere Hälfte bewiesen (Satz 3), numerisch gestützt (Runs G, K)
- *Satz 3:* Beweis in Anhang C (1,7 Seiten plus Lemmata 3–6)
- *Korollar 6:* Äquivalenz (a) und Folgerung (b); Beweis in Anhang C
- *Vermutung 2:* 410 Basin-Anteile, Medianfehler etwa 3 %; Umgebung enthält Erläuterungen (0,76 Seiten)
- *Problem 2:* Kürzen; 1,2 Seiten

### B. Supplementary remarks on cycles

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Bemerkung 5 | The case $q=1$ | 46 | 0.22 | – | Bemerkung | Kontext | Note | – |

### C. Proof of Theorem [thm:lower] and Corollary [cor:remains]

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Lemma 3 | Reduction to a window just above the targets | 47 | 0.34 | 0.43 | bewiesen | Stütze | Anhang | Abschn. 2, Satz 3, Abschn. D, Proposition 8, Korollar 7, Bemerkung 7, Proposition 9, Korollar 8 |
| Lemma 4 | Valuations in a logarithmic window | 48 | 0.21 | 0.57 | bewiesen | Stütze | Anhang | Satz 3, Abschn. D, Proposition 8, Bemerkung 7, Bemerkung 8 |
| Lemma 5 | The model | 49 | 0.38 | 2.41 | bewiesen | Stütze | Anhang | Abschn. 2, Satz 3, Abschn. D, Proposition 8, Bemerkung 6, Bemerkung 7, Proposition 9, Bemerkung 8 |
| Lemma 6 | Escape ladder | 53 | 0.23 | 0.31 | bewiesen | Stütze | Anhang | Abschn. 6, Korollar 6, Bemerkung 7 |

- *Lemma 5:* längster Einzelbeweis (2,4 Seiten)
- *Lemma 6:* für Kor. 6(b)

### D. First-passage tails: reduction, tightness and obstructions

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Proposition 8 | Sharp calibration after clipping | 55 | 0.24 | 1.04 | bedingt | Faden | Anhang | Abschn. 2, Abschn. 6, Korollar 7, Bemerkung 7, Abschn. D, Proposition 9, Korollar 8, Proposition 12 |
| Korollar 7 | Two nonnegative obstructions to the mean upper bound | 57 | 0.55 | 0.24 | bedingt | Faden | Anhang | Abschn. 7, Abschn. D, Korollar 9 |
| Bemerkung 6 | Moment criteria | 57 | 0.37 | – | Bemerkung | Ergänzung | Note | Abschn. D |
| Bemerkung 7 | Unbounded orbits and the return term | 58 | 1.55 | – | Bemerkung | Stütze | Anhang | Abschn. 7, Abschn. D, Proposition 15 |
| Proposition 9 | Exponential tightness up to a logarithmic exceptional set | 59 | 0.16 | 1.51 | bedingt | Stütze | Anhang | Abschn. 2, Abschn. 6, Abschn. 7, Korollar 8, Bemerkung 8, Proposition 12, Proposition 13, Proposition 14 (+1) |
| Korollar 8 | Where the clipping tail can live | 61 | 0.36 | 0.60 | bedingt | Ergänzung | Note | Abschn. 7, Bemerkung 6, Bemerkung 8 |
| Bemerkung 8 | What remains, and what the numerics can see | 62 | 0.81 | – | Bemerkung | Kontext | Note | Abschn. 6, Abschn. 7, Abschn. D, Problem 3 |
| Lemma 7 | Finite-horizon residue balance; cf. Terras | 63 | 0.28 | 0.21 | bewiesen | Stütze | Note | Abschn. D |
| Bemerkung 9 | Finite-support obstruction to independent restarts | 63 | 0.42 | – | Bemerkung | Ergänzung | Note | Problem 3, Abschn. D |
| Proposition 10 | Two-level return certificate | 64 | 0.26 | 0.19 | bewiesen, Lean | Ergänzung | Note | Abschn. 7, Korollar 9, Abschn. D, Proposition 11 |
| Korollar 9 | Conditional removal of the clipping tail | 65 | 0.22 | 0.22 | bedingt | Ergänzung | Note | Bemerkung 10 |
| Proposition 11 | Fixed-modulus obstruction | 65 | 0.25 | 0.33 | bewiesen, Lean | Ergänzung | Note | Abschn. 7, Bemerkung 7, Bemerkung 8, Bemerkung 10, Problem 3 |
| Bemerkung 10 | Lean verification and the unresolved step | 66 | 0.31 | – | Bemerkung | Kontext | Note | – |
| Proposition 12 | First-excess transport | 66 | 0.29 | 0.22 | bedingt | Stütze | Anhang | Problem 3, Proposition 13, Proposition 14, Proposition 15, Abschn. D |
| Problem 3 | Weighted occupation of long finite excursions | 67 | 0.58 | – | offen | Kontext | Note | Abschn. 6, Abschn. 7, Abschn. D |
| Bemerkung 11 | Arithmetic rigidity of returns | 68 | 0.61 | – | Bemerkung | Stütze | Anhang | Proposition 13, Abschn. D, Proposition 14, Proposition 15 |
| Proposition 13 | Single-depth concentration criterion | 68 | 0.32 | 0.42 | bedingt | Ergänzung | Note | Abschn. D |
| Proposition 14 | Domination by the stopped potential of odd targets | 70 | 0.36 | 0.61 | bedingt | Faden | Anhang | Abschn. 2, Abschn. 7, Abschn. D, Bemerkung 7, Proposition 15, Bemerkung 12 |
| Proposition 15 | Domination of the returns by the potential | 71 | 0.30 | 1.56 | bedingt | Faden | Anhang | Abschn. 2, Abschn. 7, Abschn. D, Bemerkung 7 |
| Bemerkung 12 | Averages, not maxima | 75 | 1.66 | – | Bemerkung | Faden | Anhang | Abschn. 2, Abschn. 7, Abschn. D, Abschn. F, Korollar 10 |

- *Proposition 8:* Kürzen; Uniformitäten der Lemmata 3–5 (Anhang D, Einleitung)
- *Korollar 7:* Zerlegung S_X = C_X + T_X + R_X; Uniformitäten der Lemmata 3–5 (Anhang D, Einleitung)
- *Bemerkung 7:* Kürzen, als Lemma fassen; Prop. 15 nutzt die Zerlegung R_X = S_Y + J_X; enthält Beweise; 1,5 Seiten
- *Proposition 9:* Kürzen; genutzt von Prop. 12–15; Uniformitäten der Lemmata 3–5 (Anhang D, Einleitung)
- *Korollar 8:* Uniformitäten der Lemmata 3–5 (Anhang D, Einleitung)
- *Lemma 7:* vgl. Terras 1976
- *Proposition 10:* endliche Fassung in ReturnLyapunov.lean
- *Korollar 9:* Annahme: gleichmäßige Lyapunov-Zertifikate
- *Proposition 11:* ReturnLargeShell.lean
- *Proposition 12:* Kürzen; für Prop. 13–15; Uniformitäten der Lemmata 3–5 (Anhang D, Einleitung)
- *Problem 3:* äquivalent zu T_X = o(X)
- *Bemerkung 11:* enthält die Identität, die Prop. 13–15 benutzen; als Lemma fassen
- *Proposition 13:* Uniformitäten der Lemmata 3–5 (Anhang D, Einleitung)
- *Proposition 14:* (UI) ⇒ T_X = o(X); Uniformitäten der Lemmata 3–5 (Anhang D, Einleitung)
- *Proposition 15:* (UI) ⇒ J_X = o(X) und obere Schranke (eq:upper); Uniformitäten der Lemmata 3–5 (Anhang D, Einleitung)
- *Bemerkung 12:* Kürzen; Run K; verbindet (UI) mit dem Fenster 1 < p < 2 (Anhang F); 1,7 Seiten

### E. Natural densities for all maps $3x+c$

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Lemma 8 | One block, two laws | 77 | 0.22 | 0.70 | bewiesen mit Preprint | Stütze | Anhang | Satz 2 |

- *Lemma 8:* Annahme: Allikvere 2026, Abschn. 6

### F. The $3$-adic Syracuse law: $L^p$ regularity and Fourier decay

| Aussage | Titel | S. | Aussage | Beweis | Status | Rolle | D2 | genutzt von |
|---|---|---|---|---|---|---|---|---|
| Lemma 9 | Self-similarity | 80 | 0.23 | 0.11 | bewiesen | Stütze | Anhang (Kern F) | Lemma 10, Abschn. F, Lemma 12 |
| Proposition 16 | The second moment | 80 | 0.20 | 0.17 | bewiesen | Faden | Anhang (Kern F) | Vermutung 3, Abschn. F, Proposition 18 |
| Proposition 17 | The singular set | 81 | 0.09 | 0.09 | bewiesen | Ergänzung | Anhang (Kern F) | Abschn. F |
| Lemma 10 | Kahane–Peyrière inequality | 81 | 0.19 | 0.24 | bewiesen | Faden | Anhang (Kern F) | Abschn. F |
| Korollar 10 | (L^p für p < 2) | 81 | 0.15 | – | bedingt | Faden | Anhang (Kern F) | Korollar 12 |
| Vermutung 3 | Bounded energy | 81 | 0.41 | – | Vermutung | Faden | Anhang (Kern F) | Abschn. 7, Abschn. F, Korollar 12 |
| Lemma 11 | Logarithmic chart | 84 | 0.23 | 0.19 | bewiesen | Stütze | Anhang (Kern F) | Proposition 18, Abschn. F |
| Proposition 18 | Spectral form of the energy | 84 | 0.24 | 0.20 | bewiesen | Stütze | Anhang (Kern F) | Abschn. F, Korollar 11, Lemma 13 |
| Proposition 19 | Singular set and non-resonance | 85 | 0.34 | 0.41 | bewiesen | Stütze | Anhang (Kern F) | Abschn. 7, Abschn. F, Lemma 12, Lemma 14 |
| Vermutung 4 | Decorrelation | 86 | 0.12 | – | Vermutung | Faden | Anhang (Kern F) | Abschn. 7, Abschn. F |
| Lemma 12 | (Fixpunkt −1, Dilatationsregeln) | 87 | 0.24 | 0.33 | bewiesen | Ergänzung | Note | Abschn. F |
| Proposition 20 | First-hit decomposition | 89 | 0.24 | 0.22 | bewiesen | Faden | Anhang (Kern F) | Korollar 11, Abschn. F |
| Korollar 11 | Conditional bound | 89 | 0.13 | 0.17 | bedingt | Ergänzung | Note | Korollar 12, Abschn. F |
| Lemma 13 | Start near the identity | 90 | 0.21 | 0.24 | bewiesen | Ergänzung | Note | Abschn. F |
| Korollar 12 | Conditional bound for the energy | 90 | 0.15 | 0.09 | bedingt | Faden | Anhang (Kern F) | Abschn. 2, Abschn. 7, Abschn. F |
| Lemma 14 | Large deviations | 91 | 0.14 | 0.11 | bewiesen | Ergänzung | Note | Abschn. F |

- *Korollar 10:* Voraussetzung: sup E_n < ∞ (Vermutung 3)
- *Vermutung 3:* numerisch E_n → 0,475 (Runs L, M); reduziert durch Kor. 12
- *Lemma 11:* Für die Kette entbehrlich (a-priori-Schranke aus Prop. 16 und Cauchy–Schwarz); trägt die Spektraldeutung
- *Proposition 18:* Für die Kette entbehrlich (a-priori-Schranke aus Prop. 16 und Cauchy–Schwarz); trägt die Spektraldeutung und Lemma 13
- *Proposition 19:* Kern: (b) und (c)
- *Vermutung 4:* Runs L, N, O stützen sie; für die g_d gleichmäßig bis N = 15
- *Korollar 11:* Voraussetzungen: c_d < 1, Regulärteil beschränkt; der Beweis von Kor. 12 verweist darauf und müsste bei Auslagerung ausgeschrieben werden (drei Zeilen)
- *Korollar 12:* Voraussetzungen: c_E < 1 (numerisch 0,32), Regulärteil der Energie beschränkt (numerisch fallend)

## Tabellen und Abbildungen

| Objekt | Inhalt | S. | Seiten |
|---|---|---|---|
| Tabelle 1 | Non-trivial $qx+1$ cycles for odd $q\leq1000$. Their role for Crandall's conjecture was studied by Franco a… | 2 | 0.31 |
| Tabelle (lang) 2 | What was known, what is added here, and the logical status of each result. | 5 | 2.38 |
| Tabelle 3 | The mirror pair $3x\pm1$: identical local structure (upper rows; the Upward classes are negatives of each o… | 10 | 0.31 |
| Tabelle 4 | Members of the same residue class in different trees of the forest of $3x-1$. For example, $32\to16\to8\to4… | 11 | 0.27 |
| Tabelle 5 | Cycles determined via binary rotation ; $r$ is the left-rotational distance from $B_{\min}$ to $B_{\max}$, … | 13 | 0.26 |
| Tabelle 6 | All positive integer cycles of $T_{q,c}$ with word length $l\le L$ (Proposition [prop:census]), given by th… | 14 | 0.64 |
| Abbildung 1 | Primitive cycles of all maps $3x+c_0$ with $|c_0|\le1000$ per word length $l\le66$: census (bars; Run A for… | 20 | 0.58 |
| Tabelle 7 | Empirical basin densities $d_N$ of all cycles of $3x-1$, $3x+5$ and $3x+13$ (every $n\le10^{10}$ lies in on… | 21 | 0.62 |
| Abbildung 2 | How the forest splits: empirical basin densities for $1\le n\le10^{10}$ (left) and the three basins of $3x-… | 23 | 0.61 |
| Tabelle 8 | Basin shares from $4\cdot10^8$ odd starting values per counting mode in the windows $[y,y^{1.1}]$ with $y=1… | 31 | 0.68 |
| Abbildung 3 | The landscape of basin densities: for each of the $133$ maps $3x+c$ with $1\le c\le400$ and $\gcd(c,6)=1$, … | 32 | 0.50 |
| Tabelle 9 | The potential against the Syracuse law: means of $a\,h(a)$. Run G: over the non-periodic targets $a\le10^6$… | 36 | 0.60 |
| Tabelle 10 | Convergence of the backward-mass predictor in the depth: the mean mass $M$ of ten consecutive levels agains… | 38 | 0.35 |
| Abbildung 4 | Predicted against measured basin shares, one point per cycle: the $410$ primitive cycles of the $98$ maps $… | 38 | 0.60 |
| Tabelle (lang) 11 | The computational runs referred to in the text. The data folders are linked; programs and scripts are liste… | 41 | 1.68 |
| Tabelle (lang) 12 | Programs, scripts and data behind the results of the paper. | 43 | 2.09 |
| Tabelle (ohne Nr.) | – | 46 | 0.38 |
| Tabelle 13 | Run K: the distribution of $v(a)=a\,h(a)$ on the windows $(X/2,X]$, and of the Syracuse profile $v_n$ under… | 74 | 0.71 |
| Abbildung 5 | Share of the mean of $a\,h(a)$ above $M$: the four maps at $X=2^{22}$ (Run K) and the Syracuse profile at t… | 75 | 0.44 |
| Tabelle 14 | Where the natural-density argument of and the first-passage analysis of use the map, and what changes for $… | 78 | 0.62 |
| Tabelle 15 | Run L: unit-frequency energy $E_n$ and moments $H_p(n)$ of the exact Syracuse law. | 82 | 0.29 |
| Tabelle 16 | Run M: fits of the largest Fourier coefficient $\chi^*_n$ over $65$ levels $16\le n\le80$. | 83 | 0.34 |
| Tabelle 17 | Run N: correlations at intertwiners (top) and at maps that are not intertwiners (bottom). $w(h)$ is the tot… | 88 | 0.40 |
| Tabelle 18 | Run O: the split of Proposition [prop:renewal] into the regular part $F_d(N,0)$ and the near-resonance part… | 94 | 0.30 |
| | **Summe** | | **15.97** |

## Feinstruktur (Unterabschnitte und Absatzüberschriften; im Haupttext ab 0,5 Seiten)

| Abschn. | Überschrift | S. | Seiten |
|---|---|---|---|
| 1 | Related work | 3 | 1.05 |
| 1 | Main results | 4 | 0.93 |
| 2 | Machine-checked finite core and its scope | 8 | 0.50 |
| 3 | Local indistinguishability | 10 | 0.71 |
| 3 | Residue classes do not decide membership | 11 | 0.52 |
| 5 | The near miss of $3x+1$ | 14 | 0.59 |
| 5 | All maps at once: rational cycles | 15 | 4.06 |
| 5 | Heuristic | 16 | 1.88 |
| 5 | A birthday factor | 18 | 0.90 |
| 6 | Existence of the densities | 22 | 4.92 |
| 6 | The predecessor potential | 27 | 2.92 |
| 6 | Basin shares far beyond brute force | 30 | 1.40 |
| 6 | The landscape of basin densities | 32 | 1.36 |
| 6 | The mean value and the Syracuse profile of the potential | 33 | 3.49 |
| 6 | A predictor from the backward tree | 36 | 2.76 |
| 7 | Code and data availability | 41 | 4.22 |
| B | The product identity | 46 | 0.32 |
| D | Calibration and decomposition | 55 | 4.02 |
| D | Exponential tightness of the first-passage mass | 59 | 3.76 |
| D | Return certificates and their limits | 63 | 14.00 |
| D | Run K | 73 | 3.54 |
| E | The transfer | 77 | 2.10 |
| F | Run L | 82 | 0.47 |
| F | Fourier maxima (Run M) | 82 | 1.05 |
| F | Logarithmic chart | 83 | 1.31 |
| F | Renormalisation and the singular set | 85 | 1.80 |
| F | The fixed point $-1$ | 87 | 1.46 |
| F | Run N | 88 | 0.29 |
| F | First-hit decomposition | 88 | 2.86 |
| F | Run O | 91 | 0.85 |
| F | Run O, all $d$ | 92 | 0.62 |
| F | What a proof needs | 93 | 1.18 |

