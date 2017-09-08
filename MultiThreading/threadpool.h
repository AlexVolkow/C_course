//
// Created by Alexandr Volkov on 07.09.2017.
//

#ifndef MULTITHREADING_THREADPOOL_H
#define MULTITHREADING_THREADPOOL_H

#include <stdexcept>
#include "worker.h"

class ThreadPool {
private:
    Worker* free_worker();

    std::vector<Worker*> workers;

public:
    ThreadPool(size_t threads);

    ~ThreadPool();

    void execute(fn_type task);
};

#endif //MULTITHREADING_THREADPOOL_H
