#pragma once

#include <functional>
class Task
{
public:
    enum class State
    {
        Running,
        Waiting,
        Completed
    };

public:
    Task() = default;
    template <typename Func, typename... Args>
    explicit Task(Func&& func, Args&&... args)
        : _func(
              [this, func = std::forward<Func>(func),
               ... arguments = std::forward<Args>(args...)](bool& stopToken)
              {
                  _state = State::Running;

                  if constexpr (std::is_invocable_v<Func, bool&, Args...>)
                      func(stopToken, arguments...);
                  else
                      func(arguments...);

                  _state = State::Completed;
              })
    {
    }

    void execute(bool& stopToken) { _func(stopToken); }

private:
    State _state = State::Waiting;

    std::function<void(bool&)> _func = nullptr;
};
