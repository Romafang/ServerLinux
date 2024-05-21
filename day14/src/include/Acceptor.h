#pragma once
#include"Macros.h"
#include<functional>
class EventLoop;
class Socket;
class Channel;
class Acceptor
{
private:
    EventLoop *loop_;
    Socket*sock_;
    Channel*channel_;
    std::function<void(Socket*)>new_connection_callback_;
public:
    explicit Acceptor(EventLoop* loop);
    ~Acceptor();
    DISALLOW_COPY_AND_MOVE(Acceptor);
    void AcceptConnection();
    void SetNewConnectionCallback(std::function<void(Socket*)>const &callback);
};

