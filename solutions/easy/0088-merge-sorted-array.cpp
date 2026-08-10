// 88. Merge Sorted Array (Easy)
// https://leetcode.com/problems/merge-sorted-array/

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

class Solution
{
public:
    void merge(std::vector<int> &nums1, int m, std::vector<int> &nums2, int n)
    {
        std::vector<int> merged(m + n);

        // copy m elements from nums1, not n
        for (int i = 0; i < m; i++)
        {
            merged[i] = nums1[i];
        }

        // copy n elements from nums2, placed after the m
        for (int i = 0; i < n; i++)
        {
            merged[m + i] = nums2[i];
        }

        std::sort(merged.begin(), merged.end());

        // everything goes back into nums1 — all m + n of them
        for (int i = 0; i < m + n; i++)
        {
            nums1[i] = merged[i];
        }
    }
};

std::string format(const std::vector<int> &values)
{
    std::string s = "[";
    for (size_t i = 0; i < values.size(); i++)
    {
        s += (i ? "," : "");
        s += std::to_string(values[i]);
    }

    return s + "]";
}

// nums1 arrives padded to m + n, with the trailing n slots zeroed, and the
// answer is left in nums1 rather than returned.
void check(const std::string &label, std::vector<int> nums1, int m,
           std::vector<int> nums2, int n, const std::vector<int> &expected)
{
    const std::vector<int> before1 = nums1;
    const std::vector<int> before2 = nums2;

    Solution sol;
    sol.merge(nums1, m, nums2, n);

    bool merged = nums1 == expected;

    // nums2 is an input, not scratch space: writing through it would pass the
    // comparison above while corrupting a caller's array.
    bool intact = nums2 == before2;

    std::cout << (merged && intact ? "PASS" : "FAIL")
              << "  " << label
              << "\n  nums1=" << format(before1) << " m=" << m
              << "  nums2=" << format(before2) << " n=" << n
              << "\n  expected=" << format(expected)
              << "\n  got     =" << format(nums1)
              << (intact ? "" : "\n  (nums2 was modified: " + format(nums2) + ")")
              << "\n"
              << std::string(60, '-') << "\n";
}

int main()
{
    // --- the LeetCode examples ---
    check("example 1", {1, 2, 3, 0, 0, 0}, 3, {2, 5, 6}, 3, {1, 2, 2, 3, 5, 6});
    check("example 2", {1}, 1, {}, 0, {1});
    check("example 3", {0}, 0, {1}, 1, {1});

    // --- one side empty ---
    // With m = 0 every element of nums1 is padding, so a merge that trusts the
    // padding instead of m would leave zeros in the answer.
    check("m = 0, several from nums2", {0, 0, 0}, 0, {1, 2, 3}, 3, {1, 2, 3});
    check("n = 0, nums1 already whole", {1, 2, 3}, 3, {}, 0, {1, 2, 3});

    // --- the two runs do not overlap ---
    check("all of nums2 sorts first", {4, 5, 6, 0, 0, 0}, 3, {1, 2, 3}, 3, {1, 2, 3, 4, 5, 6});
    check("all of nums2 sorts last", {1, 2, 3, 0, 0, 0}, 3, {4, 5, 6}, 3, {1, 2, 3, 4, 5, 6});

    // --- interleaved, and lopsided lengths ---
    check("strictly alternating", {1, 3, 5, 0, 0, 0}, 3, {2, 4, 6}, 3, {1, 2, 3, 4, 5, 6});
    check("one against many", {5, 0, 0, 0, 0}, 1, {1, 2, 3, 4}, 4, {1, 2, 3, 4, 5});
    check("many against one", {1, 2, 3, 4, 0}, 4, {5}, 1, {1, 2, 3, 4, 5});
    check("single element each", {2, 0}, 1, {1}, 1, {1, 2});

    // --- ties, which decide nothing about correctness but often trip indexing ---
    check("every value equal", {2, 2, 2, 0, 0, 0}, 3, {2, 2, 2}, 3, {2, 2, 2, 2, 2, 2});
    check("duplicates across both", {1, 2, 2, 0, 0, 0}, 3, {1, 2, 3}, 3, {1, 1, 2, 2, 2, 3});
    check("nums2 repeats nums1's head", {1, 1, 0, 0}, 2, {1, 1}, 2, {1, 1, 1, 1});

    // --- negatives, and a real zero that must not be mistaken for padding ---
    check("negative values", {-3, -1, 0, 0, 0}, 3, {-2, 0}, 2, {-3, -2, -1, 0, 0});
    check("zero is data, not padding", {0, 0, 0, 0}, 2, {0, 0}, 2, {0, 0, 0, 0});
    check("mixed signs", {-5, 3, 0, 0}, 2, {-2, 7}, 2, {-5, -2, 3, 7});

    // --- the edges of the constraints: |value| <= 10^9 ---
    check("extreme values", {-1000000000, 0, 0}, 1, {0, 1000000000}, 2,
          {-1000000000, 0, 1000000000});

    // --- the largest input allowed: m = n = 200 ---
    // The two runs interleave perfectly, so every comparison along the way has
    // to pick the other array.
    std::vector<int> big1(400, 0);
    std::vector<int> big2;
    std::vector<int> bigExpected;
    for (int i = 0; i < 200; i++)
    {
        big1[i] = 2 * i;       // 0, 2, 4, ...
        big2.push_back(2 * i + 1); // 1, 3, 5, ...
    }
    for (int i = 0; i < 400; i++)
    {
        bigExpected.push_back(i);
    }
    check("200 + 200, fully interleaved", big1, 200, big2, 200, bigExpected);

    return 0;
}
