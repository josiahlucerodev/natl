//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/util/stringConvert.h>
#include <natl/math/basicFloatOperations.h>

struct StringConvertTestSuite : public natl::TestSuite<StringConvertTestSuite> {};

template<typename test_suite_type = StringConvertTestSuite>
struct StringConvertTestSuiteTests {
	template<typename IntTests>
	constexpr static void testWithAllInts(test_suite_type& ts) noexcept {
		IntTests::template test<natl::i8>(ts, "natl::i8");
		IntTests::template test<natl::i16>(ts, "natl::i16");
		IntTests::template test<natl::i32>(ts, "natl::i32");
		IntTests::template test<natl::i64>(ts, "natl::i64");
		IntTests::template test<natl::ui8>(ts, "natl::ui8");
		IntTests::template test<natl::ui16>(ts, "natl::ui16");
		IntTests::template test<natl::ui32>(ts, "natl::ui32");
		IntTests::template test<natl::ui64>(ts, "natl::ui64");
	}

	struct IntToStringDecimalTest {
		template<typename Integer>
		constexpr static void test(test_suite_type& ts, const natl::ConstAsciiStringView& intName) noexcept {
			ts.trace(intName);
			ts.assertEqual("0", natl::intToStringDecimal<Integer>(0));
			ts.assertEqual("10", natl::intToStringDecimal<Integer>(10));
			ts.assertEqual("100", natl::intToStringDecimal<Integer>(100));

			if constexpr (natl::IsBuiltInSignedIntegerC<natl::Decay<Integer>>) {
				ts.assertEqual("-10", natl::intToStringDecimal<Integer>(-10));
				ts.assertEqual("-100", natl::intToStringDecimal<Integer>(-100));
			}
		}
	};

	constexpr static void intToStringDecimalTest(test_suite_type& ts) noexcept {
		testWithAllInts<IntToStringDecimalTest>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(intToStringDecimalTest);

	struct IntToStringBinaryTest {
		template<typename Integer>
		constexpr static void test(test_suite_type& ts, const natl::ConstAsciiStringView& intName) noexcept {
			ts.trace(intName);
			ts.assertEqual("0", natl::intToStringBinary<Integer>(0));
			ts.assertEqual("10", natl::intToStringBinary<Integer>(2));
			ts.assertEqual("100", natl::intToStringBinary<Integer>(4));
			ts.assertEqual("1000", natl::intToStringBinary<Integer>(8));
			ts.assertEqual("10000", natl::intToStringBinary<Integer>(16));
			ts.assertEqual("100000", natl::intToStringBinary<Integer>(32));
			ts.assertEqual("1000000", natl::intToStringBinary<Integer>(64));
		}
	};

	constexpr static void intToStringBinaryTest(test_suite_type& ts) noexcept {
		testWithAllInts<IntToStringBinaryTest>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(intToStringBinaryTest);

	struct IntToStringHexadecimalTest {
		template<typename Integer>
		constexpr static void test(test_suite_type& ts, const natl::ConstAsciiStringView& intName) noexcept {
			ts.trace(intName);
			ts.assertEqual("0x0", natl::intToStringHexadecimal<Integer>(0));
			ts.assertEqual("0x2", natl::intToStringHexadecimal<Integer>(2));
			ts.assertEqual("0x4", natl::intToStringHexadecimal<Integer>(4));
			ts.assertEqual("0x8", natl::intToStringHexadecimal<Integer>(8));
			ts.assertEqual("0x10", natl::intToStringHexadecimal<Integer>(16));
			ts.assertEqual("0x20", natl::intToStringHexadecimal<Integer>(32));
			ts.assertEqual("0x40", natl::intToStringHexadecimal<Integer>(64));
		}
	};

	constexpr static void intToStringHexadecimalTest(test_suite_type& ts) noexcept {
		testWithAllInts<IntToStringHexadecimalTest>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(intToStringHexadecimalTest);

	constexpr static void floatToStringDecimalTest(test_suite_type& ts) noexcept {
		using namespace natl::literals;

		ts.assertEqual("0.0", natl::floatToStringDecimal<natl::f32>(0.0_f32, 1_size));
		ts.assertEqual("1.0", natl::floatToStringDecimal<natl::f32>(1.0_f32, 1_size));
		ts.assertEqual("1.5", natl::floatToStringDecimal<natl::f32>(1.5_f32, 1_size));

		ts.assertEqual("0.0", natl::floatToStringDecimal<natl::f64>(0.0_f64, 1_size));
		ts.assertEqual("1.0", natl::floatToStringDecimal<natl::f64>(1.0_f64, 1_size));
		ts.assertEqual("1.5", natl::floatToStringDecimal<natl::f64>(1.5_f64, 1_size));
	}
	NATL_REGISTER_TEST_CONSTEXPR(floatToStringDecimalTest);

	struct StringDecimalToIntTest {
		template<typename Integer>
		constexpr static void test(test_suite_type& ts, const natl::ConstAsciiStringView& intName) noexcept {
			ts.trace(intName);
			ts.assertEqual(Integer(0), natl::stringDecimalToInt<Integer>("0").value());
			ts.assertEqual(Integer(2), natl::stringDecimalToInt<Integer>("2").value());
			ts.assertEqual(Integer(4), natl::stringDecimalToInt<Integer>("4").value());
			ts.assertEqual(Integer(8), natl::stringDecimalToInt<Integer>("8").value());
			ts.assertEqual(Integer(16), natl::stringDecimalToInt<Integer>("16").value());
			ts.assertEqual(Integer(32), natl::stringDecimalToInt<Integer>("32").value());
			ts.assertEqual(Integer(64), natl::stringDecimalToInt<Integer>("64").value());

			if constexpr (natl::IsBuiltInSignedIntegerC<natl::Decay<Integer>>) {
				ts.assertEqual(Integer(-1), natl::stringDecimalToInt<Integer>("-1").value());
				ts.assertEqual(Integer(-2), natl::stringDecimalToInt<Integer>("-2").value());
				ts.assertEqual(Integer(-4), natl::stringDecimalToInt<Integer>("-4").value());
				ts.assertEqual(Integer(-8), natl::stringDecimalToInt<Integer>("-8").value());
				//ts.assertEqual(Integer(-16), natl::stringDecimalToInt<Integer>("-16").value()); MSVC Internal compiler error
				ts.assertEqual(Integer(-32), natl::stringDecimalToInt<Integer>("-32").value());
				ts.assertEqual(Integer(-64), natl::stringDecimalToInt<Integer>("-64").value());
			}
		}
	};
	constexpr static void stringDecimalToIntTest(test_suite_type& ts) noexcept {
		testWithAllInts<StringDecimalToIntTest>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(stringDecimalToIntTest);

	struct StringBinaryToIntTest {
		template<typename Integer>
		constexpr static void test(test_suite_type& ts, const natl::ConstAsciiStringView& intName) noexcept {
			ts.trace(intName);
			ts.assertEqual(Integer(0), natl::stringBinaryToInt<Integer>("0b0").value());
			ts.assertEqual(Integer(2), natl::stringBinaryToInt<Integer>("0b10").value());
			ts.assertEqual(Integer(4), natl::stringBinaryToInt<Integer>("0b100").value());
			ts.assertEqual(Integer(8), natl::stringBinaryToInt<Integer>("0b1000").value());
			ts.assertEqual(Integer(16), natl::stringBinaryToInt<Integer>("0b10000").value());
			ts.assertEqual(Integer(32), natl::stringBinaryToInt<Integer>("0b100000").value());
			ts.assertEqual(Integer(64), natl::stringBinaryToInt<Integer>("0b1000000").value());
			ts.assertEqual(Integer(10), natl::stringBinaryToInt<Integer>("0b1010").value());

			if constexpr (natl::IsBuiltInSignedIntegerC<natl::Decay<Integer>>) {
				ts.assertEqual(Integer(-1), natl::stringBinaryToInt<Integer>("-0b1").value());
				ts.assertEqual(Integer(-2), natl::stringBinaryToInt<Integer>("-0b10").value());
				ts.assertEqual(Integer(-4), natl::stringBinaryToInt<Integer>("-0b100").value());
				ts.assertEqual(Integer(-8), natl::stringBinaryToInt<Integer>("-0b1000").value());
				ts.assertEqual(Integer(-16), natl::stringBinaryToInt<Integer>("-0b10000").value());
				ts.assertEqual(Integer(-32), natl::stringBinaryToInt<Integer>("-0b100000").value());
				ts.assertEqual(Integer(-64), natl::stringBinaryToInt<Integer>("-0b1000000").value());
				ts.assertEqual(Integer(-10), natl::stringBinaryToInt<Integer>("-0b1010").value());
			}
		}
	};

	constexpr static void stringBinaryToIntTest(test_suite_type& ts) noexcept {
		testWithAllInts<StringBinaryToIntTest>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(stringBinaryToIntTest);

	struct StringHexadecimalToIntTest {
		template<typename Integer>
		constexpr static void test(test_suite_type& ts, const natl::ConstAsciiStringView& intName) noexcept {
			ts.trace(intName);
			ts.assertEqual(Integer(0), natl::stringHexadecimalToInt<Integer>("0x0").value());
			ts.assertEqual(Integer(2), natl::stringHexadecimalToInt<Integer>("0x2").value());
			ts.assertEqual(Integer(4), natl::stringHexadecimalToInt<Integer>("0x4").value());
			ts.assertEqual(Integer(8), natl::stringHexadecimalToInt<Integer>("0x8").value());
			ts.assertEqual(Integer(16), natl::stringHexadecimalToInt<Integer>("0x10").value());
			ts.assertEqual(Integer(32), natl::stringHexadecimalToInt<Integer>("0x20").value());
			ts.assertEqual(Integer(64), natl::stringHexadecimalToInt<Integer>("0x40").value());

			if constexpr (natl::IsBuiltInSignedIntegerC<natl::Decay<Integer>>) {
				ts.assertEqual(Integer(-1), natl::stringHexadecimalToInt<Integer>("-0x1").value());
				ts.assertEqual(Integer(-2), natl::stringHexadecimalToInt<Integer>("-0x2").value());
				ts.assertEqual(Integer(-4), natl::stringHexadecimalToInt<Integer>("-0x4").value());
				ts.assertEqual(Integer(-8), natl::stringHexadecimalToInt<Integer>("-0x8").value());
				ts.assertEqual(Integer(-16), natl::stringHexadecimalToInt<Integer>("-0x10").value());
				ts.assertEqual(Integer(-32), natl::stringHexadecimalToInt<Integer>("-0x20").value());
				ts.assertEqual(Integer(-64), natl::stringHexadecimalToInt<Integer>("-0x40").value());
			}
		}
	};

	constexpr static void stringHexadecimalToIntTest(test_suite_type& ts) noexcept {
		testWithAllInts<StringHexadecimalToIntTest>(ts);
	}
	NATL_REGISTER_TEST_CONSTEXPR(stringHexadecimalToIntTest);

	constexpr static void stringDecimalToFloatTest(test_suite_type& ts) noexcept {
		using namespace natl::literals;

		ts.assertTrue(natl::math::isRoughtlyEqualF32(0.0_f32, natl::stringDecimalToFloat<natl::f32>("0.0").value(), 0.001_f32));
		ts.assertTrue(natl::math::isRoughtlyEqualF32(1.0_f32, natl::stringDecimalToFloat<natl::f32>("1.0").value(), 0.001_f32));
		ts.assertTrue(natl::math::isRoughtlyEqualF32(1.5_f32, natl::stringDecimalToFloat<natl::f32>("1.5").value(), 0.001_f32));
		ts.assertTrue(natl::math::isRoughtlyEqualF32(1.2345_f32, natl::stringDecimalToFloat<natl::f32>("1.2345").value(), 0.0001_f32));

		ts.assertTrue(natl::math::isRoughtlyEqualF64(0.0_f64, natl::stringDecimalToFloat<natl::f64>("0.0").value(), 0.001_f64));
		ts.assertTrue(natl::math::isRoughtlyEqualF64(1.0_f64, natl::stringDecimalToFloat<natl::f64>("1.0").value(), 0.001_f64));
		ts.assertTrue(natl::math::isRoughtlyEqualF64(1.5_f64, natl::stringDecimalToFloat<natl::f64>("1.5").value(), 0.001_f64));
		ts.assertTrue(natl::math::isRoughtlyEqualF64(1.2345_f64, natl::stringDecimalToFloat<natl::f64>("1.2345").value(), 0.0001_f64));
	}
	NATL_REGISTER_TEST_CONSTEXPR(stringDecimalToFloatTest);
};

NATL_REGISTER_STD_TESTS(StringConvertTestSuite);

NATL_TESTS_MAIN();