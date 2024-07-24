#include <chrono>
#include <iostream>
#include <memory>

#include "ThreadWrapper.h"
#include "Task.h"

void foo(std::string text)
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << text << '\n';
        std::this_thread::sleep_for(std::chrono::microseconds(550));
    }
}

int main()
{
    ThreadWrapper thread;

    thread.giveTask(std::make_shared<Task>([]() { foo("First Task"); }));

    thread.giveTask(std::make_shared<Task>(
        []()
        {
            for (int i = 0; i < 10; ++i)
            {
                std::cout << "Second Task\n";
                std::this_thread::sleep_for(std::chrono::microseconds(550));
            }
        }));

    thread.run();

    while (true)
    {
        std::cout << "MAIN TASK\n";
        std::this_thread::sleep_for(std::chrono::microseconds(550));
    }

    return 0;
}