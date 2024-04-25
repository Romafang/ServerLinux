#pragma once
#include<arpa/inet.h>
class InetAddress
{
private:
    /* data */
public:
    InetAddress(/* args */);
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();
};
