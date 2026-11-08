/*
 * passage.c -- basin shares of the map 3x+c far beyond brute force
 *
 * For the Syracuse map Syr(n) = (3n+c)/2^{nu_2(3n+c)} on odd n, this program
 * estimates the probability that a random odd N in the window [y, y^alpha]
 * lies in the basin of each cycle. N is drawn
 *   log  : logarithmically, P(N) proportional to 1/N  (Theorem 1, Remark 5)
 *   unif : uniformly on the odd integers of the window (natural counting,
 *          Problem 2(a), Corollary 3, Remark 6)
 * Each orbit is followed until its first passage below the table bound B;
 * the basin of the passage point is read from a precomputed table of all odd
 * m < B. By Step 2 of the proof of Theorem 1, a basin is not changed by first
 * passage, so this is the basin of N itself. The passage threshold may be
 * placed anywhere below the window; B is chosen so that the passage point can
 * be looked up.
 *
 * Mode "exact" enumerates all odd N in [y, y^alpha] (small y only) and prints
 * the exact logarithmic and uniform shares, to validate the sampling modes.
 *
 * Build:  make passage          (needs GMP and MPFR, see the Makefile)
 *         gcc -O2 -o passage passage.c -lmpfr -lgmp
 * Usage:  ./passage c log10y alpha mode nsamples seed [B [m K]]
 *   c        odd integer >= -1 (map 3x+c)
 *   log10y   window start y = 10^log10y (decimal, e.g. 50)
 *   alpha    window [y, y^alpha] (decimal, e.g. 1.1)
 *   mode     log | unif | exact
 *   nsamples number of samples (ignored in mode exact); per residue class if m is given
 *   seed     RNG seed
 *   B        table bound, default 10000000
 *   m K      residue sweep: draw the starting values from K residue classes r mod m
 *            (all admissible classes if K = 0 or K is at least their number; for even m
 *            only odd r are admissible), nsamples per class
 * Output: CSV on stdout, one line per cycle (cycle_min = -1: unresolved):
 *   c,log10y,alpha,mode,seed,n,cycle_min,count,share
 * In a residue sweep two fields m,r are appended to every line.
 * In mode exact, two blocks are printed (mode exact_log and exact_unif);
 * count is the number of odd integers in the basin, share the exact share.
 * Cycles whose members all exceed B are detected on the fly (Brent's algorithm),
 * reported on stderr and counted like the others.
 * Cluster runs: slurm/passage_array.sbatch; merge with merge_passage.py.
 * Self-tests: make test-passage.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <gmp.h>
#include <mpfr.h>

typedef unsigned __int128 u128;

#define UNRES 254            /* label: orbit not resolved within the caps */
#define INPATH 255           /* temporary mark during table construction */
#define TABLE_STEPCAP 10000000ULL
#define SAMPLE_STEPCAP 10000000ULL

static long C;               /* the map 3x+C */
static uint64_t B;           /* odd m < B are tabulated */
static uint8_t *lab;         /* label of odd m < B at index m>>1 */
static uint64_t cycmin[256]; /* cycmin[k] = minimum of cycle with label k */
static int ncyc = 0;

static inline int ctz128(u128 t)
{
    uint64_t lo = (uint64_t)t;
    if (lo) return __builtin_ctzll(lo);
    return 64 + __builtin_ctzll((uint64_t)(t >> 64));
}

static inline u128 syr128(u128 v)
{
    u128 t = 3 * v;
    if (C >= 0) t += (u128)C; else t -= (u128)(-C);
    return t >> ctz128(t);
}

static int cycle_label(uint64_t mn)
{
    for (int k = 1; k <= ncyc; k++)
        if (cycmin[k] == mn) return k;
    if (ncyc >= 253) { fprintf(stderr, "too many cycles\n"); exit(1); }
    cycmin[++ncyc] = mn;
    return ncyc;
}

/* label of the cycle through v (v lies on a cycle); new cycles above B are reported */
static int label_of_cycle_through(u128 v)
{
    u128 mn = v, w = syr128(v);
    while (w != v) { if (w < mn) mn = w; w = syr128(w); }
    if (mn >> 64) return UNRES;
    int before = ncyc, L = cycle_label((uint64_t)mn);
    if (ncyc > before && mn >= B)
        fprintf(stderr, "new cycle above the table bound: minimum %llu\n", (unsigned long long)mn);
    return L;
}

/* Label every odd m < B with the cycle its orbit reaches. */
static void build_table(void)
{
    lab = calloc((size_t)(B >> 1) + 1, 1);
    size_t cap = 1u << 16, sp;
    uint64_t *stk = malloc(cap * sizeof *stk);
    const u128 LIM = ((u128)1) << 120;
    if (!lab || !stk) { fprintf(stderr, "out of memory\n"); exit(1); }
    for (uint64_t m = 1; m < B; m += 2) {
        if (lab[m >> 1]) continue;
        u128 v = m, tort = m;
        uint64_t steps = 0, power = 1, lam = 0;
        int res;
        sp = 0;
        for (;;) {
            if (v < B) {
                uint8_t L = lab[(uint64_t)v >> 1];
                if (L == INPATH) {          /* the path closed a cycle */
                    res = label_of_cycle_through(v);
                    break;
                }
                if (L) { res = L; break; }
                lab[(uint64_t)v >> 1] = INPATH;
                if (sp == cap) {
                    cap *= 2;
                    stk = realloc(stk, cap * sizeof *stk);
                    if (!stk) { fprintf(stderr, "out of memory\n"); exit(1); }
                }
                stk[sp++] = (uint64_t)v;
                tort = v; power = 1; lam = 0;   /* restart cycle detection */
            } else if (lam && v == tort) {      /* a cycle entirely above B */
                res = label_of_cycle_through(v);
                break;
            }
            if (lam == power) { tort = v; power <<= 1; lam = 0; }
            if (v > LIM || ++steps > TABLE_STEPCAP) { res = UNRES; break; }
            v = syr128(v);
            lam++;
        }
        for (size_t i = 0; i < sp; i++) lab[stk[i] >> 1] = (uint8_t)res;
    }
    free(stk);
}

/* Follow the orbit of a (large) odd N to its first passage below B and
 * return the label of the passage point. N is destroyed. */
static int label_of_big(mpz_t N, uint64_t *steps_out)
{
    const u128 LIM = ((u128)1) << 124;
    uint64_t steps = 0;
    for (;;) {
        while (mpz_sizeinbase(N, 2) > 120) {
            mpz_mul_ui(N, N, 3);
            if (C >= 0) mpz_add_ui(N, N, (unsigned long)C);
            else mpz_sub_ui(N, N, (unsigned long)(-C));
            mpz_tdiv_q_2exp(N, N, mpz_scan1(N, 0));
            if (++steps > SAMPLE_STEPCAP) { *steps_out = steps; return UNRES; }
        }
        u128 v = (u128)mpz_getlimbn(N, 0);
        if (mpz_size(N) > 1) v |= ((u128)mpz_getlimbn(N, 1)) << 64;
        u128 tort = v;
        uint64_t power = 1, lam = 0;
        while (v >= B && v < LIM) {
            v = syr128(v);
            lam++;
            if (v == tort) { *steps_out = steps; return label_of_cycle_through(v); }
            if (lam == power) { tort = v; power <<= 1; lam = 0; }
            if (++steps > SAMPLE_STEPCAP) { *steps_out = steps; return UNRES; }
        }
        if (v < B) { *steps_out = steps; return lab[(uint64_t)v >> 1]; }
        /* rare excursion above 2^124: continue with GMP */
        mpz_set_ui(N, (unsigned long)(uint64_t)(v >> 64));
        mpz_mul_2exp(N, N, 64);
        mpz_add_ui(N, N, (unsigned long)(uint64_t)v);
    }
}

static int label_of_small(uint64_t n)
{
    if (n < B) return lab[n >> 1];
    u128 v = n, tort = n;
    uint64_t steps = 0, power = 1, lam = 0;
    while (v >= B) {
        v = syr128(v);
        lam++;
        if (v == tort) return label_of_cycle_through(v);
        if (lam == power) { tort = v; power <<= 1; lam = 0; }
        if (++steps > SAMPLE_STEPCAP) return UNRES;
    }
    return lab[(uint64_t)v >> 1];
}

int main(int argc, char **argv)
{
    if (argc < 7) {
        fprintf(stderr, "usage: %s c log10y alpha mode nsamples seed [B [m K]]\n", argv[0]);
        return 1;
    }
    C = strtol(argv[1], NULL, 10);
    const char *s_log10y = argv[2], *s_alpha = argv[3], *mode = argv[4];
    uint64_t nsamples = strtoull(argv[5], NULL, 10);
    unsigned long seed = strtoul(argv[6], NULL, 10);
    B = (argc > 7) ? strtoull(argv[7], NULL, 10) : 10000000ULL;
    uint64_t sweep_m = (argc > 8) ? strtoull(argv[8], NULL, 10) : 0;
    uint64_t sweep_K = (argc > 9) ? strtoull(argv[9], NULL, 10) : 0;
    if (sweep_m == 1 || sweep_m > (1ULL << 62)) { fprintf(stderr, "m must be at least 2 and below 2^62\n"); return 1; }
    if (C < -1 || (C % 2) == 0) { fprintf(stderr, "c must be odd and >= -1\n"); return 1; }
    if (B < 3) B = 3;

    clock_t t0 = clock();
    build_table();
    fprintf(stderr, "table: B=%llu, %d cycles, %.2fs\n",
            (unsigned long long)B, ncyc, (double)(clock() - t0) / CLOCKS_PER_SEC);

    /* window endpoints: ln y and ln y^alpha at sufficient precision */
    double l10 = atof(s_log10y), al = atof(s_alpha);
    mpfr_prec_t prec = (mpfr_prec_t)(al * l10 * 3.3219280948873623) + 192;
    mpfr_t lny, a1, t, U;
    mpfr_inits2(prec, lny, a1, t, U, (mpfr_ptr)0);
    mpfr_set_str(t, s_log10y, 10, MPFR_RNDN);
    mpfr_log_ui(lny, 10, MPFR_RNDN);
    mpfr_mul(lny, lny, t, MPFR_RNDN);              /* ln y */
    mpfr_set_str(a1, s_alpha, 10, MPFR_RNDN);
    mpfr_sub_ui(a1, a1, 1, MPFR_RNDN);             /* alpha - 1 */

    mpz_t lo, hi, N, span;
    mpz_inits(lo, hi, N, span, NULL);
    mpfr_exp(t, lny, MPFR_RNDN);
    mpfr_get_z(lo, t, MPFR_RNDU);                  /* ceil(y) */
    mpfr_mul(t, lny, a1, MPFR_RNDN);
    mpfr_add(t, t, lny, MPFR_RNDN);
    mpfr_exp(t, t, MPFR_RNDN);
    mpfr_get_z(hi, t, MPFR_RNDD);                  /* floor(y^alpha) */

    uint64_t cnt[256] = {0};

    if (strcmp(mode, "exact") == 0) {
        if (mpz_sizeinbase(hi, 2) > 62) { fprintf(stderr, "window too large for exact\n"); return 1; }
        uint64_t a = mpz_get_ui(lo) | 1, b = mpz_get_ui(hi), n = 0;
        long double w[256] = {0}, wsum = 0;
        for (uint64_t m = a; m <= b; m += 2) {
            int L = label_of_small(m);
            cnt[L]++; n++;
            w[L] += 1.0L / (long double)m; wsum += 1.0L / (long double)m;
        }
        for (int pass = 0; pass < 2; pass++) {
            for (int k = 1; k <= ncyc + 1; k++) {
                int L = (k <= ncyc) ? k : UNRES;
                if (L == UNRES && cnt[UNRES] == 0) continue;
                long long mn = (L == UNRES) ? -1 : (long long)cycmin[L];
                double share = pass == 0 ? (double)(w[L] / wsum) : (double)cnt[L] / (double)n;
                printf("%ld,%s,%s,%s,0,%llu,%lld,%llu,%.10f\n", C, s_log10y, s_alpha,
                       pass == 0 ? "exact_log" : "exact_unif", (unsigned long long)n, mn,
                       (unsigned long long)cnt[L], share);
            }
        }
        return 0;
    }

    int logmode = strcmp(mode, "log") == 0;
    if (!logmode && strcmp(mode, "unif") != 0) { fprintf(stderr, "mode must be log, unif or exact\n"); return 1; }

    gmp_randstate_t rs;
    gmp_randinit_mt(rs);
    gmp_randseed_ui(rs, seed);
    mpz_sub(span, hi, lo);
    mpz_add_ui(span, span, 1);

    /* residue classes: none (plain sampling) or a sweep over K classes r mod m */
    uint64_t nres = 1, *res = NULL;
    if (sweep_m) {
        uint64_t adm = (sweep_m % 2 == 0) ? sweep_m / 2 : sweep_m;
        if (sweep_K == 0 || sweep_K >= adm) {
            nres = adm;
            res = malloc(nres * sizeof *res);
            for (uint64_t i = 0; i < nres; i++) res[i] = (sweep_m % 2 == 0) ? 2 * i + 1 : i;
        } else {
            nres = sweep_K;
            res = malloc(nres * sizeof *res);
            uint64_t x = 0x9E3779B97F4A7C15ULL ^ (uint64_t)seed;
            for (uint64_t i = 0; i < nres; ) {            /* K distinct classes */
                x ^= x << 13; x ^= x >> 7; x ^= x << 17;   /* xorshift64 */
                uint64_t r = x % sweep_m, j;
                if (sweep_m % 2 == 0) r |= 1;
                for (j = 0; j < i && res[j] != r; j++) ;
                if (j == i) res[i++] = r;
            }
        }
        if (!res) { fprintf(stderr, "out of memory\n"); return 1; }
    }

    uint64_t steps, stepsum = 0;
    t0 = clock();
    for (uint64_t ri = 0; ri < nres; ri++) {
        uint64_t M2 = 0, r2 = 0;
        if (sweep_m) {                     /* odd representatives of the class r mod m */
            M2 = (sweep_m % 2 == 0) ? sweep_m : 2 * sweep_m;
            r2 = (sweep_m % 2 == 0) ? res[ri] : ((res[ri] % 2) ? res[ri] : res[ri] + sweep_m);
        }
        memset(cnt, 0, sizeof cnt);
        for (uint64_t i = 0; i < nsamples; i++) {
            if (logmode) {
                mpfr_urandomb(U, rs);                      /* uniform in [0,1), prec bits */
                mpfr_mul(t, U, a1, MPFR_RNDN);
                mpfr_add_ui(t, t, 1, MPFR_RNDN);
                mpfr_mul(t, t, lny, MPFR_RNDN);            /* u in [ln y, alpha ln y) */
                mpfr_exp(t, t, MPFR_RNDN);
                mpfr_get_z(N, t, MPFR_RNDD);
            } else {
                mpz_urandomm(N, rs, span);
                mpz_add(N, N, lo);
            }
            if (sweep_m) {                                 /* move N into the class r2 mod M2 */
                uint64_t nm = mpz_fdiv_ui(N, M2);
                uint64_t adj = (nm + M2 - r2) % M2;
                mpz_sub_ui(N, N, adj);
                if (mpz_cmp(N, lo) < 0) mpz_add_ui(N, N, M2);
            } else {
                mpz_setbit(N, 0);
                if (!logmode && mpz_cmp(N, hi) > 0) mpz_sub_ui(N, N, 2);
            }
            int L = label_of_big(N, &steps);
            cnt[L]++;
            stepsum += steps;
        }
        for (int k = 1; k <= ncyc + 1; k++) {
            int L = (k <= ncyc) ? k : UNRES;
            long long mn = (L == UNRES) ? -1 : (long long)cycmin[L];
            printf("%ld,%s,%s,%s,%lu,%llu,%lld,%llu,%.10f", C, s_log10y, s_alpha, mode, seed,
                   (unsigned long long)nsamples, mn, (unsigned long long)cnt[L],
                   (double)cnt[L] / (double)(nsamples ? nsamples : 1));
            if (sweep_m) printf(",%llu,%llu", (unsigned long long)sweep_m, (unsigned long long)res[ri]);
            printf("\n");
        }
    }
    double secs = (double)(clock() - t0) / CLOCKS_PER_SEC;
    uint64_t total = nsamples * nres;
    fprintf(stderr, "c=%ld log10y=%s alpha=%s mode=%s n=%llu (%llu classes): %.1fs, %.2f us/sample, mean steps %.1f\n",
            C, s_log10y, s_alpha, mode, (unsigned long long)nsamples, (unsigned long long)nres, secs,
            1e6 * secs / (double)(total ? total : 1), (double)stepsum / (double)(total ? total : 1));
    free(res);
    return 0;
}
