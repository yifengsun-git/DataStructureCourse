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
