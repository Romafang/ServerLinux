#pragma once
#include<map>
class Socket;
class EventLoop;
class Channel;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int,Connection *>connections;
public:
    Server(EventLoop*_loop);
    ~Server();
    void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
};


