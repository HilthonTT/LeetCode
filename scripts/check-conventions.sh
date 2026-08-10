#!/usr/bin/env bash
#
# Check that every solution follows the repository conventions:
#
#   * it lives in solutions/<easy|medium|hard>/
#   * it is named <4-digit-problem-number>-<title-slug>.cpp
#   * it opens with a header comment naming the problem, difficulty and URL
#   * the number and difficulty in the header match the ones in the path
#
# Prints one line per violation and exits non-zero if there was any.

set -uo pipefail

root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$root"

status=0
fail() {
    echo "$1" >&2
    status=1
}

# A .cpp anywhere else is either a stray file or a directory that was never
# meant to hold solutions; both are worth reporting.
while IFS= read -r file; do
    case $file in
        ./solutions/easy/* | ./solutions/medium/* | ./solutions/hard/*) ;;
        *) fail "$file: solutions belong in solutions/{easy,medium,hard}/" ;;
    esac
done < <(find . -name '*.cpp' -type f -not -path './.git/*')

shopt -s nullglob
for file in solutions/*/*.cpp; do
    difficulty=$(basename "$(dirname "$file")")
    name=$(basename "$file")

    if [[ ! $name =~ ^([0-9]{4})-([a-z0-9]+(-[a-z0-9]+)*)\.cpp$ ]]; then
        fail "$file: name must be <NNNN>-<title-slug>.cpp"
        continue
    fi
    number=${BASH_REMATCH[1]}
    slug=${BASH_REMATCH[2]}

    # Leading zeros are padding for sorting, not part of the problem number, so
    # the header carries the number as LeetCode writes it.
    unpadded=$((10#$number))

    header=$(head -n 2 "$file")
    expected_title="^// $unpadded\. .+ \(${difficulty^})$"
    expected_url="^// https://leetcode\.com/problems/$slug/$"

    if ! sed -n 1p <<< "$header" | grep -Eq "$expected_title"; then
        fail "$file: first line must be '// $unpadded. <Title> (${difficulty^})'"
    fi
    if ! sed -n 2p <<< "$header" | grep -Eq "$expected_url"; then
        fail "$file: second line must be '// https://leetcode.com/problems/$slug/'"
    fi
done

# Two files claiming the same problem number is nearly always a copy that was
# renamed but never finished.
duplicates=$(
    for file in solutions/*/*.cpp; do
        basename "$file" | cut -d- -f1
    done | sort | uniq -d
)
if [ -n "$duplicates" ]; then
    while IFS= read -r number; do
        fail "problem $number has more than one solution file"
    done <<< "$duplicates"
fi

[ $status -eq 0 ] && echo "conventions OK: $(ls solutions/*/*.cpp | wc -l) solutions"
exit $status
