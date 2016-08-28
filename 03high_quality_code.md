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
