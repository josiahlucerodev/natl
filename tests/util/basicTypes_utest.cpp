//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/basicTypes.h>

struct BasicTypesTestSuite : public natl::TestSuite<BasicTypesTestSuite> {};

template<typename test_suite_type = BasicTypesTestSuite>
struct BasicTypesTestSuiteTests {
	constexpr static void byteSizeTest(test_suite_type& ts) noexcept {
		ts.assertEqual(static_cast<natl::Size>(1), natl::TypeByteSize<natl::Bool>);
		ts.assertEqual(static_cast<natl::Size>(1), natl::TypeByteSize<natl::i8>);
		ts.assertEqual(static_cast<natl::Size>(2), natl::TypeByteSize<natl::i16>);
		ts.assertEqual(static_cast<natl::Size>(4), natl::TypeByteSize<natl::i32>);
		ts.assertEqual(static_cast<natl::Size>(8), natl::TypeByteSize<natl::i64>);
		ts.assertEqual(static_cast<natl::Size>(1), natl::TypeByteSize<natl::ui8>);
		ts.assertEqual(static_cast<natl::Size>(2), natl::TypeByteSize<natl::ui16>);
		ts.assertEqual(static_cast<natl::Size>(4), natl::TypeByteSize<natl::ui32>);
		ts.assertEqual(static_cast<natl::Size>(8), natl::TypeByteSize<natl::ui64>);
		ts.assertEqual(static_cast<natl::Size>(4), natl::TypeByteSize<natl::f32>);
		ts.assertEqual(static_cast<natl::Size>(8), natl::TypeByteSize<natl::f64>);
	}
	NATL_REGISTER_TEST_CONSTEXPR(byteSizeTest);

	constexpr static void bitSizeTest(test_suite_type& ts) noexcept {
		ts.assertEqual(static_cast<natl::Size>(1) * 8, natl::TypeBitSize<natl::Bool>);
		ts.assertEqual(static_cast<natl::Size>(1) * 8, natl::TypeBitSize<natl::i8>);
		ts.assertEqual(static_cast<natl::Size>(2) * 8, natl::TypeBitSize<natl::i16>);
		ts.assertEqual(static_cast<natl::Size>(4) * 8, natl::TypeBitSize<natl::i32>);
		ts.assertEqual(static_cast<natl::Size>(8) * 8, natl::TypeBitSize<natl::i64>);
		ts.assertEqual(static_cast<natl::Size>(1) * 8, natl::TypeBitSize<natl::ui8>);
		ts.assertEqual(static_cast<natl::Size>(2) * 8, natl::TypeBitSize<natl::ui16>);
		ts.assertEqual(static_cast<natl::Size>(4) * 8, natl::TypeBitSize<natl::ui32>);
		ts.assertEqual(static_cast<natl::Size>(8) * 8, natl::TypeBitSize<natl::ui64>);
		ts.assertEqual(static_cast<natl::Size>(4) * 8, natl::TypeBitSize<natl::f32>);
		ts.assertEqual(static_cast<natl::Size>(8) * 8, natl::TypeBitSize<natl::f64>);
	}
	NATL_REGISTER_TEST_CONSTEXPR(bitSizeTest);

};

NATL_REGISTER_STD_TESTS(BasicTypesTestSuite);

NATL_TESTS_MAIN();