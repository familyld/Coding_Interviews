#include "../utilities/List.h"
#include <stack>
#include <cstdio>
using namespace std;

void PrintListReversingly_Iteratively(ListNode* pHead)
{
    std::stack<ListNode*> nodes;

    ListNode* pNode = pHead;
    while(pNode != NULL)
    {
        nodes.push(pNode);
        pNode = pNode->m_pNext;
    }

    while(!nodes.empty())
    {
        pNode = nodes.top();
        printf("%d\t", pNode->m_nValue);
        nodes.pop();
    }
}

void PrintListReversingly_Recursively(ListNode* pHead)
{
    if(pHead != NULL)
    {
        if (pHead->m_pNext != NULL)
        {
            PrintListReversingly_Recursively(pHead->m_pNext);
        }

        printf("%d\t", pHead->m_nValue);
    }
}

void Test(ListNode* pHead)
{
    PrintList(pHead);
    printf("Reversingly iteratively: ");
    PrintListReversingly_Iteratively(pHead);
    printf("\n");
    printf("Reversingly recursively: ");
    PrintListReversingly_Recursively(pHead);
    printf("\n\n");
}

// 1->2->3->4->5
void Test1()
{
    printf("Test1 begins.\n");

    ListNode* pNode1 = CreateListNode(1);
    ListNode* pNode2 = CreateListNode(2);
    ListNode* pNode3 = CreateListNode(3);
    ListNode* pNode4 = CreateListNode(4);
    ListNode* pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode4, pNode5);

    Test(pNode1);

    DestroyList(pNode1);
}

// 只有一个结点的链表: 1
void Test2()
{
    printf("\nTest2 begins.\n");

    ListNode* pNode1 = CreateListNode(1);

    Test(pNode1);

    DestroyList(pNode1);
}

// 空链表
void Test3()
{
    printf("\nTest3 begins.\n");

    Test(NULL);
}

int main()
{
    Test1();
    Test2();
    Test3();

    return 0;
}

