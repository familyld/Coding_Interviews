# 英文版新增面试题

## 面试题51：数组中重复的数字

### 题目

> 在一个长度为n的数组里的所有数字都在0到n-1的范围内。数组中某些数字是重复的，但不知道有几个数字重复了，也不知道每个数字重复了几次。请找出数组中任意一个重复的数字。例如，如果输入长度为7的数组 `{2,3,1,0,2,5,3}`，那么对应的输出是重复的数字2或者3。

### 解析

这里介绍一个复杂度O(n)的方法，思想和借助哈希表排序类似，但是由于题目给了非常棒的前提条件，在**长度为n**的数组里的**所有数字都在0到n-1的范围内**，如果没有重复元素，那么必定每个数字都能被放入自己对应的位置上，我们不需要额外的辅助内容。

具体来说，我们依次遍历数组，当数字i不在位置i上时，把它交换到位置i上；如果发现位置i上的数等于数字i，就说明找到重复元素了。

为什么这种方法遍历一次数组必然就能找到重复元素呢？因为如果存在两个重复数字，那么在把前面那一个放到正确位置上后，遍历到后面那一个时会再一次检查到这个位置，此时必然能发现数字的重复。

```c++
bool duplicate(int numbers[], int length, int* duplication)
{
    if(numbers == NULL || length <= 0) // 合法性检验
    {
        return false;
    }

    for(int i = 0; i < length; ++i)    // 合法性检验
    {
        if(numbers[i] < 0 || numbers[i] > length - 1)
            return false;
    }

    for(int i = 0; i < length; ++i)
    {
        while(numbers[i] != i) // 如果数字已经和序号一致就不用处理了
        {
            // 如果发现数字和对应位置上的数相等，就说明已经找到重复元素了
            if(numbers[i] == numbers[numbers[i]])
            {
                *duplication = numbers[i];
                return true;
            }

            // 如果不相等，就把数字交换到它正确的位置上
            int temp = numbers[i];
            numbers[i] = numbers[temp];
            numbers[temp] = temp;
        }
    }

    return false;
}
```

## 面试题52：构建乘积数组

### 题目

> 给定一个数组 `A[0,1,...,n-1]`，请构建一个数组 `B[0,1,...,n-1]`，其中B中的元素 `B[i] = A[0] × A[1] × ... × A[i-1] × A[i+1] × ... × A[n-1]`。不能使用除法。

### 解析

一定要看清楚题目，这题要求数组B的每一个元素B[i]等于数组A除A[i]以外所有元素的积。

不能用除法，但是我们不妨从另外一个角度去想这个问题。**B[i]根据i的位置可以划分为两部分来求解**，它等于数组A中位置i之前的元素之积 * 数组A中位置i之后的元素之积。可以画出下图这样的矩阵：

![matrix](https://github.com/familyld/Coding_Interviews/blob/master/graph/52_ArrayConstruction.jpg?raw=true)

矩阵第i行对应着B[i]乘积的各个因子，其中位置i用1来代替。按照上面说的那样，我们可以把B[i]分成两部分，假设B[i] = C[i] * D[i]。那么C就对应着数组A中位置i之前的元素之积，D就对应着数组A中位置i之后的元素之积（1同时算入C和D中）。

我们可以从上往下计算出C[i]，初始化 `C[0] = 1`，有通项公式 `C[i] = C[i-1] *A[i]`;

我们可以从下往上计算出D[i]，初始化 `D[n-1] = 1`，有通项公式 `D[i] = D[i+1] *A[i+1]`;

写成代码：

```c++
void multiply(const vector<double>& array1, vector<double>& array2)
{
    int length1= array1.size();
    int length2 = array2.size();

    // 只有两数组长度相等，且数组长度大于1才是合法输入
    if(length1 == length2 && length2 > 1)
    {
        array2[0] = 1; // 初始化C[0]
        for(int i = 1; i < length1; ++i)
        {   // 对应从上往下计算出C[i]
            array2[i] = array2[i - 1] * array1[i - 1];
        }

        double temp = 1; // 初始化D[n-1]
        for(int i = length1 - 2; i >= 0; --i)
        {
            temp *= array1[i + 1]; // 对应从下往上计算出D[i]
            array2[i] *= temp;     // 对应B[i] = C[i] * D[i]
        }
    }
}
```

## 面试题53：正则表达式匹配

### 题目

> 请实现一个函数用来匹配包含 `'.'` 和 `'*'` 的正则表达式。模式中的字符 `'.'` 表示任意一个字符，而 `'*'` 表示它前面的字符可以出现任意次（含0次）。在本题中，匹配是指字符串的所有字符匹配整个模式。例如字符串 `"aaa"` 与模式 `"a.a"` 和 `"ab*ac*a"` 匹配，但与 `"aa.a"` 及 `"ab*a"` 均不匹配。

### 解析

这题其实还是蛮好分析的，我们用两个指针分别指向字符串和模式，如果两个指针都走到最后，也即同时遇到结束符 `'\0` 则匹配成功；如果模式用完了，字符串还有未匹配上的部分，就匹配失败；两个指针都未到达最后时，我们需要注意以下 `'.'` 和 `'*'` 的处理。`'.'` 比较简单，因为想匹配什么都可以； `'*'` 的处理就复杂一点，因为我们有多种选择。每一轮匹配的流程可以概括如下：

---

判断模式的下一位字符是否 `'*'` 号：

- 是
    - 当前字符可以匹配
        1. 模式的当前字符仅匹配一次（也即当*号不存在，字符串指针和模式指针各往后移一步）
        2. 模式的当前字符匹配多次（字符串指针往后移，模式指针不动）
        3. 模式的当前字符匹配零次（同时忽略模式当前字符和*号，字符串指针不动，模式指针后移两步）
    - 当前字符不能匹配
        1. 模式的当前字符匹配零次（同时忽略模式当前字符和*号，字符串指针不动，模式指针后移两步）
- 否
    - 当前字符可以匹配
        1. 继续下一次匹配（字符串指针和模式指针各往后移一步）
    - 当前字符不能匹配
        1. 返回匹配失败
---

使用递归的方式来实现即可，特别注意一点，虽然 `'.'` 可以匹配字符串的任意字符，但是结束符 `'\0'` 是不属于字符串的内容部分的，在写判断条件时必须把这一点加上。

```c++
bool match(char* str, char* pattern)
{
    if(str == NULL || pattern == NULL)
        return false;

    return matchCore(str, pattern);
}

bool matchCore(char* str, char* pattern)
{
    // 字符串和模式同时到达尾部，匹配成功
    if(*str == '\0' && *pattern == '\0')
        return true;

    // 模式已经到了最后，字符串还有未匹配的字符，匹配失败
    if(*str != '\0' && *pattern == '\0')
        return false;

    // 模式的下一位是*号
    if(*(pattern + 1) == '*')
    {
        // 模式能匹配当前字符，则分三种情况：匹配一个，匹配多个，匹配零个
        if(*pattern == *str || (*pattern == '.' && *str != '\0'))
                   // move on the next state
            return matchCore(str + 1, pattern + 2)
                   // stay on the current state
                || matchCore(str + 1, pattern)
                   // ignore a '*'
                || matchCore(str, pattern + 2);
        // 模式不能匹配当前字符，忽略*号
        else
                   // ignore a '*'
            return matchCore(str, pattern + 2);
    }

    // 模式的下一位不是*号，但是能匹配当前字符，就继续匹配
    if(*str == *pattern || (*pattern == '.' && *str != '\0'))
        return matchCore(str + 1, pattern + 1);

    // 模式的下一位不是*号，而且匹配不了当前字符，直接返回匹配失败
    return false;
}
```

## 面试题54：表示数值的字符串

### 题目

> 请实现一个函数用来判断字符串是否表示数值（包括整数和小数）。例如，字符串`"+100"`、`"5e2"`、`"-123"`、`"3.1416"`及`"-1E-16"`都表示数值，但`"12e"`、`"1a3.14"`、`"1.2.3"`、`"+-5"`及`"12e+5.4"`都不是。

### 解析

我们首先弄清楚怎样的字符串才表示数值：

```
[符号位] 整数部分 [.小数部分] [e|E [符号位] 指数部分]
```

可以概括如下：

- 最前面一位可以是符号位，但不是必需的；
- 整数部分是最基础的，但**一些小数可以不需要整数部分**（比如：`.5`）；
- 整数部分后可以接小数点和小数部分，它们不是必需的，但**出现了小数点就必须有小数部分**；
- 整数部分后可以接指数符号和指数部分，它们不是必需的，但**出现了指数符号就必须有指数部分**；
- 指数部分最前面一位可以是符号位，但不是必需的；
- 除了正负号、数字、小数点、大小写指数符号外不能出现其他字符；
- 小数点、大小写指数符号最多出现一次，正负号在整数部分和指数部分也各最多出现一次。

没有什么诀窍，按着这些规则编写代码，仔细检查有没有漏掉的情况就可以了。

```c++
bool isNumeric(char* str)
{
    if (str == NULL)
        return false;

    // 存在符号位就先跳过
    if (*str == '+' || *str == '-')
        ++str;
    // 如果只有符号位，则不是数值
    if (*str == '\0')
        return false;

    bool numeric = true;
    bool hasDigits = scanDigits(&str); // 跳过整数部分

    if (*str != '\0') // 如果整数部分后还有字符
    {
        // 检测到小数点
        if (*str == '.')
        {
            ++str; // 跳过小数点
            if (*str == 'e' || *str == 'E') // 小数点后直接接指数部分，非数值
                return false;
            if (!hasDigits && *str == '\0') // 没有整数部分也没有小数部分，非数值
                return false;

            scanDigits(&str); // 跳过小数部分

            // 检测到指数符号
            if (*str == 'e' || *str == 'E')
                numeric = isExponential(&str);
        }

        // 检测到指数符号
        else if (*str == 'e' || *str == 'E')
            numeric = isExponential(&str);

        // 检测到非法字符
        else
            numeric = false;
    }

    // 一直到字符串最后都没有出现非法字符
    // 如果出现了非法字符必然不会扫描到结束符
    return numeric && *str == '\0';
}

bool scanDigits(char** str)
{
    char* pBefore = *str;

    // 跳过数字部分
    while (**str != '\0' && **str >= '0' && **str <= '9')
        ++(*str);

    // 返回是否存在数字部分（如果指针发生了移动就说明存在）
    return *str > pBefore;
}

bool isExponential(char** str)
{
    if (**str != 'e' && **str != 'E')
        return false;

    ++(*str); // 跳过指数符号
    // 如果存在符号位就提哦啊过符号位
    if (**str == '+' || **str == '-')
        ++(*str);

    // 符号位后没有数字，也即没有指数部分，非数值
    if (**str == '\0')
        return false;

    // 跳过指数部分
    scanDigits(str);

    // 如果指数部分后面还有其它字符则非数值
    return (**str == '\0') ? true : false;
}
```

## 面试题55：字符流中第一个不重复的字符

### 题目

> 请实现一个函数用来找出字符流中第一个只出现一次的字符。例如，但从字符流中制度处前两个字符 `"go"` 时，第一个只出现一次的字符是 `'g'`。当从该字符流中读出前六个字符 `"gooogle"` 时，第一个只出现一次的字符是 `'l'`。

### 解析

这题其实不难，输入是数据流，所以也可以把它看作是一道大数据的题目，非常有意思。

这里我们实现一个类，通过维护一个哈希表和索引来实现题目的功能。因为是字符流，而字符只有256种可能，所以我们开一个长度为256的哈希表就可以了，索引则用来记录当前流过的字符在字符流中的位置。

哈希表的每一个格子对应一个字符的不同状态，比如做以下设定：

1. 字符未出现过，则哈希值为-1；
2. 字符出现超过一次，则哈希值为-2；
3. 字符仅出现了一次，哈希值为它在字符流中的位置（大于0）。

这样，当我们从字符流中取得一个字符时，我们可以用O(1)时间来检查一下字符的哈希值，哈希值为-2则不再关心，只对索引进行更新（索引加一）；哈希值为-1则把哈希值更新为当前索引，然后索引加一。

当我们要求第一个只出现一次的字符时，我们同样可以用O(1)时间找到答案（与输入规模，也即字符流的长度无关）。我们只需要遍历一次哈希表，找到只出现一次，且出现位置最前（索引值最小）的那个字符就可以了。

```c++
class CharStatistics
{
public:
    CharStatistics() : index (0) // 构造时index初始化为0
    {   // 哈希表全部初始化为-1，即未出现过
        for(int i = 0; i < 256; ++i)
            occurrence[i] = -1;
    }

    void Insert(char ch)
    {
        // 未出现过则把哈希表对应的值更新为在字符串中出现的位置
        if(occurrence[ch] == -1)
            occurrence[ch] = index;
        // 已经出现过则更新为-2，不再关心
        else if(occurrence[ch] >= 0)
            occurrence[ch] = -2;

        index++; // 没插入一个字符，索引加一
    }

    char FirstAppearingOnce()
    {
        char ch = '\0'; // 初始化为终止符
        // 把最小索引初始化为int型能表示的最大整数
        int minIndex = numeric_limits<int>::max();
        for(int i = 0; i < 256; ++i) // 遍历一次整个哈希表
        {
            // 每次找到一个只出现一次的数并且在字符串中位置比最小索引前
            // 就更新字符和最小索引
            if(occurrence[i] >= 0 && occurrence[i] < minIndex)
            {
                ch = (char)i; // 从ASCII码值转换回字符
                minIndex = occurrence[i];
            }
        }

        return ch;
    }

private:
    // occurrence[i]: A character with ASCII value i;
    // occurrence[i] = -1: The character has not found;
    // occurrence[i] = -2: The character has been found for mutlple times
    // occurrence[i] >= 0: The character has been found only once
    int occurrence[256]; // 哈希表
    int index;           // 索引，当前处于字符串的位置，或者说目前字符流的长度
};
```

## 面试题56：链表中环的入口结点

### 题目

> 一个链表中包含环，如何找出环的入口结点？例如，在图8.3的链表中，环的入口结点是结点3。

![ring](https://github.com/familyld/Coding_Interviews/blob/master/graph/56_EntryNodeInListLoop.jpg?raw=true)

### 解析

这题可以分为三个步骤来完成：

1. 判断链表中有没有环
2. 计算环的结点数
3. 找到环的入口结点

先分析第一步，判断链表中有没有环，可以用一快一慢两个指针来实现。为什么可以这样做不妨看看知乎上的一个问题：[为什么用快慢指针找链表的环，快指针和慢指针一定会相遇？](https://www.zhihu.com/question/23208893)，还是挺有意思的。如果链表中存在环，那么快指针必然会从后追上慢指针，发生相遇；如果链表中没有环，那么快指针就会走到尾结点。

接下来，如果存在环，我们就计算一下环中结点的数目。怎么办到呢？其实很简单，我们同样使用两个指针，一个指针固定不动在第一步快慢指针相遇的地方，另一个指针一边往前移动一边计数，那么当这两个指针再次相遇时，所得的计数就是环中结点的数目了。

最后，怎么找到环的入口结点呢？其实可能有人还没想明白第二步的作用，可以回忆一下[面试题15：链表中的倒数第k个结点](https://github.com/familyld/Coding_Interviews/blob/master/C%2B%2B/15_KthNodeFromEnd/README.md)。其实我们统计环中结点的数目就是为第三步服务的！要找到环的入口结点，实际上和这一题有异曲同工之妙。

假设我们把环的入口结点看作环的第一个结点（例子中的3），那么环的最后一个结点（例子中的6）就是环中连接到入口结点的那个结点，我们可以把它看作链表的最后一个结点。这时如果我们把环拿掉，其实这道题就变得跟面试题15一模一样了。

假设环中结点数为k，那么我们现在要找的就是整个链表的倒数第k个结点。同样使用两个指针，第一个指针先走k步，然后第二个指针再走，此时两个指针之间隔着k-1个结点。当第一个指针和第二个指针相遇时，它们指向的结点就是环的入口结点了。

```c++
ListNode* MeetingNode(ListNode* pHead)
{
    if(pHead == NULL)
        return NULL;

    ListNode* pSlow = pHead->m_pNext;
    if(pSlow == NULL)
        return NULL;

    ListNode* pFast = pSlow->m_pNext;
    while(pFast != NULL && pSlow != NULL)
    {
        if(pFast == pSlow) // 返回相遇的结点
            return pFast;

        pSlow = pSlow->m_pNext; // 慢指针走一步

        pFast = pFast->m_pNext; // 快指针走两步
        if(pFast != NULL)       // 注意判断第二步是否能走
            pFast = pFast->m_pNext;
    }

    // 不存在环，返回NULL
    return NULL;
}

ListNode* EntryNodeOfLoop(ListNode* pHead)
{
    ListNode* meetingNode = MeetingNode(pHead);
    if(meetingNode == NULL)
        return NULL;

    // 获取环中结点的数目
    int nodesInLoop = 1;
    ListNode* pNode1 = meetingNode;
    while(pNode1->m_pNext != meetingNode)
    {
        pNode1 = pNode1->m_pNext;
        ++nodesInLoop;
    }

    // 让指针1先走nodesInLoop步
    pNode1 = pHead;
    for(int i = 0; i < nodesInLoop; ++i)
        pNode1 = pNode1->m_pNext;

    // 指针1、2同时移动
    ListNode* pNode2 = pHead;
    while(pNode1 != pNode2) // 相遇时停止
    {
        pNode1 = pNode1->m_pNext;
        pNode2 = pNode2->m_pNext;
    }

    return pNode1;
}
```

## 面试题57：删除链表中重复的结点

### 题目

> 在一个排序的链表中，如何删除重复的结点？例如：

删除前：

```
1 -> 2 -> 3 -> 3 -> 4 -> 4 -> 5
```

删除后：

```
1 -> 2 -> 5
```

### 解析

这题我们需要注意的就是删除重复结点前，需要记住前面的那一个非重复结点，保证删除完毕后，前一个个非重复结点依然能连接到下一个非重复结点，链表不会发生锻炼。

另一个需要注意的点就是，头结点是有可能被删除的，所以调用函数时要按引用传递头结点指针，否则无法进行修改和删除。

```c++
void deleteDuplication(ListNode** pHead)
{
    if(pHead == NULL || *pHead == NULL)
        return;

    ListNode* pPreNode = NULL; // 前一结点初始为NULL，因为头结点没有前一结点
    ListNode* pNode = *pHead;
    while(pNode != NULL) // 遍历链表直到到达尾部
    {
        ListNode *pNext = pNode->m_pNext; // 取得下一结点
        bool needDelete = false;
        // 如果下一结点和当前结点值相同，就说明需要删除
        if(pNext != NULL && pNext->m_nValue == pNode->m_nValue)
            needDelete = true;

        // 不用删除的话就继续遍历下一结点
        // 把前一结点更新为当前结点
        if(!needDelete)
        {
            pPreNode = pNode;
            pNode = pNode->m_pNext;
        }

        // 删除结点
        else
        {
            // 从当前结点开始删除值相同的结点
            int value = pNode->m_nValue;
            ListNode* pToBeDel = pNode;
            while(pToBeDel != NULL && pToBeDel->m_nValue == value)
            {
                pNext = pToBeDel->m_pNext;

                delete pToBeDel;
                pToBeDel = NULL;

                pToBeDel = pNext;
            }

            // 如果发现是头结点被删除了，则把头结点改为（删除完重复结点后的）下一结点
            if(pPreNode == NULL)
                *pHead = pNext;
            // 否则就把前一结点的next指针指向（删除完重复结点后的）下一结点
            else
                pPreNode->m_pNext = pNext;

            pNode = pNext; // 继续遍历下一结点，前一结点不需移动
        }
    }
}
```

## 面试题58：二叉树的下一个结点

### 题目

> 给定一棵二叉树和其中的一个结点，如何找出中序遍历顺序的下一个结点？树中的结点除了有两个分别指向左右子结点的指针以外，还有一个指向父结点的指针。

```c++
struct BinaryTreeNode
{
    int                    m_nValue;
    BinaryTreeNode*        m_pLeft;
    BinaryTreeNode*        m_pRight;
    BinaryTreeNode*        m_pParent;
};
```

### 解析

所谓中序遍历，也即对于每个结点来说，首先访问左子树，然后再访问自身，最后访问右子树。在写代码时我们考虑一下三种情况：

1. 结点有右子树：下一结点为右子树的最左子结点
2. 结点无右子树：
    - 该结点是父结点的左子结点：下一结点为父结点
    - 该结点是父结点的右子结点：需要一直往上搜索，直到找到一个结点是其父结点的左子结点，则其父结点就是下一结点。若不存在则说明遍历结束。

```c++
BinaryTreeNode* GetNext(BinaryTreeNode* pNode)
{
    if(pNode == NULL)
        return NULL;

    BinaryTreeNode* pNext = NULL;
    if(pNode->m_pRight != NULL)
    { // 有右子树则下一结点为右子树的最左子结点
        BinaryTreeNode* pRight = pNode->m_pRight;
        while(pRight->m_pLeft != NULL)
            pRight = pRight->m_pLeft;

        pNext = pRight;
    }
    else if(pNode->m_pParent != NULL)
    { // 没有右子树但是有父结点就说明仍然可以找到下一结点
        BinaryTreeNode* pCurrent = pNode;
        BinaryTreeNode* pParent = pNode->m_pParent;
        while(pParent != NULL && pCurrent == pParent->m_pRight)
        { // 往上找直到找到一个结点属于父结点的左子树，则下一结点为父结点
            pCurrent = pParent;
            pParent = pParent->m_pParent;
        }

        pNext = pParent;
    }

    return pNext;
}
```

## 面试题59：对称的二叉树

### 题目

> 请实现一个函数，用来判断一棵二叉树是不是对称的。如果一棵二叉树和它的镜像一样，那么它是对称的。例如：

```
     8
   /   \
  6     6
 / \   / \
5   7 7   5
```

> 是一棵对称二叉树。

### 解析

前序遍历、中序遍历、后序遍历都是先访问左子结点再访问右子结点，而如果我们想要判断一棵树是否对称二叉树，我们可以创造一种遍历方式是先访问右子结点再访问左子结点的。如果使用两种对应遍历方式所得的遍历序列都相同，就说明这棵树是对称二叉树。

书中以前序遍历为例编写代码，但实际上中序遍历和后序遍历也是可以的~

特别注意，一种特殊情况如下：

```c++
     7
   /   \
  7     7
 / \   /
7   7 7
```

这种情况如果我们只是按上面的方式来比较是无法正确判断出的，怎么应对呢？我们只需要把NULL也考虑进来就可以了。

```c++
bool isSymmetrical(BinaryTreeNode* pRoot)
{
    return isSymmetrical(pRoot, pRoot);
}

bool isSymmetrical(BinaryTreeNode* pRoot1, BinaryTreeNode* pRoot2)
{
    // 同为NULL，则仍然认为是一样的，并且因为到达底部而返回
    if(pRoot1 == NULL && pRoot2 == NULL)
        return true;

    // 一方为NULL，而另一方不是，说明不对称，返回false
    if(pRoot1 == NULL || pRoot2 == NULL)
        return false;

    // 结点值不同，说明不对称，返回false
    if(pRoot1->m_nValue != pRoot2->m_nValue)
        return false;

    // 结点值相同，继续进行比较，树1先访问左结点再访问右结点，树2则相反
    return isSymmetrical(pRoot1->m_pLeft, pRoot2->m_pRight)
        && isSymmetrical(pRoot1->m_pRight, pRoot2->m_pLeft);
}
```

## 面试题60：把二叉树打印成多行

### 题目

> 从上到下按层打印二叉树，同一层的结点按从左到右的顺序打印，每一层打印到一行。例如，打印下面的二叉树：

```
     8
   /   \
  6     10
 / \   /  \
5   7 9    11
```

> 结果是：

```
8
6   10
5   7   9   11
```

### 解析

这题思路很简单，就是考查BFS的，逐层遍历一棵树只需要用队列模拟就可以了。

```c++
void Print(BinaryTreeNode* pRoot)
{
    if(pRoot == NULL)
        return;

    std::queue<BinaryTreeNode*> nodes;
    nodes.push(pRoot);
    int nextLevel = 0;   // 下一层需要打印的结点数
    int toBePrinted = 1; // 当前层需要打印的结点数

    while(!nodes.empty()) // 整棵树打印完毕时停止
    {
        BinaryTreeNode* pNode = nodes.front();
        printf("%d ", pNode->m_nValue);

        if(pNode->m_pLeft != NULL)
        {
            nodes.push(pNode->m_pLeft);
            ++nextLevel;
        }
        if(pNode->m_pRight != NULL)
        {
            nodes.push(pNode->m_pRight);
            ++nextLevel;
        }

        // 首结点出队，当前层需要打印的结点数减一
        nodes.pop();
        --toBePrinted;

        // 若当前层打印完毕，则进行换行，开始打印下一层
        if(toBePrinted == 0)
        {
            printf("\n");
            toBePrinted = nextLevel;
            nextLevel = 0;
        }
    }
}
```
