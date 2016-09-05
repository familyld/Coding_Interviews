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

## 面试题40：数组中只出现一次的数字

### 题目

> 一个整型数组里除了两个数字之外，其他的数字都出现了两次。请写程序找出这两个只出现一次的数字。要求时间复杂度是O(n)，空间复杂度是O(1)。

### 解析

第一次看这条题，一点思路都没有，看书中给出的解法时真的被惊艳到了。

首先，我们要明白题目所说的**只出现一次到底意味着什么**？如果我们把题目换成只有一个出现一次的数字，可以怎么找出来呢？

这里利用的是**异或的特性**，**两个相同的数异或结果为0，两个不同的数异或结果的二进制表示至少有一位为1**。举个例子，比方说要在数组 `20,10,20,30,30` 中找出只出现一次的10，那么我们只需要求整个数组异或的结果就可以了：

```c++
20 xor 10 xor 20 xor 30 xor 30 = 10
```

结果就是只出现一次的数字。

但数组中有两个只出现一次的数字，这要怎么找呢？

1. 首先对整个数组进行异或，所得结果就是那两个只出现一次的数的异或结果，因为这两个数字是不同的，所以结果的二进制表示至少有一位为1；
2. 从异或结果的二进制表示中找到从右往左第一个为1的位置，它表明**两个只出现一次的数字的二进制表示在这一位上是不同的**；
3. 按照步骤2中第一个为1的位置将数组分为两部分，一部分在该位置为1，另一部分在该位置为0；
4. 因为同一个数字的二进制表示也是相同的，所以步骤3划分的数组中，同一个数字会被分到同一边，也即划分后，**问题变成数组中只有一个出现一次的数字，其他数字都出现两次，如何找出那个只出现一次的数字**。
5. 分别对两个数组求异或结果，得到问题的解。

```c++
void FindNumsAppearOnce(int data[], int length, int* num1, int* num2)
{
    if (data == NULL || length < 2)
        return;

    // 对整个数组求异或结果
    int resultExclusiveOR = 0;
    for (int i = 0; i < length; ++ i)
        resultExclusiveOR ^= data[i];

    // 找出异或结果二进制表示中第一个1出现的位置
    unsigned int indexOf1 = FindFirstBitIs1(resultExclusiveOR);

    // 按照indexOf1划分数组并分别求异或结果（也即两个只出现一次的数字）
    *num1 = *num2 = 0;
    for (int j = 0; j < length; ++ j)
    {
        if(IsBit1(data[j], indexOf1))
            *num1 ^= data[j];
        else
            *num2 ^= data[j];
    }
}

// 找到num从右边数起第一个是1的位
unsigned int FindFirstBitIs1(int num)
{
    int indexBit = 0;
    // 数字的二进制表示最右边为0，且当前位数不超过int型表示的范围（8*4bytes = 32bits）
    while (((num & 1) == 0) && (indexBit < 8 * sizeof(int)))
    {
        num = num >> 1; //右移一位
        ++ indexBit;
    }

    return indexBit;
}

// 判断数字num的第indexBit位是不是1
bool IsBit1(int num, unsigned int indexBit)
{
    num = num >> indexBit;
    return (num & 1);
}
```

## 面试题41_1：和为s的两个数字

### 题目

> 输入一个递增排序的数组和一个数字s，在数组中查找两个数，使得它们的和正好是s。如果有多对数字的和等于s，输出任意一对即可。

### 解析

因为可以输出任意一对，所以只要找到一对就可以了。又因为这是一个递增数组，所以我们只需要维护两个指针behind和ahead，前者从数组开头（最小数字）开始往后扫描，后者从数组末尾（最大数字）开始往前扫描。每一次我们比较两个指针所指数字之和与s的大小关系，如果比s大，我们就往前移动ahead；如果比s小，我们就往后移动behind；如果和s相等，就进行输出。

这个算法的时间复杂度是O(n)，当ahead与behind相遇时，搜索就结束了，最多也只是扫描一次整个数组。如果数组中确实有和为s的一对数字，这个算法是必然能找到的。因为移动指针时，选择的指针和移动的方向都是必然的，并不需要ahead往后或者behind往前，因为外侧的数字都是已经排除掉，不可能构成指定的和的，这需要自己分析理解一下。

还是不偷懒了，简单举个例子吧.. 比方说有数组 `1,2,4,7,11,15`，指定s为15。

1. `1+15=16 > 15`，往前移动ahead，这很好理解，因为behind已经在最前了，自然不可能再往前移动它来减少和值；
2. `1+11=12 < 15`，往后移动behind。为什么不往后移动ahead呢？很简单，因为前面第一步已经验证了`1+15=16 > 15`，正是因为这样才把ahead往前移的，自然就没有理由又把它移回去了；
3. `2+11=13 < 15`，往后移动behind。为什么不往后移动ahead呢？也很简单，因为比2小的1和15加起来都超过s了，那么2加上15的和就更大了，所以不可能往后移动ahead；
4. `4+11=15 = 15`，找到了，完成查找。

```c++
bool FindNumbersWithSum(int data[], int length, int sum,
                        int* num1, int* num2)
{
    bool found = false;
    if(length < 1 || num1 == NULL || num2 == NULL)
        return found;

    int ahead = length - 1;
    int behind = 0;

    while(ahead > behind)
    {
        long long curSum = data[ahead] + data[behind];

        if(curSum == sum)
        {
            *num1 = data[behind];
            *num2 = data[ahead];
            found = true;
            break;
        }
        else if(curSum > sum)
            ahead --;
        else
            behind ++;
    }

    return found;
}
```

## 面试题41_2：和为s的连续正数序列

### 题目

> 输入一个正数s，打印出所有和为s的连续正数序列（至少会有两个数）。例如输入15，由于 `1+2+3+4+5 = 4+5+6 = 7+8 = 15`，所以结果打印出3个连续序列 `1~5`、`4~6`和`7~8`。

### 解析

首先审审题，这里不再给出数组，而是说找出**连续正数数列**，也即从 `1,2,3,4,.....` 这个正数序列中找。而所谓“序列”，指的是至少要包含两个数字。还有一点就是题目要求**打印出所有**符合要求的序列。

因为正数序列延伸到正无穷，我们自然没有办法说从序列最大值开始减少。我们转换一种思路，维护small和big两个指针来指向当前序列最小和最大的数，从和最小的序列开始找起，也即只包含两个整数的序列 `1,2`（small初始化为1，big初始化为2）。

- 当前序列和小于s时，通过往后移动big来把新的数字补充进序列，使得序列的和能更接近s。
- 当前序列和大于s时，通过往后移动small来把小的数字丢掉，使得序列的和能更接近s。
- 当前序列和为s时，打印序列，并往后移动big来把新的数字补充进序列来打破平衡，使得small可以往后移动，从而有机会找到符合要求的新序列。

注意了，什么时候停止移动呢？我们不难发现，当small大于 `(1 + s) / 2` 之后，任意两个数和都必定大于s，所以终止条件就是small大于等于 `(1 + s) / 2`。

还有一个小trick，计算序列和是一件挺麻烦的事（虽然可以用公式，但是乘除法还是开销蛮大的）。这里我们不需要这样做，因为新序列和旧序列只是相差一个元素而已。我们维护一个变量curSum，在删除/补充数字时，把curSum更新一下就可以了。

```c++
void FindContinuousSequence(int sum)
{
    if(sum < 3) // 不存在和小于3的连续正数序列，直接返回
        return;

    int small = 1;
    int big = 2;
    int middle = (1 + sum) / 2;
    int curSum = small + big;

    // small超过middle以后就不可能再有符合要求的序列产生了
    // 因为此后的任意两个数和都必定大于sum
    while(small < middle)
    {
        if(curSum == sum)
            PrintContinuousSequence(small, big);

        while(curSum > sum && small < middle)
        {
            curSum -= small;
            small ++;

            if(curSum == sum)
                PrintContinuousSequence(small, big);
        }

        // 加大big有两个意义
        // 一是curSum小于sum时补充序列，使得序列的和能更接近sum
        // 二是curSum等于sum时打破平衡，使得small可以往后移动，找到符合要求的新序列
        big ++;
        curSum += big;
    }
}

void PrintContinuousSequence(int small, int big)
{
    for(int i = small; i <= big; ++ i)
        printf("%d ", i);

    printf("\n");
}
```
