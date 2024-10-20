import matplotlib.pyplot as plt
import numpy as np

# 设置文件路径
file_path = "/Users/tang/Course/算法设计与分析/Lab/Lab 1/times/determine_continue_1000000.txt"  # 请替换为您的实际文件路径

# 固定的 n 值
n = 1000000

# 定义 k 的位置（k = n/8, 2n/8, ..., 8n/8）
k_positions = [n * i / 8 for i in range(1, 9)]
k_labels = [f"k={int(k)}" for k in k_positions]

# 实验次数
num_experiments = 10000

# 初始化一个列表来存储所有 k 的运行时间数据
runtimes = []

# 读取文件并解析数据
try:
    with open(file_path, "r") as file:
        for line_num, line in enumerate(file, start=1):
            # 去除行末的换行符并按空格分割
            run_time_strs = line.strip().split()
            # 检查每行是否有10000个数据
            if len(run_time_strs) != num_experiments:
                raise ValueError(
                    f"第 {line_num} 行的数据数量不为 {num_experiments}，实际为 {len(run_time_strs)}。"
                )
            # 将字符串转换为浮点数
            run_time_floats = [float(rt) for rt in run_time_strs]
            runtimes.append(run_time_floats)
except FileNotFoundError:
    print(f"文件未找到: {file_path}")
    exit(1)
except ValueError as ve:
    print(f"数据格式错误: {ve}")
    exit(1)

# 检查是否有8个 k 的数据
if len(runtimes) != 8:
    print(f"预期8个 k 的数据，但读取到 {len(runtimes)} 个。")
    exit(1)

# 准备x轴数据（实验次数）
x = np.arange(1, num_experiments + 1)

# 创建图形和轴
plt.figure(figsize=(14, 8))

# 为每个 k 绘制运行时间曲线
for i in range(8):
    plt.plot(
        x,
        runtimes[i],
        label=k_labels[i],
        alpha=0.6,  # 设置透明度以便更好地展示重叠部分
        linewidth=1,  # 线宽
    )

# 添加标题和轴标签
plt.title(f"Determine Select Runtime for n={n} at Different k Positions", fontsize=16)
plt.xlabel("Experiment Number", fontsize=14)
plt.ylabel("Runtime (microseconds, μs)", fontsize=14)

# 添加图例
plt.legend(title="k Positions", fontsize=10, loc="upper right")

# 添加网格
plt.grid(True, linestyle="--", alpha=0.5)

# 优化布局
plt.tight_layout()

plt.savefig("./figure/determine_continue.png")

# 显示图表
plt.show()
