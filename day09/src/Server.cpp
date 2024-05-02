#include"Server.h"
#include"util.h"
#include"Acceptor.h"
#include"Connection.h"
#include"Channel.h"
#include"Socket.h"
#include"EventLoop.h"
#include<unistd.h>
#include<string.h>
#include<functional>
Server::Server(EventLoop*_loop):loop(_loop),acceptor(nullptr)
{
    acceptor=new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setConnectionCallback(cb);
}

Server::~Server()
{
    delete acceptor;
}
void Server::newConnection(Socket*sock)
{
    Connection* cnn=new Connection(loop,sock);
    std::function<void(Socket*)>cb=std::bind(&Server::deleteConnection,this,std::placeholders::_1);
    cnn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()]=cnn;
}
void Server::deleteConnection(Socket*sock)
{
    Connection*conn=connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}