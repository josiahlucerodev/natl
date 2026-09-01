//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/container/bitArray.h>

template<typename BitArrayType>
struct BitArrayTestSuite : public natl::TestSuite<BitArrayTestSuite<BitArrayType>> {
	using bit_array_type = BitArrayType;
};

template<typename test_suite_type = BitArrayTestSuite<natl::BitArray<64>>>
struct BitArrayTestSuiteTests {
	using bit_array_type = test_suite_type::bit_array_type;

	constexpr static void constructionTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		ts.assertEqual(bit_array_type::bitCount, bits.size());
		ts.assertEqual(bit_array_type::bitCount, bits.bitSize());
		ts.assertTrue(bits.none());
		ts.assertFalse(bits.any());
		ts.assertFalse(static_cast<natl::Bool>(bits));

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			ts.assertFalse(bits.test(i));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(constructionTest);

	constexpr static void bitIndexValueConstructionTest(test_suite_type& ts) noexcept {
		constexpr natl::Size index = bit_array_type::bitCount / 2;

		bit_array_type setBits(natl::BitIndex(index), true);
		bit_array_type unsetBits(natl::BitIndex(index), false);

		ts.assertTrue(setBits.test(index));
		ts.assertFalse(unsetBits.test(index));
		ts.assertTrue(setBits.any());
		ts.assertTrue(unsetBits.none());
	}
	NATL_REGISTER_TEST_CONSTEXPR(bitIndexValueConstructionTest);

	constexpr static void setTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits.set(0);

		ts.assertTrue(bits.test(0));
		ts.assertTrue(bits.any());
		ts.assertFalse(bits.none());

		if constexpr (bit_array_type::bitCount > 1) {
			bits.set(bit_array_type::bitCount - 1);

			ts.assertTrue(bits.test(bit_array_type::bitCount - 1));
		}

		if constexpr (bit_array_type::bitCount > 2) {
			constexpr natl::Size middle = bit_array_type::bitCount / 2;

			bits.set(middle);

			ts.assertTrue(bits.test(middle));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(setTest);

	constexpr static void setValueTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits.set(0, true);
		ts.assertTrue(bits.test(0));

		bits.set(0, false);
		ts.assertFalse(bits.test(0));

		bits.set(0, true);
		ts.assertTrue(bits.test(0));
	}
	NATL_REGISTER_TEST_CONSTEXPR(setValueTest);

	constexpr static void setAllTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits.set();

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			ts.assertTrue(bits.test(i));
		}

		ts.assertTrue(bits.any());
		ts.assertTrue(bits.all());
		ts.assertFalse(bits.none());
	}
	NATL_REGISTER_TEST_CONSTEXPR(setAllTest);

	constexpr static void resetBitTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits.set();

		bits.reset(0);
		ts.assertFalse(bits.test(0));

		if constexpr (bit_array_type::bitCount > 1) {
			bits.reset(bit_array_type::bitCount - 1);
			ts.assertFalse(bits.test(bit_array_type::bitCount - 1));
		}

		if constexpr (bit_array_type::bitCount > 2) {
			constexpr natl::Size middle = bit_array_type::bitCount / 2;

			bits.reset(middle);
			ts.assertFalse(bits.test(middle));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(resetBitTest);

	constexpr static void resetTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits.set();
		bits.reset();

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			ts.assertFalse(bits.test(i));
		}

		ts.assertTrue(bits.none());
		ts.assertFalse(bits.any());
	}
	NATL_REGISTER_TEST_CONSTEXPR(resetTest);

	constexpr static void flipTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits.flip();

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			ts.assertTrue(bits.test(i));
		}

		bits.flip();

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			ts.assertFalse(bits.test(i));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(flipTest);

	constexpr static void referenceTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits[0] = true;
		ts.assertTrue(bits.test(0));
		ts.assertTrue(static_cast<natl::Bool>(bits[0]));

		bits[0] = false;
		ts.assertFalse(bits.test(0));
		ts.assertFalse(static_cast<natl::Bool>(bits[0]));
	}
	NATL_REGISTER_TEST_CONSTEXPR(referenceTest);

	constexpr static void referenceAssignmentTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits[0] = true;

		if constexpr (bit_array_type::bitCount > 1) {
			bits[1] = bits[0];

			ts.assertTrue(bits.test(0));
			ts.assertTrue(bits.test(1));

			bits[0] = false;
			bits[1] = bits[0];

			ts.assertFalse(bits.test(0));
			ts.assertFalse(bits.test(1));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(referenceAssignmentTest);

	constexpr static void referenceNotTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits[0] = false;
		ts.assertTrue(~bits[0]);

		bits[0] = true;
		ts.assertFalse(~bits[0]);
	}
	NATL_REGISTER_TEST_CONSTEXPR(referenceNotTest);

	constexpr static void anyNoneTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		ts.assertFalse(bits.any());
		ts.assertTrue(bits.none());

		bits.set(bit_array_type::bitCount / 2);

		ts.assertTrue(bits.any());
		ts.assertFalse(bits.none());

		bits.reset(bit_array_type::bitCount / 2);

		ts.assertFalse(bits.any());
		ts.assertTrue(bits.none());
	}
	NATL_REGISTER_TEST_CONSTEXPR(anyNoneTest);

	constexpr static void allTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		ts.assertFalse(bits.all());

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			bits.set(i);
		}

		ts.assertTrue(bits.all());

		bits.reset(bit_array_type::bitCount / 2);

		ts.assertFalse(bits.all());
	}
	NATL_REGISTER_TEST_CONSTEXPR(allTest);

	constexpr static void countTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		ts.assertEqual(0, bits.count());

		natl::Size expectedCount = 0;

		for (natl::Size i = 0; i < bit_array_type::bitCount; i += 3) {
			bits.set(i);
			expectedCount++;
		}

		ts.assertEqual(expectedCount, bits.count());

		for (natl::Size i = 0; i < bit_array_type::bitCount; i += 3) {
			bits.reset(i);
		}

		ts.assertEqual(0, bits.count());
	}
	NATL_REGISTER_TEST_CONSTEXPR(countTest);

	constexpr static void equalityTest(test_suite_type& ts) noexcept {
		bit_array_type lhs;
		bit_array_type rhs;

		ts.assertEqual(lhs, rhs);

		lhs.set(0);

		ts.assertNotEqual(lhs, rhs);

		rhs.set(0);

		ts.assertEqual(lhs, rhs);

		if constexpr (bit_array_type::bitCount > 1) {
			lhs.set(bit_array_type::bitCount - 1);

			ts.assertNotEqual(lhs, rhs);

			rhs.set(bit_array_type::bitCount - 1);

			ts.assertEqual(lhs, rhs);
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(equalityTest);

	constexpr static void andAssignmentTest(test_suite_type& ts) noexcept {
		bit_array_type lhs;
		bit_array_type rhs;

		lhs.set(0);

		if constexpr (bit_array_type::bitCount > 1) {
			lhs.set(1);
			rhs.set(1);
		}

		if constexpr (bit_array_type::bitCount > 2) {
			rhs.set(2);
		}

		lhs &= rhs;

		ts.assertFalse(lhs.test(0));

		if constexpr (bit_array_type::bitCount > 1) {
			ts.assertTrue(lhs.test(1));
		}

		if constexpr (bit_array_type::bitCount > 2) {
			ts.assertFalse(lhs.test(2));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(andAssignmentTest);

	constexpr static void orAssignmentTest(test_suite_type& ts) noexcept {
		bit_array_type lhs;
		bit_array_type rhs;

		lhs.set(0);

		if constexpr (bit_array_type::bitCount > 1) {
			rhs.set(1);
		}

		lhs |= rhs;

		ts.assertTrue(lhs.test(0));

		if constexpr (bit_array_type::bitCount > 1) {
			ts.assertTrue(lhs.test(1));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(orAssignmentTest);

	constexpr static void xorAssignmentTest(test_suite_type& ts) noexcept {
		bit_array_type lhs;
		bit_array_type rhs;

		lhs.set(0);
		rhs.set(0);

		if constexpr (bit_array_type::bitCount > 1) {
			rhs.set(1);
		}

		lhs ^= rhs;

		ts.assertFalse(lhs.test(0));

		if constexpr (bit_array_type::bitCount > 1) {
			ts.assertTrue(lhs.test(1));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(xorAssignmentTest);

	constexpr static void notOperatorTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		bits.set(0);

		bit_array_type inverted = ~bits;

		ts.assertFalse(inverted.test(0));

		for (natl::Size i = 1; i < bit_array_type::bitCount; i++) {
			ts.assertTrue(inverted.test(i));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(notOperatorTest);

	constexpr static void andOperatorTest(test_suite_type& ts) noexcept {
		bit_array_type lhs;
		bit_array_type rhs;

		lhs.set(0);
		rhs.set(0);

		if constexpr (bit_array_type::bitCount > 1) {
			lhs.set(1);
		}

		if constexpr (bit_array_type::bitCount > 2) {
			rhs.set(2);
		}

		bit_array_type result = lhs & rhs;

		ts.assertTrue(result.test(0));

		if constexpr (bit_array_type::bitCount > 1) {
			ts.assertFalse(result.test(1));
		}

		if constexpr (bit_array_type::bitCount > 2) {
			ts.assertFalse(result.test(2));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(andOperatorTest);

	constexpr static void orOperatorTest(test_suite_type& ts) noexcept {
		bit_array_type lhs;
		bit_array_type rhs;

		lhs.set(0);

		if constexpr (bit_array_type::bitCount > 1) {
			rhs.set(1);
		}

		bit_array_type result = lhs | rhs;

		ts.assertTrue(result.test(0));

		if constexpr (bit_array_type::bitCount > 1) {
			ts.assertTrue(result.test(1));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(orOperatorTest);

	constexpr static void xorOperatorTest(test_suite_type& ts) noexcept {
		bit_array_type lhs;
		bit_array_type rhs;

		lhs.set(0);
		rhs.set(0);

		if constexpr (bit_array_type::bitCount > 1) {
			rhs.set(1);
		}

		bit_array_type result = lhs ^ rhs;

		ts.assertFalse(result.test(0));

		if constexpr (bit_array_type::bitCount > 1) {
			ts.assertTrue(result.test(1));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(xorOperatorTest);

	constexpr static void boolConversionTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		ts.assertFalse(static_cast<natl::Bool>(bits));

		bits.set(bit_array_type::bitCount / 2);

		ts.assertTrue(static_cast<natl::Bool>(bits));

		bits.reset(bit_array_type::bitCount / 2);

		ts.assertFalse(static_cast<natl::Bool>(bits));
	}
	NATL_REGISTER_TEST_CONSTEXPR(boolConversionTest);

	constexpr static void wordBoundaryTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		constexpr natl::Size bitsPerWord = bit_array_type::bitsPerWordCount;

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			if (
				i == 0 ||
				i % bitsPerWord == 0 ||
				i % bitsPerWord == bitsPerWord - 1 ||
				i == bit_array_type::bitCount - 1
				) {
				bits.set(i);
			}
		}

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			natl::Bool expected =
				i == 0 ||
				i % bitsPerWord == 0 ||
				i % bitsPerWord == bitsPerWord - 1 ||
				i == bit_array_type::bitCount - 1;

			ts.assertEqual(expected, bits.test(i));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(wordBoundaryTest);

	constexpr static void alternatingBitsTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		for (natl::Size i = 0; i < bit_array_type::bitCount; i += 2) {
			bits.set(i);
		}

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			ts.assertEqual((i % 2) == 0, bits.test(i));
		}

		bits.flip();

		for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
			ts.assertEqual((i % 2) != 0, bits.test(i));
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(alternatingBitsTest);

	constexpr static void repeatedModificationTest(test_suite_type& ts) noexcept {
		bit_array_type bits;

		for (natl::Size iteration = 0; iteration < 100; iteration++) {
			for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
				bits.set(i);
				ts.assertTrue(bits.test(i));
			}

			for (natl::Size i = 0; i < bit_array_type::bitCount; i += 2) {
				bits.reset(i);
			}

			for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
				ts.assertEqual((i % 2) != 0, bits.test(i));
			}

			bits.flip();

			for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
				ts.assertEqual((i % 2) == 0, bits.test(i));
			}

			for (natl::Size i = 0; i < bit_array_type::bitCount; i++) {
				bits.reset(i);
			}

			ts.assertTrue(bits.none());
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(repeatedModificationTest);
};

struct BitArray1TestSuite : public BitArrayTestSuite<natl::BitArray<1>> {};
NATL_REGISTER_TESTS(BitArray1TestSuite, BitArrayTestSuiteTests);

struct BitArray8TestSuite : public BitArrayTestSuite<natl::BitArray<8>> {};
NATL_REGISTER_TESTS(BitArray8TestSuite, BitArrayTestSuiteTests);

struct BitArray31TestSuite : public BitArrayTestSuite<natl::BitArray<31>> {};
NATL_REGISTER_TESTS(BitArray31TestSuite, BitArrayTestSuiteTests);

struct BitArray32TestSuite : public BitArrayTestSuite<natl::BitArray<32>> {};
NATL_REGISTER_TESTS(BitArray32TestSuite, BitArrayTestSuiteTests);

struct BitArray33TestSuite : public BitArrayTestSuite<natl::BitArray<33>> {};
NATL_REGISTER_TESTS(BitArray33TestSuite, BitArrayTestSuiteTests);

/*
struct BitArray63TestSuite : public BitArrayTestSuite<natl::BitArray<63>> {};
NATL_REGISTER_TESTS(BitArray63TestSuite, BitArrayTestSuiteTests);

struct BitArray64TestSuite : public BitArrayTestSuite<natl::BitArray<64>> {};
NATL_REGISTER_TESTS(BitArray64TestSuite, BitArrayTestSuiteTests);

struct BitArray65TestSuite : public BitArrayTestSuite<natl::BitArray<65>> {};
NATL_REGISTER_TESTS(BitArray65TestSuite, BitArrayTestSuiteTests);

struct BitArray127TestSuite : public BitArrayTestSuite<natl::BitArray<127>> {};
NATL_REGISTER_TESTS(BitArray127TestSuite, BitArrayTestSuiteTests);

struct BitArray128TestSuite : public BitArrayTestSuite<natl::BitArray<128>> {};
NATL_REGISTER_TESTS(BitArray128TestSuite, BitArrayTestSuiteTests);

struct BitArray129TestSuite : public BitArrayTestSuite<natl::BitArray<129>> {};
NATL_REGISTER_TESTS(BitArray129TestSuite, BitArrayTestSuiteTests);
*/

NATL_TESTS_MAIN();
