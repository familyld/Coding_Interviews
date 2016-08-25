# 面试的流程

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
