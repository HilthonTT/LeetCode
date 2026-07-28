#include <string>
#include <vector>

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

int main()
{
    return 0;
}
