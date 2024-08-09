#pragma once

#include <functional>
class Task
{
public:
    template <typename Func, typename... Args>
    explicit Task(Func&& func, Args&&... args)
        : _func([func = std::forward<Func>(func), args...](void) { func(args...); })
    {
    }

    void execute() { _func(); }

private:
    std::function<void()> _func;
};
