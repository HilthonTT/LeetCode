// 58. Length of Last Word (Easy)
// https://leetcode.com/problems/length-of-last-word/

#include <string>
#include <iostream>

class Solution
{
public:
    // Given a string s consisting of words and spaces,
    // return the length of the last word in the string.
    int lengthOfLastWord(std::string s)
    {
        // Get the last index of the string
        int endIndex = s.size() - 1;

        // Skip trailing spaces from the end
        while (endIndex >= 0 && s[endIndex] == ' ')
        {
            --endIndex;
        }

        // Mark the end position of the last word
        int startIndex = endIndex;

        // Move backwards to find the start of the last word
        while (startIndex >= 0 && s[startIndex] != ' ')
        {
            --startIndex;
        }

        // Calculate and return the length of the last word
        // startIndex points to space before word or -1, endIndex points to last char of word
        return endIndex - startIndex;
    }
};

// A ten thousand character input is unreadable in the output, so a check may
// name its input instead of printing it.
void check(const std::string &s, int expected, const std::string &description = "")
{
    Solution sol;
    int got = sol.lengthOfLastWord(s);

    std::cout << (got == expected ? "PASS" : "FAIL")
              << "  s=" << (description.empty() ? "\"" + s + "\"" : description)
              << "  expected=" << expected
              << "  got=" << got
              << "\n";
}

int main()
{
    // the three LeetCode examples
    check("Hello World", 5);
    check("   fly me   to   the moon  ", 4);
    check("luffy is still joyboy", 6);

    // --- the shortest input the constraints allow: a single letter ---
    check("a", 1);
    check("Z", 1);

    // --- one word, with spaces on either side or both ---
    check("day", 3);
    check("day ", 3);
    check(" day", 3);
    check("   day   ", 3);

    // --- trailing spaces are skipped before the word is measured ---
    check("hello   ", 5);
    check("a ", 1);
    check(" a ", 1);

    // --- runs of spaces between words belong to no word ---
    check("hello     world", 5);
    check("one two   three", 5);

    // --- the last word is what counts, not the first or the longest ---
    check("elephant a", 1);
    check("a elephant", 8);
    check("bb a ccc", 3);

    // --- the word starts at index 0, so the second loop stops on -1 ---
    check("word", 4);
    check("abcdefghij", 10);

    // --- both letter cases are allowed and neither is special ---
    check("Hello wORLD", 5);

    // --- the longest input the constraints allow: 10^4 characters ---
    check(std::string(10000, 'x'), 10000, "10000 letters, one word");
    check(std::string(9999, ' ') + "x", 1, "9999 spaces then one letter");
    check("x" + std::string(9999, ' '), 1, "one letter then 9999 spaces");

    return 0;
}
