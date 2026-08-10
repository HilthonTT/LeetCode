// 13. Roman to Integer (Easy)
// https://leetcode.com/problems/roman-to-integer/

#include <string>
#include <map>
#include <unordered_map>
#include <iostream>

class Solution
{
public:
    int romanToInt(std::string s)
    {
        int total = 0;

        for (size_t i = 0; i < s.length(); i++)
        {
            int current = value(s[i]);

            bool hasNext = i + 1 < s.length();
            if (hasNext && current < value(s[i + 1]))
            {
                total -= current;
            }
            else
            {
                total += current;
            }
        }

        return total;
    }

    static constexpr int value(char c)
    {
        switch (c)
        {
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
        default:
            return 0;
        }
    }
};

void check(const std::string &s, int expected)
{
    Solution sol;
    int got = sol.romanToInt(s);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  s=\"" << s << "\""
              << "  expected=" << expected
              << "  got=" << got << "\n";
}

int main()
{
    // every single symbol
    check("I", 1);
    check("V", 5);
    check("X", 10);
    check("L", 50);
    check("C", 100);
    check("D", 500);
    check("M", 1000);

    // pure addition
    check("III", 3);
    check("XXVII", 27);
    check("LVIII", 58);

    // all six legal subtractive pairs
    check("IV", 4);
    check("IX", 9);
    check("XL", 40);
    check("XC", 90);
    check("CD", 400);
    check("CM", 900);

    // subtraction mixed with addition
    check("XIV", 14);
    check("XXIX", 29);
    check("CDXLIV", 444);
    check("MCMXCIV", 1994);

    // boundaries of the constraint range (1..3999)
    check("I", 1);
    check("MMMCMXCIX", 3999);

    // longest valid input, 15 characters
    check("MMMDCCCLXXXVIII", 3888);

    // a couple of ordinary ones
    check("MMXXVI", 2026);
    check("MCMLXIX", 1969);

    return 0;
}
