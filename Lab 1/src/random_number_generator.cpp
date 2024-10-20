#include <fstream>
#include <iostream>
#include <limits> // 引入以获取 int 类型的最小值和最大值
#include <random>
#include <string>
#include <vector>

constexpr int nDefaultCount = 10000000; // 默认生成的随机数数量

int main() {
    std::string filename = "/Users/tang/Course/算法设计与分析/Lab/Lab 1/random_numbers/random_numbers_" + std::to_string(nDefaultCount) + ".txt"; // 保存随机数的文件名

    // 创建随机数生成器
    std::random_device seed;  // 用于获取随机种子
    std::mt19937 gen(seed()); // 使用梅森旋转算法的随机数生成器

    // 定义均匀整数分布，范围为 int 类型的最小值到最大值
    std::uniform_int_distribution<int> distrib(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    // 存储生成的随机数
    std::vector<int> random_numbers;
    random_numbers.reserve(nDefaultCount);

    for (int i = 0; i < nDefaultCount; ++i) {
        random_numbers.push_back(distrib(gen));
    }

    // 将随机数写入文件
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "无法打开文件: " << filename << '\n';
        return 1;
    }

    for (const auto& num : random_numbers) {
        outfile << num << " ";
    }
    outfile << '\n';

    outfile.close(); // 关闭文件

    std::cout << "随机数已成功保存到文件: " << filename << '\n';

    return 0;
}