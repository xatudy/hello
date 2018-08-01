//输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。
//递归实现
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
         
       ListHode* head=NULL;
       if(pHead1->val<=pHead2->val)
          head=pHead1;
          Merge(pHead1->next,pHead2);
       else
          head=pHead2;
          Merge(pHead1,pHead2->next);
       return head;
  };
