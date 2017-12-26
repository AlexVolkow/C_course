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

    bool is_correct_awakening() {
        return !tasks.empty() || !enabled;
    }

    void work() {
        ThreadPool::instance = this;
        while (enabled) {
            task_type task;
            {
                std::unique_lock<std::mutex> lock(mutex);
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



public:
    thread_local static ThreadPool* instance;

    ThreadPool(size_t threads) {
        if (threads <= 0) {
            throw std::logic_error("Positive value required");
        }
        enabled = true;
        for (size_t i = 0; i < threads; i++) {
            workers.push_back(std::thread(&ThreadPool::work, this));
        }
    }

    ~ThreadPool() {
        enabled = false;
        worker_check.notify_all();
        for (auto &work: workers) {
            if (work.joinable()) {
                work.join();
            }
        }
    }

    void execute(task_type task) {
        std::unique_lock<std::mutex> lock(mutex);
        tasks.emplace(task);
        worker_check.notify_one();
    }
};


thread_local ThreadPool * ThreadPool::instance;

#endif //MULTITHREADING_THREADPOOL_H
