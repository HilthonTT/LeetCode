// 11. Container With Most Water (Medium)
// https://leetcode.com/problems/container-with-most-water/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Solution {
 public:
  int maxArea(std::vector<int>& height) {
    int left = 0;
    int right = height.size() - 1;
    int res = 0;

    while (left < right) {
      // Find the water stored in the container between
      // height[left] and height[right]
      int water = std::min(height[left], height[right]) * (right - left);
      res = std::max(water, res);

      if (height[left] < height[right]) {
        left++;
      } else {
        right--;
      }
    }

    return res;
  }
};

// Every pair of lines, measured directly. Quadratic, so it is useless on the
// sizes the problem actually allows, but it makes no argument about which pairs
// can be skipped — and skipping the right pairs is the entire claim the
// two-pointer walk above is making. That makes it the thing worth comparing
// against.
long long reference(const std::vector<int>& height) {
  long long best = 0;

  for (size_t i = 0; i < height.size(); i++) {
    for (size_t j = i + 1; j < height.size(); j++) {
      long long water = static_cast<long long>(std::min(height[i], height[j])) *
                        static_cast<long long>(j - i);
      best = std::max(best, water);
    }
  }

  return best;
}

std::string describe(const std::vector<int>& height) {
  std::string out = "[";

  for (size_t i = 0; i < height.size(); i++) {
    out += (i ? ", " : "") + std::to_string(height[i]);
  }

  return out + "]";
}

void check(std::vector<int> height, int expected) {
  Solution sol;
  int got = sol.maxArea(height);

  std::cout << (got == expected ? "PASS" : "FAIL")
            << "  height=" << describe(height) << "\n  expected " << expected
            << "\n  got      " << got << "\n"
            << std::string(60, '-') << "\n";
}

// A pass through every height array of the given length whose values are
// 0..maxHeight, counted like an odometer. Small ranges only — the count is
// maxHeight+1 to the power of the length — but exhaustive at those sizes, which
// is where an off-by-one in the pointer walk or a mishandled tie between the
// two ends would show up. Printing one line per array would drown the cases
// above, so only failures are reported in full.
void sweep(int length, int maxHeight) {
  Solution sol;
  std::vector<int> height(length, 0);
  int failures = 0;
  long long cases = 0;

  while (true) {
    std::vector<int> input = height;
    int got = sol.maxArea(input);
    long long expected = reference(height);
    cases++;

    if (got != expected) {
      failures++;

      std::cout << "FAIL"
                << "  height=" << describe(height) << "\n  expected "
                << expected << "\n  got      " << got << "\n"
                << std::string(60, '-') << "\n";
    }

    int digit = length - 1;
    while (digit >= 0 && height[digit] == maxHeight) {
      height[digit] = 0;
      digit--;
    }
    if (digit < 0) {
      break;
    }
    height[digit]++;
  }

  std::cout << (failures == 0 ? "PASS" : "FAIL")
            << "  height=every array of length " << length << " with values 0.."
            << maxHeight << "\n  expected " << cases
            << " arrays matching the all-pairs reference"
            << "\n  got      " << (cases - failures) << " of them"
            << "\n"
            << std::string(60, '-') << "\n";
}

// The exhaustive sweep can only afford short arrays, and short arrays never put
// the two pointers far enough apart for the skipping to matter much. These are
// longer and still checked against the same reference; the generator is a fixed
// linear congruence so a failure here names an array that can be reproduced by
// running the file again.
void randomSweep(int arrays, int length, int maxHeight) {
  Solution sol;
  unsigned long long seed = 0x2545F4914F6CDD1DULL;
  int failures = 0;

  auto next = [&seed]() {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    return static_cast<unsigned int>(seed >> 33);
  };

  for (int n = 0; n < arrays; n++) {
    std::vector<int> height(length);
    for (int i = 0; i < length; i++) {
      height[i] = static_cast<int>(next() % (maxHeight + 1));
    }

    std::vector<int> input = height;
    int got = sol.maxArea(input);
    long long expected = reference(height);

    if (got != expected) {
      failures++;

      std::cout << "FAIL"
                << "  height=" << describe(height) << "\n  expected "
                << expected << "\n  got      " << got << "\n"
                << std::string(60, '-') << "\n";
    }
  }

  std::cout << (failures == 0 ? "PASS" : "FAIL") << "  height=" << arrays
            << " pseudo-random arrays of length " << length << ", values 0.."
            << maxHeight << "\n  expected " << arrays
            << " matching the all-pairs reference"
            << "\n  got      " << (arrays - failures) << " of them"
            << "\n"
            << std::string(60, '-') << "\n";
}

int main() {
  // --- the examples from the problem statement ---
  check({1, 8, 6, 2, 5, 4, 8, 3, 7}, 49);
  check({1, 1}, 1);

  // --- the shortest arrays the constraints allow ---
  check({0, 0}, 0);
  check({0, 5}, 0);
  check({5, 0}, 0);
  check({4, 7}, 4);

  // --- heights that only rise, or only fall: the best pair is the outermost
  // one ---
  check({1, 2, 3, 4, 5}, 6);
  check({5, 4, 3, 2, 1}, 6);
  check({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 25);

  // --- a flat run: width alone decides, so the two ends win ---
  check({3, 3, 3, 3, 3}, 12);
  check({7, 7}, 7);

  // --- the tallest pair is adjacent, but a shorter, wider pair beats it ---
  check({1, 100, 100, 1}, 100);
  check({100, 1, 1, 100}, 300);

  // --- zeros in the middle contribute nothing and must not stop the walk ---
  check({6, 0, 0, 0, 0, 6}, 30);
  check({0, 0, 6, 6, 0, 0}, 6);
  check({2, 0, 2}, 4);

  // --- the answer sits strictly inside, so both pointers have to move in ---
  check({1, 9, 9, 1}, 9);
  check({1, 2, 9, 9, 2, 1}, 9);

  // --- equal heights at the two ends: whichever side the tie-break moves, the
  //     other end still has to be paired against everything worth pairing ---
  check({5, 1, 1, 1, 5}, 20);
  check({5, 6, 1, 6, 5}, 20);
  check({8, 20, 1, 2, 3, 4, 5, 6, 20, 8}, 140);

  // --- a single tall line has nothing to pair with but the low ones around it
  // ---
  check({1, 1000, 1}, 2);
  check({1, 2, 1, 1, 1, 1, 1, 1, 1, 1}, 9);

  // --- large values, where the area overflows a 16-bit intermediate but not an
  // int ---
  check({10000, 10000}, 10000);
  check({10000, 1, 10000}, 20000);

  // --- and then the two sweeps against the all-pairs reference ---
  sweep(2, 4);
  sweep(3, 4);
  sweep(4, 4);
  sweep(5, 3);
  sweep(6, 2);
  randomSweep(500, 30, 50);
  randomSweep(200, 120, 10000);

  return 0;
}
