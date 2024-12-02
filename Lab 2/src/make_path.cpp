#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

std::vector<std::vector<int>> generateMatrix(int n, int minVal = 1, int maxVal = 100) {
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                matrix[i][j] = minVal + rand() % (maxVal - minVal + 1);
            }
        }
    }
    return matrix;
}

int main() {
    int n;
    std::cout << "Enter the size of the matrix: ";
    std::cin >> n;

    srand(static_cast<unsigned>(time(0)));

    std::vector<std::vector<int>> matrix = generateMatrix(n);

    for (const auto& row : matrix) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << '\n';
    }

    return 0;
}