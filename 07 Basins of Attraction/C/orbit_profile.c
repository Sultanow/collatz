/* orbit_profile.c -- the mean value and the Syracuse profile of the predecessor potential
 * at large scales, from the orbits of random starting values (Run H).
 *
 * Usage:  orbit_profile C S E SEED [KMIN [KPOOL]]
 *   map 3x+c (c odd, c >= -1), S starting values drawn uniformly from [10^E, 2*10^E)
 *   (E <= 30, 128-bit arithmetic), targets in the decades [10^k, 10^(k+1)), KMIN <= k <= E-3
 *   (default KMIN = 6); the residue classes are pooled over the decades k >= KPOOL (default 9).
 *
 * For a target a, h(a) is the proportion of the starting values whose orbit contains a.
 * Summing over the orbit points instead of the targets gives, for every decade,
 *     mean of a*h(a) over the decade = (1/S) * (sum of a over all orbit points in the decade) / (9*10^k),
 * and in the same way the means over the residue classes of a/c modulo 81.  Conjecture 1 of
 * the paper predicts 2/log(4/3) and (2/log(4/3)) * 81 * P(Syrac(Z/81) = s).
 * The orbits are followed down to 1000; orbits that return from below 1000 to a decade
 * k >= 6 are too rare to matter.  Output: one line "decade k mean points" per decade and
 * one line "class s mean" per class modulo 81 (pooled over the decades), read by
 * orbit_profile_report.py.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef unsigned __int128 u128;
static uint64_t st[2];
static uint64_t splitmix(uint64_t *x) { uint64_t z = (*x += 0x9E3779B97F4A7C15ull); z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull; z = (z ^ (z >> 27)) * 0x94D049BB133111EBull; return z ^ (z >> 31); }
static uint64_t rnd(void) { uint64_t a = st[0], b = st[1], r = a + b; b ^= a; st[0] = ((a << 24) | (a >> 40)) ^ b ^ (b << 16); st[1] = (b << 37) | (b >> 27); return r; }

int main(int argc, char **argv)
{
    if (argc < 5) { fprintf(stderr, "usage: %s C S E SEED [KMIN [KPOOL]]\n", argv[0]); return 1; }
    long long c = atoll(argv[1]); long long S = atoll(argv[2]); int E = atoi(argv[3]);
    uint64_t seed = strtoull(argv[4], 0, 10); int KMIN = argc > 5 ? atoi(argv[5]) : 6; int KPOOL = argc > 6 ? atoi(argv[6]) : 9;
    if (KPOOL < KMIN) KPOOL = KMIN;
    if (E > 30 || E < KMIN + 4) { fprintf(stderr, "need KMIN+4 <= E <= 30\n"); return 1; }
    st[0] = splitmix(&seed); st[1] = splitmix(&seed);
    u128 p10[39]; p10[0] = 1; for (int i = 1; i < 39; i++) p10[i] = p10[i - 1] * 10;
    const u128 LIMIT = (u128)1 << 124;
    int cinv = 0; for (int t = 1; t < 81; t++) if ((((c % 81) + 81) * t) % 81 == 1) cinv = t;
    static double sum[40], cls[81]; static long long cnt[40]; long long discarded = 0;
    int KMAX = E - 3;
    for (long long i = 0; i < S; i++) {
        u128 x = p10[E] + ((((u128)rnd()) << 64) | rnd()) % p10[E];
        int k = E, ok = 1;
        double s_loc[40] = {0}, c_loc[81] = {0}; int n_loc[40] = {0};
        for (int step = 0; step < 100000 && x >= 1000; step++) {
            x = (x & 1) ? (u128)(((__int128)3 * (__int128)x + c) / 2) : x / 2;
            if (x > LIMIT) { ok = 0; break; }
            while (k > 0 && x < p10[k]) k--;
            while (k < 38 && x >= p10[k + 1]) k++;
            if (k >= KMIN && k <= KMAX) {
                double v = (double)x / (double)p10[k];
                s_loc[k] += v; n_loc[k]++;
                if (k >= KPOOL) c_loc[(int)((x % 81) * cinv % 81)] += v;
            }
        }
        if (!ok) { discarded++; continue; }
        for (int j = KMIN; j <= KMAX; j++) { sum[j] += s_loc[j]; cnt[j] += n_loc[j]; }
        for (int t = 0; t < 81; t++) cls[t] += c_loc[t];
    }
    long long used = S - discarded; int ndec = KMAX - KPOOL + 1;
    printf("# orbit_profile c=%lld S=%lld E=%d seed=%s kmin=%d kpool=%d kmax=%d discarded=%lld\n", c, S, E, argv[4], KMIN, KPOOL, KMAX, discarded);
    for (int j = KMIN; j <= KMAX; j++)
        printf("decade %d %.6f %.5f\n", j, sum[j] / used / 9.0, (double)cnt[j] / used);
    for (int t = 0; t < 81; t++)
        printf("class %d %.6f\n", t, cls[t] * 9.0 / used / ndec);     /* 10^k/9 targets per class and decade */
    return 0;
}
