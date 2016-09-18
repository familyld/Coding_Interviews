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

## 面试题61：按之字形顺序打印二叉树

### 题目

> 请实现一个函数按照之字形顺序打印二叉树，即第一行按照从左到右的顺序打印，第二层按照从右到左的顺序打印，第三层再按照从左到右的顺序打印，其他行以此类推。

### 解析

这题和上一题比难度就上升了，显然这不是一个简单的BFS，我们要使用什么容器和往容器中存放结点的顺序都需要认真去考虑。

这里使用两个栈再加上一点小trick来实现，注意栈的特性的先进后出，后进先出。

我们注意当奇数层是顺着打印，偶数层是倒着打印。所以！**奇数层应该倒着入栈，偶数层则顺着入栈**。

明白这一点之后就好办了，我们使用两个栈来存放，一个栈用来存放当前打印的那一层，另一个栈用来存放下一层的结点值。如果只使用一个栈是不行的，当层数更高时会发生顺序的错乱。使用两个栈时我们只需要判断一下当前栈是否为空就知道这一层是否打印完毕要转换到另一个栈了。

具体来说，举个例子，比如下面这棵树：

```
         1
      /      \
    2          3
  /  \       /   \
 4    5     6     7
/ \  / \   / \   / \
8 9 10 11 12 13 14 15
```

| 步骤 | 操作 | Stack1中的结点 | Stack2中的结点 |
|: - :|: - :|: - :|: - :|
| 1 | 打印结点1 | 2,3 |  |
| 2 | 打印结点3 | 2 | 7,6 |
| 3 | 打印结点2 |  | 7,6,5,4 |
| 4 | 打印结点4 | 8,9 | 7,6,5 |
| 5 | 打印结点5 | 8,9,10,11 | 7,6 |
| 6 | 打印结点6 | 8,9,10,11,12,13 | 7 |
| 7 | 打印结点6 | 8,9,10,11,12,13,14,15 |  |

对于奇数层的结点而言，下一层要逆着打，所以先存左子结点，再存右子结点，这样下一层就会先打印右子结点再打印左子结点；

对于偶数层的结点而言，下一层要顺着打，所以先存右子结点，再存左子结点，这样下一层就会先打印左子结点再打印右子结点；

```c++
void Print(BinaryTreeNode* pRoot)
{
    if(pRoot == NULL)
        return;

    // 使用两个栈，0号栈存的必然是奇数层结点，1号栈存的必然是偶数层结点
    std::stack<BinaryTreeNode*> levels[2];
    int current = 0; // 指示存放当前层结点值的栈
    int next = 1;    // 指示存放下一层结点值的栈

    levels[current].push(pRoot);
    // 两个栈都为空时，说明整棵树已经打印完毕
    while(!levels[0].empty() || !levels[1].empty())
    {
        BinaryTreeNode* pNode = levels[current].top();
        levels[current].pop();

        printf("%d ", pNode->m_nValue);

        if(current == 0)
        { // 当前打印的是0号栈，则下一层为偶数层，先存左子结点，再存右子结点
            if(pNode->m_pLeft != NULL)
                levels[next].push(pNode->m_pLeft);
            if(pNode->m_pRight != NULL)
                levels[next].push(pNode->m_pRight);
        }
        else
        { // 当前打印的是1号栈，则下一层为奇数层，先存右子结点，再存左子结点
            if(pNode->m_pRight != NULL)
                levels[next].push(pNode->m_pRight);
            if(pNode->m_pLeft != NULL)
                levels[next].push(pNode->m_pLeft);
        }

        // 当前层打印完毕，打印下一层
        if(levels[current].empty())
        {
            printf("\n");
            current = 1 - current;
            next = 1 - next;
        }
    }
}
```

## 面试题62：序列化二叉树

### 题目

> 请实现两个函数，分别用来序列化和反序列化二叉树。

### 解析

首先要理解一下什么是序列化，什么是反序列化？

其实，我们谈论到的很多数据结构比方说树，甚至一些更复杂的自定定义的对象，是没法直接保存或传输的，只有程序运行时可以用。当我们需要保存到硬盘或者进行传输时就必须进行序列化，然后要放入程序时就要重新反序列化解析出来。可以如下定义：

- 序列化： 将数据结构或对象转换成二进制串的过程。
- 反序列化：将在序列化过程中所生成的二进制串转换成数据结构或者对象的过程。

不过这题我们在程序中只需要把二叉树转换为字符串就可以了。

前面有一道题是通过前序遍历序列和中序遍历序列构造出一棵二叉树的，这里其实也可以用这样的思路，把二叉树序列化成前序遍历序列和中序遍历序列这两个序列来存储，然后反序列化时按这两个序列重新构造二叉树就可以了。但这种思路有两个很大的缺点就是：（1）不能有数值重复的结点；（2）要整个序列读出来才可以进行反序列化。

事实上不用这么复杂，我们只需要选择一种遍历方式就可以了，关键是用一个特殊的符号来表示NULL，这样在反序列化时我们就可以知道什么时候到达底部了。

```c++
void Serialize(BinaryTreeNode* pRoot, ostream& stream)
{
    if(pRoot == NULL) // 使用$标识NULL
    {
        stream << "$,";
        return;
    }

    // 前序遍历
    stream << pRoot->m_nValue << ','; // 序列化当前结点，以逗号为分隔
    Serialize(pRoot->m_pLeft, stream);
    Serialize(pRoot->m_pRight, stream);
}

// 从输入流中每次读出一个结点的值
bool ReadStream(istream& stream, int* number)
{
    if(stream.eof())
        return false;

    char buffer[32];
    buffer[0] = '\0';

    char ch;
    stream >> ch;
    int i = 0;
    while(!stream.eof() && ch != ',')
    { // 遇到EOF表示序列已读完，而遇到逗号则说明这个结点的值读完了
        buffer[i++] = ch;
        stream >> ch;
    }

    // 判断读出的是否数字，并进行相应的转换
    bool isNumeric = false;
    if(i > 0 && buffer[0] != '$')
    { // i大于0说明上一步有读出过内容，读出内容不为$即结点是数值，进行转换
        *number = atoi(buffer);
        isNumeric = true;
    }

    return isNumeric;
}

void Deserialize(BinaryTreeNode** pRoot, istream& stream)
{
    int number;
    if(ReadStream(stream, &number))
    { // 如果读出数字就构造结点，否则表示已到达底部，递归返回
        *pRoot = new BinaryTreeNode();
        (*pRoot)->m_nValue = number;
        (*pRoot)->m_pLeft = NULL;
        (*pRoot)->m_pRight = NULL;

        Deserialize(&((*pRoot)->m_pLeft), stream);
        Deserialize(&((*pRoot)->m_pRight), stream);
    }
}
```

这题特别注意一下传参的方式，一些参数是需要使用指针传递/引用传递的（修改实参而非形参的值），否则会无法正确修改它在内存中的值，从而导致错误。不妨看看这篇文章：[C/C++中函数参数传递详解](http://www.cnblogs.com/Romi/archive/2012/08/09/2630014.html)。参数中带`&`表示按引用传递。

## 面试题63：二叉树搜索树的第k个结点

### 题目

> 给定一棵二叉搜索树，请找出其中的第k大的结点。例如：

```
     5
   /   \
  3     7
 / \   / \
2   4 6   8
```

> 这棵二叉搜索树里，按结点数值大小顺序第三个结点是4。

### 解析

这题思路很简单，对于一棵BST搜索第k大的结点，只需要用前序遍历来检索就可以了，前序遍历访问到的第k个结点就是题目所求。

```c++
BinaryTreeNode* KthNode(BinaryTreeNode* pRoot, unsigned int k)
{
    if(pRoot == NULL || k == 0)
        return NULL;

    return KthNodeCore(pRoot, k);
}

BinaryTreeNode* KthNodeCore(BinaryTreeNode* pRoot, unsigned int& k)
{
    BinaryTreeNode* target = NULL;

    if(pRoot->m_pLeft != NULL) // 先访问左子结点
        target = KthNodeCore(pRoot->m_pLeft, k);

    if(target == NULL) // 然后访问当前结点
    {
        if(k == 1) // 若此时k已为1，则当前结点为所求
            target = pRoot;

        k--;       // 每次访问完后k值减一
    }

    if(target == NULL && pRoot->m_pRight != NULL) // 若当前结点不是，继续访问右子结点
        target = KthNodeCore(pRoot->m_pRight, k);

    return target;
}
```

## 面试题64：数据流中的中位数

### 题目

> 如何得到一个数据流中的中位数？如果从数据流中读出奇数个数值，那么中位数就是所有数值排序之后位于中间的数值。如果从数据流中读出偶数个数值，那么中位数就是所有数值排序之后中间两个数的平均值。

### 解析

这条题目其实和第5章的 面试题30：最小的k个数 有一点联系，面试题30通过维持一个大小为k的最大堆来实现从数据流中确定最小k个数的功能。而这一题要求得到数据流的中位数，这要更难一些。一是我们没有办法确定堆的大小；二是有可能从数据流中读出了偶数个数值，此时需要所有数值排序之后中间两个数的平均值。

无法确定堆的大小，其实更准确一点来说，假设我们要找出从数据流中读取k个数时的中位数，我们必须把前面的k-1个数都保存下来因为我们并不知道k的取值，任意一个数字都可能会是取某个k值时的中位数，不能丢掉。

而为了更有效地插入新数字（O(log n)复杂度）和查询中位数（O(1)复杂度），我们可以同时维护两个堆，一个最大堆，一个最小堆，以中位数划分，最大堆保存排序后数组的前半部分，最小堆则保存后半部分。我们要注意保持数据平均地分到这两个堆中，也即堆的大小相差不超过1。当从数据流中读取了奇数个数值时，取最小堆的顶部为中位数；当从数据流中读取了偶数个数值时，取最大堆顶部和最小堆顶部两个数值的平均值为中位数。

为了保证正确取得中位数我们要保证最大堆里的数都比最小堆里的小，这就对插入操作有所要求。

```c++
template<typename T> class DynamicArray
{
public:
    void Insert(T num)
    {
        if(((min.size() + max.size()) & 1) == 0)
        { // 此时从数据流中读取了奇数个数值
            if(max.size() > 0 && num < max[0])
            { // 保证要加入最小堆的数字比最大堆的所有数字都更大
                max.push_back(num);
                push_heap(max.begin(), max.end(), less<T>());

                num = max[0]; // 如果新数字没有堆顶大，就会交换两者的值

                pop_heap(max.begin(), max.end(), less<T>());
                max.pop_back();
            }

            // 把新数字加入最小堆
            min.push_back(num);
            push_heap(min.begin(), min.end(), greater<T>());
        }
        else
        { // 此时从数据流中读取了偶数个数值
            if(min.size() > 0 && min[0] < num)
            {
                min.push_back(num);
                push_heap(min.begin(), min.end(), greater<T>());

                num = min[0]; // 如果新数字没有堆顶小，就会交换两者的值

                pop_heap(min.begin(), min.end(), greater<T>());
                min.pop_back();
            }

            // 把新数字加入最大堆
            max.push_back(num);
            push_heap(max.begin(), max.end(), less<T>());
        }
    }

    T GetMedian()
    {
        int size = min.size() + max.size();
        if(size == 0)
            throw exception(); //"No numbers are available"

        T median = 0;
        if((size & 1) == 1)
            median = min[0];
        else
            median = (min[0] + max[0]) / 2;

        return median;
    }

private:
    vector<T> min;
    vector<T> max;
};
```

注意这里实现堆结构的方式和第30题不同，这里使用STL的 `push_heap`，`pop_heap` 函数以及vector来实现堆，并且以比较仿函数less和greater来实现最大堆和最小堆。

## 面试题65：滑动窗口的最大值

### 题目

> 给定一个数组和滑动窗口的大小，请找出所有滑动窗口里的最大值。例如，如果输入数组 `{2,3,4,2,6,2,5,1}` 及滑动窗口的大小3，那么一共存在6个滑动窗口，它们的最大值分别为 `{4,4,6,6,6,5}`。

### 解析

这题其实也挺有意思的，稍微修改一下题目就可以用来考查数据流、大数据处理这样的问题了。这题的关键其实是要我们明白哪些数字有可能成为滑动窗口里的最大值（应当保存），哪些数字则不会（应当舍弃）。简单来说，有以下几个问题需要考虑：

1. 使用什么数据结构进行存储？（方便删除，插入和比较）
2. 一个在当前滑动窗口非最大值的数字是否仍有可能成为另一个滑动窗口的最大值？
3. 新加入滑动窗口的数字可能对已存储的数字带来什么影响？
4. 怎样确定数字超出了滑动窗口的范围？

针对问题1，我们可以使用一个两端开口的队列（deque）来存储那些滑动窗口内可能的最大值，前后都可以pop，并且可以使用 `front()` 函数和 `back()` 函数容易地取得队首和队尾，非常方便。注意，这个队列存储的是**可能的最大值**，也可以理解为它是一个中转站。我们还需要用另一个数据结构来存储**真正的每个滑动窗口的最大值**，这个数据结构要求就没有那么高，用vector就可以了。

针对问题2，当前滑动窗口非最大值的数字是有可能成为另一个滑动窗口的最大值的。举个例子，序列 `{6,5,4}`，窗口大小为2，那么第一个滑动窗口 `{6,5}` 里面最大值是6，但5也应当被存储下来，因为在下一个滑动窗口 `{5,4}` 中5就是最大值。

针对问题3，可以分两种情况讨论，一是窗口内存在比新数字小的数；二是窗口内存在的数都比新数字大。在情况一中，那些比新数字小的数我们可以都pop掉，因为它们比新数字小所以肯定不可能是当前窗口的最大值了，而往后滑动时，它们存在于窗口时新数字也一定在，所以依然不可能成为窗口内的最大值，因此可以把它们都扔掉；但在情况二中，尽管新数字比当前窗口内的数字都小，在窗口往后滑动时，新数字依然有可能成为窗口内的最大值，比方说上一段举出的例子，因此这种情况下依然要把新数字入队。考虑好这两种情况就能**保证队列的队首永远是当前窗口的最大值**。

针对问题4，我们在队列中存储时可以不要存数值，而是存储它在数组中的索引。这样当往后移动窗口，加入新数字时，我们只需要把队列中索引值小于 新数字索引-窗口大小 的索引值出队就可以了。

想清楚以上四个问题后，不难写出以下代码：

```c++
vector<int> maxInWindows(const vector<int>& num, unsigned int size)
{
    vector<int> maxInWindows;
    if(num.size() >= size && size >= 1)
    {
        deque<int> index;

        // 索引队列的初始化
        for(unsigned int i = 0; i < size; ++i)
        {
            while(!index.empty() && num[i] >= num[index.back()])
                index.pop_back();

            index.push_back(i);
        }

        // 往后移动滑动窗口，直到到达数组尾部，i为新加入数字（窗口尾部）的索引
        for(unsigned int i = size; i < num.size(); ++i)
        {
            // 索引队列的队首是当前窗口的最大值，存入maxInWindows数组中
            maxInWindows.push_back(num[index.front()]);

            // 把索引队列里比窗口尾部数字小的都pop掉，它们不可能成为最大值了
            while(!index.empty() && num[i] >= num[index.back()])
                index.pop_back();
            // 把滑出了滑动窗口的也pop掉
            if(!index.empty() && index.front() <= (int)(i - size))
                index.pop_front();

            index.push_back(i); // 把新数字（当前窗口尾部）的索引加入索引队列
        }
        maxInWindows.push_back(num[index.front()]); // 最后一个滑动窗口的最大值
    }

    return maxInWindows;
}
```

## 面试题66：矩阵中的路径

### 题目

> 请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有字符的路径。路径可以从矩阵中任意一格开始，每一步可以在矩阵中向左、右、上、下移动一格。如果一条路径经过了矩阵的某一格，那么该路径不能再次进入该格子。例如下面的矩阵：

```
a    b    c    e
s    f    c    s
a    d    e    e
```

> 包含了一条字符串 `"bcced"` 的路径。但矩阵中不包含字符串 `"abcb"` 的路径，因为字符串的第一个字符b占据了矩阵中的第一行第二个格子之后，路径不能再次进入这个格子。

### 解析

这题其实思路相当简单，我们可以直接枚举以矩阵的每一个格子作为路径开头进行深搜的情况，直到能完整匹配字符串就停止。特别注意要使用和输入矩阵同规模的visited矩阵来标记走过的路径，避免重复走过一个格子两次的情况。

使用一个二重循环来实现枚举就可以了，注意每一次深搜时除了要注意是否匹配，还要注意边界的判断。当发现路径不合适要递归返回时，必须把路径长度减一并且设置好visited矩阵对应的值，这样才算把当前格子移出了路径。另外，搜索的时候，我们是往上下左右四个方向搜索，只要其中一个方向能成功匹配就算找到了。

```c++
bool hasPath(char* matrix, int rows, int cols, char* str)
{
    if(matrix == NULL || rows < 1 || cols < 1 || str == NULL)
        return false;

    bool *visited = new bool[rows * cols];
    memset(visited, 0, rows * cols);

    int pathLength = 0; // 初始路径长度为0
    // 使用一个二重循环来枚举以矩阵的每一个格子作为路径开头进行深搜的情况
    for(int row = 0; row < rows; ++row)
    {
        for(int col = 0; col < cols; ++col)
        {
            if(hasPathCore(matrix, rows, cols, row, col, str,
                pathLength, visited))
            { // 只要有一次找到了就算成功，直接返回
                return true;
            }
        }
    }

    delete[] visited;

    return false;
}

bool hasPathCore(char* matrix, int rows, int cols, int row, int col, char* str, int& pathLength, bool* visited)
{
    if(str[pathLength] == '\0') // 字符串匹配完成，返回true
        return true;

    bool hasPath = false;
    if(row >= 0 && row < rows && col >= 0 && col < cols
            && matrix[row * cols + col] == str[pathLength]
            && !visited[row * cols + col])
    { // 前四个判断条件用于防止溢出边界，后两个判断条件则是当前格子
      // 与当前字符是否匹配，当前格子是否未使用
        ++pathLength;
        visited[row * cols + col] = true;

        // 只要有一个方向匹配完整就算成功
        hasPath = hasPathCore(matrix, rows, cols, row, col - 1,
                    str, pathLength, visited)
                || hasPathCore(matrix, rows, cols, row - 1, col,
                    str, pathLength, visited)
                || hasPathCore(matrix, rows, cols, row, col + 1,
                    str, pathLength, visited)
                || hasPathCore(matrix, rows, cols, row + 1, col,
                    str, pathLength, visited);

        if(!hasPath)
        { // 匹配失败时要把格子移出路径，以便进行下一次搜索
            --pathLength;
            visited[row * cols + col] = false;
        }
    }

    return hasPath;
}
```

## 面试题67：机器人的运动范围

### 题目

> 地上有一个m行n列的方格。一个机器人从坐标(0,0)的格子开始移动，它每一次可以向左、右、上、下移动一格，但不能进入行坐标和列坐标的数位之和大于k的格子。例如，当k为18时，机器人能够进入方格(35,37)，因为 `3+5+3+7=18`。但它不能进入方格(35,38)，因为 `3+5+3+8=19`。请问该机器人能够到达多少个格子？

### 解析

这题比上一题稍微简单一些，不需要枚举矩阵的所有格子，只需要从(0,0)开始，进行一次深搜就可以了。要注意好不能重复计算同一个格子，所以同样使用一个和输入矩阵同规模的visited矩阵来标记。过程主要用递归来实现就可以了：

```c++
// 计算一个数字的各数位之和
int getDigitSum(int number)
{
    int sum = 0;
    while(number > 0)
    {
        sum += number % 10;
        number /= 10;
    }

    return sum;
}

// 判断机器人能否进入坐标为(row,col)的方格
bool check(int threshold, int rows, int cols, int row, int col, bool* visited)
{
    if(row >=0 && row < rows && col >= 0 && col < cols      // 满足四个边界条件
        && getDigitSum(row) + getDigitSum(col) <= threshold // 行列坐标数位之和小于阈值
        && !visited[row* cols + col])                       // 并且未计算过
        return true;

    return false;
}

int movingCountCore(int threshold, int rows, int cols, int row, int col, bool* visited)
{
    int count = 0;
    if(check(threshold, rows, cols, row, col, visited))
    { // 当前格子满足条件，标记并继续往四个方向进行移动
        visited[row * cols + col] = true;

        count = 1 + movingCountCore(threshold, rows, cols,
                    row - 1, col, visited)
                + movingCountCore(threshold, rows, cols,
                    row, col - 1, visited)
                + movingCountCore(threshold, rows, cols,
                    row + 1, col, visited)
                + movingCountCore(threshold, rows, cols,
                    row, col + 1, visited);
    }

    return count;
}

int movingCount(int threshold, int rows, int cols)
{
    bool *visited = new bool[rows * cols];
    for(int i = 0; i < rows * cols; ++i)
        visited[i] = false;

    int count = movingCountCore(threshold, rows, cols,
                    0, 0, visited);

    delete[] visited;

    return count;
}
```
