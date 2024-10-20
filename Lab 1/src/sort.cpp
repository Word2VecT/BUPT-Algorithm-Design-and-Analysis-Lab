#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main() {
    std::ifstream inputFile("/Users/tang/Course/算法设计与分析/Lab/Lab 1/random_numbers/random_numbers_1000000.txt"); // 输入文件
    std::ofstream outputFile("/Users/tang/Course/算法设计与分析/Lab/Lab 1/random_numbers/sorted_numbers_1000000.txt"); // 输出文件

    if (!inputFile.is_open()) {
        std::cerr << "无法打开输入文件！" << '\n';
        return 1;
    }

    std::vector<int> numbers;
    int number = 0;

    // 读取文件中的数字
    while (inputFile >> number) {
        numbers.push_back(number);
    }

    // 排序
    std::sort(numbers.begin(), numbers.end());

    // 输出到另一个文件
    for (const int& num : numbers) {
        outputFile << num << " ";
    }

    inputFile.close();
    outputFile.close();

    std::cout << "排序完成并写入！" << '\n';
    return 0;
}