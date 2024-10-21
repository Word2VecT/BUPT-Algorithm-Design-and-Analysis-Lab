import matplotlib.pyplot as plt

# 定义不同的 n 值
n_values = [1000, 100000, 10000000, 100000000]

# 对应的运行时间（单位：微秒），根据每个 n 下 8 个 k 值的运行时间
# Random Select 的运行时间
runtimes_random_us = [
    [4.246, 5.144, 5.530, 5.568, 5.438, 5.112, 4.391, 2.975],  # n=1000
    [
        469.199,
        528.008,
        563.914,
        580.783,
        586.067,
        543.999,
        491.631,
        347.473,
    ],  # n=100000
    [
        4695.565,
        5557.617,
        5754.285,
        6206.381,
        6026.155,
        5387.948,
        5027.979,
        3468.241,
    ],  # n=10000000
    [
        49058.997,
        54267.771,
        60192.906,
        60239.248,
        59630.191,
        54661.765,
        50720.049,
        36684.486,
    ],  # n=100000000
]

# Determine Select 的运行时间
runtimes_determine_us = [
    [12.684, 12.718, 12.605, 13.153, 13.457, 13.759, 13.722, 13.868],  # n=1000
    [
        1991.647,
        1952.677,
        2025.208,
        1938.950,
        1891.054,
        1903.455,
        1899.478,
        1898.573,
    ],  # n=100000
    [
        19083.830,
        19562.915,
        19067.446,
        20208.430,
        20625.970,
        21064.015,
        20035.567,
        19695.616,
    ],  # n=10000000
    [
        202223.426,
        204387.987,
        199386.619,
        196773.577,
        200691.490,
        200061.429,
        199558.440,
        201403.638,
    ],  # n=100000000
]

# 创建图形和轴
plt.figure(figsize=(12, 8))

# 为每个 n 值的每个 k 绘制 random select 的折线图
for j in range(8):  # 遍历 8 个 k 值
    plt.plot(
        n_values,  # n 值作为 x 轴
        [
            runtimes_random_us[i][j] for i in range(len(n_values))
        ],  # 对应 k 值的运行时间作为 y 轴
        marker="o",
        linestyle="-",
        label=f"Random Select k={j+1}n/8",
        color=f"C{j}",  # 使用不同的颜色区分不同的 k
    )

# 为每个 n 值的每个 k 绘制 determine select 的折线图
for j in range(8):  # 遍历 8 个 k 值
    plt.plot(
        n_values,  # n 值作为 x 轴
        [
            runtimes_determine_us[i][j] for i in range(len(n_values))
        ],  # 对应 k 值的运行时间作为 y 轴
        marker="s",
        linestyle="--",
        label=f"Determine Select k={j+1}n/8",
        color=f"C{j+8}",  # 使用不同的颜色区分不同的 k
    )

# 添加标题和标签
plt.title(
    "Random Select vs Determine Select Runtime at Different n Values and k Positions",
    fontsize=14,
)
plt.xlabel("n (Size of input)", fontsize=12)
plt.ylabel("Runtime (microseconds, μs)", fontsize=12)

# 设置 x 轴为 n 的大小，并且为每个 n 显示刻度
# plt.xscale("log")  # 因为 n 的范围变化较大，使用对数刻度
plt.xticks(n_values, [f"{n}" for n in n_values])

# 添加网格
plt.grid(True, linestyle="--", alpha=0.7)

# 添加图例
plt.legend(bbox_to_anchor=(1.05, 1), loc="upper left")

# 显示图表
plt.tight_layout()
plt.show()
