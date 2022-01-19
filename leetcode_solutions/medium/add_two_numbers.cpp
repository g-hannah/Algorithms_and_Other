/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution
{
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
    {
        int val = 0, carry = 0;
        ListNode *node = new ListNode(), *tmp;

        for (tmp = node; l1 && l2; )
        {
            val = l1->val + l2->val + carry;
            carry = val / 10;
            tmp->val = val % 10;

            l1 = l1->next;
            l2 = l2->next;

            if (l1 && l2)
                tmp->next = new ListNode(), tmp = tmp->next;
        }

        while (l1)
        {
            tmp->next = new ListNode(), tmp = tmp->next;
            val = l1->val + carry;
            carry = val / 10;
            tmp->val = val % 10;
            l1 = l1->next;
        }

        while (l2)
        {
            tmp->next = new ListNode(), tmp = tmp->next;
            val = l2->val + carry;
            carry = val / 10;
            tmp->val = val % 10;
            l2 = l2->next;
        }
        
        if (carry)
            tmp->next = new ListNode(carry);

        return node;
    }
};
