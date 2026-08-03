#include <unordered_set>
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
    // ListNode *deleteDuplicates(ListNode *head)
    // {
    //     // Unorder map to track unique nodes values
    //     std::unordered_set<int> st;

    //     // Initialize pointers for traversing the original list
    //     // and building the new list without duplicates
    //     ListNode *new_head = nullptr;
    //     ListNode *tail = nullptr;

    //     // Traverse the list
    //     ListNode *current = head;
    //     while (current != nullptr)
    //     {
    //         // Check if the current node's data is not in the map
    //         if (st.find(current->val) == st.end())
    //         {
    //             // Create a new node for the unique data
    //             ListNode *new_node = new ListNode(current->val);

    //             // If new_head is null, this is the
    //             // first unique node
    //             if (new_head == nullptr)
    //             {
    //                 new_head = new_node;
    //                 tail = new_head;
    //             }
    //             else
    //             {
    //                 // Append the new node to the end
    //                 // of the new list
    //                 tail->next = new_node;
    //                 tail = new_node;
    //             }

    //             // Mark this data as encountered
    //             st.insert(current->val);
    //         }

    //         // Move to the next node in the original list
    //         current = current->next;
    //     }

    //     // delete old list
    //     current = head;
    //     while (current != nullptr)
    //     {
    //         ListNode *temp = current;
    //         current = current->next;
    //         delete temp;
    //     }

    //     // Return the head of the new list with
    //     // duplicates removed
    //     return new_head;
    // }

    ListNode *deleteDuplicates(ListNode *head)
    {
        ListNode *curr = head;

        // Traverse the list
        while (curr != nullptr && curr->next != nullptr)
        {
            // Check if next value is same as current, delete the duplicate then move on to the next.
            if (curr->val == curr->next->val)
            {
                ListNode *temp = curr->next;
                curr->next = curr->next->next;
                delete temp;
            }
            else
            {
                curr = curr->next;
            }
        }

        return head;
    }
};

// void printList(ListNode *node)
// {
//     while (node != nullptr)
//     {
//         std::cout << node->val << " ";
//         node = node->next;
//     }
//     std::cout << std::endl;
// }

int main()
{

    // Create a sorted linked list
    // 11->11->11->13->13->20
    ListNode *head = new ListNode(11);
    head->next = new ListNode(11);
    head->next->next = new ListNode(11);
    head->next->next->next = new ListNode(13);
    head->next->next->next->next = new ListNode(13);
    head->next->next->next->next->next = new ListNode(20);

    // Original list
    ListNode *temp = head;
    while (temp != NULL)
    {
        std::cout << temp->val << " ";
        temp = temp->next;
    }
    std::cout << std::endl;

    Solution sol;
    head = sol.deleteDuplicates(head);

    // list withn no duplicates
    temp = head;
    while (temp != NULL)
    {
        std::cout << temp->val << " ";
        temp = temp->next;
    }

    std::cout << std::endl;

    return 0;
}