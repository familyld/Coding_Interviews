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
