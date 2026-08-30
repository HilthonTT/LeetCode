// 5. Longest Palindromic Substring (Medium)
// https://leetcode.com/problems/longest-palindromic-substring/

#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  std::string longestPalindrome(std::string s) {
    int n = s.size();
    std::vector<std::vector<bool>> dp(n, std::vector<bool>(n, false));

    // dp[i][j] if the substring  from [i to j]
    // is a palindrome or not

    int start = 0, maxLen = 1;

    // all substrings of length 1 are palindromes
    for (int i = 0; i < n; ++i) dp[i][i] = true;

    // check for substrings of length 2
    for (int i = 0; i < n - 1; ++i) {
      if (s[i] == s[i + 1]) {
        dp[i][i + 1] = true;
        if (maxLen == 1) {
          start = i;
          maxLen = 2;
        }
      }
    }

    // check for substrings of length 3 and more
    for (int len = 3; len <= n; ++len) {
      for (int i = 0; i <= n - len; ++i) {
        int j = i + len - 1;

        // if s[i] == s[j] then check for
        //  i [i+1  --- j-1] j
        if (s[i] == s[j] && dp[i + 1][j - 1]) {
          dp[i][j] = true;
          if (len > maxLen) {
            start = i;
            maxLen = len;
          }
        }
      }
    }
    return s.substr(start, maxLen);
  }
};

bool isPalindrome(const std::string& s) {
  for (size_t i = 0, j = s.size(); i + 1 < j; i++, j--) {
    if (s[i] != s[j - 1]) {
      return false;
    }
  }

  return true;
}

// Long inputs are built in bulk below, so print a shape rather than a wall of
// characters.
std::string preview(const std::string& s) {
  if (s.size() <= 32) {
    return "\"" + s + "\"";
  }

  return "\"" + s.substr(0, 14) + "..." + s.substr(s.size() - 14) + "\" (" +
         std::to_string(s.size()) + " chars)";
}

// "babad" may answer either "bab" or "aba", so pinning one string would fail a
// correct solution that happens to find the other. What is actually specified
// is that the answer be a palindrome, be a substring of s, and be as long as
// the longest one available — so check those three things instead, with only
// the length given per case.
void check(const std::string& s, size_t expectedLen) {
  Solution sol;
  std::string got = sol.longestPalindrome(s);

  bool palindromic = isPalindrome(got);
  bool contained = s.find(got) != std::string::npos;
  bool longest = got.size() == expectedLen;

  std::cout << (palindromic && contained && longest ? "PASS" : "FAIL")
            << "  s=" << preview(s) << "\n  expected a palindrome of length "
            << expectedLen << "\n  got " << preview(got) << " of length "
            << got.size();

  if (!palindromic) {
    std::cout << "\n  (not a palindrome)";
  }
  if (!contained) {
    std::cout << "\n  (not a substring of s)";
  }
  if (longest == false && palindromic && contained) {
    std::cout << (got.size() < expectedLen ? "\n  (a longer one exists)"
                                           : "\n  (longer than anything in s)");
  }

  std::cout << "\n" << std::string(60, '-') << "\n";
}

int main() {
  // --- the LeetCode examples ---
  check("babad", 3);  // "bab" and "aba" are both correct
  check("cbbd", 2);

  // --- shortest inputs ---
  check("a", 1);
  check("ab", 1);
  check("aa", 2);

  // --- the whole string is the answer ---
  check("racecar", 7);  // odd length, single centre character
  check("abba", 4);     // even length, centre falls between characters
  check("aaaa", 4);
  check("12321", 5);  // digits count as characters like any other

  // --- nothing repeats, so every character stands alone ---
  check("abcdefg", 1);

  // --- where the answer sits in the string ---
  check("abacdfg", 3);    // at the start
  check("gfdcaba", 3);    // at the end
  check("xyzabaxyz", 3);  // in the middle

  // --- an even-length answer beating an odd-length one, and the reverse ---
  // "aba" is found first and is odd; "baab" is longer and even, so a solution
  // that only expands around single characters stops at 3 here.
  check("abaab", 4);
  // The mirror image: "bb" comes first, "aba" is longer.
  check("bbaba", 3);

  // --- candidates that overlap and share characters ---
  check("aacabdkacaa", 3);  // "aca", not either of the "aa" pairs
  check("abacabad", 7);     // "abacaba", built from shorter palindromes

  // --- case matters: 'A' and 'a' are different characters ---
  check("Aa", 1);
  check("AbA", 3);
  // Reads as a palindrome only if case is folded away: 'b' != 'B' and
  // 'a' != 'A', so no pair here matches and the answer is a single character.
  check("abBA", 1);

  // --- the largest input allowed: 1000 characters ---
  const size_t limit = 1000;

  check(std::string(limit, 'a'), limit);  // every character identical

  // One odd character out at the very end, so the answer is everything before
  // it — a solution that stops early keeps the full-length wrong answer.
  check(std::string(limit - 1, 'a') + "b", limit - 1);
  check("b" + std::string(limit - 1, 'a'), limit - 1);  // and at the front

  // Two runs meeting in the middle: the answer is either half, not the whole.
  check(std::string(limit / 2, 'a') + std::string(limit / 2, 'b'), limit / 2);

  // "ababab...ab": dropping either end character leaves a palindrome, so the
  // answer is one short of the whole string.
  std::string alternating;
  while (alternating.size() < limit) {
    alternating += "ab";
  }
  check(alternating, limit - 1);

  // 1000 characters with no repeat adjacent or otherwise beyond single
  // matches: a long input whose answer is still 1.
  std::string cycling;
  for (size_t i = 0; cycling.size() < limit; i++) {
    cycling += static_cast<char>('a' + (i % 26));
  }
  check(cycling, 1);

  // --- beyond the stated constraints, which promise at least one character ---
  check("", 0);

  return 0;
}
