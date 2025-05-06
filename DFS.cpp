#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void parallelDFS(int node, const vector<vector<int>>& graph, vector<bool>& visited) {
    #pragma omp critical
    {
        if (visited[node]) return;
        visited[node] = true;
        cout << node << " ";
    }

    #pragma omp parallel for
    for (int i = 0; i < graph[node].size(); ++i) {
        int neighbor = graph[node][i];
        if (!visited[neighbor]) {
            parallelDFS(neighbor, graph, visited);
        }
    }
}

int main() {
    int nodes, edges, u, v, start;

    cout << "Enter number of nodes: ";
    cin >> nodes;

    cout << "Enter number of edges: ";
    cin >> edges;

    vector<vector<int>> graph(nodes);

    cout << "Enter " << edges << " edges (u v):\n";
    for (int i = 0; i < edges; ++i) {
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // undirected graph
    }

    cout << "Enter starting node for DFS: ";
    cin >> start;

    vector<bool> visited(nodes, false);

    cout << "\nDFS Traversal starting from node " << start << ":\n";
    parallelDFS(start, graph, visited);

    cout << endl;
    return 0;
}
