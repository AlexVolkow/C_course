//
// Created by disqustingman on 11.10.17.
//
#include <gtest/gtest.h>
#include <thread>
#include <promise.h>
#include <map.h>
#include <math.h>

TEST(map, simple_test) {
    ThreadPool threadPool(2);

    threadPool.execute([] {
        Promise<int> promise;
        Future<int> future = promise.get_future();

        Future<double> f = map(future, [](int x) {
            return x * 3.14;
        });

        promise.set_value(2);

        double doublePi = f.get();
        double res = 2 * 3.14;

        ASSERT_EQ(doublePi, res);
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

TEST(map, exception) {
    ThreadPool threadPool(2);

    threadPool.execute([] {
        Promise<int> promise;
        Future<int> future = promise.get_future();

        Future<double> f = map(future, [](int x) -> double {
            throw std::logic_error("test");
            return x * 1.0;
        });

        promise.set_value(2);

        ASSERT_ANY_THROW(f.get());
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
}
