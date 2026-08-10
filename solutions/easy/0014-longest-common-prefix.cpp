// 14. Longest Common Prefix (Easy)
// https://leetcode.com/problems/longest-common-prefix/

#include <string>
#include <vector>
#include <iostream>

class Solution
{
public:
    std::string longestCommonPrefix(std::vector<std::string> &strs)
    {
        if (strs.empty())
        {
            return "";
        }

        const std::string &first = strs.front();
        for (std::size_t i = 0; i < first.size(); ++i)
        {
            for (const std::string &s : strs)
            {
                if (i >= s.size() || s[i] != first[i])
                {
                    return first.substr(0, i);
                }
            }
        }

        return first;
    }
};

std::string toString(const std::vector<std::string> &v)
{
    std::string s = "[";
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        if (i)
            s += ", ";
        s += "\"" + v[i] + "\"";
    }
    return s + "]";
}

void check(std::vector<std::string> strs, const std::string &expected)
{
    Solution sol;
    std::string got = sol.longestCommonPrefix(strs);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  strs=" << toString(strs)
              << "  expected=\"" << expected << "\""
              << "  got=\"" << got << "\"\n";
}

int main()
{
    // the two LeetCode examples
    check({"flower", "flow", "flight"}, "fl");
    check({"dog", "racecar", "car"}, "");

    // single element -> the whole string
    check({"alone"}, "alone");

    // all identical
    check({"same", "same", "same"}, "same");

    // one string is a prefix of the others
    check({"ab", "abc", "abcd"}, "ab");
    check({"abcd", "abc", "ab"}, "ab"); // shortest last vs first

    // shortest string is the first one -> exits via the outer loop ending
    check({"a", "ab", "abc"}, "a");

    // divergence on the very first character
    check({"apple", "banana"}, "");

    // empty string present anywhere kills the prefix
    check({"", "abc"}, "");
    check({"abc", ""}, "");
    check({"abc", "", "abd"}, "");

    // single characters
    check({"a", "a"}, "a");
    check({"a", "b"}, "");

    // full match then divergence at the last position
    check({"abcde", "abcdf"}, "abcd");

    // empty vector -> guarded by the early return
    check({}, "");

    return 0;
}
