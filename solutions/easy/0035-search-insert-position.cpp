// 35. Search Insert Position (Easy)
// https://leetcode.com/problems/search-insert-position/

#include <vector>
#include <string>
#include <iostream>

class Solution
{
public:
    int searchInsert(std::vector<int> &nums, int target)
    {
        int left = 0;
        int right = nums.size() - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            // if target is found at mid
            if (nums[mid] == target)
            {
                return mid;
            }
            // if target is smaller, search in left half
            else if (nums[mid] > target)
            {
                right = mid - 1;
            }
            // if target is larger, search in right half
            else
            {
                left = mid + 1;
            }
        }

        return left;
    }
};

void check(std::vector<int> nums, int target, int expected)
{
    Solution sol;
    int got = sol.searchInsert(nums, target);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  nums=[";
    for (size_t i = 0; i < nums.size(); ++i)
    {
        std::cout << (i ? "," : "") << nums[i];
    }
    std::cout << "]  target=" << target
              << "  expected=" << expected
              << "  got=" << got
              << "\n"
              << std::string(60, '-') << "\n";
}

int main()
{
    // --- the LeetCode examples ---
    check({1, 3, 5, 6}, 5, 2);
    check({1, 3, 5, 6}, 2, 1);
    check({1, 3, 5, 6}, 7, 4);

    // --- single element ---
    check({1}, 0, 0); // insert before
    check({1}, 1, 0); // found
    check({1}, 2, 1); // insert after

    // --- target outside the range on both ends ---
    check({1, 3, 5, 6}, 0, 0);   // below everything
    check({1, 3, 5, 6}, 100, 4); // above everything

    // --- every element found, even length (no exact middle) ---
    check({1, 3, 5, 6}, 1, 0);
    check({1, 3, 5, 6}, 3, 1);
    check({1, 3, 5, 6}, 6, 3);

    // --- every gap in an even-length array ---
    check({2, 4, 6, 8}, 1, 0);
    check({2, 4, 6, 8}, 3, 1);
    check({2, 4, 6, 8}, 5, 2);
    check({2, 4, 6, 8}, 7, 3);
    check({2, 4, 6, 8}, 9, 4);

    // --- odd length, so mid lands exactly on the middle element ---
    check({1, 3, 5, 6, 9}, 5, 2);
    check({1, 3, 5, 6, 9}, 4, 2);
    check({1, 3, 5, 6, 9}, 9, 4);
    check({1, 3, 5, 6, 9}, 10, 5);

    // --- two elements: smallest array where left/right can cross ---
    check({1, 3}, 0, 0);
    check({1, 3}, 1, 0);
    check({1, 3}, 2, 1);
    check({1, 3}, 3, 1);
    check({1, 3}, 4, 2);

    // --- negative values (LeetCode allows -10^4..10^4) ---
    check({-10, -5, 0, 5}, -7, 1);
    check({-10, -5, 0, 5}, -10, 0);
    check({-10, -5, 0, 5}, 0, 2);
    check({-10, -5, 0, 5}, -11, 0);

    // --- limits of the value range ---
    check({-10000, 0, 10000}, -10000, 0);
    check({-10000, 0, 10000}, 10000, 2);
    check({-10000, 0, 10000}, 10001, 3);

    // --- larger array, to exercise more binary-search steps ---
    std::vector<int> big;
    for (int i = 0; i < 1000; ++i)
    {
        big.push_back(i * 2); // 0, 2, 4, ... 1998
    }
    check(big, 0, 0);
    check(big, 1, 1);
    check(big, 1000, 500);
    check(big, 1001, 501);
    check(big, 1998, 999);
    check(big, 1999, 1000);

    return 0;
}
