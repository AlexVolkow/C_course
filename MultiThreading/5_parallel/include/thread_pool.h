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
    std::atomic_bool *enabled;
    std::mutex mutex;
    std::condition_variable worker_check;
    std::queue<task_type> tasks;
    std::vector<std::thread> workers;

    bool is_correct_awakening(std::atomic_bool *enabled) {
        return !tasks.empty() || !(*enabled);
    }

    void work(std::atomic_bool *enabled) {
        ThreadPool::instance = this;
        while ((*enabled)) {
            task_type task;
            {
                std::unique_lock<std::mutex> lock(mutex);
                worker_check.wait(lock, [&]() { return is_correct_awakening(enabled); });
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
    thread_local static ThreadPool *instance;

    ThreadPool(size_t threads) {
        if (threads <= 0) {
            throw std::logic_error("Positive value required");
        }
        (*enabled) = true;
        for (size_t i = 0; i < threads; i++) {
            workers.push_back(std::thread(&ThreadPool::work, this, enabled));
        }
    }

    ~ThreadPool() {
        (*enabled) = false;
        worker_check.notify_all();
        for (auto &work: workers) {
            if (work.joinable()) {
                work.join();
            }
        }
        delete(enabled);
    }

    template<typename Iter, typename F>
    void parallel(Iter begin, Iter end, F f) {
        std::unique_lock<std::mutex> lock(mutex);
        while (begin != end) {
            tasks.emplace([&f, begin] {
                f(*begin);
            });
            begin++;
        }
        std::atomic_bool *is_work = new std::atomic_bool(true);
        tasks.emplace([is_work] {
            (*is_work) = false;
        });
        worker_check.notify_all();
        lock.unlock();
        work(is_work);
        delete(is_work);
    }

    void execute(task_type task) {
        std::unique_lock<std::mutex> lock(mutex);
        tasks.emplace(task);
        worker_check.notify_one();
    }
};


thread_local ThreadPool *ThreadPool::instance;

#endif //MULTITHREADING_THREADPOOL_H
