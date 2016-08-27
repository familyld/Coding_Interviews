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
