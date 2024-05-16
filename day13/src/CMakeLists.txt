set(SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
include_directories(${SRC_INCLUDE_DIR})

# 递归查找所有.cpp文件
file(GLOB_RECURSE day13_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 打印找到的源文件
message(STATUS "day13_sources: ${day13_sources}")

# 设置共享库连接选项
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fPIC -pthread")
# 添加了-fPIC和-pthread选项，-fPIC是位置无关代码（Position-Independent Code）的意思，
# 用于创建位置无关的代码，这样代码就可以在共享库和其他程序中被加载而无需重定位；-pthread是告诉链接器程序使用pthread库。
# 创建共享库
add_library(day13_shared SHARED ${day13_sources})