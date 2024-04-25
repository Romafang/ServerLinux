#pragma once
#include<sys/epoll.h>
#include<vector>
class Epoll
{
private:
    /* data */
    int epfd;
    struct epoll_event *events;
public:
    Epoll(/* args */);
    ~Epoll();
    void addFd(int fd,uint32_t op);
    std::vector<epoll_event>poll(int tiemout=-1);
};
