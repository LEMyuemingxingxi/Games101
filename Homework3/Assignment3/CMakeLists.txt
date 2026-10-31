cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(Rasterizer
    main.cpp
    rasterizer.cpp
    Triangle.cpp
    Texture.cpp
)

# 原始代码使用 <eigen3/Eigen/Eigen>，因此加入依赖的 include 目录。
target_include_directories(Rasterizer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../.deps/install/include"
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(Rasterizer PRIVATE
    Eigen3::Eigen
    ${OpenCV_LIBRARIES}
)