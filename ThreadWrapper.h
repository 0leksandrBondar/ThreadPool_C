#pragma once

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

class Task;

class ThreadWrapper
{
public:
    using ThreadId = std::thread::id;

public:
    explicit ThreadWrapper(InterThreadSync mode = InterThreadSync::Join);
    explicit ThreadWrapper(const std::shared_ptr<Task>& task,
                           InterThreadSync mode = InterThreadSync::Join);

    ~ThreadWrapper();

    void run();
    void stop();
    void requestStop();

    void setName(const std::string& name) { _name = name; }
    void setInterThreadSyncMode(InterThreadSync mode) { _mode = mode; }

    void giveTask(const std::shared_ptr<Task>& task);

    [[nodiscard]] bool isAlive() const;
    [[nodiscard]] bool stopable() const;
    [[nodiscard]] bool startable() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] ThreadId getThreadId() const;

private:
    void threadFunc();

    std::mutex _mtx;
    std::string _name;
    std::thread _thread;
    bool _isThreadStopped{ false };
    std::queue<std::shared_ptr<Task>> _queue;
    InterThreadSync _mode{ InterThreadSync::Join };
    std::condition_variable _cv;
    bool _running{ false };
};