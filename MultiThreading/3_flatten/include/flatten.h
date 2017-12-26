//
// Created by disqustingman on 08.10.17.
//

#ifndef MULTITHREADING_FLATTEN_H
#define MULTITHREADING_FLATTEN_H

#include <memory>
#include <stack>
#include <deque>
#include "../../2_promise_future/include/future.h"
#include "../../2_promise_future/include/promise.h"
#include "utils.h"

template<typename T>
Future<T> flatten(const Future<T> &fut) {
    return std::move(const_cast<Future<T> &>(fut));
}

template<typename T, typename inner_type_t = typename inner_type<T>::type>
Future<inner_type_t> flatten(const Future<Future<T>> &fut) {
    Promise<inner_type_t> promise;
    Future<inner_type_t> future = promise.get_future();

    std::thread t(
            [&fut](Promise<inner_type_t> promise) {
                try {
                    Future<inner_type_t> val = flatten_sync(fut);

                    if (val.has_exception()) {
                        promise.set_exception(val.get_exception());
                        return;
                    }

                    promise.set_value(val.get());
                } catch (...) {
                    promise.set_exception(std::current_exception());
                }
            }, std::move(promise)
    );

    t.detach();

    return future;
}

template<typename Iter, typename InsertIter>
void copy_with_get(Iter begin, Iter end, InsertIter dest) {
    while (begin != end) {
        dest = begin->get();
        begin++;
    }
}

template<template<typename...> class C, typename T>
Future<C<T>> flatten(const C<Future<T>> &collection) {
    Promise<C<T>> promise;
    Future<C<T>> future = promise.get_future();

    std::thread t(
            [&collection](Promise<C<T>> promise) {
                C <T> res;
                auto res_iter = std::inserter(res, res.begin());
                try {
                    copy_with_get(collection.begin(), collection.end(), res_iter);
                } catch (...) {
                    promise.set_exception(std::current_exception());
                    return;
                }
                promise.set_value(res);
            }, std::move(promise)
    );

    t.detach();

    return future;
}


template<template<typename, size_t> class C,
        typename T,
        size_t N>
Future<C<T, N>> flatten(const C<Future<T>, N> &collection) {
    Promise<C<T, N>> promise;
    Future<C<T, N>> future = promise.get_future();

    std::thread t(
            [&collection](Promise<C<T, N>> promise) {
                C <T, N> res;
                auto begin = collection.begin();
                auto end = collection.end();
                try {
                    int i = 0;
                    while (begin != end) {
                        res[i++] = begin->get();
                        begin++;
                    }
                } catch (...) {
                    promise.set_exception(std::current_exception());
                    return;
                }
                promise.set_value(res);
            }, std::move(promise)
    );

    t.detach();

    return future;
}

template<class T>
inner_type_t<T> flatten_tuple(const Future<T> &element) {
    return flatten_sync(element).get();
}

template<class T>
T flatten_tuple(const T &element) {
    return std::move(element);
}

template<typename ...Args, std::size_t...Is>
std::tuple<inner_type_t<Args>...> flatten_tuple_impl(const std::tuple<Args...> & tuple, std::index_sequence<Is...>) {
    return std::make_tuple(std::move(flatten_tuple(std::get<Is>(tuple)))...);
}

template<typename...Args>
std::tuple<inner_type_t<Args>...> flatten_tuple(const std::tuple<Args...> & tuple) {
    return flatten_tuple_impl(tuple, std::make_index_sequence<sizeof...(Args)>());
}

template<typename ...Args>
Future<std::tuple<inner_type_t<Args>...>> flatten(const std::tuple<Args...> &tuple) {
    Promise<std::tuple<inner_type_t<Args>...>> promise;
    auto future = promise.get_future();

    std::thread t(
            [&tuple](Promise<std::tuple<inner_type_t<Args>...>> promise) {
                try {
                    auto res = flatten_tuple(tuple);
                    promise.set_value(res);
                } catch (...) {
                    promise.set_exception(std::current_exception());
                }
            }, std::move(promise)
    );

    t.detach();

    return future;
}

#endif //MULTITHREADING_FLATTEN_H
