#pragma once
#include<functional>
class Channel;
class EventLoop;
class Epoll;
class Socket;
class Acceptor
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *acceptchannel;
    std::function<void(Socket*)>newConnectionCallback;
public:
    Acceptor(EventLoop * _loop);
    ~Acceptor();
    void acceptConnection();
    void setConnectionCallback(std::function<void(Socket*)>);
};
