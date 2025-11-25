#pragma once

#include "mainMacros.h"
#include "preprocessor.h"

#define NATL_TEST_SUITE_TESTS(Test) \
	template<typename test_suite_type = natl::TestSuiteBase> struct Test

#define NATL_REGISTER_TEST(Test) \
	static inline auto NATL_PLACE_HOLDER_NAME = natl::impl::RegisterTest<test_suite_type, &##Test>(#Test);

// #define NATL_DISABLE_CONSTEXPR_TESTS
#ifdef NATL_DISABLE_CONSTEXPR_TESTS 

#define NATL_REGISTER_TEST_CONSTEXPR(Test) \
	NATL_REGISTER_TEST(Test) 

#else

#define NATL_REGISTER_TEST_CONSTEXPR(Test) \
	NATL_REGISTER_TEST(Test) \
	static_assert(natl::impl::testIsSuiteConstexpr<test_suite_type>() && natl::impl::runTestConstexpr<test_suite_type, Test>());

#endif // NATL_DISABLE_CONSTEXPR_TESTS


#define NATL_REGISTER_PARAMETERIZED_TEST(Test) \
	static inline auto NATL_PLACE_HOLDER_NAME = natl::impl::RegisterParameterizedTest<test_suite_type, &##Test, &##Test##Gen>(#Test);

#define NATL_REGISTER_PARAMETERIZED_TEST_CONSTEXPR(Test) \
	static inline auto NATL_PLACE_HOLDER_NAME = natl::impl::RegisterParameterizedTest<test_suite_type, &##Test, &##Test##Gen>(#Test); 
//static_assert(natl::impl::testIsSuiteConstexpr<test_suite_type>() && natl::impl::runParameterizedTestConstexpr<test_suite_type, &##Test, &##Gen>());

#define NATL_REGISTER_TESTS(TestSuite, Tests) \
	static Tests<TestSuite> NATL_PLACE_HOLDER_NAME; \
	static_assert(natl::IsValidTestSuiteC<##TestSuite>); \
	static natl::impl::RegisterTestSuite<##TestSuite> NATL_PLACE_HOLDER_NAME(#TestSuite);

#define NATL_REGISTER_STD_TESTS(TestSuite) \
	NATL_REGISTER_TESTS(TestSuite, TestSuite##Tests)

#define NATL_TESTS_MAIN() \
	natl::impl::TestEnvironment natl::impl::globalTestEnvironment{}; \
	natl::ExitCode cpp_main(const natl::MainArgsView args) { \
		natl::impl::setTestResourcePathToFile(__FILE__); \
		return natl::test_main(args); \
	} \
	NATL_CPP_MAIN();

#define NAT_SET_DEFAULT_TEST_RESOURCE_PATH(ResourcePath) \
	static auto NATL_PLACE_HOLDER_NAME = [] () { \
		natl::setTestResourcePath(ResourcePath); \
		return 0; \
	}();