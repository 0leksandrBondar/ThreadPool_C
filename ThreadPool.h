#pragma once

#include "Singleton.h"
#include "ThreadWrapper.h"

#include <future>

class ThreadPool : public Singleton<ThreadPool>
{
public:
    using Thread = ThreadWrapper;

public:
    explicit ThreadPool();

    void runTasks();
    void pushTask(const Task& task);

    void createThread(const std::string& name);

private:
    void createThreads();

private:
    std::mutex _mtx;
    std::queue<Task> _tasks;
    std::vector<Thread> _threads;

    size_t _createdThreads{ 0 };
    const size_t _maxThreads{ std::thread::hardware_concurrency() };
};
