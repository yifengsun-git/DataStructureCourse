/*1. 2026春(1)第1题：链表实现联接运算（A班）
【问题描述】
给定两个存储整数（或字母）的二维表R（m₁行n₁列）和S（m₂行n₂列），基于单向链表实现二者的大于连接运算：
（1）大于连接规则：C=R⋈i>jS（R的第i列与S的第j列）；
（2）运算逻辑：先求R和S的笛卡尔积，再筛选出“R的第i列值>S的第j列值”的所有元组，最终结果构成二维表C。
【实验要求】
（1）数据存储：必须采用单向链表存储二维表的行/列数据（如每行数据为一个链表结点，一张表对应链表所有结点）。
（2）运算实现：大于连接的核心逻辑（笛卡尔积生成、大于筛选）需基于链表操作完成，禁止使用数组/容器等替代。
（3）结果要求：输出的结果表C需保留符合条件的所有元组，无冗余数据，行数≤m₁×m₂。
【输入格式】 in.txt文件输入多组数据（文件非空）
第一行测试用例个数k, 第二行：第一个样例数据类型（int或者char）。第三行：两个整数m? 、n?，空格分隔，表示二维表R的行数和列数。
接下来m?行：每行n?个（或字母）整数，空格分隔，表示R的每行数据。 随后一行：两个整数m? 、 n? ，空格分隔，
表示二维表S的行数和列。接下来m?行：每行n?个整数（或字母），空格分隔，表示S的每行数据。再一行：两个正整数i、j，
空格分隔，表示大于连接的列号（i为R的列号，j为S的列号，列号从1开始计数）。重复K个测试用例。
保证有K个样例，每个二维表类型正确后续正确，但是不保证联接列号符合要求。
【样例输入】文件in.txt内容：
 4
char
3 2
a b
c b
e c
2 2
x y
b h
2 1
int
3 3
1 2 3
2 3 6
4 5 7
2 4
3 6 9 7
5 7 8 6
3 2
int
3 3
1 2 4
3 4 3
20 45 12
2 2
10 20
30 40
1 5
float
3 4
【输出格式】输出到文件out.txt
输出in.txt文件中大于联接运算结果表的所有元组，格式要求：
（1）每行包含n₁+n₂个整数（或字母），空格分隔（为R的一行数据拼接S的一行数据）。
（2）行顺序与笛卡尔积筛选结果一致，行数≤m₁×m₂（无符合条件的元组时无输出）。
（3）每个样例结果之间空一行。
【样例输出】
 对于上述in.txt输入，输出out.txt ：
第1个char样例:
e c b h
第2个int样例:
4 5 7 3 6 9 7
第3个int样例:
列号超出范围!
第4个float样例:
未知的数据类型: float
【评分标准】
要使用链表实现，否则不能得分。*/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_COL = 100;

template<typename T>
struct DataNode {
    T data[MAX_COL];
    DataNode<T>* next;
    DataNode() : next(nullptr) {}
};

template<typename T>
struct HNode {
    int row, col;
    DataNode<T>* next;
    HNode() : row(0), col(0), next(nullptr) {}
    HNode(int r, int c) : row(r), col(c), next(nullptr) {}
};

template<typename T>
class HList {
private:
    HNode<T>* head;
    void clear() 
    {
        DataNode<T>* p = head->next;
        while (p) 
        {
            DataNode<T>* q = p->next;
            delete p;
            p = q;
        }
        head->next = nullptr;
        head->row = head->col = 0;
    }

public:
    HList() 
    {
        head = new HNode<T>();
    }

    ~HList() 
    {
        clear();
        delete head;
    }

    // 禁止拷贝
    HList(const HList&) = delete;
    HList& operator=(const HList&) = delete;

    // 允许移动
    HList(HList&& other) noexcept 
    {
        head = other.head;
        other.head = nullptr;
    }

    HList& operator=(HList&& other) noexcept
    {
        if (this != &other) 
        {
            clear();
            delete head;
            head = other.head;
            other.head = nullptr;
        }
        return *this;
    }

    bool readTable(ifstream& in, int rows, int cols) 
    {
        if (rows <= 0 || cols <= 0 || cols > MAX_COL) 
        {
            return false;
        }
        clear();
        head->row = rows;
        head->col = cols;
        DataNode<T>* tail = nullptr;
        for (int r = 0; r < rows; ++r) 
        {
            DataNode<T>* node = new DataNode<T>();
            for (int c = 0; c < cols; ++c)
            {
                in >> node->data[c];
            }
            if (head->next == nullptr)
            {
                head->next = node;
                tail = node;
            }
            else 
            {
                tail->next = node;
                tail = node;
            }
        }
        return true;
    }

    int getRows() const { return head->row; }
    int getCols() const { return head->col; }
    DataNode<T>* getDataHead() const { return head->next; }

    // 大于连接运算
    HList<T> greaterJoin(const HList<T>& S, int i, int j) const 
    {
        HList<T> result;
        if (i < 1 || i > head->col || j < 1 || j > S.head->col)
        {
            return result;  // 返回空表，触发移动构造
        }
        int newCols = head->col + S.head->col;
        result.head->col = newCols;
        int newRows = 0;
        DataNode<T>* tail = nullptr;

        for (DataNode<T>* rPtr = head->next; rPtr; rPtr = rPtr->next) 
        {
            for (DataNode<T>* sPtr = S.head->next; sPtr; sPtr = sPtr->next)
            {
                if (rPtr->data[i - 1] > sPtr->data[j - 1]) 
                {
                    DataNode<T>* newNode = new DataNode<T>();
                    for (int c = 0; c < head->col; ++c) 
                    {
                        newNode->data[c] = rPtr->data[c];
                    }
                    for (int c = 0; c < S.head->col; ++c) 
                    {
                        newNode->data[head->col + c] = sPtr->data[c];
                    }
                    if (result.head->next == nullptr)
                    {
                        result.head->next = newNode;
                        tail = newNode;
                    }
                    else 
                    {
                        tail->next = newNode;
                        tail = newNode;
                    }
                    newRows++;
                }
            }
        }
        result.head->row = newRows;
        return result;      // 触发移动构造（或 RVO）
    }

    void print(ofstream& out) const
    {
        DataNode<T>* p = head->next;
        while (p) 
        {
            for (int c = 0; c < head->col; ++c)
            {
                out << p->data[c];
                if (c < head->col - 1) out << " ";
            }
            out << endl;
            p = p->next;
        }
    }
};

// 跳过未知类型测试用例
void skipTestCase(ifstream& in) 
{
    int m1, n1, m2, n2, i, j;
    string dummy;
    in >> m1 >> n1;
    for (int r = 0; r < m1; ++r)
        for (int c = 0; c < n1; ++c)
            in >> dummy;
    in >> m2 >> n2;
    for (int r = 0; r < m2; ++r)
        for (int c = 0; c < n2; ++c)
            in >> dummy;
    in >> i >> j;
}

int main() 
{
    ifstream in("in.txt");
    ofstream out("out.txt");
    if (!in || !out)
    {
        cerr << "无法打开文件！" << endl;
        return 1;
    }

    int k;
    in >> k;
    for (int caseIdx = 1; caseIdx <= k; ++caseIdx)
    {
        string typeStr;
        in >> typeStr;
        out << "第" << caseIdx << "个" << typeStr << "样例:" << endl;

        if (typeStr == "int") 
        {
            HList<int> R, S;
            int m1, n1, m2, n2, i, j;
            in >> m1 >> n1;
            R.readTable(in, m1, n1);
            in >> m2 >> n2;
            S.readTable(in, m2, n2);
            in >> i >> j;

            if (i < 1 || i > n1 || j < 1 || j > n2) 
            {
                out << "列号超出范围!" << endl;
            }
            else 
            {
                HList<int> C = R.greaterJoin(S, i, j);   // 移动语义生效
                C.print(out);
            }
        }
        else if (typeStr == "char")
        {
            HList<char> R, S;
            int m1, n1, m2, n2, i, j;
            in >> m1 >> n1;
            R.readTable(in, m1, n1);
            in >> m2 >> n2;
            S.readTable(in, m2, n2);
            in >> i >> j;

            if (i < 1 || i > n1 || j < 1 || j > n2)
            {
                out << "列号超出范围!" << endl;
            }
            else
            {
                HList<char> C = R.greaterJoin(S, i, j);
                C.print(out);
            }
        }
        else 
        {
            out << "未知的数据类型: " << typeStr << endl;
            skipTestCase(in);
        }

        if (caseIdx < k) out << endl;
    }

    in.close();
    out.close();
    return 0;
}
