#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <climits>

using namespace std;

const int MAXN = 3005;

// Graph representation for flow network
int capacity[MAXN][MAXN];
vector<int> graph[MAXN];

// Edmonds-Karp algorithm for maximum flow
int maxFlow(int source, int sink, int n) {
    int totalFlow = 0;
    
    // Use static array for residual graph to avoid stack overflow
    static int residual[MAXN][MAXN];
    
    // Initialize residual graph
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            residual[i][j] = capacity[i][j];
        }
    }
    
    while (true) {
        // Find augmenting path using BFS
        vector<int> parent(n+1, -1);
        queue<int> q;
        q.push(source);
        parent[source] = -2;
        
        while (!q.empty() && parent[sink] == -1) {
            int u = q.front();
            q.pop();
            
            for (int v = 1; v <= n; v++) {
                if (parent[v] == -1 && residual[u][v] > 0) {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        // No augmenting path found
        if (parent[sink] == -1) break;
        
        // Find minimum residual capacity on the path
        int pathFlow = INT_MAX;
        int cur = sink;
        while (cur != source) {
            int prev = parent[cur];
            pathFlow = min(pathFlow, residual[prev][cur]);
            cur = prev;
        }
        
        // Update residual capacities
        cur = sink;
        while (cur != source) {
            int prev = parent[cur];
            residual[prev][cur] -= pathFlow;
            residual[cur][prev] += pathFlow;
            cur = prev;
        }
        
        totalFlow += pathFlow;
    }
    
    return totalFlow;
}

int main() {
    int n, m;
    cin >> n >> m;
    
    // Initialize capacity matrix
    memset(capacity, 0, sizeof(capacity));
    
    // Build graph and capacity matrix
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        capacity[a][b] = 1;
        capacity[b][a] = 1;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    
    // Calculate sum of maximum flows for all pairs a < b
    long long total = 0;
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            total += maxFlow(a, b, n);
        }
    }
    
    cout << total << endl;
    
    return 0;
}