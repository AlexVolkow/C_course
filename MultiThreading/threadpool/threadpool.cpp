//
// Created by Alexandr Volkov on 07.09.2017.
//

#include "threadpool.h"

using namespace std;


ThreadPool::ThreadPool(size_t threads) {
    if (threads <= 0) {
        throw std::logic_error("Positive value required");
    }
    enabled = true;
    for (size_t i = 0; i < threads; i++) {
        workers.push_back(thread(work, this));
    }
}

void ThreadPool::work() {
    while (enabled) {
        task_type task;
        {
            unique_lock<std::mutex> lock(mutex);
            worker_check.wait(lock, [&]() { return is_correct_awakening(); });
            if (!tasks.empty()) {
                task = tasks.front();
                tasks.pop();
            }
        }
        if (task) {
            task();
        }
    }
}

ThreadPool::~ThreadPool() {
    enabled = false;
    worker_check.notify_all();
    for (auto &work: workers) {
        if (work.joinable()) {
            work.join();
        }
    }
}

void ThreadPool::execute(task_type task) {
    unique_lock<std::mutex> lock(mutex);
    tasks.push(task);
    worker_check.notify_one();
}


bool ThreadPool::is_correct_awakening() {
    return !tasks.empty() || !enabled;
}
