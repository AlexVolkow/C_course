//
// Created by Alexandr Volkov on 16.09.2017.
//

#ifndef MULTITHREADING_UTILS_H
#define MULTITHREADING_UTILS_H

#include <memory>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif //MULTITHREADING_UTILS_H
