/*
 * census.c -- exhaustive census of the positive integer cycles of
 *
 *        T(x) = (q*x + c)/2   if x is odd,
 *        T(x) =  x/2          if x is even,         (q, c odd)
 *
 * via parity words (Boehm-Sontacchi 1978; Lagarias 1990; "rotating binaries").
 *
 * A word B = b_0 b_1 ... b_{l-1} with k ones (type (l,k)) determines the unique
 * periodic point with parity sequence BBB...:
 *
 *        x_B = c * z_q(B) / D,      D = 2^l - q^k,
 *        z_q(B) = sum_{j=1..k} q^(k-j) * 2^(p_j),
 *
 * where p_1 < ... < p_k are the positions of the ones (position 0 = first step).
 * x_B is a positive integer iff D divides c*z_q(B) and c*D > 0. Enumerating ALL
 * words with Lmin <= l <= Lmax therefore finds ALL positive integer cycles whose
 * parity word has a length in that range.
 *
 * Usage:   census q c Lmax [Lmin [s i]]
 *
 *   q, c   odd integers; c may be negative (3x-1: q=3, c=-1)
 *   Lmax   maximal word length (at most 62)
 *   Lmin   minimal word length (default 1)
 *   s, i   sharding for array jobs: only words whose first s bits, read as a
 *          binary number with b_0 as least significant bit, equal i
 *          (0 <= i < 2^s). Lengths l <= s are handled completely by shard 0.
 *          The shards 0 .. 2^s-1 together cover every word exactly once.
 *
 * Output:  a header line, then one line per cycle found
 *
 *   min=<smallest member> l=<length> k=<odd steps> D=<2^l-q^k> word=<parity word from min>
 *
 * A cycle with primitive word length l0 reappears at the lengths 2*l0, 3*l0, ...
 * (the word repeated). When merging several runs or shards, keep for every min
 * the line with the smallest l (see merge_census.py).
 *
 * Build:   gcc -O2 -o census census.c        (needs __int128, i.e. GCC or Clang on 64 bit)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef __int128 i128;

static void print_i128(i128 v)
{
    char buf[64];
    int n = 0, neg = v < 0;
    if (neg) v = -v;
    do { buf[n++] = (char)('0' + (int)(v % 10)); v /= 10; } while (v);
    if (neg) putchar('-');
    while (n) putchar(buf[--n]);
}

#define MAXCYC 65536
static i128 cyc_min[MAXCYC];
static int  cyc_l[MAXCYC], cyc_k[MAXCYC];
static i128 cyc_D[MAXCYC];
static int  ncyc = 0;

static long q, c;

static i128 T(i128 x) { return (x & 1) ? (q * x + c) / 2 : x / 2; }

/* test one word (bit p of m = parity at step p) */
static long long checked = 0;
static void test_word(uint64_t m, int l, int k, i128 D)
{
    checked++;
    i128 z = 0, qp = 1;
    for (int p = l - 1; p >= 0; p--)
        if ((m >> p) & 1) { z += qp * (((i128)1) << p); qp *= q; }
    i128 num = (i128)c * z;
    if (num % D != 0) return;
    i128 x = num / D;
    if (x <= 0) return;
    /* smallest member of the cycle */
    i128 y = x, mn = x;
    for (int s = 0; s < l; s++) { y = T(y); if (y < mn) mn = y; }
    for (int i = 0; i < ncyc; i++)
        if (cyc_min[i] == mn) { if (l < cyc_l[i]) { cyc_l[i] = l; cyc_k[i] = k; cyc_D[i] = D; } return; }
    if (ncyc < MAXCYC) { cyc_min[ncyc] = mn; cyc_l[ncyc] = l; cyc_k[ncyc] = k; cyc_D[ncyc] = D; ncyc++; }
}

/* enumerate all r-bit masks with exactly w ones (Gosper's hack); call f(prefix | rest<<s) */
static void enum_rest(uint64_t prefix, int s, int r, int w, int l, int k, i128 D)
{
    if (w < 0 || w > r) return;
    if (w == 0) { test_word(prefix, l, k, D); return; }
    uint64_t m = (w == 64) ? ~0ULL : ((1ULL << w) - 1);
    uint64_t lim = (r == 64) ? 0 : (1ULL << r);
    while (m < lim) {
        test_word(prefix | (m << s), l, k, D);
        uint64_t t = m | (m - 1);
        m = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(m) + 1));
    }
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stderr, "usage: %s q c Lmax [Lmin [s i]]\n", argv[0]);
        return 1;
    }
    q = atol(argv[1]);
    c = atol(argv[2]);
    int Lmax = atoi(argv[3]);
    int Lmin = (argc > 4) ? atoi(argv[4]) : 1;
    int s = (argc > 6) ? atoi(argv[5]) : 0;
    long shard = (argc > 6) ? atol(argv[6]) : 0;
    if (q % 2 == 0 || c % 2 == 0 || q < 1) { fprintf(stderr, "q and c must be odd, q >= 1\n"); return 1; }
    if (Lmax > 62 || Lmin < 1 || Lmin > Lmax) { fprintf(stderr, "need 1 <= Lmin <= Lmax <= 62\n"); return 1; }
    if (s < 0 || s > 30 || shard < 0 || shard >= (1L << s)) { fprintf(stderr, "need 0 <= i < 2^s, s <= 30\n"); return 1; }
    double lq = log2((double)q);

    for (int l = Lmin; l <= Lmax; l++) {
        int sharded = (l > s);
        if (!sharded && shard != 0) continue;          /* short lengths: shard 0 only */
        i128 P2 = ((i128)1) << l, qk = 1;
        for (int k = 1; k <= l; k++) {
            qk *= q;
            if (c > 0 && qk > P2) break;               /* D < 0 for all larger k */
            i128 D = P2 - qk;
            if (D == 0) continue;
            if (!((c > 0 && D > 0) || (c < 0 && D < 0))) continue;
            if (k * lq + l > 125.0) {                  /* z_q(B) < q^k 2^l must fit into 128 bit */
                fprintf(stderr, "Lmax too large for 128-bit arithmetic at (l,k)=(%d,%d)\n", l, k);
                return 2;
            }
            if (!sharded) {
                enum_rest(0, 0, l, k, l, k, D);
            } else {
                uint64_t prefix = (uint64_t)shard;
                int w = k - __builtin_popcountll(prefix);
                enum_rest(prefix, s, l - s, w, l, k, D);
            }
        }
    }

    printf("q=%ld c=%ld Lmin=%d Lmax=%d shard=%ld/%ld words=%lld cycles=%d\n",
           q, c, Lmin, Lmax, shard, 1L << s, checked, ncyc);
    for (int i = 0; i < ncyc; i++) {
        printf("min="); print_i128(cyc_min[i]);
        printf(" l=%d k=%d D=", cyc_l[i], cyc_k[i]); print_i128(cyc_D[i]);
        printf(" word=");
        i128 y = cyc_min[i];
        for (int st = 0; st < cyc_l[i]; st++) { putchar((y & 1) ? '1' : '0'); y = T(y); }
        putchar('\n');
    }
    return 0;
}
