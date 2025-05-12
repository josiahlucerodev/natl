
//natl
#include <natl/util/stringConvert.h>
#include <natl/math/basicFloatOperations.h>
#include <natl/util/test.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlStringConvertTest";

template<typename IntTests>
constexpr natl::Bool testWithAllInts(const natl::ConstAsciiStringView& testName) noexcept {
	natl::Test test(natlTestFrom, testName, natl::TestType::node);
	natl::subTestAssert(test, IntTests::template test<natl::i8>("natl::i8"));
	natl::subTestAssert(test, IntTests::template test<natl::i16>("natl::i16"));
	natl::subTestAssert(test, IntTests::template test<natl::i32>("natl::i32"));
	natl::subTestAssert(test, IntTests::template test<natl::i64>("natl::i64"));
	natl::subTestAssert(test, IntTests::template test<natl::ui8>("natl::ui8"));
	natl::subTestAssert(test, IntTests::template test<natl::ui16>("natl::ui16"));
	natl::subTestAssert(test, IntTests::template test<natl::ui32>("natl::ui32"));
	natl::subTestAssert(test, IntTests::template test<natl::ui64>("natl::ui64"));
	return test;
}

struct IntToStringDecimalTest {
	template<typename Integer>
	constexpr static natl::Bool test(const natl::ConstAsciiStringView& intName) noexcept {
		natl::Test test(natlTestFrom, intName, natl::TestType::leaf);
		natl::testAssertEquals(test, "0", natl::intToStringDecimal<Integer>(0));
		natl::testAssertEquals(test, "10", natl::intToStringDecimal<Integer>(10));
		natl::testAssertEquals(test, "100", natl::intToStringDecimal<Integer>(100));

		if constexpr (natl::IsBuiltInSignedIntegerC<natl::Decay<Integer>>) {
			natl::testAssertEquals(test, "-10", natl::intToStringDecimal<Integer>(-10));
			natl::testAssertEquals(test, "-100", natl::intToStringDecimal<Integer>(-100));
		}

		return test;
	}
};

constexpr natl::Bool intToStringDecimalTests() noexcept {
	return testWithAllInts<IntToStringDecimalTest>("int to string decimal test");
}
static_assert(intToStringDecimalTests());

struct IntToStringBinaryTest {
	template<typename Integer>
	constexpr static natl::Bool test(const natl::ConstAsciiStringView& intName) noexcept {
		natl::Test test(natlTestFrom, intName, natl::TestType::leaf);
		natl::testAssertEquals(test, "0", natl::intToStringBinary<Integer>(0));
		natl::testAssertEquals(test, "10", natl::intToStringBinary<Integer>(2));
		natl::testAssertEquals(test, "100", natl::intToStringBinary<Integer>(4));
		natl::testAssertEquals(test, "1000", natl::intToStringBinary<Integer>(8));
		natl::testAssertEquals(test, "10000", natl::intToStringBinary<Integer>(16));
		natl::testAssertEquals(test, "100000", natl::intToStringBinary<Integer>(32));
		natl::testAssertEquals(test, "1000000", natl::intToStringBinary<Integer>(64));
		return test;
	}
};

constexpr natl::Bool intToStringBinaryTests() noexcept {
	return testWithAllInts<IntToStringBinaryTest>("int to string binary test");
}
static_assert(intToStringBinaryTests());

struct IntToStringHexadecimalTest {
	template<typename Integer>
	constexpr static natl::Bool test(const natl::ConstAsciiStringView& intName) noexcept {
		natl::Test test(natlTestFrom, intName, natl::TestType::leaf);
		natl::testAssertEquals(test, "0x0", natl::intToStringHexadecimal<Integer>(0));
		natl::testAssertEquals(test, "0x2", natl::intToStringHexadecimal<Integer>(2));
		natl::testAssertEquals(test, "0x4", natl::intToStringHexadecimal<Integer>(4));
		natl::testAssertEquals(test, "0x8", natl::intToStringHexadecimal<Integer>(8));
		natl::testAssertEquals(test, "0x10", natl::intToStringHexadecimal<Integer>(16));
		natl::testAssertEquals(test, "0x20", natl::intToStringHexadecimal<Integer>(32));
		natl::testAssertEquals(test, "0x40", natl::intToStringHexadecimal<Integer>(64));
		return test;
	}
};

constexpr natl::Bool intToStringHexadecimalTests() noexcept {
	return testWithAllInts<IntToStringHexadecimalTest>("int to string hexadecimal test");
}
static_assert(intToStringHexadecimalTests());

constexpr natl::Bool floatToStringDecimalTests() noexcept {
	using namespace natl::literals;

	natl::Test test(natlTestFrom, "float to string decimal tests", natl::TestType::leaf);
	
	natl::testAssertEquals(test, "0.0", natl::floatToStringDecimal<natl::f32>(0.0, 1));
	natl::testAssertEquals(test, "1.0", natl::floatToStringDecimal<natl::f32>(1.0, 1));
	natl::testAssertEquals(test, "1.5", natl::floatToStringDecimal<natl::f32>(1.5, 1));

	natl::testAssertEquals(test, "0.0", natl::floatToStringDecimal<natl::f64>(0.0, 1));
	natl::testAssertEquals(test, "1.0", natl::floatToStringDecimal<natl::f64>(1.0, 1));
	natl::testAssertEquals(test, "1.5", natl::floatToStringDecimal<natl::f64>(1.5, 1));

	return test;
}
static_assert(floatToStringDecimalTests());

struct StringDecimalToIntTest {
	template<typename Integer>
	constexpr static natl::Bool test(const natl::ConstAsciiStringView& intName) noexcept {
		natl::Test test(natlTestFrom, intName, natl::TestType::leaf);
		natl::testAssertEquals(test, 0, natl::stringDecimalToInt<Integer>("0").value());
		natl::testAssertEquals(test, 2, natl::stringDecimalToInt<Integer>("2").value());
		natl::testAssertEquals(test, 4, natl::stringDecimalToInt<Integer>("4").value());
		natl::testAssertEquals(test, 8, natl::stringDecimalToInt<Integer>("8").value());
		natl::testAssertEquals(test, 16, natl::stringDecimalToInt<Integer>("16").value());
		natl::testAssertEquals(test, 32, natl::stringDecimalToInt<Integer>("32").value());
		natl::testAssertEquals(test, 64, natl::stringDecimalToInt<Integer>("64").value());

		if constexpr (natl::IsBuiltInSignedIntegerC<natl::Decay<Integer>>) {
			natl::testAssertEquals(test, -1, natl::stringDecimalToInt<Integer>("-1").value());
			natl::testAssertEquals(test, -2, natl::stringDecimalToInt<Integer>("-2").value());
			natl::testAssertEquals(test, -4, natl::stringDecimalToInt<Integer>("-4").value());
			natl::testAssertEquals(test, -8, natl::stringDecimalToInt<Integer>("-8").value());
			natl::testAssertEquals(test, -16, natl::stringDecimalToInt<Integer>("-16").value());
			natl::testAssertEquals(test, -32, natl::stringDecimalToInt<Integer>("-32").value());
			natl::testAssertEquals(test, -64, natl::stringDecimalToInt<Integer>("-64").value());
		}

		return test;
	}
};

constexpr natl::Bool stringDecimalToIntTests() noexcept {
	return testWithAllInts<StringDecimalToIntTest>("string decimal to int tests");
}
//static_assert(stringDecimalToIntTests());

struct StringBinaryToIntTest {
	template<typename Integer>
	constexpr static natl::Bool test(const natl::ConstAsciiStringView& intName) noexcept {
		natl::Test test(natlTestFrom, intName, natl::TestType::leaf);
		natl::testAssertEquals(test, 0, natl::stringBinaryToInt<Integer>("0b0").value());
		natl::testAssertEquals(test, 2, natl::stringBinaryToInt<Integer>("0b10").value());
		natl::testAssertEquals(test, 4, natl::stringBinaryToInt<Integer>("0b100").value());
		natl::testAssertEquals(test, 8, natl::stringBinaryToInt<Integer>("0b1000").value());
		natl::testAssertEquals(test, 16, natl::stringBinaryToInt<Integer>("0b10000").value());
		natl::testAssertEquals(test, 32, natl::stringBinaryToInt<Integer>("0b100000").value());
		natl::testAssertEquals(test, 64, natl::stringBinaryToInt<Integer>("0b1000000").value());
		natl::testAssertEquals(test, 10, natl::stringBinaryToInt<Integer>("0b1010").value());

		if constexpr (natl::IsBuiltInSignedIntegerC<natl::Decay<Integer>>) {
			natl::testAssertEquals(test, -1, natl::stringBinaryToInt<Integer>("-0b1").value());
			natl::testAssertEquals(test, -2, natl::stringBinaryToInt<Integer>("-0b10").value());
			natl::testAssertEquals(test, -4, natl::stringBinaryToInt<Integer>("-0b100").value());
			natl::testAssertEquals(test, -8, natl::stringBinaryToInt<Integer>("-0b1000").value());
			natl::testAssertEquals(test, -16, natl::stringBinaryToInt<Integer>("-0b10000").value());
			natl::testAssertEquals(test, -32, natl::stringBinaryToInt<Integer>("-0b100000").value());
			natl::testAssertEquals(test, -64, natl::stringBinaryToInt<Integer>("-0b1000000").value());
			natl::testAssertEquals(test, -10, natl::stringBinaryToInt<Integer>("-0b1010").value());
		}

		return test;
	}
};

constexpr natl::Bool stringBinaryToIntTests() noexcept {
	return testWithAllInts<StringBinaryToIntTest>("string binary to int tests");
}
static_assert(stringBinaryToIntTests());

struct StringHexadecimalToIntTest {
	template<typename Integer>
	constexpr static natl::Bool test(const natl::ConstAsciiStringView& intName) noexcept {
		natl::Test test(natlTestFrom, intName, natl::TestType::leaf);
		natl::testAssertEquals(test, 0, natl::stringHexadecimalToInt<Integer>("0x0").value());
		natl::testAssertEquals(test, 2, natl::stringHexadecimalToInt<Integer>("0x2").value());
		natl::testAssertEquals(test, 4, natl::stringHexadecimalToInt<Integer>("0x4").value());
		natl::testAssertEquals(test, 8, natl::stringHexadecimalToInt<Integer>("0x8").value());
		natl::testAssertEquals(test, 16, natl::stringHexadecimalToInt<Integer>("0x10").value());
		natl::testAssertEquals(test, 32, natl::stringHexadecimalToInt<Integer>("0x20").value());
		natl::testAssertEquals(test, 64, natl::stringHexadecimalToInt<Integer>("0x40").value());

		if constexpr (natl::IsBuiltInSignedIntegerC<natl::Decay<Integer>>) {
			natl::testAssertEquals(test, -1, natl::stringHexadecimalToInt<Integer>("-0x1").value());
			natl::testAssertEquals(test, -2, natl::stringHexadecimalToInt<Integer>("-0x2").value());
			natl::testAssertEquals(test, -4, natl::stringHexadecimalToInt<Integer>("-0x4").value());
			natl::testAssertEquals(test, -8, natl::stringHexadecimalToInt<Integer>("-0x8").value());
			natl::testAssertEquals(test, -16, natl::stringHexadecimalToInt<Integer>("-0x10").value());
			natl::testAssertEquals(test, -32, natl::stringHexadecimalToInt<Integer>("-0x20").value());
			natl::testAssertEquals(test, -64, natl::stringHexadecimalToInt<Integer>("-0x40").value());
		}

		return test;
	}
};

constexpr natl::Bool stringHexadecimalToIntTests() noexcept {
	return testWithAllInts<StringHexadecimalToIntTest>("string hexadecimal to int tests");
}
static_assert(stringHexadecimalToIntTests());

constexpr natl::Bool stringDecimalToFloatTest() noexcept {
	using namespace natl::literals;

	natl::Test test(natlTestFrom, "string decimal to float tests", natl::TestType::leaf);

	natl::testAssert(test, natl::math::isRoughtlyEqualF32(0.0_f32, natl::stringDecimalToFloat<natl::f32>("0.0").value(), 0.001_f32));
	natl::testAssert(test, natl::math::isRoughtlyEqualF32(1.0_f32, natl::stringDecimalToFloat<natl::f32>("1.0").value(), 0.001_f32));
	natl::testAssert(test, natl::math::isRoughtlyEqualF32(1.5_f32, natl::stringDecimalToFloat<natl::f32>("1.5").value(), 0.001_f32));
	natl::testAssert(test, natl::math::isRoughtlyEqualF32(1.2345_f32, natl::stringDecimalToFloat<natl::f32>("1.2345").value(), 0.0001_f32));

	natl::testAssert(test, natl::math::isRoughtlyEqualF64(0.0_f64, natl::stringDecimalToFloat<natl::f64>("0.0").value(), 0.001_f64));
	natl::testAssert(test, natl::math::isRoughtlyEqualF64(1.0_f64, natl::stringDecimalToFloat<natl::f64>("1.0").value(), 0.001_f64));
	natl::testAssert(test, natl::math::isRoughtlyEqualF64(1.5_f64, natl::stringDecimalToFloat<natl::f64>("1.5").value(), 0.001_f64));
	natl::testAssert(test, natl::math::isRoughtlyEqualF64(1.2345_f64, natl::stringDecimalToFloat<natl::f64>("1.2345").value(), 0.0001_f64));

	return test;
}
static_assert(stringDecimalToFloatTest());

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, intToStringDecimalTests());
	natl::subTestAssert(test, intToStringBinaryTests());
	natl::subTestAssert(test, intToStringHexadecimalTests());
	natl::subTestAssert(test, floatToStringDecimalTests());

	natl::subTestAssert(test, stringDecimalToIntTests());
	natl::subTestAssert(test, stringBinaryToIntTests());
	natl::subTestAssert(test, stringHexadecimalToIntTests());
	natl::subTestAssert(test, stringDecimalToFloatTest());
	return test;
}


int main() {
	tests();
	//static_assert(natl::stringDecimalToInt<natl::i64>(natl::intToStringDecimal(32)).value() == 32);
	//static_assert(natl::stringBinaryToInt<natl::ui64>(natl::intToStringBinary(32)).value() == 32);
	//static_assert(natl::stringDecimalToInt<natl::i64>(natl::intToStringDecimal(32)).value() == 32);
	//static_assert(natl::stringDecimalToInt<natl::i64>("32").value() == 32);
	//static_assert(natl::math::isRoughtlyEqual<natl::f32>(natl::stringDecimalToFloat<natl::f32>("32.2").value(), 32.2f, 0.001f));
}