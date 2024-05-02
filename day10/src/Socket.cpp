#include"Socket.h"
#include"InetAddress.h"
#include<sys/socket.h>
#include"util.h"
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
Socket::Socket():fd(-1)
{
    fd=socket(AF_INET,SOCK_STREAM,0);
    errif(fd==-1,"socket创建失败");
}
Socket::Socket(int _fd):fd(_fd)
{
    errif(fd==-1,"socket创建失败");
}
Socket::~Socket(){
    if(fd!=-1)
    {
        close(fd);
        fd=-1;
    }
}
void Socket::bind(InetAddress *_addr)
{
    struct sockaddr_in addr=_addr->getAddr();
    socklen_t addr_len=_addr->getLen();
    errif(::bind(fd,(sockaddr*)&addr,addr_len)==-1,"绑定socket套接字的ip地址失败");
    _addr->setInetAddr(addr,addr_len);
}
void Socket::listen()
{
    errif(::listen(fd,SOMAXCONN)==-1,"socket监听失败");
}
void Socket::setnonblocking()
{
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL)|O_NONBLOCK);
}
int Socket::accept(InetAddress*_addr)
{
    struct sockaddr_in addr;
    socklen_t addr_len=sizeof(addr);
    bzero(&addr,addr_len);
    int clnt_addr=::accept(fd,(sockaddr*)&addr,&addr_len);
    errif(clnt_addr==-1,"socket接收信息失败");
    _addr->setInetAddr(addr,addr_len);
    return clnt_addr;
}
int Socket::getFd()
{
    return fd;
}
void Socket::connect(InetAddress* _addr){
    struct sockaddr_in addr=_addr->getAddr();
    socklen_t len=_addr->getLen();
    errif(::connect(fd,(sockaddr*)&addr,len)==-1,"连接套接字失败");

}