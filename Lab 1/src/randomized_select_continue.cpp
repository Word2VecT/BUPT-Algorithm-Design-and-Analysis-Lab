#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip> // 引入用于格式化输出的头文件
#include <iostream>
#include <random>
#include <string>
#include <vector>

constexpr int nDefaultCount = 1000000;
constexpr int trials = 100;
constexpr int kTimes = 8;

// 快速选择算法：查找第 k 大的元素（k 从 1 开始）
int quickSelect(std::vector<int>& nums, int left, int right, int k, std::mt19937& gen) // NOLINT
{
    if (left == right) {
        return nums[left];
    }

    // 随机选择枢轴
    std::uniform_int_distribution<> dis(left, right);
    int pivotIndex = dis(gen);
    int pivot = nums[pivotIndex];

    // 移动枢轴到末尾
    std::swap(nums[pivotIndex], nums[right]);

    // 分区
    int storeIndex = left;
    for (int i = left; i < right; ++i) {
        if (nums[i] > pivot) { // 查找第 k 大，因此使用 > 符号
            std::swap(nums[i], nums[storeIndex]);
            storeIndex++;
        }
    }
    // 将枢轴放到正确的位置
    std::swap(nums[storeIndex], nums[right]);

    // 计算枢轴位置相对于第 k 大的位置
    int count = storeIndex - left + 1;
    if (count == k) {
        return nums[storeIndex];
    }
    if (k < count) {
        return quickSelect(nums, left, storeIndex - 1, k, gen);
    }
    return quickSelect(nums, storeIndex + 1, right, k - count, gen);
}

int main()
{
    // 构造文件名
    std::string filename = "/Users/tang/Course/算法设计与分析/Lab/Lab 1/random_numbers/random_numbers_" + std::to_string(nDefaultCount) + ".txt";

    // 读取文件中的数字
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "无法打开文件: " << filename << '\n';
        return 1;
    }

    std::vector<int> nums;
    int num = 0;
    while (infile >> num) {
        nums.push_back(num);
    }
    infile.close();

    if (nums.empty()) {
        std::cerr << "文件中没有读取到任何数字。" << '\n';
        return 1;
    }

    // 将平均运行时间附加到文件末尾，以空格隔开
    filename = "/Users/tang/Course/算法设计与分析/Lab/Lab 1/times/random_continue_" + std::to_string(nDefaultCount) + ".txt";
    std::ofstream outfile(filename, std::ios::app);
    if (!outfile) {
        std::cerr << "无法以追加模式打开文件: " << filename << '\n';
        return 1;
    }

    for (int i = 1; i <= kTimes; ++i) {
        int k_count = nDefaultCount / kTimes * i;

        // 初始化随机数生成器
        std::random_device seed;
        std::mt19937 gen(seed());

        double total_duration_us = 0.0;
        std::vector<int> nums_copy = nums;

        for (int j = 0; j < trials; ++j) {
            // 记录开始时间
            auto start_time = std::chrono::high_resolution_clock::now();

            // 调用快速选择算法
            quickSelect(nums_copy, 0, static_cast<int>(nums_copy.size()) - 1, k_count, gen);

            // 记录结束时间
            auto end_time = std::chrono::high_resolution_clock::now();

            // 计算持续时间
            std::chrono::duration<double, std::micro> duration_us = end_time - start_time;
            total_duration_us += duration_us.count();

            std::cout << "单次查找耗时: " << std::fixed << std::setprecision(3) << duration_us.count() << " 微秒" << '\n';
            outfile << std::fixed << std::setprecision(3) << duration_us.count() << ' ';
        }
        double average_duration_us = total_duration_us / trials;

        // 输出结果
        std::cout << "共 " << nums.size() << " 个元素" << '\n';
        std::cout << "第 " << k_count << " 大的元素:" << '\n';
        std::cout << "平均查找耗时: " << std::fixed << std::setprecision(3) << average_duration_us << " 微秒\n" << '\n';

        outfile << '\n';
    }
    outfile.close();

    return 0;
}