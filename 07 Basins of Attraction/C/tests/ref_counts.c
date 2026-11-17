/* Brute-force predecessor counts in the output format of `potential` (a,count,cycle_min),
   for testing ui_report.py (Run K) on small instances.
   usage: ref_counts C N A > out.csv
   pi_a(N) = #{1 <= n <= N : a lies on the orbit of n under T_{3,c}}; cycle_min = minimum of
   the cycle if a is periodic, else 0. Every orbit is followed until it closes a cycle inside
   [1, A] (all cycles of the tested maps lie there; a step cap guards against surprises). */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static inline uint64_t T(uint64_t x, int64_t c) {
    return (x & 1) ? (uint64_t)((3 * (__int128)x + c) / 2) : x / 2;
}

int main(int argc, char **argv) {
    if (argc != 4) { fprintf(stderr, "usage: ref_counts C N A\n"); return 2; }
    int64_t c = atoll(argv[1]);
    uint64_t N = strtoull(argv[2], 0, 10), A = strtoull(argv[3], 0, 10);
    uint64_t *cnt = calloc(A + 1, sizeof *cnt), *stamp = calloc(A + 1, sizeof *stamp);
    if (!cnt || !stamp) return 1;
    for (uint64_t n = 1; n <= N; n++) {
        uint64_t x = n;
        for (long s = 0; s < 10000000; s++) {
            if (x <= A) {
                if (stamp[x] == n) break;
                stamp[x] = n;
                cnt[x]++;
            }
            x = T(x, c);
            if (s == 10000000 - 1) { fprintf(stderr, "step cap at n=%llu\n", (unsigned long long)n); return 1; }
        }
    }
    printf("a,count,cycle_min\n");
    for (uint64_t a = 1; a <= A; a++) {
        uint64_t x = a, mn = a, per = 0;
        for (int s = 0; s < 100000; s++) {
            x = T(x, c);
            if (x < mn) mn = x;
            if (x == a) { per = 1; break; }
            if (x > (uint64_t)1 << 62) break;
        }
        printf("%llu,%llu,%llu\n", (unsigned long long)a, (unsigned long long)cnt[a],
               (unsigned long long)(per ? mn : 0));
    }
    return 0;
}
