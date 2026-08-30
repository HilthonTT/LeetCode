// 70. Climbing Stairs (Easy)
// https://leetcode.com/problems/climbing-stairs/

#include <iostream>

class Solution {
 public:
  // You are climbing a staircase that takes n steps to reach the top.
  // Each time you can climb either 1 or 2 steps,
  // return the number of distinct ways you can climb to the top.
  int climbStairs(int n) {
    // The last move onto step i is either a single step from i - 1 or a
    // double step from i - 2, and those two sets of climbs share nothing,
    // so ways(i) = ways(i - 1) + ways(i - 2): the Fibonacci recurrence.
    // Only the previous two values are ever needed, so two variables
    // walking up the staircase replace the whole table.

    // The two values below step 1: one way to stand at the bottom without
    // climbing, and a zero that makes the first iteration produce ways(1) = 1.
    int prev2 = 0;
    int prev1 = 1;

    // Each pass moves the pair one step up the staircase, so after the
    // iteration for step i, prev1 holds ways(i) and prev2 holds ways(i - 1).
    for (int i = 0; i < n; i++) {
      int current = prev1 + prev2;

      prev2 = prev1;
      prev1 = current;
    }

    // n passes leave ways(n) in prev1. n is at most 45, so the count fits in an
    // int.
    return prev1;
  }
};

void check(int n, int expected) {
  Solution sol;
  int got = sol.climbStairs(n);

  std::cout << (got == expected ? "PASS" : "FAIL") << "  n=" << n
            << "  expected=" << expected << "  got=" << got << "\n";
}

int main() {
  // the two LeetCode examples
  check(2, 2);  // 1+1, 2
  check(3, 3);  // 1+1+1, 1+2, 2+1

  // --- the smallest input the constraints allow ---
  check(1, 1);  // the single step, the one loop pass that turns the seed into 1

  // --- the small cases, enumerable by hand ---
  check(4, 5);
  check(5, 8);
  check(6, 13);
  check(7, 21);

  // --- further up, where the counts are Fibonacci numbers ---
  check(10, 89);
  check(15, 987);
  check(20, 10946);
  check(30, 1346269);
  check(40, 165580141);

  // --- the largest input the constraints allow, and the one below it ---
  check(44, 1134903170);
  check(45, 1836311903);  // still inside int: 2147483647 is the ceiling

  return 0;
}
