// Is the higher early-escape probability of the first-passage entrance law explained by
// parity and scale alone?  For X = 2^k, B = floor(X^{1-2beta}), Y = X^{1-beta}, L = floor(ln X):
//  (1) uniform n in [1,B]                       (as in the research note)
//  (2) uniform odd n in [1,B]
//  (3) entrance law: first odd Syracuse iterate <= B from log-uniform odd starts in [2^{k+5}, 2^{k+20}]
//  (4) "scale-matched": escape probability of uniform odd n per dyadic shell, weighted with the
//      shell distribution of (3)
// If (3) and (4) agree, the excess over (1) is parity and scale, not 2-adic bias.
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <random>
#include <vector>
typedef unsigned __int128 u128;

static bool escapes(uint64_t n, double Y, int L) {
    u128 b = n;
    for (int t = 1; t <= L; t++) {
        b = (b & 1) ? (3 * b + 1) / 2 : b / 2;
        if ((long double)b > Y) return true;
    }
    return false;
}

int main(int argc, char **argv) {
    double beta = argc > 1 ? atof(argv[1]) : 0.1;
    long long N = argc > 2 ? atoll(argv[2]) : 2000000;
    for (int k : {20, 24, 28}) {
        double X = ldexp(1.0, k), Y = pow(X, 1 - beta);
        uint64_t B = (uint64_t)floor(pow(X, 1 - 2 * beta));
        int L = (int)floor(log(X));
        int S = 64 - __builtin_clzll(B);                  // shells [2^s, 2^{s+1}), s < S
        std::vector<double> esc_s(S, 0), cnt_s(S, 0);
        uint64_t e_all = 0, e_odd = 0, n_odd = 0;
        for (uint64_t n = 1; n <= B; n++) {
            bool e = escapes(n, Y, L);
            e_all += e;
            if (n & 1) {
                n_odd++; e_odd += e;
                int s = 63 - __builtin_clzll(n);
                cnt_s[s]++; esc_s[s] += e;
            }
        }
        std::mt19937_64 gen(20261118 + k);
        std::uniform_real_distribution<double> U((k + 5) * log(2.0), (k + 20) * log(2.0));
        std::vector<double> ent_s(S, 0);
        uint64_t hits = 0, e_ent = 0;
        for (long long j = 0; j < N; j++) {
            uint64_t m = (uint64_t)exp(U(gen)) | 1;
            u128 a = m; int steps = 0;
            while (a > B && steps < 100000) { u128 v = 3 * a + 1; while (!(v & 1)) v >>= 1; a = v; steps++; }
            if (a > B) continue;
            uint64_t y = (uint64_t)a; hits++;
            e_ent += escapes(y, Y, L);
            ent_s[63 - __builtin_clzll(y)]++;
        }
        double matched = 0;
        for (int s = 0; s < S; s++) if (cnt_s[s] > 0) matched += (ent_s[s] / hits) * (esc_s[s] / cnt_s[s]);
        double top2 = (ent_s[S - 1] + (S >= 2 ? ent_s[S - 2] : 0)) / hits;
        printf("X=2^%d beta=%.2f B=%llu L=%d | uniform %.4f | uniform odd %.4f | entrance %.4f (n=%llu) | "
               "scale-matched odd %.4f | entrance mass in top two shells %.3f\n",
               k, beta, (unsigned long long)B, L, (double)e_all / B, (double)e_odd / n_odd,
               (double)e_ent / hits, (unsigned long long)hits, matched, top2);
        fflush(stdout);
    }
}
