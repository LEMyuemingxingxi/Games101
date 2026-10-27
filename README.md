# GAMES101 Windows 开发环境

当前作业：`Homework1/Assignment1`。使用本机 w64devkit GCC 16.2、CMake、Ninja、GDB，C++17；依赖为 Eigen 3.3.7 和 OpenCV 4.10.0。

依赖安装在工作区 `.deps/install`，OpenCV 使用同一套 GCC 从源码编译为静态库，支持 Windows 原生窗口、PNG 和 JPEG。不要删除 `.deps/install`，否则需要重新安装依赖。`.deps` 和构建产物不提交到版本控制。

## 在 VS Code 中使用

用 VS Code 打开整个 `Games101` 文件夹：

- `Ctrl+Shift+B`：配置并编译整个作业。
- `F5`：编译并通过 GDB 启动调试，可以在 `main.cpp` 中下断点。
- “终端 → 运行任务 → GAMES101: run”：编译并运行窗口程序。
- 窗口获得焦点后，`A` / `D` 修改旋转角度，`Esc` 退出。旋转效果需要先实现作业中的模型矩阵。

请使用上述项目任务。编辑器右上角的“运行 C/C++ 文件”可能只编译当前文件，不会自动链接完整作业和依赖。

## 在 PowerShell 中使用

从工作区根目录执行：

```powershell
cd Homework1/Assignment1
cmake --preset windows-gcc
cmake --build --preset windows-gcc
./build/Rasterizer.exe
```

仅输出图片，无需打开窗口：

```powershell
./build/Rasterizer.exe -r 0 output.png
```

输出文件位于当前目录。`get_model_matrix` 和 `get_projection_matrix` 仍是原始作业 TODO；初始运行的图形和旋转效果不代表作业已完成。

## 配置说明

`CMakePresets.json` 指定 Ninja、Debug 模式和本地依赖路径。CMake 自动生成 `build/compile_commands.json`，供 C++ 扩展解析头文件和编译选项。Eigen 头文件使用跨平台的 `<Eigen/Eigen>` 写法，CMake 通过 `Eigen3::Eigen` 提供搜索路径。

`.vscode/launch.json` 的 GDB 路径对应当前电脑的 w64devkit 安装位置；迁移到其他电脑时需要调整，并重新安装或编译依赖。后续作业可沿用 `.deps/install`，在对应 CMake 项目中设置 `CMAKE_PREFIX_PATH` 并链接 Eigen 与所需 OpenCV 模块。
