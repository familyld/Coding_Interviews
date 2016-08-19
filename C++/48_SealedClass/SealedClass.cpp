#include <cstdio>
using namespace std;

// ====================方法一====================
class SealedClass1
{
public:
    static SealedClass1* GetInstance()
    {
        return new SealedClass1();
    }

    static void DeleteInstance( SealedClass1* pInstance)
    {
        delete pInstance;
    }

private:
    SealedClass1() {}
    ~SealedClass1() {}
};

// 如果试图从SealedClass1继承出新的类型，
// 将会导致编译错误。
/*
class Try1 : public SealedClass1
{
public:
    Try1() {}
    ~Try1() {}
};
*/

// ====================方法二====================
template <typename T> class MakeSealed
{
    friend T;

private:
    MakeSealed() {}
    ~MakeSealed() {}
};

class SealedClass2 : virtual public MakeSealed<SealedClass2>
{
public:
    SealedClass2() {}
    ~SealedClass2() {}
};

// 如果试图从SealedClass1继承出新的类型，
// 将会导致编译错误。
/*
class Try2 : public SealedClass2
{
public:
    Try2() {}
    ~Try2() {}
};
*/

int main()
{
    return 0;
}

