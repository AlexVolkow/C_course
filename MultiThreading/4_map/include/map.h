//
// Created by Alexandr Volkov on 24.10.2017.
//

#ifndef PROJECT_MAP_H
#define PROJECT_MAP_H

#include <future.h>
#include <type_traits>
#include <promise.h>
#include <thread>
#include "thread_pool.h"

template<typename T, typename F,
        typename RT = std::result_of_t<F(T)>>
Future<RT> map(Future<T> const &future, F const &f) {
    Promise<RT> promise;

    std::shared_ptr<Promise<RT>> promise_ptr = std::make_shared<Promise<RT>>(std::move(promise));

    Future<RT> res = promise_ptr->get_future();

    auto work = [&f, &future, promise_ptr] {
                try {
                    T val = future.get();
                    RT map_val = f(val);
                    promise_ptr->set_value(map_val);
                }catch (...) {
                    promise_ptr->set_exception(std::current_exception());
                }
            };

    if (ThreadPool::instance == nullptr) {
        throw std::logic_error("ThreadPool not found");
    }

    ThreadPool::instance->execute(work);

    return res;
}


#endif //PROJECT_MAP_H
