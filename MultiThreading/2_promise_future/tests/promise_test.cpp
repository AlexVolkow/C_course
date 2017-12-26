//
// Created by eugene on 28.9.2017
//

#include <thread>
#include <vector>
#include <gtest/gtest.h>
#include <stack>
#include "../include/promise.h"

TEST(promise, is_ready) {
    Promise<int> promise;
    Future<int> f = promise.get_future();


    ASSERT_FALSE(f.is_ready());

    int const x = 10;
    promise.set_value(x);
    ASSERT_TRUE(f.is_ready());
}

TEST(promise, Set_int) {
    Promise<int> promise;
    Future<int> f = promise.get_future();

    int const x = 10;
    promise.set_value(x);
    ASSERT_EQ(f.get(), x);
}

TEST(promise, Set_int_ampersand) {
    Promise<int &> promise;
    Future<int &> f = promise.get_future();

    int const y = 100;
    int x = 10;

    promise.set_value(x);

    f.get() = y;

    ASSERT_EQ(x, y);
}

TEST(promise, Set_void) {
    Promise<void> promise;
    Future<void> f = promise.get_future();

    ASSERT_FALSE(f.is_ready());

    promise.set_value();

    f.get();

    ASSERT_TRUE(f.is_ready());
}

TEST(promise, two_threads_get_set) {
    Promise<int> promise;
    Future<int> future = promise.get_future();

    int x = 777;

    std::thread t(
        [x] (Promise<int> promise) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            promise.set_value(x);
        }, std::move(promise)
    );

    for (int i = 0; !future.is_ready(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ASSERT_EQ(future.get(), x);

    t.join();
}

TEST(promise, two_threads_get_throw_exception) {
    Promise<int> promise;
    Future<int> future = promise.get_future();

    int x = 777;

    std::thread t(
        [x] (Promise<int> promise) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }, std::move(promise)
    );

    ASSERT_ANY_THROW(future.get());

    t.join();
}


