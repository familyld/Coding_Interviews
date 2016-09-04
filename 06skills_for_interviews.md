# 面试中的各项能力

## 面试题38：数字在排序数组中出现的次数

### 题目

> 统计一个数字在排序数组中出现的次数。例如输入排序数组 `{1,2,3,3,3,3,4,5}` 和数字3，由于3在这个数组中出现了4次，因此输出4。

### 解析

最简单的方法就是顺序查找，然后计数了，这种方法的时间复杂度是O(n)。但是既然题目给定的是一个排序数组，那么就要充分利用好这个优势，我们可以基于**二分查找**来做。

具体来说我们使用二分查找的方式来分别找到输入数字第一次出现的索引和最后一次出现的索引，然后继续差值就是了。注意数字可能根本没有出现在数组中，这时应返回0。

实现很简单，修改一下递归二分查找的判定条件就可以了。

```c++
int GetNumberOfK(int* data, int length, int k)
{
    int number = 0;

    if(data != NULL && length > 0)
    {
        int first = GetFirstK(data, length, k, 0, length - 1);
        int last = GetLastK(data, length, k, 0, length - 1);

        if(first > -1 && last > -1)
            number = last - first + 1;
    }

    return number;
}

// 找到数组中第一个k的下标。如果数组中不存在k，返回-1
int GetFirstK(int* data, int length, int k, int start, int end)
{
    if(start > end)
        return -1;

    int middleIndex = (start + end) / 2;
    int middleData = data[middleIndex];

    if(middleData == k)
    {
        if((middleIndex > 0 && data[middleIndex - 1] != k)
            || middleIndex == 0) // 前面没有k了，或者当前索引为0没有更前面了
            return middleIndex;
        else // 前面还有k，继续搜索前半段
            end  = middleIndex - 1;
    }
    else if(middleData > k) // 中位数大于k，继续搜索前半段
        end = middleIndex - 1;
    else                    // 中位数小于k，搜索后半段
        start = middleIndex + 1;

    return GetFirstK(data, length, k, start, end);
}

// 找到数组中最后一个k的下标。如果数组中不存在k，返回-1
int GetLastK(int* data, int length, int k, int start, int end)
{
    if(start > end)
        return -1;

    int middleIndex = (start + end) / 2;
    int middleData = data[middleIndex];

    if(middleData == k)
    {
        if((middleIndex < length - 1 && data[middleIndex + 1] != k)
            || middleIndex == length - 1) // 后面没有k了，或者当前索引已是最后
            return middleIndex;
        else // 后面还有k，继续搜索后半段
            start  = middleIndex + 1;
    }
    else if(middleData < k) // 中位数小于k，继续搜索后半段
        start = middleIndex + 1;
    else                    // 中位数大于k，搜索前半段
        end = middleIndex - 1;

    return GetLastK(data, length, k, start, end);
}
```

## 面试题39_1：二叉树的深度

### 题目

> 输入一棵二叉树的根节点，求该树的深度。从根结点到叶结点依次经过的结点（含根、叶结点）形成树的一条路径，最长路径的长度为树的深度。二叉树结点的定义如下：

```c++
struct BinaryTreeNode
{
    int                    m_nValue;
    BinaryTreeNode*        m_pLeft;
    BinaryTreeNode*        m_pRight;
};
```

### 解析

第一次看到这题，可能还有点迷茫，需要所有路径都走一次然后取最长路径的长度吗？并不是这样的，其实这题很简单，我们可以从另一个角度来理解树的深度：

- 如果树只有一个根结点，则深度为1；
- 如果根结点只有左子树，则深度为左子树深度+1；
- 如果根结点只有右子树，则深度为右子树深度+1；
- 如果根结点既有左子树，又有右子树，则深度为左右子树深度的较大值+1；

就是这么四种情况，我们可以非常容易地写出递归实现：

```c++
int TreeDepth(BinaryTreeNode* pRoot)
{
    if(pRoot == NULL)
        return 0;

    int nLeft = TreeDepth(pRoot->m_pLeft);
    int nRight = TreeDepth(pRoot->m_pRight);

    return (nLeft > nRight) ? (nLeft + 1) : (nRight + 1);
}
```
