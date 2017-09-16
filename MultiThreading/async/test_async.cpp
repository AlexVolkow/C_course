//
// Created by Alexandr Volkov on 16.09.2017.
//
#include <memory>
#include <thread>
#include <zconf.h>
#include <iostream>
#include "promise.h"


void handle_eptr(std::exception_ptr eptr) {
    try {
        if (eptr) {
            std::rethrow_exception(eptr);
        }
    } catch (const std::exception &e) {
        std::cout << "Caught exception \"" << e.what() << "\"\n";
    }
}

int main() {
    auto spPromise = std::make_shared<Promise<int>>(Promise<int>());
    Future<int> waiter = spPromise.get()->get_future();
    auto call = [spPromise]() {
        printf("Waiting for a miracle...\n");
        sleep(1);
        spPromise.get()->set_value(42);
    };
    std::thread thread(call);
    thread.detach();
    int res = waiter.get();
    printf("Future = %d\n", res);
    try {
        //Future<int> waiter1 = spPromise.get()->get_future();
        spPromise.get()->set_exception(std::make_exception_ptr(std::logic_error("test")));
        waiter.get();
    } catch (...) {
        std::exception_ptr eptr = std::current_exception();
        handle_eptr(eptr);
    }
}