# LeetCode

Modern C++ solutions to LeetCode problems.

This repository is my running log of LeetCode practice. Each problem gets its own
self-contained `.cpp` file with the `Solution` class as LeetCode expects it, plus a
`main()` that runs the sample cases so the file can be compiled and executed on its own.

## Layout

Solutions are grouped by the difficulty LeetCode assigns them:

```
Easy/
Medium/
Hard/
```

File names are the problem title in `snake_case` (e.g. `maximum_product_of_two_digits.cpp`).

## Building

The solutions target C++23 (they use headers such as `<print>`), so a recent compiler is
required — MSVC 19.3x+, GCC 14+, or Clang 18+.

```sh
# MSVC
cl /std:c++latest /EHsc Easy/maximum_product_of_two_digits.cpp

# GCC / Clang
g++ -std=c++23 -O2 -o solution Easy/maximum_product_of_two_digits.cpp
```

Running the produced binary prints the expected and actual result for each sample case.

## License

[MIT](LICENSE)
