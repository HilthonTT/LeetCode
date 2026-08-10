# Contributing

This repository is a personal LeetCode practice log rather than a shared solution
collection, so it is not looking for new problems from contributors. Corrections are very
welcome: a solution that is wrong, a solution that is accepted but has the complexity
described incorrectly, or an input that is not covered by its checks.

## Reporting a problem

Open an issue with the problem number, what you expected, and the input that shows the
difference. If you can, include the compiler and standard you used — a solution that fails
only under one toolchain is a different bug from one that is simply wrong.

## Adding or changing a solution

One problem, one file, no shared headers: every file has to compile and run on its own.
CI enforces the rules below on every push and pull request; run `./scripts/check-conventions.sh`
locally to check them before pushing.

**Location.** `solutions/<difficulty>/`, where the difficulty is `easy`, `medium` or
`hard`, exactly as LeetCode assigns it.

**File name.** The problem number padded to four digits, then the title slug from the
problem's URL on leetcode.com:

```
solutions/easy/0001-two-sum.cpp
solutions/hard/0010-regular-expression-matching.cpp
```

The padding exists so the files sort by number; problems numbered above 9999 keep their
own width.

**Header.** The first two lines name the problem and link to it. The number, difficulty
and slug have to agree with the path:

```cpp
// 1. Two Sum (Easy)
// https://leetcode.com/problems/two-sum/
```

**Shape.** After the header come the includes, then the `Solution` class copied verbatim
from LeetCode — same name, same signature, no changes to make it nicer to call locally —
then whatever helpers the checks need, then `main()`.

**Checks.** `main()` runs the examples from the problem statement plus the edge cases the
constraints allow: empty input, a single element, the minimum and maximum values, ties.
Each check prints `PASS` or `FAIL` followed by the input, the expected result and the
actual one. CI treats a `FAIL` anywhere in the output as a failed build, so a check that
is known to fail cannot be committed.

## Building

`./scripts/build.sh <path>` compiles a single file with the newest standard the compiler accepts,
and `./scripts/run-all.sh [difficulty...]` compiles and runs the whole set the way CI
does. Both honour `CXX` and `CXXSTD`. `make clean` removes the binaries and object files
they leave behind.

Solutions target C++23, so they need GCC 14+, Clang 18+ or MSVC 19.3x+.
