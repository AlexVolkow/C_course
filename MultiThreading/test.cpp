#include <iostream>
#include "threadpool.h"
#include <zconf.h>

using namespace std;

void foo() {
    for (int i = 0; i < 5; i++) {
        printf("Hello world %d (id=%d)\n", i + 1, this_thread::get_id());
    }
}

void bar() {
    for (int i = 0; i < 5; i++) {
        printf("It's alive! %d (id=%d)\n", i + 1, this_thread::get_id());
    }
}

void baz() {
    for (int i = 0; i < 5; i++) {
        printf("This world is yours %d (id=%d)\n", i + 1, this_thread::get_id());
    }
}

int main() {
    ThreadPool pool(2);

    pool.execute(&foo);
    pool.execute(&bar);
    pool.execute(&baz);
    sleep(1);
    return 0;

}