//
// Created by Alexandr Volkov on 19.10.2017.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include "../../2_promise_future/include/future.h"

template <typename T>
struct inner_type {
    typedef T type;
};

template <typename T>
struct inner_type<Future<T>> {
    typedef typename inner_type<T>::type type;
};

template <typename X>
using inner_type_t = typename inner_type<X>::type;

template<typename T>
Future<T> flatten_sync(const Future<T> &fut) {
    return std::move(const_cast<Future<T> &>(fut));
}

template<typename T, typename inner_type_t = typename inner_type<T>::type>
Future<inner_type_t> flatten_sync(const Future<Future<T>> &fut) {
    return flatten_sync(fut.get());
}
#endif //PROJECT_UTILS_H
