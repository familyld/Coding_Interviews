#include "..\utilities\StringUtil.h"
#include <cstring>
#include <cstdio>
using namespace std;

char* LeftRotateString(char* pStr, int n)
{
    if(pStr != NULL)
    {
        int nLength = static_cast<int>(strlen(pStr));
        if(nLength > 0 && n > 0 && n < nLength)
        {
            char* pFirstStart = pStr;
            char* pFirstEnd = pStr + n - 1;
            char* pSecondStart = pStr + n;
            char* pSecondEnd = pStr + nLength - 1;

            // ·­×ª×Ö·û´®µÄÇ°Ãæn¸ö×Ö·û
            Reverse(pFirstStart, pFirstEnd);
            // ·­×ª×Ö·û´®µÄºóÃæ²¿·Ö
            Reverse(pSecondStart, pSecondEnd);
            // ·­×ªÕû¸ö×Ö·û´®
            Reverse(pFirstStart, pSecondEnd);
        }
    }

    return pStr;
}

// ====================²âÊÔ´úÂë====================
void Test(char* testName, char* input, int num, char* expectedResult)
{
    if(testName != NULL)
        printf("%s begins: ", testName);

    char* result = LeftRotateString(input, num);

    if((input == NULL && expectedResult == NULL)
        || (input != NULL && strcmp(result, expectedResult) == 0))
        printf("Passed.\n\n");
    else
        printf("Failed.\n\n");
    printf("\n");
}

// ¹¦ÄÜ²âÊÔ
void Test1()
{
    char input[] = "abcdefg";
    char expected[] = "cdefgab";

    Test("Test1", input, 2, expected);
}

// ±ß½çÖµ²âÊÔ
void Test2()
{
    char input[] = "abcdefg";
    char expected[] = "bcdefga";

    Test("Test2", input, 1, expected);
}

// ±ß½çÖµ²âÊÔ
void Test3()
{
    char input[] = "abcdefg";
    char expected[] = "gabcdef";

    Test("Test3", input, 6, expected);
}

// Â³°ôÐÔ²âÊÔ
void Test4()
{
    Test("Test4", NULL, 6, NULL);
}

// Â³°ôÐÔ²âÊÔ
void Test5()
{
    char input[] = "abcdefg";
    char expected[] = "abcdefg";

    Test("Test5", input, 0, expected);
}

// Â³°ôÐÔ²âÊÔ
void Test6()
{
    char input[] = "abcdefg";
    char expected[] = "abcdefg";

    Test("Test6", input, 7, expected);
}

int main()
{
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();

    return 0;
}

