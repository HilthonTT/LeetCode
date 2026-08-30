// 20. Valid Parentheses (Easy)
// https://leetcode.com/problems/valid-parentheses/

#include <iostream>
#include <stack>
#include <string>

class Solution {
 public:
  bool isValid(const std::string& s) {
    // Odd-length input can never be balance
    if (s.size() % 2) {
      return false;
    }

    std::stack<char> st;

    for (char c : s) {
      switch (c) {
        case '(':
        case '{':
        case '[': {
          st.push(c);
          break;
        }
        case ')':
        case '}':
        case ']': {
          // No opening bracket
          if (st.empty()) {
            return false;
          }

          char top = st.top();
          if ((c == ')' && top != '(') || (c == '}' && top != '{') ||
              (c == ']' && top != '[')) {
            return false;
          }

          // Pop the matching opening bracket
          st.pop();

          break;
        }
        default:
          break;
      }
    }

    // Balanced if stack is empty
    return st.empty();
  }
};

void check(const std::string& s, bool expected) {
  Solution sol;
  bool got = sol.isValid(s);

  std::cout << std::boolalpha << (got == expected ? "PASS" : "FAIL") << "  s=\""
            << s << "\""
            << "  expected=" << expected << "  got=" << got << "\n";
}

int main() {
  // empty
  check("", true);

  // each pair alone
  check("()", true);
  check("{}", true);
  check("[]", true);

  // sequential
  check("()[]{}", true);
  check("[()()]{}", true);

  // nested
  check("([{}])", true);
  check("{[()]}", true);
  check("((((()))))", true);

  // wrong type closes
  check("(]", false);
  check("([)]", false);  // interleaved, the classic trap
  check("{[}]", false);

  // unclosed openers -> stack non-empty at the end
  check("(", false);
  check("(((", false);
  check("([{}]", false);

  // unmatched closers -> stack empty on arrival
  check(")", false);
  check(")))", false);
  check("())", false);

  // closer first, then a valid pair
  check(")(", false);

  // odd length can never balance
  check("(()", false);

  // longer mixed cases
  check("{[()()]}[]{()}", true);
  check("{[()()]}[]{(}", false);

  return 0;
}