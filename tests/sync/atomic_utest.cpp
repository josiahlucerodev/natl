//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/sync/atomic.h>
#include <natl/sync/thread.h>
#include <natl/container/dynArray.h>
#include <natl/system/printFormatted.h>
#include <natl/util/uninitialized.h>

struct ExampleTestSuite : public natl::TestSuite<ExampleTestSuite> {};

template<typename test_suite_type = ExampleTestSuite>
struct ExampleTestSuiteTests {

    constexpr static void incrementTest(test_suite_type& ts) noexcept {
        natl::AtomicSize atomicSize(2);
        ts.assertEqual(2, atomicSize);
        atomicSize++;
        ts.assertEqual(3, atomicSize);
        atomicSize++;
        ts.assertEqual(4, atomicSize);
        atomicSize++;
        ts.assertEqual(5, atomicSize);
    }
    NATL_REGISTER_TEST_CONSTEXPR(incrementTest);

    static inline natl::AtomicSize atomicConuter = 0;
    static inline natl::Size counter = 0;

    static void countWork() noexcept {
        for (natl::Size n = 0; n < 10000; ++n) {
            ++atomicConuter;
            ++counter;
        }
    }

    constexpr static void threadTest(test_suite_type& ts) noexcept {
        natl::Array<natl::UninitializedValue<natl::Thread>, 10> pool;
        for (natl::Size i = 0; i < pool.size(); i++) {
            pool[i].construct(countWork);
        }

        for (auto& thread : pool) {
            thread.value().join();
            thread.deconstruct();
        }
        ts.assertNotEqual(counter, 10000 * 10);
        ts.assertNotEqual(counter, atomicConuter.load());
    }
    NATL_REGISTER_TEST(threadTest);
};

NATL_REGISTER_STD_TESTS(ExampleTestSuite);

NATL_TESTS_MAIN();
