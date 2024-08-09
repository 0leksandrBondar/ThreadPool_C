#pragma once

#include "ThreadWrapper.h"

class ThreadPool
{
public:
    using Thread = ThreadWrapper;

public:
    explicit ThreadPool(int threadCount);

private:
    std::vector<Thread> _threads;
};
