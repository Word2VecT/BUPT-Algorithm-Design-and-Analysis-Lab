#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <vector>

const int NoEdge = std::numeric_limits<int>::max();
const double alpha = 1.0; // 信息素重要程度
const double beta = 5.0;  // 启发式信息重要程度
const double rho = 0.5;   // 信息素挥发系数
const double Q = 100.0;   // 常数用于信息素更新
const int MAX_ITER = 1000; // 最大迭代次数
const int NUM_ANTS = 20;   // 蚂蚁数量

int CalculatePathCost(const std::vector<int> &path, const std::vector<std::vector<int>> &adj) {
    int cost = 0;
    int n = path.size();
    for (int i = 0; i < n - 1; ++i) {
        cost += adj[path[i]][path[i + 1]];
    }
    cost += adj[path.back()][path[0]];
    return cost;
}

int SelectNextCity(int currentCity, const std::vector<std::vector<double>> &pheromone,
                   const std::vector<std::vector<int>> &adj, const std::vector<std::vector<double>> &visibility,
                   const std::vector<bool> &visited) {
    double sumProb = 0.0;
    int n = adj.size();
    std::vector<double> prob(n, 0.0);

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && adj[currentCity][i] != NoEdge) {
            prob[i] = pow(pheromone[currentCity][i], alpha) * pow(visibility[currentCity][i], beta);
            sumProb += prob[i];
        }
    }

    if (sumProb == 0) return -1;

    double randVal = ((double)rand() / RAND_MAX) * sumProb;
    double cumSum = 0.0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && adj[currentCity][i] != NoEdge) {
            cumSum += prob[i];
            if (cumSum >= randVal) return i;
        }
    }

    return -1;
}

void UpdatePheromone(std::vector<std::vector<double>> &pheromone, const std::vector<std::vector<int>> &adj,
                     const std::vector<std::vector<int>> &antPaths, const std::vector<int> &pathCosts) {
    int n = adj.size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pheromone[i][j] *= (1.0 - rho);
        }
    }

    for (size_t ant = 0; ant < antPaths.size(); ++ant) {
        int pathCost = pathCosts[ant];
        for (size_t i = 0; i < antPaths[ant].size() - 1; ++i) {
            int from = antPaths[ant][i];
            int to = antPaths[ant][i + 1];
            pheromone[from][to] += Q / pathCost;
            pheromone[to][from] += Q / pathCost;
        }
    }
}

std::pair<int, std::vector<int>> AntColonyOptimization(const std::vector<std::vector<int>> &graph) {
    int n = graph.size();
    std::vector<std::vector<double>> pheromone(n, std::vector<double>(n, 1.0));
    std::vector<std::vector<double>> visibility(n, std::vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graph[i][j] != NoEdge) {
                visibility[i][j] = 1.0 / graph[i][j];
            }
        }
    }

    std::vector<int> bestPath;
    int bestCost = NoEdge;
    srand(time(0)); 

    for (int iter = 0; iter < MAX_ITER; ++iter) {
        std::vector<std::vector<int>> antPaths(NUM_ANTS, std::vector<int>(n));
        std::vector<int> pathCosts(NUM_ANTS, 0);

        for (int ant = 0; ant < NUM_ANTS; ++ant) {
            std::vector<bool> visited(n, false);
            int currentCity = rand() % n;
            visited[currentCity] = true;
            antPaths[ant][0] = currentCity;

            for (int step = 1; step < n; ++step) {
                int nextCity = SelectNextCity(currentCity, pheromone, graph, visibility, visited);
                antPaths[ant][step] = nextCity;
                visited[nextCity] = true;
                currentCity = nextCity;
            }

            antPaths[ant].push_back(antPaths[ant][0]);

            pathCosts[ant] = CalculatePathCost(antPaths[ant], graph);
        }

        UpdatePheromone(pheromone, graph, antPaths, pathCosts);

        for (int ant = 0; ant < NUM_ANTS; ++ant) {
            if (pathCosts[ant] < bestCost) {
                bestCost = pathCosts[ant];
                bestPath = antPaths[ant];
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
            std::cin >> adj[i][j];
        }
    }

    auto [bestCost, bestPath] = AntColonyOptimization(adj);

    std::cout << "近似最小成本: " << bestCost << "\n";
    std::cout << "路径: ";
    for (size_t i = 0; i < bestPath.size(); ++i) {
        std::cout << bestPath[i] + 1;
        if (i < bestPath.size() - 1) {
            std::cout << " -> ";
        } else {
            std::cout << "\n";
        }
    }

    return 0;
}