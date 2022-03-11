#include <cstdint>
#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include <cstring>
#include <stdexcept>
#include <unordered_set>
#include <map>
#include <tuple>
#include <set>
#include <algorithm>

#define ASSERT_MSG(cond, msg) { if (!(cond)) throw std::runtime_error("Assertion (" #cond ") failed at line " + std::to_string(__LINE__) + "! Msg: '" + std::string(msg) + "'."); }
#define ASSERT(cond) ASSERT_MSG(cond, "")

inline bool exact_divisibility_two(int64_t x) {
    int64_t d = 0;
    while (x % 2 == 0) {
        x /= 2;
        d += 1;
    }
    return d;
}

std::vector<int64_t> generate_collatz_set(
        int64_t q, int64_t v1, int64_t n) {
    std::vector<int64_t> collatz_set;
    if (n < 1 || v1 % 2 == 0)
        return collatz_set;
    int64_t vi = v1;
    collatz_set.push_back(vi);
    for (int64_t i = 0; i < n - 1; ++i) {
        vi = q * vi + 1;
        while (vi % 2 == 0) {
            vi /= 1 << exact_divisibility_two(vi);
            collatz_set.push_back(vi);
        }
    }
    return collatz_set;
}

void find_collatz(int64_t q_lim, int64_t x0_lim, int64_t n_lim, bool show_all) {
    std::unordered_set<int64_t> present;
    std::vector<int64_t> seq;
    std::map<std::string, std::vector<std::tuple<int64_t, int64_t>>> ans_all, ans_odd, ans_rot;

    auto FindCycle = [&](std::vector<int64_t> const & v) {
        std::vector<int64_t> r;
        for (ptrdiff_t i = ptrdiff_t(v.size()) - 2; i >= 0; --i)
            if (v[i] == v.back()) {
                for (size_t j = i; j < v.size() - 1; ++j)
                    r.push_back(v[j]);
                return r;
            }
        return r;
    };

    auto CycleRot = [&](std::vector<int64_t> v) {
        std::rotate(v.begin(), std::min_element(v.begin(), v.end()), v.end());
        return v;
    };

    std::function<bool(bool, int, int, int)> F =
        [&](bool capture, auto q, auto x0, auto n) {
            int64_t x = x0;
            seq.clear();
            present.clear();
            for (int64_t icycle = 0; icycle < n; ++icycle) {
                if (present.count(x)) {
                    seq.push_back(x);
                    if (!capture)
                        return F(true, q, x0, n_lim);
                    if (show_all) {
                        std::cout
                            << "q " << q << " x0 " << x0
                            << " len " << icycle << std::endl;
                        for (auto x: seq)
                            std::cout << x << ", ";
                        std::cout << std::endl;
                    }
                    auto const res = FindCycle(seq);
                    if (show_all) {
                        for (auto x: res)
                            std::cout << x << ", ";
                        std::cout << "(";
                        for (auto x: res)
                            if (x & 1)
                                std::cout << x << ", ";
                        std::cout << ")" << std::endl;
                    }
                    std::stringstream ss, ss2, ss3;
                    for (auto x: res)
                        ss << x << ", ";
                    ss << " (";
                    for (auto x: res)
                        if (x & 1) {
                            ss << x << ", ";
                            ss2 << x << ", ";
                        }
                    ss << ")";
                    ans_all[ss.str()].push_back(std::make_tuple(q, x0));
                    ans_odd[ss2.str()].push_back(std::make_tuple(q, x0));
                    auto const cycle_rot = CycleRot(res);
                    for (auto x: cycle_rot)
                        if (x & 1)
                            ss3 << x << ", ";
                    ans_rot[ss3.str()].push_back(std::make_tuple(q, x0));
                    return true;
                }
                present.insert(x);
                if (capture)
                    seq.push_back(x);
                if (x >= (1ULL << 50))
                    break;
                x = x & 1 ? q * x + 1 : x >> 1;
            }
            return false;
        };
    if (show_all)
        std::cout << "All found sequences:" << std::endl;
    for (int64_t q = 3; q < q_lim; q += 2)
        for (int64_t x0 = 1; x0 < x0_lim; ++x0)
            F(false, q, x0, n_lim);
    std::cout << std::endl << "All found cycles:" << std::endl;
    for (auto const & [k, v]: ans_all) {
        std::cout << k << " (";
        for (auto [q_, x0_]: v)
            std::cout << "q = " << q_ << ", x0 = " << x0_ << "; ";
        std::cout << ")" << std::endl;
    }
    std::cout << std::endl << "All found odd cycles:" << std::endl;
    for (auto const & [k, v]: ans_odd) {
        std::cout << k << " (";
        for (auto [q_, x0_]: v)
            std::cout << "q = " << q_ << ", x0 = " << x0_ << "; ";
        std::cout << ")" << std::endl;
    }
    std::cout << std::endl << "All found odd rotated cycles:" << std::endl;
    for (auto const & [k, v]: ans_rot) {
        std::cout << k << " (";
        for (auto [q_, x0_]: v)
            std::cout << "q = " << q_ << ", x0 = " << x0_ << "; ";
        std::cout << ")" << std::endl;
    }
}

int main() {
    try {
        find_collatz(4100, 1000, 1000, true);
        return 0;
    } catch (std::exception const & ex) {
        std::cout << "Exception: " << ex.what() << std::endl;
        return -1;
    }
}