
//natl
#include <natl/util/bits.h>
#include <natl/util/test.h>
#include <natl/system/timer.h>

#define NATL_TEST_FROM "NatlBitsTest"

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericPopcountTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(NATL_TEST_FROM, natl::String(IntegerName) + " popcount", natl::TestType::leaf);
	NATL_TEST_ASSERT(natl::popcount(NumericType(0)) == NumericType(0), "popcount 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		NATL_TEST_ASSERT(natl::popcount(natl::Limits<NumericType>::max()) == natl::TypeBitSize<NumericType>, "popcount max");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		NATL_TEST_ASSERT(natl::popcount(NumericType(1)) == NumericType(1), "popcount 1");
		NATL_TEST_ASSERT(natl::popcount(NumericType(2)) == NumericType(1), "popcount 2");
		NATL_TEST_ASSERT(natl::popcount(NumericType(4)) == NumericType(1), "popcount 4");
		NATL_TEST_ASSERT(natl::popcount(NumericType(8)) == NumericType(1), "popcount 8");
		NATL_TEST_ASSERT(natl::popcount(NumericType(16)) == NumericType(1), "popcount 16");

		NATL_TEST_ASSERT(natl::popcount(NumericType(3)) == NumericType(2), "popcount 3");
	}

	return test;
}

constexpr natl::Bool popcountTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "popcount tests", natl::TestType::node);
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::i8, "natl::i8">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::i16, "natl::i16">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::i32, "natl::i32">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::i64, "natl::i64">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::ui8, "natl::ui8">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::ui16, "natl::ui16">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::ui32, "natl::ui32">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::ui64, "natl::ui64">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::f32, "natl::f32">()));
	NATL_SUB_TEST_ASSERT((numericPopcountTests<natl::f64, "natl::f64">()));
	return test;
}

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericBitscanForwardTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(NATL_TEST_FROM, natl::String(IntegerName) + " bitscan forward", natl::TestType::leaf);
	NATL_TEST_ASSERT(natl::bitscanForwardResult(NumericType(0)).result == false, "bitscan forward 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		NATL_TEST_ASSERT(natl::bitscanForwardResult<NumericType>(natl::Limits<NumericType>::max()).index == natl::TypeBitSize<NumericType> - 1, "bitscan forward max");
		NATL_TEST_ASSERT(natl::bitscanForwardResult<NumericType>(3).index == 1, "bitscan forward 3");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		NATL_TEST_ASSERT(natl::bitscanForwardResult<NumericType>(1).index == 0, "bitscan forward 1");
		NATL_TEST_ASSERT(natl::bitscanForwardResult<NumericType>(2).index == 1, "bitscan forward 2");
		NATL_TEST_ASSERT(natl::bitscanForwardResult<NumericType>(4).index == 2, "bitscan forward 4");
		NATL_TEST_ASSERT(natl::bitscanForwardResult<NumericType>(8).index == 3, "bitscan forward 8");
		NATL_TEST_ASSERT(natl::bitscanForwardResult<NumericType>(16).index == 4, "bitscan forward 16");
	}

	return test;
}

constexpr natl::Bool bitscanForwardTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "bitscan forward tests", natl::TestType::node);
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::i8, "natl::i8">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::i16, "natl::i16">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::i32, "natl::i32">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::i64, "natl::i64">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::ui8, "natl::ui8">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::ui16, "natl::ui16">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::ui32, "natl::ui32">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::ui64, "natl::ui64">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::f32, "natl::f32">()));
	NATL_SUB_TEST_ASSERT((numericBitscanForwardTests<natl::f64, "natl::f64">()));
	return test;
}

template<typename NumericType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool numericBitscanBackwardTests() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(NATL_TEST_FROM, natl::String(IntegerName) + " bitscan backward", natl::TestType::leaf);
	NATL_TEST_ASSERT(natl::bitscanBackwardResult(NumericType(0)).result == false, "bitscan backward 0");

	if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
		NATL_TEST_ASSERT(natl::bitscanBackwardResult<NumericType>(natl::Limits<NumericType>::max()).index == 0, "bitscan backward max");
		NATL_TEST_ASSERT(natl::bitscanBackwardResult<NumericType>(3).index == 0, "bitscan backward 3");
	}

	if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
		NATL_TEST_ASSERT(natl::bitscanBackwardResult<NumericType>(1).index == 0, "bitscan backward 1");
		NATL_TEST_ASSERT(natl::bitscanBackwardResult<NumericType>(2).index == 1, "bitscan backward 2");
		NATL_TEST_ASSERT(natl::bitscanBackwardResult<NumericType>(4).index == 2, "bitscan backward 4");
		NATL_TEST_ASSERT(natl::bitscanBackwardResult<NumericType>(8).index == 3, "bitscan backward 8");
		NATL_TEST_ASSERT(natl::bitscanBackwardResult<NumericType>(16).index == 4, "bitscan backward 16");
	}

	return test;
}

constexpr natl::Bool bitscanBackwardTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "bitscan backward tests", natl::TestType::node);
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::i8, "natl::i8">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::i16, "natl::i16">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::i32, "natl::i32">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::i64, "natl::i64">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::ui8, "natl::ui8">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::ui16, "natl::ui16">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::ui32, "natl::ui32">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::ui64, "natl::ui64">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::f32, "natl::f32">()));
	NATL_SUB_TEST_ASSERT((numericBitscanBackwardTests<natl::f64, "natl::f64">()));
	return test;
}

static_assert(popcountTests());
static_assert(bitscanBackwardTests());

natl::Bool tests() noexcept {
	natl::Test test(NATL_TEST_FROM, "all", natl::TestType::root);
	NATL_SUB_TEST_ASSERT(popcountTests());
	NATL_SUB_TEST_ASSERT(bitscanForwardTests());
	NATL_SUB_TEST_ASSERT(bitscanBackwardTests());
	return test;
}


int main() {
	natl::Timer timer;
	tests();
	natl::printlnf("natl: ", NATL_TEST_FROM, " time: ", timer.getMillisecondsInt());
}