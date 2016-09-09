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

## 面试题42_1：翻转单词顺序

### 题目

> 输入一个英文句子，翻转句子中单词的顺序，但单词内字符的顺序不变。为简单起见，标点符号和普通字母一样处理。例如输入字符串 `"I am a student."`，则输出 `"student. a am I"`。

### 解析

这条题我们可以分为两步处理：

1. 翻转整个句子
2. 以空格和结束符 `'\0'` 划分，逐个单词翻转

写成代码实现：

```c++
// 翻转两个字符指针之间的内容
void Reverse(char *pBegin, char *pEnd)
{
    if(pBegin == NULL || pEnd == NULL)
        return;

    while(pBegin < pEnd)
    {
        char temp = *pBegin;
        *pBegin = *pEnd;
        *pEnd = temp;

        pBegin ++, pEnd --;
    }
}

char* ReverseSentence(char *pData)
{
    if(pData == NULL)
        return NULL;

    char *pBegin = pData;

    char *pEnd = pData;
    while(*pEnd != '\0')
        pEnd ++;
    pEnd--;

    // 翻转整个句子
    Reverse(pBegin, pEnd);

    // 翻转句子中的每个单词
    pBegin = pEnd = pData;
    while(*pBegin != '\0')
    {
        if(*pBegin == ' ') // 处理连续空格，末字符为空格的情形
        {
            pBegin ++;
            pEnd ++;
        }
        else if(*pEnd == ' ' || *pEnd == '\0') // 找到了划分单词的地方
        {
            Reverse(pBegin, --pEnd); // 翻转整个单词
            pBegin = ++pEnd;         // 令pBegin跳到下一个单词的开头
        }
        else
        {
            pEnd ++; // 单词未结束，继续找
        }
    }

    return pData;
}
```

## 面试题42_2：左旋转字符串

### 题目

> 字符串的左旋转操作是把字符串前面的若干个字符转移到字符串的尾部。请定义一个函数实现字符串左旋转操作的功能。比如输入字符串 `"abcdefg"` 和数字2，该函数将返回左旋转2位得到的结果 `"cdefgab"`。

### 解析

这题很有意思，我们先再分析一下上一题翻转单词顺序，比方说有一个句子 `Hello world`，上一题就是把这个句子处理为 `world Hello`。不看空格的话，这实际上可以看作是把一部分字符转移到字符串的尾部，也即对这个字符串左旋了5位。

有了这样的启发，不难发现，我们可以先按左旋的位数把数组分割为两部分，然后分别翻转这两部分，最后翻转整个数组就完成左旋转这个操作了。

书中给出的代码是按照这样的思路实现的，和上一题有一点点差别，因为上一题是先翻转整个字符串，再分别翻转各个部分。这个顺序调转一下其实也是可以的，这里先翻转部分，再翻转整体的原因是先翻转整体之后，左旋位数所指的位置就不是划分两个部分的地方了。

还是举个例子吧~避免自己再看到的时候会懵圈。还是 `helloworld`，左旋2位，那么划分两部分的位置就在e和l之间，正确答案是 `lloworldhe`。

- 如果先翻转整个字符串，字符串变为 `dlrowolleh`，2指向的地方就变成了l和r之间了，根据这个划分来翻转部分得到的是错误答案 `ldhellowor`。当然，其实真要算出正确的划分位置也不难..就是字符串长度10-左旋位数2=8嘛..可以解出答案 `lloworldhe`，所以还是可以正确划分的，改改就好了。


- 如果先翻转部分，按左旋位数2划分出两个部分，分别翻转后，字符串变为 `ehdlrowoll`，这时再翻转整个字符串，就得到了正确答案 `lloworldhe`。

```c++
// 翻转两个字符指针之间的内容
void Reverse(char *pBegin, char *pEnd)
{
    if(pBegin == NULL || pEnd == NULL)
        return;

    while(pBegin < pEnd)
    {
        char temp = *pBegin;
        *pBegin = *pEnd;
        *pEnd = temp;

        pBegin ++, pEnd --;
    }
}

char* LeftRotateString(char* pStr, int n)
{
    if(pStr != NULL)
    {
        int nLength = static_cast<int>(strlen(pStr));
        if(nLength > 0 && n > 0 && n < nLength)
        {
            char* pFirstStart = pStr;
            char* pFirstEnd = pStr + n - 1;
            char* pSecondStart = pStr + n;
            char* pSecondEnd = pStr + nLength - 1;

            // 翻转字符串的前面n个字符
            Reverse(pFirstStart, pFirstEnd);
            // 翻转字符串的后面部分
            Reverse(pSecondStart, pSecondEnd);
            // 翻转整个字符串
            Reverse(pFirstStart, pSecondEnd);
        }
    }

    return pStr;
}
```

## 面试题43：n个骰子的点数

### 题目

> 把n个骰子扔在地上，所有骰子朝上一面的点数之和为s。输入n，打印出s的所有可能的值出现的概率。

### 解析

n个骰子朝上一面的点数之和最小是n，最大是6n，一共有6n-n+1种可能。而n个骰子中每个骰子在一次投掷中可能的取值都为1~6，因此将它们的点数排成序列的话有6^n种不同的排列组合。

最简单的做法是使用递归来把这6^n种排列组合都试一次，每次排列完毕得到和值就在计数数组中为这个和值对应的计数加一。最后把计数数组每个计数除以6^n就得到出现的概率了。

上面这种做法的代码实现比较简单，但是当n变大时，递归实现的效率会变得相当低。**因为这种做法其实有很多重复的计算**，怎么理解呢？举个例子，假设加入一个新骰子时，我们想计算得到和为s的次数，它实际上等于和为s-1，s-2，s-3，s-4，s-5，s-6的次数的和，因为新骰子为和值贡献的只可能是1~6中的一个。如果按照递归的思路，我们要从第一个骰子开始算；但如果我们能保证和值生成的顺序就能有效地利用之前计算出的结果，从而把重复计算节省下来。

具体来说，代码实现的时候还是有不少需要注意的地方。可以简单分为以下几点：

1. **新加入骰子能产生的和值和未加入之前可能会有重叠**，比方说一个骰子时和值可以取2，两个骰子时和值也可以取2，那么怎样去更新和值的计数呢？要注意！新加入骰子后和值为3是基于未加入前和值为1和2的计数来计算的，如果我们把和值为2的计数更新为加入新骰子的情况，然后再计算和值为3的计数就会得到了错误的结果；

2. 计数数组需要初始化，只有第一个骰子加入后才可以使用上面提到的形式来计算和值的出现次数；

3. 并不是每一个和值s都可以分拆为s-1，s-2，s-3，s-4，s-5，s-6这六项，比如和值为3就只可分拆为和值为2（即s-1）以及1（即s-2）这两项；

4. 新加入骰子可能会令和值的出现次数为0，比方说加入第二个骰子时，和的最小值变为2，此时和值1的出现次数就要更新为0了。

这里第1点比较重要，为了不会发生更新覆盖导致错误结果的情况，我们不妨**使用两个数组来存储和值的计数**。可以看代码注释加深理解：

```c++
void PrintProbability_Solution2(int number)
{
    if(number < 1) // 至少也要有1个骰子
        return;

     // 使用两个数组来存储和值的计数
    int* pProbabilities[2];
    pProbabilities[0] = new int[g_maxValue * number + 1];
    pProbabilities[1] = new int[g_maxValue * number + 1];

    // 计数数组初始化为0
    for(int i = 0; i < g_maxValue * number + 1; ++i)
    {
        pProbabilities[0][i] = 0;
        pProbabilities[1][i] = 0;
    }

    int flag = 0; // 用于标志使用哪一个计数数组
    // 加入第一个骰子
    for (int i = 1; i <= g_maxValue; ++i)
        pProbabilities[flag][i] = 1;

    // 依次加入其它骰子，k为当前骰子数目
    for (int k = 2; k <= number; ++k)
    {
        // 加入第k个骰子时，最小值更新为k，所以和值 `0~k-1` 的出现次数变为0
        for(int i = 0; i < k; ++i)
            pProbabilities[1 - flag][i] = 0;

        // 加入第k个骰子时，产生的和值范围是[k, 6*k]，依次计算出它们的出现次数
        for (int i = k; i <= g_maxValue * k; ++i)
        {
            pProbabilities[1 - flag][i] = 0; //初始化为0
            // 分拆和值，注意不是所有和值都能分拆出6项，所以要设置j<=i。
            // 特别注意j=i这种状况，可以举例分析一下
            for(int j = 1; j <= i && j <= g_maxValue; ++j)
                pProbabilities[1 - flag][i] += pProbabilities[flag][i - j];
        }

        // 改变标志位，使加入新骰子后计数不会覆盖未加入前的结果
        flag = 1 - flag;
    }

    double total = pow((double)g_maxValue, number);
    for(int i = number; i <= g_maxValue * number; ++i)
    {
        double ratio = (double)pProbabilities[flag][i] / total;
        printf("%d: %e\n", i, ratio);
    }

    delete[] pProbabilities[0];
    delete[] pProbabilities[1];
}
```

可能有人还会疑惑，使用两个数组来计数，那最后输出那个数组是否就包含了所有和值的计数呢？会不会有遗漏呢？其实，确实是包含了全部的，因为每次用于赋值的那个计数数组，计数都包含了和值0~加入新骰子后的最大和值，没有遗漏。另外，我们最后输出应该用什么标志位也应该很清楚，应该使用对应于最后一次用于赋值的数组的那个标志位。

## 面试题44：扑克牌的顺子

### 题目

> 从扑克牌中随机抽5张牌，判断是不是一个顺子，即这5张牌是不是连续的。2~10为数字本身，A为1，J为11，Q为12，K为13，而大、小王可以看成任意数字。

### 解析

这题还挺有意思的，在转换为计算机语言时，手牌其实就是一个数组，为了方便，不妨把大、小王先看作数字0。要判断是否顺子，我们可以先对数组排序，然后分别统计以下0的数目，以及需要填补的间隔大小。最后判断间隔能否被填补好就可以了。

在数组排序这一步，书中给出的是使用qsort快排函数以及自定义的比较函数实现O(n logn)d的排序，我们也可以基于哈希表来实现O(n)的排序。不过鉴于n的规模并不大，所以这两种排序算法相差不大。基于O(n)的排序可以看一下这篇文章：[特殊的O(n)时间排序[sort ages with hashtable]](http://www.cnblogs.com/hellogiser/p/sort-ages-with-hashtable.html)。其实就是先扫一遍数组用哈希表计数，然后扫一遍哈希表重排数组。

```c++
// 两个参数分别是手牌和手牌数（题目中是5 ）
bool IsContinuous(int* numbers, int length)
{
    if(numbers == NULL || length < 1)
        return false;

    qsort(numbers, length, sizeof(int), compare);

    int numberOfZero = 0; // 大、小王的张数
    int numberOfGap = 0;  // 需要填补的牌数

    // 统计数组中0的个数
    for(int i = 0; i < length && numbers[i] == 0; ++i)
        ++ numberOfZero;

    // 统计数组中的间隔数目
    int small = numberOfZero;
    int big = small + 1;
    while(big < length)
    {
        // 两个数相等，有对子，不可能是顺子
        if(numbers[small] == numbers[big])
            return false;

        // 两个数不等，计算间隔，两张牌相差1也即相邻时间隔为0
        numberOfGap += numbers[big] - numbers[small] - 1;
        // 继续比较下两张牌
        small = big;
        ++big;
    }

    // 若大、小王能把间隔都填补上或者不需要填补间隔就会返回true
    return (numberOfGap > numberOfZero) ? false : true;
}

int compare(const void *arg1, const void *arg2)
{
   return *(int*)arg1 - *(int*)arg2;
}

```

## 面试题45：圆圈中最后剩下的数字

### 题目

> `0,1,...,n-1` 这n个数字排成一个圆圈，从数字0开始每次从这个圆圈里删除第m个数字。求出这个圆圈里剩下的最后一个数字。

### 解析

最简单的做法，用一个环形链表来模拟就好了，每走m-1步删除一个元素，若干轮后就能得到最后一个数字了。在c++中，我们可以用标准库提供的list容器来实现，list是双向循环链表，插入删除等操作都封装好了，使用很方便。

```c++
// ====================方法1====================
int LastRemaining_Solution1(unsigned int n, unsigned int m)
{
    if(n < 1 || m < 1)
        return -1;

    // 初始化环形链表
    unsigned int i = 0;
    list<int> numbers;
    for(i = 0; i < n; ++ i)
        numbers.push_back(i);

    list<int>::iterator current = numbers.begin();
    while(numbers.size() > 1)
    {
        for(int i = 1; i < m; ++ i)
        {// 先走m-1步，找到这一轮的第m个数
            current ++;
            // 如果到底了尾部就返回头部，保证环形链表的遍历顺序
            // 注意list.end()指向的是最后一个元素后面的一个位置
            if(current == numbers.end())
                current = numbers.begin();
        }

        // 记住这一轮第m+1个数的位置
        list<int>::iterator next = ++ current;
        if(next == numbers.end())
            next = numbers.begin();

        // 删除这一轮的第m个数
        -- current;
        numbers.erase(current);
        current = next; // 从第m+1个数开始新的一轮
    }

    return *(current);
}
```

要注意的就是我们使用迭代器来遍历数组，其实就是用一个指针来走list上的结点，走到list.end()就证明已经到达最后了，此时指向的不是最后一个元素（我猜指的应该是NULL），我们需要把迭代器移动回链表头。

另外，在删除时，其实是把指针指向的内存空间释放掉，封装好的erase函数会帮我们把断开的链表接好，但是我们还是需要用一个指针预先记住下一个数的位置，不能删除之后就找不到它了。

假设链表长为n，每m个数删除一个，那么上面的解法复杂度就是O(mn)，因为每删除一个数字都要经历m个操作。而且这种解法还要借助一个O(n)的环形链表来辅助。有没有更好的方法呢？

这里给出一种时间复杂度为O(n)，空间复杂度为O(1)的解法。

---

给定n，把数列 `0,1,2,3,...,n-2,n-1` 这n个数字围成一圈，从0开始走，每次删除第m个数字，求最后剩下的数字。那么第一个被删除的数字就是 `(m-1)%n`。

为了方便，先把数字 `(m-1)%n` 记为k，删除k以后数列剩下n-1个数字。在下一轮中，k+1成为第一个被遍历的数字，k-1则成为最后一个。我们不妨做个映射p：`p(x) = (x-k-1)%n`：

```c++
k+1 -> 0
k+2 -> 1
...
n-1 -> n-k-2
0   -> n-k-1
1   -> n-k
...
k-2 -> n-3
k-1 -> n-2

```

此时，问题变为了： 给定n-1，把数列 `0,1,2,3,...,n-3,n-2` 这n-1个数字围成一圈，从0开始走，每次删除第m个数字，求最后剩下的数字。也即**映射后，问题变为了相同的形式**！这样我们就可以很简单地利用递归或者循环来求解了！

进一步分析一下，假设原问题的解是 `f(n,m)`，映射后问题的解是 `f(n-1,m)`，我们只要**对映射后问题的解进行逆映射就能得到和原文题同样的解**！也即：

```c++
f(n,m) = p^(-1)[f(n-1,m)]
```

其中逆映射 `p^(-1)(x) = (x+k+1)%n`。而且有 `k=(m-1)%n`，因此：

```c++
f(n,m) = p^(-1)[f(n-1,m)]
       = [f(n-1,m)+(m-1)%n+1]%n
       = [f(n-1,m)+m]%n
```

得到通项公式后，我们可以基于循环来实现，注意初项的设置（即n为1时问题的解）：

---

```c++
// ====================方法2====================
int LastRemaining_Solution2(unsigned int n, unsigned int m)
{
    if(n < 1 || m < 1)
        return -1;

    int last = 0; // 当n为1，数列中只有0，问题的解就是0
    // 从长度为1的数列开始算，i=数列长度+1
    for (int i = 2; i <= n; i ++)
        last = (last + m) % i;

    return last;
}
```

## 面试题46：求1+2+...+n

### 题目

> 求 `1+2+...+n`，要求不能使用乘除法、for、while、if、else、switch、case等关键字及条件判断语句（`A?B:C`）。

### 解析

这题目不属于数据结构和算法的范畴，我不是太关心，它更偏向于对C++语言特性的考查。不过其他编程语言也有共性，所以这里还是简单地进行一下解析。

方法一借助构造函数和静态变量求解，把累加逻辑隐藏在构造函数中，然后创建n个实例来达到累加的目的。

```c++
// ====================方法一====================
class Temp
{
public:
    Temp() { ++ N; Sum += N; }

    static void Reset() { N = 0; Sum = 0; }
    static unsigned int GetSum() { return Sum; }

private:
    static unsigned int N;
    static unsigned int Sum;
};

unsigned int Temp::N = 0;
unsigned int Temp::Sum = 0;

unsigned int Sum_Solution1(unsigned int n)
{
    Temp::Reset();

    Temp *a = new Temp[n];
    delete []a;
    a = NULL;

    return Temp::GetSum();
}
```

方法二借助两个虚函数实现递归功能，类A的Sum用于终止条件，类B的Sum用于不断累加和递归：

```c++
// ====================方法二====================
class A;
A* Array[2];

class A
{
public:
    virtual unsigned int Sum (unsigned int n)
    {
        return 0;
    }
};

class B: public A
{
public:
    virtual unsigned int Sum (unsigned int n)
    {
        // n是数值变量，!!n可以把它转换为布尔类型，非零->true，0->false
        // 所以当n未到0时，会调用类B的Sum累加，这就类似递归的逻辑
        // 最后n减至0时调用类A返回0，终止递归。
        return Array[!!n]->Sum(n-1) + n;
    }
};

int Sum_Solution2(int n)
{
    A a;
    B b;
    Array[0] = &a;
    Array[1] = &b;

    int value = Array[1]->Sum(n);

    return value;
}
```

方法三与二的思路是类似的，在纯C环境中没有虚函数，可以用函数指针来模拟：

```c++
// ====================方法三====================
typedef unsigned int (*fun)(unsigned int);

unsigned int Solution3_Teminator(unsigned int n)
{
    return 0;
}

unsigned int Sum_Solution3(unsigned int n)
{
    static fun f[2] = {Solution3_Teminator, Sum_Solution3};
    return n + f[!!n](n - 1);
}
```

方法四也是模拟递归，但借助的是模版类型：

```c++
// ====================方法四====================
template <unsigned int n> struct Sum_Solution4
{
    enum Value { N = Sum_Solution4<n - 1>::N + n};
};

template <> struct Sum_Solution4<1>
{
    enum Value { N = 1};
};

template <> struct Sum_Solution4<0>
{
    enum Value { N = 0};
};
```

## 面试题47：不用加减乘除做加法

### 题目

> 写一个函数，求两个整数之和，要求在函数体内不得使用+、-、×、÷四则运算符号。

### 解析

不能用四则运算符号，那么剩下的选择就是位运算了。**怎么用位运算模拟加法**呢？

我们先不急着分析基于二进制的位运算，从十进制的角度来看，加法可以怎样分解呢？不妨举个例子，计算 `915+87`：

1. 首先不管进位，对915和87逐位相加，百位9+0=9，十位1+8=9，个位5+7=2（不管进位）；
2. 然后我们处理进位1，它由个位产生，我们需要把它**左移**一位，加到十位上，十位9+1=0（不管进位）；
3. 因为还是有进位，所以要继续处理。这次进位1由十位产生，我们同样把它左移一位，加到百位上，百位9+1=0（不管进位）；
4. 因为还是有进位，所以要继续处理。这次进位1由百位产生，我们同样把它左移一位，加到千位上，千位0+1=1；
5. 没有进位，结束运算，得到结果1002。

简单来说，**每一次加法可以分成（1）不管进位相加、（2）获得进位 和 （3）处理进位 这三步**，其中第三步其实又是一次加法，所以其实是重复第一步和第二步，直到第二步获得的进位为0，也即不需再进位时结束。还有注意一点，上面例子中每次加法只有一个进位，实际上可能有多个进位，这时流程也是一样的。

好了，有了以上分析，在使用位运算模拟时怎么做呢？

首先看步骤1，不管进位相加，那么对应二进制就是 `1和0得1`，`0和1得1`，`0和0得0`，`1和1得0`。是的，这一步对应于**异或操作**。

然后看步骤2，怎么获得进位，并且把它们放在正确的位置上呢？其实很简单，因为二进制只有两个数字这一位都是1才会进位，所以我们可以**用位与获得产生进位的位置**，注意了，**产生进位的位置和进位要作用到的位置是不同的**，所以还要对位与结果**左移一位**。

最后我们判断一下步骤2获得的进位是否为0，为0就结束；不为0就继续对步骤1和步骤2的结果做加法，也即继续重复步骤1和步骤2。

写成代码也非常简洁：

```c++
int Add(int num1, int num2)
{
    int sum, carry;
    do
    {
        sum = num1 ^ num2;
        carry = (num1 & num2) << 1;

        num1 = sum;
        num2 = carry;
    }
    while(num2 != 0);

    return num1;
}
```

## 面试题48：不能被继承的类

### 题目

> 用C++设计一个不能被继承的类。

### 解析

这题目也不属于数据结构和算法的范畴，我不是太关心，更偏向于对C++对象设计语法的考查。

方法一把构造函数设为私有函数，当别的类试图继承它时，会自动调用它的构造函数和析构函数，从而导致编译出错。因此这样设计的类是无法被继承的。我们可以通过一个公有函数来获取它的实例。

```c++
// ====================方法一====================
class SealedClass1
{
public:
    static SealedClass1* GetInstance()
    {
        return new SealedClass1();
    }

    static void DeleteInstance( SealedClass1* pInstance)
    {
        delete pInstance;
    }

private:
    SealedClass1() {}
    ~SealedClass1() {}
};
```

方法二利用虚拟继承和友元类型，就不详细解释了。想要深入探究的话自己查找资料就好了~

```c++
// ====================方法二====================
template <typename T> class MakeSealed
{
    friend T;

private:
    MakeSealed() {}
    ~MakeSealed() {}
};

class SealedClass2 : virtual public MakeSealed<SealedClass2>
{
public:
    SealedClass2() {}
    ~SealedClass2() {}
};
```
