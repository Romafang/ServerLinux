#pragma once
#include<condition_variable>
#include<functional>
#include<future>
#include<memory>
#include<mutex>
#include<queue>
#include<vector>
#include<thread>
#include<utility>
#include"Macros.h"
class ThreadPool
{
private:
    std::vector<std::thread>workers_;
    std::queue<std::function<void()>>tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_variable_;
    bool stop_{false};
public:
    ThreadPool(unsigned int size=std::thread::hardware_concurrency());
    ~ThreadPool();
    DISALLOW_COPY_AND_MOVE(ThreadPool);
    template <class F, class... Args>
    auto Add(F&& f, Args&&... args) -> std::future<decltype(f(args...))> ;
};

template <class F, class... Args>
auto ThreadPool::Add(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
    // 创建一个packaged_task对象，使用std::bind将函数和参数绑定
    auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    // 获取packaged_task的future对象，用于获取异步任务的结果
    std::future<decltype(f(args...))> result_future = task->get_future();

    // 将任务添加到队列中
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);

        // 如果线程池已经停止，则抛出异常
        if (stop_) {
            throw std::runtime_error("线程池已经停止");
        }

        // 将任务添加到任务队列中
        tasks_.emplace([task]() { (*task)(); });
    }

    // 通知一个等待中的线程
    condition_variable_.notify_one();

    // 返回future对象，以便调用者获取异步任务的结果
    return result_future;
}