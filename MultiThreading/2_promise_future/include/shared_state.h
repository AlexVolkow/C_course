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
#include "base_shared_state.h"
#include "future.h"

template <typename T>
class Future;

template<typename T>
class SharedState : public BaseSharedState {
private:
    T value;
    std::mutex mutex;

    T try_get();

public:
    SharedState() : BaseSharedState() {}

    SharedState(bool used) : BaseSharedState() { this->used = used; }

    void set_value(T &&v);

    void set_value(T const &v);

    T get();
};


template<typename T>
T SharedState<T>::get() {
    wait();
    return try_get();
}


template<typename T>
T SharedState<T>::try_get() {
    std::lock_guard<std::mutex> lock(mutex);
    used = true;
    rethrow_exception();
    return value;
}

template<typename T>
void SharedState<T>::set_value(T const &v) {
    set([&]() { value = std::move(v); });
}

template<typename T>
void SharedState<T>::set_value(T &&v) {
    set_value(v);
}

template <typename T>
class SharedState<T&> : public BaseSharedState {
private:
    T *value;
    std::mutex mutex;

    T& try_get();

public:
    SharedState(bool used) : BaseSharedState() { this->used = used; }

    SharedState() : BaseSharedState() {}

    void set_value(T &v);

    T& get();
};


template<typename T>
T& SharedState<T&>::get() {
    wait();
    return try_get();
}


template<typename T>
T& SharedState<T&>::try_get() {
    std::lock_guard<std::mutex> lock(mutex);
    used = true;
    rethrow_exception();
    return *value;
}

template<typename T>
void SharedState<T&>::set_value(T &v) {
    set([&]() { value = &v; });
}

template <typename T>
class SharedState<Future<T>> : public BaseSharedState {
private:
    Future<T> *value;
    std::mutex mutex;

    Future<T> try_get();

public:
    SharedState(bool used) : BaseSharedState() { this->used = used; }

    SharedState() : BaseSharedState() {}

    void set_value(Future<T> &&v);

    Future<T> get();
};


template<typename T>
Future<T> SharedState<Future<T>>::get() {
    wait();
    return try_get();
}


template<typename T>
Future<T> SharedState<Future<T>>::try_get() {
    std::lock_guard<std::mutex> lock(mutex);
    used = true;
    rethrow_exception();
    return std::move(*value);
}

template<typename T>
void SharedState<Future<T>>::set_value(Future<T> &&v) {
    set([&]() { value = &v; });
}

template <>
class SharedState<void> : public BaseSharedState {
private:
    std::mutex mutex;

    void try_get();

public:
    SharedState(bool used) : BaseSharedState() { this->used = used; }

    SharedState() : BaseSharedState() {}

    void set_value();

    void get();
};

void SharedState<void>::set_value() {
    set([&]() {});
}

void SharedState<void>::get() {
    wait();
    try_get();
}

void SharedState<void>::try_get() {
    std::lock_guard<std::mutex> lock(mutex);
    used = true;
    rethrow_exception();
}

#endif //MULTITHREADING_SHAREDSTATE_H
