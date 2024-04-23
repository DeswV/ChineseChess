[>English version here<](./README-en.md)

![screenshot](C:\My Files\ChineseChess\images\screenshot.png)

### 简介

这是一个简单（而粗糙）的中国象棋程序，使用C++和Qt框架，允许用户与程序对弈。

这个程序使用传统的alpha-beta剪枝算法来计算最佳行动，其搜索深度为4（大于4就会很耗时），使用启发式函数来估计棋局优势。

### 如何构建

如果你从未了解过Qt，那么获取Qt库的最简单的方法是使用[vcpkg]([microsoft/vcpkg: C++ Library Manager for Windows, Linux, and MacOS (github.com)](https://github.com/microsoft/vcpkg))。

1. 安装vcpkg

   选一个合适的位置（比如C:/dev/）打开终端执行以下命令：

   ```powershell
   git clone https://github.com/microsoft/vcpkg
   cd vcpkg
   ./bootstrap-vcpkg.bat
   ./vcpkg integrate install
   ```

   

2. 安装Qt库

   在你安装vcpkg的位置（比如C:/dev/vcpkg/）执行：

   ```powershell
   ./vcpkg install qt-base:x64-windows
   ```

   

3. 下载并构建项目

   克隆该仓库，用Visual Studio打开根目录，Visual Studio将会自动识别CMake项目并使用vcpkg。按下Ctrl+F5即可构建运行。

   也可以不使用Visual Studio，具体可参考[vcpkg的使用说明]([microsoft/vcpkg: C++ Library Manager for Windows, Linux, and MacOS (github.com)](https://github.com/microsoft/vcpkg))。