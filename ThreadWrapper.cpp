#include "ThreadWrapper.h"

#include "Task.h"

ThreadWrapper::ThreadWrapper(InterThreadSync mode)
    : _mode(mode), _isThreadStopped(false), _running(true)
{
    _thread = std::thread(&ThreadWrapper::threadFunc, this);
}

ThreadWrapper::ThreadWrapper(const std::shared_ptr<Task>& task, InterThreadSync mode)
    : ThreadWrapper(mode)
{
    giveTask(task);
}

void ThreadWrapper::run()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _running = true;
    _cv.notify_one();
}

void ThreadWrapper::stop()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _isThreadStopped = true;
    }
    _cv.notify_all();
    if (_thread.joinable())
        _thread.join();
}

void ThreadWrapper::giveTask(const std::shared_ptr<Task>& task)
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _queue.push(task);
    }
    _cv.notify_one();
}

ThreadWrapper::~ThreadWrapper() { stop(); }

void ThreadWrapper::requestStop()
{
    std::lock_guard<std::mutex> lock(_mtx);
    _isThreadStopped = true;
    _cv.notify_all();
}

void ThreadWrapper::threadFunc()
{
    std::unique_lock<std::mutex> lock(_mtx);
    while (!_isThreadStopped)
    {
        _cv.wait(lock, [this] { return !_queue.empty() || _isThreadStopped; });

        if (!_queue.empty())
        {
            auto task = std::move(_queue.front());
            _queue.pop();
            lock.unlock();

            task->execute();

            lock.lock();
        }
    }
}