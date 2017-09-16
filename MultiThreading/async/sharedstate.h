//
// Created by Alexandr Volkov on 16.09.2017.
//

#ifndef MULTITHREADING_SHAREDSTATE_H
#define MULTITHREADING_SHAREDSTATE_H

#include <atomic>
#include <exception>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "utils.h"

template<typename T>
class SharedState {
private:
    std::atomic_bool ready;
    std::exception_ptr exception;
    std::unique_ptr<T> value;
    std::mutex mutex;
    std::condition_variable state_notification;

    bool used;

    T try_get();

    void set_any(std::function<void()> logic);

public:
    SharedState() : ready(false), used(false) {};

    void set_value(T &&v);

    void set_value(T &v);

    void set_exception(std::exception_ptr e);

    T get();

    void wait();

    bool is_ready();
};

template<typename T>
T SharedState<T>::get() {
    wait();
    return try_get();
}

template<typename T>
void SharedState<T>::wait() {
    if (used) {
        throw std::logic_error("invalid state");
    }
    if (is_ready()) {
        return;
    }
    std::unique_lock<std::mutex> lock(mutex);
    state_notification.wait(lock, [&]() { return is_ready(); });
}

template<typename T>
T SharedState<T>::try_get() {
    std::lock_guard<std::mutex> lock(mutex);
    used = true;
    if (exception) {
        std::rethrow_exception(exception);
    }
    if (value) {
        return std::move(*value.release());
    }
}

template<typename T>
bool SharedState<T>::is_ready() {
    return ready.load();
}

template<typename T>
void SharedState<T>::set_value(T &v) {
    set_any([&]() { value = make_unique<T>(v); });
}

template<typename T>
void SharedState<T>::set_value(T &&v) {
    set_any([&]() { value = make_unique<T>(std::move(v)); });
}

template<typename T>
void SharedState<T>::set_exception(std::exception_ptr e) {
    set_any([&]() { exception = e; });
}

template<typename T>
void SharedState<T>::set_any(std::function<void()> logic) {
    std::lock_guard<std::mutex> lock(mutex);
    if (used) {
        throw std::logic_error("invalid state");
    }
    logic();
    ready = true;
    state_notification.notify_all();
}

#endif //MULTITHREADING_SHAREDSTATE_H
