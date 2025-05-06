#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

void parallelBFS(const vector<vector<int>>& graph, int startNode) {
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    visited[startNode] = true;
    q.push(startNode);
    cout << "\nBFS Traversal starting from node " << startNode << ":\n";
    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> nextLevel;

        #pragma omp parallel for
        for (int i = 0; i < levelSize; i++) {
            int current;

            #pragma omp critical
            {
                if (!q.empty()) {
                    current = q.front();
                    q.pop();
                } else {
                    current = -1;
                }
            }

            if (current != -1) {
                #pragma omp critical
                cout << current << " ";

                for (int neighbor : graph[current]) {   
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            nextLevel.push_back(neighbor);
                        }
                    }
                }
            }
        }

        for (int node : nextLevel) {
            q.push(node);
        }
    }

    cout << endl;
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

    cout << "Enter starting node for BFS: ";
    cin >> start;

    parallelBFS(graph, start);
    
    return 0;
}
