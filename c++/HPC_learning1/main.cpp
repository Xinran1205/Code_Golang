#include <iostream>
#include <thread>
#include <mutex>

int sharedVariable = 0; // 共享变量
std::mutex mtx;         // 用于同步的互斥锁

void increment() {
    for (int i = 0; i < 10000; ++i) {
        mtx.lock();
        ++sharedVariable; // 保护共享资源
        mtx.unlock();
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Shared Variable: " << sharedVariable << std::endl;
    return 0;
}
