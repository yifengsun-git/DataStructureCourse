#第一题
【问题描述】B地区现有编号为0～N-1的N个村庄及M条双向公路，地震后村庄需逐步重建，公路保持完好。规则为：仅连接两个均已重建村庄的公路可通行，
且仅能抵达已重建村庄。现有Q次查询，每次询问：第t天时，从村庄x到村庄y的最短路径长度为多少。
规则如下：
① 村庄i在第ti天完成重建，重建当日即可通行；若t<ti，则该村庄未重建、不可通行。仅连接两个均已重建村庄的公路可使用，每条公路长度不超过10000。 
②若起点x或终点y未重建，或二者在仅使用已重建村庄的前提下无通路，输出−1。 
③各村庄重建时间ti非递减（ti≥0，ti=0表示初始即可通车），查询时间t同样非递减。 
④数据范围：N≤100，M≤500，Q≤100。
【输入形式】键盘输入：
第一行输入两个正整数N、M，分别表示村庄总数与公路条数。
第二行输入N个非负整数ti，依次表示编号0～N−1村庄的重建完成时间，数据保证ti非递减。
接下来M行，每行输入三个非负整数i、j、w，表示村庄i与j之间存在一条长度为w的双向道路，其中w≤10000，保证i≠j且任意两村庄间至多一条公路。
随后一行输入一个正整数Q，表示查询次数。
接下来Q行，每行输入三个非负整数x、y、t，表示询问第t天时从村庄x到y的最短路径长度，查询时间t非递减。
【输出形式】输出共 Q 行，对每一个询问(x,y,t)输出对应的答案，即在第 t 天，从村庄 x 到村庄 y 的最短路径长度为多少。
如果经过若干个已重建完成的村庄，在第 t 天无法找到从 x 村庄到 y 村庄的路径，或者村庄 x 或村庄 y 在第t 天仍未修复完成，则输出-1。
【样例输入】                        
4 5
1 2 3 4
0 2 1
2 3 1
3 1 2
2 1 4
0 3 5
4
2 0 2
0 1 2
0 1 3
0 1 4
【样例输出】 
-1
-1
5
4
'''
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int main() {
    int N, M;
    cin >> N >> M;
    vector<int> rebuild_time(N);
    for (int i = 0; i < N; ++i)
        cin >> rebuild_time[i];

    // 邻接表存储公路
    vector<vector<pair<int, int> > > graph(N);
    for (int i = 0; i < M; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back(make_pair(v, w));
        graph[v].push_back(make_pair(u, w));
    }

    int Q;
    cin >> Q;
    while (Q--) {
        int x, y, t;
        cin >> x >> y >> t;

        // 起点或终点未重建
        if (rebuild_time[x] > t || rebuild_time[y] > t) {
            cout << -1 << "\n";
            continue;
        }

        // Dijkstra
        vector<int> dist(N, INF);
        dist[x] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
        pq.push(make_pair(0, x));

        while (!pq.empty()) {
            pair<int, int> top = pq.top();
            pq.pop();
            int d = top.first;
            int u = top.second;
            if (d != dist[u]) continue;

            for (int j = 0; j < graph[u].size(); ++j) {
                int v = graph[u][j].first;
                int w = graph[u][j].second;
                // 只走已重建的村庄
                if (rebuild_time[v] > t) continue;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        if (dist[y] == INF)
            cout << -1 << "\n";
        else
            cout << dist[y] << "\n";
    }

    return 0;
}
'''
【问题描述】从标准输入读取若干整数（允许重复）、待删除整数x和待查找整数y；构建结点包含数据值与重复出现次数的二叉排序树。依次完成以下操作：按二叉树括号表示法输出原树；删除一个x后输出树的中序遍历序列；在该中序序列中对y执行二分查找，并输出查找比较次数（无论成功与否）。例如，依次输入12个元素50 35 70 50 40 55 65 50 20 80 20 40，所建立的二叉排序树如图
tu1.png
【实验要求】
（1）二叉排序树的构造方法可自行选择。
（2）中序遍历序列仅输出结点数据值，不输出重复次数。
（3）在二叉排序树中删除整数x：若x重复出现，则将其次数减1；若x仅出现一次，则直接删除对应结点。
【输入形式】从键盘输入三行数据：第一行为若干以空格分隔的整数（可重复），第二行、第三行分别输入待删除整数x与待二分查找的整数y。
【输出形式】按要求输出三行内容：
第一行输出初始二叉排序树的括号表示法，
第二行输出删除元素 x后的树的中序遍历序列，
第三行输出对y进行二分查找的比较次数。
【样例输入】                        
50 35 70 50 40 55 65 50 20 80 20 40
55
66
【样例输出】 
50|3(35|1(20|2,40|2),70|1(55|1(,65|1),80|1))
20 35 40 50 65 70 80
3

```
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Node {
    int data;
    int freq;
    Node* lchild, * rchild;
    Node(int val, int f = 1)
        : data(val), freq(f), lchild(nullptr), rchild(nullptr) {
    }
};

Node* insert(Node* root, int val)
{
    if (!root)
    {
        return new Node(val);
    }
    if (val < root->data)
    {
        root->lchild = insert(root->lchild, val);
    }
    else if (val > root->data)
    {
        root->rchild = insert(root->rchild, val);
    }
    else
    {
        root->freq++;
    }
    return root;
}

//查找节点
Node* search(Node* root, int val)
{
    if (!root)return nullptr;
    if (val < root->data)
    {
        return search(root->lchild, val);
    }
    if (val > root->data)
    {
        return search(root->rchild, val);
    }
    return root;
}

Node* findMin(Node* root)
{
    while (root && root->lchild)root = root->lchild;
    return root;
}

Node* remove(Node* root, int val)
{
    if (!root)return nullptr;
    if (val < root->data)
    {
        root->lchild = remove(root -> lchild, val);
    }
    else if (val > root->data)
    {
        root->rchild = remove(root->rchild, val);
    }
    else
    {
        if (root->freq > 1)
        {
            root->freq--;
            return root;
        }
        else
        {
            if (!root->lchild) {
                Node* tmp = root->rchild;
                delete root;
                return tmp;
            }
            else if (!root->rchild) {
                Node* tmp = root->lchild;
                delete root;
                return tmp;
            }
            else
            {
                Node* minNode = findMin(root->rchild);
                root->data = minNode->data;
                root->freq = minNode->freq;
                minNode->freq = 1;
                root->rchild = remove(root->rchild, minNode->data);
            }
        }
    }
    return root;
}

//括号表示法输出
void printTree(Node* root)
{
    if (!root)return;
    cout << root->data << "|" << root->freq;
    if (root->lchild || root->rchild)
    {
        cout << "(";
        if (root->lchild)printTree(root->lchild);
        cout << ",";
        if (root->rchild)printTree(root->rchild);
        cout << ")";
    }
}

void inorder(Node* root, vector<int>& seq)
{
    if (!root)return;
    inorder(root -> lchild, seq);
    seq.push_back(root->data);
    inorder(root->rchild, seq);
}

//二分查找
int binarySearch(const vector<int>& arr, int target)
{
    int left = 0, right = arr.size() - 1;
    int cmp = 0;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        cmp++;
        if (arr[mid] == target)
        {
            return cmp;
        }
        else if (arr[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return cmp;
}

int main()
{
    string line;
    getline(cin, line);
    istringstream iss(line);
    vector<int>input;
    int num;
    while (iss >> num)
    {
        input.push_back(num);
    }
    int x, y;
    cin >> x >> y;
    Node* root = nullptr;
    for (int val : input)
    {
        root = insert(root, val);
    }
    printTree(root);
    cout << endl;

    root = remove(root, x);

    vector<int>seq;
    inorder(root, seq);
    for (size_t i = 0; i < seq.size(); ++i)
    {
        if (i > 0)cout << " "; 
        cout << seq[i];
    }
    cout << endl;

    int cmpTimes = binarySearch(seq, y);
    cout << cmpTimes << endl;

    return 0;
}
```
