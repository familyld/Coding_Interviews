# 面试题23：从上往下打印二叉树

> 题目：从上往下打印出二叉树的每个结点，同一层的结点按照从左到右的顺序打印。例如输入下面的二叉树，则依次打印出 `8、6、10、5、7、9、11`。

```
      8
    /   \
   6     10
  / \    / \
 5   7  9  11
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
