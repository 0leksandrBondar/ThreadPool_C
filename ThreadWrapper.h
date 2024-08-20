#pragma once

#include "Task.h"

#include <queue>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

class ThreadWrapper
{
public:
    using ThreadId = std::jthread::id;

    enum class InterThreadSync
    {
        Join,
        Detach
    };

public:
    explicit ThreadWrapper(InterThreadSync mode = InterThreadSync::Join);
    explicit ThreadWrapper(const Task& task, InterThreadSync mode = InterThreadSync::Join);

    ThreadWrapper(ThreadWrapper&& other) noexcept;

    ThreadWrapper(const ThreadWrapper&) = delete;
    ThreadWrapper& operator=(const ThreadWrapper&) = delete;

    virtual ~ThreadWrapper();

    void stop();

    void setName(const std::string& name) { _name = name; }
    void setInterThreadSyncMode(InterThreadSync mode) { _mode = mode; }

    void giveTask(const Task& task);

    [[nodiscard]] std::string getName() const { return _name; };
    [[nodiscard]] ThreadId getThreadId() const { return _thread.get_id(); };
    [[nodiscard]] bool isAvailable() const { return !_task.has_value(); }

private:
    void runTasks();

private:
    std::string _name;
    std::jthread _thread;

    std::optional<Task> _task{ std::nullopt };

    bool _requestStop{ false };

    InterThreadSync _mode{ InterThreadSync::Join };
};
