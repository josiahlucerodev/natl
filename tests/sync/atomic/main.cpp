//natl
#include <natl/sync/atomic.h>
#include <natl/sync/thread.h>
#include <natl/container/dynArray.h>
#include <natl/system/printFormatted.h>
#include <natl/util/uninitialized.h>

constexpr natl::Size constexprTest() {
    natl::AtomicSize atomicSize(3);
    atomicSize++;
    atomicSize++;
    atomicSize++;
    return atomicSize;
}

natl::AtomicI64 atomicConuter;
natl::Size counter;

void f() {
    for (natl::Size n = 0; n < 10000; ++n) {
        ++atomicConuter;
        ++counter;
    }
}

int main() {
    [[maybe_unused]] constexpr natl::Size testValue = constexprTest();
    {
        natl::Array<natl::UninitializedValue<natl::Thread>, 10> pool;
        for (natl::Size i = 0; i < pool.size(); i++)
            pool[i].construct(f);

        for (auto& thread : pool) {
            thread.value().join();
            thread.deconstruct();
        }
    }
    natl::printlnf("The atomic counter is ", atomicConuter.getValue(), natl::FormatNewLine{},
        "The non-atomic counter is ", counter, natl::FormatNewLine{});
}