//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/sync/synchronizedValue.h>

struct ExampleTestSuite : public natl::TestSuite<ExampleTestSuite> {};

template<typename test_suite_type = ExampleTestSuite>
struct ExampleTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		natl::SynchronizedValue<natl::Bool> syncValue(false);
		{
			natl::UpdateGuard<natl::Bool> guard = syncValue;
			ts.assertEqual(false, guard.value());
			guard.value() = true;
		}
		{
			natl::UpdateGuard<const natl::Bool> guard = syncValue;
			ts.assertEqual(true, guard.value());
		}

		natl::synchronizedApply([](natl::Bool& value) -> void { value = false; }, syncValue);
		{
			natl::UpdateGuard<const natl::Bool> guard = syncValue;
			ts.assertEqual(false, guard.value());
		}

		ts.assertEqual(false, syncValue.blockingGetCopy());
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(ExampleTestSuite);

NATL_TESTS_MAIN();