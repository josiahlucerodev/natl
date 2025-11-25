//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/system/print.h>
#include <natl/system/printFormatted.h>
#include <natl/system/printColor.h>

struct PrintTestSuite : public natl::TestSuite<PrintTestSuite> {};

template<typename test_suite_type = PrintTestSuite>
struct PrintTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		natl::enablePrintExtendedColor();
		natl::println(natl::getTypeInfo<natl::Size>().name());
		natl::printlnf(false);
		natl::printf(natl::formatArgText<"b">(natl::PrintColor::red), true, natl::PrintAllDefaultColor{});
		natl::printlnf(false);
		natl::printlnc("the", 3, natl::PrintBackgroundColor::red);
		natl::printlnc(natl::getTypeInfo<natl::Size>().name(), natl::PrintBackgroundColor::red);
		natl::printlnfc(natl::PrintExtendedForegroundColor(128, 128, 0), "the");
		ts.assertFalse(natl::sformat(natl::formatArgText<"b">(natl::PrintColor::red)).isEmpty());
	}
	NATL_REGISTER_TEST(tests);
};

NATL_REGISTER_STD_TESTS(PrintTestSuite);

NATL_TESTS_MAIN();
