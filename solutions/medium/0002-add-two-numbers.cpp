// 2. Add Two Numbers (Medium)
// https://leetcode.com/problems/add-two-numbers/

#include <iostream>

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution
{
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {
        ListNode dummy; // stack-allocated sentinel, nothing to free
        ListNode *tail = &dummy;
        int carry = 0;

        while (l1 || l2 || carry)
        {
            int v = carry;
            if (l1)
            {
                v += l1->val;
                l1 = l1->next;
            }
            if (l2)
            {
                v += l2->val;
                l2 = l2->next;
            }

            carry = v / 10;
            tail->next = new ListNode(v % 10);
            tail = tail->next;
        }

        return dummy.next;
    }
};

void printList(ListNode *head)
{
    ListNode *curr = head;
    while (curr != nullptr)
    {
        std::cout << curr->val;
        if (curr->next != NULL)
        {
            std::cout << " -> ";
        }
        curr = curr->next;
    }
    std::cout << std::endl;
}

int main()
{
    ListNode *num1 = new ListNode(1);
    num1->next = new ListNode(2);
    num1->next->next = new ListNode(3);

    ListNode *num2 = new ListNode(9);
    num2->next = new ListNode(9);
    num2->next->next = new ListNode(9);

    Solution sol;

    ListNode *sum = sol.addTwoNumbers(num1, num2);
    printList(sum);

    return 0;
}
