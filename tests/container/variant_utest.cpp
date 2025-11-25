//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/container/variant.h>
#include <natl/container/dynArray.h>
#include <natl/util/test.h>

struct VariantTestSuite : public natl::TestSuite<VariantTestSuite> {};

template<typename test_suite_type = VariantTestSuite>
struct VariantTestSuiteTests {
	constexpr static void tests(test_suite_type& ts) noexcept {
		using VariantTestType = natl::Variant<
			natl::NamedElement<"i8", natl::i8>,
			natl::NamedElement<"i64", natl::i64>,
			natl::NamedElement<"ui8", natl::ui8>,
			natl::NamedElement<"ui64", natl::ui64>,
			natl::NamedElement<"f32", natl::f32>,
			natl::NamedElement<"f64", natl::f64>
		>;

		VariantTestType value;

		value.assign<"i8">(natl::Limits<natl::i8>::min());
		ts.assertEqual(natl::Limits<natl::i8>::min(), value.get<"i8">());
		value.assign<"i8">(natl::Limits<natl::i8>::max());
		ts.assertEqual(natl::Limits<natl::i8>::max(), value.get<"i8">());

		value.assign<"i64">(natl::Limits<natl::i64>::min());
		ts.assertEqual(natl::Limits<natl::i64>::min(), value.get<"i64">());
		value.assign<"i64">(natl::Limits<natl::i64>::max());
		ts.assertEqual(natl::Limits<natl::i64>::max(), value.get<"i64">());

		value.assign<"ui8">(natl::Limits<natl::ui8>::min());
		ts.assertEqual(natl::Limits<natl::ui8>::min(), value.get<"ui8">());
		value.assign<"ui8">(natl::Limits<natl::ui8>::max());
		ts.assertEqual(natl::Limits<natl::ui8>::max(), value.get<"ui8">());

		value.assign<"ui64">(natl::Limits<natl::ui64>::min());
		ts.assertEqual(natl::Limits<natl::ui64>::min(), value.get<"ui64">());
		value.assign<"ui64">(natl::Limits<natl::ui64>::max());
		ts.assertEqual(natl::Limits<natl::ui64>::max(), value.get<"ui64">());

		value.assign<"f32">(natl::Limits<natl::f32>::min());
		ts.assertEqual(natl::Limits<natl::f32>::min(), value.get<"f32">());
		value.assign<"f32">(natl::Limits<natl::f32>::max());
		ts.assertEqual(natl::Limits<natl::f32>::max(), value.get<"f32">());

		value.assign<"f64">(natl::Limits<natl::f64>::min());
		ts.assertEqual(natl::Limits<natl::f64>::min(), value.get<"f64">());
		value.assign<"f64">(natl::Limits<natl::f64>::max());
		ts.assertEqual(natl::Limits<natl::f64>::max(), value.get<"f64">());
	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(VariantTestSuite);

NATL_TESTS_MAIN();
