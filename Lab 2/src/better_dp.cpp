#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <cstring> // For memset

const int NoEdge = std::numeric_limits<int>::max();

struct TSP_Solver {
    int n; // 城市数量
    std::vector<std::vector<int>> adj; // 邻接矩阵
    std::vector<std::vector<int>> dp; // DP表
    std::vector<std::vector<int>> parent; // 父节点信息，用于路径重建

    TSP_Solver(int num_cities, const std::vector<std::vector<int>> &adj_matrix)
        : n(num_cities), adj(adj_matrix),
          dp(1 << num_cities, std::vector<int>(num_cities, NoEdge)),
          parent(1 << num_cities, std::vector<int>(num_cities, -1)) {}

    std::vector<int> ReconstructPath(int final_mask, int last_city) const {
        std::vector<int> path;
        int mask = final_mask;
        int current_city = last_city;

        while (current_city != 0) { 
            path.push_back(current_city);
            int prev_city = parent[mask][current_city];
            if (prev_city == -1) {
                return {};
            }
            mask ^= (1 << current_city);
            current_city = prev_city;
        }

        path.push_back(0);
        std::reverse(path.begin(), path.end());
        path.push_back(0); 

        return path;
    }

    int Solve(std::vector<int> &bestx) {
        dp[1 << 0][0] = 0;

        int N = 1 << n; 
        for (int mask = 1; mask < N; ++mask) {
            for (int u = 0; u < n; ++u) {
                if (!(mask & (1 << u))) continue;

                if (dp[mask][u] == NoEdge) continue;

                for (int v = 0; v < n; ++v) {
                    if (mask & (1 << v)) continue;
                    if (adj[u][v] == NoEdge) continue;

                    int next_mask = mask | (1 << v);
                    if (dp[next_mask][v] > dp[mask][u] + adj[u][v]) {
                        dp[next_mask][v] = dp[mask][u] + adj[u][v];
                        parent[next_mask][v] = u;
                    }
                }
            }
        }

        int final_mask = N - 1;
        int min_cost = NoEdge;
        int last_city = -1;

        for (int u = 1; u < n; ++u) {
            if (adj[u][0] != NoEdge && dp[final_mask][u] != NoEdge) {
                int total_cost = dp[final_mask][u] + adj[u][0];
                if (total_cost < min_cost) {
                    min_cost = total_cost;
                    last_city = u;
                }
            }
        }

        if (min_cost == NoEdge) {
            return NoEdge;
        }

        bestx = ReconstructPath(final_mask, last_city);
        return min_cost;
    }
};

int main() {
    int n;
    std::cout << "请输入城市数量 n (建议 n <= 20): ";
    std::cin >> n;

    if(n <= 1){
        std::cout << "城市数量必须大于1。\n";
        return 0;
    }

    std::vector<std::vector<int>> adj(n, std::vector<int>(n, NoEdge));
    std::cout << "请输入邻接矩阵 (使用 " << NoEdge << " 表示没有边):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int cost;
            std::cin >> cost;
            if (cost != NoEdge || i == j) { // 允许自环以存储0成本
                adj[i][j] = cost;
            }
        }
    }

    // 检查每个城市是否至少有一条出边
    bool valid = true;
    for(int i = 0; i < n; ++i){
        bool hasOut = false;
        for(int j = 0; j < n; ++j){
            if(i != j && adj[i][j] != NoEdge){
                hasOut = true;
                break;
            }
        }
        if(!hasOut){
            std::cout << "城市 " << (i + 1) << " 没有出边，无法完成旅行。\n";
            valid = false;
            break;
        }
    }

    if(!valid){
        return 0;
    }

    // 创建TSP求解器实例
    TSP_Solver solver(n, adj);
    std::vector<int> bestx;
    int bestCost = solver.Solve(bestx);

    if (bestCost == NoEdge) {
        std::cout << "没有找到有效的旅行路径。\n";
    } else {
        std::cout << "最小成本: " << bestCost << "\n";
        std::cout << "最佳路径: ";
        for (size_t i = 0; i < bestx.size(); ++i) {
            std::cout << (bestx[i] + 1); // 转换为1-based编号
            if (i < bestx.size() - 1) {
                std::cout << " -> ";
            } else {
                std::cout << "\n";
            }
        }
    }

    return 0;
}
