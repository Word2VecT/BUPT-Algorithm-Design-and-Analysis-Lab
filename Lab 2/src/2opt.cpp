#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

const int NoEdge = std::numeric_limits<int>::max();

int CalculatePathCost(const std::vector<int> &path, const std::vector<std::vector<int>> &adj) {
    int cost = 0;
    int n = path.size();
    for (int i = 0; i < n - 1; ++i) {
        cost += adj[path[i]][path[i + 1]];
    }
    cost += adj[path.back()][path[0]];
    return cost;
}

std::vector<int> TwoOptSwap(const std::vector<int> &path, int i, int k) {
    std::vector<int> newPath;
    newPath.insert(newPath.end(), path.begin(), path.begin() + i);
    std::vector<int> reversedSegment(path.begin() + i, path.begin() + k + 1);
    std::reverse(reversedSegment.begin(), reversedSegment.end());
    newPath.insert(newPath.end(), reversedSegment.begin(), reversedSegment.end());
    newPath.insert(newPath.end(), path.begin() + k + 1, path.end());
    return newPath;
}

std::pair<int, std::vector<int>> TwoOpt(const std::vector<std::vector<int>> &graph, const std::vector<int> &initialPath) {
    int n = initialPath.size();
    std::vector<int> bestPath = initialPath;
    int bestCost = CalculatePathCost(bestPath, graph);

    bool improvement = true;
    while (improvement) {
        improvement = false;
        for (int i = 1; i < n - 1; ++i) {
            for (int k = i + 1; k < n; ++k) {
                std::vector<int> newPath = TwoOptSwap(bestPath, i, k);
                int newCost = CalculatePathCost(newPath, graph);
                if (newCost < bestCost) {
                    bestPath = newPath;
                    bestCost = newCost;
                    improvement = true;
                }
            }
        }
    }

    return {bestCost, bestPath};
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

    std::vector<int> initialPath(n);
    for (int i = 0; i < n; ++i) {
        initialPath[i] = i;
    }

    auto [bestCost, bestPath] = TwoOpt(adj, initialPath);

    std::cout << "近似最小成本: " << bestCost << "\n";
    std::cout << "路径: ";
    for (size_t i = 0; i < bestPath.size(); ++i) {
        std::cout << bestPath[i] + 1;
        if (i < bestPath.size() - 1) {
            std::cout << " -> ";
        } else {
            std::cout << " -> " << bestPath[0] + 1 << "\n";
        }
    }

    return 0;
}
