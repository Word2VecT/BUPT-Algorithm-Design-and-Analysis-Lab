#include <algorithm>
#include <iostream>
#include <vector>

const int NoEdge = INT32_MAX;

std::vector<int> ReconstructPath(const std::vector<std::vector<int>> &parent, int final_mask, int last_city, int n) {
    std::vector<int> path;
    
    int mask = final_mask;
    int current_city = last_city;

    while (current_city != 0) {
        path.push_back(current_city);
        int prev_city = parent[mask][current_city];
        mask &= ~(1 << current_city);
        current_city = prev_city;
    }

    path.push_back(0);

    std::reverse(path.begin(), path.end());

    path.push_back(0);

    return path;
}

int TSP_DP(const std::vector<std::vector<int>> &a, int n, std::vector<int> &bestx) {
    int N = 1 << n;
    std::vector<std::vector<int>> dp(N, std::vector<int>(n, NoEdge));
    std::vector<std::vector<int>> parent(N, std::vector<int>(n, -1));

    dp[1][0] = 0;

    for (int mask = 1; mask < N; ++mask) {
        for (int u = 0; u < n; ++u) {
            if (mask & (1 << u)) {
                if (dp[mask][u] == NoEdge) continue;

                for (int v = 0; v < n; ++v) {
                    if (!(mask & (1 << v)) && a[u][v] != NoEdge) {
                        int next_mask = mask | (1 << v);
                        if (dp[next_mask][v] > dp[mask][u] + a[u][v]) {
                            dp[next_mask][v] = dp[mask][u] + a[u][v];
                            parent[next_mask][v] = u;
                        }
                    }
                }
            }
        }
    }

    int final_mask = N - 1; 
    int min_cost = NoEdge;
    int last_city = -1;

    for (int u = 1; u < n; ++u) {
        if (a[u][0] != NoEdge && dp[final_mask][u] != NoEdge) {
            int total_cost = dp[final_mask][u] + a[u][0];
            if (total_cost < min_cost) {
                min_cost = total_cost;
                last_city = u;
            }
        }
    }

    if (min_cost == NoEdge) {
        return NoEdge;
    }

    bestx = ReconstructPath(parent, final_mask, last_city, n);

    return min_cost;
}

int main() {
    int n;
    std::cout << "请输入城市数量 n (建议 n <= 20): ";
    std::cin >> n;
    std::vector<std::vector<int>> a(n, std::vector<int>(n, NoEdge));
    std::cout << "请输入邻接矩阵 (使用 " << NoEdge << " 表示没有边):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int cost;
            std::cin >> cost;
            if (cost != NoEdge || i == j) {
                a[i][j] = cost;
            }
        }
    }

    std::vector<int> bestx;

    int bestCost = TSP_DP(a, n, bestx);

    if (bestCost == NoEdge) {
        std::cout << "没有找到有效的旅行路径。\n";
    } else {
        std::cout << "最小成本: " << bestCost << "\n";
        std::cout << "最佳路径: ";
        for (size_t i = 0; i < bestx.size(); ++i) {
            std::cout << (bestx[i] + 1);
            if (i < bestx.size() - 1) {
                std::cout << " -> ";
            } else {
                std::cout << "\n";
            }
        }
    }

    return 0;
}