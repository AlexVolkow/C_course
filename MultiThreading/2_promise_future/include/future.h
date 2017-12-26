//
// Created by Alexandr Volkov on 16.09.2017.
//

#ifndef MULTITHREADING_FUTURE_H
#define MULTITHREADING_FUTURE_H

#include "shared_state.h"

size_t GLOBAL_COUNTER = 0;

template<typename T>
class SharedState;

template<typename T>
class Future {
private:
    size_t id = GLOBAL_COUNTER++;
    std::shared_ptr<SharedState<T>> state;

public:
    Future(std::shared_ptr<SharedState<T>> state) : state(state) {};

    Future() : state(std::make_shared<SharedState<T>>(true)) {};

    Future(const Future &rhs) = delete;

    Future &operator=(const Future &rhs) = delete;

    Future(Future &&) noexcept = default;

    Future &operator=(Future &&) noexcept = default;

    friend bool operator<(const Future &l, const Future &r) {
        return l.id < r.id;
    }

    T get() const;

    void wait() const;

    bool is_ready() const;

    bool has_exception() const;

    std::exception_ptr get_exception() const {
        return state->get_exception();
    }
};

template<typename T>
T Future<T>::get() const {
    return state->get();
}

template<typename T>
void Future<T>::wait() const {
    state->wait();
}

template<typename T>
bool Future<T>::is_ready() const {
    return state->is_ready();
}

template<typename T>
bool Future<T>::has_exception() const {
    return state->has_exception();
}

#endif //MULTITHREADING_FUTURE_H
