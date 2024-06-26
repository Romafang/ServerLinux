#include"Acceptor.h"
#include"Socket.h"
#include"Channel.h"
#include<iostream>
using std::cout;
using std::endl;
using std::function;
Acceptor::Acceptor(EventLoop*_loop):
loop(_loop),sock(nullptr),acceptChannel(nullptr)
{
    sock=new Socket();
    InetAddress *addr=new InetAddress("127.0.0.1",1234);
    sock->bind(addr);
    sock->listen();
    acceptChannel=new Channel(loop,sock->getFd());
    function<void()>cb=std::bind(&Acceptor::acceptConnection,this);
    acceptChannel->setReadCallback(cb);
    acceptChannel->enableRead();
    delete addr;
}
Acceptor::~Acceptor()
{
    delete sock;
    delete acceptChannel;
}
void Acceptor::acceptConnection()
{
    InetAddress*clnt_addr=new InetAddress();
    Socket *clnt_sock=new Socket(sock->accept(clnt_addr));
    cout<<"新客户端fd是"<<clnt_sock->getFd()<<"IP:"<<clnt_addr->getIp()<<"Port:"<<clnt_addr->getPort()<<endl;
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}
void Acceptor::setNewConnectionCallback(function<void(Socket*)>_cb)
{
    newConnectionCallback=_cb;
}