// 43. Multiply Strings (Medium)
// https://leetcode.com/problems/multiply-strings/

#include <string>
#include <vector>
#include <iostream>

class Solution
{
public:
    // Given two non-negative integers num1 and num2 represented as strings,
    // return their product as a string, without converting the inputs to an
    // integer type directly.
    std::string multiply(std::string num1, std::string num2)
    {
        if (num1 == "0" || num2 == "0")
        {
            return "0";
        }

        int len1 = num1.size();
        int len2 = num2.size();

        // Initialize result array with size len1 + len2 (maximum possible digits in product)
        std::vector<int> result(len1 + len2, 0);

        // Multiply each digit of num1 with each digit of num2
        // Starting from the rightmost digit (least significant)
        for (int i = len1 - 1; i >= 0; --i)
        {
            int digit1 = num1[i] - '0'; // Convert char to int

            for (int j = len2 - 1; j >= 0; --j)
            {
                int digit2 = num2[j] - '0'; // Convert char to int

                // Multiply digits and add to corresponding position
                // Position i + j + 1 corresponds to the result position
                result[i + j + 1] += digit1 * digit2;
            }
        }

        // Handle carries from right to left
        for (int i = result.size() - 1; i > 0; --i)
        {
            result[i - 1] += result[i] / 10; // Add carry to previous position
            result[i] %= 10;                 // Keep only the last digit
        }

        // Skip leading zeros (if any)
        int startIndex = result[0] == 0 ? 1 : 0;

        // Build the final result string
        std::string answer;
        for (int i = startIndex; i < static_cast<int>(result.size()); ++i)
        {
            answer += '0' + result[i]; // Convert int to char and append
        }

        return answer;
    }
};

// A four hundred digit product is unreadable in the output, so a check may name
// its inputs with a description instead of printing them.
void check(const std::string &num1, const std::string &num2, const std::string &expected,
           const std::string &description = "")
{
    Solution sol;
    std::string got = sol.multiply(num1, num2);

    bool passed = got == expected;

    std::cout << (passed ? "PASS" : "FAIL");

    if (description.empty())
    {
        std::cout << "  " << num1 << " * " << num2 << " = " << expected;
    }
    else
    {
        std::cout << "  (" << description << ")";
    }

    // A mismatch is the only case worth spending hundreds of digits of output on.
    if (!passed)
    {
        std::cout << "  expected=" << expected << "  got=" << got;
    }

    std::cout << "\n";
}

// Multiplication is commutative, but the two arguments run through different
// loops in the solution, so a pair is worth checking in both orders.
void checkBothWays(const std::string &num1, const std::string &num2, const std::string &expected,
                   const std::string &description = "")
{
    check(num1, num2, expected, description);
    check(num2, num1, expected, description.empty() ? "" : description + ", arguments swapped");
}

// The string of n nines, which is 10^n - 1.
std::string nines(int n)
{
    return std::string(n, '9');
}

// A 1 followed by n zeros, which is 10^n.
std::string powerOfTen(int n)
{
    return "1" + std::string(n, '0');
}

int main()
{
    // the two LeetCode examples
    check("2", "3", "6");
    check("123", "456", "56088");

    // --- zero, the only input allowed to start with a zero digit ---
    check("0", "0", "0");
    check("0", "52", "0");
    check("52", "0", "0");
    check("0", nines(200), "0", "zero against the longest allowed operand");

    // --- one, which has to leave the other operand untouched ---
    checkBothWays("1", "1", "1");
    checkBothWays("1", "999", "999");
    checkBothWays("1", "123456789", "123456789");

    // --- single digits, the shortest inputs the constraints allow ---
    check("3", "3", "9", "a single-digit product that does not carry");
    check("9", "9", "81", "the largest single-digit product, which carries into two digits");

    // --- products one digit shorter than len1 + len2, so a leading zero is skipped ---
    check("11", "11", "121");
    check("12", "12", "144");
    check("31", "31", "961", "three digits, just under the four-digit boundary");

    // --- products exactly len1 + len2 digits, so nothing is skipped ---
    check("99", "99", "9801");
    check("32", "32", "1024", "the first two-by-two product that needs all four digits");

    // --- long carry chains, where a column exceeds 10 many times over ---
    check("999", "999", "998001");
    check("99999", "99999", "9999800001");
    check("9999999999", "9999999999", "99999999980000000001");

    // --- operands of very different lengths ---
    checkBothWays("2", "12345", "24690");
    checkBothWays("9", nines(50), "8" + nines(49) + "1", "one digit against fifty nines");
    checkBothWays("999", powerOfTen(21), "999" + std::string(21, '0'),
                  "three digits against a twenty-two digit power of ten");

    // --- zeros inside and at the end of an operand ---
    check("100", "100", "10000");
    check("1001", "1001", "1002001");
    check("505", "505", "255025");
    checkBothWays("1020", "3040", "3100800", "interior and trailing zeros in both operands");

    // --- products past what a 64-bit integer could hold ---
    check("123456789", "987654321", "121932631112635269", "the largest of these that still fits in int64");
    check("999999999999999999", "999999999999999999", "999999999999999998000000000000000001",
          "thirty-six digits, past the int64 ceiling");
    check("12345678901234567890", "98765432109876543210", "1219326311370217952237463801111263526900",
          "forty digits");

    // --- the longest operands the constraints allow: 200 digits each ---

    // (10^200 - 1)^2 = 10^400 - 2 * 10^200 + 1, which written out is
    // 199 nines, then an 8, then 199 zeros, then a 1.
    check(nines(200), nines(200), nines(199) + "8" + std::string(199, '0') + "1",
          "200 nines squared, the heaviest carrying the constraints allow");

    // 10^199 * 10^199 = 10^398: a 1 followed by 398 zeros.
    check(powerOfTen(199), powerOfTen(199), powerOfTen(398),
          "two 200-digit powers of ten, the sparsest 200-by-200 product");

    // A 200-digit operand times 1 is the operand itself, so the leading slot of
    // the 201-digit buffer holds the zero that has to be skipped.
    checkBothWays("1", nines(200), nines(200), "200 nines times one");

    return 0;
}
