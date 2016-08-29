# 高质量的代码

## 面试题11：数值的整数次方

### 题目

> 实现函数 `double Power(double base, int exponent)`，求base的exponent次方。不得使用库函数，同时不需要考虑大数问题。

### 解析

乍看之下，这道题其实挺简单的，似乎写个循环就好了，但其实包含了很多小的细节。

比方说从减少时间复杂度的角度考虑，直接用循环求一个数的n次方，复杂度是O(n)；而如果采用**快速求幂**：

1. a^n = a^(n/2) · a^(n/2),&ensp;&ensp;&ensp; n为偶数
2. a^n = a^(n/2) · a^(n/2) · a, n为奇数

复杂度就变为了O(log n)

更深入地，如何判断一个数是奇数还是偶数呢？直接**用求余运算符（%）的效率不如使用位运算**。我们可以通过判断一个数与1相与的结果来判断，因为奇数的二进制表示最右边一位必然是1。

有了上面这些分析，基本的框架就搭起来了。但是！还有很多细节没注意到！

如果指数为0，应该怎么处理呢？指数是负数的话前面的计算方法还成立吗？如果底数为0，又应该怎么处理呢？

首先，指数为0的求幂应该返回结果1，特别地，0的0次方在数学上是没有意义的，返回0或者1都可以，具体可以跟面试官沟通一下。

然后，对于指数为负的状况，我们可以先把指数变为绝对值，然后求幂，最后取倒数即可。特别地，取倒数时**不应该用1作为被除数**，而应用1.0。因为题目中给出的底数是double类型，返回值也是double类型。如果我们用1作为被除数，那么当底数是1是，求倒数得到的就会变成了int类型。

最后，针对底数为0的问题，我们要注意到**对0求负数幂是不可能的**，因为0无法作为分母。这时候应该返回什么呢？可以选择返回0，但是怎样**和正常返回0的情况作区分**呢？我们可以再设置一个全局标志。虽然直接返回数值允许我们在调用这个函数时可以直接赋值，但如果**忘记检查全局标志**依然会出错。

最后的最后，还有一个小细节，在检查底数是否为0时，不能直接使用 `base == 0` 来判定。因为**计算机中表示小数（double类型和float类型）是有误差的**，所以如果要判断两个小数是否相等，我们应该看它们的**差的绝对值是否足够小**。

```c++
bool g_InvalidInput = false;

double Power(double base, int exponent)
{
    g_InvalidInput = false;

    if(equal(base, 0.0) && exponent < 0)
    {
        g_InvalidInput = true;
        return 0.0;
    }

    unsigned int absExponent = (unsigned int)(exponent);
    if(exponent < 0)
        absExponent = (unsigned int)(-exponent);

    double result = PowerWithUnsignedExponent(base, absExponent);
    if(exponent < 0)
        result = 1.0 / result;

    return result;
}

double PowerWithUnsignedExponent(double base, unsigned int exponent)
{
    if(exponent == 0)
        return 1;
    if(exponent == 1)
        return base;

    double result = PowerWithUnsignedExponent(base, exponent >> 1);
    result *= result;
    if((exponent & 0x1) == 1)
        result *= base;

    return result;
}

bool equal(double num1, double num2)
{
    if((num1 - num2 > -0.0000001)
        && (num1 - num2 < 0.0000001))
        return true;
    else
        return false;
}
```

## 面试题12：打印1到最大的n位数

### 题目

> 输入数字n，按顺序打印出从1最大的n位十进制数。比如输入3，则打印出1、2、3一直到最大的3位数即999。

### 解析

初看这条题，容易产生好简单的错觉。如果我们不需要考虑**大数问题**，那这条题将没有任何难度。而事实上，题目所说的n位数很可能是超出int甚至long long所能表示的，因此我们只能用字符串或者数组来进行表示了。

考虑字符串表示的方法，最直观的做法是**模拟加法**来实现，代码中的解法一就是这样做的。但是这样写的代码不太简洁，有没有更好的思路呢？

其实，题目要求按顺序打印出从1最大的n位十进制数，这个数的每一位无非就只有10种可能，也即我们可以把它想像成一个**排列组合问题**。只要我们按顺序罗列出所有排列组合就可以了。

可以使用**递归**的方式来实现，递归结束的条件就是设置完最后一位的数值。

最后要注意，使用字符串来表示数字，从索引0到n-1分别对应着数字的最高位到最低位。但不是所有数都是n位数，有可能是两位数、三位数等等，因此输出时要把高位的0过滤掉。特别注意，按照前面的思路，我们有可能得到n位全是0这种排列，按题目要求是从1开始输出，所以我们也要注意避免输出数字0。

```c++
void Print1ToMaxOfNDigits_2(int n)
{
    if(n <= 0) // 避免不合法的输入
        return;

    char* number = new char[n + 1];
    number[n] = '\0';

    // 依次尝试最高位数字的10种可能
    for(int i = 0; i < 10; ++i)
    {
        number[0] = i + '0'; //int转为对应char
        Print1ToMaxOfNDigitsRecursively(number, n, 0);
    }

    delete[] number;
}

// 利用递归来完成数字各个位置的设置，并尝试所有的可能
void Print1ToMaxOfNDigitsRecursively(char* number, int length, int index)
{
    // 索引为length-1时，数字设置完成，打印当前数字
    if(index == length - 1)
    {
        PrintNumber(number);
        return;
    }

    // 未设置完成则依次尝试右边一位的10种可能
    for(int i = 0; i < 10; ++i)
    {
        number[index + 1] = i + '0';
        Print1ToMaxOfNDigitsRecursively(number, length, index + 1);
    }
}

// 字符串number表示一个数字，数字有若干个0开头
// 打印出这个数字，并忽略开头的0
void PrintNumber(char* number)
{
    bool isBeginning0 = true;
    int nLength = strlen(number);

    for(int i = 0; i < nLength; ++ i)
    {
        if(isBeginning0 && number[i] != '0')
            isBeginning0 = false;

        if(!isBeginning0)
        {
            printf("%c", number[i]);
        }
    }

    if(!isBeginning0)
        printf("\t");
}
```

## 面试题13：在O(1)时间删除链表结点

### 题目

> 给定单向链表的头指针和一个结点指针，定义一个函数在O(1)时间删除该结点。链表结点与函数的定义如下：

```c++
struct ListNode
{
    int       m_nValue;
    ListNode* m_pNext;
};
```

```
void DeleteNode(ListNode** pListHead, ListNode* pToBeDeleted);
```

### 解析

最常规的一种思路是遍历链表找到这个结点，删除它并把它前一结点的指针指向下一结点。但这样做复杂度是O(n)，而题目要求的是O(1)的时间复杂度，怎么做到呢？

其实不难，所谓O(1)意思是**处理的时间与输入规模无关**。我们可以直接从要删除的结点下手，把下一结点的内容复制到要删除的结点中，然后把要删除的结点的指针指向下下个结点，并把下一结点删除掉。这样做和前面的效果是一样的。

但是！注意了，如果**要删除的结点是尾**结点就无法这样做了，因为没有下一结点，这时只能顺序遍历来删除。还有一个细节，如果**链表只有一个结点**，那么除了删除，还要把头结点设置为NULL。

此外，以上分析都是基于链表中存在待删除结点来讨论的，如果要删除的结点不在链表中就会出错，这就需要调用函数的人自己注意了。要确定一个结点是否在链表中还是需要O(n)的时间。

```c++
void DeleteNode(ListNode** pListHead, ListNode* pToBeDeleted)
{
    if(!pListHead || !pToBeDeleted)
        return;

    // 要删除的结点不是尾结点
    if(pToBeDeleted->m_pNext != NULL)
    {
        ListNode* pNext = pToBeDeleted->m_pNext;
        pToBeDeleted->m_nValue = pNext->m_nValue;
        pToBeDeleted->m_pNext = pNext->m_pNext;

        delete pNext;
        pNext = NULL;
    }
    // 链表只有一个结点，删除头结点（也是尾结点）
    else if(*pListHead == pToBeDeleted)
    {
        delete pToBeDeleted;
        pToBeDeleted = NULL;
        *pListHead = NULL;
    }
    // 链表中有多个结点，删除尾结点
    else
    {
        ListNode* pNode = *pListHead;
        while(pNode->m_pNext != pToBeDeleted)
        {
            pNode = pNode->m_pNext;
        }

        pNode->m_pNext = NULL;
        delete pToBeDeleted;
        pToBeDeleted = NULL;
    }
}
```

## 面试题14：调整数组顺序使奇数位于偶数前面

### 题目

> 输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有奇数位于数组的前半部分，所有偶数位于数组的后半部分。

### 解析

这条题的思路其实不难，我们**维护两个指针**即可。指针1初始化指向数组的第一个元素，并向后移动；指针2初始化指向数组的最后一个元素，并向前移动。

首先往后移动指针1，当指针1指向偶数时，开始往前移动指针2，直至指针2指向奇数。注意，在这两个过程中，有可能指针1和指针2会相遇，这表示数组已经调整完毕。如果没有相遇，就证明还需继续调整，将此时指针1和指针2所指的数字交换位置，再继续下一轮的查找，直至两指针相遇时结束。

这道题其实可以做很多扩展，可能不是针对奇偶数重排，而是素数非素数，正数负数等等。**考虑扩展性**的话，我们不妨把判断部分分离出来写成一个函数，在重排时**把判断函数作为一个参数传入**即可。

具体来说，这里依然使用位运算来实现判断奇偶，把这个操作封装为一个函数，传入参数为int型变量，返回值为bool型变量。

在声明重排函数的参数时，格式就是 `返回值类型 (*函数别名)(函数的参数类型)`。其中函数别名根据自己喜欢来取就可以了。

在调用重排函数时，我们传入函数名就可以了，不需要作其他处理。

```c++
void ReorderOddEven_2(int *pData, unsigned int length)
{
    Reorder(pData, length, isEven);
}

void Reorder(int *pData, unsigned int length, bool (*func)(int))
{
    if(pData == NULL || length == 0)
        return;

    int *pBegin = pData;
    int *pEnd = pData + length - 1;

    while(pBegin < pEnd)
    {
        // 向后移动pBegin
        while(pBegin < pEnd && !func(*pBegin))
            pBegin ++;

        // 向前移动pEnd
        while(pBegin < pEnd && func(*pEnd))
            pEnd --;

        if(pBegin < pEnd)
        {
            int temp = *pBegin;
            *pBegin = *pEnd;
            *pEnd = temp;
        }
    }
}

bool isEven(int n)
{
    return (n & 1) == 0;
}
```

## 面试题15：链表中的倒数第k个结点

### 题目

> 输入一个链表，输出该链表中倒数第k个结点。为了符合大多数人的习惯，本题从1开始计数，即链表的尾结点是倒数第1个结点。例如一个链表有6个结点，从头结点开始它们的值依次是1、2、3、4、5、6。这个链表的倒数第3个结点是值为4的结点。链表结点定义如下：

```c++
struct ListNode
{
    int       m_nValue;
    ListNode* m_pNext;
};
```

### 解析

由于这是一个单向链表，所以就不存在先走到链表尾部，再倒退k-1步的可能了。

最普通的一个思路是，使用一个指针，先遍历一次整个链表，得到链表长度n。倒数第k个结点是链表中的第n-k+1个结点。我们只要然后重新从链表头开始走n-k步就能走到它了。

但是这样需要遍历两次，有没有**只遍历一次**就能找到它的方法呢？

有的。类似上一道题，我们维护两个指针就可以了。指针1先走，在指针1走了k-1步之后指针2再走。这样当指针1到底尾结点时，指针2所指的就是倒数第k个结点。

上面的解法有3个很大的漏洞：

1. **链表头结点是空指针**，遍历这样的链表会造成访问空指针指向的内存，引起程序崩溃，所以要额外处理。

2. **链表结点数目少于k**，如果我们直接让指针1在循环中走k-1步而不加判断，同样会访问到空指针指向的内存。

3. **k为0**，如果我们定义k是unsigned int型，那么k-1就是unsigned int型能表示的最大整数4294967295（即二进制的0xFFFFFFFF，最高位不需用于标识符号），循环的次数将会非常恐怖.. 所以我们必须对k值是否合法也做一个判断。

```c++
ListNode* FindKthToTail(ListNode* pListHead, unsigned int k)
{
    // 空指针处理和合法k值检查
    if(pListHead == NULL || k == 0)
        return NULL;

    ListNode *pAhead = pListHead;
    ListNode *pBehind = NULL;

    // 指针1走k-1步
    for(unsigned int i = 0; i < k - 1; ++ i)
    {
        if(pAhead->m_pNext != NULL) // 对链表长度不足k作出处理
            pAhead = pAhead->m_pNext;
        else
        {
            return NULL;
        }
    }

    // 指针2开始走，直到指针1走到链表尾时停止
    pBehind = pListHead;
    while(pAhead->m_pNext != NULL)
    {
        pAhead = pAhead->m_pNext;
        pBehind = pBehind->m_pNext;
    }

    return pBehind;
}
```

## 面试题16：反转链表

### 题目

> 定义一个函数，输入一个链表的头结点，反转该链表并输出反转后链表的头结点。链表结点定义如下：

```c++
struct ListNode
{
    int       m_nValue;
    ListNode* m_pNext;
};
```

### 解析

关于链表的题目，其实最好画图来把过程中的各个步骤都理清楚。要实现反转链表，我们需要维护三个指针，一个指向当前结点，一个指向上一结点，还有一个指向下一结点。

当我们实现反转时，其实是把原链表中上一结点变成当前结点的下一结点，但这样就会使得链表发生断裂，原链表中下一结点就找不到了，所以我们要先记住它。

在每一个翻转过程中，有以下步骤：

1. 得到当前结点的下一结点
2. 判断下一结点是否NULL，也即是否到达原链表的链表尾
    - 若下一结点为NULL，说明当前结点就是反转链表的链表头
3. 把当前结点的指针指向上一结点
4. 更新
    - 当前结点变为上一结点
    - 下一结点变为当前结点
5. 当前结点为NULL时结束，否则继续下一次反转

```c++
ListNode* ReverseList(ListNode* pHead)
{
    ListNode* pReversedHead = NULL;
    ListNode* pNode = pHead;
    ListNode* pPrev = NULL;
    while(pNode != NULL)
    {
        ListNode* pNext = pNode->m_pNext;

        if(pNext == NULL)
            pReversedHead = pNode;

        pNode->m_pNext = pPrev;

        pPrev = pNode;
        pNode = pNext;
    }

    return pReversedHead;
}
```

## 面试题17：合并两个排序的链表

### 题目

> 输入两个递增排序的链表，合并这两个链表并使新链表中的结点仍然是按照递增排序的。例如下面的链表1和链表2可以合并为链表3。

```
链表1： 1 -> 3 -> 5 -> 7
链表2： 2 -> 4 -> 6 -> 8
链表3： 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8
```

> 链表结点定义如下：

```c++
struct ListNode
{
    int       m_nValue;
    ListNode* m_pNext;
};
```

### 解析

这题其实写成代码不算难，但还是要理清思路。合并两个**递增排序链表**，我们可以采取递归的方式。

令**递归函数返回合并链表的链表头**，在每次递归中，我们先对两个链表的链表头进行判断，取较小的一个作为合并链表的头结点（并在函数结束时返回）。然后，还要重新设置这个结点的next指针，它指向的是两个链表未被并入的结点中最小的那个结点。要实现这一点，我们只需要令这一轮头结点被并入的链表的头结点指针往后移动一个位置，然后继续递归合并两个链表就可以了。**下一轮递归返回的链表头就是当前合并链表的头结点的next指针指向的结点**。

关键是**递归结束条件的设置**，有以下三种情况：

1. 链表1的头结点为NULL而链表2不是，此时返回链表2的头结点即可，因为链表2本身也是递增排序，所以合并后依然能保持递增排序；

2. 链表2的头结点为NULL而链表1不是，此时返回链表1的头结点即可，因为链1本身也是递增排序，所以合并后依然能保持递增排序；

3. 链表1和链表2的头结点都是NULL，此时返回NULL即可。

```c++
ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
{
    // 这里实际已经包含了三种递归结束条件
    if(pHead1 == NULL)
        return pHead2;
    else if(pHead2 == NULL)
        return pHead1;

    ListNode* pMergedHead = NULL;

    if(pHead1->m_nValue < pHead2->m_nValue)
    {
        pMergedHead = pHead1;
        pMergedHead->m_pNext = Merge(pHead1->m_pNext, pHead2);
    }
    else
    {
        pMergedHead = pHead2;
        pMergedHead->m_pNext = Merge(pHead1, pHead2->m_pNext);
    }

    return pMergedHead;
}
```

## 面试题18：树的子结构

> 题目：输入两棵二叉树A和B，判断B是不是A的子结构。二叉树结点的定义如下：

```c++
struct BinaryTreeNode
{
    int                    m_nValue;
    BinaryTreeNode*        m_pLeft;
    BinaryTreeNode*        m_pRight;
};
```

### 解析

树的结构要比链表更加复杂，但是只要分析得好，这一题不算太难。

我们将整个判断过程分为两个步骤：

1. 寻找树A中与树B根结点值相同的结点R；
2. 分析树A中以结点R为根结点的子树是否和树B的结构一样。

具体来说可以实现两个递归函数。

函数1执行步骤1。输入两棵树的根结点，如果树A根结点与B的不同，则把它左右子树的根结点作为树A的根节点传入函数1来递归查找；如果两棵树根结点相同就调用函数2。递归的终止条件有两种情形：

1. 树A或树B的根节点为NULL，有可能是因为输入了空指针，也可能是递归查找到达了树A的叶结点处仍然没匹配上，这两种情况都是返回false；
2. 这一轮成功匹配上，匹配上之后就不需要再递归查找了，所以我们在 调用函数2得到结果后，先判断有没匹配成功，没有再看左子树，左子树也没有才看右子树。

函数2执行步骤2。输入两棵树的当前结点，如果树B当前结点为NULL，说明匹配成功了；否则如果树A当前结点为NULL，说明A已经到叶结点但还没匹配完，也即匹配失败。另外，只要在这一轮中，两棵树的当前结点值不同，也算匹配失败。如果经过以上判断后，仍然没有得到匹配结果，那就说明目前还是能匹配上的，下一步继续递归地检查两棵树当前结点的左子树和右子树是否也能匹配。

```c++
bool HasSubtree(BinaryTreeNode* pRoot1, BinaryTreeNode* pRoot2)
{
    bool result = false;

    // 如果到了叶结点依然没匹配上或者传入的树是空指针就直接返回false
    if(pRoot1 != NULL && pRoot2 != NULL)
    {
        if(pRoot1->m_nValue == pRoot2->m_nValue)
            result = DoesTree1HaveTree2(pRoot1, pRoot2);
        // 实际隐藏了一个递归结束的条件
        // 成功匹配时，不会再继续递归搜索左右子树了
        // 因此成功匹配的那一轮就停下了并返回结果
        if(!result)
            result = HasSubtree(pRoot1->m_pLeft, pRoot2);
        if(!result)
            result = HasSubtree(pRoot1->m_pRight, pRoot2);
    }

    return result;
}

bool DoesTree1HaveTree2(BinaryTreeNode* pRoot1, BinaryTreeNode* pRoot2)
{
    if(pRoot2 == NULL)
        return true;

    if(pRoot1 == NULL)
        return false;

    if(pRoot1->m_nValue != pRoot2->m_nValue)
        return false;

    return DoesTree1HaveTree2(pRoot1->m_pLeft, pRoot2->m_pLeft) &&
        DoesTree1HaveTree2(pRoot1->m_pRight, pRoot2->m_pRight);
}
```
