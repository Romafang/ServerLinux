#include"Acceptor.h"
#include"Channel.h"
#include"EventLoop.h"
#include<iostream>
#include"Socket.h"
#include"util.h"
#include<unistd.h>
#include"InetAddress.h"
Acceptor::Acceptor(EventLoop* _loop):loop(_loop),sock(nullptr),acceptchannel(nullptr)
{
    sock=new Socket();
    InetAddress *addr=new InetAddress("127.0.0.1",1234);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    acceptchannel =new Channel(loop,sock->getFd());
    std::function<void()>cb=std::bind(&Acceptor::acceptConnection,this);
    acceptchannel->setCallback(cb);
    acceptchannel->enableReading();
    delete addr;
}
Acceptor::~Acceptor()
{
    delete sock;
    delete acceptchannel;
}
void Acceptor::acceptConnection()
{
    InetAddress *clnt_addr=new InetAddress();
    Socket* clnt_sock=new Socket(sock->accept(clnt_addr));
    std::cout<<"新客户端的fd是: "<<clnt_sock->getFd()<<"其IP地址是: "<<inet_ntoa(clnt_addr->getAddr().sin_addr)<<"端口号为:"<<ntohs(clnt_addr->getAddr().sin_port)<<std::endl;
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}
void Acceptor::setConnectionCallback(std::function<void(Socket*)>_cb)
{
    newConnectionCallback=_cb;
}