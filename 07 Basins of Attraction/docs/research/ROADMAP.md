# Schlachtplan und roter Faden (Stand 22.11.2026, nach Entwurf v4j)

Die Sammelphase ist abgeschlossen. Diese Datei ist der rote Faden für alles Weitere. Sie wird bei
jedem Schritt aktualisiert (Spalte Status). Details stehen in den `STATUS.md` der Unterordner.

## Der rote Faden

Das Paper hat **eine** zentrale offene Kette. Unser Hebel ist ihr letztes Glied.

1. Die untere Schranke für die Mittel von `a h(a)` ist bewiesen (Theorem 3). Die obere Schranke folgt
   aus (UI) des Potentials der ungeraden Ziele (Proposition 15).
2. Das Momentenfenster `1 < p < 2` für (UI) ist scharf, sobald das Syracuse-Profil für alle `p < 2` in
   `L^p` liegt (Korollar 10).
3. Das folgt aus `sup E_n < ∞` (Conjecture 3).
4. Conjecture 3 folgt aus `Cor_n(g_d)` beschränkt (Conjecture 4).
5. Der Term `g_2` trägt 97–103 % der Zuwächse von `E_n` (Run L/N).

**Pilotziel:** `Cor_n(g_2) <= C` für alle `n`.

Seit v4j (noch nicht im Paper, mit `syracuse_lp.py` bis n = 13 exakt geprüft):

- `Cor_n(g_2) = Cor_n(y -> y + 1/2)`, bewiesen mit dem Argument von Lemma 11(b) und
  `f(2y+1) = 2 f(y + 1/2) - f_T(2y+1)`.
- Das Pilotziel lautet damit: **Die Differenz `S' - S` zweier unabhängiger Syracuse-Variablen hat im
  Punkt `1/2` eine beschränkte Dichte.**
- Exakt gilt `f_n(-1/2) = f_n(-1)/2`. Der langsame Anteil ist also das Singularitätenpaar `-1`,
  `-1/2` im Abstand `1/2`. Sein Beitrag fällt heuristisch mit `3^{-1} (3/2)^2 = 3/4` pro Stufe.

## Strang A: Conjecture 3 (Priorität 1)

| ID | Aufgabe | Ergebnis | braucht | Art | Status |
|---|---|---|---|---|---|
| A0 | `Cor(g_2) = Cor(y + 1/2)` und `f(-1/2) = f(-1)/2` aufschreiben. Das Pilotziel als Dichte von `S' - S` im Punkt `1/2` formulieren. | Lemma (Note Abschnitt 8) | – | Schreibtisch, ½ Tag | **erledigt** (Note Lemma 14, Dilatationsregeln; `y ± 1/2` ist kein Intertwiner) |
| A1 | Exakte **Renewal-Identität** (Vorschlag 7, Theorieteil): `Cor_n(g_d)` nach der ersten Annäherung des Walks an die Singularmenge zerlegen, mit den Gewichten `C_{k,r}(d)` (siehe A3) und den Werten `H_2(r)`. Gegen Run L prüfen. | Proposition: legt fest, **was** gezählt werden muss | – | Theorie und kleine Numerik | **erledigt** (Note Prop. 15, Kor. 16): `Cor_N(g_d)` = Regulärteil `F_d(N,0)` + Σ `F_d(N-r,r) H_2(r)`. Bootstrap: Pilotziel folgt aus `rho* < 44/15` (Dichte der Tiefe-k-Zustände an der Identitätsklasse) und beschränktem Regulärteil. |
| A2 | **Großabweichungs-Lemma** (Vorschlag 5): Höhenschranke (Prop. 19(c)) plus Chernoff für `\|A\| - k`. Annäherungen mit Zusatzpräzision `r >= beta_0 k` haben das Gewicht `(3/2)^k binom(m+k-1, k-1) 3^{-r}` mit `m = r log_2 3`, also exponentiell klein, sobald `beta_0 ≈ 2.70` (erste Rechnung). Rigoros mit expliziten Konstanten machen. | Lemma und Karte des Restgebiets `1 <= r < 3k` | A1 (Einbau) | Schreibtisch | **erledigt** (Note Lemma 17): `beta* = 2.7022`, mit a-priori-Schranke für `H_2` `beta** = 6.4188` |
| A3 | **Run O** (Vorschläge 1 und 4): exakte gewichtete Kongruenzzählungen `C_{k,r}(d) = 3^{k+r} P(\|A\|-\|B\| = d, S(B) ≡ g_d(S(A)) mod 3^{k+r})`. Rechnung per FFT auf `Z/3^M` mit einem Fourierparameter `theta` für die Länge; er trennt die Diagonale (`d = 0`) exakt ab. Für `M <= 17–18` auf dem Cluster; Meet-in-the-Middle für kleine `k` und große `r` (bis `M ≈ 40`). Frage: Fällt `C_{k,r}` gleichmäßig in `k` wie `3^{-r}`? Gibt es eine Kontraktionsungleichung in `k`? Vorschlag 6 (`r = 1, 2, 3`) läuft hier als Diagnosespalte mit. | Tabelle `C_{k,r}(2)`, Fits, Kandidaten-Ungleichung | A1 | Cluster (Run O) | Skript `congruence_counts.py` fertig, lokal bis N = 13; Cluster-Job `slurm/slp_runO.sbatch` (N ≤ 17) bereit |
| A4 | **Resonanz bei −1** (Vorschlag 3): Renormierungsoperator auf dem Orbit von `2y+1` (Abbildungen `2^e y + t`, `t` in `Z[1/2]`). Die `(1,1)`-Schleife (Gewicht 3/4) und das Paar `-1`, `-1/2` abspalten, dominante Moden bestimmen. Die beobachteten Raten (0,866 bei n = 20, Fit 0,78) quantitativ erklären. | Zerlegung „Resonanz + Rest“, Rate des Rests | A0 | Numerik, ggf. Cluster | offen |
| **E1** | **Entscheidungspunkt:** Schließen Renewal (A1), Großabweichung (A2) und Zählung (A3)? | ja: A5; nein: Plan revidieren | A1–A4 | – | – |
| A5 | Bedingter Satz: `Cor_n(g_2) <= C` unter einer präzise formulierten Mischungshypothese `H(k, r)` für die abgeschnittenen Syracuse-Summen modulo `3^{k+r}` im Restgebiet `r < 3k`. | Satz (bedingt) | E1 | Theorie | – |
| A6 | **Mehrskalen-Kontraktion** (Vorschlag 2): Blockoperatoren exakt berechnen und Kontraktionsfaktoren mit Intervallarithmetik zertifizieren. Kontrahieren müssen die **Inkremente** bzw. die Abweichung von der Gleichverteilung, nicht `E_n` selbst (das gegen 0,475 > 0 geht). Die Norm kommt aus A1/A4; Taos Proposition 1.14/1.17 dient als Ausgangspunkt. Ziel: `H(k, r)` beweisen. | Beweis von `H` (Ziel) | A5 | Theorie, Cluster | – |
| A7 | Von `g_2` auf alle `g_d`: `v_3(d/2) = 0` analog; `v_3(d/2) >= 1` bringt Nähe zur Identität, also einen `H_2(v_3)`-Term. Daraus folgen Conjecture 4 für die `g_d`, Conjecture 3 und `L^p` für `p < 2`. | Satz | A6 | Theorie | – |

**Abbruchkriterium Strang A:** Fällt `C_{k,r}` in Run O nicht gleichmäßig in `k` ab, ist Conjecture 4
in Frage zu stellen. Dann gilt: Numerik und Teilresultate (A0, A2, Prop. 19) sichern und Strang A
schließen.

### Einordnung der Ideen

| Vorschlag | Einordnung | Task |
|---|---|---|
| 1. Fourier mit Längenparameter | Sehr gut. Der Parameter `theta` trennt genau die Diagonale `A = B` ab, die sonst den Beitrag O(1) pro Tiefe liefert. Erste Wahl für die Numerik. | A3 |
| 2. Mehrskalen-Mischung | Richtig als Beweisweg, aber erst nach A1/A4. Es muss der Defekt kontrahieren, nicht die Energie. | A6 |
| 3. Resonanz −1 abspalten | Gut und konkret. Mit A0 ist die Resonanz das Paar `-1`, `-1/2`. Allein schließt das den Beweis nicht, weil der Rest noch die neutrale Identität und Gewichte bis 5/3 enthält. | A4 |
| 4. Meet-in-the-Middle für `X_A - X_B` | Zählt dieselben Größen wie 1. Gehört in denselben Run; MitM nur dort, wo die FFT nicht hinreicht. | A3 |
| 5. Seltene Bewertungsfolgen | Trägt. Erste Rechnung: `r >= 2.7 k` ist rigoros erledigbar, das harte Gebiet ist `1 <= r < 3k`. | A2 |
| 6. Feste Zusatzpräzision | **Nicht schwächer**: Die Schwierigkeit ist die Gleichmäßigkeit in `k`, und schon `r = 0` ist im Wesentlichen `Cor_k(g_2)` selbst. Nur als Diagnose verwenden. | A3 (Spalten) |
| 7. Renewal-Gleichung | Der richtige Rahmen; zuerst exakt herleiten. | A1 |

### Stand nach A0–A2 (22.11.2026, spät)

- Die schwierige Stelle ist jetzt der **Regulärteil** `F_2(N, 0)`, das Gewicht der Pfade, die der Identität nie nahe
  kommen. Der Resonanzteil braucht nur die grobe Schranke `rho* < 2,93` (numerisch ≤ 1,15).
- Lokal (N ≤ 13) konvergieren Regulärteil (Faktor ≈ 0,72) und Resonanzteil (≈ 0,76) einzeln schneller als
  `Cor_N(g_2)` (≈ 0,91). Die langsame Rate aus Run L ist eine Beinahe-Auslöschung. Run O auf dem Cluster (N ≤ 17)
  soll zeigen, ob die asymptotische Rate ≈ 3/4 ist.
- Nächste Schreibtisch-Aufgabe vor E1: Regulärteil verstehen. Ist er monoton? Gibt es eine Rekursion, die ihn
  durch `rho` und sich selbst schließt?

## Strang B: Fourier-Maximum (geparkt)

| ID | Aufgabe | Status |
|---|---|---|
| B1 | Transferoperator auf `theta = 2^k/3^n` (`theta -> 3 theta / 2^a`) für Taos `O(exp(-cn))`; numerisch `A n^{-1} 0.943^n` (Run M). Nur aufnehmen, falls A3/A6 Werkzeuge liefern. | geparkt |

## Strang C: Zyklus-Paket (Abschnitt 4, „für später“)

| ID | Aufgabe | Ergebnis | braucht | Art | Status |
|---|---|---|---|---|---|
| C1 | **Halbwort-Kollisionsformel:** Für gerade `l, k` und `P = 2^{l/2} + 3^{k/2}` reduziert `2^{l/2} ≡ -3^{k/2} (mod P)` die Bedingung `P \| z_3(B_1 B_2)` auf eine (bei ungleichen Halbtypen verdrehte) Kongruenz zwischen `z_3(B_1)` und `z_3(B_2)`. Weil die Halbwort-Zähler von der Größe `P` sind, ist sie fast eine Gleichung, und fast nur die Diagonale `B'B'` überlebt. Ziel: exakte Formel, Vergleich mit den Faktoren 0,61 (`D > 0`) und 1,40 (`D < 0`) aus Run J. | Lemma für Abschnitt 4 | – | Schreibtisch, Run-J-Daten | offen |
| C2 | **Meet-in-the-Middle-Zykluszählung (Run P):** Paritätswort halbieren, `z_3` modulo `2^l - 3^k` über Hash-Tabellen der Hälften abgleichen (C++/MPI). Ziel: Wortlänge 60–70 statt 40. Zuerst eine Laufzeitprobe bei `l = 48`. | erweiterter Zensus | C1 (Deutung), Cluster frei | Cluster | offen |
| C3 | Abschnitt 4: Lemma und Tabelle ergänzen; Problem 1 und Outlook (3) anpassen. | Paper | C1, C2 | Schreiben | – |

## Strang D: Kompaktierung und Journal (nach E1)

| ID | Aufgabe | Status |
|---|---|---|
| D1 | Inventar aller Aussagen (bewiesen/bedingt/numerisch) mit Seitenkosten; Entwurf v4j hat 87 Seiten | offen |
| D2 | Auswahl Haupttext / Anhang / research notes. Anhang F auf den Kern eindampfen (Lemma 8, 9, Prop. 16, 17, 19(b), Kor. 10, Conj. 3, 4), den Rest in `syracuse_lp_note` | offen |
| D3 | Zielzeitschrift und Seitenbudget festlegen | offen |
| D4 | Konsistenzdurchlauf: Nummerierung, Runs- und Reproduktionstabelle, Abstract, Lean-Hinweis | offen |

## Reihenfolge

1. ~~**Jetzt:** A0 (aufschreiben), A1 und A2 (Schreibtisch), Skript für Run O mit lokalem Test bis `M = 12`.~~ erledigt (lokal bis N = 13)
2. **Cluster:** Run O (A3); parallel A4 numerisch.
3. **E1:** entscheiden, A5/A6 oder Revision.
4. **Parallel, wenn Luft ist:** C1 (Schreibtisch); C2, wenn der Cluster frei ist.
5. **Danach:** D1–D4, Journal-Version.

## Runs (vergeben und geplant)

| Run | Inhalt | Status |
|---|---|---|
| L, M, N | exaktes Gesetz bis n = 20; Monte Carlo Fourier bis n = 80; logarithmische Karte bis n = 15 | im Paper (v4j) |
| O | Trefferwerte `w_d(k, r)`, First-hit-Zerlegung (A1/A3) | lokal bis N = 13 erledigt; Cluster N ≤ 17 offen |
| P | Meet-in-the-Middle-Zykluszensus (C2) | geplant |
