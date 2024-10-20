import matplotlib.pyplot as plt

# 固定的 n 值
n = 1000000

# 定义 k 的位置（k = n/8, 2n/8, ..., 8n/8）
k_positions = [n * i / 8 for i in range(1, 9)]
k_labels = [f"{int(k)}" for k in k_positions]

# 对应的运行时间（单位：微秒）
# Random Select 的运行时间
runtimes_random_us = [
    49058.997,
    54267.771,
    60192.906,
    60239.248,
    59630.191,
    54661.765,
    50720.049,
    36684.486,
]

# Determine Select 的运行时间（请根据实际数据填写）
runtimes_determine_us = [
    202223.426,
    204387.987,
    199386.619,
    196773.577,
    200691.490,
    200061.429,
    199558.440,
    201403.638,
]

# 创建图形和轴
plt.figure(figsize=(12, 8))

# 绘制 Random Select 折线图
plt.plot(
    k_positions,
    runtimes_random_us,
    marker="o",
    linestyle="-",
    color="b",
    label="Random Select",
)

# 绘制 Determine Select 折线图
plt.plot(
    k_positions,
    runtimes_determine_us,
    marker="s",
    linestyle="--",
    color="r",
    label="Determine Select",
)

# 标注具体的运行时间（Random Select）
for k, runtime in zip(k_positions, runtimes_random_us):
    plt.annotate(
        f"{runtime}",
        (k, runtime),
        textcoords="offset points",
        xytext=(0, 5),
        ha="center",
        color="b",
    )

# 标注具体的运行时间（Determine Select）
for k, runtime in zip(k_positions, runtimes_determine_us):
    plt.annotate(
        f"{runtime}",
        (k, runtime),
        textcoords="offset points",
        xytext=(0, -15),
        ha="center",
        color="r",
    )

# 添加标题和标签
plt.title(
    f"Random Select vs Determine Select Runtime at Different k Positions when n={n}",
    fontsize=14,
)
plt.xlabel("k Position (k)", fontsize=12)
plt.ylabel("Runtime (microseconds, μs)", fontsize=12)

# 设置 x 轴刻度为整数
plt.xticks(k_positions, k_labels)

# 添加网格
plt.grid(True, linestyle="--", alpha=0.7)

# 添加图例
plt.legend()

# 显示图表
plt.tight_layout()
plt.savefig(f"./figure/{n}.png")
plt.show()
