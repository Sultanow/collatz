/*
 * basin.c -- basin counts for the cycles of
 *
 *        T(x) = (q*x + c)/2   if x is odd,
 *        T(x) =  x/2          if x is even,         (q, c odd)
 *
 * For every n in [start, end) the program determines which cycle the orbit of n
 * falls into and counts these events per cycle. The cycles are given by their
 * smallest members (e.g. from cycles_for_map.py). Counts of several ranges can
 * simply be added, so the work can be split into SLURM array tasks (merge_basin.py).
 *
 * Usage:   basin [-w] q c start end cache m1 [m2 ...]
 *
 *   -w          additionally report the counts per dyadic window [2^j, 2^(j+1))
 *   q, c        odd integers (3x-1: q=3, c=-1)
 *   start, end  range [start, end) of starting values, start >= 1
 *   cache       size of the label cache 1..cache (one byte per number). The
 *               orbit of n > cache is followed until it drops to <= cache.
 *               The cache is enlarged automatically to the largest cycle member.
 *   m1 m2 ...   smallest members of the known cycles (at most 254)
 *
 * Orbits that exceed 2^120 or 10^6 steps without reaching a known cycle are
 * counted as "unlabeled" (candidates for divergence or for an unknown cycle).
 * For c <= -3 an orbit can leave the positive integers (e.g. 3x-5 maps 1 to -1);
 * such starting values are counted as "escaped".
 *
 * Output:  start=<..> end=<..> cache=<..> unlabeled=<..> escaped=<..>
 *          m=<min> count=<..> share=<count/(end-start)>        (one line per cycle)
 *          with -w additionally, per window intersecting [start, end):
 *          w=<j> lo=<..> hi=<..> unlabeled=<..> escaped=<..> counts=<c1,c2,...>
 *
 * Memory:  about cache bytes.
 *
 * Build:   gcc -O2 -o basin basin.c        (needs __int128, i.e. GCC or Clang on 64 bit)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef __int128 i128;

static long q, c;
static i128 T(i128 x) { return (x & 1) ? (q * x + c) / 2 : x / 2; }

#define MAXSTEPS 1000000L
#define ESC 255                       /* label: orbit reached a value <= 0 */
static const i128 BIG = ((i128)1) << 120;

int main(int argc, char **argv)
{
    int windows = 0, a0 = 1;
    if (argc > 1 && strcmp(argv[1], "-w") == 0) { windows = 1; a0 = 2; }
    if (argc - a0 < 6) {
        fprintf(stderr, "usage: %s [-w] q c start end cache m1 [m2 ...]\n", argv[0]);
        return 1;
    }
    q = atol(argv[a0]);
    c = atol(argv[a0 + 1]);
    long long start = atoll(argv[a0 + 2]), end = atoll(argv[a0 + 3]), cache = atoll(argv[a0 + 4]);
    int nc = argc - (a0 + 5);
    if (q % 2 == 0 || c % 2 == 0) { fprintf(stderr, "q and c must be odd\n"); return 1; }
    if (start < 1 || end <= start) { fprintf(stderr, "need 1 <= start < end\n"); return 1; }
    if (nc > 254) { fprintf(stderr, "at most 254 cycles\n"); return 1; }

    long long *mins = malloc(sizeof(long long) * nc);
    long long maxmember = 0;
    for (int i = 0; i < nc; i++) {
        mins[i] = atoll(argv[a0 + 5 + i]);
        i128 x = mins[i];
        long steps = 0;
        do {
            x = T(x);
            if (x > maxmember) maxmember = (long long)x;
            if (++steps > MAXSTEPS || x > BIG) { fprintf(stderr, "%lld is not on a cycle\n", mins[i]); return 1; }
        } while (x != mins[i]);
    }
    if (cache < maxmember) cache = maxmember;
    if (cache < 1) cache = 1;

    uint8_t *lab = calloc((size_t)cache + 1, 1);
    if (!lab) { fprintf(stderr, "cannot allocate cache of %lld bytes\n", cache); return 1; }

    for (int i = 0; i < nc; i++) {
        i128 x = mins[i];
        do { lab[(long long)x] = (uint8_t)(i + 1); x = T(x); } while (x != mins[i]);
    }

    for (long long n = 1; n <= cache; n++) {
        if (lab[n]) continue;
        i128 x = n;
        long steps = 0;
        uint8_t L = 0;
        for (;;) {
            x = T(x);
            if (x <= 0) { L = ESC; break; }
            if (x < n) { L = lab[(long long)x]; break; }
            if (x <= cache && lab[(long long)x]) { L = lab[(long long)x]; break; }
            if (++steps > MAXSTEPS || x > BIG) { L = 0; break; }
        }
        lab[n] = L;
    }

    long long *cnt = calloc(256, sizeof(long long));
    long long (*wcnt)[256] = windows ? calloc(64, sizeof(*wcnt)) : NULL;
    for (long long n = start; n < end; n++) {
        uint8_t L;
        if (n <= cache) {
            L = lab[n];
        } else {
            i128 x = n;
            long steps = 0;
            L = 0;
            for (;;) {
                x = T(x);
                if (x <= 0) { L = ESC; break; }
                if (x <= cache) { L = lab[(long long)x]; break; }
                if (++steps > MAXSTEPS || x > BIG) break;
            }
        }
        cnt[L]++;
        if (windows) wcnt[63 - __builtin_clzll((unsigned long long)n)][L]++;
    }

    double total = (double)(end - start);
    printf("start=%lld end=%lld cache=%lld unlabeled=%lld escaped=%lld\n", start, end, cache, cnt[0], cnt[ESC]);
    for (int i = 0; i < nc; i++)
        printf("m=%lld count=%lld share=%.6f\n", mins[i], cnt[i + 1], cnt[i + 1] / total);
    if (windows) {
        for (int j = 0; j < 63; j++) {
            long long lo = 1LL << j, hi = (j < 62) ? (1LL << (j + 1)) : (long long)INT64_MAX;
            if (lo < start) lo = start;
            if (hi > end) hi = end;
            if (lo >= hi) continue;
            printf("w=%d lo=%lld hi=%lld unlabeled=%lld escaped=%lld counts=", j, lo, hi, wcnt[j][0], wcnt[j][ESC]);
            for (int i = 0; i < nc; i++) printf(i ? ",%lld" : "%lld", wcnt[j][i + 1]);
            putchar('\n');
        }
    }
    return 0;
}
