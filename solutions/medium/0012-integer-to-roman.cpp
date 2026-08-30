// 12. Integer to Roman (Medium)
// https://leetcode.com/problems/integer-to-roman/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  std::string intToRoman(int num) {
    std::vector<int> bases = {1000, 900, 500, 400, 100, 90, 50,
                              40,   10,  9,   5,   4,   1};
    std::vector<std::string> symbols = {"M",  "CM", "D",  "CD", "C",  "XC", "L",
                                        "XL", "X",  "IX", "V",  "IV", "I"};

    std::string result = "";

    for (int i = 0; i < bases.size(); i++) {
      while (num >= bases[i]) {
        result += symbols[i];
        num -= bases[i];
      }
    }

    return result;
  }
};

// The same numerals, built from the digits rather than by subtracting bases:
// one table per decimal place, indexed by the digit sitting there. It never
// loops and never touches the input, so it arrives at the answer a different
// way from the solution above — which is what makes it worth comparing against.
std::string reference(int num) {
  const std::string thousands[] = {"", "M", "MM", "MMM"};
  const std::string hundreds[] = {"",  "C",  "CC",  "CCC",  "CD",
                                  "D", "DC", "DCC", "DCCC", "CM"};
  const std::string tens[] = {"",  "X",  "XX",  "XXX",  "XL",
                              "L", "LX", "LXX", "LXXX", "XC"};
  const std::string ones[] = {"",  "I",  "II",  "III",  "IV",
                              "V", "VI", "VII", "VIII", "IX"};

  return thousands[num / 1000] + hundreds[(num / 100) % 10] +
         tens[(num / 10) % 10] + ones[num % 10];
}

// Reading a numeral back: a symbol worth less than the one after it is being
// subtracted, everything else adds. Enough to confirm a numeral names the
// number it was built from, though not that it is the numeral a Roman would
// have used — "IIII" and "IV" both read as 4, which is why the shape is checked
// separately.
int romanToInt(const std::string& s) {
  auto value = [](char c) {
    switch (c) {
      case 'I':
        return 1;
      case 'V':
        return 5;
      case 'X':
        return 10;
      case 'L':
        return 50;
      case 'C':
        return 100;
      case 'D':
        return 500;
      case 'M':
        return 1000;
    }
    return 0;
  };

  int total = 0;

  for (size_t i = 0; i < s.size(); i++) {
    if (i + 1 < s.size() && value(s[i]) < value(s[i + 1])) {
      total -= value(s[i]);
    } else {
      total += value(s[i]);
    }
  }

  return total;
}

// The rules the problem states about how a numeral may be spelled: only the
// seven symbols, I, X, C and M repeated at most three times, and V, L and D not
// repeated at all. A numeral that reads back to the right number can still
// break these — "VIIII" is 9 — so both checks are needed to pin the answer down
// to one string.
bool isWellFormed(const std::string& s) {
  if (s.find_first_not_of("IVXLCDM") != std::string::npos) {
    return false;
  }

  for (char once : {'V', 'L', 'D'}) {
    if (std::count(s.begin(), s.end(), once) > 1) {
      return false;
    }
  }

  for (size_t i = 3; i < s.size(); i++) {
    if (s[i] == s[i - 1] && s[i] == s[i - 2] && s[i] == s[i - 3]) {
      return false;
    }
  }

  return true;
}

void check(int num, const std::string& expected) {
  Solution sol;
  std::string got = sol.intToRoman(num);

  std::cout << (got == expected ? "PASS" : "FAIL") << "  num=" << num
            << "\n  expected " << expected << "\n  got      " << got << "\n"
            << std::string(60, '-') << "\n";
}

// The constraints only allow 1 to 3999, which is few enough to check in full:
// the three properties below together leave exactly one valid numeral per
// number. One line per value would bury the cases above in sixteen thousand
// lines of output, so the sweep reports each failure in the usual detail and
// everything else as a single line.
void sweep(int low, int high) {
  Solution sol;
  int failures = 0;

  for (int num = low; num <= high; num++) {
    std::string got = sol.intToRoman(num);
    std::string expected = reference(num);
    int readBack = romanToInt(got);
    bool wellFormed = isWellFormed(got);

    if (got == expected && readBack == num && wellFormed) {
      continue;
    }

    failures++;

    std::cout << "FAIL"
              << "  num=" << num << "\n  expected  " << expected
              << "\n  got       " << got << "\n  reads as  " << readBack
              << "\n  spelling  "
              << (wellFormed ? "ok" : "breaks the repetition rules") << "\n"
              << std::string(60, '-') << "\n";
  }

  std::cout << (failures == 0 ? "PASS" : "FAIL") << "  num=" << low << ".."
            << high << " (every value in range)"
            << "\n  expected " << (high - low + 1)
            << " numerals matching the reference, reading back, well formed"
            << "\n  got      " << (high - low + 1 - failures) << " of them"
            << "\n"
            << std::string(60, '-') << "\n";
}

int main() {
  // --- the examples from the problem statement ---
  check(3749, "MMMDCCXLIX");
  check(58, "LVIII");
  check(1994, "MCMXCIV");

  // --- the ends of the allowed range ---
  check(1, "I");
  check(2, "II");
  check(3998, "MMMCMXCVIII");
  check(3999, "MMMCMXCIX");

  // --- each of the thirteen values that has a symbol of its own ---
  check(1, "I");
  check(4, "IV");
  check(5, "V");
  check(9, "IX");
  check(10, "X");
  check(40, "XL");
  check(50, "L");
  check(90, "XC");
  check(100, "C");
  check(400, "CD");
  check(500, "D");
  check(900, "CM");
  check(1000, "M");

  // --- the three repeats a symbol is allowed, in every place ---
  check(3, "III");
  check(30, "XXX");
  check(300, "CCC");
  check(3000, "MMM");

  // --- either side of every subtractive pair, where a solution that gets the
  //     order of the bases wrong stops agreeing ---
  check(6, "VI");
  check(8, "VIII");
  check(11, "XI");
  check(39, "XXXIX");
  check(41, "XLI");
  check(49, "XLIX");
  check(89, "LXXXIX");
  check(91, "XCI");
  check(99, "XCIX");
  check(101, "CI");
  check(399, "CCCXCIX");
  check(401, "CDI");
  check(499, "CDXCIX");
  check(501, "DI");
  check(899, "DCCCXCIX");
  check(901, "CMI");
  check(999, "CMXCIX");
  check(1001, "MI");

  // --- a zero in one place contributes nothing, it does not shift the rest ---
  check(1005, "MV");
  check(1010, "MX");
  check(1100, "MC");
  check(2000, "MM");
  check(2001, "MMI");
  check(3009, "MMMIX");

  // --- 3888 needs fifteen symbols, the most any number in range does ---
  check(888, "DCCCLXXXVIII");
  check(2888, "MMDCCCLXXXVIII");
  check(3888, "MMMDCCCLXXXVIII");

  // --- ordinary values, the ones a year is usually written in ---
  check(1066, "MLXVI");
  check(1453, "MCDLIII");
  check(1776, "MDCCLXXVI");
  check(1900, "MCM");
  check(1987, "MCMLXXXVII");
  check(1990, "MCMXC");
  check(2024, "MMXXIV");
  check(2026, "MMXXVI");

  // --- and then the whole range, against the digit-table reference ---
  sweep(1, 3999);

  return 0;
}
