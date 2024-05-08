cmake_minimum_required(VERSION 3.10)
project(day11)

# server可执行文件所需的源文件列表
set(SERVER_SOURCES 
    src/util.cpp 
    src/Buffer.cpp 
    src/Socket.cpp 
    src/InetAddress.cpp 
    src/Epoll.cpp 
    src/Connection.cpp 
    src/Channel.cpp 
    src/EventLoop.cpp 
    src/Server.cpp 
    src/Acceptor.cpp 
    src/ThreadPool.cpp
    src/ThreadPool.h
    server.cpp
)

# client可执行文件所需的源文件列表
set(CLIENT_SOURCES 
    src/util.cpp 
    src/Buffer.cpp 
    src/Socket.cpp 
    src/InetAddress.cpp 
    client.cpp
)

# ThreadPoolTest可执行文件所需的源文件列表
set(THREADPOOLTEST_SOURCES 
    src/ThreadPool.cpp 
    ThreadPoolTest.cpp
)
set(TEST_SOURCES 
    src/util.cpp 
    src/Buffer.cpp 
    src/Socket.cpp 
    src/InetAddress.cpp 
    src/ThreadPool.cpp
    test.cpp
)
# 创建可执行文件 server
add_executable(server ${SERVER_SOURCES})
target_link_libraries(server pthread)
# 创建可执行文件 client
add_executable(client ${CLIENT_SOURCES})

# 创建可执行文件 ThreadPoolTest，并链接 pthread 库
add_executable(ThreadPoolTest ${THREADPOOLTEST_SOURCES})
target_link_libraries(ThreadPoolTest pthread)
# 创建可执行文件 test，并链接 pthread 库
add_executable(test ${TEST_SOURCES})
target_link_libraries(test pthread)