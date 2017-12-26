//
// Created by disqustingman on 11.10.17.
//
#include <gtest/gtest.h>
#include <thread>
#include "../../2_promise_future/include/future.h"
#include "../../2_promise_future/include/promise.h"
#include "../include/flatten.h"

TEST(flatten, one_thread) {
    Promise<Future<Future<int>>> promise;
    Future<Future<Future<int>>> fut = promise.get_future();

    Promise<Future<int>> simple;

    Future<Future<int>> fut_simple = simple.get_future();
    promise.set_value(std::move(fut_simple));

    Promise<int> simple2;

    Future<int> fut_simple2 = simple2.get_future();
    simple.set_value(std::move(fut_simple2));

    Future<int> res = flatten(fut);
    simple2.set_value(42);

    int v = res.get();

    ASSERT_EQ(v, 42);
}

TEST(flatten, multithread) {
    Promise<Future<int>> promise;
    Future<Future<int>> fut = promise.get_future();

    Promise<int> simple;

    Future<int> fut_simple = simple.get_future();
    promise.set_value(std::move(fut_simple));

    std::thread t(
            [](Promise<int> simple) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                simple.set_value(42);
                simple.set_exception(std::make_exception_ptr(std::logic_error("test")));
            }, std::move(simple)
    );

    t.join();

    ASSERT_ANY_THROW(flatten(fut).get());
}

TEST(flatten, collection) {
    Promise<int> promise1;
    Promise<int> promise2;
    Promise<int> promise3;

    std::vector<Future<int>> vec;
    Future<int> fut1 = promise1.get_future();
    Future<int> fut2 = promise2.get_future();
    Future<int> fut3 = promise3.get_future();
    vec.push_back(std::move(fut1));
    vec.push_back(std::move(fut2));
    vec.push_back(std::move(fut3));

    promise1.set_value(42);
    promise2.set_value(69);
    promise3.set_value(451);

    Future<std::vector<int>> res = flatten(vec);
    std::vector<int> values = res.get();

    std::vector<int> temp;
    temp.push_back(42);
    temp.push_back(69);
    temp.push_back(451);

    ASSERT_EQ(values, temp);
}

TEST(flatten, dequeue) {
    Promise<int> promise1;
    Promise<int> promise2;
    Promise<int> promise3;

    std::deque<Future<int>> vec;
    Future<int> fut1 = promise1.get_future();
    Future<int> fut2 = promise2.get_future();
    Future<int> fut3 = promise3.get_future();
    vec.push_back(std::move(fut1));
    vec.push_back(std::move(fut2));
    vec.push_back(std::move(fut3));

    promise1.set_value(42);
    promise2.set_value(69);
    promise3.set_value(451);

    Future<std::deque<int>> res = flatten(vec);
    std::deque<int> values = res.get();

    std::deque<int> temp;
    temp.push_back(42);
    temp.push_back(69);
    temp.push_back(451);

    ASSERT_EQ(values, temp);
}

TEST(flatten, set) {
    Promise<int> promise1;
    Promise<int> promise2;
    Promise<int> promise3;

    std::set<Future<int>> vec;
    Future<int> fut1 = promise1.get_future();
    Future<int> fut2 = promise2.get_future();
    Future<int> fut3 = promise3.get_future();
    vec.insert(std::move(fut1));
    vec.insert(std::move(fut2));
    vec.insert(std::move(fut3));

    promise1.set_value(42);
    promise2.set_value(69);
    promise3.set_value(451);

    Future<std::set<int>> res = flatten(vec);
    std::set<int> values = res.get();

    std::set<int> temp;
    temp.insert(42);
    temp.insert(69);
    temp.insert(451);

    ASSERT_EQ(values, temp);
}

TEST(flatten, array) {
    Promise<int> promise1;
    Promise<int> promise2;
    Promise<int> promise3;

    std::array<Future<int>,3> vec;
    Future<int> fut1 = promise1.get_future();
    Future<int> fut2 = promise2.get_future();
    Future<int> fut3 = promise3.get_future();
    vec[0] = std::move(fut1);
    vec[1] = std::move(fut2);
    vec[2] = std::move(fut3);

    promise1.set_value(42);
    promise2.set_value(69);
    promise3.set_value(451);

    Future<std::array<int, 3>> res = flatten(vec);
    std::array<int,3> values = res.get();

    std::array<int,3> temp {42 , 69, 451};

    ASSERT_EQ(values, temp);
}

TEST(tuple, three_elements_one_simple_future) {
    Promise<int> p;
    auto t = std::make_tuple(p.get_future(), 2.5, 3);
    auto ft = flatten(t);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p.set_value(42);
    auto j = ft.get();
    ASSERT_EQ(ft.is_ready(), true);
    ASSERT_EQ(std::get<0>(j), 42);
    ASSERT_EQ(std::get<1>(j), 2.5);
    ASSERT_EQ(std::get<2>(j), 3);
}

TEST(tuple, three_elements_no_futures) {
    Promise<int> p;
    auto ft = flatten(std::make_tuple(0.5, 2, 3));
    bool g = ft.is_ready();
    ASSERT_EQ(g, false);
    auto t = ft.get();
    bool x = std::make_tuple(0.5, 2, 3) == t;
    ASSERT_EQ(x, true);
}

TEST(tuple, tree_future) {
    Promise<int> p1;
    Promise<int> p2;
    Promise<int> p3;

    auto tpl = std::make_tuple(p1.get_future(), p2.get_future(), p3.get_future());
    auto ft = flatten(tpl);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    p1.set_value(42);
    p2.set_value(69);
    p3.set_value(451);

    auto t = ft.get();

    ASSERT_EQ(std::get<0>(t), 42);
    ASSERT_EQ(std::get<1>(t), 69);
    ASSERT_EQ(std::get<2>(t), 451);
}

TEST(tuple, collection) {
    Promise<std::vector<int>> p;

    auto tpl = std::make_tuple(1, p.get_future());
    auto ft = flatten(tpl);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::vector<int> vec = {1, 2, 3};
    p.set_value(vec);

    auto t = ft.get();
    ASSERT_EQ(std::get<0>(t), 1);
    ASSERT_EQ(std::get<1>(t), vec);
}

TEST(tuple, exception) {
    Promise<std::vector<int>> p;

    auto tpl = std::make_tuple(1, p.get_future());
    auto ft = flatten(tpl);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    p.set_exception(std::make_exception_ptr(std::logic_error("test")));

    ASSERT_ANY_THROW(ft.get());
}