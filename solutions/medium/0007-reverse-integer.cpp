// 7. Reverse Integer (Medium)
// https://leetcode.com/problems/reverse-integer/

#include <climits>
#include <iostream>
#include <string>

class Solution
{
public:
    int reverse(int x)
    {
        int reversedNumber = 0;

        // Process each digit of the input number
        while (x != 0)
        {
            // Check for potential overflow before multiplying by 10
            // INT_MAX = 2147483647, INT_MIN = -2147483648
            // If reversedNumber > INT_MAX/10, then reversedNumber * 10 will overflow
            // If reversedNumber < INT_MIN/10, then reversedNumber * 10 will underflow
            if (reversedNumber > INT_MAX / 10 || reversedNumber < INT_MIN / 10)
            {
                return 0;
            }

            // Extract the last digit and append it to the reversed number
            int lastDigit = x % 10;
            reversedNumber = reversedNumber * 10 + lastDigit;

            // Remove the last digit from the original number
            x /= 10;
        }

        return reversedNumber;
    }
};

void check(int x, int expected)
{
    Solution sol;
    int got = sol.reverse(x);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  x=" << x
              << "\n  expected " << expected
              << "\n  got      " << got
              << "\n"
              << std::string(60, '-') << "\n";
}

// Reversing a number twice gives it back, as long as neither step overflows and
// the number has no trailing zeros to lose. Useful for values whose reversal is
// tedious to write out by hand.
void checkRoundTrip(int x)
{
    Solution sol;
    int once = sol.reverse(x);
    int twice = sol.reverse(once);

    std::cout << (twice == x && once != 0 ? "PASS" : "FAIL")
              << "  x=" << x << " (round trip)"
              << "\n  reversed " << once
              << "\n  back to  " << twice
              << "\n"
              << std::string(60, '-') << "\n";
}

int main()
{
    // --- the examples from the problem statement ---
    check(123, 321);
    check(-123, -321);
    check(120, 21);

    // --- zero, and the shortest inputs there are ---
    check(0, 0);
    check(1, 1);
    check(-1, -1);
    check(9, 9);
    check(-9, -9);

    // --- trailing zeros disappear, they cannot become leading ones ---
    check(100, 1);
    check(-100, -1);
    check(1000000000, 1);
    check(-1000000000, -1);
    check(10, 1);
    check(-10, -1);
    check(1200, 21);

    // --- zeros in the middle are kept ---
    check(1002, 2001);
    check(-1002, -2001);
    check(101, 101);

    // --- palindromes come back unchanged ---
    check(1221, 1221);
    check(-1221, -1221);
    check(1234554321, 1234554321);

    // --- ordinary multi-digit values ---
    check(1534, 4351);
    check(-1534, -4351);
    check(987654321, 123456789);
    check(-987654321, -123456789);

    // --- the largest and smallest values that still fit after reversing.
    //     2147483647 is INT_MAX, so anything above 2147483641 overflows ---
    check(1463847412, 2147483641);
    check(-1463847412, -2147483641);
    check(2147483641, 1463847412);
    check(-2147483641, -1463847412);

    // --- one past that boundary in each direction: the reversal no longer
    //     fits in an int, so the answer is 0 ---
    check(1563847412, 0);  // would be 2147483651
    check(-1563847412, 0); // would be -2147483651
    check(1534236469, 0);  // would be 9646324351
    check(-1534236469, 0);

    // --- the constraint bounds themselves, both of which overflow ---
    check(INT_MAX, 0); //  2147483647 reversed is 7463847412
    check(INT_MIN, 0); // -2147483648 reversed is -8463847412

    // --- INT_MIN has no positive counterpart, so a solution that negates the
    //     input before reversing it breaks here rather than returning 0 ---
    check(-2147483641, -1463847412);

    // --- every ten-digit value starting with 9 overflows; every one starting
    //     with 1 does not, because the reversal ends in 1 ---
    check(999999999, 999999999); // nine digits, the largest that always fits
    check(1999999999, 0);        // would be 9999999991
    check(1111111111, 1111111111);

    // --- reversing twice returns the original ---
    checkRoundTrip(123);
    checkRoundTrip(-123);
    checkRoundTrip(7);
    checkRoundTrip(1463847412);
    checkRoundTrip(-1463847412);

    for (int x = 1; x <= 999999999; x *= 3)
    {
        checkRoundTrip(x);
        checkRoundTrip(-x);
    }

    return 0;
}
