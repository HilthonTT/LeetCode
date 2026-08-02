#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <climits>
#include <cmath>

class Solution
{
public:
    double findMedianSortedArrays(std::vector<int> &nums1, std::vector<int> &nums2)
    {
        int n = nums1.size(), m = nums2.size();
        int i = 0, j = 0;

        // m1 to store the middle element
        // m2 to store the second middle element
        int m1 = -1, m2 = -1;

        // loop till (m+n)/2
        for (int count = 0; count <= (m + n) / 2; count++)
        {
            m2 = m1;

            // if both the arrays have remaining elements
            if (i != n && j != m)
            {
                m1 = (nums1[i] > nums2[j]) ? nums2[j++] : nums1[i++];
            }

            // if only a[] has remaining elements
            else if (i < n)
            {
                m1 = nums1[i++];
            }

            // if only b[] has remaining elements
            else
            {
                m1 = nums2[j++];
            }
        }

        // return median based on odd/even size
        if ((m + n) % 2 == 1)
        {
            return m1;
        }
        else
        {
            // Halve each side before adding: m1 + m2 would be summed as int
            // first, which overflows once both medians sit near a bound of
            // the int range. Halves of an int are exact in a double, so the
            // result is unchanged for every other input.
            return (m1 / 2.0) + (m2 / 2.0);
        }
    }
};

std::string toString(const std::vector<int> &nums)
{
    std::string out = "[";
    for (size_t i = 0; i < nums.size(); i++)
    {
        if (i > 0)
        {
            out += ",";
        }
        out += std::to_string(nums[i]);
    }
    return out + "]";
}

// The median is always either a whole number or a .5, so an exact
// comparison would do, but a tolerance keeps the intent obvious.
void check(std::vector<int> nums1, std::vector<int> nums2, double expected)
{
    Solution sol;
    double got = sol.findMedianSortedArrays(nums1, nums2);

    std::cout << (std::abs(got - expected) < 1e-9 ? "PASS" : "FAIL")
              << "  nums1=" << toString(nums1)
              << "  nums2=" << toString(nums2)
              << "  expected=" << expected
              << "  got=" << got
              << "\n";
}

int main()
{
    // the two LeetCode examples
    check({1, 3}, {2}, 2.0);
    check({1, 2}, {3, 4}, 2.5);

    // --- one array empty: the median comes from the other one alone ---
    check({}, {1}, 1.0);
    check({1}, {}, 1.0);
    check({}, {1, 2}, 1.5);
    check({1, 2, 3}, {}, 2.0);
    check({}, {1, 2, 3, 4}, 2.5);

    // --- single elements on both sides ---
    check({1}, {2}, 1.5);
    check({2}, {1}, 1.5);
    check({1}, {1}, 1.0);

    // --- disjoint ranges, so the merge drains one array before the other ---
    check({1, 2, 3}, {4, 5, 6}, 3.5);
    check({4, 5, 6}, {1, 2, 3}, 3.5);
    check({1, 2}, {3, 4, 5}, 3.0);
    check({3, 4, 5}, {1, 2}, 3.0);

    // --- interleaved values ---
    check({1, 3, 5}, {2, 4, 6}, 3.5);
    check({1, 4, 7}, {2, 3, 8, 9}, 4.0);
    check({1, 5}, {2, 3, 4, 6}, 3.5);

    // --- very unbalanced sizes, median sits inside the long array ---
    check({1}, {2, 3, 4, 5, 6, 7, 8, 9}, 5.0);
    check({100}, {1, 2, 3, 4, 5}, 3.5);
    check({1, 2, 3, 4, 5}, {100}, 3.5);

    // --- duplicates, including a value repeated across both arrays ---
    check({1, 1, 1}, {1, 1, 1}, 1.0);
    check({2, 2}, {2, 2, 2}, 2.0);
    check({1, 2, 2}, {2, 2, 3}, 2.0);

    // --- negatives and a mixed-sign span ---
    check({-5, -3, -1}, {-4, -2}, -3.0);
    check({-3, -1}, {1, 3}, 0.0);
    check({-2, -1, 0}, {1, 2, 3}, 0.5);

    // --- the tail of the int range, where the even-size branch can overflow ---
    check({INT_MAX}, {INT_MAX}, 2147483647.0);
    check({INT_MIN}, {INT_MIN}, -2147483648.0);
    check({INT_MIN}, {INT_MAX}, -0.5);

    return 0;
}
