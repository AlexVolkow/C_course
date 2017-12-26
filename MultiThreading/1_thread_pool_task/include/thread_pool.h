//
// Created by Alexandr Volkov on 07.09.2017.
//

#ifndef MULTITHREADING_THREADPOOL_H
#define MULTITHREADING_THREADPOOL_H

#include <stdexcept>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <atomic>

typedef std::function<void()> task_type;

class ThreadPool {
private:
    std::atomic_bool enabled;
    std::mutex mutex;
    std::condition_variable	worker_check;
    std::queue<task_type> tasks;
    std::vector<std::thread> workers;

    void work();

    bool is_correct_awakening();

public:
    ThreadPool(size_t threads) noexcept;

    ~ThreadPool();

    void execute(task_type task);
};

#endif //MULTITHREADING_THREADPOOL_H
