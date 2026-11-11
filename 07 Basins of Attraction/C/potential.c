/* potential.c -- predecessor counts pi_a(N) = #{ n <= N : the orbit of n under
 * T(n) = n/2 (n even), (3n+c)/2 (n odd) contains a } for ALL targets a <= A at once.
 *
 * Usage:  potential C N A OUT.csv        (c odd, c >= -1;  OMP_NUM_THREADS sets the threads)
 *
 * Method.  For n let f(n) be the first iterate of n that is smaller than n ("stopping
 * point"); n is a root if there is none (cycle minima, and numbers below a cycle whose
 * orbits never return below themselves).  The orbit of a starting value s is the
 * concatenation of the segments seg(s_i) = [s_i, ..., before f(s_i)] along its chain of
 * stopping points s = s_0 > s_1 > ... > root.  All values of seg(n) are >= n, so a target
 * a <= A can only lie in segments of numbers n <= a.  With
 *     W(n) = number of starting values <= N whose chain passes through n
 *          = 1 + sum of W(n') over all n' with f(n') = n,
 * computed in one descending pass, pi_a(N) is the sum of W(n) over the n <= a whose
 * segment contains a.  Periodic targets are counted once, at the root of the chain.
 *
 * Memory: 8 bytes per n <= N (80 GB for N = 1e10).  Output: one line per target a <= A
 * with a, pi_a(N), the minimum of its cycle if a is periodic (else 0).
 * The exact identities
 *     pi_a(N) = [a <= N] + pi_{2a}(N) + pi_{(2a-c)/3}(N)        (a not periodic)
 *     #basin(C) = #(C in [1,N]) + sum of pi_x(N) over the entry points x of C
 * (Proposition "Predecessor potential") are checked by potential_report.py.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif

typedef unsigned __int128 u128;
typedef uint64_t u64;

static long long C;
#define CAP 200000          /* steps before a number without a smaller iterate is treated as a root */

static inline u128 T(u128 x) { return (x & 1) ? (u128)(((__int128)3 * (__int128)x + C) / 2) : x / 2; }

/* first iterate below n; 0 if the orbit returns to n or does not drop within CAP steps */
static u64 stopping_point(u64 n)
{
    u128 x = n;
    for (int k = 0; k < CAP; k++) {
        x = T(x);
        if (x < n) return (u64)x;
        if (x == n) return 0;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 5) { fprintf(stderr, "usage: %s C N A OUT.csv\n", argv[0]); return 1; }
    C = atoll(argv[1]);
    u64 N = strtoull(argv[2], 0, 10), A = strtoull(argv[3], 0, 10);
    if (A > N) A = N;
    u64 *W = calloc(N + 1, sizeof(u64));              /* W[n] = W(n) - 1 */
    u64 *cnt = calloc(A + 1, sizeof(u64));
    u64 *cycmin = calloc(A + 1, sizeof(u64));
    const u64 CH = 1ull << 24;
    u64 *f = malloc(CH * sizeof(u64));
    if (!W || !cnt || !cycmin || !f) { fprintf(stderr, "out of memory\n"); return 1; }

    /* pass 1: descending, W(f(n)) += W(n) */
    u64 nroots = 0;
    for (u64 hi = N; hi >= 1; ) {
        u64 lo = (hi > CH) ? hi - CH + 1 : 1;
        #pragma omp parallel for schedule(dynamic, 4096)
        for (u64 n = lo; n <= hi; n++) f[n - lo] = stopping_point(n);
        for (u64 n = hi; n >= lo; n--) {
            u64 g = f[n - lo];
            if (g) W[g] += W[n] + 1; else nroots++;
            if (n == lo) break;
        }
        if (lo == 1) break;
        hi = lo - 1;
    }

    /* pass 2a: roots <= A and their cycles */
    u64 unresolved = 0, cap = 1 << 20;
    u128 *path = malloc(cap * sizeof(u128));
    for (u64 n = 1; n <= A; n++) {
        if (stopping_point(n)) continue;
        /* Brent-free: walk until a value repeats (orbits of roots are short) */
        u64 len = 0; u128 x = n; long long rep = -1;
        while (len < cap) {
            for (u64 i = 0; i < len; i++) if (path[i] == x) { rep = (long long)i; break; }
            if (rep >= 0) break;
            path[len++] = x; x = T(x);
            if (len > 20000) break;
        }
        if (rep < 0) { unresolved++; continue; }
        u128 mn = path[rep];
        for (u64 i = rep; i < len; i++) if (path[i] < mn) mn = path[i];
        for (u64 i = rep; i < len; i++) if (path[i] <= A) cycmin[(u64)path[i]] = (u64)mn;
    }
    /* pass 2b: distribute W over the targets in the segments of all n <= A */
    for (u64 n = 1; n <= A; n++) {
        u64 w = W[n] + 1;
        if (stopping_point(n)) {                      /* ordinary segment: n, ..., before f(n) */
            u128 x = n;
            do { if (x <= A && !cycmin[(u64)x]) cnt[(u64)x] += w; x = T(x); } while (x >= n);
        } else {                                      /* root: tail and cycle, each value once */
            u64 len = 0; u128 x = n; int closed = 0;
            while (len < 20001) {
                for (u64 i = 0; i < len; i++) if (path[i] == x) { closed = 1; break; }
                if (closed) break;
                path[len++] = x; x = T(x);
            }
            if (!closed) continue;
            for (u64 i = 0; i < len; i++) if (path[i] <= A) cnt[(u64)path[i]] += w;
        }
    }

    FILE *out = fopen(argv[4], "w");
    fprintf(out, "# potential: c=%lld N=%llu A=%llu roots=%llu unresolved=%llu\n", C,
            (unsigned long long)N, (unsigned long long)A, (unsigned long long)nroots, (unsigned long long)unresolved);
    fprintf(out, "a,count,cycle_min\n");
    for (u64 a = 1; a <= A; a++)
        fprintf(out, "%llu,%llu,%llu\n", (unsigned long long)a, (unsigned long long)cnt[a], (unsigned long long)cycmin[a]);
    fclose(out);
    fprintf(stderr, "potential: c=%lld N=%llu A=%llu roots=%llu unresolved=%llu\n", C,
            (unsigned long long)N, (unsigned long long)A, (unsigned long long)nroots, (unsigned long long)unresolved);
    return 0;
}
