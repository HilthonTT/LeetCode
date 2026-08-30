// 21. Merge Two Sorted Lists (Easy)
// https://leetcode.com/problems/merge-two-sorted-lists/

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
  ListNode* mergeTwoLists(ListNode* head1, ListNode* head2) {
    // base cases
    if (head1 == nullptr) {
      return head2;
    }
    if (head2 == nullptr) {
      return head1;
    }

    // recursive merging based on smaller value
    if (head1->val <= head2->val) {
      head1->next = mergeTwoLists(head1->next, head2);
      return head1;
    } else {
      head2->next = mergeTwoLists(head1, head2->next);
      return head2;
    }
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

void check(const std::vector<int>& l1, const std::vector<int>& l2,
           const std::vector<int>& expected) {
  Solution sol;
  ListNode* merged = sol.mergeTwoLists(buildList(l1), buildList(l2));
  std::vector<int> got = toVector(merged);
  freeList(merged);

  std::cout << (got == expected ? "PASS" : "FAIL") << "  l1=" << toString(l1)
            << "  l2=" << toString(l2) << "  expected=" << toString(expected)
            << "  got=" << toString(got) << "\n";
}

int main() {
  // the LeetCode examples
  check({1, 2, 4}, {1, 3, 4}, {1, 1, 2, 3, 4, 4});
  check({}, {}, {});
  check({}, {0}, {0});

  // one side empty -> the other list is returned untouched
  check({1, 2, 3}, {}, {1, 2, 3});
  check({}, {1, 2, 3}, {1, 2, 3});

  // fully disjoint ranges, both orders
  check({1, 2, 3}, {4, 5, 6}, {1, 2, 3, 4, 5, 6});
  check({4, 5, 6}, {1, 2, 3}, {1, 2, 3, 4, 5, 6});

  // strict interleaving
  check({1, 3, 5}, {2, 4, 6}, {1, 2, 3, 4, 5, 6});
  check({2, 4, 6}, {1, 3, 5}, {1, 2, 3, 4, 5, 6});

  // uneven lengths -> tail of the longer list gets appended
  check({1}, {2, 3, 4, 5}, {1, 2, 3, 4, 5});
  check({1, 2, 3, 4}, {5}, {1, 2, 3, 4, 5});

  // ties -> <= keeps l1's node first, but the values are what matter
  check({1, 1, 1}, {1, 1}, {1, 1, 1, 1, 1});
  check({1, 2}, {1, 2}, {1, 1, 2, 2});

  // single nodes
  check({1}, {2}, {1, 2});
  check({2}, {1}, {1, 2});
  check({1}, {1}, {1, 1});

  // duplicates spread across both lists
  check({1, 2, 2, 5}, {2, 3, 5, 5}, {1, 2, 2, 2, 3, 5, 5, 5});

  // negatives and the LeetCode value bounds
  check({-100, -3, 0}, {-50, -1, 100}, {-100, -50, -3, -1, 0, 100});
  check({-100}, {100}, {-100, 100});

  // the original ad-hoc example
  check({5, 10, 15, 40}, {2, 3, 20}, {2, 3, 5, 10, 15, 20, 40});

  return 0;
}
