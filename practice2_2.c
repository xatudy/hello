输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。
//非递归实现

/*
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};*/
class Solution {
public:
    ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
    {
        if(pHead1==NULL)
            return pHead2;
        if(pHead2==NULL)
            return pHead1;
        ListNode* newlist=new ListNode(0);
        ListNode* head=newlist;
        while(pHead1&&pHead2)
        {
            if(pHead1->val<=pHead2->val)
            {
                head->next=pHead1;
                pHead1=pHead1->next;
                head=head->next;
            }
            else
            {
                head->next=pHead2;
                pHead2=pHead2->next;
                head=head->next;
            }
        }
        if(pHead1!=NULL)
            head->next=pHead1;
        if(pHead2!=NULL)
            head->next=pHead2;
        
        return newlist->next;
    }
};
