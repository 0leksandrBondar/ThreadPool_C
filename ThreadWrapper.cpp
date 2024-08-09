#include "ThreadWrapper.h"

#include "Task.h"

ThreadWrapper::ThreadWrapper(InterThreadSync mode) : _mode(mode), _isThreadStopped(false)
{
    _thread = std::jthread(
        [this](std::stop_token stopToken)
        {
            std::unique_lock<std::mutex> lock(_mtx);

            while (!_isThreadStopped && !stopToken.stop_requested())
            {
                _cv.wait(lock,
                         [this, &stopToken] {
                             return !_queue.empty() || _isThreadStopped
                                    || stopToken.stop_requested();
                         });

                if (!_queue.empty())
                {
                    auto task = std::move(_queue.front());
                    _queue.pop();

                    lock.unlock();
                    task.execute();
                    lock.lock();
                }
            }
        });
}

ThreadWrapper::ThreadWrapper(const Task& task, InterThreadSync mode) : ThreadWrapper(mode)
{
    giveTask(task);
}

void ThreadWrapper::run()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _cv.notify_one();
}

void ThreadWrapper::stop()
{
    requestStop();
    if (_thread.joinable())
        _thread.join();
}

void ThreadWrapper::giveTask(const Task& task)
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
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _isThreadStopped = true;
    }
    _cv.notify_all();
}
