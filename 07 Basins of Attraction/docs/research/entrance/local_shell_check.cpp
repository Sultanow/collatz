// Independent check of the local first-passage progression theorem (3x+1, beta = 1/200).
// Part A (exhaustive, odd n in (B, 2B]):  for every valuation word of the first passage below B,
//   the starts form one class mod 2^{A+1}, consecutive starts differ by 2^{A+1}, the entrance points
//   differ by 2*3^k, and every odd class mod 2^L receives N/m +- 1 of them.  Also: collision energy
//   Q = m * sum_r q(r)^2 of the entrance law, P(tau <= 5), early-excursion probability.
// Part B (sampled): debiased collision energy of the entrance law from log-uniform odd starts in
//   [2^{k+5}, 2^{k+20}] (the research note's diagnostic), N samples.
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <map>
#include <random>
#include <string>
#include <vector>
typedef unsigned __int128 u128;
typedef uint64_t u64;

int main(int argc, char **argv) {
    double beta = 1.0 / 200;
    long long NS = argc > 1 ? atoll(argv[1]) : 2000000;
    for (int k : {20, 22, 24}) {
        long double X = ldexpl(1.0L, k), Y = powl(X, 1 - beta);
        u64 B = (u64)floorl(powl(X, 1 - 2 * beta));
        int L = (int)floorl(logl(X));
        u64 M = 1ULL << L, m = M / 2;
        // ---- Part A
        std::vector<u64> cnt(M, 0);
        std::map<std::string, std::vector<std::pair<u64, u64>>> words;   // word -> (n, y)
        u64 R = 0, tau5 = 0, early = 0;
        bool exhaustive_words = (k <= 22);
        for (u64 n = (B + 1) | 1; n <= 2 * B; n += 2) {
            if (!(n & 1)) continue;
            R++;
            u64 y = n; int tau = 0; std::string w;
            while (y > B) {
                u64 t = 3 * y + 1; int a = __builtin_ctzll(t); y = t >> a; tau++;
                if (exhaustive_words) w.push_back((char)a);
            }
            if (tau <= 5) tau5++;
            cnt[y & (M - 1)]++;
            if (exhaustive_words) words[w].push_back({n, y});
            u128 z = y;
            for (int t = 1; t <= L; t++) { z = (z & 1) ? (3 * z + 1) / 2 : z / 2; if ((long double)z > Y) { early++; break; } }
        }
        long double s2 = 0;
        for (u64 r = 1; r < M; r += 2) s2 += (long double)cnt[r] * cnt[r];
        double Q = (double)(m * s2 / ((long double)R * R));
        long long worst_dev = 0; u64 nwords = 0; bool ap_ok = true;
        if (exhaustive_words) {
            for (auto &kv : words) {
                const std::string &w = kv.first; auto &v = kv.second; nwords++;
                int A = 0; for (char a : w) A += a;
                int kk = (int)w.size();
                u128 dn = (u128)1 << (A + 1); u128 p3 = 1; for (int i = 0; i < kk; i++) p3 *= 3;
                for (size_t i = 1; i < v.size(); i++) {
                    if ((u128)(v[i].first - v[i - 1].first) != dn) ap_ok = false;
                    if ((u128)(v[i].second - v[i - 1].second) != 2 * p3) ap_ok = false;
                }
                std::vector<u64> c(M, 0);
                for (auto &p : v) c[p.second & (M - 1)]++;
                long double expct = (long double)v.size() / m;
                for (u64 r = 1; r < M; r += 2) {
                    long long d = (long long)llroundl(fabsl((long double)c[r] - expct) + 0.4999L);
                    if (fabsl((long double)c[r] - expct) > worst_dev) worst_dev = (long long)ceill(fabsl((long double)c[r] - expct));
                }
            }
        }
        // ---- Part B
        std::mt19937_64 gen(777 + k);
        std::uniform_real_distribution<double> U((k + 5) * log(2.0), (k + 20) * log(2.0));
        std::vector<u64> cs(M, 0); u64 hits = 0;
        for (long long j = 0; j < NS; j++) {
            u128 a = ((u64)expl(U(gen))) | 1; int st = 0;
            while (a > B && st < 100000) { u128 v = 3 * a + 1; while (!(v & 1)) v >>= 1; a = v; st++; }
            if (a > B) continue;
            cs[(u64)a & (M - 1)]++; hits++;
        }
        long double sc = 0;
        for (u64 r = 1; r < M; r += 2) sc += (long double)cs[r] * (cs[r] - 1);
        double Qs = (double)(m * sc / ((long double)hits * (hits - 1)));
        printf("X=2^%d B=%llu L=%d m=%llu | local shell: R=%llu P(tau<=5)=%.5f Q=%.5f early=%.5f",
               k, (unsigned long long)B, L, (unsigned long long)m, (unsigned long long)R,
               (double)tau5 / R, Q, (double)early / R);
        if (exhaustive_words) printf(" words=%llu AP_ok=%s max|count-N/m|<=%lld", (unsigned long long)nwords, ap_ok ? "yes" : "NO", worst_dev);
        printf(" | sampled entrance (N=%lld): Q_debiased=%.5f\n", NS, Qs);
        fflush(stdout);
    }
}
