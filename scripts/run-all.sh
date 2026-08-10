#!/usr/bin/env bash
#
# Compile and run every solution in the repository.
#
#   ./scripts/run-all.sh              # all of them
#   ./scripts/run-all.sh easy medium  # only the given difficulties
#
# Building is delegated to scripts/build.sh, so CXX and CXXSTD work here too. A solution
# reports its checks as PASS/FAIL lines rather than through its exit code, so a
# run counts as failed if the process fails *or* if it printed FAIL.

set -uo pipefail

root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$root"

difficulties=("$@")
[ ${#difficulties[@]} -gt 0 ] || difficulties=(easy medium hard)

shopt -s nullglob
sources=()
for difficulty in "${difficulties[@]}"; do
    sources+=("solutions/$difficulty"/*.cpp)
done

if [ ${#sources[@]} -eq 0 ]; then
    echo "no solutions found for: ${difficulties[*]}" >&2
    exit 1
fi

passed=0
failed=()

for src in "${sources[@]}"; do
    echo "::group::$src" 2>/dev/null || true
    printf '=== %s\n' "$src"

    if ! output=$(./scripts/build.sh "$src" 2>&1); then
        printf '%s\n' "$output"
        failed+=("$src (compile)")
        echo "::endgroup::" 2>/dev/null || true
        continue
    fi

    binary=${src%.cpp}
    [ -x "$binary" ] || binary=$binary.exe

    if ! output=$("$binary" 2>&1); then
        printf '%s\n' "$output"
        failed+=("$src (crashed or exited non-zero)")
    elif grep -q 'FAIL' <<< "$output"; then
        printf '%s\n' "$output"
        failed+=("$src (check failed)")
    else
        printf '%s\n' "$output"
        passed=$((passed + 1))
    fi

    rm -f "$binary" "${binary%.exe}.obj"
    echo "::endgroup::" 2>/dev/null || true
done

echo
echo "$passed/${#sources[@]} solutions passed"

if [ ${#failed[@]} -gt 0 ]; then
    echo "failed:" >&2
    printf '  %s\n' "${failed[@]}" >&2
    exit 1
fi
