//
// Created by disqustingman on 07.10.17.
//

#ifndef MULTITHREADING_BASE_SHARED_STATE_H
#define MULTITHREADING_BASE_SHARED_STATE_H

#include <atomic>
#include <mutex>
#include <functional>
#include <condition_variable>

class BaseSharedState {
private:
    std::atomic_bool ready;
    std::exception_ptr exception;
    std::mutex mutex;
    std::condition_variable state_notification;

public:
    bool used;

    BaseSharedState() : ready(false), used(false) {}

    void set_exception(std::exception_ptr e);

    void wait();

    bool is_ready();

    bool has_exception();

    std::exception_ptr get_exception();

    void set(std::function<void()> logic);

    void check_state();

    void rethrow_exception();
};

void BaseSharedState::check_state() {
    if (used) {
        throw std::logic_error("invalid state");
    }
}

void BaseSharedState::wait() {
    check_state();
    if (is_ready()) {
        return;
    }
    std::unique_lock<std::mutex> lock(mutex);
    state_notification.wait(lock, [&]() { return is_ready(); });
}

bool BaseSharedState::is_ready() {
    return ready.load();
}

void BaseSharedState::set(std::function<void()> logic) {
    std::lock_guard<std::mutex> lock(mutex);
    check_state();
    logic();
    ready = true;
    state_notification.notify_all();
}

void BaseSharedState::set_exception(std::exception_ptr e) {
    set([&]() { exception = e; });
}

void BaseSharedState::rethrow_exception() {
    if (exception) {
        std::rethrow_exception(exception);
    }
}

bool BaseSharedState::has_exception() {
    if (exception) {
        return true;
    }
    return false;
}

std::exception_ptr BaseSharedState::get_exception() {
    return exception;
}

#endif //MULTITHREADING_BASE_SHARED_STATE_H
