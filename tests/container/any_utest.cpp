//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/any.h>
#include <natl/container/string.h>

struct AnyTestSuite : public natl::TestSuite<AnyTestSuite> {};

template<typename test_suite_type = AnyTestSuite>
struct AnyTestSuiteTests {
	constexpr static void constructionTest(test_suite_type& ts) noexcept {
		using namespace natl::literals;
		ts.assertEqual(1, natl::makeAny<natl::Size>(1_size).getAs<natl::Size>().value());
		ts.assertEqual(2, natl::makeAny<natl::Size>(2_size).getAs<natl::Size>().value());
		ts.assertEqual("Test String", natl::makeAny<natl::String256>("Test String").getAs<natl::String256>().value());
	}
	NATL_REGISTER_TEST_CONSTEXPR(constructionTest);

	constexpr static void moveTest(test_suite_type& ts) noexcept {
		natl::Any anyValue = natl::makeAny<natl::String256>("Test String");
		natl::Any anyMoveDst = move(anyValue);
		ts.assertEqual("Test String", anyMoveDst.getAs<natl::String256>().value());
		ts.assertFalse(anyValue.hasValue());
	}
	NATL_REGISTER_TEST_CONSTEXPR(moveTest);
};

NATL_REGISTER_STD_TESTS(AnyTestSuite);

NATL_TESTS_MAIN();