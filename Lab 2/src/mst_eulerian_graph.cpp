#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

const int NoEdge = INT32_MAX;

struct Edge {
    int u, v, weight;
    bool operator<(const Edge &other) const {
        return weight < other.weight;
    }
};

std::vector<Edge> FindMST(const std::vector<std::vector<int>> &graph, int n) {
    std::vector<Edge> edges;
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (graph[u][v] != NoEdge) {
                edges.push_back({u, v, graph[u][v]});
            }
        }
    }

    std::sort(edges.begin(), edges.end());

    std::vector<int> parent(n);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    auto Find = [&](int x) -> int {
        while (x != parent[x]) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    };

    auto Union = [&](int x, int y) {
        parent[Find(x)] = Find(y);
    };

    std::vector<Edge> mst;
    for (const Edge &e : edges) {
        if (Find(e.u) != Find(e.v)) {
            mst.push_back(e);
            Union(e.u, e.v);
            if (mst.size() == n - 1) {
                break;
            }
        }
    }

    return mst;
}

std::vector<int> FindOddVertices(const std::vector<Edge> &mst, int n) {
    std::vector<int> degree(n, 0);
    for (const Edge &e : mst) {
        degree[e.u]++;
        degree[e.v]++;
    }

    std::vector<int> oddVertices;
    for (int i = 0; i < n; ++i) {
        if (degree[i] % 2 == 1) {
            oddVertices.push_back(i);
        }
    }

    return oddVertices;
}

std::vector<Edge> FindMinimumPerfectMatching(const std::vector<int> &oddVertices,
                                             const std::vector<std::vector<int>> &graph) {
    std::vector<Edge> matching;
    std::set<int> unmatched(oddVertices.begin(), oddVertices.end());

    while (!unmatched.empty()) {
        int u = *unmatched.begin();
        unmatched.erase(u);

        int minWeight = INT32_MAX;
        int bestMatch = -1;

        for (int v : unmatched) {
            if (graph[u][v] < minWeight) {
                minWeight = graph[u][v];
                bestMatch = v;
            }
        }

        if (bestMatch != -1) {
            unmatched.erase(bestMatch);
            matching.push_back({u, bestMatch, minWeight});
        } else {
        }
    }

    return matching;
}

std::vector<int> FindEulerianTour(const std::vector<Edge> &mst,
                                  const std::vector<Edge> &matching, int n) {
    std::vector<std::vector<int>> adjList(n, std::vector<int>());
    for (const Edge &e : mst) {
        adjList[e.u].push_back(e.v);
        adjList[e.v].push_back(e.u);
    }
    for (const Edge &e : matching) {
        adjList[e.u].push_back(e.v);
        adjList[e.v].push_back(e.u);
    }

    std::vector<int> tour;
    std::vector<std::vector<int>> localAdj = adjList;

    std::function<void(int)> Hierholzer = [&](int u) {
        while (!localAdj[u].empty()) {
            int v = localAdj[u].back();
            localAdj[u].pop_back();
            auto &vec = localAdj[v];
            vec.erase(std::find(vec.begin(), vec.end(), u));
            Hierholzer(v);
        }
        tour.push_back(u);
    };

    Hierholzer(0);
    std::reverse(tour.begin(), tour.end());
    return tour;
}

std::vector<int> CompressEulerianToHamiltonian(const std::vector<int> &eulerTour) {
    std::vector<int> hamiltonian;
    std::set<int> visited;
    for (int city : eulerTour) {
        if (visited.find(city) == visited.end()) {
            visited.insert(city);
            hamiltonian.push_back(city);
        }
    }
    return hamiltonian;
}

std::pair<int, std::vector<int>> GetMSTBasedTSP(const std::vector<std::vector<int>> &graph, int n) {
    std::vector<Edge> mst = FindMST(graph, n);

    std::vector<int> oddVertices = FindOddVertices(mst, n);

    std::vector<Edge> matching = FindMinimumPerfectMatching(oddVertices, graph);

    std::vector<int> eulerTour = FindEulerianTour(mst, matching, n);

    std::vector<int> hamiltonianPath = CompressEulerianToHamiltonian(eulerTour);

    if (!hamiltonianPath.empty()) {
        hamiltonianPath.push_back(hamiltonianPath[0]);
    }

    int totalCost = 0;
    for (size_t i = 0; i < hamiltonianPath.size() - 1; ++i) {
        int from = hamiltonianPath[i];
        int to = hamiltonianPath[i + 1];
        if (graph[from][to] == NoEdge) {
        }
        totalCost += graph[from][to];
    }

    return {totalCost, hamiltonianPath};
}

int main() {
    int n;
    std::cout << "请输入城市数量 n: ";
    std::cin >> n;

    std::vector<std::vector<int>> adj(n, std::vector<int>(n, NoEdge));
    std::cout << "请输入邻接矩阵 (使用 " << NoEdge << " 表示没有边):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int cost;
            std::cin >> cost;
            adj[i][j] = cost;
        }
    }

    auto [totalCost, hamiltonianPath] = GetMSTBasedTSP(adj, n);

    std::cout << "近似最小成本: " << totalCost << "\n";
    std::cout << "路径: ";
    for (size_t i = 0; i < hamiltonianPath.size(); ++i) {
        std::cout << hamiltonianPath[i] + 1; // 转换为 1-based 编号
        if (i < hamiltonianPath.size() - 1) {
            std::cout << " -> ";
        } else {
            std::cout << "\n";
        }
    }

    return 0;
}
