#include <iostream>
#include <vector>

const int NoEdge = INT32_MAX;

bool NearestNeighbor(const std::vector<std::vector<int>> &adj, int n, std::vector<int> &path, int &totalCost) {
    if (n == 0) return false;

    std::vector<bool> visited(n, false);
    path.clear();
    totalCost = 0;

    int currentCity = 0;
    path.push_back(currentCity);
    visited[currentCity] = true;

    for (int step = 1; step < n; ++step) {
        int nearestCity = -1;
        int minCost = NoEdge;

        for (int city = 0; city < n; ++city) {
            if (!visited[city] && adj[currentCity][city] < minCost && city != currentCity) {
                minCost = adj[currentCity][city];
                nearestCity = city;
            }
        }

        if (nearestCity == -1 || minCost == NoEdge) {
            return false;
        }

        path.push_back(nearestCity);
        visited[nearestCity] = true;
        totalCost += minCost;
        currentCity = nearestCity;
    }

    if (adj[currentCity][path[0]] == NoEdge) {
        return false;
    }
    path.push_back(path[0]);
    totalCost += adj[currentCity][path[0]];

    return true;
}

int main() {
    int n;
    std::cout << "请输入城市数量 n: ";
    std::cin >> n;

    std::vector<std::vector<int>> adj(n, std::vector<int>(n, NoEdge));
    std::cout << "请输入邻接矩阵 (使用 " << NoEdge << " 表示没有边):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int cost = 0;
            std::cin >> cost;
            if (cost != NoEdge) {
                adj[i][j] = cost;
            }
        }
    }

    std::vector<int> path;
    int totalCost = 0;

    bool success = NearestNeighbor(adj, n, path, totalCost);

    if (!success) {
        std::cout << "没有找到有效的旅行路径。\n";
    } else {
        std::cout << "近似最小成本: " << totalCost << "\n";
        std::cout << "近似最佳路径: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << (path[i] + 1);
            if (i < path.size() - 1) {
                std::cout << " -> ";
            } else {
                std::cout << "\n";
            }
        }
    }

    return 0;
}