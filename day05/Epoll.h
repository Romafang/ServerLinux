#pragma once
#include<sys/epoll.h>
#include<vector>
using std::vector;
class Channel;
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
    void updateChannel(Channel*channel);
    vector<Channel*>poll(int tiemout=-1);
};
