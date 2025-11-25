//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/option.h>

struct OptionTestSuite : public natl::TestSuite<OptionTestSuite> {};

template<typename test_suite_type = OptionTestSuite>
struct OptionTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		using namespace natl::literals;
		natl::Option<natl::Size> number1;
		ts.assertFalse(number1.hasValue());
		
		number1 = 1_size;
		ts.assertTrue(number1.hasValue());
		ts.assertEqual(1_size, number1.value());


		natl::Option<natl::Size> number2 = 5_size;
		ts.assertTrue(number2.hasValue());

		number2 = natl::OptionEmpty();
		ts.assertFalse(number2.hasValue());

		number2 = 3_size;
		ts.assertTrue(number2.hasValue());

		natl::Option<natl::String> number3 = "5";
		ts.assertTrue(number3.hasValue());
		ts.assertEqual("5", number3.value());


		natl::OptionPtr<natl::Size> number4Ptr;
		ts.assertFalse(number4Ptr.hasValue());

		natl::Size number4 = 6_size;
		number4Ptr = &number4;
		ts.assertTrue(number4Ptr.hasValue());
		ts.assertEqual(&number4, number4Ptr.get());

		natl::OptionPtr<natl::Size> number5Ptr = nullptr;
		ts.assertFalse(number5Ptr.hasValue());

		natl::Size number5 = 5_size;
		number5Ptr = &number5;
		ts.assertTrue(number5Ptr.hasValue());
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(OptionTestSuite);

NATL_TESTS_MAIN();