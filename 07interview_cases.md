# 两个面试案例

## 面试题49：把字符串转换成整数

### 题目

> 写一个函数StrToInt，实现把字符串转换为整数这个功能。不能使用atoi或者其他类似的库函数。

### 解析

这条题看似简单，要把数字字符转为int型数值，只需要减去字符0就可以了。但是实际上这里有很多需要考虑的状况，这里简单地概括一下：

1. 能检测出非法的输入，包括空指针、空字符串、只有正号/负号没有数字、数字部分出现非数字字符。
2. 输入非法时，依然能返回一个整数（比方说0），而且能和正常返回该整数的状况区别开来。因此还要设置一个全局标志位（atoi就是这样做的）。
3. 字符串表示的数字超出int型的表示范围时，同样属于非法输入，程序能够检测到并做恰当的处理。

```c++
enum Status {kValid = 0, kInvalid};
// 全局标志，输入无效字符串时函数返回0，该标志用于和输入字符串'0'作区分
int g_nStatus = kValid;

int StrToInt(const char* str)
{
    g_nStatus = kInvalid;
    long long num = 0; // 使用long long来获取转换的数，然后再判断是否超出int型（发生溢出）

    if(str != NULL && *str != '\0') // 避免空指针和空字符串的情况
    {
        // 取出正负号，并移动到数字部分的第一位
        // 如果只有正负号没有数字则会移动到结束符'\0'，不会继续转换
        bool minus = false;
        if(*str == '+')
            str ++;
        else if(*str == '-')
        {
            str ++;
            minus = true;
        }

        if(*str != '\0')
        {
            num = StrToIntCore(str, minus);
        }
    }

    return (int)num; //记得最后要转换为int型返回
}

long long StrToIntCore(const char* digit, bool minus)
{
    long long num = 0;

    while(*digit != '\0')
    {
        if(*digit >= '0' && *digit <= '9')
        {
            int flag = minus ? -1 : 1;
            num = num * 10 + flag * (*digit - '0');

            // 判断是否超出了int型的表示范围，也即发生溢出
            if((!minus && num > 0x7FFFFFFF)
                || (minus && num < (signed int)0x80000000))
            {
                num = 0;
                break;
            }

            digit++;
        }
        else // 一旦遇到非数字字符，就退出并返回0
        {
            num = 0;
            break;
        }
    }

    // 成功转换完成，把全局标志置为有效
    if(*digit == '\0')
    {
        g_nStatus = kValid;
    }

    return num;
}
```
