// 136. Single Number (Easy)
// https://leetcode.com/problems/single-number/

#include <vector>
#include <string>
#include <iostream>

class Solution
{
public:
    // Given a non-empty array of integers where every element appears twice
    // except for one, return that single one. The solution must run in linear
    // time and use only constant extra space.
    int singleNumber(std::vector<int> &nums)
    {
        // XOR is its own inverse (x ^ x == 0) and it is both commutative and
        // associative, so the order the array is folded in does not matter.
        // Every value that appears twice cancels itself out to 0, and 0 ^ x == x
        // leaves those cancellations invisible, so what survives the fold is
        // exactly the value that appeared once.
        int result = 0;

        for (int x : nums)
        {
            result ^= x;
        }

        // A single accumulator and one pass: constant space, linear time.
        return result;
    }
};

void check(std::vector<int> nums, int expected, const std::string &description = "")
{
    Solution sol;
    // singleNumber takes a non-const reference, so it needs the local copy above.
    int got = sol.singleNumber(nums);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  expected=" << expected
              << "  got=" << got;

    if (!description.empty())
    {
        std::cout << "  (" << description << ")";
    }

    std::cout << "\n";
}

int main()
{
    // the three LeetCode examples
    check({2, 2, 1}, 1);
    check({4, 1, 2, 1, 2}, 4);
    check({1}, 1);

    // --- the smallest input the constraints allow ---
    check({0}, 0, "the lone element is 0, the same value the fold starts from");

    // --- the single element in each position ---
    check({7, 3, 3, 5, 5}, 7, "single element first");
    check({3, 3, 7, 5, 5}, 7, "single element in the middle");
    check({3, 3, 5, 5, 7}, 7, "single element last");

    // --- the pairs are not required to be adjacent or sorted ---
    check({4, 1, 2, 9, 1, 4, 2}, 9, "pairs interleaved");
    check({5, 9, 5, 1, 9, 1, 8}, 8, "pairs in scrambled order");

    // --- negatives, which XOR handles through their two's complement bits ---
    check({-1, -1, -3}, -3, "all negative");
    check({-2, 4, -2}, 4, "a negative pair around a positive single");
    check({6, -7, 6}, -7, "a positive pair around a negative single");
    check({-5, 3, 3, -5, -9}, -9, "mixed signs");

    // --- a 0 that is itself paired, so it must not be mistaken for the answer ---
    check({0, 0, 12}, 12, "the pair is 0 and the single is not");
    check({0, 0, -12}, -12, "a paired 0 with a negative single");

    // --- values that share bits, so cancellation has to be exact ---
    check({1, 2, 3, 1, 2}, 3, "1 ^ 2 == 3, so a sloppy fold could return 3 for the wrong reason");
    check({1, 2, 1, 2, 3}, 3, "same values, order that rules out the coincidence above");

    // --- the ends of the value range the constraints allow ---
    check({30000, -30000, 30000}, -30000, "the minimum value as the single");
    check({-30000, 30000, -30000}, 30000, "the maximum value as the single");

    // --- the largest input the constraints allow: 29999 pairs plus one single ---
    {
        std::vector<int> nums;
        nums.reserve(30000 - 1);

        for (int i = 1; i < 15000; i++)
        {
            nums.push_back(i);
            nums.push_back(i);
        }

        nums.push_back(-30000);

        // 14999 pairs is 29998 elements, plus the single: 29999, one under the cap.
        check(nums, -30000, "29999 elements, the largest array the constraints allow");
    }

    return 0;
}
