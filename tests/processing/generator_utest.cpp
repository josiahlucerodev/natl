//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/processing/generator.h>

struct GeneratorTestSuite : public natl::TestSuite<GeneratorTestSuite> {};

template<typename test_suite_type = GeneratorTestSuite>
struct GeneratorTestSuiteTests {
	static natl::Generator<natl::Ascii> charExplode(const natl::String& string) noexcept {
		for (const natl::Ascii& character : string) {
			natl::Ascii characterStorage = character;
			co_yield characterStorage;
		}
	}

	constexpr static void tests(test_suite_type& ts) noexcept {
		natl::Size i = 0;
		for (const natl::Ascii& character : charExplode("01234")) {
			ts.assertEqual(i, natl::convertDecimalCharacterToNumber<natl::Size>(character));
			i++;
		}
		ts.assertEqual(i, 5);
	}
	NATL_REGISTER_TEST(tests);
};

NATL_REGISTER_STD_TESTS(GeneratorTestSuite);

NATL_TESTS_MAIN();