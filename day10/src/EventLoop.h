#pragma once
#include<functional>
class Epoll;
class Channel;
class ThreadPool;
class EventLoop
{
private:
    Epoll *ep;
    bool quit;
    ThreadPool *threadpool;
public:
    EventLoop(/* args */);
    ~EventLoop();
    void loop();
    void updateChannel(Channel*);
    void addThread(std::function<void()>);
};

