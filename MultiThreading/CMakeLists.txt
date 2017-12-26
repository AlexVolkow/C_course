#set(CMAKE_CXX_COMPILER "clang++")

cmake_minimum_required(VERSION 3.3.2)

add_subdirectory(3dparty/googletest-1.8.0)
add_subdirectory(1_thread_pool_task)
add_subdirectory(2_promise_future)
add_subdirectory(3_flatten)
add_subdirectory(4_map)
add_subdirectory(5_parallel)