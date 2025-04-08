#pragma once 

//own
#include "mutex.h"
#include "atomic.h"

//interface 
namespace natl {
    struct OnceFlag {
        Mutex mutex;
        Atomic<bool> called{false}; 
    };

    template<typename Functor, typename... Args>
        requires(requires(Functor&& func, Args&&... args) { { forward<Functor>(func)(forward<Args>(args)...) }; })
    void callOnce(OnceFlag& flag, Functor&& func, Args&&... args) {
        if (!flag.called.load(natl::memory_order_acquire)) {
            LockGuard<Mutex> lock(flag.mutex);
            if (!flag.called.load(natl::memory_order_relaxed)) {
                forward<Functor>(func)(forward<Args>(args)...);
                flag.called.store(true, natl::memory_order_release);
            }
        }
    }
}