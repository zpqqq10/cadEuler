cmake_minimum_required(VERSION 3.25)
project(cadEuler)
set(TARGET_NAME main)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_DEBUG " $ENV{CXXFLAGS} -O0 -Wall -g -ggdb ")
set(CMAKE_CXX_FLAGS_RELEASE " $ENV{CXXFLAGS} -O3 -Wall ")

# 添加include文件夹路径
include_directories(include)

# 添加源文件
file(GLOB SOURCES src/euler.cpp src/painter.cpp src/GUI.cpp src/main.cpp)

# 添加头文件
file(GLOB HEADERS include/ds.hpp include/euler.hpp include/painter.hpp include/GUI.hpp)

# 添加可执行文件
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

# 添加链接库
target_link_libraries(${TARGET_NAME} PRIVATE GL GLU glut GLEW)