// 94. Binary Tree Inorder Traversal (Easy)
// https://leetcode.com/problems/binary-tree-inorder-traversal/

#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <vector>

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right)
      : val(x), left(left), right(right) {}
};

class Solution {
 public:
  std::vector<int> inorderTraversal(TreeNode* root) {
    // Vector to store the result of inorder traversal
    std::vector<int> result;

    // Lambda function for recursive depth-first search
    // Captures result vector by reference to modify it
    std::function<void(TreeNode*)> dfs = [&](TreeNode* node) {
      // Base case: if node is null, return
      if (!node) {
        return;
      }

      // In order traversal: Left -> Root -> Right
      dfs(node->left);              // Traverse Left subtree
      result.push_back(node->val);  // Process current node
      dfs(node->right);
    };

    dfs(root);

    return result;
  }
};

// Build a tree from LeetCode's level-order notation, where std::nullopt is
// "null" and only the children of nodes that exist take up a slot.
TreeNode* buildTree(const std::vector<std::optional<int>>& level) {
  if (level.empty() || !level[0].has_value()) {
    return nullptr;
  }

  TreeNode* root = new TreeNode(*level[0]);

  // Nodes still waiting for their two slots to be read
  std::queue<TreeNode*> pending;
  pending.push(root);

  size_t i = 1;
  while (!pending.empty() && i < level.size()) {
    TreeNode* node = pending.front();
    pending.pop();

    // Left child, then right: each parent consumes the next two slots
    if (level[i].has_value()) {
      node->left = new TreeNode(*level[i]);
      pending.push(node->left);
    }
    i++;

    if (i < level.size() && level[i].has_value()) {
      node->right = new TreeNode(*level[i]);
      pending.push(node->right);
    }
    i++;
  }

  return root;
}

// Post-order delete: both subtrees go before the node holding them
void freeTree(TreeNode* node) {
  if (!node) {
    return;
  }

  freeTree(node->left);
  freeTree(node->right);
  delete node;
}

std::string format(const std::vector<int>& values) {
  std::string s = "[";
  for (size_t i = 0; i < values.size(); i++) {
    s += (i ? "," : "") + std::to_string(values[i]);
  }

  return s + "]";
}

std::string format(const std::vector<std::optional<int>>& values) {
  std::string s = "[";
  for (size_t i = 0; i < values.size(); i++) {
    s += (i ? "," : "");
    s += values[i].has_value() ? std::to_string(*values[i]) : "null";
  }

  return s + "]";
}

// Run the traversal on a tree that has already been built, so the deep cases
// below can be assembled by hand instead of spelled out slot by slot.
void checkTree(const std::string& label, TreeNode* root,
               const std::vector<int>& expected) {
  Solution sol;
  std::vector<int> got = sol.inorderTraversal(root);
  freeTree(root);

  std::cout << (got == expected ? "PASS" : "FAIL") << "  tree=" << label
            << "  expected=" << format(expected) << "  got=" << format(got)
            << "\n"
            << std::string(60, '-') << "\n";
}

void check(const std::vector<std::optional<int>>& level,
           const std::vector<int>& expected) {
  checkTree(format(level), buildTree(level), expected);
}

int main() {
  constexpr std::optional<int> null = std::nullopt;

  // --- the LeetCode examples ---
  check({1, null, 2, 3}, {1, 3, 2});
  check({}, {});
  check({1}, {1});
  check({1, 2}, {2, 1});
  check({1, null, 2}, {1, 2});

  // --- complete trees: Left -> Root -> Right at every level ---
  check({1, 2, 3}, {2, 1, 3});
  check({1, 2, 3, 4, 5, 6, 7}, {4, 2, 5, 1, 6, 3, 7});

  // --- a BST, whose inorder traversal must come out sorted ---
  check({4, 2, 6, 1, 3, 5, 7}, {1, 2, 3, 4, 5, 6, 7});

  // --- skewed trees, one child per node ---
  check({3, 2, null, 1}, {1, 2, 3});
  check({1, null, 2, null, 3}, {1, 2, 3});

  // --- lopsided: a subtree hanging off one side only ---
  check({1, 2, null, 3, 4}, {3, 2, 4, 1});
  check({1, null, 2, 3, 4}, {1, 3, 2, 4});
  check({5, 3, 8, 1, null, null, 9}, {1, 3, 5, 8, 9});

  // --- values the traversal must carry through untouched ---
  check({0, -1, 1}, {-1, 0, 1});
  check({1, 1, 1}, {1, 1, 1});
  check({-100, -100, 100}, {-100, -100, 100});

  // --- 100 nodes, the largest tree the constraints allow ---
  std::vector<std::optional<int>> full;
  std::vector<int> fullExpected;
  for (int i = 1; i <= 100; i++) {
    full.push_back(i);
  }
  // In a complete tree the values sit in level order, so walking the built
  // tree is the only honest way to state what inorder should produce.
  std::function<void(int)> inorder = [&](int i) {
    if (i > 100) {
      return;
    }

    inorder(2 * i);
    fullExpected.push_back(i);
    inorder(2 * i + 1);
  };
  inorder(1);
  check(full, fullExpected);

  // --- 100 nodes in a single left chain: the deepest recursion possible ---
  TreeNode* chain = nullptr;
  std::vector<int> chainExpected;
  for (int i = 100; i >= 1; i--) {
    chain = new TreeNode(i, chain, nullptr);
  }
  for (int i = 100; i >= 1; i--) {
    chainExpected.push_back(i);
  }
  checkTree("left chain of 100", chain, chainExpected);

  return 0;
}
