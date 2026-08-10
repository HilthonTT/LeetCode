// 28. Find the Index of the First Occurrence in a String (Easy)
// https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/

#include <string>
#include <iostream>

class Solution
{
public:
    // Given two strings needle and haystack,
    // return the index of the first occurrence of needle in haystack,
    // or -1 if needle is not part of haystack.
    int strStr(std::string haystack, std::string needle)
    {
        if (needle.empty())
        {
            return 0;
        }

        size_t haystackLength = haystack.length();
        size_t needleLength = needle.length();

        // If the needle's word is longer than the haystack's word
        // then it's impossible for it to have an occurence since the word is longer.
        if (needleLength > haystackLength)
        {
            return -1;
        }

        // Iterate over the haystack and attempt to find the first occurence of needle in haystack then return the index.
        for (int startIndex = 0; startIndex <= haystackLength - needleLength; startIndex++)
        {
            if (haystack.substr(startIndex, needleLength) == needle)
            {
                return startIndex;
            }
        }

        return -1;
    }
};

void check(const std::string &haystack, const std::string &needle, int expected)
{
    Solution sol;
    int got = sol.strStr(haystack, needle);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  haystack=\"" << haystack << "\""
              << "  needle=\"" << needle << "\""
              << "  expected=" << expected
              << "  got=" << got
              << "\n";
}

int main()
{
    // the two LeetCode examples
    check("sadbutsad", "sad", 0);
    check("leetcode", "leeto", -1);

    // --- an empty needle always matches at 0, whatever the haystack ---
    check("abc", "", 0);
    check("", "", 0);

    // --- needle longer than the haystack -> guarded early return ---
    check("", "a", -1);
    check("ab", "abc", -1);

    // --- match position within the haystack ---
    check("hello", "ll", 2);
    check("hello", "o", 4);   // last character
    check("hello", "h", 0);   // first character
    check("abc", "abc", 0);   // whole string
    check("abc", "d", -1);    // absent single character

    // --- the first occurrence wins, not a later one ---
    check("ababab", "ab", 0);
    check("aaaaa", "aa", 0);
    check("mississippi", "issip", 4);

    // --- partial matches that must not be reported ---
    check("mississippi", "issipi", -1);
    check("aaa", "aab", -1);
    check("abcabd", "abd", 3); // restarts after a failed partial match

    // --- match sitting at the very end, the last index the loop visits ---
    check("abcde", "de", 3);
    check("abcde", "e", 4);

    // --- repeated characters and longer needles ---
    check("aaaaaaab", "aaab", 4);
    check("banana", "nan", 2);
    check("banana", "na", 2);

    return 0;
}
