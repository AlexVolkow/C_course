cmake_minimum_required(VERSION 3.6)
project(MultiThreading)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES threadpool/test_threads.cpp threadpool/threadpool.h threadpool/threadpool.cpp async/sharedstate.h async/future.h async/promise.h async/test_async.cpp async/utils.h)
add_executable(MultiThreading ${SOURCE_FILES})