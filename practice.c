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
    ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) {
    if(pListHead==NULL){
      return NULL;
    
    }
    int count=0;
    ListNode* p1=pListHead;
     while(p1!=NULL){
            count++;
            p1=p1->next;
        }
        count-=k;
        p1=pListHead;
        if(count>=0){
          while(count--){
          //pListHead=pListHead->next;
          p1=p1->next;
          }
          return p1;
        }else
            return NULL;
    }
};
