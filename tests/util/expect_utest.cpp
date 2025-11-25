//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/expect.h>

struct ExpectTestSuite : public natl::TestSuite<ExpectTestSuite> {};

template<typename test_suite_type = ExpectTestSuite>
struct ExpectTestSuiteTests {

	enum struct TestError {
		none,
		invalid,
		overflow
	};

	constexpr static natl::Expect<natl::Bool, TestError> testFunc(natl::Bool value, TestError error) noexcept {
		if (error != TestError::none) {
			return natl::unexpected<TestError>(error);
		} else {
			return value;
		}
	}

	constexpr static void tests(test_suite_type& ts) noexcept {
		ts.assertTrue(testFunc(true, TestError::none).hasValue());
		ts.assertFalse(testFunc(true, TestError::none).hasError());
		ts.assertTrue(testFunc(true, TestError::none).value());

		ts.assertTrue(testFunc(false, TestError::none).hasValue());
		ts.assertFalse(testFunc(true, TestError::none).hasError());
		ts.assertFalse(testFunc(false, TestError::none).value());

		ts.assertFalse(testFunc(false, TestError::invalid).hasValue());
		ts.assertTrue(testFunc(true, TestError::invalid).hasError());
		ts.assertEqual(TestError::invalid, testFunc(false, TestError::invalid).error());
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(ExpectTestSuite);

NATL_TESTS_MAIN();