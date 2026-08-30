// 18. 4Sum (Medium)
// https://leetcode.com/problems/4sum/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  std::vector<std::vector<int>> fourSum(std::vector<int>& nums, int target) {
    int size = nums.size();
    std::vector<std::vector<int>> result;

    // The contraint: We require at least 4 numbers to form a quadruplet
    if (size < 4) {
      return result;
    }

    // Sort the array to enable two-pointer technique
    std::sort(nums.begin(), nums.end());

    // First number: iterate through possible values
    for (int i = 0; i < size - 3; ++i) {
      // Skip duplicate values for the first number
      if (i > 0 && nums[i] == nums[i - 1]) {
        continue;
      }

      // Second number: iterate through possible values
      for (int j = i + 1; j < size - 2; ++j) {
        // Skip duplicate values for the second number
        if (j > i + 1 && nums[j] == nums[j - 1]) {
          continue;
        }

        // Use two pointers for remaining two numbers
        int left = j + 1;
        int right = size - 1;

        while (left < right) {
          // Use long long to prevent integer overflow
          long long sum = static_cast<long long>(nums[i]) + nums[j] +
                          nums[left] + nums[right];

          if (sum < target) {
            // Sum is too small, move left pointer to increase sum
            ++left;
          } else if (sum > target) {
            // Sum is too large, move right pointer to decrease sum
            --right;
          } else {
            // Found a valid quadruplet
            result.push_back({nums[i], nums[j], nums[left], nums[right]});

            // Move both pointers and skip duplicates
            ++left;
            --right;

            // Skip duplicate values for the third number
            while (left < right && nums[left] == nums[left - 1]) {
              ++left;
            }

            // Skip duplicate values for the fourth number
            while (left < right && nums[right] == nums[right + 1]) {
              --right;
            }
          }
        }
      }
    }

    return result;
  }
};

std::string toString(const std::vector<int>& v) {
  std::string s = "[";
  for (std::size_t i = 0; i < v.size(); ++i) {
    if (i) {
      s += ", ";
    }
    s += std::to_string(v[i]);
  }
  return s + "]";
}

std::string toString(const std::vector<std::vector<int>>& vv) {
  std::string s = "[";
  for (std::size_t i = 0; i < vv.size(); ++i) {
    if (i) {
      s += ", ";
    }
    s += toString(vv[i]);
  }
  return s + "]";
}

// LeetCode does not fix the order of the quadruplets, so both sides are put in
// the same order before they are compared. The quadruplets themselves come out
// ascending already, since the two-pointer scan walks a sorted array.
void check(std::vector<int> nums, int target,
           std::vector<std::vector<int>> expected) {
  const std::vector<int> original = nums;

  Solution sol;
  std::vector<std::vector<int>> got = sol.fourSum(nums, target);

  std::vector<std::vector<int>> gotSorted = got;
  std::sort(gotSorted.begin(), gotSorted.end());
  std::sort(expected.begin(), expected.end());

  std::cout << (gotSorted == expected ? "PASS" : "FAIL")
            << "  nums=" << toString(original) << "  target=" << target
            << "  expected=" << toString(expected) << "  got=" << toString(got)
            << "\n";
}

int main() {
  // the LeetCode examples
  check({1, 0, -1, 0, -2, 2}, 0,
        {{-2, -1, 1, 2}, {-2, 0, 0, 2}, {-1, 0, 0, 1}});
  check({2, 2, 2, 2, 2}, 8, {{2, 2, 2, 2}});

  // fewer than four numbers -> no quadruplet exists
  check({}, 0, {});
  check({0}, 0, {});
  check({0, 0}, 0, {});
  check({0, 0, 0}, 0, {});

  // exactly four numbers, hit and miss
  check({1, 2, 3, 4}, 10, {{1, 2, 3, 4}});
  check({1, 2, 3, 4}, 0, {});
  check({0, 0, 0, 0}, 0, {{0, 0, 0, 0}});
  check({0, 0, 0, 0}, 1, {});

  // repeated inputs collapse to a single entry
  check({0, 0, 0, 0, 0}, 0, {{0, 0, 0, 0}});
  check({1, 1, 1, 1, 2, 2, 2, 2}, 6, {{1, 1, 2, 2}});

  // a target that is not zero, on both sides
  check({-3, -1, 0, 2, 4, 5}, 2, {{-3, -1, 2, 4}});
  check({-5, 5, 4, -3, 0, 0, 4, -2}, 4, {{-5, 0, 4, 5}, {-3, -2, 4, 5}});
  check({-4, -3, -2, -1}, -10, {{-4, -3, -2, -1}});

  // many overlapping quadruplets from one array, duplicates and all
  check({-3, -2, -1, 0, 0, 1, 2, 3}, 0,
        {{-3, -2, 2, 3},
         {-3, -1, 1, 3},
         {-3, 0, 0, 3},
         {-3, 0, 1, 2},
         {-2, -1, 0, 3},
         {-2, -1, 1, 2},
         {-2, 0, 0, 2},
         {-1, 0, 0, 1}});

  // the LeetCode value bounds, where a four-number sum overflows a 32-bit int
  check({1000000000, 1000000000, 1000000000, 1000000000}, -294967296, {});
  check({1000000000, 1000000000, 1000000000, 1000000000}, 0, {});
  check({-1000000000, -1000000000, 1000000000, 1000000000}, 0,
        {{-1000000000, -1000000000, 1000000000, 1000000000}});
  check({-1000000000, -1000000000, -1000000000, -1000000000}, 294967296, {});

  return 0;
}
