# 面试题13：在O(1)时间删除链表结点

> 题目：给定单向链表的头指针和一个结点指针，定义一个函数在O(1)时间删除该结点。链表结点与函数的定义如下：

```c++
struct ListNode
{
    int       m_nValue;
    ListNode* m_pNext;
};
```

```
void DeleteNode(ListNode** pListHead, ListNode* pToBeDeleted);
```
