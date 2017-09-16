//
// Created by Alexandr Volkov on 16.09.2017.
//

#ifndef MULTITHREADING_FUTURE_H
#define MULTITHREADING_FUTURE_H

#include "sharedstate.h"

template<typename T>
class Future {
private:
    std::shared_ptr<SharedState<T>> state;

public:
    Future(std::shared_ptr<SharedState<T>> state) : state(state) {};

    Future(const Future &rhs) = delete;

    Future(Future &&) = default;

    Future &operator=(const Future &rhs) = delete;

    Future &operator=(Future &&) = default;

    T get();

    void wait();

    bool is_ready();
};

template<typename T>
T Future<T>::get() {
    return state->get();
}

template<typename T>
void Future<T>::wait() {
    state->wait();
}

template<typename T>
bool Future<T>::is_ready() {
    return state->is_ready();
}

#endif //MULTITHREADING_FUTURE_H
