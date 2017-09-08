//
// Created by Alexandr Volkov on 07.09.2017.
//

#include "threadpool.h"

Worker* ThreadPool::free_worker() {
    Worker* free = workers.front();
    for (auto it : workers) {
        if (it->free()) {
            return it;
        }
        if (free->task_count() > it->task_count()) {
            free = it;
        }
    }
    return free;
}

ThreadPool::ThreadPool(size_t threads) {
    if (threads <= 0) {
        throw std::logic_error("Positive value required");
    }
    for (size_t i = 0; i < threads; i++) {
        Worker* worker = new Worker();
        workers.push_back(worker);
    }
}


ThreadPool::~ThreadPool() {
    for (auto it: workers) {
        delete it;
    }
}

void ThreadPool::execute(fn_type task) {
    free_worker()->add_task(task);
}
