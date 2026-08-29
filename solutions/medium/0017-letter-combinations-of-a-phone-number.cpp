// 17. Letter Combinations of a Phone Number (Medium)
// https://leetcode.com/problems/letter-combinations-of-a-phone-number/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Solution
{
public:
    std::vector<std::string> letterCombinations(std::string digits)
    {
        if (digits.empty())
        {
            return {};
        }

        std::vector<std::string> digitToLetters = {
            "abc",  // 2
            "def",  // 3
            "ghi",  // 4
            "jkl",  // 5
            "mno",  // 6
            "pqrs", // 7
            "tuv",  // 8
            "wxyz"  // 9
        };

        std::vector<std::string> result = {""};

        for (char digit : digits)
        {
            std::string letters = digitToLetters[digit - '2'];

            std::vector<std::string> newCombinations;

            // For each existing combination in result
            for (const std::string &existingCombination : result)
            {
                // Append each possible letter to create new combinations
                for (char letter : letters)
                {
                    newCombinations.push_back(existingCombination + letter);
                }
            }

            result = std::move(newCombinations);
        }

        return result;
    }
};

// The keypad as the problem statement gives it, kept beside the checks so they
// do not have to reach into the solution for the letters of a digit.
std::string lettersFor(char digit)
{
    static const std::vector<std::string> keypad = {
        "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

    return keypad[digit - '2'];
}

std::string toString(const std::vector<std::string> &v)
{
    std::string s = "[";
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        if (i)
        {
            s += ", ";
        }
        s += "\"" + v[i] + "\"";
    }
    return s + "]";
}

// LeetCode accepts the combinations in any order, so both sides are put in the
// same order before they are compared.
void check(const std::string &digits, std::vector<std::string> expected)
{
    Solution sol;
    std::vector<std::string> got = sol.letterCombinations(digits);

    std::vector<std::string> gotSorted = got;
    std::sort(gotSorted.begin(), gotSorted.end());
    std::sort(expected.begin(), expected.end());

    std::cout << (gotSorted == expected ? "PASS" : "FAIL")
              << "  digits=\"" << digits << "\""
              << "  expected=" << toString(expected)
              << "  got=" << toString(got) << "\n";
}

// Four digits off the four-letter keys make 256 combinations, which no line of
// output is going to make readable, and writing them out by hand would only be
// the solution's own loop written a second time. Such a case is checked by the
// properties the answer has to have instead: one entry per path through the
// keypad, no entry twice, and every letter taken from the digit in its own
// position. The shorter cases above pin the letters themselves against lists
// copied from the keypad.
void checkShape(const std::string &digits, const std::string &description)
{
    Solution sol;
    std::vector<std::string> got = sol.letterCombinations(digits);

    std::size_t expectedCount = 1;
    for (char digit : digits)
    {
        expectedCount *= lettersFor(digit).size();
    }

    std::string problem;

    if (got.size() != expectedCount)
    {
        problem = "expected " + std::to_string(expectedCount) + " combinations, got " +
                  std::to_string(got.size());
    }

    // Every combination spells the digits back, one letter per digit.
    for (const std::string &combination : got)
    {
        if (!problem.empty())
        {
            break;
        }

        if (combination.size() != digits.size())
        {
            problem = "\"" + combination + "\" is not " + std::to_string(digits.size()) +
                      " letters long";
            break;
        }

        for (std::size_t i = 0; i < digits.size(); ++i)
        {
            const std::string &letters = lettersFor(digits[i]);
            if (letters.find(combination[i]) == std::string::npos)
            {
                problem = "\"" + combination + "\" has '" + combination[i] + "' where digit " +
                          digits[i] + " only allows \"" + letters + "\"";
                break;
            }
        }
    }

    // Distinct paths have to give distinct words, so a repeat means one path was
    // walked twice and another not at all.
    if (problem.empty())
    {
        std::vector<std::string> sorted = got;
        std::sort(sorted.begin(), sorted.end());
        auto duplicate = std::adjacent_find(sorted.begin(), sorted.end());
        if (duplicate != sorted.end())
        {
            problem = "\"" + *duplicate + "\" appears more than once";
        }
    }

    std::cout << (problem.empty() ? "PASS" : "FAIL")
              << "  digits=\"" << digits << "\""
              << "  (" << description << ", " << expectedCount << " combinations)";

    if (!problem.empty())
    {
        std::cout << "  " << problem;
    }

    std::cout << "\n";
}

int main()
{
    // the LeetCode examples
    check("23", {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"});
    check("", {});
    check("2", {"a", "b", "c"});

    // --- one digit: the letters of that key and nothing else ---
    check("3", {"d", "e", "f"});
    check("4", {"g", "h", "i"});
    check("5", {"j", "k", "l"});
    check("6", {"m", "n", "o"});
    check("7", {"p", "q", "r", "s"});
    check("8", {"t", "u", "v"});
    check("9", {"w", "x", "y", "z"});

    // --- two digits, both three-letter keys ---
    check("34", {"dg", "dh", "di", "eg", "eh", "ei", "fg", "fh", "fi"});

    // --- the same digit twice, where a letter has to be allowed to repeat ---
    check("22", {"aa", "ab", "ac", "ba", "bb", "bc", "ca", "cb", "cc"});
    check("77", {"pp", "pq", "pr", "ps", "qp", "qq", "qr", "qs",
                 "rp", "rq", "rr", "rs", "sp", "sq", "sr", "ss"});

    // --- the two four-letter keys, alone and together ---
    check("79", {"pw", "px", "py", "pz", "qw", "qx", "qy", "qz",
                 "rw", "rx", "ry", "rz", "sw", "sx", "sy", "sz"});

    // --- keys of different sizes, in both orders, so a letter cannot drift
    //     from one position to the other ---
    check("29", {"aw", "ax", "ay", "az", "bw", "bx", "by", "bz", "cw", "cx", "cy", "cz"});
    check("92", {"wa", "wb", "wc", "xa", "xb", "xc", "ya", "yb", "yc", "za", "zb", "zc"});

    // --- three digits ---
    check("234", {"adg", "adh", "adi", "aeg", "aeh", "aei", "afg", "afh", "afi",
                  "bdg", "bdh", "bdi", "beg", "beh", "bei", "bfg", "bfh", "bfi",
                  "cdg", "cdh", "cdi", "ceg", "ceh", "cei", "cfg", "cfh", "cfi"});

    // --- four digits, the longest input the constraints allow ---
    checkShape("2345", "four three-letter keys, all different");
    checkShape("2222", "the same three-letter key four times");
    checkShape("7979", "the four-letter keys alternating");
    checkShape("7799", "four four-letter keys, the widest answer possible");
    checkShape("7234", "a four-letter key ahead of three three-letter ones");
    checkShape("2347", "the same keys with the four-letter one last");

    return 0;
}
