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
