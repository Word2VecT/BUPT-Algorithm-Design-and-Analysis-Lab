#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip> // 引入用于格式化输出的头文件
#include <iostream>
#include <string>
#include <vector>

constexpr int nDefaultCount = 1000000;
constexpr int trials = 10000;
constexpr int kTimes = 1;
constexpr int groupSize = 5;

int findMedian(std::vector<int>& nums, int left, int right) {
    std::sort(nums.begin() + left, nums.begin() + right + 1);
    return nums[left + (right - left) / 2];
}

// 中位数中位数算法：确定性的线性时间选择算法
int deterministicSelect(std::vector<int>& nums, int left, int right, int k) { //NOLINT
    // 如果数组足够小，直接返回中位数
    if (right - left + 1 <= groupSize) {
        return findMedian(nums, left, right);
    }

    // 将数组分成每组最多5个元素，找到每组的中位数
    std::vector<int> medians;
    for (int i = left; i <= right; i += groupSize) {
        int sub_right = std::min(i + 4, right);
        medians.push_back(findMedian(nums, i, sub_right));
    }

    // 递归地找到中位数的中位数
    int medianOfMedians = deterministicSelect(medians, 0, static_cast<int>(medians.size()) - 1, static_cast<int>(medians.size()) / 2);

    // 找到中位数中位数的位置
    std::vector<int>::difference_type pivotIndex = std::distance(nums.begin(), std::find(nums.begin() + left, nums.begin() + right + 1, medianOfMedians));

    // 移动枢轴到末尾
    std::swap(nums[pivotIndex], nums[right]);

    // 分区
    int storeIndex = left;
    for (int i = left; i < right; ++i) {
        if (nums[i] > medianOfMedians) { // 查找第 k 大，因此使用 > 符号
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
        return deterministicSelect(nums, left, storeIndex - 1, k);
    }
    return deterministicSelect(nums, storeIndex + 1, right, k - count);
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
    filename = "/Users/tang/Course/算法设计与分析/Lab/Lab 1/times/determine_continue_khalfn_" + std::to_string(nDefaultCount) + ".txt";
    std::ofstream outfile(filename, std::ios::app);
    if (!outfile) {
        std::cerr << "无法以追加模式打开文件: " << filename << '\n';
        return 1;
    }

    for (int i = 1; i <= kTimes; ++i) {
        int k_count = nDefaultCount / 2;

        double total_duration_us = 0.0;
        std::vector<int> nums_copy = nums;

        for (int j = 0; j < trials; ++j) {
            // 记录开始时间
            auto start_time = std::chrono::high_resolution_clock::now();

            // 调用快速选择算法
            deterministicSelect(nums_copy, 0, static_cast<int>(nums_copy.size()) - 1, k_count);

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