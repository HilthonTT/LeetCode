// 16. 3Sum Closest (Medium)
// https://leetcode.com/problems/3sum-closest/

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  int threeSumClosest(std::vector<int>& nums, int target) {
    std::sort(nums.begin(), nums.end());

    int size = nums.size();
    int closestSum = 1 << 30;

    for (int i = 0; i < size; ++i) {
      int left = i + 1;
      int right = size - 1;

      while (left < right) {
        int currentSum = nums[i] + nums[left] + nums[right];
        if (currentSum == target) {
          return currentSum;
        }

        // Update closest sum if current sum is closer to target
        if (std::abs(currentSum - target) < std::abs(closestSum - target)) {
          closestSum = currentSum;
        }

        // Move pointers based on comparison with target
        if (currentSum > target) {
          // If sum is too large, move right pointer left to decrease sum
          --right;
        } else {
          // If sum is too small, move left pointer right to increase sum
          ++left;
        }
      }
    }

    return closestSum;
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

// Two different triplets can sit the same distance from the target, and
// LeetCode accepts either one, so the check compares the distance rather than
// the sum itself. `expected` is one sum the grader would accept.
void check(std::vector<int> nums, int target, int expected) {
  const std::vector<int> original = nums;

  Solution sol;
  int got = sol.threeSumClosest(nums, target);

  const bool pass = std::abs(got - target) == std::abs(expected - target);

  std::cout << (pass ? "PASS" : "FAIL") << "  nums=" << toString(original)
            << "  target=" << target << "  expected=" << expected
            << "  got=" << got << "\n";
}

int main() {
  // the LeetCode examples
  check({-1, 2, 1, -4}, 1, 2);
  check({0, 0, 0}, 1, 0);

  // the shortest possible input, where the one triplet is the answer
  check({1, 1, 1}, 0, 3);
  check({-1, 0, 1}, 0, 0);
  check({-1, 0, 1}, 5, 0);

  // the target is hit exactly, so the early return fires
  check({-1, 0, 1, 2, -1, -4}, 0, 0);
  check({4, 0, 5, -5, 3, 3, 0, -4, -5}, -2, -2);

  // every number positive -> the answer is pinned to the smallest or
  // largest triplet once the target moves past the reachable range
  check({1, 2, 3, 4}, 0, 6);
  check({1, 2, 3, 4}, 100, 9);

  // and the same on the negative side
  check({-4, -3, -2, -1}, 0, -6);
  check({-4, -3, -2, -1}, -100, -9);

  // duplicates, which the closest-sum problem does not have to collapse
  check({0, 0, 0, 0}, 1, 0);
  check({0, 0, 0, 0}, -1, 0);
  check({1, 1, 1, 1, 1}, 2, 3);

  // a tie: 6 and 8 are both one away from the target, either is accepted
  check({0, 2, 4, 6}, 7, 6);

  // negatives mixed with positives, no exact hit available
  check({-3, -2, -5, 3, -4}, -1, -2);
  check({1, 1, 1, 0}, -100, 2);
  check({-1, 2, 1, -4}, 2, 2);

  // the LeetCode value bounds on both nums and target
  check({-1000, -1000, -1000}, -10000, -3000);
  check({1000, 1000, 1000}, 10000, 3000);
  check({-1000, 1000, 0}, 0, 0);
  check({-1000, -1000, 1000, 1000}, 0, -1000);

  return 0;
}
