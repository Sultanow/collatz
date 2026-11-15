#!/usr/bin/env python3
"""Independent ARITHMETIC smoke test, not a Lean proof or verification.

Confirms over a finite range that Python's exact-integer simulation agrees with
our algebraic descriptions and finite-horizon hitting recursion.
"""
from fractions import Fraction


def step(c: int, n: int) -> int:
    return n // 2 if n % 2 == 0 else (3 * n + c) // 2


def candidates(c: int, a: int) -> set[int]:
    return {2 * a, (2 * a - c) // 3}


def predecessors(c: int, a: int) -> set[int]:
    return {n for n in candidates(c, a) if step(c, n) == a}


def reaches_within(c: int, n: int, a: int, N: int) -> bool:
    for _ in range(N + 1):
        if n == a:
            return True
        n = step(c, n)
    return False


def main() -> None:
    tested = 0
    for c in (-1, 1, 3, 5, 13, 29):
        for n in range(-200, 201):
            a = step(c, n)
            assert n in predecessors(c, a)
            assert (n == 2 * a) or (n % 2 == 1 and 3 * n + c == 2 * a)
            tested += 1
        for a in range(-200, 201):
            for n in candidates(c, a):
                assert (n in predecessors(c, a)) == (step(c, n) == a)
                tested += 1
    for c in (-1, 1, 5, 13):
        for n in range(1, 150):
            assert step(c, n) > 0
        for n in range(1, 30):
            for a in range(1, 30):
                for N in range(5):
                    assert reaches_within(c, n, a, N + 1) == (
                        n == a or reaches_within(c, step(c, n), a, N)
                    )
                    tested += 1
    source = range(1, 40)
    c, a, N = -1, 17, 8
    w = lambda n: Fraction(n * n + 1, 39 * (39 + 1))
    lhs = sum((w(n) for n in source if reaches_within(c, n, a, N + 1)), Fraction(0))
    rhs = sum((w(n) for n in source if n == a or reaches_within(c, step(c, n), a, N)), Fraction(0))
    assert lhs == rhs
    print(f"PASS: {tested} finite arithmetic and recursion checks; weighted identity holds.")
    print("Note: these tests do not type-check Lean and are not certificates.")


if __name__ == "__main__":
    main()
