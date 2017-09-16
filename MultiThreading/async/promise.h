//
// Created by Alexandr Volkov on 16.09.2017.
//

#ifndef MULTITHREADING_PROMISE_H
#define MULTITHREADING_PROMISE_H

#include "sharedstate.h"
#include "future.h"

template<typename T>
class Promise {
private:
    std::shared_ptr<SharedState<T>> state;
    bool used;

public:
    Promise() : state(std::make_shared<SharedState<T>>()), used(false) {}

    Promise(const Promise &rhs) = delete;

    Promise(Promise &&) = default;

    Promise &operator=(const Promise &rhs) = delete;

    Promise &operator=(Promise &&) = default;

    void set_exception(std::exception_ptr exception);

    void set_value(T &&v);

    void set_value(T &v);

    Future<T> get_future();
};

template<typename T>
void Promise<T>::set_exception(std::exception_ptr exception) {
    state->set_exception(exception);
}

template<typename T>
void Promise<T>::set_value(T &&v) {
    state->set_value(v);
}

template<typename T>
Future<T> Promise<T>::get_future() {
    if (used) {
        throw std::logic_error("get_future() can be called once");
    }
    used = true;
    return Future<T>(state);
}

template<typename T>
void Promise<T>::set_value(T &v) {
    state->set_value(v);
}

#endif //MULTITHREADING_PROMISE_H
