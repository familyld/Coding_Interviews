# 面试题1：赋值运算符函数

> 题目：如下为类型CMyString的声明，请为该类型添加赋值运算符函数。

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
