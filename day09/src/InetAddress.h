#pragma once
#include <arpa/inet.h>
#include<sys/socket.h>
class InetAddress
{
private:
    struct sockaddr_in addr;
    socklen_t len;
public:
    InetAddress(/* args */);

    ~InetAddress();
    InetAddress(const char* ip, uint16_t port);
    sockaddr_in getAddr();
    socklen_t getLen();
    void setInetAddr(sockaddr_in,socklen_t);
};
