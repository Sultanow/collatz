/*
 * basin.c -- basin counts for the cycles of
 *
 *        T(x) = (q*x + c)/2   if x is odd,
 *        T(x) =  x/2          if x is even,         (q, c odd)
 *
 * For every n in [start, end) the program determines which cycle the orbit of n
 * falls into and counts these events per cycle. The cycles are given by their
 * smallest members (e.g. from census.c). Counts of several ranges can simply be
 * added, so the work can be split into SLURM array tasks (see merge_basin.py).
 *
 * Usage:   basin q c start end cache m1 [m2 ...]
 *
 *   q, c        odd integers (3x-1: q=3, c=-1)
 *   start, end  range [start, end) of starting values, start >= 1
 *   cache       size of the label cache 1..cache (one byte per number). The
 *               orbit of n > cache is followed until it drops to <= cache.
 *               The cache is enlarged automatically to the largest cycle member.
 *   m1 m2 ...   smallest members of the known cycles (at most 254)
 *
 * Orbits that exceed 2^120 or 10^6 steps without reaching a known cycle are
 * counted as "unlabeled" (candidates for divergence or for an unknown cycle).
 *
 * Output:  start=<..> end=<..> cache=<..> unlabeled=<..>
 *          then one line per cycle:  m=<min> count=<..> share=<count/(end-start)>
 *
 * Memory:  about cache bytes. Time: roughly proportional to (end-start) * log(end/cache).
 *
 * Build:   gcc -O2 -o basin basin.c        (needs __int128, i.e. GCC or Clang on 64 bit)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef __int128 i128;

static long q, c;
static i128 T(i128 x) { return (x & 1) ? (q * x + c) / 2 : x / 2; }

#define MAXSTEPS 1000000L
static const i128 BIG = ((i128)1) << 120;

int main(int argc, char **argv)
{
    if (argc < 7) {
        fprintf(stderr, "usage: %s q c start end cache m1 [m2 ...]\n", argv[0]);
        return 1;
    }
    q = atol(argv[1]);
    c = atol(argv[2]);
    long long start = atoll(argv[3]), end = atoll(argv[4]), cache = atoll(argv[5]);
    int nc = argc - 6;
    if (q % 2 == 0 || c % 2 == 0) { fprintf(stderr, "q and c must be odd\n"); return 1; }
    if (start < 1 || end <= start) { fprintf(stderr, "need 1 <= start < end\n"); return 1; }
    if (nc > 254) { fprintf(stderr, "at most 254 cycles\n"); return 1; }

    long long *mins = malloc(sizeof(long long) * nc);
    long long maxmember = 0;
    for (int i = 0; i < nc; i++) {
        mins[i] = atoll(argv[6 + i]);
        /* walk once around the cycle to find its largest member */
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

    /* label all cycle members */
    for (int i = 0; i < nc; i++) {
        i128 x = mins[i];
        do { lab[(long long)x] = (uint8_t)(i + 1); x = T(x); } while (x != mins[i]);
    }

    /* build the cache: follow n until the orbit drops below n or hits a labeled member */
    for (long long n = 1; n <= cache; n++) {
        if (lab[n]) continue;
        i128 x = n;
        long steps = 0;
        uint8_t L = 0;
        for (;;) {
            x = T(x);
            if (x < n) { L = lab[(long long)x]; break; }
            if (x <= cache && lab[(long long)x]) { L = lab[(long long)x]; break; }
            if (++steps > MAXSTEPS || x > BIG) { L = 0; break; }
        }
        lab[n] = L;
    }

    /* count the range */
    long long *cnt = calloc(nc + 1, sizeof(long long));
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
                if (x <= cache) { L = lab[(long long)x]; break; }
                if (++steps > MAXSTEPS || x > BIG) break;
            }
        }
        cnt[L]++;
    }

    double total = (double)(end - start);
    printf("start=%lld end=%lld cache=%lld unlabeled=%lld\n", start, end, cache, cnt[0]);
    for (int i = 0; i < nc; i++)
        printf("m=%lld count=%lld share=%.6f\n", mins[i], cnt[i + 1], cnt[i + 1] / total);
    return 0;
}
