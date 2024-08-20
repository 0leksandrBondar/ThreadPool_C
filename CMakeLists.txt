cmake_minimum_required(VERSION 3.16)

project(ThreadPool)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SOURCES
        Main.cpp
        ThreadPool.h
        ThreadPool.cpp
        ThreadWrapper.h
        ThreadWrapper.cpp
        Task.cpp
        Task.h
        Singleton.cpp
        Singleton.h
)

add_executable(${PROJECT_NAME} ${SOURCES})