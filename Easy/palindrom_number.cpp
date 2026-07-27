#include <iostream>

class Solution
{
public:
    bool isPalindrome(int n)
    {
        if (n < 0)
        {
            return false;
        }

        int t = n;
        long long reverse = 0;

        while (t > 0)
        {
            int dig = t % 10;
            reverse = reverse * 10 + dig;
            t /= 10;
        }

        return n == reverse;
    }
};

void check(int n, bool expected)
{
    Solution sol;
    bool got = sol.isPalindrome(n);

    std::cout << std::boolalpha
              << (got == expected ? "PASS" : "FAIL")
              << "  n=" << n
              << "  expected=" << expected
              << "  got=" << got << "\n";
}

int main()
{
    // single digits
    check(0, true);
    check(7, true);

    // basic palindromes
    check(121, true);
    check(1221, true);
    check(12321, true);

    // basic non-palindromes
    check(10, false);
    check(123, false);
    check(1232, false);

    // negatives are never palindromes (the '-' has no counterpart)
    check(-121, false);
    check(-1, false);

    // trailing zero: 100 reversed is 1, not 001
    check(100, false);

    // overflow territory
    check(1234567899, false); // reverses to 9987654321 > INT_MAX
    check(1463847412, false); // reverses to 2147483641, just under INT_MAX
    check(2147483647, false); // INT_MAX itself
    check(1000000001, true);  // large but safe palindrome
}
