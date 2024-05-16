#include"Server.h"
#include"Socket.h"
#include"EventLoop.h"
#include"Connection.h"
#include"ThreadPool.h"
#include"Acceptor.h"
#include<unistd.h>
#include<functional>
using std::map;
using std::vector;
using std::function;
Server::Server(EventLoop*_loop): mainReactor(_loop), acceptor(nullptr)
{
    acceptor=new Acceptor(mainReactor);
    function<void(Socket*)>cb=std::bind(&Server::newConnection,this,std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
    int size=std::thread::hardware_concurrency();
    thpool=new ThreadPool(size);
    for (size_t i = 0; i < size; i++)
    {
        subReactor.push_back(new EventLoop());
    }
    for (size_t i = 0; i < size; i++)
    {
        function<void()>sub_loop=std::bind(&EventLoop::loop,subReactor[i]);
        thpool->add(sub_loop);
    }
    
    
}

Server::~Server()
{
    delete acceptor;
    delete thpool;
}
void Server::newConnection(Socket*sock)
{
    if (sock->getFd()!=-1)
    {
        int random=sock->getFd()%subReactor.size();
        Connection*conn=new Connection(subReactor[random],sock);
        function<void(int)>cb=std::bind(&Server::deleteConnection,this,std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[sock->getFd()]=conn;
    }
    
}
void Server::deleteConnection(int sockfd)
{
    if (sockfd!=-1)
    {
        auto it=connections.find(sockfd);
        if (it!=connections.end())
        {
            Connection*conn=connections[sockfd];
            connections.erase(sockfd);
            delete conn;
        }
        
    }
    
}