//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/container/dynArray.h>  
#include <natl/system/print.h>  
#include <natl/util/main.h>  
#include <natl/util/random.h>
#include <natl/sync/thread.h>

//natl test
#include <natl/util/test.h>   

struct ExampleTestSuite : public natl::TestSuite<ExampleTestSuite> {
private:
	natl::Bool isSetup;
	natl::Bool isTeardown;

public:
	constexpr ExampleTestSuite() : isSetup(false), isTeardown(true) {}

public:
	constexpr natl::Bool setup() noexcept override {
		if (isSetup || !isTeardown) { return false; }
		isSetup = true;
		isTeardown = false;
		return true;
	}
	constexpr natl::Bool teardown() noexcept override {
		if (!isSetup || isTeardown) { return false; }
		isTeardown = true;
		isSetup = false;
		return true;
	}
};

template<typename test_suite_type = ExampleTestSuite>
struct ExampleTestSuiteTests {
	constexpr static void testSeed(test_suite_type& ts) noexcept {
		ts.trace();
		ts.trace("Test Begin");
		ts.trace("seed: ", natl::formatArgText<"hex">(ts.seed()));
		ts.assertNotEqual(0, ts.seed());
		ts.trace("Test End");
	}
	NATL_REGISTER_TEST_CONSTEXPR(testSeed);

	constexpr static void testAssert(test_suite_type& ts) noexcept {
		ts.trace("Test Begin");
		ts.tassert(true);
		ts.trace("Test End");
	}
	NATL_REGISTER_TEST_CONSTEXPR(testAssert);

	constexpr static void testConditions(test_suite_type& ts) noexcept {
		ts.trace("Test Begin");

		ts.assertTrue(true);
		ts.assertFalse(false);
		
		natl::Byte val;
		natl::Byte* ptr = nullptr;
		ts.assertNull(ptr);
		ptr = &val;
		ts.assertNotNull(ptr);

		ts.assertEqual(0, 0);
		ts.assertNotEqual(0, 1);
		ts.assertLessThan(0, 1);
		ts.assertLessThanEqual(0, 1);
		ts.assertLessThanEqual(1, 1);
		ts.assertGreaterThan(1, 0);
		ts.assertGreaterThanEqual(1, 0);
		ts.assertGreaterThanEqual(1, 1);

		ts.assertEq(0, 0);
		ts.assertNe(0, 1);
		ts.assertLt(0, 1);
		ts.assertLe(0, 1);
		ts.assertLe(1, 1);
		ts.assertGt(1, 0);
		ts.assertGe(1, 0);
		ts.assertGe(1, 1);

		ts.trace("Test End");
	}
	NATL_REGISTER_TEST_CONSTEXPR(testConditions);

	constexpr static void testParameterized(test_suite_type& ts, natl::i64 value) noexcept {
		ts.trace("Test Begin");
		ts.trace(value);
		ts.assertNotEqual(0, value);
		ts.trace("Test End");
	}
	constexpr static natl::ParameterizedValues<natl::i64> testParameterizedGen(test_suite_type&) noexcept {
		return { 1, 2, 3, 4, 5 };
	}
	NATL_REGISTER_PARAMETERIZED_TEST(testParameterized);
};

NATL_REGISTER_STD_TESTS(ExampleTestSuite);

NATL_TESTS_MAIN();