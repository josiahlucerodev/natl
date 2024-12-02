
//natl
#include <natl/util/bits.h>
#include <natl/util/test.h>
#include <natl/system/timer.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlBitsTest";

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericPopcountTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " popcount", natl::TestType::leaf);
	natl::testAssertEquals(test, NumericType(0), NumericType(natl::popcount(NumericType(0))), "popcount 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		natl::testAssertEquals(test, natl::TypeBitSize<NumericType>, natl::popcount(natl::Limits<NumericType>::max()), "popcount max");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		natl::testAssertEquals(test, NumericType(1), natl::popcount(NumericType(1)), "popcount 1");
		natl::testAssertEquals(test, NumericType(1), natl::popcount(NumericType(2)), "popcount 2");
		natl::testAssertEquals(test, NumericType(1), natl::popcount(NumericType(4)), "popcount 4");
		natl::testAssertEquals(test, NumericType(1), natl::popcount(NumericType(8)), "popcount 8");
		natl::testAssertEquals(test, NumericType(1), natl::popcount(NumericType(16)), "popcount 16");
		natl::testAssertEquals(test, NumericType(2), natl::popcount(NumericType(3)), "popcount 3");
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
static_assert(popcountTests());

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericBitscanForwardTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " bitscan forward", natl::TestType::leaf);
	natl::testAssertEquals(test, false, natl::bitscanForwardResult(NumericType(0)).result, "bitscan forward 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		natl::testAssertEquals(test, NumericType(0),
			natl::bitscanForwardResult<NumericType>(natl::Limits<NumericType>::max()).index, "bitscan forward max");
		natl::testAssertEquals(test, NumericType(0),
			natl::bitscanForwardResult<NumericType>(NumericType(3)).index, "bitscan forward 3");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		using uint_type = natl::UIntOfByteSize<natl::TypeByteSize<NumericType>>;
		constexpr NumericType addMask = natl::bitCast<NumericType, uint_type>(uint_type(1) << (uint_type(natl::TypeBitSize<NumericType>) - uint_type(1)));

		natl::testAssertEquals(test, NumericType(0), 
			natl::bitscanForwardResult<NumericType>(NumericType(1) | addMask).index, "bitscan forward 1");
		natl::testAssertEquals(test, NumericType(1), 
			natl::bitscanForwardResult<NumericType>(NumericType(2) | addMask).index, "bitscan forward 2");
		natl::testAssertEquals(test, NumericType(2), 
			natl::bitscanForwardResult<NumericType>(NumericType(4) | addMask).index, "bitscan forward 4");
		natl::testAssertEquals(test, NumericType(3), 
			natl::bitscanForwardResult<NumericType>(NumericType(8) | addMask).index, "bitscan forward 8");
		natl::testAssertEquals(test, NumericType(4), 
			natl::bitscanForwardResult<NumericType>(NumericType(16) | addMask).index, "bitscan forward 16");
		natl::testAssertEquals(test, NumericType(5), 
			natl::bitscanForwardResult<NumericType>(NumericType(32) | addMask).index, "bitscan forward 32");
		natl::testAssertEquals(test, NumericType(6), 
			natl::bitscanForwardResult<NumericType>(NumericType(64) | addMask).index, "bitscan forward 64");
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
static_assert(bitscanForwardTests());

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericBitscanBackwardTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " bitscan backward", natl::TestType::leaf);
	natl::testAssertEquals(test, false, natl::bitscanBackwardResult(NumericType(0)).result, "bitscan backward 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		natl::testAssertEquals(test, NumericType(natl::TypeBitSize<NumericType> - 1), 
			natl::bitscanBackwardResult<NumericType>(natl::Limits<NumericType>::max()).index, "bitscan backward max");
		natl::testAssertEquals(test, NumericType(1), 
			natl::bitscanBackwardResult<NumericType>(NumericType(3)).index, "bitscan backward 3");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		constexpr NumericType addMask = NumericType(1);
		natl::testAssertEquals(test, NumericType(0), natl::bitscanBackwardResult<NumericType>(NumericType(1) | addMask).index, "bitscan backward 1");
		natl::testAssertEquals(test, NumericType(1), natl::bitscanBackwardResult<NumericType>(NumericType(2) | addMask).index, "bitscan backward 2");
		natl::testAssertEquals(test, NumericType(2), natl::bitscanBackwardResult<NumericType>(NumericType(4) | addMask).index, "bitscan backward 4");
		natl::testAssertEquals(test, NumericType(3), natl::bitscanBackwardResult<NumericType>(NumericType(8) | addMask).index, "bitscan backward 8");
		natl::testAssertEquals(test, NumericType(4), natl::bitscanBackwardResult<NumericType>(NumericType(16) | addMask).index, "bitscan backward 16");
		natl::testAssertEquals(test, NumericType(5), natl::bitscanBackwardResult<NumericType>(NumericType(32) | addMask).index, "bitscan backward 32");
		natl::testAssertEquals(test, NumericType(6), natl::bitscanBackwardResult<NumericType>(NumericType(64) | addMask).index, "bitscan backward 64");
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
static_assert(bitscanBackwardTests());


natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, popcountTests());
	natl::subTestAssert(test, bitscanForwardTests());
	natl::subTestAssert(test, bitscanBackwardTests());
	return test;
}


int main() {
	tests();
}