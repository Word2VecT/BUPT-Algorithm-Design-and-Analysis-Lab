#include <algorithm>
#include <iostream>
#include <vector>

const int NoEdge = INT32_MAX;

int bestc = NoEdge;
std::vector<int> bestx;
int cc = 0;

void Backtrack(int i, int n, const std::vector<std::vector<int>> &a, std::vector<int> &x) {
    if (i == n) {
        if (a[x[n - 1]][x[n]] != NoEdge && a[x[n]][0] != NoEdge) {
            int totalCost = cc + a[x[n - 1]][x[n]] + a[x[n]][0];
            if (totalCost < bestc || bestc == NoEdge) {
                bestc = totalCost;
                // Update the best path
                for (int j = 0; j <= n; ++j) {
                    bestx[j] = x[j];
                }
            }
        }
    } else {
        for (int j = i; j <= n; ++j) {
            if (a[x[i - 1]][x[j]] != NoEdge &&
                (cc + a[x[i - 1]][x[j]] < bestc || bestc == NoEdge)) {
                std::swap(x[i], x[j]);
                cc += a[x[i - 1]][x[i]];
                Backtrack(i + 1, n, a, x);
                cc -= a[x[i - 1]][x[i]];
                std::swap(x[i], x[j]); // backtrack
            }
        }
    }
}

int TSP(const std::vector<std::vector<int>> &a, std::vector<int> &v, int n) {
    std::vector<int> x(n + 1);
    for (int i = 0; i <= n; ++i) {
        x[i] = i;
    }

    bestx.assign(n + 1, 0);
    for (int i = 0; i <= n; ++i) {
        bestx[i] = x[i];
    }

    Backtrack(1, n, a, x);

    for (int i = 0; i <= n; ++i) {
        v[i] = bestx[i];
    }

    return bestc;
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
            if (cost != NoEdge) {
                adj[i][j] = cost;
            }
        }
    }

    bestx.assign(n + 1, 0);

    std::vector<int> v(n + 1, 0);

    int bestCost = TSP(adj, v, n - 1); 

    if (bestCost == NoEdge) {
        std::cout << "没有找到有效的旅行路径。\n";
    } else {
        std::cout << "最小成本: " << bestCost << "\n";
        std::cout << "最佳路径: ";
        for (int i = 0; i <= n; ++i) {
            std::cout << (v[i] + 1);
            if (i < n) {
                std::cout << " -> ";
            } else {
                std::cout << "\n";
            }
        }
    }

    return 0;
}