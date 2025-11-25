//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/container/functional.h>
#include <natl/util/test.h>

struct FunctionalTestSuite : public natl::TestSuite<FunctionalTestSuite> {};

template<typename test_suite_type = FunctionalTestSuite>
struct FunctionalTestSuiteTests {
	constexpr static natl::Bool work() noexcept {
		return true;
	}

	constexpr static void tests(test_suite_type& ts) noexcept {
		auto test = []() noexcept -> natl::Bool { return true; };
		natl::impl::testHasFunctionSignature<decltype(test), natl::Bool>();
		natl::Function<natl::Bool()> function = []() noexcept -> natl::Bool { return true; };
		natl::Function<natl::Bool()> function2 = natl::move(function);
		natl::Function<natl::Bool()> function3 = function2;
		natl::Function<natl::Bool()> function4(work);

		ts.assertTrue(function3.invoke());
		ts.assertTrue(function4.invoke());
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(FunctionalTestSuite);

NATL_TESTS_MAIN();