#include"Socket.h"
#include<fcntl.h>
#include<sys/socket.h>
#include<unistd.h>
#include<cerrno>
#include<cstdio>
#include<cstring>
#include <iostream>
#include"util.h"
InetAddress::InetAddress()=default;
InetAddress::InetAddress(const char *ip,uint16_t port)
{
    memset(&addr_,0,sizeof(addr_));
    addr_.sin_family=AF_INET;
    addr_.sin_addr.s_addr=inet_addr(ip);
    addr_.sin_port=htons(port);
}
void InetAddress::SetAddr(sockaddr_in addr){addr_=addr;}
sockaddr_in InetAddress::GetAddr(){return addr_;}
const char *InetAddress::GetIp(){return inet_ntoa(addr_.sin_addr);}
uint16_t InetAddress::GetPort(){return ntohs(addr_.sin_port);}
Socket::Socket(){
    fd_=socket(AF_INET,SOCK_STREAM,0);
    errif(fd_==-1,"socket 创建失败");
}
Socket::Socket(int fd):fd_(fd){
    errif(fd_==-1,"socket 创建失败");
}
Socket::~Socket()
{
    if (fd_!=-1)
    {
        close(fd_);
        fd_=-1;
    }
    
}
void Socket::Bind(InetAddress*addr)
{
    struct sockaddr_in tmp_addr=addr->GetAddr();
    errif(bind(fd_,(sockaddr*)&tmp_addr,sizeof(tmp_addr))==-1,"socket绑定失败");
}
void Socket::Listen()
{
    errif(listen(fd_,SOMAXCONN)==-1,"socket监听失败");
}
void Socket::SetNonBlocking()
{
    fcntl(fd_,F_SETFL,fcntl(fd_,F_GETFL)|O_NONBLOCK);
}
bool Socket::IsNonBlocking()
{
    return fcntl(fd_,F_GETFL)&O_NONBLOCK!=0;
}
int Socket::GetFd()
{
    return fd_;
}
int Socket::Accept(InetAddress*addr)
{
    int clnt_sockfd=-1;
    struct sockaddr_in tmp_addr{};
    socklen_t addr_len=sizeof(tmp_addr);
    if (IsNonBlocking())
    {
        while (true)
        {
            clnt_sockfd=accept(fd_,(sockaddr*)&tmp_addr,&addr_len);
            if (clnt_sockfd==-1&&((errno==EAGAIN)||errno==EWOULDBLOCK))
            {
                std::cout<<"没有连接目前"<<std::endl;
                continue;
            }
            if (clnt_sockfd==-1)
            {
                errif(true,"socket接收信息失败");

            }else
            break;
            
            
        }
        
    }else{
        clnt_sockfd =accept(fd_,(sockaddr*)&tmp_addr,&addr_len);
        errif(clnt_sockfd==-1,"socket接收信息失败");
    }
    addr->SetAddr(tmp_addr);
    return clnt_sockfd;
    
}
void Socket::Connect(InetAddress *addr) {
  // for client socket
  struct sockaddr_in tmp_addr = addr->GetAddr();
  if (fcntl(fd_, F_GETFL) & O_NONBLOCK) {
    while (true) {
      int ret = connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr));
      if (ret == 0) {
        break;
      }
      if (ret == -1 && (errno == EINPROGRESS)) {
        continue;
        /* 连接非阻塞式sockfd建议的做法：
          这里为了简单、不断连接直到连接完成，相当于阻塞式
        */
      }
      if (ret == -1) {
        errif(true, "socket connect error");
      }
    }
  } else {
    errif(connect(fd_, (sockaddr *)&tmp_addr, sizeof(tmp_addr)) == -1, "socket connect error");
  }
}
void Socket::Connect(const char *ip, uint16_t port) {
  InetAddress *addr = new InetAddress(ip, port);
  Connect(addr);
  delete addr;
}