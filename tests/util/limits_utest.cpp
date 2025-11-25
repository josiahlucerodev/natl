//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/any.h>
#include <natl/container/string.h>

struct LimitsTestSuite : public natl::TestSuite<LimitsTestSuite> {};

template<typename test_suite_type = LimitsTestSuite>
struct LimitsTestSuiteTests {
	constexpr static void minTest(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Limits<natl::i8>::min(), natl::i8(-128));
		ts.assertEqual(natl::Limits<natl::i16>::min(), natl::i16(-32768));
		ts.assertEqual(natl::Limits<natl::i32>::min(), natl::i32(-2147483648));
		ts.assertEqual(natl::Limits<natl::i64>::min(), natl::i64((-9223372036854775807L) - 1));
		ts.assertEqual(natl::Limits<natl::ui8>::min(), natl::ui8(0));
		ts.assertEqual(natl::Limits<natl::ui16>::min(), natl::ui16(0));
		ts.assertEqual(natl::Limits<natl::ui32>::min(), natl::ui32(0));
		ts.assertEqual(natl::Limits<natl::ui64>::min(), natl::ui64(0));
		ts.assertEqual(natl::Limits<natl::f32>::min(), natl::f32(-3.40282347e+38f));
		ts.assertEqual(natl::Limits<natl::f64>::min(), natl::f64(-1.7976931348623158e+308));
	}
	NATL_REGISTER_TEST_CONSTEXPR(minTest);

	constexpr static void sizeTest(test_suite_type& ts) noexcept {
		ts.assertEqual(sizeof(natl::Limits<natl::i8>::value_type), 1);
		ts.assertEqual(sizeof(natl::Limits<natl::i16>::value_type), 2);
		ts.assertEqual(sizeof(natl::Limits<natl::i32>::value_type), 4);
		ts.assertEqual(sizeof(natl::Limits<natl::i64>::value_type), 8);
		ts.assertEqual(sizeof(natl::Limits<natl::ui8>::value_type), 1);
		ts.assertEqual(sizeof(natl::Limits<natl::ui16>::value_type), 2);
		ts.assertEqual(sizeof(natl::Limits<natl::ui32>::value_type), 4);
		ts.assertEqual(sizeof(natl::Limits<natl::ui64>::value_type), 8);
		ts.assertEqual(sizeof(natl::Limits<natl::f32>::value_type), 4);
		ts.assertEqual(sizeof(natl::Limits<natl::f64>::value_type), 8);
	}
	NATL_REGISTER_TEST_CONSTEXPR(sizeTest);
};

NATL_REGISTER_STD_TESTS(LimitsTestSuite);

NATL_TESTS_MAIN();