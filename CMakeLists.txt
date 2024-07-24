cmake_minimum_required(VERSION 3.16)
project(ThreadPool)

set(SOURCES Main.cpp ThreadPool.h ThreadPool.cpp ThreadWrapper.h ThreadWrapper.cpp Task.cpp Task.h)

add_executable(${PROJECT_NAME} ${SOURCES})