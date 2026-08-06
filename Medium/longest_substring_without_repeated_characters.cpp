#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class Solution
{
public:
    int lengthOfLongestSubstring(const std::string &s)
    {
        std::vector<bool> vis(128, false);
        size_t res = 0, left = 0;

        for (size_t right = 0; right < s.length(); ++right)
        {
            unsigned char c = static_cast<unsigned char>(s[right]);
            while (vis[c])
            {
                vis[static_cast<unsigned char>(s[left])] = false;
                ++left;
            }
            vis[c] = true;
            res = std::max(res, right - left + 1);
        }
        return static_cast<int>(res);
    }
};

void check(const std::string &s, int expected)
{
    Solution sol;
    int got = sol.lengthOfLongestSubstring(s);

    // keep the printed input short so the stress cases stay readable
    std::string shown = s.size() <= 30 ? s : s.substr(0, 27) + "...";

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  s=\"" << shown << "\""
              << "  len=" << s.size()
              << "  expected=" << expected
              << "  got=" << got
              << "\n"
              << std::string(60, '-') << "\n";
}

int main()
{
    // --- the LeetCode examples ---
    check("abcabcbb", 3); // "abc"
    check("bbbbb", 1);    // "b"
    check("pwwkew", 3);   // "wke", not the subsequence "pwke"

    // --- empty and single character ---
    check("", 0);
    check("a", 1);
    check(" ", 1);

    // --- two characters, both ways ---
    check("aa", 1);
    check("au", 2);

    // --- the repeat sits outside the current window ---
    // naive "reset left to the duplicate + 1" solutions get these wrong
    check("abba", 2);    // "ab" / "ba"
    check("dvdf", 3);    // "vdf"
    check("tmmzuxt", 5); // "mzuxt"
    check("abcb", 3);    // "abc", and 'b' must not drag left backwards

    // --- duplicate at the front, middle, and back ---
    check("aab", 2);    // "ab"
    check("abca", 3);   // "abc"
    check("abcda", 4);  // "abcd"
    check("cdd", 2);    // "cd"
    check("abcdd", 4);   // "abcd"
    check("aabcdef", 6); // "abcdef"

    // --- no repeats at all ---
    check("abcdefg", 7);
    check("abcdefghijklmnopqrstuvwxyz", 26);

    // --- every character the same ---
    check(std::string(1000, 'x'), 1);

    // --- the answer is the whole tail / the whole head ---
    check("aaabcdef", 6); // tail
    check("abcdefaaa", 6); // head

    // --- digits, symbols and spaces are all valid input ---
    check("0123456789", 10);
    check("112233", 2);
    check("a b c", 3);       // "a b" -- the spaces repeat too
    check("hello world", 6); // " world"
    check("!@#$%^&*()", 10);
    check("a!a!b", 2);   // "a!" / "!a" / "!b"
    check("A a A a", 3); // "A a" -- 'A' and 'a' differ, the spaces do not
    check("aAbBcC", 6);  // upper and lower are distinct

    // --- longer, mixed input ---
    check("geeksforgeeks", 7); // "ksforge"
    check("longestsubstring", 8); // "ubstring"

    // --- stress: 26 letters repeated, answer stays 26 ---
    std::string cycle;
    for (int i = 0; i < 2000; ++i)
    {
        for (char c = 'a'; c <= 'z'; ++c)
        {
            cycle += c;
        }
    }
    check(cycle, 26); // 52000 chars, above the 5*10^4 constraint

    // --- stress: one long unique run buried in repeats ---
    std::string buried = std::string(10000, 'q');
    for (char c = 'a'; c <= 'p'; ++c)
    {
        buried += c; // 16 unique characters
    }
    buried += std::string(10000, 'q');
    check(buried, 17); // the 16 unique ones plus one neighbouring 'q'

    return 0;
}
