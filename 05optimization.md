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
    sprintf(strN, "%d", n);

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
