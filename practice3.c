//输入一个复杂链表（每个节点中有节点值，以及两个指针，一个指向下一个节点，另一个特殊指针指向任意一个节点），
//返回结果为复制后复杂链表的head。（注意，输出结果中请不要返回参数中的节点引用，否则判题程序会直接返回空）

/*
struct RandomListNode {
    int label;
    struct RandomListNode *next, *random;
    RandomListNode(int x) :
            label(x), next(NULL), random(NULL) {
    }
};
*/
//从头遍历链表，对每一个节点复制一个，插在它后边；接着遍历这个链表，
//那么原链表中节点的特殊指针若为空，则复制链表对应节点的特殊指针也为空，
//若源链表中结点的特殊指针不为空，那复制链表中对应节点的特殊指针为原链表中结点特殊指针的->next。
class Solution {
public:
    RandomListNode* Clone(RandomListNode* pHead)
    {
        if(pHead==NULL){
            return NULL;
        }
        
        RandomListNode* ptr=pHead;
        while(ptr!=NULL)
        {
         RandomListNode* tmp=new RandomListNode(ptr->label);
         tmp->next=ptr->next;
         ptr->next=tmp;
         ptr=tmp->next;
        }
         ptr = pHead;
        while (ptr != NULL) {
            if (ptr->random != NULL) {
            ptr->next->random = ptr->random->next;
        }
        ptr = ptr->next->next;
        }
        
    RandomListNode* head = pHead->next;
    RandomListNode* q = head;
    ptr = pHead;
    //这个部分需要注意一下
    while (q->next != NULL) {
        ptr->next = q->next;
        ptr = ptr->next;
        
        q->next = ptr->next;
        q = q->next;
    }
    ptr->next = NULL;  //最后将原来链表的尾部设置为null
    return head;
    }
};
