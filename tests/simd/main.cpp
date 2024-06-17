
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


	//add
	{
		simd_register addResult = nasimd::simd_add<DataType, SimdArchType>(constant1, constant2);
		simd_mmask addMask = nasimd::simd_compare_equal<DataType, SimdArchType>(addResult, constant3);
		natl::Bool addCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(addMask);
		natl::testAssert(test, addCondition, "add");

		simd_register mmaskedAddResult = nasimd::simd_mmasked_add<DataType, SimdArchType>(constant1, constant2, evenMask);
		simd_mmask mmaskedAddMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedAddResult, constant3);
		natl::Bool mmaskedAddCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedAddMask) == halfCount ;
		natl::testAssert(test, mmaskedAddCondition, "mmasked add");

		simd_mmask mmaskedAddRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedAddResult, constant0, oddMask);
		natl::Bool mmaskedAddRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedAddRemainMask) == halfCount;
		natl::testAssert(test, mmaskedAddRemainCondition, "mmasked add remain");

		simd_register mmaskedSrcAddResult = nasimd::simd_mmasked_src_add<DataType, SimdArchType>(constant1, constant2, constant1, evenMask);
		simd_mmask mmaskedSrcAddMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedSrcAddResult, constant3);
		natl::Bool mmaskedSrcAddCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcAddMask) == halfCount;
		natl::testAssert(test, mmaskedSrcAddCondition, "mmasked src add");

		simd_mmask mmaskedSrcAddRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedSrcAddResult, constant1, oddMask);
		natl::Bool mmaskedSrcAddRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcAddRemainMask) == halfCount;
		natl::testAssert(test, mmaskedSrcAddRemainCondition, "mmasked src add remain");
	}

	//sub
	{
		simd_register subResult = nasimd::simd_sub<DataType, SimdArchType>(constant3, constant2);
		simd_mmask subMask = nasimd::simd_compare_equal<DataType, SimdArchType>(subResult, constant1);
		natl::Bool subCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(subMask);
		natl::testAssert(test, subCondition, "sub");

		simd_register mmaskedSubResult = nasimd::simd_mmasked_sub<DataType, SimdArchType>(constant3, constant2, evenMask);
		simd_mmask mmaskedSubMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedSubResult, constant1);
		natl::Bool mmaskedSubCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSubMask) == halfCount;
		natl::testAssert(test, mmaskedSubCondition, "mmasked sub");

		simd_mmask mmaskedSubRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedSubResult, constant0, oddMask);
		natl::Bool mmaskedSubRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSubRemainMask) == halfCount;
		natl::testAssert(test, mmaskedSubRemainCondition, "mmasked sub remain");

		simd_register mmaskedSrcSubResult = nasimd::simd_mmasked_src_sub<DataType, SimdArchType>(constant3, constant2, constant2, evenMask);
		simd_mmask mmaskedSrcSubMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedSrcSubResult, constant1);
		natl::Bool mmaskedSrcSubCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcSubMask) == halfCount;
		natl::testAssert(test, mmaskedSrcSubCondition, "mmasked src sub");

		simd_mmask mmaskedSrcSubRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedSrcSubResult, constant2, oddMask);
		natl::Bool mmaskedSrcSubRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcSubRemainMask) == halfCount;
		natl::testAssert(test, mmaskedSrcSubRemainCondition, "mmasked src sub remain");
	}

	//mul
	{
		simd_register mulResult = nasimd::simd_mul<DataType, SimdArchType>(constant2, constant2);
		simd_mmask mulMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mulResult, constant4);
		natl::Bool mulCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(mulMask);
		natl::testAssert(test, mulCondition, "mul");

		simd_register mmaskedMulResult = nasimd::simd_mmasked_mul<DataType, SimdArchType>(constant2, constant2, evenMask);
		simd_mmask mmaskedMulMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedMulResult, constant4);
		natl::Bool mmaskedMulCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedMulMask) == halfCount;
		natl::testAssert(test, mmaskedMulCondition, "mmasked mul");

		simd_mmask mmaskedMulRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedMulResult, constant0, oddMask);
		natl::Bool mmaskedMulRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedMulRemainMask) == halfCount;
		natl::testAssert(test, mmaskedMulRemainCondition, "mmasked mul remain");

		simd_register mmaskedSrcMulResult = nasimd::simd_mmasked_src_mul<DataType, SimdArchType>(constant2, constant2, constant1, evenMask);
		simd_mmask mmaskedSrcMulMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedSrcMulResult, constant4);
		natl::Bool mmaskedSrcMulCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcMulMask) == halfCount;
		natl::testAssert(test, mmaskedSrcMulCondition, "mmasked src mul");

		simd_mmask mmaskedSrcMulRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedSrcMulResult, constant1, oddMask);
		natl::Bool mmaskedSrcMulRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcMulRemainMask) == halfCount;
		natl::testAssert(test, mmaskedSrcMulRemainCondition, "mmasked src mul remain");
	}

	//div
	{
		simd_register divResult = nasimd::simd_div<DataType, SimdArchType>(constant4, constant2);
		simd_mmask divMask = nasimd::simd_compare_equal<DataType, SimdArchType>(divResult, constant2);
		natl::Bool divCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(divMask);
		natl::testAssert(test, divCondition, "div");

		simd_register mmaskedDivResult = nasimd::simd_mmasked_div<DataType, SimdArchType>(constant4, constant2, evenMask);
		simd_mmask mmaskedDivMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedDivResult, constant2);
		natl::Bool mmaskedDivCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedDivMask) == halfCount;
		natl::testAssert(test, mmaskedDivCondition, "mmasked div");

		simd_mmask mmaskedDivRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedDivResult, constant0, oddMask);
		natl::Bool mmaskedDivRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedDivRemainMask) == halfCount;
		natl::testAssert(test, mmaskedDivRemainCondition, "mmasked div remain");

		simd_register mmaskedSrcDivResult = nasimd::simd_mmasked_src_div<DataType, SimdArchType>(constant4, constant2, constant1, evenMask);
		simd_mmask mmaskedSrcDivMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedSrcDivResult, constant2);
		natl::Bool mmaskedSrcDivCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcDivMask) == halfCount;
		natl::testAssert(test, mmaskedSrcDivCondition, "mmasked src div");

		simd_mmask mmaskedSrcDivRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedSrcDivResult, constant1, oddMask);
		natl::Bool mmaskedSrcDivRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcDivRemainMask) == halfCount;
		natl::testAssert(test, mmaskedSrcDivRemainCondition, "mmasked src div remain");
	}

	
	//remainder
	if constexpr(natl::IsBuiltInIntegerC<DataType>) {
		simd_register remainderResult = nasimd::simd_remainder<DataType, SimdArchType>(constant3, constant2);
		simd_mmask remainderMask = nasimd::simd_compare_equal<DataType, SimdArchType>(remainderResult, constant1);
		natl::Bool remainderCondition = nasimd::simd_mask_test_all_active<DataType, SimdArchType>(remainderMask);
		natl::testAssert(test, remainderCondition, "remainder");

		simd_register mmaskedRemainderResult = nasimd::simd_mmasked_remainder<DataType, SimdArchType>(constant3, constant2, evenMask);
		simd_mmask mmaskedRemainderMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedRemainderResult, constant1);
		natl::Bool mmaskedRemainderCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedRemainderMask) == halfCount;
		natl::testAssert(test, mmaskedRemainderCondition, "mmasked remainder");

		simd_mmask mmaskedRemainderRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedRemainderResult, constant0, oddMask);
		natl::Bool mmaskedRemainderRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedRemainderRemainMask) == halfCount;
		natl::testAssert(test, mmaskedRemainderRemainCondition, "mmasked remainder remain");

		simd_register mmaskedSrcRemainderResult = nasimd::simd_mmasked_src_remainder<DataType, SimdArchType>(constant3, constant2, constant4, evenMask);
		simd_mmask mmaskedSrcRemainderMask = nasimd::simd_compare_equal<DataType, SimdArchType>(mmaskedSrcRemainderResult, constant1);
		natl::Bool mmaskedSrcRemainderCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcRemainderMask) == halfCount;
		natl::testAssert(test, mmaskedSrcRemainderCondition, "mmasked src remainder");

		simd_mmask mmaskedSrcRemainderRemainMask = nasimd::simd_mmasked_compare_equal<DataType, SimdArchType>(mmaskedSrcRemainderResult, constant4, oddMask);
		natl::Bool mmaskedSrcRemainderRemainCondition = nasimd::simd_mask_popcount<DataType, SimdArchType>(mmaskedSrcRemainderRemainMask) == halfCount;
		natl::testAssert(test, mmaskedSrcRemainderRemainCondition, "mmasked src remainder remain");
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