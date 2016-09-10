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

## 面试题50：树中两个结点的最低公共祖先

### 题目

> 输入两个树结点，求它们的最低公共祖先。

### 解析

这一题的题目其实故意没有说清楚，给定两个树节点，那么这是一棵怎样的树呢？数据结构的定义是怎样的呢？面试官希望我们能想到这些未曾提到的点，并做出反应。

如果这是一棵二叉搜索树，那找到两个结点的最低公共祖先很简单。我们从根结点开始遍历这棵树，如果当前结点比两个输入结点都大，那么两个输入结点必然都在当前结点的左子树上；如果当前结点比两个输入结点都小，那么两个输入结点必然都在当前结点的右子树上。当我们遍历到一个结点比其中一个输入结点大，比另外一个输入结点小，就找到两个输入结点的最低公共祖先了。因为从这个结点再往下，两个输入结点不会再出现在同一棵子树上了。

如果是一棵普通的树，但是每个结点有指向父结点的指针，我们把这棵树换个方向来看，其实这道题就变成了前面求两个链表的第一个共同结点的题目。

如果只是一棵普通的树，树结点定义如下：

```c++
struct TreeNode
{
    int                    m_nValue;
    std::vector<TreeNode*>    m_vChildren;
};
```

每个结点有个数不等的多个子结点，且只有指向子结点的指针。这时又应该怎样求解呢？

最笨的方法是从根结点开始遍历，逐个结点判断两个输入结点是否都在它的同一棵子树中，当到达一个结点，两个输入结点被分散在该结点的不同子树中时，该结点就是最低公共祖先。

这个方法效率不高，因为从上往下扫的过程中产生了相当多重复的查找。这里介绍一种借助辅助空间来减少时间复杂度的方法。

我们可以使用两个双向链表（list容器）来分别记录下到达两个结点的完整路径（基于前序遍历来获取），然后就可以把问题转化为求两个链表的第一个共同结点（把list最后一个元素作为链表第一个元素）了。这里因为我们可以直接得到链表头，所以这个问题又可以写作求两个链表的最后一个共同结点（从list.begin()开始）。



```c++
// 前序遍历根结点为pRoot的树，找到pNode所在的路径，记录在path里面
bool GetNodePath(TreeNode* pRoot, TreeNode* pNode, list<TreeNode*>& path)
{
    if(pRoot == pNode)
        return true;

    path.push_back(pRoot);

    bool found = false;

    // 前序遍历当前结点的所有子结点，直到找到pNode
    vector<TreeNode*>::iterator i = pRoot->m_vChildren.begin();
    while(!found && i < pRoot->m_vChildren.end())
    {
        found = GetNodePath(*i, pNode, path);
        ++i;
    }

    // 当前路径无法到达pNode，从路径中移除当前结点，返回上一层
    if(!found)
        path.pop_back();

    return found;
}

// 获取path1和path2两条路径的最后一个共同结点
TreeNode* GetLastCommonNode
(
    const list<TreeNode*>& path1,
    const list<TreeNode*>& path2
)
{
    // const_iterator指向的对象是常量，即iterator指向常对象，
    // 迭代器本身指向可变，但它指向的对象的内容是const的。
    list<TreeNode*>::const_iterator iterator1 = path1.begin();
    list<TreeNode*>::const_iterator iterator2 = path2.begin();

    TreeNode* pLast = NULL;

    while(iterator1 != path1.end() && iterator2 != path2.end())
    {
        if(*iterator1 == *iterator2) // 每次找到共同结点都会更新
            pLast = *iterator1;

        iterator1++;
        iterator2++;
    }

    return pLast;
}

TreeNode* GetLastCommonParent(TreeNode* pRoot, TreeNode* pNode1, TreeNode* pNode2)
{
    if(pRoot == NULL || pNode1 == NULL || pNode2 == NULL)
        return NULL;

    // 先分别拿到路径
    list<TreeNode*> path1;
    GetNodePath(pRoot, pNode1, path1);

    list<TreeNode*> path2;
    GetNodePath(pRoot, pNode2, path2);

    // 再找路径的最后共同结点
    return GetLastCommonNode(path1, path2);
}
```
