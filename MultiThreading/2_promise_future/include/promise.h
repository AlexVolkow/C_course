//
// Created by Alexandr Volkov on 16.09.2017.
//

#ifndef MULTITHREADING_PROMISE_H
#define MULTITHREADING_PROMISE_H

#include "shared_state.h"
#include "future.h"

template<typename T>
class Promise {
private:
    std::shared_ptr<SharedState<T>> state;
    bool used;

public:
    Promise() : state(std::make_shared<SharedState<T>>()), used(false) {}

    ~Promise() {
        if (state && !state->is_ready()) {
            set_exception(std::make_exception_ptr(std::logic_error("invalid state(promise is dead)")));
        }
    }

    Promise(const Promise &rhs) = delete;

    Promise(Promise &&) noexcept = default;

    Promise &operator=(const Promise &rhs) = delete;

    Promise &operator=(Promise &&) = default;

    void set_exception(std::exception_ptr exception);

    void set_value(T &&v);

    void set_value(T const &v);

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
void Promise<T>::set_value(T const &v) {
    state->set_value(v);
}

template<>
class Promise<void> {
private:
    std::shared_ptr<SharedState<void>> state;
    bool used;

public:
    Promise() : state(std::make_shared<SharedState<void>>()), used(false) {}

    ~Promise() {
        if (state && !state->is_ready()) {
            set_exception(std::make_exception_ptr(std::logic_error("invalid state(promise is dead)")));
        }
    }

    Promise(const Promise &rhs) = delete;

    Promise(Promise &&) noexcept = default;

    Promise &operator=(const Promise &rhs) = delete;

    Promise &operator=(Promise &&) = default;

    void set_exception(std::exception_ptr exception);

    void set_value();

    Future<void> get_future();
};

void Promise<void>::set_exception(std::exception_ptr exception) {
    state->set_exception(exception);
}

void Promise<void>::set_value() {
    state->set_value();
}

Future<void> Promise<void>::get_future() {
    if (used) {
        throw std::logic_error("get_future() can be called once");
    }
    used = true;
    return Future<void>(state);
}

template<typename T>
class Promise<T&> {
private:
    std::shared_ptr<SharedState<T&>> state;
    bool used;

public:
    Promise() : state(std::make_shared<SharedState<T&>>()), used(false) {}

    ~Promise() {
        if (state && !state->is_ready()) {
            set_exception(std::make_exception_ptr(std::logic_error("invalid state(promise is dead)")));
        }
    }

    Promise(const Promise &rhs) = delete;

    Promise(Promise &&) noexcept = default;

    Promise &operator=(const Promise &rhs) = delete;

    Promise &operator=(Promise &&) = default;

    void set_exception(std::exception_ptr exception);

    void set_value(T &v);

    Future<T&> get_future();
};

template<typename T>
Future<T&> Promise<T&>::get_future() {
    if (used) {
        throw std::logic_error("get_future() can be called once");
    }
    used = true;
    return Future<T&>(state);
}

template<typename T>
void Promise<T&>::set_value(T &v) {
    state->set_value(v);
}

template<typename T>
void Promise<T&>::set_exception(std::exception_ptr exception) {
    state->set_exception(exception);
}

template<typename T>
class Promise<Future<T>> {
private:
    std::shared_ptr<SharedState<Future<T>>> state;
    bool used;

public:
    Promise() : state(std::make_shared<SharedState<Future<T>>>()), used(false) {}

    ~Promise() {
        if (state && !state->is_ready()) {
            set_exception(std::make_exception_ptr(std::logic_error("invalid state(promise is dead)")));
        }
    }

    Promise(const Promise &rhs) = delete;

    Promise(Promise &&) noexcept = default;

    Promise &operator=(const Promise &rhs) = delete;

    Promise &operator=(Promise &&) = default;

    void set_exception(std::exception_ptr exception) {
        state->set_exception(exception);
    };

    void set_value(Future<T> &&v) {
        state->set_value(std::move(v));
    };

    Future<Future<T>> get_future() {
        if (used) {
            throw std::logic_error("get_future() can be called once");
        }
        used = true;
        return Future<Future<T>>(state);
    };
};

#endif //MULTITHREADING_PROMISE_H
