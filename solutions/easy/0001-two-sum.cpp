// 1. Two Sum (Easy)
// https://leetcode.com/problems/two-sum/

#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

class Solution
{
public:
    std::vector<int> twoSum(std::vector<int> &nums, int target)
    {
        std::unordered_map<int, int> seen; // value -> index

        std::cout << "target = " << target << "\n\n";

        for (size_t i = 0; i < nums.size(); ++i)
        {
            std::cout << "i=" << i << "  nums[i]=" << nums[i] << "\n";

            int need = target - nums[i];
            std::cout << "    need = " << target << " - " << nums[i]
                      << " = " << need << "\n";

            auto it = seen.find(need);

            if (it != seen.end())
            {
                std::cout << "    found " << need << " in map at index "
                          << it->second << "  -> returning {"
                          << it->second << ", " << i << "}\n";
                return {it->second, static_cast<int>(i)};
            }

            std::cout << "    " << need << " not in map yet\n";

            seen[nums[i]] = static_cast<int>(i);
            std::cout << "    stored nums[i]=" << nums[i]
                      << " -> index " << i << "\n";
            std::cout << "    map now: " << dumpMap(seen) << "\n\n";
        }

        std::cout << "loop finished, no pair found -> returning {}\n";
        return {};
    }

private:
    static std::string dumpMap(const std::unordered_map<int, int> &m)
    {
        std::string s = "{";
        bool first = true;
        for (const auto &[value, index] : m)
        {
            if (!first)
            {
                s += ", ";
            }
            s += std::to_string(value) + ":" + std::to_string(index);
            first = false;
        }
        return s + "}";
    }
};

std::string toString(const std::vector<int> &v)
{
    std::string s = "[";
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (i)
            s += ", ";
        s += std::to_string(v[i]);
    }
    return s + "]";
}

void check(std::vector<int> nums, int target, std::vector<int> expected)
{
    Solution sol;
    std::vector<int> got = sol.twoSum(nums, target);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  nums=" << toString(nums)
              << " target=" << target
              << "  expected=" << toString(expected)
              << "  got=" << toString(got)
              << "\n";
}

int main()
{
    check({2, 7, 11, 15}, 9, {0, 1});
    check({3, 2, 4}, 6, {1, 2});
    check({3, 3}, 6, {0, 1});
    check({0, 4, 3, 0}, 0, {0, 3});
    check({-1, -2, -3, -4, -5}, -8, {2, 4});

    return 0;
}
