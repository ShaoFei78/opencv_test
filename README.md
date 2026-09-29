# OpenCV 图像处理练习

用 C++ 和 OpenCV 完成图像处理练习，每个任务用一个分支管理。

| 分支 | 任务 | 内容 |
| --- | --- | --- |
| `main` | 任务一 | 霍夫圆检测（Hough Circle），滑动条实时调参 |
| `task2` | 任务二 | 颜色提取，提取魔方的黄色和蓝色并框出位置 |

当前分支是 `task2`，下面的说明针对这个分支的代码。

## 功能

程序读取 `resource/task2.png`，一共打开四个窗口：

| 窗口 | 内容 |
| --- | --- |
| `Image` | 原始图像 |
| `GrayImage` | 灰度图（`cvtColor` + `COLOR_BGR2GRAY`） |
| `Color Detection` | 颜色提取结果，白色部分是检出的黄色和蓝色区域 |
| `Detected Colors` | 在彩色原图上用红色矩形框出检出的区域 |

## 处理流程

1. 用 `cvtColor` 把原图从 BGR 转到 HSV。HSV 把色调、饱和度、明度分开，比 BGR 更适合按颜色分割。
2. 用 `inRange` 按上下限阈值分别提取黄色和蓝色，得到两张二值掩膜。
3. 用 `bitwise_or` 把两张掩膜合并成一张。
4. 用 `morphologyEx` 做一次闭运算再开运算，去掉零散噪点、填补区域内部的空洞。
5. 用 `findContours` 找出每块区域的轮廓，`boundingRect` 求外接矩形，面积小于 200 的小块忽略。
6. 在原图的副本上用 `rectangle` 画出红色边框。

## 滑动条参数

12 个滑动条分成两组，分别控制黄色和蓝色的 HSV 阈值，拖动后立即重新处理并刷新窗口。

| 滑动条 | 作用 | 当前默认值 |
| --- | --- | --- |
| `Y_Hmin` / `Y_Hmax` | 黄色色调范围 | 23 ~ 37 |
| `Y_Smin` / `Y_Smax` | 黄色饱和度范围 | 72 ~ 255 |
| `Y_Vmin` / `Y_Vmax` | 黄色明度范围 | 90 ~ 255 |
| `B_Hmin` / `B_Hmax` | 蓝色色调范围 | 77 ~ 124 |
| `B_Smin` / `B_Smax` | 蓝色饱和度范围 | 90 ~ 255 |
| `B_Vmin` / `B_Vmax` | 蓝色明度范围 | 33 ~ 255 |

小提示：OpenCV 里 H 的范围是 0~179 而不是 360，S 和 V 是 0~255。调参时如果背景被误检进来，优先提高 `Smin`、`Vmin` 把灰白和暗部压掉；如果目标总是漏检，先放宽这两个下限，再微调 H 的两端。

## 实现要点

阈值对象 `Scalar` 必须在回调函数**内部**构造，每次都重新读滑动条的当前值：

```cpp
void onTrackbar(int, void*) {
    Scalar y_lower(y_hmin, y_smin, y_vmin);
    Scalar y_upper(y_hmax, y_smax, y_vmax);
    // ...
    inRange(imgHSV, y_lower, y_upper, y_mask);
}
```

如果把它们写成全局变量，只会在程序启动时计算一次，之后拖动滑动条时 `inRange` 用的还是最初那组阈值，画面不会发生任何变化。

## 运行环境

| 项目 | 版本 |
| --- | --- |
| 编译器 / 调试器 | MinGW-w64 13.1.0（`g++` / `gdb`） |
| OpenCV | 4.11.0（MinGW 版） |
| C++ 标准 | C++17 |

`.vscode/` 里已经配好编译和调试。用 VS Code 打开本文件夹、打开 `main.cpp`，按 `F5` 自动编译并启动调试，按 `Ctrl+Shift+B` 只编译。

> **换机器时注意**：`.vscode` 下的配置写的是本机绝对路径，需要改成你自己的安装位置：
> - `tasks.json`：`-I` 头文件路径、`-L` 库路径、`-lopencv_world4110` 版本号
> - `launch.json`：`environment` 里 PATH 中的 OpenCV `bin` 目录
> - `c_cpp_properties.json`：`includePath`

## 使用方法

1. 用 VS Code 打开本文件夹（注意是「打开文件夹」，不是「打开文件」）
2. 打开 `main.cpp`，按 `F5`
3. 拖动滑动条调参，按任意键关闭窗口

## 目录结构

```
.
├── main.cpp                  # 主程序
├── resource/
│   └── task2.png             # 测试图片
└── .vscode/                  # 编译与调试配置
    ├── tasks.json
    ├── launch.json
    ├── c_cpp_properties.json
    └── settings.json
```
