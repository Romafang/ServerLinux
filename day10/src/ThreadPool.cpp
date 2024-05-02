#include"ThreadPool.h"

ThreadPool::ThreadPool(int size):stop(false)
{
    for (size_t i = 0; i < size; i++)
    {
        threads.emplace_back(std::thread([this](){
            while (true)
            {
                std::function<void()>task;
                {
                    std::unique_lock<std::mutex>lock(task_mx);
                    cv.wait(lock,[this](){//void wait (unique_lock<mutex>& lck, Predicate pred);第二个参数是一个判断条件，为true就不阻塞
                        return stop||!tasks.empty();
                    });
                    if(stop&&tasks.empty())return;
                    task=tasks.front();
                    tasks.pop();
                }
                task();
            }
            
        }));
    }
    
}

ThreadPool::~ThreadPool()
{
    {std::unique_lock<std::mutex>lock(task_mx);
    stop=true;}
    for(std::thread &th:threads)
    {   
        if(th.joinable())
        {
            th.join();//阻塞等待未完成进程结束
        }
    }
}
void ThreadPool::add(std::function<void()>func)
{
    {std::unique_lock<std::mutex>lock(task_mx);
    if(stop)
    {
        throw std::runtime_error("ThreadPool已经停止，不可以继续添加");
    }
    tasks.emplace(func);}
    cv.notify_one();
}