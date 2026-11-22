// Monte Carlo estimate of the Syracuse characteristic function chi_n(xi) = E e(xi S_n / 3^n) at the
// unit frequencies xi = 2^k, k = round(4(n-1)/3) - W .. + W, for nmin <= n <= nmax <= 80
// (research notes, syracuse_lp, job B).
//
// S_n = sum_{j=1}^n 3^{j-1} 2^{-A_j} mod 3^n, A_j = a_1 + ... + a_j, a_i iid geometric, P(a = k) = 2^-k.
// One sample of a_1..a_nmax gives S_n = S_nmax mod 3^n for every n (prefix form), computed exactly
// in unsigned 128-bit arithmetic by Horner's rule from the inside: v <- 2^{-a_j}(1 + 3v) mod 3^{nmax-j+1},
// where 1 + 3v needs no reduction and division by 2 modulo an odd M is (v odd ? (v+M)/2 : v/2).
// The phase 2^k S_n mod 3^n is obtained by doubling; cos and sin are accumulated per (n, k).
//
// build: gcc -O3 -march=native -fopenmp -o syrac_fourier_mc syrac_fourier_mc.c -lm
// usage: syrac_fourier_mc NMIN NMAX SAMPLES SEED [W] > out.csv   (W = 6 by default)
#define _GNU_SOURCE
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 u128;
#define NMAXMAX 80          // values stay below 2*3^80 < 2^128
#define WMAX 8

static inline uint64_t rotl(const uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }
typedef struct { uint64_t s[4]; } rng_t;
static inline uint64_t next(rng_t *r) {             // xoshiro256**
    uint64_t *s = r->s;
    const uint64_t res = rotl(s[1] * 5, 7) * 9, t = s[1] << 17;
    s[2] ^= s[0]; s[3] ^= s[1]; s[1] ^= s[2]; s[0] ^= s[3]; s[2] ^= t; s[3] = rotl(s[3], 45);
    return res;
}
static uint64_t splitmix(uint64_t *x) {
    uint64_t z = (*x += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL; z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}
static inline int geometric(rng_t *r) {             // P(a = k) = 2^-k, k >= 1
    int a = 1;
    for (;;) {
        uint64_t x = next(r);
        if (x) return a + __builtin_ctzll(x);
        a += 64;
    }
}

int main(int argc, char **argv) {
    if (argc < 5) { fprintf(stderr, "usage: %s NMIN NMAX SAMPLES SEED [W]\n", argv[0]); return 1; }
    int nmin = atoi(argv[1]), nmax = atoi(argv[2]);
    long long N = atoll(argv[3]);
    uint64_t seed = strtoull(argv[4], 0, 10);
    int W = argc > 5 ? atoi(argv[5]) : 6;
    if (nmin < 1 || nmax > NMAXMAX || nmin > nmax || W < 0 || W > WMAX) { fprintf(stderr, "bad arguments\n"); return 1; }
    u128 P3[NMAXMAX + 2];
    P3[0] = 1;
    for (int i = 1; i <= NMAXMAX + 1; i++) P3[i] = P3[i - 1] * 3;
    int K = 2 * W + 1, NN = nmax - nmin + 1;
    int kmin[NMAXMAX + 1];
    for (int n = nmin; n <= nmax; n++) kmin[n] = (int)lround(4.0 * (n - 1) / 3.0) - W;
    size_t cells = (size_t)NN * K;
    double *C = calloc(cells, sizeof(double)), *Sn = calloc(cells, sizeof(double)),
           *C2 = calloc(cells, sizeof(double)), *S2 = calloc(cells, sizeof(double));
    long long done = 0;
#pragma omp parallel
    {
        int tid = omp_get_thread_num(), nt = omp_get_num_threads();
        rng_t r;
        uint64_t sm = seed * 1000003ULL + (uint64_t)tid * 7919ULL + 12345ULL;
        for (int i = 0; i < 4; i++) r.s[i] = splitmix(&sm);
        double *c = calloc(cells, sizeof(double)), *s = calloc(cells, sizeof(double)),
               *c2 = calloc(cells, sizeof(double)), *s2 = calloc(cells, sizeof(double));
        // block accumulators keep the double sums short
        double *bc = calloc(cells, sizeof(double)), *bs = calloc(cells, sizeof(double));
        long long lo = N * tid / nt, hi = N * (tid + 1) / nt, cnt = 0;
        int a[NMAXMAX + 1];
        for (long long it = lo; it < hi; it++) {
            for (int j = 1; j <= nmax; j++) a[j] = geometric(&r);
            u128 v = 0;
            for (int j = nmax; j >= 1; j--) {
                u128 M = P3[nmax - j + 1];
                v = 1 + 3 * v;
                for (int h = 0; h < a[j]; h++) v = (v & 1) ? (v + M) >> 1 : v >> 1;
            }
            for (int n = nmin; n <= nmax; n++) {
                u128 M = P3[n], X = v % M;
                for (int h = 0; h < kmin[n]; h++) { X <<= 1; if (X >= M) X -= M; }
                double Md = (double)M;
                size_t base = (size_t)(n - nmin) * K;
                for (int q = 0; q < K; q++) {
                    double th = 2.0 * M_PI * ((double)X / Md), sn, cs;
                    sincos(th, &sn, &cs);
                    bc[base + q] += cs; bs[base + q] += sn;
                    c2[base + q] += cs * cs; s2[base + q] += sn * sn;
                    X <<= 1; if (X >= M) X -= M;
                }
            }
            if (++cnt == 1000000) {
                for (size_t i = 0; i < cells; i++) { c[i] += bc[i]; s[i] += bs[i]; bc[i] = bs[i] = 0; }
                cnt = 0;
            }
        }
        for (size_t i = 0; i < cells; i++) { c[i] += bc[i]; s[i] += bs[i]; }
#pragma omp critical
        {
            for (size_t i = 0; i < cells; i++) { C[i] += c[i]; Sn[i] += s[i]; C2[i] += c2[i]; S2[i] += s2[i]; }
            done += hi - lo;
        }
        free(c); free(s); free(c2); free(s2); free(bc); free(bs);
    }
    printf("n,k,re,im,abs,stderr,samples\n");
    for (int n = nmin; n <= nmax; n++)
        for (int q = 0; q < K; q++) {
            size_t i = (size_t)(n - nmin) * K + q;
            double re = C[i] / done, im = Sn[i] / done;
            double var = (C2[i] / done - re * re) + (S2[i] / done - im * im);
            printf("%d,%d,%.10e,%.10e,%.10e,%.3e,%lld\n", n, kmin[n] + q, re, im, hypot(re, im), sqrt(var / done), done);
        }
    return 0;
}
