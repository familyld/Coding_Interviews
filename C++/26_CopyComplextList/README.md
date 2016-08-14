# 面试题26：复杂链表的复制

> 题目：请实现函数 `ComplexListNode* Clone(ComplexListNode* pHead)`，复制一个复杂链表。在复杂链表中，每个结点除了有一个 `m_pNext` 指针指向下一个结点外，还有一个 `m_pSibling` 指向链表中的任意节点或者NULL。结点的C++定义如下：

```c++
struct ComplexListNode
{
    int                 m_nValue;
    ComplexListNode*    m_pNext;
    ComplexListNode*    m_pSibling;
};
```
