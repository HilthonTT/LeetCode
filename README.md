# LeetCode

Modern C++ solutions to LeetCode problems.

This repository is my running log of LeetCode practice. Each problem gets its own
self-contained `.cpp` file with the `Solution` class as LeetCode expects it, plus a
`main()` that runs a set of checks so the file can be compiled and executed on its own.

## Layout

Solutions are grouped into one directory per difficulty level, as LeetCode assigns it.
File names are the problem title in `snake_case`.

## Building

The solutions target C++23 (they use headers such as `<print>`), so a recent compiler is
required — MSVC 19.3x+, GCC 14+, or Clang 18+.

```sh
# MSVC
cl /std:c++latest /EHsc <path-to-solution>.cpp

# GCC / Clang
g++ -std=c++23 -O2 -o solution <path-to-solution>.cpp
```

Running the produced binary prints one line per check — `PASS` or `FAIL`, followed by
the inputs, the expected result and the actual one.

```
make clean   # remove every .obj and .exe left behind in the repository
```

## License

[MIT](LICENSE)
