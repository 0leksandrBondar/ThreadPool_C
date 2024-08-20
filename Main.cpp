#include <chrono>
#include <iostream>

#include "Task.h"
#include "ThreadPool.h"

void AcceptClients(bool& requestStop, std::string text) {  std::cout << text << " started!" << '\n'; }
void HandleIncomingMessage(bool& requestStop, std::string text) {  std::cout << text << " started!" << '\n'; }
void CheckClientsOnDisconnect(bool& requestStop, std::string text) { std::cout << text << " started!" << '\n'; }

int main()
{
    Task task(AcceptClients, "AcceptClients");
    Task task2(CheckClientsOnDisconnect, "HandleIncomingMessage");
    Task task3(HandleIncomingMessage, "CheckClientsOnDisconnect");

    ThreadPool::GetInstance()->pushTask(task);
    ThreadPool::GetInstance()->pushTask(task2);
    ThreadPool::GetInstance()->pushTask(task3);
    ThreadPool::GetInstance()->runTasks();


    return 0;
}