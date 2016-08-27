# 面试需要的基础知识

## 面试题1：赋值运算符函数

### 题目

> 如下为类型CMyString的声明，请为该类型添加赋值运算符函数。

```c++
class CMyString
{
public:
    CMyString(char* pData = NULL);
    CMyString(const CMyString& str);
    ~CMyString(void);

    CMyString& operator = (const CMyString& str);

    void Print();

private:
    char* m_pData;
};
```

### 解析

乍一看，好像这个题目还挺简单的。但要注意的地方也不少。最基本的有以下几个需要考虑的点：

1. 返回值设置为**该类型的引用**，只有返回引用，才能够进行**连续赋值**。

2. 传入参数设置为**常量引用**。如果直接传入实例（按值传递）的话，从形参到实参就需要调用一次复制构造函数，造成一些不必要的开销。又因为赋值运算中传入参数不需要被修改，所以声明为常量。

3. 释放当前实例本身（等号左方）的内存。如果没有释放，又分配了新的空间，就会发生**内存泄漏**（未释放的部分一直占据着内存单元，直到程序结束）。

4. 判断**传入参数和当前实例是否同一个实例**，如果是的话，直接返回当前实例。如果不做这个判断，那么释放当前实例内存时，就会把传入参数的内存也释放掉了，要赋值的内容就丢失了。

```c++
CMyString& CMyString::operator = (const CMyString& str)
{
    if(this == &str)
        return *this;

    delete []m_pData;
    m_pData = NULL;

    m_pData = new char[strlen(str.m_pData) + 1]; //注意+1，用于字符串的结尾字符'\0'
    strcpy(m_pData, str.m_pData);

    return *this;
}
```

更进一步的话，针对**内存不足**的情况，可以在释放实例本身内存之前，检查一下，余下的内存空间是否足以分配所需的新空间。

具体来说，书中用了临时实例来实现，创建成功就交换当前实例和临时实例的值，在赋值完成后，临时实例被销毁，也即调用了析构函数把原本当前实例的内存释放了。

## 面试题2：实现Singleton模式

### 题目

> 设计一个类，我们只能生成该类的一个实例。

题目要求实现单例这种设计模式，这个需求还是挺常见的。由于书中采用c#语言去实现，而且这题和语言本身的特性关系比较大，所以我不在这里进行详细解析了。网上也有用c++实现单例的代码，感兴趣的话可以查找一下相关资料。

## 面试题3：二维数组中的查找

### 题目

> 在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。

### 解析

按照这个二维数组的规律，要找出数组中是否包含目标数字，我们需要进行若干次比较操作。

如果是一个完全无序的数组，就需要逐个元素进行比较。

但对于这样有规律的数组，我们可以只跟对角线上的元素比较。比方说从左对角线（二维数组的左上角到右下角）的第一个元素（左上角）开始比较。当比较到上一个对角线元素比目标数字小，而下一个对角线元素比目标数字大时，只需要检查上一对角线元素的所在行的右边所有元素和所在列的下边所有元素就可以了。

能不能再减少一些比较次数呢？能！

如果从右对角线的第一个元素开始比较，只要该数比目标数字小，我们就可以删除掉它所在的行；只要该数比目标数字大，我们就可以删除掉它所在的列；如果相等，就查找结束。相比起上一种方法，这种方法需要查找的次数更少！（另外，从右对角线的最后一个元素开始也是可行的）。

```c++
bool Find(int* matrix, int rows, int columns, int number)
{
    bool found = false;

    if(matrix != NULL && rows > 0 && columns > 0)
    {
        int row = 0;
        int column = columns - 1;
        while(row < rows && column >=0)
        {
            if(matrix[row * columns + column] == number)
            {
                found = true;
                break;
            }
            else if(matrix[row * columns + column] > number)
                -- column;
            else
                ++ row;
        }
    }

    return found;
}
```

## 面试题4：替换空格

### 题目

> 请实现一个函数，把字符串中的每个空格替换成"%20"，例如输入 “We are happy.”，则输出 “We%20are%20ha

### 解析

如果我们每替换一次都往后挪后面的所有字符，那么时间复杂度就是O(n^2)的，这样做了很多无用功，因为每个字符重复移动了多次。

更好的做法是先计算后新字符串的长度，然后从字符串的后部开始检查并赋值，这样每个需要移动的字符只需要移动一次就完成了，总得时间复杂度是O(n)。

```c++
void ReplaceBlank(char string[], int length)
{
    if(string == NULL && length <= 0)
        return;

    /*originalLength 为字符串string的实际长度*/
    int originalLength = 0;
    int numberOfBlank = 0;
    int i = 0;
    while(string[i] != '\0')
    {
        ++ originalLength;

        if(string[i] == ' ')
            ++ numberOfBlank;

        ++ i;
    }

    /*newLength 为把空格替换成'%20'之后的长度*/
    int newLength = originalLength + numberOfBlank * 2;
    if(newLength > length)
        return;

    int indexOfOriginal = originalLength;
    int indexOfNew = newLength;
    while(indexOfOriginal >= 0 && indexOfNew > indexOfOriginal)
    {
        if(string[indexOfOriginal] == ' ')
        {
            string[indexOfNew --] = '0';
            string[indexOfNew --] = '2';
            string[indexOfNew --] = '%';
        }
        else
        {
            string[indexOfNew --] = string[indexOfOriginal];
        }

        -- indexOfOriginal;
    }
}
```

## 面试题5：从尾到头打印链表

### 题目

> 输入一个链表的头结点，从尾到头反过来打印出每个结点的值。链表结点定义如下：

```c++
struct ListNode
{
    int       m_nValue;
    ListNode* m_pNext;
};
```

### 解析

两种想法：

1. 要反过来输出的话，可以首先顺着遍历链表并且将每个结点的值放入栈中，**利用栈后进先出的特性**来逐个pop出再输出；

2. 利用递归，递归本质上就是一个栈结构。但是当链表很长的时候，使用递归有可能导致函数调用的层级很深，从而发生函数调用栈溢出的问题。

```c++
void PrintListReversingly_Iteratively(ListNode* pHead)
{
    std::stack<ListNode*> nodes;

    ListNode* pNode = pHead;
    while(pNode != NULL)
    {
        nodes.push(pNode);
        pNode = pNode->m_pNext;
    }

    while(!nodes.empty())
    {
        pNode = nodes.top();
        printf("%d\t", pNode->m_nValue);
        nodes.pop();
    }
}

void PrintListReversingly_Recursively(ListNode* pHead)
{
    if(pHead != NULL)
    {
        if (pHead->m_pNext != NULL)
        {
            PrintListReversingly_Recursively(pHead->m_pNext);
        }

        printf("%d\t", pHead->m_nValue);
    }
}
```

## 面试题6：重建二叉树

### 题目

> 输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列 `{1,2,4,7,3,5,6,8}` 和中序遍历序列 `{4,7,2,1,5,3,8,6}`，则重建出如下所示的二叉树:

```
      1
     / \
    2   3
   /   / \
  4   5   6
   \     /
    7   8
```

> 二叉树结点的定义如下：

```c++
struct BinaryTreeNode
{
    int                    m_nValue;
    BinaryTreeNode*        m_pLeft;
    BinaryTreeNode*        m_pRight;
};
```

### 解析

由中序遍历和前序遍历序列重建树，我们可以每次取出前序遍历序列的第一个点，它对应当前树的根节点。

而在中序遍历序列中，这个点将序列分为左右两部分，左边就是根节点的左子树，右边的右子树。

并且，假设左边有m个点，右边有n个点。那么在前序遍历序列中，第一个点接下来的m个点必然就是左子树，再往后的n个点必然就是右子树。

因此，我们可以利用递归来完成重建，每次都按前序遍历序列的第一个点来划分中序遍历序列，进而划分前序遍历序列，然后分别重建根节点的左右子树即可。

特别要注意**结束条件的设置**。

```c++
BinaryTreeNode* Construct(int* preorder, int* inorder, int length)
{
    if(preorder == NULL || inorder == NULL || length <= 0)
        return NULL;

    return ConstructCore(preorder, preorder + length - 1,
        inorder, inorder + length - 1);
}

BinaryTreeNode* ConstructCore
(
    int* startPreorder, int* endPreorder,
    int* startInorder, int* endInorder
)
{
    // 前序遍历序列的第一个数字是根结点的值
    int rootValue = startPreorder[0];
    BinaryTreeNode* root = new BinaryTreeNode();
    root->m_nValue = rootValue;
    root->m_pLeft = root->m_pRight = NULL;

    // 递归到前序遍历的最后一个点
    if(startPreorder == endPreorder)
    {
        // 递归到中序遍历的最后一个点，并且这个点和前序遍历的最后一个点相同
        if(startInorder == endInorder && *startPreorder == *startInorder)
            return root;
        else
            throw exception();
    }

    // 在中序遍历中找到根结点的值
    int* rootInorder = startInorder;
    while(rootInorder <= endInorder && *rootInorder != rootValue)
        ++ rootInorder;

    // 遍历到最后还是找不到根节点就说明两个遍历序列是不相容的，属于异常输入
    if(rootInorder == endInorder && *rootInorder != rootValue)
        throw exception();

    int leftLength = rootInorder - startInorder;
    int* leftPreorderEnd = startPreorder + leftLength;
    if(leftLength > 0)
    {
        // 构建左子树
        root->m_pLeft = ConstructCore(startPreorder + 1, leftPreorderEnd,
            startInorder, rootInorder - 1);
    }
    if(leftLength < endPreorder - startPreorder)
    {
        // 构建右子树
        root->m_pRight = ConstructCore(leftPreorderEnd + 1, endPreorder,
            rootInorder + 1, endInorder);
    }

    return root;
}
```

## 面试题7：用两个栈实现队列

### 题目

> 用两个栈实现一个队列。队列的声明如下，请实现它的两个函数 appendTail 和 deleteHead，分别完成在队列尾部插入结点和在队列头部删除结点的功能。

```c++
template <typename T> class CQueue
{
public:
    CQueue(void);
    ~CQueue(void);

    // 在队列末尾添加一个结点
    void appendTail(const T& node);

    // 删除队列的头结点
    T deleteHead();

private:
    stack<T> stack1;
    stack<T> stack2;
};
```

### 解析

思路其实很简单，使用两个栈来模拟。每次要往队尾插入一个数，就放到栈1里面。当需要出队，也即删除队首时，只需要把栈1的数依次出栈（顺序是队尾到队首）再放入栈2，这时从栈2pop出的数就是队首了。

注意，并不是每次出队都需要这样操作，我们首先做个判断，如果栈2空了再做。如果栈2没空，那么现在的队首就应该是栈2的栈顶元素了~

```c++
template<typename T> void CQueue<T>::appendTail(const T& element)
{
    stack1.push(element);
}

template<typename T> T CQueue<T>::deleteHead()
{
    if(stack2.size()<= 0)
    {
        while(stack1.size()>0)
        {
            T& data = stack1.top();
            stack1.pop();
            stack2.push(data);
        }
    }

    if(stack2.size() == 0)
        throw exception();

    T head = stack2.top();
    stack2.pop();

    return head;
}
```

## 面试题8：旋转数组的最小数字

### 题目

> 把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。输入一个递增排序的数组的一个旋转，输出旋转数组的最小元素。例如数组 `{3,4,5,1,2}` 为 `{1,2,3,4,5}` 的一个旋转，该数组的最小值为1。

### 解析

这道题其实有点类似于有序数组查找一个数的情况，还是可以**用二分查找的思路**。

旋转数组其实可以分两成两个递增子数组A1和A2，而最小元素就是数组A2的第一个元素。

具体来说，我们使用两个指针p1和p2，分别指向A1的第一个元素和A2的最后一个元素。p1右移，p2左移，不断缩小需要比较的范围。当p1和p2相差1时，p2所指的就是数组的最小元素，这就是终止条件。

怎样使用二分查找的思路缩小范围呢？很简单。假设当前序列的中间序列为m，我们比较一下m和p1所指的元素谁比较大。如果m更大或者与p1相等，就说明m位于A1中，那么p1和m之间的元素我们都不再需要比较了，它们都比p1大，肯定不是最小元素，此时我们可以令p1指向m，得到新的序列，并继续下一次查找。

反过来，如果m比p1所指的元素小，那就说明m位于A2中，此时最小元素在p1和m之间，我们可以令p2指向m来缩小查找范围。

注意了！既然是比大小，那就很有可能出现相等的情况。当p1，p2和m都相等时，我们无法判断m到底属于A1还是A2，也就无法缩小范围。比方说数组 `{1,0,1,1,1}`，如果我们按照先比较p1和m的方式，下一步就变成搜索数组 `{1,1,1}` 了，显然是不对的。

怎样处理这种情况呢？很可惜，没有取巧的方法，遇到这样的情况，我们只能顺序查找当前数组了。

注意，在初始化中间元素时，不应直接初始化为0，而应初始化为p1所指的元素。因为旋转数组有可能是将0个数字旋转到尾部，也即整个数组依然是一个递增数组。如果我们将p1大于等于p2作为循环查找的条件，那就要特别注意这个点。

```c++
int Min(int* numbers, int length)
{
    if(numbers == NULL || length <= 0)
        throw exception();

    int index1 = 0;
    int index2 = length - 1;
    int indexMid = index1; // 如果旋转了0个元素，那么最小元素就是index1所指的，所以这样初始化
    while(numbers[index1] >= numbers[index2])
    {
        // 如果index1和index2指向相邻的两个数，
        // 则index1指向第一个递增子数组的最后一个数字，
        // index2指向第二个子数组的第一个数字，也就是数组中的最小数字
        if(index2 - index1 == 1)
        {
            indexMid = index2;
            break;
        }

        // 如果下标为index1、index2和indexMid指向的三个数字相等，
        // 则只能顺序查找
        indexMid = (index1 + index2) / 2;
        if(numbers[index1] == numbers[index2] && numbers[indexMid] == numbers[index1])
            return MinInOrder(numbers, index1, index2);

        // 缩小查找范围
        if(numbers[indexMid] >= numbers[index1])
            index1 = indexMid;
        else if(numbers[indexMid] <= numbers[index2])
            index2 = indexMid;
    }

    return numbers[indexMid];
}

int MinInOrder(int* numbers, int index1, int index2)
{
    int result = numbers[index1];
    // 按顺序查找
    for(int i = index1 + 1; i <= index2; ++i)
    {
        if(result > numbers[i])
            result = numbers[i];
    }

    return result;
}

```

## 面试题9：斐波拉契数列

### 题目

> 写一个函数，输入n，求斐波拉契（Fibonacci）数列的第n项。斐波拉契数列的定义如下：

![fib1](https://wikimedia.org/api/rest_v1/media/math/render/svg/f00c4321176b6522fe148a11a80a8e5fca9e88da)，初始值：![fib1](https://wikimedia.org/api/rest_v1/media/math/render/svg/6890c552b2226e339520693a236386ed2346a63a)

### 解析

求斐波拉契数列最简单的一种思路是使用递归，但是递归其实是一种效率非常低的解法，举个例子，求数列的第10项，过程如下：

```
             f(10)
            /    \
        f(9)      f(8)
       /    \    /    \
    f(8)  f(7) f(7)  f(6)
...  ...  ...  ...  ...  ...
```

每一项都被分解为左右两棵子树来求，可以看到左右两棵子树会有很多重复的结点，这些都是冗余的。当n相当大时，会有大量的冗余结点，造成求解的效率非常低。

更好的思路是采用O(n)的解法，从f(2)开始逐步往上算，这样每一项只求一次就可以了。

```c++
long long Fibonacci_Solution2(unsigned n)
{
    int result[2] = {0, 1};
    if(n < 2)
        return result[n];

    long long  fibNMinusOne = 1;
    long long  fibNMinusTwo = 0;
    long long  fibN = 0;
    for(unsigned int i = 2; i <= n; ++ i)
    {
        fibN = fibNMinusOne + fibNMinusTwo; // f(n) = f(n-1) + f(n-2)

        // 更新f(n-1) 和 f(n-2)
        fibNMinusTwo = fibNMinusOne;
        fibNMinusOne = fibN;
    }

     return fibN;
}
```

此外还有基于矩阵乘法的O(log n)解法，这里暂不深入探索，感兴趣的话可以在代码文件中查看。

## 面试题10：二进制中1的个数

### 题目

> 请实现一个函数，输入一个整数，输出该数二进制表示中1的个数。例如把9表示成二进制是1001，有2位是1。因此如果输入9，该函数输出2。

### 解析

最简单的一个思路，每次将这个整数的二进制表示右移一位，统计个位是1的次数，数字变为0时结束。

二进制右移一位这个操作其实相当于将整数除以2，但这里我们不使用除法，因为除法效率很低，**在实际任务中，应尽量使用位运算**。另外，判断各位是不是1，只要和1相与就可以了，整数1的二进制表示是31个0并上一个1，因此只有各位是1时，两数相与才会得到1。

但是！上面的思路有一个很严重的问题，题目说的是整数，而不是正整数。c++中整数的表示范围是 `[-2147483648,2147483647]`，用32个bit表示，并且最高位是符号位（正数为0，负数为1）。在右移的时候，因为要保证移位前后符号相同，所以负数高位补的是1。这样数字永远都不会是0，程序会陷入死循环中。

这里补充一点，负数的二进制表示是将对应的正数所有位取反，然后加一。举个例子：

32位的int类型中，

- 正数5的二进制表示为 `00000000000000000000000000000101`；
- 而负数-5的二进制表示就是 `11111111111111111111111111111011`。

- 最大整数2147483647的二进制表示是    `01111111111111111111111111111111`；
- 对应负数-2147483647的二进制表示则是 `10000000000000000000000000000001`。

特别地，最小负数-2147483648的二进制表示是 `10000000000000000000000000000000`。

注意了，这些表示是要看数值类型所用的位数来定的。

在了解了这些情况之后，可以想到一个新的思路。设定unsigned int类型的变量flag为1，我们可以把对输入的整数进行右移改为对flag进行左移，每一次相与之后就把flag左移一位，对于flag来说，左移32位后就变为0了，我们可以利用这点作为循环结束的条件。代码如下：

```c++
int NumberOf1_Solution1(int n)
{
    int count = 0;
    unsigned int flag = 1;
    while(flag)
    {
        if(n & flag)
            count ++;

        flag = flag << 1;
    }

    return count;
}
```

不难发现，上面的解法是根据数值类型来决定循环次数的，一个int类型的数就需要循环32次。有没有办法再减少次数呢？有的！

我们先做个分析，一个数减去1之后，它的二进制表示有两种可能的变化：

- 最右边一位是1，则最右边一位变0，其他位不变；
- 最右边一位没有1，则最靠右的1（只要一个数不为0，则至少存在一位是1）变0，该位的右边都变为1。

如果我们把减去1之后的数和原来的数相与，会发生什么呢？可以发现，这样操作实质是**把原来数字最靠右的1变为0，其余位不变**！于是，一个整数的二进制表示**有多少个1就可以进行多少次这样的操作**，大大减少了循环的次数！

```c++
int NumberOf1_Solution2(int n)
{
    int count = 0;

    while (n)
    {
        ++ count;
        n = (n - 1) & n;
    }

    return count;
}
```
