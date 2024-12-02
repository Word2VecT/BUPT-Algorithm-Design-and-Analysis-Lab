#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>

const int NoEdge = std::numeric_limits<int>::max();

struct TSP_Solver {
    int bestc;                      // 最佳成本
    std::vector<int> bestx;         // 最佳路径
    int cc;                         // 当前路径成本
    std::vector<int> min_out;       // 每个城市的最小出边成本

    TSP_Solver(int n) : bestc(NoEdge), bestx(n, 0), cc(0), min_out(n, NoEdge) {}

    void precompute_min_out(int n, const std::vector<std::vector<int>> &a){
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n; ++j){
                if(i != j && a[i][j] < min_out[i]){
                    min_out[i] = a[i][j];
                }
            }
        }
    }

    void Backtrack(int i, int n, const std::vector<std::vector<int>> &a, std::vector<int> &x, int visited) {
        if(i == n){
            // 检查是否可以回到起点
            if(a[x[n - 1]][x[0]] != NoEdge){
                int totalCost = cc + a[x[n - 1]][x[0]];
                if(totalCost < bestc){
                    bestc = totalCost;
                    bestx = x;
                }
            }
            return;
        }

        for(int j = 0; j < n; ++j){
            if(!(visited & (1 << j)) && a[x[i - 1]][j] != NoEdge){
                int new_cost = cc + a[x[i - 1]][j];

                // 计算下界
                int lower_bound = new_cost;
                for(int k = 0; k < n; ++k){
                    if(!(visited & (1 << k))){
                        lower_bound += min_out[k];
                    }
                }

                if(lower_bound < bestc){
                    x[i] = j;
                    cc += a[x[i - 1]][j];           
                    Backtrack(i + 1, n, a, x, visited | (1 << j)); 
                    cc -= a[x[i - 1]][j];           
                }
            }
        }
    }

    int solve(const std::vector<std::vector<int>> &a, std::vector<int> &v, int n){
        std::vector<int> x(n, 0); 
        for(int i = 0; i < n; ++i){
            x[i] = i;
        }

        bestx = x;
        precompute_min_out(n, a); 
        Backtrack(1, n, a, x, 1 << 0);

        v = bestx; 
        return bestc;
    }
};

int main(){
    int n;
    std::cout << "请输入城市数量 n: ";
    std::cin >> n;

    if(n <= 1){
        std::cout << "城市数量必须大于1。\n";
        return 0;
    }

    std::vector<std::vector<int>> adj(n, std::vector<int>(n, NoEdge));

    std::cout << "请输入邻接矩阵 (使用 " << NoEdge << " 表示没有边):\n";
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            int cost;
            std::cin >> cost;
            if(cost != NoEdge){
                adj[i][j] = cost;
            }
        }
    }

    // 检查是否每个城市至少有一条出边
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

    std::vector<int> v(n, 0);
    TSP_Solver solver(n);
    int bestCost = solver.solve(adj, v, n);

    if(bestCost == NoEdge){
        std::cout << "没有找到有效的旅行路径。\n";
    }
    else{
        std::cout << "最小成本: " << bestCost << "\n";
        std::cout << "最佳路径: ";
        for(int i = 0; i < n; ++i){
            std::cout << (v[i] + 1);
            if(i < n - 1){
                std::cout << " -> ";
            }
            else{
                std::cout << " -> " << (v[0] + 1) << "\n"; // 回到起点
            }
        }
    }

    return 0;
}
