// 69. Sqrt(x) (Easy)
// https://leetcode.com/problems/sqrtx/

#include <string>
#include <iostream>

class Solution
{
public:
    int mySqrt(int n)
    {
        if (n == 0)
        {
            return 0;
        }

        // Initialize vars for binary search
        int low = 1;
        int high = n;
        int result = 1;

        while (low <= high)
        {
            int mid = low + (high - low) / 2;

            // if square of the mid is less than or equal to n
            // update the result and search in upper half
            if (mid <= n / mid)
            {
                result = mid;
                low = mid + 1;
            }
            else
            {
                // if the square of mid exceeds n,
                // search in the lower half
                high = mid - 1;
            }
        }

        return result;
    }
};

void check(int n, int expected)
{
    Solution sol;
    int got = sol.mySqrt(n);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  n=" << n
              << "  expected=" << expected
              << "  got=" << got
              << "\n"
              << std::string(60, '-') << "\n";
}

int main()
{
    // --- edge cases at the bottom of the range ---
    check(0, 0);
    check(1, 1);
    check(2, 1); // first n where floor(sqrt) < n
    check(3, 1);

    // --- perfect squares and the values just around them ---
    check(4, 2);
    check(8, 2);
    check(9, 3);
    check(15, 3);
    check(16, 4);
    check(24, 4);
    check(25, 5);

    // --- a mid-size value ---
    check(100, 10);
    check(101, 10);
    check(9999, 99); // 99^2 = 9801, 100^2 = 10000

    // --- the overflow-sensitive tail of the int range ---
    // 46340^2 = 2147395600, 46341^2 = 2147488281 > INT_MAX
    check(2147395599, 46339);
    check(2147395600, 46340);
    check(2147395601, 46340);
    check(INT_MAX, 46340); // 2147483647

    return 0;
}
