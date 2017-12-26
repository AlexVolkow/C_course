cmake_minimum_required(VERSION 3.6)
project(MultiThreading)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES threadpool/test_threads.cpp threadpool/threadpool.h threadpool/threadpool.cpp async/shared_state.h async/future.h async/promise.h async/test_async.cpp async/utils.h)
add_executable(MultiThreading ${SOURCE_FILES})
add_definitions(-D_GTEST)
add_executable(tests/hw2/source/main.cpp)