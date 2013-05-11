#include <iostream>
#include <vector>
using namespace std;

/*
 * This code finds unoriented 2-layer trees that have order dimension more than 2
 */

struct Edge {
    int begin;
    int end;
};
typedef vector<Edge> Graph;

ostream& operator << (ostream& os, const Graph& graph) {
    os << "{\n";
    for (const auto& edge: graph) {
        os << "\t" << edge.begin << " " << edge.end << "\n";
    }
    os << "}\n";
}

ostream& operator << (ostream& os, const vector<int>& order) {
    for (int num: order) {
        os << num;
    }
}

void enumerateTreesDfs(int num, int depth, vector<Graph>& graphs, Graph& curGraph) {
    graphs.push_back(curGraph);
    if (num == depth) {
        return;
    }
    Edge edge;
    edge.end = num;
    for (int i = 0; i < num; ++i) {
        edge.begin = i;
        for (int direction = 0; direction < 2; ++direction) {
            curGraph.push_back(edge);
            enumerateTreesDfs(num + 1, depth, graphs, curGraph);
            curGraph.pop_back();
            swap(edge.begin, edge.end);
        }
    }
}

vector<Graph> enumerateTrees(int depth) {
    vector<Graph> graphs;
    Graph curGraph;
    enumerateTreesDfs(1, depth, graphs, curGraph);
    return graphs;
}


void orderDfs(const Graph& graph, vector<int>& order, vector<bool>& visited, vector< vector<int> >& orders) {
    int n = graph.size() + 1;
    if (order.size() == n) {
        orders.push_back(order);
    }

    for (int i = 0; i < n; ++i) {
        if (visited[i]) continue;

        bool ok = true;
        for (const auto& edge: graph) {
            if (visited[edge.end] && edge.begin == i) ok = false;
        }
        if (!ok) continue;

        order.push_back(i);
        visited[i] = true;
        orderDfs(graph, order, visited, orders);
        visited[i] = false;
        order.pop_back();
    }
}

vector< vector<int> > reachability(const vector<int>& orderA, const vector<int>& orderB) {
    int n = orderA.size();
    vector< vector<int> > r(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            r[orderA[i]][orderA[j]] += 1;
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            r[orderB[i]][orderB[j]] += 1;
        }
    }
    
    vector< vector<int> > res(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (r[i][j] == 2) res[i].push_back(j);
        }
    }
    return res; 
}

vector< vector<int> > reachability(const Graph& graph) {
    int n = graph.size() + 1;
    vector< vector<bool> > closure(n, vector<bool>(n, false));
    for (int i = 0; i < n; ++i) {
        closure[i][i] = true;
    }
    for (int i = 0; i < graph.size(); ++i) {
        closure[graph[i].begin][graph[i].end] = true;
    }

    for (int iter = 0; iter < n; ++iter) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (!closure[i][j]) continue;
                for (int k = 0; k < n; ++k) {
                    if (closure[j][k]) closure[i][k] = true;
                }
            }
        }
    }

    vector< vector<int> > res(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (closure[i][j]) res[i].push_back(j);
        }
    }
    return res; 
}

bool isOrderTwo(const Graph& graph) {
    vector< vector<int> > suitableOrders;
    {
        vector<bool> visited(graph.size() + 1, false);
        vector<int> order;
        orderDfs(graph, order, visited, suitableOrders);
    }

    auto graphReachability = reachability(graph);
    for (int i = 0; i < suitableOrders.size(); ++i) {
        for (int j = 0; j <= i; ++j) {
            if (graphReachability == reachability(suitableOrders[i], suitableOrders[j])) {
                //if (graph.size() == 4) {
                //    cout << "Graph " << graph << suitableOrders[i] << "\n" << suitableOrders[j] << "\n\n";
                //}
                return true;
            }
        }
    }

    //cout << "Graph " << graph << endl;
    //cout << "Suitable orders:" << endl;
    //for (const auto& order: suitableOrders) {
    //    for (int num: order) {
    //        cout << num << " ";
    //    }
    //    cout << endl;
    //}
}

bool flipsDfs(int v, int p, bool direction, vector<vector<pair<int, bool>>> edges) {
    for (int i = 0; i < edges[v].size(); ++i) {
        int u = edges[v][i].first;
        if (u == p) continue;
        bool edgeDirection = edges[v][i].second;
        if (!direction && edgeDirection) return false;
        if (!flipsDfs(u, v, edgeDirection, edges)) return false;
    }
    return true;
}

vector<Graph> filterTwoLayer(const vector<Graph>& graphs) {
    vector<Graph> filtered;
    for (const auto& graph: graphs) {
        int n = graph.size() + 1;
        vector<vector<pair<int, bool>>> edges(n);
        for (const auto& edge: graph) {
            edges[edge.begin].push_back({edge.end, true});
            edges[edge.end].push_back({edge.begin, false});
        }
        if (flipsDfs(0, -1, true, edges)) {
            filtered.push_back(graph);
        }
    }
    return filtered;
}

int main() {
    auto trees = enumerateTrees(7);
    cout << "Trees count " << trees.size() << endl;
    trees = filterTwoLayer(trees);
    cout << "Filtered count " << trees.size() << endl;
    for (const auto& tree: trees) {
        if (!isOrderTwo(tree)) {
            cout << tree;
            cout << endl;
        }
    }
    
    return 0;
}
