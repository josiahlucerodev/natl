//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/bits.h>
#include <natl/util/test.h>
#include <natl/system/timer.h>

struct BitsTestSuite : public natl::TestSuite<BitsTestSuite> {};

template<typename test_suite_type = BitsTestSuite>
struct BitsTestSuiteTests {
	template<typename NumericType>
	constexpr static void numericPopcountTests(test_suite_type& ts, natl::ConstStringView numericName) noexcept {
		ts.trace(numericName, " popcount");
		ts.assertEqual(NumericType(0), NumericType(natl::popcount(NumericType(0))));

		if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
			ts.assertEqual(natl::TypeBitSize<NumericType>, natl::popcount(natl::Limits<NumericType>::max()));
		}

		if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
			ts.assertEqual(NumericType(1), natl::popcount(NumericType(1)));
			ts.assertEqual(NumericType(1), natl::popcount(NumericType(2)));
			ts.assertEqual(NumericType(1), natl::popcount(NumericType(4)));
			ts.assertEqual(NumericType(1), natl::popcount(NumericType(8)));
			ts.assertEqual(NumericType(1), natl::popcount(NumericType(16)));
			ts.assertEqual(NumericType(2), natl::popcount(NumericType(3)));
		}
	}
	constexpr static void popcountTests(test_suite_type& ts) noexcept {
		numericPopcountTests<natl::i8>(ts, "i8");
		numericPopcountTests<natl::i16>(ts, "i16");
		numericPopcountTests<natl::i32>(ts, "i32");
		numericPopcountTests<natl::i64>(ts, "i64");
		numericPopcountTests<natl::ui8>(ts, "ui8");
		numericPopcountTests<natl::ui16>(ts, "ui16");
		numericPopcountTests<natl::ui32>(ts, "ui32");
		numericPopcountTests<natl::ui64>(ts, "ui64");
		numericPopcountTests<natl::f32>(ts, "f32");
		numericPopcountTests<natl::f64>(ts, "f64");
	}
	NATL_REGISTER_TEST_CONSTEXPR(popcountTests);

	template<typename NumericType>
	constexpr static void numericBitscanForwardTests(test_suite_type& ts, natl::ConstStringView numericName) noexcept {
		ts.trace(numericName, " bitscan forward");
		ts.assertFalse(natl::bitscanForwardResult(NumericType(0)).result);

		if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
			ts.assertEqual(NumericType(0), natl::bitscanForwardResult<NumericType>(natl::Limits<NumericType>::max()).index);
			ts.assertEqual(NumericType(0), natl::bitscanForwardResult<NumericType>(NumericType(3)).index);
		}

		if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
			using uint_type = natl::UIntOfByteSize<natl::TypeByteSize<NumericType>>;
			constexpr NumericType addMask = natl::bitCast<NumericType, uint_type>(uint_type(1) << (uint_type(natl::TypeBitSize<NumericType>) - uint_type(1)));
			ts.assertEqual(NumericType(0), natl::bitscanForwardResult<NumericType>(NumericType(1) | addMask).index);
			ts.assertEqual(NumericType(1), natl::bitscanForwardResult<NumericType>(NumericType(2) | addMask).index);
			ts.assertEqual(NumericType(2), natl::bitscanForwardResult<NumericType>(NumericType(4) | addMask).index);
			ts.assertEqual(NumericType(3), natl::bitscanForwardResult<NumericType>(NumericType(8) | addMask).index);
			ts.assertEqual(NumericType(4), natl::bitscanForwardResult<NumericType>(NumericType(16) | addMask).index);
			ts.assertEqual(NumericType(5), natl::bitscanForwardResult<NumericType>(NumericType(32) | addMask).index);
			ts.assertEqual(NumericType(6), natl::bitscanForwardResult<NumericType>(NumericType(64) | addMask).index);
		}
	}

	constexpr static void bitscanForwardTests(test_suite_type& ts) noexcept {
		numericBitscanForwardTests<natl::i8>(ts, "i8");
		numericBitscanForwardTests<natl::i16>(ts, "i16");
		numericBitscanForwardTests<natl::i32>(ts, "i32");
		numericBitscanForwardTests<natl::i64>(ts, "i64");
		numericBitscanForwardTests<natl::ui8>(ts, "ui8");
		numericBitscanForwardTests<natl::ui16>(ts, "ui16");
		numericBitscanForwardTests<natl::ui32>(ts, "ui32");
		numericBitscanForwardTests<natl::ui64>(ts, "ui64");
		numericBitscanForwardTests<natl::f32>(ts, "f32");
		numericBitscanForwardTests<natl::f64>(ts, "f64");
	}
	NATL_REGISTER_TEST_CONSTEXPR(bitscanForwardTests);

	template<typename NumericType>
	constexpr static void numericBitscanBackwardTests(test_suite_type& ts, natl::ConstStringView numericName) noexcept {
		ts.trace(numericName, " bitscan forward");
		ts.assertFalse(natl::bitscanBackwardResult(NumericType(0)).result);

		if constexpr (natl::IsBuiltInUnsignedIntegerC<NumericType>) {
			ts.assertEqual(NumericType(natl::TypeBitSize<NumericType> -1), natl::bitscanBackwardResult<NumericType>(natl::Limits<NumericType>::max()).index);
			ts.assertEqual(NumericType(1), natl::bitscanBackwardResult<NumericType>(NumericType(3)).index);
		}

		if constexpr (natl::IsBuiltInIntegerC<NumericType>) {
			constexpr NumericType addMask = NumericType(1);
			ts.assertEqual(NumericType(0), natl::bitscanBackwardResult<NumericType>(NumericType(1) | addMask).index);
			ts.assertEqual(NumericType(1), natl::bitscanBackwardResult<NumericType>(NumericType(2) | addMask).index);
			ts.assertEqual(NumericType(2), natl::bitscanBackwardResult<NumericType>(NumericType(4) | addMask).index);
			ts.assertEqual(NumericType(3), natl::bitscanBackwardResult<NumericType>(NumericType(8) | addMask).index);
			ts.assertEqual(NumericType(4), natl::bitscanBackwardResult<NumericType>(NumericType(16) | addMask).index);
			ts.assertEqual(NumericType(5), natl::bitscanBackwardResult<NumericType>(NumericType(32) | addMask).index);
			ts.assertEqual(NumericType(6), natl::bitscanBackwardResult<NumericType>(NumericType(64) | addMask).index);
		}
	}

	constexpr static void bitscanBackwardTests(test_suite_type& ts) noexcept {
		numericBitscanBackwardTests<natl::i8>(ts, "natl::i8");
		numericBitscanBackwardTests<natl::i16>(ts, "natl::i16");
		numericBitscanBackwardTests<natl::i32>(ts, "natl::i32");
		numericBitscanBackwardTests<natl::i64>(ts, "natl::i64");
		numericBitscanBackwardTests<natl::ui8>(ts, "natl::ui8");
		numericBitscanBackwardTests<natl::ui16>(ts, "natl::ui16");
		numericBitscanBackwardTests<natl::ui32>(ts, "natl::ui32");
		numericBitscanBackwardTests<natl::ui64>(ts, "natl::ui64");
		numericBitscanBackwardTests<natl::f32>(ts, "natl::f32");
		numericBitscanBackwardTests<natl::f64>(ts, "natl::f64");
	}
	NATL_REGISTER_TEST_CONSTEXPR(bitscanBackwardTests);
};

NATL_REGISTER_STD_TESTS(BitsTestSuite);

NATL_TESTS_MAIN();