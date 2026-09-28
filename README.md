# Hough Circle Detection (OpenCV)

用 OpenCV 对图片做霍夫圆检测，并通过滑动条实时调整检测参数。

## 功能

- 读取 `resource/test.png`，做灰度化 + 高斯模糊预处理
- 用 `cv::HoughCircles` 检测圆，在原图上画出圆心和轮廓
- 5 个滑动条实时调参，拖动即刷新结果
- 结果图上用文字显示当前检测到的圆数量，方便调参

## 运行环境

| 项目 | 版本 / 路径 |
| --- | --- |
| 编译器 | MinGW-w64 13.1.0（`g++` / `gdb`） |
| OpenCV | 4.11.0（MinGW 版） |

`.vscode/` 里已经配好编译和调试，用 VS Code 打开本文件夹、打开 `main.cpp`，按 `F5` 即可自动编译并启动调试；按 `Ctrl+Shift+B` 只编译。

> **换机器时注意**：`.vscode` 下的配置写的是本机绝对路径，需要按你的实际安装位置修改这三处：
> - `tasks.json`：`-I` 头文件路径、`-L` 库路径、`-lopencv_world4110` 版本号
> - `launch.json`：`environment` 里 PATH 中的 OpenCV `bin` 目录
> - `c_cpp_properties.json`：`includePath`

## 使用方法

1. 用 VS Code 打开本文件夹（注意是「打开文件夹」，不是「打开文件」）
2. 打开 `main.cpp`，按 `F5`
3. 拖动滑动条调参，按任意键关闭窗口

## 参数说明

| 滑动条 | 对应参数 | 含义与调参建议 |
| --- | --- | --- |
| Min Dist | `minDist` | 圆心之间的最小距离。太小会在同一个圆上检出多个重复圆心 |
| Param1 | `param1` | Canny 边缘检测的高阈值，一般 100 左右 |
| Param2 | `param2` | 累加器阈值，越大检出的圆越少但越可靠，是主要调节对象 |
| Min Radius | `minRadius` | 半径搜索下限，先放宽再收紧 |
| Max Radius | `maxRadius` | 半径搜索上限，先放宽再收紧 |

经验顺序：先把半径范围放宽，再从 10 左右开始慢慢调大 `param2`，最后用 `Min Dist` 去掉挨得太近的重复圆心。

## 目录结构

```
.
├── main.cpp                  # 主程序
├── resource/
│   └── test.png              # 测试图片
└── .vscode/                  # 编译与调试配置
    ├── tasks.json
    ├── launch.json
    ├── c_cpp_properties.json
    └── settings.json
```
