#include <vector>
#include <optional>
#include <queue>
#include <string>
#include <iostream>

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution
{
public:
    bool isSameTree(TreeNode *p, TreeNode *q)
    {
        // They're both nullptrs, they're the same.
        if (p == nullptr && q == nullptr)
        {
            return true;
        }

        // One of them is a nullptr, they're not the same.
        if (p == nullptr || q == nullptr)
        {
            return false;
        }

        return (p->val == q->val) && isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
};

// Build a tree from LeetCode's level-order notation, where std::nullopt is
// "null" and only the children of nodes that exist take up a slot.
TreeNode *buildTree(const std::vector<std::optional<int>> &level)
{
    if (level.empty() || !level[0].has_value())
    {
        return nullptr;
    }

    TreeNode *root = new TreeNode(*level[0]);

    // Nodes still waiting for their two slots to be read
    std::queue<TreeNode *> pending;
    pending.push(root);

    size_t i = 1;
    while (!pending.empty() && i < level.size())
    {
        TreeNode *node = pending.front();
        pending.pop();

        // Left child, then right: each parent consumes the next two slots
        if (level[i].has_value())
        {
            node->left = new TreeNode(*level[i]);
            pending.push(node->left);
        }
        i++;

        if (i < level.size() && level[i].has_value())
        {
            node->right = new TreeNode(*level[i]);
            pending.push(node->right);
        }
        i++;
    }

    return root;
}

// Post-order delete: both subtrees go before the node holding them
void freeTree(TreeNode *node)
{
    if (!node)
    {
        return;
    }

    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

std::string format(const std::vector<std::optional<int>> &values)
{
    std::string s = "[";
    for (size_t i = 0; i < values.size(); i++)
    {
        s += (i ? "," : "");
        s += values[i].has_value() ? std::to_string(*values[i]) : "null";
    }

    return s + "]";
}

// Compare two trees that have already been built, so the deep cases below can
// be assembled by hand instead of spelled out slot by slot.
void checkTrees(const std::string &labelP, const std::string &labelQ,
                TreeNode *p, TreeNode *q, bool expected)
{
    Solution sol;
    bool got = sol.isSameTree(p, q);

    // "Same" is symmetric, so swapping the arguments must not change the
    // answer: a check that only looks at p, or only at q, would be caught here.
    bool swapped = sol.isSameTree(q, p);

    freeTree(p);
    freeTree(q);

    std::cout << (got == expected && swapped == expected ? "PASS" : "FAIL")
              << "  p=" << labelP
              << "  q=" << labelQ
              << "  expected=" << (expected ? "true" : "false")
              << "  got=" << (got ? "true" : "false")
              << (swapped == got ? "" : "  (swapping p and q flipped the answer)")
              << "\n"
              << std::string(60, '-') << "\n";
}

void check(const std::vector<std::optional<int>> &p,
           const std::vector<std::optional<int>> &q, bool expected)
{
    checkTrees(format(p), format(q), buildTree(p), buildTree(q), expected);
}

int main()
{
    constexpr std::optional<int> null = std::nullopt;

    // --- the LeetCode examples ---
    check({1, 2, 3}, {1, 2, 3}, true);
    check({1, 2}, {1, null, 2}, false);
    check({1, 2, 1}, {1, 1, 2}, false);

    // --- empty and single-node trees ---
    check({}, {}, true);
    check({}, {1}, false);
    check({1}, {1}, true);
    check({1}, {2}, false);

    // --- same shape, one value out of place ---
    check({1, 2, 3, 4, 5, 6, 7}, {1, 2, 3, 4, 5, 6, 7}, true);
    check({1, 2, 3, 4, 5, 6, 7}, {1, 2, 3, 4, 5, 6, 8}, false);  // deepest, last
    check({1, 2, 3, 4, 5, 6, 7}, {1, 2, 3, 9, 5, 6, 7}, false);  // deepest, first
    check({1, 2, 3, 4, 5, 6, 7}, {9, 2, 3, 4, 5, 6, 7}, false);  // at the root

    // --- same values, different shape ---
    check({1, 2, 3}, {1, 3, 2}, false);                  // mirrored
    check({1, 2, 2, 3}, {1, 2, 2, null, 3}, false);      // grandchild on the other side
    check({1, null, 2, null, 3}, {1, 2, null, 3}, false); // right chain vs left chain

    // --- one tree is the other plus an extra node ---
    check({1, 2, 3}, {1, 2, 3, 4}, false);
    check({1, 2, 3, 4, 5}, {1, 2, 3, 4}, false);

    // --- lopsided trees that agree ---
    check({5, 3, 8, 1, null, null, 9}, {5, 3, 8, 1, null, null, 9}, true);
    check({5, 3, 8, 1, null, null, 9}, {5, 3, 8, null, 1, null, 9}, false);

    // --- values at the edges of the constraints ---
    check({0, -1, 1}, {0, -1, 1}, true);
    check({-10000, 10000}, {-10000, 10000}, true);
    check({-10000}, {10000}, false);
    check({1, 1, 1}, {1, 1, 1}, true);

    // --- 100 nodes, the largest tree the constraints allow ---
    std::vector<std::optional<int>> full;
    for (int i = 1; i <= 100; i++)
    {
        full.push_back(i);
    }
    check(full, full, true);

    // The one node that differs is the last leaf, so a comparison that gives
    // up early would call these equal.
    std::vector<std::optional<int>> almost = full;
    almost.back() = 101;
    check(full, almost, false);

    // --- 100-node chains: the deepest recursion possible ---
    TreeNode *leftA = nullptr;
    TreeNode *leftB = nullptr;
    TreeNode *right = nullptr;
    for (int i = 100; i >= 1; i--)
    {
        leftA = new TreeNode(i, leftA, nullptr);
        leftB = new TreeNode(i, leftB, nullptr);
        right = new TreeNode(i, nullptr, right);
    }
    checkTrees("left chain of 100", "left chain of 100", leftA, leftB, true);

    // Same 100 values in the same order, hanging off the other side
    TreeNode *leftC = nullptr;
    for (int i = 100; i >= 1; i--)
    {
        leftC = new TreeNode(i, leftC, nullptr);
    }
    checkTrees("left chain of 100", "right chain of 100", leftC, right, false);

    return 0;
}
