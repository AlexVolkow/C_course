//
// Created by eugene on 14.9.2017
//
#include <atomic>

#include <gtest/gtest.h>
#include "../include/thread_pool.h"

TEST(thread_pool, one_thread_one_task) {
    std::atomic<int> counter(0);

    {
        ThreadPool pool(1);

        pool.execute([&counter]{ counter = 1; });
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    ASSERT_EQ(counter, 1);
}

TEST(thread_pool, ten_threads) {
    std::atomic<int> counter(0);

    int32_t const cnt_threads = 10;
    {
        ThreadPool pool(cnt_threads);

        for (int32_t i = 0; i < cnt_threads; ++i) {
            pool.execute([&counter]{ counter += 1; });
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    ASSERT_EQ(counter, cnt_threads);
}

TEST(thread_pool, one_thread_several_tasks) {
    std::atomic<int> counter(0);

    int32_t const cnt_threads = 1;
    int32_t const cnt_tasks = 1;
    {
        ThreadPool pool(cnt_threads);

        for (int32_t i = 0; i < cnt_tasks; ++i) {
            pool.execute([&counter]{ counter += 1; });
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    ASSERT_EQ(counter, cnt_tasks);
}
