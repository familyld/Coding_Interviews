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

## 面试题21：包含min函数的栈

### 题目

> 定义栈的数据结构，请在该类型中实现一个能够得到栈的最小元素的min函数。在该栈中，调用min、push及pop的时间复杂度都是O(1)。

### 解析

要实现带min函数的栈，难度不仅在于每次入栈，最小元素都会变动；还在于每次出栈一个元素，最小元素也会变动。如果我们只用一个int类型来记录最小元素，那么出栈时我们就不得不把所有剩余元素都检查一遍才能更新最小元素。

更好的方法是在数据栈之外再维护一个辅助栈。每次把新元素压入数据栈，如果新元素比辅助栈的栈顶（当前最小元素）小，就把新元素也压入辅助栈；如果新元素较大，就把辅助栈栈顶元素再一次压入辅助栈。

出栈时，同时把数据栈和辅助栈的栈顶出栈即可。按照前面入栈的操作，此时辅助栈栈顶的元素必然也是数据栈中最小的元素。

要获取最小元素的值只需要访问辅助栈的栈顶就可以了。

以上三个操作的时间复杂度都是O(1)，符合题目要求。还有一个小细节需要注意一下，就是在调用pop和min时应检查一下栈是否非空。

```c++
template <typename T> class StackWithMin
{
public:
    StackWithMin(void) {}
    virtual ~StackWithMin(void) {}

    T& top(void);
    const T& top(void) const;

    void push(const T& value);
    void pop(void);

    const T& min(void) const;

    bool empty() const;
    size_t size() const;

private:
    std::stack<T>   m_data;     // 数据栈，存放栈的所有元素
    std::stack<T>   m_min;      // 辅助栈，存放栈的最小元素
};

template <typename T> void StackWithMin<T>::push(const T& value)
{
    // 把新元素添加到辅助栈
    m_data.push(value);

    // 当辅助栈为空或者新元素比之前的最小元素小时，把新元素插入辅助栈里；
    // 否则把之前的最小元素重复插入辅助栈里
    if(m_min.size() == 0 || value < m_min.top())
        m_min.push(value);
    else
        m_min.push(m_min.top());
}

template <typename T> void StackWithMin<T>::pop()
{
    assert(m_data.size() > 0 && m_min.size() > 0);

    m_data.pop();
    m_min.pop();
}


template <typename T> const T& StackWithMin<T>::min() const
{
    assert(m_data.size() > 0 && m_min.size() > 0);

    return m_min.top();
}

template <typename T> T& StackWithMin<T>::top()
{
    return m_data.top();
}

template <typename T> const T& StackWithMin<T>::top() const
{
    return m_data.top();
}

template <typename T> bool StackWithMin<T>::empty() const
{
    return m_data.empty();
}

template <typename T> size_t StackWithMin<T>::size() const
{
    return m_data.size();
}
```

## 面试题22：栈的压入、弹出序列

### 题目

> 输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否为该栈的弹出顺序。假设压入栈的数字均不想等。例如序列 `1、2、3、4、5` 是某栈的压栈序列，序列 `4、5、3、2、1` 是该压栈序列对应的一个弹出序列，但 `4、3、5、1、2` 就不可能是该压栈序列的弹出序列。

### 解析

要实现题目要求，其实思路还是蛮清晰的。我们每次检查弹出序列的头部元素，如果此时栈内没有该元素，就按照压栈序列的顺序将数字压入辅助栈，直到辅助栈的栈顶和弹出序列的头部元素相同。这时可以从辅助栈弹出一个元素，而弹出序列的头部也往后移动一位，继续下一次检查。

要注意循环的终止条件和匹配成功的条件。当匹配完弹出序列最后一位时顺利退出，但此时还应检查压栈序列的数是否已全部用完，否则仍然不算成功匹配；另外，在压栈的过程中，有可能压入了所有数字后仍然无法匹配到弹出序列，此时也应退出循环。

```c++
bool IsPopOrder(const int* pPush, const int* pPop, int nLength)
{
    bool bPossible = false;

    if(pPush != NULL && pPop != NULL && nLength > 0)
    {
        const int* pNextPush = pPush;
        const int* pNextPop = pPop;

        std::stack<int> stackData;

        // 全部pop完就停止
        while(pNextPop - pPop < nLength)
        {
            // 当辅助栈的栈顶元素不是要弹出的元素
            // 先压入一些数字入栈
            while(stackData.empty() || stackData.top() != *pNextPop)
            {
                // 如果所有数字都压入辅助栈了，退出循环
                if(pNextPush - pPush == nLength)
                    break;

                stackData.push(*pNextPush);

                pNextPush ++;
            }

            // 所有数字都已压入辅助栈却无法与弹出序列匹配就直接退出
            if(stackData.top() != *pNextPop)
                break;

            stackData.pop();
            pNextPop ++;
        }

        // 辅助栈空了，并且弹出序列也匹配到了最后，就说明匹配成功
        if(stackData.empty() && pNextPop - pPop == nLength)
            bPossible = true;
    }

    return bPossible;
}
```

