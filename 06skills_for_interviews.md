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
