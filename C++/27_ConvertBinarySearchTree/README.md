# 面试题27：二叉搜索树与双向链表

> 题目： 输入一棵二叉搜索树，将该二叉搜索树转换成一个排序的双向链表。要求不能创建任何新的结点，只能调整书中结点指针的指向。比如二叉搜索树：

```
     10
   /    \
  6      14
 / \    /  \
4   8  12  16
```

转换为双向链表：

```
4 ⇆ 6 ⇆ 8 ⇆ 10 ⇆ 12 ⇆ 14 ⇆ 16
```

> 二叉树结点的定义如下：

```c++
struct BinaryTreeNode
{
    int                    m_nValue;
    BinaryTreeNode*        m_pLeft;
    BinaryTreeNode*        m_pRight;
};
```
