# LeetCode

[![CI](https://github.com/HilthonTT/LeetCode/actions/workflows/ci.yml/badge.svg)](https://github.com/HilthonTT/LeetCode/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Modern C++ solutions to [LeetCode](https://leetcode.com) problems.

This repository is my running log of LeetCode practice. Every problem gets its own
self-contained `.cpp` file: the `Solution` class exactly as LeetCode expects it, plus a
`main()` that runs a handful of checks, so any file can be compiled and executed on its
own with no build system, no dependencies and no shared headers.

## Conventions

Each solution file opens with a comment naming the problem number, title, difficulty and
its URL on leetcode.com, and is named after the same number and title slug. Solutions are
grouped by the difficulty LeetCode assigns.

Running a compiled solution prints one line per check — `PASS` or `FAIL`, followed by the
inputs, the expected result and the actual one.

## Building

Solutions target C++23, so a recent compiler is required: GCC 14+, Clang 18+ or MSVC
19.3x+. A file is a single translation unit, so compiling one directly is always enough:

```sh
# GCC / Clang
g++ -std=c++23 -O2 -o solution <path-to-solution>.cpp && ./solution

# MSVC
cl /std:c++latest /EHsc <path-to-solution>.cpp
```

A helper script picks the compiler and the newest standard it accepts, and writes the
executable next to the source:

```sh
./scripts/build.sh <path-to-solution>.cpp   # or just the file name — it is searched for
make clean                                  # remove every binary and object file left behind
```

## Contributing

This is a personal practice log, so pull requests adding solutions are not expected — but
if you spot a wrong answer or a case that is not covered, an issue or a PR is welcome.
[CONTRIBUTING.md](CONTRIBUTING.md) documents the file template and naming rules that CI
enforces.

## License

[MIT](LICENSE)
