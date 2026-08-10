#!/usr/bin/env bash
#
# Build one solution file.
#
#   ./scripts/build.sh solutions/easy/0001-two-sum.cpp [extra compiler flags...]
#   ./scripts/build.sh 0001-two-sum     # searched for under the repo
#
# Runs from any working directory: paths are tried as given, then relative to
# the repo root, then matched by name against the .cpp files in the repo.
#
# The executable is written next to the source and named after it. Set CXX to
# pick a specific compiler; otherwise the first of g++, clang++ or cl that is on
# PATH is used. Set CXXSTD to force a standard instead of probing for the
# newest one the compiler accepts.

set -euo pipefail

if [ $# -lt 1 ]; then
    echo "usage: $(basename "$0") <path-to-solution>.cpp [compiler flags...]" >&2
    exit 2
fi

# Where the script itself lives, following a symlink if we were invoked through
# one. Everything below is anchored here rather than to $PWD.
self=${BASH_SOURCE[0]}
while [ -L "$self" ]; do
    link=$(readlink "$self")
    case $link in
        /*) self=$link ;;
        *) self=$(dirname "$self")/$link ;;
    esac
done
root=$(cd -- "$(dirname -- "$self")/.." && pwd)

query=$1
shift

resolve() {
    local q=$1

    # As typed, relative to $PWD or absolute.
    [ -f "$q" ] && { printf '%s\n' "$q"; return 0; }

    # Relative to the repo root.
    [ -f "$root/$q" ] && { printf '%s\n' "$root/$q"; return 0; }

    # By name anywhere in the repo, with or without the extension. Sorted so a
    # given input always picks the same file.
    local base=${q%.cpp}
    local -a hits=()
    while IFS= read -r hit; do
        hits+=("$hit")
    done < <(find "$root" -name "$base.cpp" -type f -not -path '*/.git/*' | sort)

    case ${#hits[@]} in
        0) return 1 ;;
        1) printf '%s\n' "${hits[0]}"; return 0 ;;
        *)
            echo "$q is ambiguous:" >&2
            printf '  %s\n' "${hits[@]}" >&2
            return 1
            ;;
    esac
}

src=$(resolve "$query") || {
    echo "$query: no such file under $root" >&2
    exit 1
}

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

# Compilers older than the flag they are handed fail on it, so ask for the
# newest standard this one actually accepts.
pick_std() {
    local flag=$1 std
    for std in c++23 c++20 c++17; do
        if echo 'int main(){}' | "$cxx" "$flag$std" -x c++ - -fsyntax-only >/dev/null 2>&1; then
            printf '%s\n' "$std"
            return 0
        fi
    done
    return 1
}

case $(basename "$cxx") in
    cl|cl.exe)
        std=${CXXSTD:-c++latest}
        # cl scatters its output over the working directory unless told where to
        # put it, so name both the executable and the object file explicitly.
        "$cxx" /nologo "/std:$std" /EHsc /O2 "$@" "$src" \
            /Fe:"$out" /Fo:"${out%.exe}.obj"
        ;;
    *)
        std=${CXXSTD:-$(pick_std -std= || echo c++17)}
        "$cxx" "-std=$std" -O2 -Wall -Wextra -o "$out" "$src" "$@"
        ;;
esac

echo "built $out"
