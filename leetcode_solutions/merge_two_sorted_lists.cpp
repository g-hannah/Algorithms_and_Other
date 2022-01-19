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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2)
    {
        if (nullptr == list1 && nullptr == list2)
            return nullptr;
        
        if (nullptr == list1 && list2)
            return list2;
        
        if (nullptr == list2 && list1)
            return list1;
        
        ListNode *current, *other, *tmp, *start;
        
        if (list2->val < list1->val)
        {
            start = list2;
        }
        else
        {
            current = tmp = start = list1;
            other = list2;
        }
        
        while (current->next)
        {
            if (other->val < current->next->val)
            {
                tmp = current->next;
                current->next = other;
                current = other;
                other = tmp;
                tmp = current;
            }
            else
            {
                tmp = current = current->next;
            }
        }
        
        current->next = other;
        
        return start;
    }
};
