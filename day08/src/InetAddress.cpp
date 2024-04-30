#include"InetAddress.h"
#include<string.h>
#include<sys/socket.h>

InetAddress::InetAddress():len(sizeof(addr))
{
     bzero(&addr, sizeof(addr));
}

InetAddress::~InetAddress()
{
}
InetAddress::InetAddress(const char* ip, uint16_t port):len(sizeof(addr))
{
    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(ip);
    addr.sin_port=htons(port);
    len=sizeof(addr);
}
sockaddr_in InetAddress::getAddr()
{
    return addr;
}
socklen_t InetAddress::getLen()
{
    return len;
}
void InetAddress::setInetAddr(sockaddr_in _addr,socklen_t _len)
{
    addr=_addr;
    len=_len;
}