#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

class Solution
{
public:
    int removeElement(std::vector<int> &nums, int val)
    {
        int k = 0;
        for (int i = 0; i < nums.size(); i++)
        {
            // Place the element which is not
            // equal to ele at the kth position
            if (nums[i] != val)
            {
                std::swap(nums[k], nums[i]);

                // Increment the count of
                // elements not equal to ele
                k++;
            }
        }

        return k;
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

// The judge only looks at the first k elements and ignores their order,
// so compare them as multisets against the expected survivors.
void check(std::vector<int> nums, int val, std::vector<int> expected)
{
    Solution sol;
    std::vector<int> original = nums;
    int k = sol.removeElement(nums, val);

    std::vector<int> got(nums.begin(), nums.begin() + k);
    std::vector<int> gotSorted = got;
    std::sort(gotSorted.begin(), gotSorted.end());
    std::sort(expected.begin(), expected.end());

    bool ok = k == static_cast<int>(expected.size()) && gotSorted == expected;

    std::cout << (ok ? "PASS" : "FAIL")
              << "  nums=" << toString(original)
              << "  val=" << val
              << "  expected k=" << expected.size() << " " << toString(expected)
              << "  got k=" << k << " " << toString(got) << "\n";
}

int main()
{
    // the two LeetCode examples
    check({3, 2, 2, 3}, 3, {2, 2});
    check({0, 1, 2, 2, 3, 0, 4, 2}, 2, {0, 1, 3, 0, 4});

    // empty input
    check({}, 0, {});

    // single element, hit and miss
    check({1}, 1, {});
    check({1}, 2, {1});

    // val absent -> everything survives, order untouched
    check({1, 2, 3, 4}, 5, {1, 2, 3, 4});

    // every element matches -> k == 0
    check({7, 7, 7, 7}, 7, {});

    // matches only at the front
    check({2, 2, 1, 3}, 2, {1, 3});

    // matches only at the back
    check({1, 3, 2, 2}, 2, {1, 3});

    // alternating matches
    check({1, 2, 1, 2, 1}, 2, {1, 1, 1});
    check({2, 1, 2, 1, 2}, 2, {1, 1});

    // one match in the middle
    check({1, 2, 3}, 2, {1, 3});

    // duplicates that are not val must all be kept
    check({4, 4, 5, 4, 4}, 5, {4, 4, 4, 4});

    // negatives and zero as val (0 is inside the LeetCode value range)
    check({0, -1, 0, -2, 0}, 0, {-1, -2});
    check({-1, -1, 2}, -1, {2});

    // LeetCode value bounds
    check({50, 100, 0, 100}, 100, {50, 0});

    // longer mixed case
    check({3, 1, 3, 2, 3, 3, 4, 3, 5}, 3, {1, 2, 4, 5});

    return 0;
}
