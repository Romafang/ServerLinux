#include"Socket.h"
#include"util.h"
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<iostream>
using std::cout;
using std::endl;
Socket::Socket():fd(-1)
{
    fd=socket(AF_INET,SOCK_STREAM,0);
    errif(fd==-1,"socket创建失败");
}
Socket::Socket(int _fd):fd(_fd)
{
    errif(fd==-1,"socket创建失败");
}
Socket::~Socket()
{
    if(fd!=-1)
    {
        close(fd);
        fd=-1;
    }
}
//先完善InetAddress类
InetAddress::InetAddress()
{
    bzero(&addr,sizeof(addr));
}
InetAddress::InetAddress(const char *_ip,uint16_t _port)
{
    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(_ip);
    addr.sin_port=htons(_port);
}
InetAddress::~InetAddress()
{}
void InetAddress::setInetAddress(sockaddr_in _addr)
{
    addr=_addr;
}
sockaddr_in InetAddress::getAddr()
{
    return addr;
}
char* InetAddress::getIp()
{
    return inet_ntoa(addr.sin_addr);
}
uint16_t InetAddress::getPort()
{
    return ntohs(addr.sin_port);
}
void Socket::bind(InetAddress* _addr)
{
    struct sockaddr_in addr=_addr->getAddr();
    errif(::bind(fd,(sockaddr*)&addr,sizeof(addr))==-1,"socket绑定失败");
}
void Socket::listen()
{
    errif(::listen(fd,SOMAXCONN)==-1,"socket监听失败");
}
void Socket::setnonblocking()
{
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL)|O_NONBLOCK);
}
int Socket::getFd()
{
    return fd;
}
int Socket::accept(InetAddress*_addr)
{
    int clnt_sockfd=-1;//创建一个客户端socket接收信息
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    socklen_t addr_len=sizeof(addr);
    if(fcntl(fd,F_GETFL)&O_NONBLOCK)//判断是否是无阻塞模式
    {
        while (true)
        {
            clnt_sockfd=::accept(fd,(sockaddr*)&addr,&addr_len);//当前socket接收到的信息
            if (clnt_sockfd==-1&&((errno==EAGAIN)||(errno==EWOULDBLOCK)))
            {
                cout<<"目前没有连接"<<endl;
                continue;
            }
            else if(clnt_sockfd==-1)
            {
                errif(true,"socket接收错误");
            }
            else
            {
                break;
            }
            
        }
        
    }
    else{//阻塞模式
    clnt_sockfd=::accept(fd,(sockaddr*)&addr,&addr_len);
    errif(clnt_sockfd==-1,"socket接收错误");
    }
    _addr->setInetAddress(addr);
    return clnt_sockfd;
}
void Socket::connect(InetAddress*_addr)
{
    struct sockaddr_in addr=_addr->getAddr();
    if (fcntl(fd,F_GETFL)&O_NONBLOCK)//非阻塞式
    {
        while (true)
        {
            int ret=::connect(fd,(sockaddr*)&addr,sizeof(addr));
            if(ret==0)
            {
                break;
            }
            else if(ret==-1&&(errno==EINPROGRESS)){
                continue;//不断连接直至完成，相当阻塞式
            }
            else if (ret==-1)
            {
                errif(true,"socket连接失败");
            }
            
        }
        
    }
    else{
        errif(::connect(fd,(sockaddr*)&addr,sizeof(addr))==-1,"socket连接失败");
    }
    
}