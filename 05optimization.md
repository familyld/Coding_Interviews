# 优化时间和空间效率

## 面试题29：数组中出现次数超过一半的数字

> 题目：数组中有一个数字出现的次数超过数组长度的一半，请找出这个数字。例如输入一个长度为9的数组 `{1，2，3，2，2，2，5，4，2}`。由于数字2在数组中出现了5次，超过数组长度的一半，因此输出2。

### 解析

其实对于一个排序数组来说，要找出这个数字是很简单的，只用遍历一次数组来计数就可以了。但是要排序的话时间复杂度至少也是O(n*logn)，有没有O(n)的方法呢？

有的，首先介绍一种基于partition函数的方法。partition函数用于在快排中把一个数字放到合适的位置上，使得这个数字的左边都比它小，右边都比它大。题目要求找出出现次数超过一半的数字，那么这个数字在排序数组中有什么特点呢？答案是**出现次数超过数组长度的一半的数字必然会是中位数**。

于是，我们就可以通过多次调用partition函数来找这个数。partition函数随机选取范围内的一个数放到合适的位置，如果位置比中位数前，那就缩小范围到这个数和数组末尾之间；如果比中位数后，那就缩小范围到数组开头到这个数之间；如果这个位置恰是中位数的位置，那就成功找到了。

注意，上面只是一种**快速找到中位数**的方法，出现次数有没有超过数组长度的一半，还得遍历一次数组对这个数的出现次数进行计数。

基于partition函数的方法算法复杂度分析有一定困难，但总的来说还是可以认为是O(n)的算法。有一个缺点是**需要改变数组**，这在实际任务中不一定被允许。

下面再介绍另一种更直观的O(n)解法。

所谓“出现的次数超过数组长度的一半”，其实可以理解为**这个数字出现的次数比其他全部数字出现次数的总和还要多**。基于这个思路，我们可以想出下面的解法：

1. 初始化变量result为数组的第一个元素，初始化变量times为1
2. 遍历数组
    - 如果times为0，把result换为当前数字
    - 如果result不等于当前数字，则times-1
    - 如果result等于当前数字，则times+1
3. 检查result出现次数是否超过数组长度的一半

如果数组中真的存在一个数出现的次数超过数组长度的一半，那么它必定会是最后的result。但是，注意了，是不是最后的result一定就是题目要求的数字呢？并不是的！有可能数组根本就没有这样的数字，所以类似上一种解法，我们最后要进行一次遍历检查。

```c++
bool g_bInputInvalid = false;

bool CheckInvalidArray(int* numbers, int length)
{
    g_bInputInvalid = false;
    if(numbers == NULL && length <= 0)
        g_bInputInvalid = true;

    return g_bInputInvalid;
}

bool CheckMoreThanHalf(int* numbers, int length, int number)
{
    int times = 0;
    for(int i = 0; i < length; ++i)
    {
        if(numbers[i] == number)
            times++;
    }

    bool isMoreThanHalf = true;
    if(times * 2 <= length)
    {
        g_bInputInvalid = true;
        isMoreThanHalf = false;
    }

    return isMoreThanHalf;
}
```

## 面试题30：最小的k个数

> 题目：输入n个整数，找出其中最小的k个数。例如输入4、5、1、6、2、7、3、8这8个数字，则最小的4个数字是1、2、3、4。

### 解析

和上一题那样，我们可以使用O(n)的基于partition函数的解法，因为题目只要求找出最小的k个数，没有说这k个数还得排序好返回。所以只要partition函数返回的下标是k-1就结束了，如果不是就根据情况缩小范围即可。

```c++
// ====================方法1====================
void GetLeastNumbers_Solution1(int* input, int n, int* output, int k)
{
    if(input == NULL || output == NULL || k > n || n <= 0 || k <= 0)
        return;

    int start = 0;
    int end = n - 1;
    int index = Partition(input, n, start, end);
    while(index != k - 1)
    {
        if(index > k - 1)
        {
            end = index - 1;
            index = Partition(input, n, start, end);
        }
        else
        {
            start = index + 1;
            index = Partition(input, n, start, end);
        }
    }

    for(int i = 0; i < k; ++i)
        output[i] = input[i];
}
```

基于partition函数求解一个不好的地方就是会改变数组。有没有一种不改变数组但又时间复杂度相对小的解法呢？有的，就是**基于最大堆**的算法，复杂度为O(n logk)，k设定为堆的大小。

具体来说，我们依次读入数组的n个整数。如果最大堆中已有的数字少于k个，则直接把这次读入的整数放入容器中；如果最大堆中已经满了，则把堆顶（最大元素）和这次读入的整数作比较，如果堆顶较小则不需修改，堆顶较大则删除堆顶，并且插入这次读入的整数。

这里不讨论怎样实现最大堆，直接使用C++中的multiset来实现（multiset与set都是**基于红黑树实现**的，区别在于**multiset允许有重复元素**），可以把它看成一个序列，插入/删除都能在O(logk)的时间内完成，而且**能时刻保证序列中的数是有序的**。

基于最大堆的解法比基于partition的解法要慢一些，但是！这种解法不仅不需要改变原数组（所有操作都在堆里进行），而且**适合处理海量数据**。因为数据太多时无法一次载入内存，只能从辅助存储空间（比如硬盘）分批读入，而基于最大堆的解法就允许我们每次读入一个数字，直到遍历完毕也就处理完了（当然，k还是不能超出内存的限制的）。

```c++
// ====================方法2====================
typedef multiset<int, greater<int> >            intSet;
typedef multiset<int, greater<int> >::iterator  setIterator;

void GetLeastNumbers_Solution2(const vector<int>& data, intSet& leastNumbers, int k)
{
    leastNumbers.clear();

    if(k < 1 || data.size() < k)
        return;

    vector<int>::const_iterator iter = data.begin();
    for(; iter != data.end(); ++ iter)
    {
        if((leastNumbers.size()) < k)
            leastNumbers.insert(*iter);

        else
        {
            setIterator iterGreatest = leastNumbers.begin();

            if(*iter < *(leastNumbers.begin()))
            {
                leastNumbers.erase(iterGreatest);
                leastNumbers.insert(*iter);
            }
        }
    }
}
```

## 面试题31：连续子数组的最大和

### 题目

> 输入一个整型数组，数组里有正数也有负数。数组中一个或连续的多个整数组成一个子数组。求所有子数组的和的最大值。要求时间复杂度为O(n)。

### 解析

这题初看之下，似乎有些难度。但实际上思路是很简单的，使用两个变量存储当前子数组的和（初始化为0）以及最大的和（初始化为0x80000000，即int类型的最小负数），求解的过程可以分为以下几种情况：

1. 当前和为负，此时如果把新的数字并入连续子数组，所得的和还没有这个数字本身大。所以后续再并入其它数字组成数组的和要比单独这个数字并入后续数字组成数组的和小。于是把当前连续子数组舍弃掉，然后往空数组并入新的数字即可（也即把当前子数组的和设为新数字的值）；

2. 当前和非负，这时直接累加新数字就可以了，即使新数字是负数，后续加入其它数字连成的数组也有可能产生更大的和；

3. 如果新产生的和比最大和要大，那就更新最大和。

最后，应该注意，**输入不合法时应返回什么**。这里设定返回0，并且为了和最大和为0的情况区分开，采用了一个全局标志来进行标识。

```c++
bool g_InvalidInput = false;

int FindGreatestSumOfSubArray(int *pData, int nLength)
{
    if((pData == NULL) || (nLength <= 0))
    {
        g_InvalidInput = true;
        return 0;
    }

    g_InvalidInput = false;

    int nCurSum = 0;
    int nGreatestSum = 0x80000000; // 初始化为最小负数
    for(int i = 0; i < nLength; ++i)
    {
        if(nCurSum <= 0)
            nCurSum = pData[i];
        else
            nCurSum += pData[i];

        if(nCurSum > nGreatestSum)
            nGreatestSum = nCurSum;
    }

    return nGreatestSum;
}
```

## 面试题32：从1到n整数中1出现的次数

### 题目

> 输入一个整数n，求从1到n这n个整数的十进制表示中1出现的次数。例如输入12，从1到12这些整数中包含1的数字有1，10，11和12，1一共出现了5次。

### 解析

不考虑时间复杂度，最简单的方法就是逐个数字判断，每次判断完各位是否1之后把数字除以10，当数字变为0时结束。

```c++
// ====================方法一====================
int NumberOf1(unsigned int n);

int NumberOf1Between1AndN_Solution1(unsigned int n)
{
    int number = 0;

    for(unsigned int i = 1; i <= n; ++ i)
        number += NumberOf1(i);

    return number;
}

int NumberOf1(unsigned int n)
{
    int number = 0;
    while(n) // 当n不为0时
    {
        if(n % 10 == 1)
            number ++;

        n = n / 10;
    }

    return number;
}
```

这里介绍一种考虑数字规律的O(log n)方法，也即复杂度视输入数字n的位数（等于log10(n)+1下取整）而定。

将求从1到n这n个整数的十进制表示中1出现的次数这个任务分为三个子任务来完成：

1. 统计 n去掉最高位+1 ~ n 这段范围内最高位出现的1
2. 统计 n去掉最高位+1 ~ n 这段范围内除最高位之外出现的1
3. 统计 1 ~ n去掉最高位 这段范围内出现的1

那么具体来说怎么完成呢？

首先，对于 n去掉最高位+1 ~ n 这段范围内最高位出现的1，有两种情况：

1. 最高位大于1，此时所有最高位为1的情况都包含了，最高位为1的次数等于 `10^(位数-1)`
    - 比如22，十位为1的10~19全部都被包含了，十位为1的次数是 `10^(2-1)=10（次）`
2. 最高位等于1，此时最高位为1的次数等于 `n去掉最高位+1`
    - 比如12，去掉十位是2，十位为1的次数是 `2+1=3（次）`

然后，对于 n去掉最高位+1 ~ n 这段范围内除最高位之外出现的1，我们使用公式 `最高位*（位数-1）*10^(位数-2)` 次方。

首先，假设最高位数字为k，则 n去掉最高位+1 ~ n 这段范围可以被等分为k份。例如n=3721，则k=3，此时 722~3721 可以等分为3份，也即 722~1721，1722~2721 以及 2722~3721。又因为对于这3段范围来说，除最高位外出现1的次数都是相同的（最高位已经在第一个子任务中算好了，这里不用管），所以我们只需要算出其中一段就可以了，这是公式第一项的意义。

然后，假设n是一个d位数，则在去掉最高位后剩下的就是d-1位数字，我们在d-1位数字中选择一位固定为1，有d-1种选择，这是公式第二项的意义。

最后，在固定了其中一位数字为1之后，剩余的d-2位数字按照排列组合，每位数字都可以选择0~9中的一个，所以一共有 `10^(d-2)` 中组合，这是公式第三项的意义。

看到这里可能还有小小的迷糊，按照上面排列组合的方法计算，那么3111这个数在子任务2中不就被计算了3次吗？事实上，这并没有错，因为题目求得是1出现的次数，而不是出现1的数字有多少个？所以，除最高位外3111出现了3个1，在子任务2中计数为3是正确的。

最后的最后，我们还要求子任务3，统计 1 ~ n去掉最高位 这段范围内出现的1，这个使用递归求解就可以了，也即把n去掉最高位作为下一次递归传入的n。注意设置好终止条件，最好到达个位数时就停止了。若个位为0，则返回0；若各位大于0，则返回1。

```c++
// ====================方法二====================
int NumberOf1(const char* strN);
int PowerBase10(unsigned int n);

int NumberOf1Between1AndN_Solution2(int n)
{
    if(n <= 0)
        return 0;

    char strN[50];
    sprintf(strN, "%d", n); //int转字符串

    return NumberOf1(strN);
}

int NumberOf1(const char* strN)
{
    if(!strN || *strN < '0' || *strN > '9' || *strN == '\0')
        return 0;

    int first = *strN - '0';
    unsigned int length = static_cast<unsigned int>(strlen(strN));

    if(length == 1 && first == 0) // 个位为0
        return 0;

    if(length == 1 && first > 0) // 个位为1
        return 1;

    // 假设strN是"21345"
    // numFirstDigit是数字10000-19999的第一个位中1的数目
    int numFirstDigit = 0;
    if(first > 1)
        numFirstDigit = PowerBase10(length - 1);
    else if(first == 1)
        numFirstDigit = atoi(strN + 1) + 1;

    // numOtherDigits是01346-21345除了第一位之外的数位中1的数目
    int numOtherDigits = first * (length - 1) * PowerBase10(length - 2);
    // numRecursive是1-1345中1的数目
    int numRecursive = NumberOf1(strN + 1);

    return numFirstDigit + numOtherDigits + numRecursive;
}

int PowerBase10(unsigned int n)
{
    int result = 1;
    for(unsigned int i = 0; i < n; ++ i)
        result *= 10;

    return result;
}
```

## 面试题33：把数组排成最小的数

### 题目

> 输入一个正整数数组，把数组里所有数字拼接起来排成一个数，打印能拼接处的所有数字中最小的一个。例如输入数组 `{3,32,321}`，则打印出这3个数字能排成的最小数字321323.

### 解析

这题如果我们采用全排列的方式，n个数字就有n!个排列，时间复杂度相当大。

那么这条题目实际我们做什么呢？其实是希望我们**实现一个比较函数**。

在这里我们并不是简单地比较两个数字的大小，而是要比较它们拼接的时候谁排在前面能产生更小的数。要实现这样的功能，我们可以先将两个数字转换为字符串，然后使用strcat来进行拼接，使用strcmp来进行比较。

实现了比较函数之后，我们可以使用qsort快速排序在O(n logn)的时间内完成排序，此时按顺序输出数组的数字得到的就是最小的数。

在这里简单再减少以下几个函数：

1. `sprintf(a, "%d", b)` 将int*类型的b转换为char*类型的a，也即整数数组转字符串；
2. `strcpy(a,b)` 将b复制到a；
3. `strcat(a,b)` 将b拼接到a后面；
4. `strcmp(a,b)` 对a和b两个字符串从左向右逐个字符相比（按ASCII值大小相比较），直到出现不同的字符或遇'\0'为止。若a>b则返回正数，a<b则返回负数，相同则返回0。

```c++
// int型整数用十进制表示最多只有10位
const int g_MaxNumberLength = 10;

char* g_StrCombine1 = new char[g_MaxNumberLength * 2 + 1];
char* g_StrCombine2 = new char[g_MaxNumberLength * 2 + 1];

void PrintMinNumber(int* numbers, int length)
{
    if(numbers == NULL || length <= 0)
        return;

    char** strNumbers = (char**)(new int[length]);
    for(int i = 0; i < length; ++i)
    {
        strNumbers[i] = new char[g_MaxNumberLength + 1];
        sprintf(strNumbers[i], "%d", numbers[i]);
    }

    qsort(strNumbers, length, sizeof(char*), compare);

    for(int i = 0; i < length; ++i)
        printf("%s", strNumbers[i]);
    printf("\n");

    for(int i = 0; i < length; ++i)
        delete[] strNumbers[i];
    delete[] strNumbers;
}

// 如果[strNumber1][strNumber2] > [strNumber2][strNumber1], 返回值大于0
// 如果[strNumber1][strNumber2] = [strNumber2][strNumber1], 返回值等于0
// 如果[strNumber1][strNumber2] < [strNumber2][strNumber1], 返回值小于0
int compare(const void* strNumber1, const void* strNumber2)
{
    // [strNumber1][strNumber2]
    strcpy(g_StrCombine1, *(const char**)strNumber1);
    strcat(g_StrCombine1, *(const char**)strNumber2);

    // [strNumber2][strNumber1]
    strcpy(g_StrCombine2, *(const char**)strNumber2);
    strcat(g_StrCombine2, *(const char**)strNumber1);

    return strcmp(g_StrCombine1, g_StrCombine2);
}
```

## 面试题34：丑数

### 题目

> 我们把只包含因子2、3和5的数称作丑数（Ugly Number）。求按小到大的顺序的第1500个丑数。例如6、8都是丑数，但14不是，因为它包含因子7。习惯上我们把1当做第一个丑数。

### 解析

最简单的解法，写一个函数用于判断是否丑数（需要求模和做除法），然后从1开始依次判断是否丑数，直到得到1500个丑数时停止。这种方法的时间开销很大，因为即使一个数不是丑数，我们仍然要对它进行判断，做求模和除法这些开销较大的操作。

在允许使用额外空间时，我们不妨**用空间换时间**。按照丑数的定义，**丑数应该是另一个丑数乘以2、3或者5的结果（1除外）**。所以我们可以创建一个数组，里面的数字是排好序的丑数，每个丑数都是前面的丑数乘以2、3或者5得到的。

现在关键在于**怎么确保数组里的丑数是排好序的**？

假设我们有一个排好序的丑数数组，最大的一个丑数为M，则我们在生成下一个丑数时，我们会把已有丑数都乘2，乘3和乘5，这会产生一些小于等于M的数字，但由于我们是按序生成的，所以我们并不care这些数字。要继续有序生成，我们需要找到第一个比M大的数字。对于乘2操作来说，把第一个比M大的数字的数字记为M2；对于乘3操作来说，把第一个比M大的数字的数字记为M3；对于乘5操作来说，把第一个比M大的数字的数字记为M5。那么数组的下一个丑数必然是M2，M3，M5这三个数中的最小者。

在实现的时候，我们可以用三个指针来分别记录M2/2，M3/3和M5/5的坐标，在下一次产生丑数时，我们就不需要再从1开始逐个数乘2，乘3和乘5了，因为下一个丑数必然是这三个指针以后的数乘2，乘3或乘5产生的。

```c++
int GetUglyNumber_Solution2(int index)
{
    if(index <= 0)
        return 0;

    int *pUglyNumbers = new int[index];
    pUglyNumbers[0] = 1;
    int nextUglyIndex = 1;

    int *pMultiply2 = pUglyNumbers;
    int *pMultiply3 = pUglyNumbers;
    int *pMultiply5 = pUglyNumbers;

    while(nextUglyIndex < index)
    {
        int min = Min(*pMultiply2 * 2, *pMultiply3 * 3, *pMultiply5 * 5);
        pUglyNumbers[nextUglyIndex] = min;

        while(*pMultiply2 * 2 <= pUglyNumbers[nextUglyIndex])
            ++pMultiply2;
        while(*pMultiply3 * 3 <= pUglyNumbers[nextUglyIndex])
            ++pMultiply3;
        while(*pMultiply5 * 5 <= pUglyNumbers[nextUglyIndex])
            ++pMultiply5;

        ++nextUglyIndex;
    }

    int ugly = pUglyNumbers[nextUglyIndex - 1];
    delete[] pUglyNumbers;
    return ugly;
}

int Min(int number1, int number2, int number3)
{
    int min = (number1 < number2) ? number1 : number2;
    min = (min < number3) ? min : number3;

    return min;
}
```

## 面试题35：第一个只出现一次的字符

### 题目

> 在字符串中找出第一个只出现一次的字符。如输入 `"abaccdeff"`，则输出 `'b'`。

### 解析

对于统计次数的题目，我们很容易想到使用哈希表来实现。c++的标准模版库中并没有哈希表的实现，当我们可以利用一个int型数组来实现，因为char类型中每个字符是8bits的，也即有256种可能，我们可以用一个长度为256的数组来模拟。

有了哈希表，我们只需要扫描2次字符串就可以实现题目要求了，也即时间复杂度为O(n)。具体来说，第一次扫描，我们统计每个字符的次数；第二次扫描则搜索哈希表中的次数，第一次出现次数为1时就代表找到了。

特别注意一下，哈希表要进行初始化，另外，如果没有只出现1次的字符，要明确该返回什么。

```c++
char FirstNotRepeatingChar(char* pString)
{
    if(pString == NULL)
        return '\0';

    const int tableSize = 256;
    unsigned int hashTable[tableSize];
    // 初始化哈希表
    for(unsigned int i = 0; i<tableSize; ++ i)
        hashTable[i] = 0;

    // 第一次扫描字符串
    // 统计各字符的出现数目
    char* pHashKey = pString;
    while(*(pHashKey) != '\0')
        hashTable[*(pHashKey++)] ++;

    // 第二次扫描字符串
    // 依次把字符作为key检索哈希表
    pHashKey = pString;
    while(*pHashKey != '\0')
    {
        if(hashTable[*pHashKey] == 1)
            return *pHashKey;

        pHashKey++;
    }

    return '\0';
}
```

## 面试题36：数组中的逆序对

### 题目

> 在数组中的两个数字如果前面一个数字大于后面的数字，则这两个数字组成一个逆序对。输入一个数组，求出这个数组中的逆序对的总数。

### 解析

最简单的解法是逐个扫描数组的数字，对每个数字都检查其后的所有数字来统计逆序对，复杂度是O(n^2)。

但是，这种解法显然太过耗费时间。有没有更快的方法呢？有的，采用**归并排序**的思想，我们可以借助一个和原数组等长的辅助数组来实现O(n logn)时间复杂度的解法。

具体来说，过程如下：

1. 把原数组对半分为两个子数组，对子数组继续对半分，直到子数组只包含单个数字，然后开始对相邻的子数组进行合并。

2. 在合并时，我们从两个子数组的最后一位开始往前扫描（两个子数组都是递增数组），每次选择较大的一位放入辅助数组（从辅助数组的最后开始赋值，保证生成的也是递增数组）。
    - 如果靠前的子数组数字较大，那就代表这合并后它会和靠后子数组剩余的数字（都比它小）构成逆序对，此时总逆序对数目应加上靠后子数组剩余数字的数目。
    - 如果靠后的子数组数字较大，就代表不构成逆序对，直接合并到辅助数组就可以了。

3. 如此经过多轮的分拆和合并，最后就能得到一个递增排序的辅助数组，同时统计好了逆序对的数目。

再举个简单例子来捋顺思路吧，假设我们要统计数组 `7,5,6,4` 的逆序对数目，步骤如下：

1. 对半分数组 `7,5,6,4`，得到子数组 `7,5` 和 `6,4`；
2. 对半分数组 `7,5`，得到子数组 `7` 和 `5`；
3. 合并子数组 `7` 和 `5`，7比5大，逆序对加一，7放入辅助数组，子数组1空了，把子数组2的数字也放入辅助数组，得到辅助数组 `5,7`；
4. 对半分数组 `6,4`，得到子数组 `6` 和 `4`；
5. 合并子数组 `6` 和 `4`，6比4大，逆序对加一，6放入辅助数组，子数组1空了，把子数组2的数字也放入辅助数组，得到辅助数组 `4,6`；
6. 子数组 `5,7` 和 `4,6`，7比6大，**逆序对加二**，7放入辅助数组。5比6小，6放入辅助数组。5比4大，逆序对加一，5放入辅助 数组，子数组1空了，把子数组2的数字也放入辅助数组，得到辅助数组 `4,5,6,7`；
7. 合并完成，得到递增数组 `4,5,6,7`，逆序对数目为 `1+1+2+1=5（个）`。

当然最好还是自己拿着纸举例分析来体验一下。假设我们把每一次归并中，用于合并的两个子数组称为数据数组，合并好的数组称为辅助数组。那么在转化为代码的过程中，**合并出这一轮辅助数组用的是在获取这一轮两个数据数组时用的辅助数组**。比方说上面例子中，步骤6用的两个数据数组其实就是步骤3和步骤5中得到的辅助数组 `5,7` 和辅助数组 `4,6`。

**那我们是否需要使用多个长度不一的辅助数组呢**？不是的，我们用一个和输入数组等长的辅助数组就够了，只是用的方式有一点巧妙。在传递参数时，把子数组在原数组中start和end的索引也作为参数传递就可以了，这样相当于截取了原数组的一段。

```c++
int InversePairs(int* data, int length)
{
    if(data == NULL || length < 0)
        return 0;

    int* copy = new int[length];
    for(int i = 0; i < length; ++ i)
        copy[i] = data[i];

    int count = InversePairsCore(data, copy, 0, length - 1);
    delete[] copy;

    return count;
}

int InversePairsCore(int* data, int* copy, int start, int end)
{
    // 当分割到子数组只剩1个数字时，直接合并入辅助数组，逆序对数目为0
    if(start == end)
    {
        copy[start] = data[start];
        return 0;
    }

    // 把当前数组分割成两部分
    // 注意data和copy作为参数的位置是换过来的
    // 因为这一轮用于归并的数据数组是两个子数组归并好的的结果（它们那一轮的辅助数组）
    int length = (end - start) / 2;
    int left = InversePairsCore(copy, data, start, start + length);
    int right = InversePairsCore(copy, data, start + length + 1, end);

    // i初始化为前半段最后一个数字的下标
    int i = start + length;
    // j初始化为后半段最后一个数字的下标
    int j = end;

    // 辅助数组的指针初始化在最后
    int indexCopy = end;
    // 逆序对数目初始化为0
    int count = 0;
    // 合并两个子数组到辅助数组，从末尾开始比较，到达其中一方的头部时停止
    while(i >= start && j >= start + length + 1)
    {
        if(data[i] > data[j])
        {
            copy[indexCopy--] = data[i--];
            count += j - start - length;
        }
        else
        {
            copy[indexCopy--] = data[j--];
        }
    }

    // 把另一方余下的数字也合并到辅助数组中
    for(; i >= start; --i)
        copy[indexCopy--] = data[i];

    for(; j >= start + length + 1; --j)
        copy[indexCopy--] = data[j];

    // 逆序对总数由三部分组成，两个子数组各自的逆序对数目加上合并它们产生的逆序对数目
    return left + right + count;
}
```

## 面试题37：两个链表的第一个公共结点

### 题目

> 输入两个链表，找出它们的第一个公共结点。链表结点定义如下：

```c++
struct ListNode
{
    int       m_nValue;
    ListNode* m_pNext;
};
```

### 解析

最呆萌的做法就是依次遍历其中一个链表的结点，每访问一个结点就遍历一次另一个链表，知道找到两个链表的公共结点。如果两个链表长度分别为m和n，这种方法的时间复杂度就是O(mn)。

当然不用这么笨的方法.. 我们先观察一下两个有公共结点的链表有什么特点：

```c++
1 -> 2 -> 3
           \
            6 -> 7
           /
     4 -> 5
```

不难发现，**两个有公共结点的链表从第一个公共结点到链表尾都是一样的**。而且，如果两个链表相差k步，我们在长链表上先走k步，然后再同时遍历两个链表，就能很容易地找到第一个公共结点（如果没有则会一起走到链表尾的NULL）。

基于这个思路，实现代码并不难：

```c++
ListNode* FindFirstCommonNode( ListNode *pHead1, ListNode *pHead2)
{
    // 得到两个链表的长度
    unsigned int nLength1 = GetListLength(pHead1);
    unsigned int nLength2 = GetListLength(pHead2);
    int nLengthDif = nLength1 - nLength2;

    ListNode* pListHeadLong = pHead1;
    ListNode* pListHeadShort = pHead2;
    if(nLength2 > nLength1)
    {
        pListHeadLong = pHead2;
        pListHeadShort = pHead1;
        nLengthDif = nLength2 - nLength1;
    }

    // 先在长链表上走几步，再同时在两个链表上遍历
    for(int i = 0; i < nLengthDif; ++ i)
        pListHeadLong = pListHeadLong->m_pNext;

    while((pListHeadLong != NULL) &&
        (pListHeadShort != NULL) &&
        (pListHeadLong != pListHeadShort))
    {
        pListHeadLong = pListHeadLong->m_pNext;
        pListHeadShort = pListHeadShort->m_pNext;
    }

    // 得到第一个公共结点
    ListNode* pFisrtCommonNode = pListHeadLong;

    return pFisrtCommonNode;
}

unsigned int GetListLength(ListNode* pHead)
{
    unsigned int nLength = 0;
    ListNode* pNode = pHead;
    while(pNode != NULL)
    {
        ++ nLength;
        pNode = pNode->m_pNext;
    }

    return nLength;
}
```
