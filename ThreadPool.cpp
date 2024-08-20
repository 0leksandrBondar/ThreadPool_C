#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool() { createThreads(); }

void ThreadPool::createThreads()
{
    _threads.resize(_maxThreads);
    _createdThreads = _threads.size();
}

void ThreadPool::pushTask(const Task& task)
{
    std::unique_lock lock(_mtx);
    _tasks.push(task);
}

void ThreadPool::runTasks()
{
    std::unique_lock lock(_mtx);

    while(true)
    {
        for (auto & _thread : _threads)
        {
            if (_thread.isAvailable() && !_tasks.empty())
            {
                _thread.giveTask(_tasks.front());
                _tasks.pop();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }
}
