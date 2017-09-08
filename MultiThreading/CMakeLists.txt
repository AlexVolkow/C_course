cmake_minimum_required(VERSION 3.6)
project(MultiThreading)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES test.cpp worker.cpp worker.h threadpool.h threadpool.cpp)
add_executable(MultiThreading ${SOURCE_FILES})