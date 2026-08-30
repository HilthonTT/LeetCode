// 10. Regular Expression Matching (Hard)
// https://leetcode.com/problems/regular-expression-matching/

#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  bool isMatch(const std::string& s, const std::string& p) {
    const size_t n = s.size();
    const size_t m = p.size();

    // DP table where dp[i][j] means whether the first i characters in s
    // match the first j characters in p
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(m + 1, false));

    // Empty pattern matches empty text
    dp[0][0] = true;

    // The empty text is matched only by a pattern that can vanish
    // completely: a*, a*b*, a*b*c* etc. Every '*' drops both itself and
    // the character it repeats, so the answer carries over from j - 2.
    for (size_t j = 2; j <= m; ++j) {
      if (p[j - 1] == '*') {
        dp[0][j] = dp[0][j - 2];
      }
    }

    // Fill the table
    for (size_t i = 1; i <= n; ++i) {
      for (size_t j = 1; j <= m; ++j) {
        // '*' is tested first: it is a quantifier on the preceding
        // pattern character, never a character to compare s[i - 1]
        // against. Testing it second would misread a literal '*' in
        // the text as a match.
        if (p[j - 1] == '*') {
          // A leading '*' has nothing to repeat. LeetCode never
          // produces one, so treat the malformed pattern as
          // unmatchable rather than reading past the front.
          if (j < 2) {
            continue;
          }

          // Two cases:
          // 1. '*' represents zero occurrences of the preceding
          //    character, so skip the pair
          // 2. '*' consumes s[i - 1] and stays available for the
          //    rest of the text
          dp[i][j] =
              dp[i][j - 2] || (dp[i - 1][j] && matches(s[i - 1], p[j - 2]));
        }

        // Characters match
        else if (matches(s[i - 1], p[j - 1])) {
          dp[i][j] = dp[i - 1][j - 1];
        }
      }
    }

    return dp[n][m];
  }

 private:
  // One pattern character consumes one text character: '.' takes anything,
  // any other character only itself.
  static bool matches(char text, char pattern) {
    return pattern == '.' || pattern == text;
  }
};

void check(const std::string& s, const std::string& p, bool expected) {
  Solution sol;
  bool got = sol.isMatch(s, p);

  // keep the printed input short so the stress cases stay readable
  auto shorten = [](const std::string& v) {
    return v.size() <= 30 ? v : v.substr(0, 27) + "...";
  };

  std::cout << (got == expected ? "PASS" : "FAIL") << "  s=\"" << shorten(s)
            << "\" (" << s.size() << ")"
            << "  p=\"" << shorten(p) << "\" (" << p.size() << ")"
            << "  expected=" << std::boolalpha << expected << "  got=" << got
            << "\n"
            << std::string(60, '-') << "\n";
}

int main() {
  // --- the LeetCode examples ---
  check("aa", "a", false);  // 'a' does not cover the whole text
  check("aa", "a*", true);  // one 'a' repeated
  check("ab", ".*", true);  // any character, any number of times
  check("aab", "c*a*b", true);
  check("mississippi", "mis*is*p*.", false);
  check("mississippi", "mis*is*ip*.", true);

  // --- empty text, empty pattern ---
  check("", "", true);
  check("", "a", false);
  check("", ".", false);
  check("", "a*", true);      // the star can erase its character
  check("", "a*b*c*", true);  // ... and so can a whole row of them
  check("", ".*", true);
  check("", ".*.*", true);
  check("", "a*b", false);  // the trailing 'b' still has to match
  check("a", "", false);    // an empty pattern matches only empty text

  // --- plain literals, no metacharacters ---
  check("abc", "abc", true);
  check("abc", "abd", false);
  check("abc", "ab", false);  // pattern too short
  check("ab", "abc", false);  // pattern too long
  check("a", "a", true);
  check("a", "b", false);

  // --- '.' matches exactly one character, never zero ---
  check("a", ".", true);
  check("abc", "...", true);
  check("abc", "..", false);
  check("abc", "....", false);
  check("abc", "a.c", true);
  check("a.c", "a.c", true);  // a literal dot in the text is fine too
  check("abc", "a.d", false);

  // --- '*' on a literal ---
  check("aaa", "a*", true);
  check("aaa", "a", false);
  check("aaa", "aaaa", false);
  check("a", "ab*", true);   // b* contributes nothing
  check("ab", "ab*", true);  // ... and here it takes the one 'b'
  check("abb", "ab*", true);
  check("ab", "a*b", true);
  check("ab", "a*b*", true);
  check("ba", "a*b*", false);  // the pattern is ordered, the text is not
  check("aaa", "a*a", true);
  check("aaa", "a*a*a*", true);
  check("abcd", "d*", false);
  check("xyz", "x*y*z*", true);
  check("aaa", "ab*a*c*a", true);
  check("aaba", "ab*a*c*a", false);

  // --- '.*' as the catch-all ---
  check("abcd", ".*", true);
  check("abc", "a.*", true);
  check("abc", ".*c", true);
  check("abc", ".*d", false);
  check("ab", ".*c", false);
  check("ab", ".*..", true);    // the star gives up both characters
  check("a", ".*..a*", false);  // ".." needs two characters, the text has one
  check("bbbba", ".*a*a", true);
  check("abcdefg", ".*.*.*", true);
  check("abcdefghijklmnopqrstuvwxyz", ".*", true);

  // --- the star has to be able to give characters back ---
  // A greedy scan that never reconsiders gets these wrong
  check("aaa", "a*aa", true);
  check("aaa", "a*aaa", true);
  check("aaa", "a*aaaa", false);
  check("aasdfasdfasdfasdfas", "aasdf.*asdf.*asdf.*asdf.*s", true);

  // --- case is significant ---
  check("A", "a", false);
  check("aA", "aA", true);
  check("aA", "a.", true);
  check("AAA", "a*", false);

  // --- malformed patterns LeetCode never hands out ---
  check("a", "*a", false);  // nothing for the leading star to repeat
  check("", "*", false);

  // --- stress: nothing here should take measurable time ---
  // A naive backtracking matcher goes exponential on these
  check(std::string(30, 'a'), std::string(30, 'a') + "*", true);
  check(std::string(30, 'a'), std::string(31, 'a') + "*", true);
  check(std::string(30, 'a'), std::string(32, 'a') + "*", false);

  std::string manyStars;
  for (int i = 0; i < 100; ++i) {
    manyStars += "a*";
  }
  check(std::string(2000, 'a'), manyStars, true);
  check(std::string(2000, 'a') + "b", manyStars, false);
  check(std::string(30, 'a'), manyStars + "b", false);

  std::string dotStars;
  for (int i = 0; i < 100; ++i) {
    dotStars += ".*";
  }
  check(std::string(2000, 'z'), dotStars, true);

  return 0;
}
