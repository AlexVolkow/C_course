#include <iostream>
#include "threadpool.h"
#include <thread>
#include <zconf.h>

using namespace std;

void foo() {
    for (int i = 0; i < 10; i++) {
        cout << "hello world " << (i + 1) << " " << this_thread::get_id() << endl ;
    }
}

void bar() {
    for (int i = 0; i < 10; i++) {
        cout << "It's alive! " << (i + 1) << " " << this_thread::get_id() << endl ;
    }
}

void baz() {
    cout << "This world is yours " << this_thread::get_id() << endl ;
}
int main() {
    ThreadPool pool(2);

    pool.execute(&foo);
    pool.execute(&bar);
    pool.execute(&baz);
    return 0;
}