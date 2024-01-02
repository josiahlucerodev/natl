//natl
#include <natl/atomic.h>


//std
#include <iostream>
#include <thread>
#include <vector>

constexpr natl::Size constexprTest() {
    natl::AtomicSize atomicSize(3);
    atomicSize++;
    atomicSize++;
    atomicSize++;
    return atomicSize;
}

natl::AtomicI64 acnt;
natl::Size cnt;

//based on https://en.cppreference.com/w/cpp/atomic/atomic
void f() {
    for (natl::Size n = 0; n < 10000; ++n) {
        ++acnt;
        ++cnt;
    }
}

int main() {
    [[maybe_unused]] constexpr natl::Size testValue = constexprTest();
    {
        std::vector<std::jthread> pool;
        for (natl::Size n = 0; n < natl::Size(10); ++n)
            pool.emplace_back(f);
    }
    std::cout << "The atomic counter is " << acnt << '\n' << "The non-atomic counter is " << cnt << '\n';
}