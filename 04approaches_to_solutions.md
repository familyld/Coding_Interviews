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

## 面试题20：顺时针打印矩阵

### 题目

> 输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字。例如：如果输入如下矩阵

```
1   2   3   4
5   6   7   8
9   10  11  12
13  14  15  16
```

> 则依次打印出数字 `1、2、3、4、8、12、16、15、14、13、9、5、6、7、11、10`。

### 解析

题目要求从外向里顺时针打印一个矩阵，其实可以把这个矩阵从外向里分为若干层，每一层从左上角开始（比如上面例子中的1和6）。这样题目的要求就变为依次顺时针打印每一层的数字。我们会发现，**每一层左上角的行坐标和列坐标都是相等的**。

那么我们可以用一个循环来打印出所有的层，但是到底有多少层呢？或者会说，每一层左上角的坐标 `(start, start)` 最大可以是多少呢？通过举例分析，我们会发现，**给定任意一个矩阵，层数是行数的一半和列数的一半这两者中的较小者**。具体来说，写为代码就是 `columns > start * 2 && rows > start * 2`（注意坐标从0开始），不满足这个条件时退出循环即可。

```c++
void PrintMatrixClockwisely(int** numbers, int columns, int rows)
{
    if(numbers == NULL || columns <= 0 || rows <= 0)
        return;

    int start = 0;

    while(columns > start * 2 && rows > start * 2)
    {
        PrintMatrixInCircle(numbers, columns, rows, start);

        ++start;
    }
}
```

有了某一层左上角的坐标之后，怎么顺时针打印这一层的数字呢？可以分为四个操作。

1. 从左到右打印一行
2. 从上到下打印一列
3. 从右到左打印一行
4. 从下到上打印一列

但是，必须注意到，**不是所有情况下都能完整地进行这四个操作的**。当行列数不同时，**最内层的矩阵有可能退化为只有一行/一列甚至只有一个数字**。那么进行每个操作的前提条件是什么呢？

操作1是必然会进行的，如果只有一行或一个数字，那么操作1结束后其他操作就不需要了；

进行操作2的前提是终止行号大于起始行号，即至少要有两行；

进行操作3的前提是至少有两行两列。只有1列的话，完成操作1和2就打印完了；

进行操作4的前提是至少有三行两列；

```c++
void PrintMatrixInCircle(int** numbers, int columns, int rows, int start)
{
    int endX = columns - 1 - start; // 所在层的最大横坐标
    int endY = rows - 1 - start;    // 所在列的最大纵坐标

    // 从左到右打印一行
    for(int i = start; i <= endX; ++i)
    {
        int number = numbers[start][i];
        printNumber(number);
    }

    // 从上到下打印一列
    if(start < endY)
    {
        for(int i = start + 1; i <= endY; ++i)
        {
            int number = numbers[i][endX];
            printNumber(number);
        }
    }

    // 从右到左打印一行
    if(start < endX && start < endY)
    {
        for(int i = endX - 1; i >= start; --i)
        {
            int number = numbers[endY][i];
            printNumber(number);
        }
    }

    // 从下到上打印一列
    if(start < endX && start < endY - 1)
    {
        for(int i = endY - 1; i >= start + 1; --i)
        {
            int number = numbers[i][start];
            printNumber(number);
        }
    }
}

void printNumber(int number)
{
    printf("%d\t", number);
}
```
