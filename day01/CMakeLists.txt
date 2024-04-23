# CMakeList.txt: day01 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("day01")

# Add server executable
add_executable(server ./server.cpp)

# Add client executable
add_executable(client ./client.cpp)
