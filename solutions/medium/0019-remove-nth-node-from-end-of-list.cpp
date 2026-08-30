// 19. Remove Nth Node From End of List (Medium)
// https://leetcode.com/problems/remove-nth-node-from-end-of-list/

#include <iostream>
#include <numeric>
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
  ListNode* removeNthFromEnd(ListNode* head, int n) {
    // Create a dummy node pointing to head to handle edge cases
    // (e.g., removing the head node)
    ListNode* dummyNode = new ListNode(0, head);

    // Initialize two pointers for the two-pointer technique
    ListNode* fastPointer = dummyNode;
    ListNode* slowPointer = dummyNode;

    // Move fast pointer n steps ahead
    // This creates a gap of n nodes between fasts and slow pointers
    while (n--) {
      fastPointer = fastPointer->next;
    }

    // Move both pointers simultaneously until fast reaches last node
    // When fast reaches the end, slow will be at the node before the target
    while (fastPointer->next != nullptr) {
      slowPointer = slowPointer->next;
      fastPointer = fastPointer->next;
    }

    // Remove the nth node from the end by skipping it
    // slowPointer is now at the node before the one to be removed
    ListNode* nodeToDelete = slowPointer->next;
    slowPointer->next = slowPointer->next->next;

    // Cleanup memory
    delete nodeToDelete;

    // Store the actual head before deleting dummy;
    ListNode* newHead = dummyNode->next;
    delete dummyNode;

    return newHead;
  }
};

// Builds a list from its values and returns the head, or nullptr when there is
// nothing to build.
ListNode* buildList(const std::vector<int>& values) {
  ListNode dummy;
  ListNode* tail = &dummy;

  for (int value : values) {
    tail->next = new ListNode(value);
    tail = tail->next;
  }

  return dummy.next;
}

// The values of a list, in order.
std::vector<int> toVector(ListNode* head) {
  std::vector<int> values;
  for (ListNode* curr = head; curr != nullptr; curr = curr->next) {
    values.push_back(curr->val);
  }
  return values;
}

void freeList(ListNode* head) {
  while (head != nullptr) {
    ListNode* next = head->next;
    delete head;
    head = next;
  }
}

std::string format(const std::vector<int>& values) {
  if (values.empty()) {
    return "[]";
  }

  std::string text = "[";
  for (std::size_t i = 0; i < values.size(); ++i) {
    text += std::to_string(values[i]);
    if (i + 1 < values.size()) {
      text += ",";
    }
  }
  return text + "]";
}

void check(const std::vector<int>& values, int n,
           const std::vector<int>& expected, const std::string& description) {
  Solution sol;
  ListNode* head = sol.removeNthFromEnd(buildList(values), n);

  std::vector<int> got = toVector(head);
  freeList(head);

  bool passed = got == expected;

  std::cout << (passed ? "PASS" : "FAIL") << "  head=" << format(values)
            << " n=" << n << "  (" << description << ")";

  if (!passed) {
    std::cout << "  expected=" << format(expected)
              << "  got=" << format(got);
  }

  std::cout << "\n";
}

// The list 1..size, the shape most of the position checks are cut from.
std::vector<int> sequence(int size) {
  std::vector<int> values(size);
  std::iota(values.begin(), values.end(), 1);
  return values;
}

// 1..size with the value at index removed, which is what removing the nth node
// from the end of that list has to leave behind.
std::vector<int> sequenceWithout(int size, int index) {
  std::vector<int> values = sequence(size);
  values.erase(values.begin() + index);
  return values;
}

int main() {
  // the LeetCode examples
  check({1, 2, 3, 4, 5}, 2, {1, 2, 3, 5}, "the example from the statement");
  check({1}, 1, {}, "the only node, so the list is left empty");
  check({1, 2}, 1, {1}, "the last of two nodes");

  // --- the smallest lists the constraints allow ---
  check({1, 2}, 2, {2}, "the head of two nodes");
  check({7, 7}, 1, {7}, "two equal values, so position is all that decides");
  check({7, 7}, 2, {7}, "the head of two equal values");

  // --- every position of a five-node list, from the tail inwards ---
  check({1, 2, 3, 4, 5}, 1, {1, 2, 3, 4}, "the tail");
  check({1, 2, 3, 4, 5}, 2, {1, 2, 3, 5}, "one before the tail");
  check({1, 2, 3, 4, 5}, 3, {1, 2, 4, 5}, "the middle node");
  check({1, 2, 3, 4, 5}, 4, {1, 3, 4, 5}, "one after the head");
  check({1, 2, 3, 4, 5}, 5, {2, 3, 4, 5}, "the head, where the dummy earns its keep");

  // --- the boundary values the node values are allowed to take ---
  check({0, 0, 0}, 2, {0, 0}, "the smallest value a node may hold");
  check({100, 100, 100}, 3, {100, 100}, "the largest value a node may hold");
  check({0, 100, 0}, 2, {0, 0}, "the extremes alternating");

  // --- duplicates, so a solution matching on value rather than position
  //     removes the wrong node ---
  check({1, 1, 1, 1, 1}, 3, {1, 1, 1, 1}, "five equal values");
  check({2, 1, 2, 1, 2}, 4, {2, 2, 1, 2}, "a value that repeats around the target");

  // --- the longest list the constraints allow, at both ends and the middle ---
  check(sequence(30), 1, sequenceWithout(30, 29), "the tail of thirty nodes");
  check(sequence(30), 15, sequenceWithout(30, 15),
        "the middle of thirty nodes");
  check(sequence(30), 30, sequenceWithout(30, 0), "the head of thirty nodes");

  return 0;
}
