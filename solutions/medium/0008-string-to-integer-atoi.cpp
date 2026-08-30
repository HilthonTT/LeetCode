// 8. String to Integer (atoi) (Medium)
// https://leetcode.com/problems/string-to-integer-atoi/

#include <climits>
#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  int myAtoi(std::string s) {
    int sign = 1;
    int res = 0;
    size_t idx = 0;

    // Ignore leading whitespace
    while (idx < s.size() && s[idx] == ' ') {
      idx++;
    }

    // Store the sign of number
    if (idx < s.size() && (s[idx] == '-' || s[idx] == '+')) {
      if (s[idx++] == '-') {
        sign = -1;
      }
    }

    // Construct the number digit by digit
    while (idx < s.size() && s[idx] >= '0' && s[idx] <= '9') {
      // handling overflow/underflow test case
      // INT_MAX = 2147483647 ends in 7, so a result already at
      // INT_MAX / 10 = 214748364 can still take a digit up to 7. An
      // eighth digit is only in range for a negative number, where it
      // makes exactly INT_MIN = -2147483648, which is what gets returned
      // anyway — so clamping here is correct for both signs.
      if (res > INT_MAX / 10 || (res == INT_MAX / 10 && s[idx] - '0' > 7)) {
        return sign == 1 ? INT_MAX : INT_MIN;
      }

      // Append current digit to the result
      res = 10 * res + (s[idx++] - '0');
    }

    return res * sign;
  }
};

// The same parse, but accumulating into a type wide enough to hold the value
// and clamping only at the end. Slower and reliant on 64-bit arithmetic the
// real solution avoids, which is what makes it useful for the generated cases
// below: it gets to the answer a different way.
int reference(const std::string& s) {
  size_t i = 0;

  while (i < s.size() && s[i] == ' ') {
    i++;
  }

  long long sign = 1;
  if (i < s.size() && (s[i] == '+' || s[i] == '-')) {
    if (s[i] == '-') {
      sign = -1;
    }
    i++;
  }

  long long value = 0;
  bool tooBig = false;

  while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
    if (!tooBig) {
      value = value * 10 + (s[i] - '0');

      // Past the magnitude of INT_MIN nothing can bring the value back
      // into range, so stop accumulating rather than let a long long
      // overflow of its own creep in.
      if (value > 2147483648LL) {
        tooBig = true;
      }
    }
    i++;
  }

  if (tooBig) {
    return sign == 1 ? INT_MAX : INT_MIN;
  }

  value *= sign;

  if (value > INT_MAX) {
    return INT_MAX;
  }
  if (value < INT_MIN) {
    return INT_MIN;
  }

  return static_cast<int>(value);
}

// Inputs run to 200 characters, and spaces at either end are the whole point of
// several cases, so quote them and shorten the long ones.
std::string preview(const std::string& s) {
  if (s.size() <= 32) {
    return "\"" + s + "\"";
  }

  return "\"" + s.substr(0, 14) + "..." + s.substr(s.size() - 14) + "\" (" +
         std::to_string(s.size()) + " chars)";
}

void check(const std::string& s, int expected) {
  Solution sol;
  int got = sol.myAtoi(s);

  std::cout << (got == expected ? "PASS" : "FAIL") << "  s=" << preview(s)
            << "\n  expected " << expected << "\n  got      " << got << "\n"
            << std::string(60, '-') << "\n";
}

// For the generated inputs, whose answers are not worth spelling out one by
// one.
void checkAgainstReference(const std::string& s) { check(s, reference(s)); }

int main() {
  // --- the examples from the problem statement ---
  check("42", 42);
  check(" -042", -42);
  check("1337c0d3", 1337);
  check("0-1", 0);
  check("words and 987", 0);

  // --- nothing to read: empty, all spaces, or no digit at all ---
  check("", 0);
  check(" ", 0);
  check("    ", 0);
  check("a", 0);
  check("  abc", 0);
  check("+-", 0);
  check(".", 0);

  // --- a sign with no digits behind it is not a number ---
  check("+", 0);
  check("-", 0);
  check("++", 0);
  check("--", 0);
  check("+-12", 0);
  check("-+12", 0);
  check("  ++1", 0);
  check("  --1", 0);

  // --- the sign has to touch the digits; a space after it ends the parse ---
  check("+ 413", 0);
  check("- 413", 0);
  check("  +  413", 0);

  // --- single digits, with and without a sign ---
  check("0", 0);
  check("-0", 0);
  check("+0", 0);
  check("5", 5);
  check("-5", -5);
  check("+5", 5);
  check("9", 9);
  check("  9  ", 9);  // trailing spaces are junk like anything else

  // --- leading zeros are not significant, however many there are ---
  check("0000", 0);
  check("-0000", 0);
  check("000123", 123);
  check("  -0012a42", -12);
  check("0000000000012345678", 12345678);
  check(std::string(199, '0') + "7", 7);
  check("  -00000000000000000000000042", -42);

  // --- the first non-digit stops the parse, whatever it is ---
  check("3.14", 3);
  check("-3.14", -3);
  check(".5", 0);
  check("-.5", 0);
  check("12abc34", 12);
  check("12 34", 12);
  check("9-9", 9);
  check("  -12+34", -12);
  check("0000000000000-42a1234", 0);
  check("  +0 123", 0);

  // --- exactly the bounds an int can hold ---
  check("2147483646", 2147483646);
  check("2147483647", INT_MAX);
  check("-2147483647", -2147483647);
  check("-2147483648", INT_MIN);

  // --- one step outside them in each direction, which clamps. INT_MAX ends
  //     in 7 and INT_MIN in 8, so this is the digit the overflow check has to
  //     treat differently depending on the sign ---
  check("2147483648", INT_MAX);
  check("-2147483649", INT_MIN);
  check("2147483650", INT_MAX);
  check("-2147483650", INT_MIN);

  // --- far outside them, including past what 64 bits hold ---
  check("21474836470", INT_MAX);
  check("-21474836480", INT_MIN);
  check("3000000000", INT_MAX);
  check("-3000000000", INT_MIN);
  check("91283472332", INT_MAX);
  check("-91283472332", INT_MIN);
  check("9223372036854775808", INT_MAX);   // one past LLONG_MAX
  check("-9223372036854775809", INT_MIN);  // one past LLONG_MIN
  check("20000000000000000000", INT_MAX);

  // --- clamping still happens with the digits dressed up in padding, and
  //     with junk after them that never gets read ---
  check("  0000002147483648  ", INT_MAX);
  check("  -0000002147483648", INT_MIN);
  check("+2147483648abc", INT_MAX);
  check("-2147483649.5", INT_MIN);

  // --- the longest input the constraints allow: 200 characters ---
  check(std::string(200, '1'), INT_MAX);
  check("-" + std::string(199, '9'), INT_MIN);
  check(std::string(200, ' '), 0);
  check(std::string(197, ' ') + "-42", -42);
  check(std::string(199, ' ') + "7", 7);

  // --- every value either side of the two bounds, checked against the
  //     wide-accumulator reference ---
  for (long long magnitude = 2147483640LL; magnitude <= 2147483655LL;
       magnitude++) {
    checkAgainstReference(std::to_string(magnitude));
    checkAgainstReference("-" + std::to_string(magnitude));
  }

  // --- the same handful of numbers through every combination of padding,
  //     sign and trailing junk ---
  const std::vector<std::string> digits = {
      "0", "7", "42", "2147483647", "2147483648", "91283472332"};
  const std::vector<std::string> spaces = {"", "  "};
  const std::vector<std::string> signs = {"", "+", "-"};
  const std::vector<std::string> junk = {"", "abc", ".5"};

  for (const std::string& space : spaces) {
    for (const std::string& sign : signs) {
      for (const std::string& number : digits) {
        for (const std::string& tail : junk) {
          checkAgainstReference(space + sign + number + tail);
          checkAgainstReference(space + sign + "000" + number + tail);
        }
      }
    }
  }

  // --- beyond the stated constraints, which allow only letters, digits,
  //     ' ', '+', '-' and '.': other whitespace is not skipped ---
  check("\t42", 0);
  check("\n42", 0);
  check("  \t 42", 0);

  return 0;
}
