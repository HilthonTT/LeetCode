#!/usr/bin/env bash
#
# Build one solution file.
#
#   ./build.sh Easy/two_sum.cpp [extra compiler flags...]
#
# The executable is written next to the source and named after it. Set CXX to
# pick a specific compiler; otherwise the first of g++, clang++ or cl that is on
# PATH is used.

set -euo pipefail

if [ $# -lt 1 ]; then
    echo "usage: $(basename "$0") <path-to-solution>.cpp [compiler flags...]" >&2
    exit 2
fi

src=$1
shift

[ -f "$src" ] || { echo "$src: no such file" >&2; exit 1; }

cxx=${CXX:-}
if [ -z "$cxx" ]; then
    for candidate in g++ clang++ cl; do
        if command -v "$candidate" >/dev/null 2>&1; then
            cxx=$candidate
            break
        fi
    done
fi
[ -n "$cxx" ] || { echo "no C++ compiler on PATH; set CXX" >&2; exit 1; }

out=${src%.*}
case $(uname -s 2>/dev/null || echo unknown) in
    MINGW*|MSYS*|CYGWIN*) out=$out.exe ;;
esac

case $(basename "$cxx") in
    cl|cl.exe)
        # cl scatters its output over the working directory unless told where to
        # put it, so name both the executable and the object file explicitly.
        "$cxx" /nologo /std:c++latest /EHsc /O2 "$@" "$src" \
            /Fe:"$out" /Fo:"${out%.exe}.obj"
        ;;
    *)
        "$cxx" -std=c++23 -O2 -Wall -Wextra -o "$out" "$src" "$@"
        ;;
esac

echo "built $out"
