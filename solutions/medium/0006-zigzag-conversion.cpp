// 6. Zigzag Conversion (Medium)
// https://leetcode.com/problems/zigzag-conversion/

#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  std::string convert(std::string s, int numRows) {
    // With a single row the zigzag never turns, so the string is unchanged.
    // This also keeps the cycle length below from being zero.
    if (numRows == 1) {
      return s;
    }

    std::string result;

    // The answer is read row by row, so build it that way and skip the
    // intermediate grid entirely.
    for (int row = 0; row < numRows; row++) {
      // One cycle is a full column down plus the diagonal back up, which
      // together consume 2 * numRows - 2 characters. Every character in a
      // given row sits one cycle after the previous one in that row.
      int cycleLength = 2 * (numRows - 1);

      for (int i = row; i < static_cast<int>(s.length()); i += cycleLength) {
        // The character on the vertical stroke of this cycle.
        result += s[i];

        // Middle rows are visited twice per cycle: once going down, and
        // once on the diagonal coming back up. The diagonal character
        // is as far before the next cycle's vertical one as the
        // vertical one is after the previous cycle's, which puts it at
        // i + cycleLength - 2 * row. The first and last rows are only
        // touched by the vertical stroke, so they are skipped.
        if (row > 0 && row < numRows - 1 &&
            i + cycleLength - 2 * row < static_cast<int>(s.length())) {
          result += s[i + cycleLength - 2 * row];
        }
      }
    }

    return result;
  }
};

// The straightforward version of the problem: walk the zigzag one character at
// a time, filling in the rows, then join them. It is slower and uses more
// memory than the solution above, which is exactly why it is useful here — the
// generated cases below are too long to write expected answers out by hand.
std::string reference(const std::string& s, int numRows) {
  if (numRows == 1) {
    return s;
  }

  std::vector<std::string> rows(numRows);
  int row = 0;
  int direction = 1;  // +1 while travelling down, -1 while coming back up

  for (char c : s) {
    rows[row] += c;

    if (row == 0) {
      direction = 1;
    } else if (row == numRows - 1) {
      direction = -1;
    }

    row += direction;
  }

  std::string joined;
  for (const std::string& r : rows) {
    joined += r;
  }

  return joined;
}

// Some inputs below run to a thousand characters, so print their shape rather
// than a wall of text.
std::string preview(const std::string& s) {
  if (s.size() <= 32) {
    return "\"" + s + "\"";
  }

  return "\"" + s.substr(0, 14) + "..." + s.substr(s.size() - 14) + "\" (" +
         std::to_string(s.size()) + " chars)";
}

void check(const std::string& s, int numRows, const std::string& expected) {
  Solution sol;
  std::string got = sol.convert(s, numRows);

  std::cout << (got == expected ? "PASS" : "FAIL") << "  s=" << preview(s)
            << ", numRows=" << numRows << "\n  expected " << preview(expected)
            << "\n  got      " << preview(got) << "\n"
            << std::string(60, '-') << "\n";
}

// For inputs whose answer is impractical to spell out, compare against the
// row-filling reference instead of a literal.
void checkAgainstReference(const std::string& s, int numRows) {
  check(s, numRows, reference(s, numRows));
}

int main() {
  // --- the examples from the problem statement ---
  check("PAYPALISHIRING", 3, "PAHNAPLSIIGYIR");
  check("PAYPALISHIRING", 4, "PINALSIGYAHRPI");
  check("A", 1, "A");

  // --- a single row never turns, so nothing moves ---
  check("AB", 1, "AB");
  check("PAYPALISHIRING", 1, "PAYPALISHIRING");

  // --- two rows: the zigzag degenerates into odd and even positions ---
  check("ABC", 2, "ACB");
  check("ABCD", 2, "ACBD");
  check("ABCDE", 2, "ACEBD");

  // --- shortest inputs the constraints allow ---
  check("A", 2, "A");  // the first row is the only one reached
  check("AB", 2, "AB");
  check("AB", 3, "AB");  // the turn never happens

  // --- more rows than characters, so every character gets its own row and
  //     the order is unchanged ---
  check("ABC", 5, "ABC");
  check("ABC", 1000, "ABC");

  // --- exactly one full column, and one column plus part of a diagonal ---
  check("ABC", 3, "ABC");  // one straight column down
  check("ABCD", 3,
        "ABDC");  // 'D' lands on the diagonal, back in the middle row
  check("ABCDE", 3, "AEBDC");

  // --- the input is exactly one cycle, so the second column is a full
  //     diagonal and every middle row holds two characters ---
  check("ABCDEF", 4, "ABFCED");

  // --- characters other than letters are moved the same way ---
  check("0123456789", 4, "0615724839");
  check("a b c d", 3, "ac   bd");

  // --- repeated characters: the result is a permutation, so this only
  //     catches a solution that drops or duplicates one ---
  check(std::string(10, 'x'), 4, std::string(10, 'x'));

  // --- longer inputs, checked against the row-filling reference ---
  std::string alphabet;
  for (int i = 0; i < 26; i++) {
    alphabet += static_cast<char>('A' + i);
  }

  for (int numRows = 1; numRows <= 30; numRows++) {
    checkAgainstReference(alphabet, numRows);
  }

  // --- the largest input allowed: 1000 characters ---
  const size_t limit = 1000;

  std::string cycling;
  for (size_t i = 0; cycling.size() < limit; i++) {
    cycling += static_cast<char>('a' + (i % 26));
  }

  checkAgainstReference(cycling, 2);
  checkAgainstReference(cycling, 3);
  checkAgainstReference(cycling, 7);
  checkAgainstReference(cycling, 499);  // just under half the length
  checkAgainstReference(cycling, 500);  // one cycle spans the whole string
  checkAgainstReference(cycling,
                        999);  // the last character starts the diagonal
  checkAgainstReference(cycling, 1000);  // one character per row
  checkAgainstReference(cycling, 1000 + 1);

  // --- beyond the stated constraints, which promise at least one character ---
  check("", 1, "");
  check("", 5, "");

  return 0;
}
