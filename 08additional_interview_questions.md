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
