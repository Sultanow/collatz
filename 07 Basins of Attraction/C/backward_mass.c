/*
 * backward_mass.c -- harmonic mass of the backward tree of a cycle of 3x+c
 *
 * A heuristic predictor of basin densities (Problem 2(c) of the paper). For a cycle C of
 * T(x) = (3x+c)/2 (x odd), x/2 (x even), level 0 consists of the predecessors of the
 * cycle members that are not on the cycle; level d+1 consists of the predecessors of
 * level d: 2y always, and (2y-c)/3 if it is a positive odd integer. Nodes divisible by 3
 * are not expanded (their trees are doubling chains of density 0). The mass of a level is
 * the sum of 1/x over its nodes not divisible by 3 and at least K*c (default K = 1/2:
 * below c/2 a node has no odd predecessor, its subtree is a doubling chain, and counting
 * it would only add transient spikes). In expectation the mass is the same on every
 * level (the even and the odd predecessor carry half of it each); its average over the
 * deep levels is the predicted basin share, up to a factor common to all cycles of a map.
 * The tree is walked depth first, so memory does not grow with D; time grows like (4/3)^D.
 *
 * Input (stdin): lines "c cycle_min".  Output: c,cycle_min,l,mass_level0,avg_d1,avg_d3
 * (avg_d1 = mean mass of the levels d1..D-1, avg_d3 = mean of d3..D-1), or with the fifth
 * argument "levels" one line c,cycle_min,d,mass_d per level d = 0..D-1.
 * Build: make backward_mass     Usage: ./backward_mass D d1 d3 [K [levels]] < cycles
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned __int128 u128;
static long C;
static u128 T(u128 x) { return (x & 1) ? (3 * x + (u128)C) / 2 : x / 2; }

int main(int argc, char **argv)
{
    int D = argc > 1 ? atoi(argv[1]) : 46, d1 = argc > 2 ? atoi(argv[2]) : 30, d3 = argc > 3 ? atoi(argv[3]) : 38;
    double K = argc > 4 ? atof(argv[4]) : 0.5;   /* count only nodes x >= K*c */
    int per_level = argc > 5 && argv[5][0] == 'l';
    if (D < 1 || D > 255) { fprintf(stderr, "D must be between 1 and 255\n"); return 1; }
    long c; unsigned long long mn;
    static u128 cyc[100000], stk_v[4096]; static int stk_d[4096];
    long double mass[256];
    while (scanf("%ld %llu", &c, &mn) == 2) {
        C = c;
        int l = 0; u128 x = mn;
        do { cyc[l++] = x; x = T(x); } while (x != (u128)mn && l < 100000);
        for (int d = 0; d < D; d++) mass[d] = 0;
        for (int i = 0; i < l; i++) {
            u128 y = cyc[i], cand[2]; int nc = 0;
            cand[nc++] = 2 * y;
            if (2 * y > (u128)C && (2 * y - (u128)C) % 3 == 0 && (((2 * y - (u128)C) / 3) & 1))
                cand[nc++] = (2 * y - (u128)C) / 3;
            for (int j = 0; j < nc; j++) {
                int on = 0;
                for (int t = 0; t < l && !on; t++) on = cyc[t] == cand[j];
                if (on) continue;
                int sp = 0;                          /* depth-first walk of the subtree */
                stk_v[sp] = cand[j]; stk_d[sp++] = 0;
                while (sp) {
                    u128 v = stk_v[--sp]; int d = stk_d[sp];
                    if (v % 3 == 0) continue;
                    if ((long double)v >= (long double)K * (long double)C) mass[d] += 1.0L / (long double)v;
                    if (d + 1 >= D) continue;
                    stk_v[sp] = 2 * v; stk_d[sp++] = d + 1;
                    if (2 * v > (u128)C && (2 * v - (u128)C) % 3 == 0 && (((2 * v - (u128)C) / 3) & 1)) {
                        stk_v[sp] = (2 * v - (u128)C) / 3; stk_d[sp++] = d + 1;
                    }
                }
            }
        }
        long double s1 = 0, s3 = 0;
        for (int d = d1; d < D; d++) s1 += mass[d];
        for (int d = d3; d < D; d++) s3 += mass[d];
        if (per_level)
            for (int d = 0; d < D; d++) printf("%ld,%llu,%d,%.10Le\n", c, mn, d, mass[d]);
        else
            printf("%ld,%llu,%d,%.10Le,%.10Le,%.10Le\n", c, mn, l, mass[0], s1 / (D - d1), s3 / (D - d3));
        fflush(stdout);
    }
    return 0;
}
