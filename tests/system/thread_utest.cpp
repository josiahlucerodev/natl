//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/sync/thread.h>
#include <natl/sync/smartThread.h>
#include <natl/sync/mutex.h>

struct ThreadTestSuite : public natl::TestSuite<ThreadTestSuite> {};

template<typename test_suite_type = ThreadTestSuite>
struct ThreadTestSuiteTests {

	static void work(natl::ThreadStopAlert stopControl, natl::Bool* stopped) noexcept {
		while (!stopControl.stopRequested()) {
			natl::thisThreadSleep(natl::Milliseconds<natl::i64>(20));
			*stopped = true;
		}
		return;
	}

	constexpr static void tests(test_suite_type& ts) noexcept {
		natl::Bool stopped = false;
		natl::SmartThread thread(work, &stopped);
		ts.assertTrue(thread.isActive());

		natl::thisThreadSleep(natl::Milliseconds<natl::i64>(100));
		thread.requestStop();
		natl::thisThreadSleep(natl::Milliseconds<natl::i64>(100));
		thread.join();

		ts.assertFalse(thread.isActive());
		ts.assertTrue(stopped);
	}
	NATL_REGISTER_TEST(tests);
};

NATL_REGISTER_STD_TESTS(ThreadTestSuite);

NATL_TESTS_MAIN();
