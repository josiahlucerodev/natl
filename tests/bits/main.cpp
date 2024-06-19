
//natl
#include <natl/util/bits.h>
#include <natl/util/test.h>
#include <natl/system/timer.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlBitsTest";

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericPopcountTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " popcount", natl::TestType::leaf);
	natl::testAssert(test, natl::popcount(NumericType(0)) == 0, "popcount 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		natl::testAssert(test, natl::popcount(natl::Limits<NumericType>::max()) == natl::TypeBitSize<NumericType>, "popcount max");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		natl::testAssert(test, natl::popcount(NumericType(1)) == NumericType(1), "popcount 1");
		natl::testAssert(test, natl::popcount(NumericType(2)) == NumericType(1), "popcount 2");
		natl::testAssert(test, natl::popcount(NumericType(4)) == NumericType(1), "popcount 4");
		natl::testAssert(test, natl::popcount(NumericType(8)) == NumericType(1), "popcount 8");
		natl::testAssert(test, natl::popcount(NumericType(16)) == NumericType(1), "popcount 16");

		natl::testAssert(test, natl::popcount(NumericType(3)) == NumericType(2), "popcount 3");
	}

	return test;
}

constexpr natl::Bool popcountTests() noexcept {
	natl::Test test(natlTestFrom, "popcount tests", natl::TestType::node);
	natl::subTestAssert(test, numericPopcountTests<natl::i8, "natl::i8">());
	natl::subTestAssert(test, numericPopcountTests<natl::i16, "natl::i16">());
	natl::subTestAssert(test, numericPopcountTests<natl::i32, "natl::i32">());
	natl::subTestAssert(test, numericPopcountTests<natl::i64, "natl::i64">());
	natl::subTestAssert(test, numericPopcountTests<natl::ui8, "natl::ui8">());
	natl::subTestAssert(test, numericPopcountTests<natl::ui16, "natl::ui16">());
	natl::subTestAssert(test, numericPopcountTests<natl::ui32, "natl::ui32">());
	natl::subTestAssert(test, numericPopcountTests<natl::ui64, "natl::ui64">());
	natl::subTestAssert(test, numericPopcountTests<natl::f32, "natl::f32">());
	natl::subTestAssert(test, numericPopcountTests<natl::f64, "natl::f64">());
	return test;
}

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericBitscanForwardTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " bitscan forward", natl::TestType::leaf);
	natl::testAssert(test, natl::bitscanForwardResult(NumericType(0)).result == false, "bitscan forward 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		natl::testAssert(test, natl::bitscanForwardResult<NumericType>(natl::Limits<NumericType>::max()).index == natl::TypeBitSize<NumericType> - 1, "bitscan forward max");
		natl::testAssert(test, natl::bitscanForwardResult<NumericType>(3).index == 1, "bitscan forward 3");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		natl::testAssert(test, natl::bitscanForwardResult<NumericType>(1).index == 0, "bitscan forward 1");
		natl::testAssert(test, natl::bitscanForwardResult<NumericType>(2).index == 1, "bitscan forward 2");
		natl::testAssert(test, natl::bitscanForwardResult<NumericType>(4).index == 2, "bitscan forward 4");
		natl::testAssert(test, natl::bitscanForwardResult<NumericType>(8).index == 3, "bitscan forward 8");
		natl::testAssert(test, natl::bitscanForwardResult<NumericType>(16).index == 4, "bitscan forward 16");
	}

	return test;
}

constexpr natl::Bool bitscanForwardTests() noexcept {
	natl::Test test(natlTestFrom, "bitscan forward tests", natl::TestType::node);
	natl::subTestAssert(test, numericBitscanForwardTests<natl::i8, "natl::i8">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::i16, "natl::i16">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::i32, "natl::i32">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::i64, "natl::i64">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::ui8, "natl::ui8">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::ui16, "natl::ui16">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::ui32, "natl::ui32">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::ui64, "natl::ui64">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::f32, "natl::f32">());
	natl::subTestAssert(test, numericBitscanForwardTests<natl::f64, "natl::f64">());
	return test;
}

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericBitscanBackwardTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " bitscan backward", natl::TestType::leaf);
	natl::testAssert(test, natl::bitscanBackwardResult(NumericType(0)).result == false, "bitscan backward 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		natl::testAssert(test, natl::bitscanBackwardResult<NumericType>(natl::Limits<NumericType>::max()).index == 0, "bitscan backward max");
		natl::testAssert(test, natl::bitscanBackwardResult<NumericType>(3).index == 0, "bitscan backward 3");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		natl::testAssert(test, natl::bitscanBackwardResult<NumericType>(1).index == 0, "bitscan backward 1");
		natl::testAssert(test, natl::bitscanBackwardResult<NumericType>(2).index == 1, "bitscan backward 2");
		natl::testAssert(test, natl::bitscanBackwardResult<NumericType>(4).index == 2, "bitscan backward 4");
		natl::testAssert(test, natl::bitscanBackwardResult<NumericType>(8).index == 3, "bitscan backward 8");
		natl::testAssert(test, natl::bitscanBackwardResult<NumericType>(16).index == 4, "bitscan backward 16");
	}

	return test;
}

constexpr natl::Bool bitscanBackwardTests() noexcept {
	natl::Test test(natlTestFrom, "bitscan backward tests", natl::TestType::node);
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::i8, "natl::i8">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::i16, "natl::i16">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::i32, "natl::i32">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::i64, "natl::i64">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::ui8, "natl::ui8">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::ui16, "natl::ui16">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::ui32, "natl::ui32">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::ui64, "natl::ui64">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::f32, "natl::f32">());
	natl::subTestAssert(test, numericBitscanBackwardTests<natl::f64, "natl::f64">());
	return test;
}

static_assert(popcountTests());
static_assert(bitscanBackwardTests());

natl::Bool tests() noexcept {
	natl::TestTimer timer(natlTestFrom);
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, popcountTests());
	natl::subTestAssert(test, bitscanForwardTests());
	natl::subTestAssert(test, bitscanBackwardTests());
	return test;
}


int main() {
	tests();
}