import matplotlib.pyplot as plt
import numpy as np

# 设置有序和无序数据的文件路径
ordered_file_path = "/Users/tang/Course/算法设计与分析/Lab/Lab 1/times/determine_continue_khalfn_1000000.txt"  # 请替换为有序文件路径
unordered_file_path = "/Users/tang/Course/算法设计与分析/Lab/Lab 1/times/determine_continue_khalfn_sorted_1000000.txt"  # 请替换为无序文件路径

# 固定的 n 值
n = 1000000

# 实验次数
num_experiments = 10000

# 初始化两个列表来存储有序和无序数据的运行时间
ordered_runtimes = []
unordered_runtimes = []

# 定义 k 的位置为 n/2
k_label = f"k={n // 2}"

# 读取有序数据文件
try:
    with open(ordered_file_path, "r") as file:
        for line_num, line in enumerate(file, start=1):
            run_time_strs = line.strip().split()
            if len(run_time_strs) != num_experiments:
                raise ValueError(
                    f"有序数据第 {line_num} 行的数据数量不为 {num_experiments}，实际为 {len(run_time_strs)}。"
                )
            run_time_floats = [float(rt) for rt in run_time_strs]
            ordered_runtimes.append(run_time_floats)
except FileNotFoundError:
    print(f"文件未找到: {ordered_file_path}")
    exit(1)
except ValueError as ve:
    print(f"数据格式错误: {ve}")
    exit(1)

# 读取无序数据文件
try:
    with open(unordered_file_path, "r") as file:
        for line_num, line in enumerate(file, start=1):
            run_time_strs = line.strip().split()
            if len(run_time_strs) != num_experiments:
                raise ValueError(
                    f"无序数据第 {line_num} 行的数据数量不为 {num_experiments}，实际为 {len(run_time_strs)}。"
                )
            run_time_floats = [float(rt) for rt in run_time_strs]
            unordered_runtimes.append(run_time_floats)
except FileNotFoundError:
    print(f"文件未找到: {unordered_file_path}")
    exit(1)
except ValueError as ve:
    print(f"数据格式错误: {ve}")
    exit(1)

# 检查是否读取到正确的k=n/2的数据
if len(ordered_runtimes) != 1 or len(unordered_runtimes) != 1:
    print(
        f"预期1组有序和1组无序数据，但读取到 {len(ordered_runtimes)} 组有序和 {len(unordered_runtimes)} 组无序数据。"
    )
    exit(1)

# 准备x轴数据（实验次数）
x = np.arange(1, num_experiments + 1)

# 创建图形和轴
plt.figure(figsize=(14, 8))

# 绘制有序和无序数据的运行时间曲线
plt.plot(
    x,
    ordered_runtimes[0],
    label=f"Ordered Origin Data {k_label}",
    alpha=0.6,
    linewidth=1,
    color="blue",
)
plt.plot(
    x,
    unordered_runtimes[0],
    label=f"Unordered Origin Data {k_label}",
    alpha=0.6,
    linewidth=1,
    color="red",
)

# 添加标题和轴标签
plt.title(f"Determine Select Runtime Comparison for n={n} at k={n//2}", fontsize=16)
plt.xlabel("Experiment Number", fontsize=14)
plt.ylabel("Runtime (microseconds, μs)", fontsize=14)

# 添加图例
plt.legend(title="Order Status", fontsize=10, loc="upper right")

# 添加网格
plt.grid(True, linestyle="--", alpha=0.5)

# 优化布局
plt.tight_layout()

# 保存图像
plt.savefig("./figure/determine_continue_order_vs_unorder.png")

# 显示图表
plt.show()
