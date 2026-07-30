#include <vector>
#include <iostream>
#include <unordered_set>

class Solution
{
public:
    int removeDuplicates1(std::vector<int> &nums)
    {
        if (nums.empty())
        {
            return 0;
        }

        // To track seen elements
        std::unordered_set<int> set;

        // Maintain new size of the array
        int idx = 0;

        for (int i = 0; i < nums.size(); i++)
        {
            // If not found, add it the set list.
            if (set.find(nums[i]) == set.end())
            {
                set.insert(nums[i]);
                nums[idx++] = nums[i];
            }
        }

        // Return the size of the array
        // with unique elements
        return set.size();
    }

    int removeDuplicates2(std::vector<int> &nums)
    {
        if (nums.empty())
        {
            return 0;
        }

        int n = nums.size();
        if (n <= 1)
            return n;

        // Start from the second element
        int idx = 1;

        for (int i = 1; i < n; i++)
        {
            if (nums[i] != nums[i - 1])
            {
                nums[idx++] = nums[i];
            }
        }
        return idx;
    }
};

int main()
{
    Solution sol;

    std::vector<int> arr1 = {1, 2, 2, 3, 4, 4, 4, 5, 5};
    int newSize = sol.removeDuplicates1(arr1);

    std::cout << "SOLUTION 1: WITH SET APPROACH" << std::endl;
    for (int i = 0; i < newSize; i++)
    {
        std::cout << arr1[i] << " ";
    }

    std::cout << std::endl;

    std::vector<int> arr2 = {1, 2, 2, 3, 4, 4, 4, 5, 5};
    std::cout << "SOLUTION 2: EXPECTED APPROACH" << std::endl;
    newSize = sol.removeDuplicates2(arr2);
    for (int i = 0; i < newSize; i++)
    {
        std::cout << arr1[i] << " ";
    }

    return 0;
}
