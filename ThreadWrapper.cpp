#include "ThreadWrapper.h"
#include "Task.h"

#include <iostream>
#include <optional>

ThreadWrapper::ThreadWrapper(InterThreadSync mode) : _mode(mode)
{
    _thread = std::jthread(&ThreadWrapper::runTasks, this);
}

ThreadWrapper::ThreadWrapper(const Task& task, InterThreadSync mode) : ThreadWrapper(mode)
{
    giveTask(task);
    _thread = std::jthread(&ThreadWrapper::runTasks, this);
}

void ThreadWrapper::giveTask(const Task& task) { _task = task; }

void ThreadWrapper::stop() { _requestStop = true; }

ThreadWrapper::ThreadWrapper(ThreadWrapper&& other) noexcept
    : _name{ std::move(other._name) }, _thread{ std::move(other._thread) }, _mode{ other._mode }
{
    other._mode = InterThreadSync::Join;
}

void ThreadWrapper::runTasks()
{
    while (true)
    {
        if (_task.has_value())
        {
            _task->execute(_requestStop);
            _task.reset();
            break;
        }
    }
}

ThreadWrapper::~ThreadWrapper()
{
    if (_thread.joinable())
        _thread.join();
}
