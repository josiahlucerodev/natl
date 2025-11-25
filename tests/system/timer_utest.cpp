//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/system/timer.h>
#include <natl/system/printFormatted.h>
#include <natl/sync/thread.h>

struct TimerTestSuite : public natl::TestSuite<TimerTestSuite> {};

template<typename test_suite_type = TimerTestSuite>
struct TimerTestSuiteTests {
	static void tests(test_suite_type& ts) noexcept {
		natl::Milliseconds<natl::i64> sleepTime(10);
		for ([[maybe_unused]] natl::Size i : natl::Repeat(10)) {
			natl::Timer timer;
			natl::thisThreadSleep(sleepTime);

			ts.assertLe(sleepTime.value(), timer.getMillisecondsInt().value());
			ts.assertGt(sleepTime.value() * 5, timer.getMillisecondsInt().value());
		}
	}
	NATL_REGISTER_TEST(tests);
};

NATL_REGISTER_STD_TESTS(TimerTestSuite);

NATL_TESTS_MAIN();
