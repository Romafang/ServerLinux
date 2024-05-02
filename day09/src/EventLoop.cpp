#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop(/* args */):ep(nullptr),quit(false)
{
    ep=new Epoll();
}

EventLoop::~EventLoop()
{
    delete ep;
}

void EventLoop::updateChannel(Channel*channel)
{
    ep->updateChannel(channel);
}
void EventLoop::loop()
{
    while (!quit)
    {
        std::vector<Channel*>chs=ep->poll();
        for(auto it=chs.begin();it!=chs.end();it++)
        {
            (*it)->handleEvent();
        }
    }
    
}