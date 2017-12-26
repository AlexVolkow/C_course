//
// Created by disqustingman on 11.10.17.
//
#include <gtest/gtest.h>
#include <thread>
#include <math.h>
#include <thread_pool.h>

TEST(parallel, simple_test) {
    ThreadPool threadPool(2);

    std::vector<int> v;
    v.push_back(451);
    v.push_back(69);
    v.push_back(1984);
    v.push_back(42);
    v.push_back(13);
    v.push_back(1024);

    std::hash<std::thread::id> h;
    for (int i = 0; i < 4; i++) {
        threadPool.execute([=] { printf("Some task %d (id = %d)\n", i, h(std::this_thread::get_id())); });
    }

    threadPool.parallel(v.begin(), v.end(), [&](int x) {
        printf("Magic number %d(id = %d)\n", x * 10, h(std::this_thread::get_id()));
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
}
