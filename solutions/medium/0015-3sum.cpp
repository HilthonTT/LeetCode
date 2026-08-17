// 15. 3Sum (Medium)
// https://leetcode.com/problems/3sum/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Solution
{
public:
    std::vector<std::vector<int>> threeSum(std::vector<int> &nums)
    {
        // Sort the array to enable two-pointer technique
        std::sort(nums.begin(), nums.end());

        std::vector<std::vector<int>> result;
        int size = nums.size();

        // Iterate through the array as the first number
        // Stop at size-2 since we need at least 3 numbers
        // Also stop if current number > 0 (optimization: if smallest > 0, sum can't be 0)
        for (int first = 0; first < size - 2 && nums[first] <= 0; ++first)
        {
            // Skip duplicate values for the first number to avoid duplicate triplets
            if (first > 0 && nums[first] == nums[first - 1])
            {
                continue;
            }

            // Use two pointers for the remaining two numbers
            int left = first + 1;
            int right = size - 1;

            // Find all valid pairs for current first number
            while (left < right)
            {
                int currentSum = nums[first] + nums[left] + nums[right];

                if (currentSum < 0)
                {
                    // Sum too small, move left pointer to increase sum
                    ++left;
                }
                else if (currentSum > 0)
                {
                    // Sum too large, move right pointer to decrease sum
                    --right;
                }
                else
                {
                    // Found a valid triplet with sum = 0
                    result.push_back({nums[first], nums[left], nums[right]});

                    // Move both pointers
                    ++left;
                    --right;

                    // Skip duplicate values for the second number
                    while (left < right && nums[left] == nums[left - 1])
                    {
                        ++left;
                    }

                    // Skip duplicate values for the third number
                    while (left < right && nums[right] == nums[right + 1])
                    {
                        --right;
                    }
                }
            }
        }

        return result;
    }
};

std::string toString(const std::vector<int> &v)
{
    std::string s = "[";
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        if (i)
        {
            s += ", ";
        }
        s += std::to_string(v[i]);
    }
    return s + "]";
}

std::string toString(const std::vector<std::vector<int>> &vv)
{
    std::string s = "[";
    for (std::size_t i = 0; i < vv.size(); ++i)
    {
        if (i)
        {
            s += ", ";
        }
        s += toString(vv[i]);
    }
    return s + "]";
}

// LeetCode does not fix the order of the triplets, so both sides are put in the
// same order before they are compared. The triplets themselves come out
// ascending already, since the two-pointer scan walks a sorted array.
void check(std::vector<int> nums, std::vector<std::vector<int>> expected)
{
    const std::vector<int> original = nums;

    Solution sol;
    std::vector<std::vector<int>> got = sol.threeSum(nums);

    std::vector<std::vector<int>> gotSorted = got;
    std::sort(gotSorted.begin(), gotSorted.end());
    std::sort(expected.begin(), expected.end());

    std::cout << (gotSorted == expected ? "PASS" : "FAIL")
              << "  nums=" << toString(original)
              << "  expected=" << toString(expected)
              << "  got=" << toString(got) << "\n";
}

int main()
{
    // the LeetCode examples
    check({-1, 0, 1, 2, -1, -4}, {{-1, -1, 2}, {-1, 0, 1}});
    check({0, 1, 1}, {});
    check({0, 0, 0}, {{0, 0, 0}});

    // fewer than three numbers -> no triplet exists
    check({}, {});
    check({0}, {});
    check({0, 0}, {});

    // every number on the same side of zero -> the sum can never be zero
    check({1, 2, 3, 4}, {});
    check({-1, -2, -3, -4}, {});

    // a single triplet, already sorted and in every other order
    check({-1, 0, 1}, {{-1, 0, 1}});
    check({1, -1, 0}, {{-1, 0, 1}});
    check({0, 1, -1}, {{-1, 0, 1}});

    // repeated input triplets collapse to one entry
    check({0, 0, 0, 0}, {{0, 0, 0}});
    check({-1, 0, 1, -1, 0, 1}, {{-1, 0, 1}});
    check({-2, 0, 0, 2, 2}, {{-2, 0, 2}});

    // two of the three values equal, at both ends of the triplet
    check({-2, 1, 1}, {{-2, 1, 1}});
    check({-1, -1, 2}, {{-1, -1, 2}});

    // several distinct triplets from one array
    check({-2, -1, -1, 0, 1, 1, 2, 2},
          {{-2, 0, 2}, {-2, 1, 1}, {-1, -1, 2}, {-1, 0, 1}});
    check({-1, 0, 1, 2, -1, -4, -2, -3, 3, 0, 4},
          {{-4, 0, 4}, {-4, 1, 3}, {-3, -1, 4}, {-3, 0, 3}, {-3, 1, 2}, {-2, -1, 3}, {-2, 0, 2}, {-1, -1, 2}, {-1, 0, 1}});

    // zeros mixed with a pair that cancels
    check({-1, 0, 0, 1}, {{-1, 0, 1}});
    check({-1, 0, 0, 0, 1}, {{-1, 0, 1}, {0, 0, 0}});

    // the LeetCode value bounds, where the sum still has to stay exact
    check({-100000, 0, 100000}, {{-100000, 0, 100000}});
    check({-100000, 50000, 50000}, {{-100000, 50000, 50000}});
    check({100000, 100000, -100000}, {});
    check({-100000, -100000, 100000}, {});

    // a near miss on either side of zero -> nothing is reported
    check({-1, 0, 2}, {});
    check({-2, 0, 1}, {});

    return 0;
}
