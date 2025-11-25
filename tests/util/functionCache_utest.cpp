//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/functionCache.h>

struct FunctionCacheTestSuite : public natl::TestSuite<FunctionCacheTestSuite> {};

template<typename test_suite_type = FunctionCacheTestSuite>
struct FunctionCacheTestSuiteTests {
	constexpr static void cacheTest(test_suite_type& ts) noexcept {
		natl::FunctionCache<natl::Size(natl::Size)> functionCache([](natl::Size a) -> natl::Size { return 4 + a; });
		functionCache.bindArguments(2);
		ts.assertEqual(6, functionCache.invoke());
		natl::FunctionCache<natl::Size(natl::Size)> functionCache2 = natl::move(functionCache);
		ts.assertEqual(6, functionCache2.invoke());

		functionCache2.bindArguments(4);
		ts.assertEqual(8, functionCache2.invoke());
	}
	NATL_REGISTER_TEST(cacheTest);

	constexpr static natl::Size recursiveFibonacci(const natl::Size number) noexcept {
		if (number == 0 || number == 1) {
			return number;
		} else {
			return recursiveFibonacci(number - 1) + recursiveFibonacci(number - 2);
		}
	}
	constexpr static natl::Size recursiveFibonacciCached(const natl::Size number) noexcept {
		if (number == 0 || number == 1) {
			return number;
		} else {
			return natl::callFunctionCached(recursiveFibonacciCached, number - 1)
				+ natl::callFunctionCached(recursiveFibonacciCached, number - 2);
		}
	}

	constexpr static void callCacheTest(test_suite_type& ts) noexcept {
		natl::Size amount = 20;
		if (natl::isConstantEvaluated()) {
			amount = 6;
		}
		for (const natl::Size i : natl::Repeat(amount)) {
			ts.assertEqual(recursiveFibonacci(i), recursiveFibonacciCached(i));
		}
	}
	NATL_REGISTER_TEST(callCacheTest);
};

NATL_REGISTER_STD_TESTS(FunctionCacheTestSuite);

NATL_TESTS_MAIN();