#pragma once 
#include<functional>
#include<vector>
#include<queue>
#include<unistd.h>
#include<thread>
#include<mutex>
#include<condition_variable>
class ThreadPool
{
private:
    std::vector<std::thread>threads;
    std::queue<std::function<void()>>tasks;
    std::mutex task_mx;
    std::condition_variable cv;
    bool stop;

public:
    ThreadPool(int size=10);
    ~ThreadPool();
    void add(std::function<void()>);
};
