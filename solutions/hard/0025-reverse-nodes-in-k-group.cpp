// 25. Reverse Nodes in k-Group (Hard)
// https://leetcode.com/problems/reverse-nodes-in-k-group/

#include <iostream>
#include <string>
#include <vector>

struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
 public:
  ListNode* reverseKGroup(ListNode* head, int k) {
    // Create a dummy node pointing to head for easier manipulation
    ListNode* dummyHead = new ListNode(0, head);

    // Pointer to track the node before the current group
    ListNode* prevGroupEnd = dummyHead;

    while (prevGroupEnd != nullptr) {
      // Check if there are at least k nodes remaining
      ListNode* currentNode = prevGroupEnd;

      for (int i = 0; i < k; i++) {
        currentNode = currentNode->next;
        if (currentNode == nullptr) {
          // Less than k nodes remaining, return the result
          return dummyHead->next;
        }
      }

      // Save pointers for the current group
      ListNode* groupStart = prevGroupEnd->next;  // First node of current group
      ListNode* nextGroupStart = currentNode->next;  // First node of next group

      // Disconnect the current group from the rest of the list
      currentNode->next = nullptr;

      // Reverse the current group and connect it back
      prevGroupEnd->next = reverseList(groupStart);

      // After reversal, groupStart becomes the last node of the reversed group
      groupStart->next = nextGroupStart;

      // Move prevGroupEnd to the end of the current reversed group
      prevGroupEnd = groupStart;
    }

    return dummyHead->next;
  }

 private:
  ListNode* reverseList(ListNode* head) {
    // Create a dummy node to simplify the reversal process
    ListNode* dummyNode = new ListNode();
    ListNode* current = head;

    // Iterate through the list and reverse connections
    while (current != nullptr) {
      ListNode* nextNode = current->next;  // Save the next node
      current->next =
          dummyNode->next;  // Point current to the previous reversed portion
      dummyNode->next = current;  // Update dummy to point to current
      current = nextNode;         // Move to the next node
    }

    return dummyNode->next;
  }
};

ListNode* buildList(const std::vector<int>& v) {
  ListNode* head = nullptr;
  ListNode* tail = nullptr;
  for (int x : v) {
    ListNode* node = new ListNode(x);
    if (head == nullptr) {
      head = node;
    } else {
      tail->next = node;
    }
    tail = node;
  }
  return head;
}

std::vector<int> toVector(ListNode* curr) {
  std::vector<int> v;
  while (curr != nullptr) {
    v.push_back(curr->val);
    curr = curr->next;
  }
  return v;
}

void freeList(ListNode* curr) {
  while (curr != nullptr) {
    ListNode* next = curr->next;
    delete curr;
    curr = next;
  }
}

std::string toString(const std::vector<int>& v) {
  std::string s = "[";
  for (std::size_t i = 0; i < v.size(); ++i) {
    if (i) {
      s += ", ";
    }
    s += std::to_string(v[i]);
  }
  return s + "]";
}

void check(const std::vector<int>& list, int k,
           const std::vector<int>& expected) {
  Solution sol;
  ListNode* result = sol.reverseKGroup(buildList(list), k);
  std::vector<int> got = toVector(result);
  freeList(result);

  std::cout << (got == expected ? "PASS" : "FAIL")
            << "  list=" << toString(list) << "  k=" << k
            << "  expected=" << toString(expected) << "  got=" << toString(got)
            << "\n";
}

int main() {
  // the LeetCode examples
  check({1, 2, 3, 4, 5}, 2, {2, 1, 4, 3, 5});
  check({1, 2, 3, 4, 5}, 3, {3, 2, 1, 4, 5});

  // k = 1 -> every group is a single node, so the list is unchanged
  check({1, 2, 3, 4, 5}, 1, {1, 2, 3, 4, 5});

  // k equal to the length -> the whole list is reversed exactly once
  check({1, 2, 3, 4, 5}, 5, {5, 4, 3, 2, 1});

  // k larger than the length -> fewer than k nodes, nothing moves
  check({1, 2, 3}, 5, {1, 2, 3});

  // length an exact multiple of k -> no leftover tail
  check({1, 2, 3, 4, 5, 6}, 2, {2, 1, 4, 3, 6, 5});
  check({1, 2, 3, 4, 5, 6}, 3, {3, 2, 1, 6, 5, 4});
  check({1, 2, 3, 4, 5, 6}, 6, {6, 5, 4, 3, 2, 1});

  // leftover tail of every size below k stays in its original order
  check({1, 2, 3, 4, 5, 6, 7}, 3, {3, 2, 1, 6, 5, 4, 7});
  check({1, 2, 3, 4, 5, 6, 7, 8}, 3, {3, 2, 1, 6, 5, 4, 7, 8});

  // single node, the smallest list the constraints allow
  check({1}, 1, {1});
  check({1}, 2, {1});

  // empty list -> nothing to reverse for any k
  check({}, 1, {});
  check({}, 3, {});

  // duplicates -> reversal is by position, not by value
  check({1, 1, 2, 2}, 2, {1, 1, 2, 2});
  check({1, 2, 2, 1}, 2, {2, 1, 1, 2});

  // negatives and the LeetCode value bounds
  check({-1000, 0, 1000}, 3, {1000, 0, -1000});
  check({-1000, -999, 999, 1000}, 2, {-999, -1000, 1000, 999});

  // a longer list, to keep the group boundaries honest past the first few
  check({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 4, {4, 3, 2, 1, 8, 7, 6, 5, 9, 10});

  return 0;
}
