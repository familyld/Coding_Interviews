#include <cstdio>
#include "StackWithMin.h"
using namespace std;

void Test(char* testName, const StackWithMin<int>& stack, int expected)
{
    if(testName != NULL)
        printf("%s begins: ", testName);

    if(stack.min() == expected)
        printf("Passed.\n");
    else
        printf("Failed.\n");
    printf("\n");
}

int main()
{
    StackWithMin<int> stack;

    stack.push(3);
    Test("Test1", stack, 3);

    stack.push(4);
    Test("Test2", stack, 3);

    stack.push(2);
    Test("Test3", stack, 2);

    stack.push(3);
    Test("Test4", stack, 2);

    stack.pop();
    Test("Test5", stack, 2);

    stack.pop();
    Test("Test6", stack, 3);

    stack.pop();
    Test("Test7", stack, 3);

    stack.push(0);
    Test("Test8", stack, 0);

    return 0;
}

