//
// Created by Alexandr Volkov on 07.09.2017.
//

#ifndef MULTITHREADING_WORKER_H
#define MULTITHREADING_WORKER_H

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

typedef std::function<void()> fn_type;

class Worker {
private:
    bool enabled;
    std::condition_variable	worker_check;
    std::queue<fn_type>	fqueue;
    std::mutex	mutex;
    std::thread	thread;

    void work();

    bool is_correct_awakening();

public:
    Worker();

    ~Worker();

    void add_task(fn_type task);

    std::size_t task_count();

    bool free();
};


#endif //MULTITHREADING_WORKER_H
