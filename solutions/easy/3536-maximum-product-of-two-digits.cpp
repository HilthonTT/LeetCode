// 3536. Maximum Product of Two Digits (Easy)
// https://leetcode.com/problems/maximum-product-of-two-digits/

#include <iostream>
#include <print>

class Solution
{
public:
    int maxProduct(int n)
    {
        int best = 0;
        int secondBest = 0;

        while (n > 0)
        {
            int d = n % 10;

            if (d > best)
            {
                secondBest = best;
                best = d;
            }
            else if (d > secondBest)
            {
                secondBest = d;
            }

            n /= 10;
        }

        return best * secondBest;
    }
};

int main()
{
    Solution sol;

    int product = sol.maxProduct(267);
    std::cout << "Expected output: 42, got: " << product << "\n";

    product = sol.maxProduct(31);
    std::cout << "Expected output: 3, got: " << product << "\n";

    product = sol.maxProduct(22);
    std::cout << "Expected output: 4, got: " << product << "\n";

    return 0;
}