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

## 面试题39_2：判断是否平衡二叉树

### 题目

> 输入一棵二叉树的根结点，判断该树是不是平衡二叉树。如果某二叉树中任意结点的左右子树的深度相差不超过1，那么它就是一棵平衡二叉树。

### 解析

有了上一题的思路，我们很容易想到这一题可以逐个结点来判断是否平衡二叉树，也即对每个结点都检查左右子树的深度是否相差1，但是这个做法并不够好，因为它需要多次遍历同一结点，比如二叉树：

```c++
      10
     /  \
    7    8
   /    / \
  5    4   3
 /
2
```

我们在检查根结点10时，需要计算以7和8为根结点的左右子树的深度，按照递归的方式，我们需要先计算出以5，4，3为根结点的这几棵子树的深度。由于以7和8为根结点的左右子树的深度不超过1，所以继续检查。这时我们需要根结点7了，此时又需要再计算一次以5为根结点的子树的深度，所以说**产生了重复计算，效率不高**。

这里介绍一种只需要遍历每个结点1次的方法。在上面的方法中，我们是由上到下逐层检查，所以每次都需要对当前层下面的层计算一次。只要我们转换一下思路，**由下往上逐层检查，并且把树深作为参数传递然后累加**，就不需要作重复的计算了，所有结点都只需访问1次。

```c++
bool IsBalanced_Solution2(BinaryTreeNode* pRoot)
{
    int depth = 0;
    return IsBalanced(pRoot, &depth);
}

bool IsBalanced(BinaryTreeNode* pRoot, int* pDepth)
{
    if(pRoot == NULL)
    {
        *pDepth = 0;
        return true;
    }

    int left, right;
    if(IsBalanced(pRoot->m_pLeft, &left)
        && IsBalanced(pRoot->m_pRight, &right))
    {
        int diff = left - right;
        if(diff <= 1 && diff >= -1)
        {
            *pDepth = 1 + (left > right ? left : right);
            return true;
        }
    }

    return false;
}
```
