//
// Created by Alexandr Volkov on 07.09.2017.
//

#include <iostream>
#include <thread>
#include "worker.h"

Worker::Worker() {
    enabled = true;
    fqueue = std::queue<fn_type>();
    thread = std::thread(work, this);
}

Worker::~Worker() {
    enabled = false;
    worker_check.notify_one();
    thread.join();
}

void Worker::add_task(fn_type task) {
    std::unique_lock<std::mutex> lock(mutex);
    fqueue.push(task);
    worker_check.notify_one();
}

std::size_t Worker::task_count() {
    std::unique_lock<std::mutex> lock(mutex);
    return fqueue.size();
}

bool Worker::free() {
    std::unique_lock<std::mutex> lock(mutex);
    return fqueue.empty();
}

bool Worker::is_correct_awakening() {
    return !fqueue.empty() || !enabled;
}

void Worker::work() {
    std::unique_lock<std::mutex> lock(mutex);

    worker_check.wait(lock, [&]() { return is_correct_awakening(); });
    while (!fqueue.empty()) {
        fn_type task = fqueue.front();

        task();

        fqueue.pop();
    }
}
