
//natl
#include <natl/simd.h>
#include <natl/util/typeInfo.h>
#include <natl/util/test.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlSimdTest";

template<typename DataType, typename SimdArchType>
constexpr natl::Bool constructionTest() noexcept {
	[[maybe_unused]] nasimd::SimdRegister<DataType, SimdArchType> simdRegister;
	[[maybe_unused]] nasimd::SimdRegisterInfo<DataType, SimdArchType> simdRegisterInfo;
	[[maybe_unused]] nasimd::SimdMask<DataType, SimdArchType> simdMask;
	[[maybe_unused]] nasimd::SimdMaskInfo<DataType, SimdArchType> simdMaskInfo;
	[[maybe_unused]] nasimd::SimdCMask<DataType, SimdArchType> simdCmask;
	[[maybe_unused]] nasimd::SimdCMaskInfo<DataType, SimdArchType> simdCmaskInfo;
	return true;
}

//compare
template<typename DataType, typename SimdArchType>
constexpr natl::Bool compareTest() noexcept {
	natl::Test test(natlTestFrom, "compare", natl::TestType::leaf);

	using simd_register = nasimd::SimdRegister<DataType, SimdArchType>;
	using simd_mmask = nasimd::SimdMask<DataType, SimdArchType>;
	constexpr natl::Size halfCount = nasimd::SimdRegisterToInfo<simd_register>::count() / 2;

	simd_mmask evenMask = nasimd::createEvenMMask<DataType, SimdArchType>();
	simd_mmask oddMask = nasimd::createOddMMask<DataType, SimdArchType>();
	simd_register constant1 = nasimd::simd_set<DataType, SimdArchType>(1);
	simd_register constant2 = nasimd::simd_set<DataType, SimdArchType>(2);
	simd_register constant3 = nasimd::simd_set<DataType, SimdArchType>(3);

	//equal 
	{
		simd_mmask compareEqualTrueMask = nasimd::simd_compare_equal<DataType, SimdArchType>(constant2, constant2);
		natl::Bool compereEqualTrueCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(compareEqualTrueMask);
		natl::testAssert(test, compereEqualTrueCondition, "compare equal true");

		simd_mmask compareEqualFalseMask = nasimd::simd_compare_equal<DataType, SimdArchType>(constant1, constant2);
		natl::Bool compereEqualFalseCondition = nasimd::simd_mask_test_all_inactive<DataType, SimdArchType>(compareEqualFalseMask);
		natl::testAssert(test, compereEqualFalseCondition, "compare equal false");

		simd_mmask mmaskedCompareEqualTrueMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(constant2, constant2, evenMask);
		natl::Bool mmaskedCompareEqualTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareEqualTrueMask) == halfCount;
		natl::testAssert(test, mmaskedCompareEqualTrueCondition, "mmasked compare equal true");

		simd_mmask mmaskedCompareEqualFalseMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(constant1, constant2, evenMask);
		natl::Bool mmaskedCompareEqualFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareEqualFalseMask) == 0;
		natl::testAssert(test, mmaskedCompareEqualFalseCondition, "mmasked compare equal false");

		simd_mmask mmaskedSrcCompareEqualTrueMask = nasimd::simd_mmasked_src_compare_equal<DataType, SimdArchType>(constant2, constant2, evenMask, evenMask);
		natl::Bool mmaskedSrcCompareEqualTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareEqualTrueMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareEqualTrueCondition, "mmasked src compare equal true");

		simd_mmask mmaskedSrcCompareEqualFalseMask = nasimd::simd_mmasked_src_compare_equal<DataType, SimdArchType>(constant1, constant2, oddMask, evenMask);
		natl::Bool mmaskedSrcCompareEqualFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareEqualFalseMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareEqualFalseCondition, "mmasked src compare equal false");
	}

	//not equal   
	{
		simd_mmask compareNotEqualTrueMask = nasimd::simd_compare_not_equal<DataType, SimdArchType>(constant1, constant2);
		natl::Bool compereNotEqualTrueCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(compareNotEqualTrueMask);
		natl::testAssert(test, compereNotEqualTrueCondition, "compare not equal true");

		simd_mmask compareNotEqualFalseMask = nasimd::simd_compare_not_equal<DataType, SimdArchType>(constant2, constant2);
		natl::Bool compereNotEqualFalseCondition = nasimd::simd_mask_test_all_inactive<DataType, SimdArchType>(compareNotEqualFalseMask);
		natl::testAssert(test, compereNotEqualFalseCondition, "compare not equal false");

		simd_mmask mmaskedCompareNotEqualTrueMask = nasimd::simd_mmasked_compare_not_equal<DataType, SimdArchType>(constant1, constant2, evenMask);
		natl::Bool mmaskedCompareNotEqualTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareNotEqualTrueMask) == halfCount;
		natl::testAssert(test, mmaskedCompareNotEqualTrueCondition, "mmasked compare not equal true");

		simd_mmask mmaskedCompareNotEqualFalseMask = nasimd::simd_mmasked_compare_not_equal<DataType, SimdArchType>(constant2, constant2, evenMask);
		natl::Bool mmaskedCompareNotEqualFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareNotEqualFalseMask) == 0;
		natl::testAssert(test, mmaskedCompareNotEqualFalseCondition, "mmasked compare not equal false");

		simd_mmask mmaskedSrcCompareNotEqualTrueMask = nasimd::simd_mmasked_src_compare_not_equal<DataType, SimdArchType>(constant1, constant2, evenMask, evenMask);
		natl::Bool mmaskedSrcCompareNotEqualTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareNotEqualTrueMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareNotEqualTrueCondition, "mmasked src compare not equal true");

		simd_mmask mmaskedSrcCompareNotEqualFalseMask = nasimd::simd_mmasked_src_compare_not_equal<DataType, SimdArchType>(constant2, constant2, oddMask, evenMask);
		natl::Bool mmaskedSrcCompareNotEqualFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareNotEqualFalseMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareNotEqualFalseCondition, "mmasked src compare not equal false");
	}

	//less than   
	{
		simd_mmask compareLessThanTrueMask = nasimd::simd_compare_less_than<DataType, SimdArchType>(constant1, constant2);
		natl::Bool compereLessThanTrueCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(compareLessThanTrueMask);
		natl::testAssert(test, compereLessThanTrueCondition, "compare less than true");

		simd_mmask compareLessThanFalseMask = nasimd::simd_compare_less_than<DataType, SimdArchType>(constant2, constant2);
		natl::Bool compereLessThanFalseCondition = nasimd::simd_mask_test_all_inactive<DataType, SimdArchType>(compareLessThanFalseMask);
		natl::testAssert(test, compereLessThanFalseCondition, "compare less than false");

		simd_mmask mmaskedCompareLessThanTrueMask = nasimd::simd_mmasked_compare_less_than<DataType, SimdArchType>(constant1, constant2, evenMask);
		natl::Bool mmaskedCompareLessThanTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareLessThanTrueMask) == halfCount;
		natl::testAssert(test, mmaskedCompareLessThanTrueCondition, "mmasked compare less than true");

		simd_mmask mmaskedCompareLessThanFalseMask = nasimd::simd_mmasked_compare_less_than<DataType, SimdArchType>(constant2, constant2, evenMask);
		natl::Bool mmaskedCompareLessThanFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareLessThanFalseMask) == 0;
		natl::testAssert(test, mmaskedCompareLessThanFalseCondition, "mmasked compare less than false");

		simd_mmask mmaskedSrcCompareLessThanTrueMask = nasimd::simd_mmasked_src_compare_less_than<DataType, SimdArchType>(constant1, constant2, evenMask, evenMask);
		natl::Bool mmaskedSrcCompareLessThanTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareLessThanTrueMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareLessThanTrueCondition, "mmasked src compare less than true");

		simd_mmask mmaskedSrcCompareLessThanFalseMask = nasimd::simd_mmasked_src_compare_less_than<DataType, SimdArchType>(constant2, constant2, oddMask, evenMask);
		natl::Bool mmaskedSrcCompareLessThanFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareLessThanFalseMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareLessThanFalseCondition, "mmasked src compare less than false");
	}
	 
	//greater than   
	{
		simd_mmask compareGreaterThanTrueMask = nasimd::simd_compare_greater_than<DataType, SimdArchType>(constant2, constant1);
		natl::Bool compereGreaterThanTrueCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(compareGreaterThanTrueMask);
		natl::testAssert(test, compereGreaterThanTrueCondition, "compare greater than true");

		simd_mmask compareGreaterThanFalseMask = nasimd::simd_compare_greater_than<DataType, SimdArchType>(constant2, constant2);
		natl::Bool compereGreaterThanFalseCondition = nasimd::simd_mask_test_all_inactive<DataType, SimdArchType>(compareGreaterThanFalseMask);
		natl::testAssert(test, compereGreaterThanFalseCondition, "compare greater than false");

		simd_mmask mmaskedCompareGreaterThanTrueMask = nasimd::simd_mmasked_compare_greater_than<DataType, SimdArchType>(constant2, constant1, evenMask);
		natl::Bool mmaskedCompareGreaterThanTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareGreaterThanTrueMask) == halfCount;
		natl::testAssert(test, mmaskedCompareGreaterThanTrueCondition, "mmasked compare greater than true");

		simd_mmask mmaskedCompareGreaterThanFalseMask = nasimd::simd_mmasked_compare_greater_than<DataType, SimdArchType>(constant2, constant2, evenMask);
		natl::Bool mmaskedCompareGreaterThanFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareGreaterThanFalseMask) == 0;
		natl::testAssert(test, mmaskedCompareGreaterThanFalseCondition, "mmasked compare greater than false");

		simd_mmask mmaskedSrcCompareGreaterThanTrueMask = nasimd::simd_mmasked_src_compare_greater_than<DataType, SimdArchType>(constant2, constant1, evenMask, evenMask);
		natl::Bool mmaskedSrcCompareGreaterThanTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareGreaterThanTrueMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareGreaterThanTrueCondition, "mmasked src compare greater than true");

		simd_mmask mmaskedSrcCompareGreaterThanFalseMask = nasimd::simd_mmasked_src_compare_greater_than<DataType, SimdArchType>(constant2, constant2, oddMask, evenMask);
		natl::Bool mmaskedSrcCompareGreaterThanFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareGreaterThanFalseMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareGreaterThanFalseCondition, "mmasked src compare greater than false");
	}

	//less than or equal
	{
		//equal
		simd_mmask compareEqualTrueMask = nasimd::simd_compare_less_than_or_equal<DataType, SimdArchType>(constant2, constant2);
		natl::Bool compereEqualTrueCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(compareEqualTrueMask);
		natl::testAssert(test, compereEqualTrueCondition, "compare (less than or equal) equal true");

		simd_mmask compareEqualFalseMask = nasimd::simd_compare_less_than_or_equal<DataType, SimdArchType>(constant3, constant2);
		natl::Bool compereEqualFalseCondition = nasimd::simd_mask_test_all_inactive<DataType, SimdArchType>(compareEqualFalseMask);
		natl::testAssert(test, compereEqualFalseCondition, "compare (less than or equal) equal false");

		simd_mmask mmaskedCompareEqualTrueMask = nasimd::simd_mmasked_compare_less_than_or_equal<DataType, SimdArchType>(constant2, constant2, evenMask);
		natl::Bool mmaskedCompareEqualTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareEqualTrueMask) == halfCount;
		natl::testAssert(test, mmaskedCompareEqualTrueCondition, "mmasked compare (less than or equal) equal true");

		simd_mmask mmaskedCompareEqualFalseMask = nasimd::simd_mmasked_compare_less_than_or_equal<DataType, SimdArchType>(constant3, constant2, evenMask);
		natl::Bool mmaskedCompareEqualFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareEqualFalseMask) == 0;
		natl::testAssert(test, mmaskedCompareEqualFalseCondition, "mmasked compare (less than or equal) equal false");

		simd_mmask mmaskedSrcCompareEqualTrueMask = nasimd::simd_mmasked_src_compare_less_than_or_equal<DataType, SimdArchType>(constant2, constant2, evenMask, evenMask);
		natl::Bool mmaskedSrcCompareEqualTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareEqualTrueMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareEqualTrueCondition, "mmasked src compare (less than or equal) equal true");

		simd_mmask mmaskedSrcCompareEqualFalseMask = nasimd::simd_mmasked_src_compare_less_than_or_equal<DataType, SimdArchType>(constant3, constant2, oddMask, evenMask);
		natl::Bool mmaskedSrcCompareEqualFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareEqualFalseMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareEqualFalseCondition, "mmasked src compare (less than or equal) equal false");

		//less than
		simd_mmask compareLessThanTrueMask = nasimd::simd_compare_less_than_or_equal<DataType, SimdArchType>(constant1, constant2);
		natl::Bool compereLessThanTrueCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(compareLessThanTrueMask);
		natl::testAssert(test, compereLessThanTrueCondition, "compare (less than or equal) less than true");

		simd_mmask compareLessThanFalseMask = nasimd::simd_compare_less_than_or_equal<DataType, SimdArchType>(constant3, constant2);
		natl::Bool compereLessThanFalseCondition = nasimd::simd_mask_test_all_inactive<DataType, SimdArchType>(compareLessThanFalseMask);
		natl::testAssert(test, compereLessThanFalseCondition, "compare (less than or equal) less than false");

		simd_mmask mmaskedCompareLessThanTrueMask = nasimd::simd_mmasked_compare_less_than_or_equal<DataType, SimdArchType>(constant1, constant2, evenMask);
		natl::Bool mmaskedCompareLessThanTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareLessThanTrueMask) == halfCount;
		natl::testAssert(test, mmaskedCompareLessThanTrueCondition, "mmasked compare (less than or equal) less than true");

		simd_mmask mmaskedCompareLessThanFalseMask = nasimd::simd_mmasked_compare_less_than_or_equal<DataType, SimdArchType>(constant3, constant2, evenMask);
		natl::Bool mmaskedCompareLessThanFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareLessThanFalseMask) == 0;
		natl::testAssert(test, mmaskedCompareLessThanFalseCondition, "mmasked compare (less than or equal) less than false");

		simd_mmask mmaskedSrcCompareLessThanTrueMask = nasimd::simd_mmasked_src_compare_less_than_or_equal<DataType, SimdArchType>(constant1, constant2, evenMask, evenMask);
		natl::Bool mmaskedSrcCompareLessThanTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareLessThanTrueMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareLessThanTrueCondition, "mmasked src (less than or equal) compare less than true");

		simd_mmask mmaskedSrcCompareLessThanFalseMask = nasimd::simd_mmasked_src_compare_less_than_or_equal<DataType, SimdArchType>(constant3, constant2, oddMask, evenMask);
		natl::Bool mmaskedSrcCompareLessThanFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareLessThanFalseMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareLessThanFalseCondition, "mmasked src (less than or equal) compare less than false");
	}

	//greater than or eqaul 
	{
		//eqaul 
		simd_mmask compareEqualTrueMask = nasimd::simd_compare_greater_than_or_equal<DataType, SimdArchType>(constant2, constant2);
		natl::Bool compereEqualTrueCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(compareEqualTrueMask);
		natl::testAssert(test, compereEqualTrueCondition, "compare (greater than or equal) equal true");

		simd_mmask compareEqualFalseMask = nasimd::simd_compare_greater_than_or_equal<DataType, SimdArchType>(constant1, constant2);
		natl::Bool compereEqualFalseCondition = nasimd::simd_mask_test_all_inactive<DataType, SimdArchType>(compareEqualFalseMask);
		natl::testAssert(test, compereEqualFalseCondition, "compare (greater than or equal) equal false");

		simd_mmask mmaskedCompareEqualTrueMask = nasimd::simd_mmasked_compare_greater_than_or_equal<DataType, SimdArchType>(constant2, constant2, evenMask);
		natl::Bool mmaskedCompareEqualTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareEqualTrueMask) == halfCount;
		natl::testAssert(test, mmaskedCompareEqualTrueCondition, "mmasked compare (greater than or equal) equal true");

		simd_mmask mmaskedCompareEqualFalseMask = nasimd::simd_mmasked_compare_greater_than_or_equal<DataType, SimdArchType>(constant1, constant2, evenMask);
		natl::Bool mmaskedCompareEqualFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareEqualFalseMask) == 0;
		natl::testAssert(test, mmaskedCompareEqualFalseCondition, "mmasked compare (greater than or equal) equal false");

		simd_mmask mmaskedSrcCompareEqualTrueMask = nasimd::simd_mmasked_src_compare_greater_than_or_equal<DataType, SimdArchType>(constant2, constant2, evenMask, evenMask);
		natl::Bool mmaskedSrcCompareEqualTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareEqualTrueMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareEqualTrueCondition, "mmasked src compare (greater than or equal) equal true");

		simd_mmask mmaskedSrcCompareEqualFalseMask = nasimd::simd_mmasked_src_compare_greater_than_or_equal<DataType, SimdArchType>(constant1, constant2, oddMask, evenMask);
		natl::Bool mmaskedSrcCompareEqualFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareEqualFalseMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareEqualFalseCondition, "mmasked src compare (greater than or equal) equal false");

		//greater than 
		simd_mmask compareGreaterThanTrueMask = nasimd::simd_compare_greater_than_or_equal<DataType, SimdArchType>(constant2, constant1);
		natl::Bool compereGreaterThanTrueCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(compareGreaterThanTrueMask);
		natl::testAssert(test, compereGreaterThanTrueCondition, "compare (greater than or equal) greater than true");

		simd_mmask compareGreaterThanFalseMask = nasimd::simd_compare_greater_than_or_equal<DataType, SimdArchType>(constant1, constant2);
		natl::Bool compereGreaterThanFalseCondition = nasimd::simd_mask_test_all_inactive<DataType, SimdArchType>(compareGreaterThanFalseMask);
		natl::testAssert(test, compereGreaterThanFalseCondition, "compare (greater than or equal) greater than false");

		simd_mmask mmaskedCompareGreaterThanTrueMask = nasimd::simd_mmasked_compare_greater_than_or_equal<DataType, SimdArchType>(constant2, constant1, evenMask);
		natl::Bool mmaskedCompareGreaterThanTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareGreaterThanTrueMask) == halfCount;
		natl::testAssert(test, mmaskedCompareGreaterThanTrueCondition, "mmasked compare (greater than or equal) greater than true");

		simd_mmask mmaskedCompareGreaterThanFalseMask = nasimd::simd_mmasked_compare_greater_than_or_equal<DataType, SimdArchType>(constant1, constant2, evenMask);
		natl::Bool mmaskedCompareGreaterThanFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedCompareGreaterThanFalseMask) == 0;
		natl::testAssert(test, mmaskedCompareGreaterThanFalseCondition, "mmasked compare (greater than or equal) greater than false");

		simd_mmask mmaskedSrcCompareGreaterThanTrueMask = nasimd::simd_mmasked_src_compare_greater_than_or_equal<DataType, SimdArchType>(constant2, constant1, evenMask, evenMask);
		natl::Bool mmaskedSrcCompareGreaterThanTrueCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareGreaterThanTrueMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareGreaterThanTrueCondition, "mmasked src compare (greater than or equal) greater than true");

		simd_mmask mmaskedSrcCompareGreaterThanFalseMask = nasimd::simd_mmasked_src_compare_greater_than_or_equal<DataType, SimdArchType>(constant1, constant2, oddMask, evenMask);
		natl::Bool mmaskedSrcCompareGreaterThanFalseCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcCompareGreaterThanFalseMask) == halfCount;
		natl::testAssert(test, mmaskedSrcCompareGreaterThanFalseCondition, "mmasked src compare (greater than or equal) greater than false");
	}

	return test;
}

template<typename DataType, typename SimdArchType>
constexpr void testOpIfEqaul(natl::Test& test,
	nasimd::SimdRegister<DataType, SimdArchType> result,
	nasimd::SimdRegister<DataType, SimdArchType> expectedValue,
	const natl::ConstAsciiStringView& message) noexcept {
	nasimd::SimdMask<DataType, SimdArchType> mask = nasimd::simd_compare_equal<DataType, SimdArchType>(result, expectedValue);
	natl::Bool condition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(mask);
	natl::testAssert(test, condition, message);
}

template<typename DataType, typename SimdArchType>
constexpr void testOpIfEqaulCount(natl::Test& test,
	nasimd::SimdRegister<DataType, SimdArchType> result,
	nasimd::SimdRegister<DataType, SimdArchType> expectedValue,
	const natl::Size count,
	const natl::ConstAsciiStringView& message) noexcept {
	nasimd::SimdMask<DataType, SimdArchType> mask = nasimd::simd_compare_equal<DataType, SimdArchType>(result, expectedValue);
	natl::Bool condition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mask) == count;
	natl::testAssert(test, condition, message);
}

template<typename DataType, typename SimdArchType>
constexpr void testOpIfEqaulMaskedCount(natl::Test& test,
	nasimd::SimdRegister<DataType, SimdArchType> result,
	nasimd::SimdRegister<DataType, SimdArchType> expectedValue,
	nasimd::SimdMask<DataType, SimdArchType> mmask,
	const natl::Size count,
	const natl::ConstAsciiStringView& message) noexcept {
	nasimd::SimdMask<DataType, SimdArchType> mask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(result, expectedValue, mmask);
	natl::Bool condition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mask) == count;
	natl::testAssert(test, condition, message);
}

//arithmetic
template<typename DataType, typename SimdArchType>
constexpr natl::Bool arithmeticTest() noexcept {
	natl::Test test(natlTestFrom, "arithmetic", natl::TestType::leaf);

	using simd_register = nasimd::SimdRegister<DataType, SimdArchType>;
	using simd_mmask = nasimd::SimdMask<DataType, SimdArchType>;
	constexpr natl::Size halfCount = nasimd::SimdRegisterToInfo<simd_register>::count() / 2;

	simd_mmask evenMask = nasimd::createEvenMMask<DataType, SimdArchType>();
	simd_mmask oddMask = nasimd::createOddMMask<DataType, SimdArchType>();
	simd_register constant0 = nasimd::simd_set<DataType, SimdArchType>(0);
	simd_register constant1 = nasimd::simd_set<DataType, SimdArchType>(1);
	simd_register constant2 = nasimd::simd_set<DataType, SimdArchType>(2);
	simd_register constant3 = nasimd::simd_set<DataType, SimdArchType>(3);
	simd_register constant4 = nasimd::simd_set<DataType, SimdArchType>(4);

	simd_register constantMax = nasimd::simd_set<DataType, SimdArchType>(natl::Limits<DataType>::max());
	simd_register constantMin = nasimd::simd_set<DataType, SimdArchType>(natl::Limits<DataType>::min());

	//add
	{
		simd_register addResult = nasimd::simd_add<DataType, SimdArchType>(constant1, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, addResult, constant3, "add");

		simd_register mmaskedAddResult = nasimd::simd_mmasked_add<DataType, SimdArchType>(constant1, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedAddResult, constant3, halfCount, "mmasked add");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedAddResult, constant0, oddMask, halfCount, "mmasked add remain");

		simd_register mmaskedSrcAddResult = nasimd::simd_mmasked_src_add<DataType, SimdArchType>(constant1, constant2, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcAddResult, constant3, halfCount, "mmasked src add");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcAddResult, constant1, oddMask, halfCount, "mmasked src add remain");
	}

	//sub
	{
		simd_register subResult = nasimd::simd_sub<DataType, SimdArchType>(constant3, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, subResult, constant1, "sub");

		simd_register mmaskedSubResult = nasimd::simd_mmasked_sub<DataType, SimdArchType>(constant3, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSubResult, constant1, halfCount, "mmasked sub");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSubResult, constant0, oddMask, halfCount, "mmasked sub remain");

		simd_register mmaskedSrcSubResult = nasimd::simd_mmasked_src_sub<DataType, SimdArchType>(constant3, constant2, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcSubResult, constant1, halfCount, "mmasked src sub");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcSubResult, constant2, oddMask, halfCount, "mmasked src sub remain");
	}

	//mul
	{
		simd_register mulResult = nasimd::simd_mul<DataType, SimdArchType>(constant2, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, mulResult, constant4, "mul");

		simd_register mmaskedMulResult = nasimd::simd_mmasked_mul<DataType, SimdArchType>(constant2, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedMulResult, constant4, halfCount, "mmasked mul");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedMulResult, constant0, oddMask, halfCount, "mmasked mul remain");

		simd_register mmaskedSrcMulResult = nasimd::simd_mmasked_src_mul<DataType, SimdArchType>(constant2, constant2, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcMulResult, constant4, halfCount, "mmasked src mul");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcMulResult, constant1, oddMask, halfCount, "mmasked src mul remain");
	}

	//div
	{
		simd_register divResult = nasimd::simd_div<DataType, SimdArchType>(constant4, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, divResult, constant2, "div");

		simd_register mmaskedDivResult = nasimd::simd_mmasked_div<DataType, SimdArchType>(constant4, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedDivResult, constant2, halfCount, "mmasked div");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedDivResult, constant0, oddMask, halfCount, "mmasked div remain");

		simd_register mmaskedSrcDivResult = nasimd::simd_mmasked_src_div<DataType, SimdArchType>(constant4, constant2, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcDivResult, constant2, halfCount, "mmasked src div");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcDivResult, constant1, oddMask, halfCount, "mmasked src div remain");
	}

	//add sat 
	if constexpr (natl::IsBuiltInIntegerC<DataType>) {
		simd_register addSatResult = nasimd::simd_add_sat<DataType, SimdArchType>(constant1, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, addSatResult, constant3, "add sat");

		simd_register mmaskedAddSatResult = nasimd::simd_mmasked_add_sat<DataType, SimdArchType>(constant1, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedAddSatResult, constant3, halfCount, "mmasked add sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedAddSatResult, constant0, oddMask, halfCount, "mmasked add sat remain");

		simd_register mmaskedSrcAddSatResult = nasimd::simd_mmasked_src_add_sat<DataType, SimdArchType>(constant1, constant2, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcAddSatResult, constant3, halfCount, "mmasked src add sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcAddSatResult, constant1, oddMask, halfCount, "mmasked src add sat remain");

		simd_register addSatOverflowMaxResult = nasimd::simd_add_sat<DataType, SimdArchType>(constantMax, constantMax);
		testOpIfEqaul<DataType, SimdArchType>(test, addSatOverflowMaxResult, constantMax, "add sat overflow max");

		simd_register mmaskedAddSatOverflowMaxResult = nasimd::simd_mmasked_add_sat<DataType, SimdArchType>(constantMax, constantMax, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedAddSatOverflowMaxResult, constantMax, halfCount, "mmasked add sat overflow max");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedAddSatOverflowMaxResult, constant0, oddMask, halfCount, "mmasked add sat overflow max remain");

		simd_register mmaskedSrcAddSatOverflowMaxResult = nasimd::simd_mmasked_src_add_sat<DataType, SimdArchType>(constantMax, constantMax, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcAddSatOverflowMaxResult, constantMax, halfCount, "mmasked src add sat overflow max");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcAddSatOverflowMaxResult, constant1, oddMask, halfCount, "mmasked src add sat overflow max remain");

		simd_register addSatOverflowMinResult = nasimd::simd_add_sat<DataType, SimdArchType>(constantMin, constantMin);
		testOpIfEqaul<DataType, SimdArchType>(test, addSatOverflowMinResult, constantMin, "add sat overflow min");

		simd_register mmaskedAddSatOverflowMinResult = nasimd::simd_mmasked_add_sat<DataType, SimdArchType>(constantMin, constantMin, evenMask);
		if constexpr (natl::IsBuiltInSignedIntegerC<DataType>) {
			testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedAddSatOverflowMinResult, constantMin, halfCount, "mmasked add sat overflow min");
			testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedAddSatOverflowMinResult, constant0, oddMask, halfCount, "mmasked add sat overflow min remain");
		} else {
			testOpIfEqaul<DataType, SimdArchType>(test, mmaskedAddSatOverflowMinResult, constantMin, "mmasked add sat overflow min");
		}

		simd_register mmaskedSrcAddSatOverflowMinResult = nasimd::simd_mmasked_src_add_sat<DataType, SimdArchType>(constantMin, constantMin, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcAddSatOverflowMinResult, constantMin, halfCount, "mmasked src add sat overflow min");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcAddSatOverflowMinResult, constant1, oddMask, halfCount, "mmasked src add sat overflow min remain");
	}

	//sub sat 
	if constexpr (natl::IsBuiltInIntegerC<DataType>) {
		simd_register subSatResult = nasimd::simd_sub_sat<DataType, SimdArchType>(constant3, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, subSatResult, constant1, "sub sat");

		simd_register mmaskedSubSatResult = nasimd::simd_mmasked_sub_sat<DataType, SimdArchType>(constant3, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSubSatResult, constant1, halfCount, "mmasked sub sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSubSatResult, constant0, oddMask, halfCount, "mmasked sub sat remain");

		simd_register mmaskedSrcSubSatResult = nasimd::simd_mmasked_src_sub_sat<DataType, SimdArchType>(constant3, constant2, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcSubSatResult, constant1, halfCount, "mmasked src sub sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcSubSatResult, constant2, oddMask, halfCount, "mmasked src sub sat remain");

		simd_register subSatOverflowMaxResult = nasimd::simd_sub_sat<DataType, SimdArchType>(constantMax, constantMin);
		testOpIfEqaul<DataType, SimdArchType>(test, subSatOverflowMaxResult, constantMax, "sub sat overflow max");

		simd_register mmaskedSubSatOverflowMaxResult = nasimd::simd_mmasked_sub_sat<DataType, SimdArchType>(constantMax, constantMin, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSubSatOverflowMaxResult, constantMax, halfCount, "mmasked sub sat overflow max");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSubSatOverflowMaxResult, constant0, oddMask, halfCount, "mmasked sub sat overflow max remain");

		simd_register mmaskedSrcSubSatOverflowMaxResult = nasimd::simd_mmasked_src_sub_sat<DataType, SimdArchType>(constantMax, constantMin, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcSubSatOverflowMaxResult, constantMax, halfCount, "mmasked src sub sat overflow max" );
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcSubSatOverflowMaxResult, constant1, oddMask, halfCount, "mmasked src sub sat overflow max remain");

		simd_register subSatOverflowMinResult = nasimd::simd_sub_sat<DataType, SimdArchType>(constantMin, constantMax);
		testOpIfEqaul<DataType, SimdArchType>(test, subSatOverflowMinResult, constantMin, "sub sat overflow min");

		simd_register mmaskedSubSatOverflowMinResult = nasimd::simd_mmasked_sub_sat<DataType, SimdArchType>(constantMin, constantMax, evenMask);
		if constexpr (natl::IsBuiltInSignedIntegerC<DataType>) {
			testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSubSatOverflowMinResult, constantMin, halfCount, "mmasked sub sat overflow min");
			testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSubSatOverflowMinResult, constant0, oddMask, halfCount, "mmasked sub sat overflow min remain");
		} else {
			testOpIfEqaul<DataType, SimdArchType>(test, mmaskedSubSatOverflowMinResult, constantMin, "mmasked sub sat overflow min");
		}

		simd_register mmaskedSrcSubSatOverflowMinResult = nasimd::simd_mmasked_src_sub_sat<DataType, SimdArchType>(constantMin, constantMax, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcSubSatOverflowMinResult, constantMin, halfCount, "mmasked src sub sat overflow min");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcSubSatOverflowMinResult, constant1, oddMask, halfCount, "mmasked src sub sat overflow min remain");
	}

	//mul sat 
	if constexpr (natl::IsBuiltInIntegerC<DataType>) {
		simd_register mulSatResult = nasimd::simd_mul_sat<DataType, SimdArchType>(constant2, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, mulSatResult, constant4, "mul sat");

		simd_register mmaskedMulSatResult = nasimd::simd_mmasked_mul_sat<DataType, SimdArchType>(constant2, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedMulSatResult, constant4, halfCount, "mmasked mul sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedMulSatResult, constant0, oddMask, halfCount, "mmasked mul sat remain");

		simd_register mmaskedSrcMulSatResult = nasimd::simd_mmasked_src_mul_sat<DataType, SimdArchType>(constant2, constant2, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcMulSatResult, constant4, halfCount, "mmasked src mul sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcMulSatResult, constant1, oddMask, halfCount, "mmasked src mul sat remain");

		simd_register mulSatOverflowMaxResult = nasimd::simd_mul_sat<DataType, SimdArchType>(constantMax, constantMax);
		testOpIfEqaul<DataType, SimdArchType>(test, mulSatOverflowMaxResult, constantMax, "mul sat overflow max");

		simd_register mmaskedMulSatOverflowMaxResult = nasimd::simd_mmasked_mul_sat<DataType, SimdArchType>(constantMax, constantMax, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedMulSatOverflowMaxResult, constantMax, halfCount, "mmasked mul sat overflow max");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedMulSatOverflowMaxResult, constant0, oddMask, halfCount, "mmasked mul sat overflow max remain");

		simd_register mmaskedSrcMulSatOverflowMaxResult = nasimd::simd_mmasked_src_mul_sat<DataType, SimdArchType>(constantMax, constantMax, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcMulSatOverflowMaxResult, constantMax, halfCount, "mmasked src mul sat overflow max");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcMulSatOverflowMaxResult, constant1, oddMask, halfCount, "mmasked src mul sat overflow max remain");

		simd_register mulSatOverflowMinResult = nasimd::simd_mul_sat<DataType, SimdArchType>(constantMin, constantMax);
		testOpIfEqaul<DataType, SimdArchType>(test, mulSatOverflowMinResult, constantMin, "mul sat overflow min");

		simd_register mmaskedMulSatOverflowMinResult = nasimd::simd_mmasked_mul_sat<DataType, SimdArchType>(constantMin, constantMax, evenMask);
		if constexpr (natl::IsBuiltInSignedIntegerC<DataType>) {
			testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedMulSatOverflowMinResult, constantMin, halfCount, "mmasked mul sat overflow min");
			testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedMulSatOverflowMinResult, constant0, oddMask, halfCount, "mmasked mul sat overflow min remain");
		} else {
			testOpIfEqaul<DataType, SimdArchType>(test, mmaskedMulSatOverflowMinResult, constantMin, "mmasked mul sat overflow min");
		}

		simd_register mmaskedSrcMulSatOverflowMinResult = nasimd::simd_mmasked_src_mul_sat<DataType, SimdArchType>(constantMin, constantMax, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcMulSatOverflowMinResult, constantMin, halfCount, "mmasked src mul sat overflow max");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcMulSatOverflowMinResult, constant1, oddMask, halfCount, "mmasked src mul sat overflow max remain");
	}
	
	//mul div 
	if constexpr (natl::IsBuiltInIntegerC<DataType>) {
		simd_register divSatResult = nasimd::simd_div_sat<DataType, SimdArchType>(constant4, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, divSatResult, constant2, "div sat");

		simd_register mmaskedDivSatResult = nasimd::simd_mmasked_div_sat<DataType, SimdArchType>(constant4, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedDivSatResult, constant2, halfCount, "mmasked div sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedDivSatResult, constant0, oddMask, halfCount, "mmasked div sat remain");

		simd_register mmaskedSrcDivSatResult = nasimd::simd_mmasked_src_div_sat<DataType, SimdArchType>(constant4, constant2, constant1, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcDivSatResult, constant2, halfCount, "mmasked src div sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcDivSatResult, constant1, oddMask, halfCount, "mmasked src div sat remain");

		if constexpr (natl::IsBuiltInSignedIntegerC<DataType>) {
			simd_register constantNeg1 = nasimd::simd_set<DataType, SimdArchType>(DataType(-1));

			simd_register divSatOverflowMaxResult = nasimd::simd_div_sat<DataType, SimdArchType>(constantMin, constantNeg1);
			testOpIfEqaul<DataType, SimdArchType>(test, divSatOverflowMaxResult, constantMax, "div sat overflow max");

			simd_register mmaskedDivSatOverflowMaxResult = nasimd::simd_mmasked_div_sat<DataType, SimdArchType>(constantMin, constantNeg1, evenMask);
			testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedDivSatOverflowMaxResult, constantMax, halfCount, "mmasked div sat overflow max");
			testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedDivSatOverflowMaxResult, constant0, oddMask, halfCount, "mmasked div sat overflow max remain");

			simd_register mmaskedSrcDivSatOverflowMaxResult = nasimd::simd_mmasked_src_div_sat<DataType, SimdArchType>(constantMin, constantNeg1, constant1, evenMask);
			testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcDivSatOverflowMaxResult, constantMax, halfCount, "mmasked src div sat overflow max");
			testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcDivSatOverflowMaxResult, constant1, oddMask, halfCount, "mmasked src div sat overflow max remain");
		}
	}

	//remainder
	if constexpr(natl::IsBuiltInIntegerC<DataType>) {
		simd_register remainderResult = nasimd::simd_remainder<DataType, SimdArchType>(constant3, constant2);
		testOpIfEqaul<DataType, SimdArchType>(test, remainderResult, constant1, "remainder");
		
		simd_register mmaskedRemainderResult = nasimd::simd_mmasked_remainder<DataType, SimdArchType>(constant3, constant2, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedRemainderResult, constant1, halfCount, "mmasked add sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedRemainderResult, constant0, oddMask, halfCount, "mmasked add sat remain");

		simd_register mmaskedSrcRemainderResult = nasimd::simd_mmasked_src_remainder<DataType, SimdArchType>(constant3, constant2, constant4, evenMask);
		testOpIfEqaulCount<DataType, SimdArchType>(test, mmaskedSrcRemainderResult, constant1, halfCount, "mmasked src add sat");
		testOpIfEqaulMaskedCount<DataType, SimdArchType>(test, mmaskedSrcRemainderResult, constant4, oddMask, halfCount, "mmasked src add sat remain");
	}

	return test;
}


template<typename DataType, typename SimdArchType>
constexpr natl::Bool genericTypeSimdTest() noexcept {
	natl::Test test(natlTestFrom, natl::getNameOfBuiltInNumeric<DataType>(), natl::TestType::node);
	constructionTest<DataType, SimdArchType>();
	compareTest<DataType, SimdArchType>();
	arithmeticTest<DataType, SimdArchType>();
	return test;
}

template<typename SimdArchType>
constexpr natl::Bool genericSimdArchTest() noexcept {
	natl::ConstAsciiStringView simdArchName = natl::getNameOfType<SimdArchType>();
	natl::Test test(natlTestFrom, simdArchName, natl::TestType::node);
	natl::subTestAssert(test, genericTypeSimdTest<natl::i8, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::i16, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::i32, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::i64, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::ui8, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::ui16, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::ui32, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::ui64, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::f32, SimdArchType>());
	natl::subTestAssert(test, genericTypeSimdTest<natl::f64, SimdArchType>());
	return test;
}

natl::Bool tests() noexcept {
	natl::TestTimer timer(natlTestFrom);
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, genericSimdArchTest<nasimd::StandardArch<256>>());
	return test;
}

int main() noexcept {
	tests();
}