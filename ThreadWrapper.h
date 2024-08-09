#pragma once

#include "Task.h"

#include <queue>
#include <functional>
#include <mutex>
#include <string>
#include <thread>

enum class InterThreadSync
{
    Join,
    Detach
};


class ThreadWrapper
{
public:
    using ThreadId = std::jthread::id;

public:
    explicit ThreadWrapper(InterThreadSync mode = InterThreadSync::Join);
    explicit ThreadWrapper(const Task& task, InterThreadSync mode = InterThreadSync::Join);

    ~ThreadWrapper();

    void run();
    void stop();
    void requestStop();

    void setName(const std::string& name) { _name = name; }
    void setInterThreadSyncMode(InterThreadSync mode) { _mode = mode; }

    void giveTask(const Task& task);

    [[nodiscard]] std::string getName() const { return _name; };
    [[nodiscard]] bool isStopped() const { return _isThreadStopped; }
    [[nodiscard]] ThreadId getThreadId() const { return _thread.get_id(); };

private:
    std::mutex _mtx;
    std::string _name;
    std::jthread _thread;
    std::queue<Task> _queue;
    bool _isThreadStopped{ false };
    std::condition_variable _cv;
    InterThreadSync _mode{ InterThreadSync::Join };
};
