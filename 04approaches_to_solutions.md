# 解决面试题的思路

## 面试题19：二叉树的镜像

### 题目

> 请完成一个函数，输入一个二叉树，该函数输出它的镜像。二叉树结点的定义如下：

```c++
struct BinaryTreeNode
{
    int                    m_nValue;
    BinaryTreeNode*        m_pLeft;
    BinaryTreeNode*        m_pRight;
};
```

### 解析

如果画图分析过，很容易就会发现，其实要得到一棵二叉树的镜像，只需要把每个结点的左右子结点都进行交换就可以了。

在每一轮交换中，我们先记住原来的左子结点，然后把当前结点的左子结点指针改为指向右子结点，最后把当前结点的右子结点指针改为指向原来的左子结点就可以了。

使用递归的话，注意结束条件是到达叶结点。特别地，要记得处理输入为空指针的状况。处理完当前结点后，若左子结点不为NULL，就继续处理左子结点，右子结点同理；

使用迭代的话，其实也很类似，可以用栈来模拟，递归本身就是一种栈结构。

递归解法：

```c++
void MirrorRecursively(BinaryTreeNode *pNode)
{
    if((pNode == NULL) || (pNode->m_pLeft == NULL && pNode->m_pRight))
        return;

    BinaryTreeNode *pTemp = pNode->m_pLeft;
    pNode->m_pLeft = pNode->m_pRight;
    pNode->m_pRight = pTemp;

    if(pNode->m_pLeft)
        MirrorRecursively(pNode->m_pLeft);

    if(pNode->m_pRight)
        MirrorRecursively(pNode->m_pRight);
}
```

迭代解法：

```c++
void MirrorIteratively(BinaryTreeNode* pRoot)
{
    if(pRoot == NULL)
        return;

    std::stack<BinaryTreeNode*> stackTreeNode;
    stackTreeNode.push(pRoot);

    while(stackTreeNode.size() > 0)
    {
        BinaryTreeNode *pNode = stackTreeNode.top();
        stackTreeNode.pop();

        BinaryTreeNode *pTemp = pNode->m_pLeft;
        pNode->m_pLeft = pNode->m_pRight;
        pNode->m_pRight = pTemp;

        if(pNode->m_pLeft)
            stackTreeNode.push(pNode->m_pLeft);

        if(pNode->m_pRight)
            stackTreeNode.push(pNode->m_pRight);
    }
}
```
