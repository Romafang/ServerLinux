cmake_minimum_required(VERSION 3.10)
project(day09)

add_executable(server server.cpp src/util.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Buffer.cpp src/Connection.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp )
add_executable(client src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp client.cpp)