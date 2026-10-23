/*
 * census_all.c -- simultaneous census of the positive integer cycles of ALL maps
 *
 *        T_{q,c}(x) = (q*x + c)/2   if x is odd,
 *        T_{q,c}(x) =  x/2          if x is even,        c odd, 1 <= |c| <= DMAX,
 *
 * in a single pass over all parity words.
 *
 * For a word B = b_0 ... b_{l-1} of type (l,k) put D = 2^l - q^k and
 * z = z_q(B) = sum_{j=1..k} q^(k-j) 2^(p_j) (p_j = positions of the ones,
 * position 0 = first step). The periodic point of T_{q,c} with parity
 * sequence BBB... is c*z/D. Let d0 be the denominator of z/D in lowest terms.
 * Then B yields a positive integer cycle of T_{q,c} exactly for
 * c = sign(D) * m * d0 with m odd, and that cycle is m times the PRIMITIVE
 * cycle of T_{q,c0}, c0 = sign(D) * d0, which contains x = |numerator of z/D|.
 * (For q = 3 these are Lagarias' rational cycles of 3x+1 with denominator d0.)
 *
 * One pass over all words with Lmin <= l <= Lmax therefore lists every
 * primitive cycle with |c0| <= DMAX whose parity word has a length in that
 * range. Each cycle is printed exactly once: by the rotation of its word that
 * starts at the smallest member, and only for its primitive length.
 *
 * Usage:   census_all q DMAX Lmax [Lmin [s i]]
 *
 *   q      odd, q >= 3
 *   DMAX   largest |c0| to report
 *   Lmax   maximal word length, Lmin minimal word length (default 1)
 *   s, i   sharding for array jobs: only words whose first s bits, read as a
 *          binary number with b_0 as least significant bit, equal i
 *          (0 <= i < 2^s). Lengths l <= s are handled completely by shard 0.
 *
 * Output:  one line per primitive cycle, then a summary line starting with '#':
 *
 *   c=<c0> min=<smallest member> l=<length> k=<odd steps> D=<2^l-q^k> word=<word from min>
 *   # q=.. DMAX=.. Lmin=.. Lmax=.. shard=i/2^s words=<checked> cycles=<printed> mismatches=<0>
 *
 * The cycles of one particular map T_{q,c}: all lines with c0 | c and the same
 * sign, scaled by c/c0 (cycles_for_map.py). "mismatches" counts violations of
 * the parity check and must be 0.
 *
 * Build:   gcc -O2 -o census_all census_all.c -lm      (needs __int128)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef __int128 i128;
typedef unsigned __int128 u128;

static long q, dmax;
static long long checked = 0, printed = 0, mismatches = 0;

static void print_i128(i128 v)
{
    char buf[64];
    int n = 0, neg = v < 0;
    u128 u = neg ? (u128)(-v) : (u128)v;
    do { buf[n++] = (char)('0' + (int)(u % 10)); u /= 10; } while (u);
    if (neg) putchar('-');
    while (n) putchar(buf[--n]);
}

/* Denominator of r/A in lowest terms (0 <= r < A) if it is <= dmax; returns 0 otherwise.
   Continued-fraction denominators grow monotonically, so we stop as soon as one exceeds dmax. */
static long small_denominator(u128 A, u128 r)
{
    if (r == 0) return 1;
    if (A <= (u128)UINT64_MAX) {                      /* fast 64-bit path */
        uint64_t a = (uint64_t)A, b = (uint64_t)r, kprev = 0, k = 1;
        while (b) {
            uint64_t t = a / b, rem = a % b;
            u128 knew = (u128)t * k + kprev;
            if (knew > (u128)dmax) return 0;
            kprev = k; k = (uint64_t)knew;
            a = b; b = rem;
        }
        return (long)k;
    }
    u128 a = A, b = r, kprev = 0, k = 1;
    while (b) {
        u128 t = a / b, rem = a % b;
        if (t > (u128)dmax) return 0;
        u128 knew = t * k + kprev;
        if (knew > (u128)dmax) return 0;
        kprev = k; k = knew;
        a = b; b = rem;
    }
    return (long)k;
}

static void test_word(uint64_t m, int l, int k, i128 D)
{
    checked++;
    u128 z = 0, qp = 1;
    for (int p = l - 1; p >= 0; p--)
        if ((m >> p) & 1) { z += qp << p; qp *= (u128)q; }
    u128 A = (D > 0) ? (u128)D : (u128)(-D);
    long d0 = small_denominator(A, z % A);
    if (!d0) return;
    u128 x = z / (A / (u128)d0);                       /* exact: A/d0 divides z */
    i128 c0 = (D > 0) ? d0 : -d0;

    /* walk once around: parity check, primitivity, canonical rotation (x = smallest member) */
    i128 y = (i128)x;
    for (int s = 0; s < l; s++) {
        if ((int)(y & 1) != (int)((m >> s) & 1)) { mismatches++; return; }
        y = (y & 1) ? ((i128)q * y + c0) / 2 : y / 2;
        if (s < l - 1) {
            if (y == (i128)x) return;                  /* word is a power of a shorter word */
            if (y < (i128)x) return;                   /* not the rotation starting at the minimum */
        }
    }
    if (y != (i128)x) { mismatches++; return; }

    printed++;
    printf("c="); print_i128(c0);
    printf(" min="); print_i128((i128)x);
    printf(" l=%d k=%d D=", l, k); print_i128(D);
    printf(" word=");
    for (int s = 0; s < l; s++) putchar(((m >> s) & 1) ? '1' : '0');
    putchar('\n');
}

static void enum_rest(uint64_t prefix, int s, int r, int w, int l, int k, i128 D)
{
    if (w < 0 || w > r) return;
    if (w == 0) { test_word(prefix, l, k, D); return; }
    uint64_t m = (1ULL << w) - 1;
    uint64_t lim = 1ULL << r;
    while (m < lim) {
        test_word(prefix | (m << s), l, k, D);
        uint64_t t = m | (m - 1);
        m = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(m) + 1));
    }
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stderr, "usage: %s q DMAX Lmax [Lmin [s i]]\n", argv[0]);
        return 1;
    }
    q = atol(argv[1]);
    dmax = atol(argv[2]);
    int Lmax = atoi(argv[3]);
    int Lmin = (argc > 4) ? atoi(argv[4]) : 1;
    int s = (argc > 6) ? atoi(argv[5]) : 0;
    long shard = (argc > 6) ? atol(argv[6]) : 0;
    if (q < 3 || q % 2 == 0) { fprintf(stderr, "q must be odd and >= 3\n"); return 1; }
    if (dmax < 1) { fprintf(stderr, "DMAX must be >= 1\n"); return 1; }
    if (Lmax > 62 || Lmin < 1 || Lmin > Lmax) { fprintf(stderr, "need 1 <= Lmin <= Lmax <= 62\n"); return 1; }
    if (s < 0 || s > 30 || shard < 0 || shard >= (1L << s)) { fprintf(stderr, "need 0 <= i < 2^s, s <= 30\n"); return 1; }
    double lq = log2((double)q);

    for (int l = Lmin; l <= Lmax; l++) {
        int sharded = (l > s);
        if (!sharded && shard != 0) continue;
        i128 P2 = ((i128)1) << l, qk = 1;
        for (int k = 1; k <= l; k++) {
            /* members are below q^k 2^l; one more factor q must fit into signed 128 bit */
            if ((k + 1) * lq + l > 125.0) {
                fprintf(stderr, "Lmax too large for 128-bit arithmetic at (l,k)=(%d,%d)\n", l, k);
                return 2;
            }
            qk *= q;
            i128 D = P2 - qk;
            if (D == 0) continue;
            if (!sharded) enum_rest(0, 0, l, k, l, k, D);
            else enum_rest((uint64_t)shard, s, l - s, k - __builtin_popcountll((uint64_t)shard), l, k, D);
        }
    }
    printf("# q=%ld DMAX=%ld Lmin=%d Lmax=%d shard=%ld/%ld words=%lld cycles=%lld mismatches=%lld\n",
           q, dmax, Lmin, Lmax, shard, 1L << s, checked, printed, mismatches);
    return mismatches ? 3 : 0;
}
