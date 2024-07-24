#pragma once

#include <functional>
class Task
{
public:
    explicit Task(std::function<void()> func) : _func(std::move(func)) {}

    void execute() { _func(); }

private:
    std::function<void()> _func;
};
