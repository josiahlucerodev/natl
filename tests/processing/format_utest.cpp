//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/processing/format.h>
#include <natl/container/string.h>

struct FormatTestSuite : public natl::TestSuite<FormatTestSuite> {};

template<typename test_suite_type = FormatTestSuite>
struct FormatTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		//boolean format
		ts.assertEqual("true", natl::sformat(true));
		ts.assertEqual("false", natl::sformat(false));
		ts.assertEqual("True", natl::sformat(natl::formatArgText<"Full">(true)));
		ts.assertEqual("False", natl::sformat(natl::formatArgText<"Full">(false)));
		ts.assertEqual("TRUE", natl::sformat(natl::formatArgText<"FULL">(true)));
		ts.assertEqual("FALSE", natl::sformat(natl::formatArgText<"FULL">(false)));
		ts.assertEqual("t", natl::sformat(natl::formatArgText<"shorthand">(true)));
		ts.assertEqual("f", natl::sformat(natl::formatArgText<"shorthand">(false)));
		ts.assertEqual("T", natl::sformat(natl::formatArgText<"Shorthand">(true)));
		ts.assertEqual("F", natl::sformat(natl::formatArgText<"Shorthand">(false)));

		//string literal format
		ts.assertEqual("Hello", natl::sformat("Hello"));

		//string view fromat
		ts.assertEqual("Hello", natl::sformat(natl::ConstAsciiStringView("Hello")));
		ts.assertEqual("Hello", natl::sformat(natl::ConstAsciiStringView("Hello")));

		//string format
		ts.assertEqual("Hello", natl::sformat(natl::String("Hello")));

		//int format 
		ts.assertEqual("1010", natl::sformat(natl::formatArgText<"binary">(10)));
		ts.assertEqual("10", natl::sformat(natl::formatArgText<"decimal">(10)));
		ts.assertEqual("0xA", natl::sformat(natl::formatArgText<"hexadecimal">(10)));

		//float fromat 
		ts.assertEqual("1.23", natl::sformat(natl::formatArgText<"standard", "p: 2">(1.234)));
	}
	NATL_REGISTER_TEST(tests);
};

NATL_REGISTER_STD_TESTS(FormatTestSuite);

NATL_TESTS_MAIN();