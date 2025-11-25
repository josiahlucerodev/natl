//@file_not_module

//@begin_non_modules
//std
#include <cmath>

//natl
#define NATL_DISABLE_CONSTEXPR_TESTS
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/simd/all.h>
#include <natl/util/typeInfo.h>
#include <natl/util/test.h>

template<typename Type, typename SimdArchType>
struct SimdTestSuite : public natl::TestSuite<SimdTestSuite<Type, SimdArchType>> {
	using type = Type;
	using simd_arch_type = SimdArchType;
};

using standard_arch = nasimd::StandardArch<256>;

template<typename test_suite_type = SimdTestSuite<natl::i8, standard_arch>>
struct SimdFloatTestSuiteTests {};

template<typename test_suite_type>
	requires(natl::IsBuiltInFloatingPointC<natl::Decay<typename test_suite_type::type>>)
struct SimdFloatTestSuiteTests<test_suite_type> {
	using type = test_suite_type::type;
	using simd_arch_type = test_suite_type::simd_arch_type;

	constexpr static void testMathFunction(test_suite_type& ts,
		nasimd::SimdRegister<type, simd_arch_type>(*customFunc)(nasimd::SimdRegister<type, simd_arch_type>),
		nasimd::SimdRegister<type, simd_arch_type>(*mmaskedCustomFunc)(nasimd::SimdRegister<type, simd_arch_type>, nasimd::SimdMask<type, simd_arch_type>),
		nasimd::SimdRegister<type, simd_arch_type>(*mmaskedSrcCustomFunc)(nasimd::SimdRegister<type, simd_arch_type>, nasimd::SimdRegister<type, simd_arch_type>, nasimd::SimdMask<type, simd_arch_type>),
		type(*testFunc)(type),
		type testStart, type testEnd, type incrementNum,
		type relativeTolorance, type discardSmallDif) noexcept {

		constexpr natl::Size count = nasimd::SimdRegisterInfo<type, simd_arch_type>::count();
		constexpr natl::Size halfCount = count / 2;

		using simd_register = nasimd::SimdRegister<type, simd_arch_type>;
		using simd_mmask = nasimd::SimdMask<type, simd_arch_type>;

		simd_mmask evenMask = nasimd::createEvenMMask<type, simd_arch_type>();
		simd_mmask oddMask = nasimd::createOddMMask<type, simd_arch_type>();
		simd_register constantMax = nasimd::simd_set<type, simd_arch_type>(natl::Limits<type>::max());

		for (type testValue = testStart; testValue < testEnd; testValue += (incrementNum * count)) {

			natl::Array<type, count> accurateValuesArray;
			natl::Array<type, count> testValuesArray;
			for (natl::Size i = 0; i < count; i++) {
				testValuesArray[i] = testValue + (incrementNum * static_cast<type>(i));
				accurateValuesArray[i] = testFunc(testValuesArray[i]);
			}
			simd_register testValues = nasimd::simd_load<type, simd_arch_type>(testValuesArray.data());
			simd_register accurateValues = nasimd::simd_load<type, simd_arch_type>(accurateValuesArray.data());
			simd_register customValues = customFunc(testValues);

			simd_mmask relativelyCloseMask = natlm::isRelativelyClose<type, simd_arch_type>(customValues, accurateValues, relativeTolorance);
			simd_mmask roughtlyEqualMask = natlm::isRoughtlyEqual<type, simd_arch_type>(customValues, accurateValues, discardSmallDif);
			simd_mmask closeEnoughMask = nasimd::simd_mask_bitwise_or<type, simd_arch_type>(relativelyCloseMask, roughtlyEqualMask);
			natl::Bool condition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(closeEnoughMask);
			ts.assertTrue(condition);

			simd_register mmaskedCustomValues = mmaskedCustomFunc(testValues, evenMask);
			simd_mmask mmaskedRelativelyCloseMask = natlm::isRelativelyClose<type, simd_arch_type>(mmaskedCustomValues, accurateValues, relativeTolorance);
			simd_mmask mmaskedRoughtlyEqualMask = natlm::isRoughtlyEqual<type, simd_arch_type>(mmaskedCustomValues, accurateValues, discardSmallDif);
			simd_mmask mmaskedCloseEnoughMask = nasimd::simd_mask_bitwise_or<type, simd_arch_type>(mmaskedRelativelyCloseMask, mmaskedRoughtlyEqualMask);
			mmaskedCloseEnoughMask = nasimd::simd_mask_bitwise_and<type, simd_arch_type>(mmaskedCloseEnoughMask, evenMask);
			natl::Bool mmaskedCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCloseEnoughMask) == halfCount;
			ts.assertTrue(mmaskedCondition);

			simd_mmask mmaskedRemainMask = nasimd::simd_mmasked_compare_equal<type, simd_arch_type>(mmaskedCustomValues, testValues, oddMask);
			natl::Bool mmaskedRemainCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedRemainMask) == halfCount;
			ts.assertTrue(mmaskedRemainCondition);

			simd_register mmaskedSrcCustomValues = mmaskedSrcCustomFunc(testValues, constantMax, evenMask);
			simd_mmask mmaskedSrcRelativelyCloseMask = natlm::isRelativelyClose<type, simd_arch_type>(mmaskedSrcCustomValues, accurateValues, relativeTolorance);
			simd_mmask mmaskedSrcRoughtlyEqualMask = natlm::isRoughtlyEqual<type, simd_arch_type>(mmaskedSrcCustomValues, accurateValues, discardSmallDif);
			simd_mmask mmaskedSrcCloseEnoughMask = nasimd::simd_mask_bitwise_or<type, simd_arch_type>(mmaskedSrcRelativelyCloseMask, mmaskedSrcRoughtlyEqualMask);
			mmaskedSrcCloseEnoughMask = nasimd::simd_mask_bitwise_and<type, simd_arch_type>(mmaskedSrcCloseEnoughMask, evenMask);
			natl::Bool mmaskedSrcCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCloseEnoughMask) == halfCount;
			ts.assertTrue(mmaskedSrcCondition);

			simd_mmask mmaskedSrcRemainMask = nasimd::simd_mmasked_compare_equal<type, simd_arch_type>(mmaskedSrcCustomValues, constantMax, oddMask);
			natl::Bool mmaskedSrcRemainCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcRemainMask) == halfCount;
			ts.assertTrue(mmaskedSrcRemainCondition);
		}
	}

	constexpr static void powerTest(test_suite_type& ts) noexcept {
		using namespace natl::literals;
		if constexpr (sizeof(type) == 4) {
			testMathFunction(ts,
				nasimd::simd_square_root<type, simd_arch_type>,
				nasimd::simd_mmasked_square_root<type, simd_arch_type>,
				nasimd::simd_mmasked_src_square_root<type, simd_arch_type>,
				sqrtf,
				0.0_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);

			testMathFunction(ts,
				nasimd::simd_reciprocal_square_root<type, simd_arch_type>,
				nasimd::simd_mmasked_reciprocal_square_root<type, simd_arch_type>,
				nasimd::simd_mmasked_src_reciprocal_square_root<type, simd_arch_type>,
				[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
				0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);

			testMathFunction(ts,
				nasimd::simd_reciprocal<type, simd_arch_type>,
				nasimd::simd_mmasked_reciprocal<type, simd_arch_type>,
				nasimd::simd_mmasked_src_reciprocal<type, simd_arch_type>,
				[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / value; },
				0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		} else {
			testMathFunction(ts,
				nasimd::simd_square_root<type, simd_arch_type>,
				nasimd::simd_mmasked_square_root<type, simd_arch_type>,
				nasimd::simd_mmasked_src_square_root<type, simd_arch_type>,
				sqrt,
				0.0_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);

			testMathFunction(ts,
				nasimd::simd_reciprocal_square_root<type, simd_arch_type>,
				nasimd::simd_mmasked_reciprocal_square_root<type, simd_arch_type>,
				nasimd::simd_mmasked_src_reciprocal_square_root<type, simd_arch_type>,
				[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
				0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);

			testMathFunction(ts,
				nasimd::simd_reciprocal<type, simd_arch_type>,
				nasimd::simd_mmasked_reciprocal<type, simd_arch_type>,
				nasimd::simd_mmasked_src_reciprocal<type, simd_arch_type>,
				[](const natl::f64 value) -> natl::f64 { return 1.0 / value; },
				0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(powerTest);
};


template<typename test_suite_type = SimdTestSuite<natl::i8, standard_arch>>
struct SimdTestSuiteTests : public SimdFloatTestSuiteTests<test_suite_type> {
	using type = test_suite_type::type;
	using simd_arch_type = test_suite_type::simd_arch_type;

	constexpr static void constructionTest(test_suite_type& ts) noexcept {
		[[maybe_unused]] nasimd::SimdRegister<type, simd_arch_type> simdRegister;
		[[maybe_unused]] nasimd::SimdRegisterInfo<type, simd_arch_type> simdRegisterInfo;
		[[maybe_unused]] nasimd::SimdMask<type, simd_arch_type> simdMask;
		[[maybe_unused]] nasimd::SimdMaskInfo<type, simd_arch_type> simdMaskInfo;
		[[maybe_unused]] nasimd::SimdCMask<type, simd_arch_type> simdCmask;
		[[maybe_unused]] nasimd::SimdCMaskInfo<type, simd_arch_type> simdCmaskInfo;
		ts.assertLt(0, sizeof(decltype(simdRegister)));
		ts.assertLt(0, sizeof(decltype(simdRegisterInfo)));
		ts.assertLt(0, sizeof(decltype(simdMask)));
		ts.assertLt(0, sizeof(decltype(simdMaskInfo)));
		ts.assertLt(0, sizeof(decltype(simdCmask)));
		ts.assertLt(0, sizeof(decltype(simdCmaskInfo)));
	}
	NATL_REGISTER_TEST_CONSTEXPR(constructionTest);

	constexpr static void compareTest(test_suite_type& ts) noexcept {
		using simd_register = nasimd::SimdRegister<type, simd_arch_type>;
		using simd_mmask = nasimd::SimdMask<type, simd_arch_type>;
		constexpr natl::Size halfCount = nasimd::SimdRegisterToInfo<simd_register>::count() / 2;

		simd_mmask evenMask = nasimd::createEvenMMask<type, simd_arch_type>();
		simd_mmask oddMask = nasimd::createOddMMask<type, simd_arch_type>();
		simd_register constant1 = nasimd::simd_set<type, simd_arch_type>(1);
		simd_register constant2 = nasimd::simd_set<type, simd_arch_type>(2);
		simd_register constant3 = nasimd::simd_set<type, simd_arch_type>(3);

		//equal 
		{
			simd_mmask compareEqualTrueMask = nasimd::simd_compare_equal<type, simd_arch_type>(constant2, constant2);
			natl::Bool compereEqualTrueCondition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(compareEqualTrueMask);
			ts.assertTrue(compereEqualTrueCondition);

			simd_mmask compareEqualFalseMask = nasimd::simd_compare_equal<type, simd_arch_type>(constant1, constant2);
			natl::Bool compereEqualFalseCondition = nasimd::simd_mask_test_all_inactive<type, simd_arch_type>(compareEqualFalseMask);
			ts.assertTrue(compereEqualFalseCondition);

			simd_mmask mmaskedCompareEqualTrueMask = nasimd::simd_mmasked_compare_equal<type, simd_arch_type>(constant2, constant2, evenMask);
			natl::Bool mmaskedCompareEqualTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareEqualTrueMask) == halfCount;
			ts.assertTrue(mmaskedCompareEqualTrueCondition);

			simd_mmask mmaskedCompareEqualFalseMask = nasimd::simd_mmasked_compare_equal<type, simd_arch_type>(constant1, constant2, evenMask);
			natl::Bool mmaskedCompareEqualFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareEqualFalseMask) == 0;
			ts.assertTrue(mmaskedCompareEqualFalseCondition);

			simd_mmask mmaskedSrcCompareEqualTrueMask = nasimd::simd_mmasked_src_compare_equal<type, simd_arch_type>(constant2, constant2, evenMask, evenMask);
			natl::Bool mmaskedSrcCompareEqualTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareEqualTrueMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareEqualTrueCondition);

			simd_mmask mmaskedSrcCompareEqualFalseMask = nasimd::simd_mmasked_src_compare_equal<type, simd_arch_type>(constant1, constant2, oddMask, evenMask);
			natl::Bool mmaskedSrcCompareEqualFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareEqualFalseMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareEqualFalseCondition);
		}

		//not equal   
		{
			simd_mmask compareNotEqualTrueMask = nasimd::simd_compare_not_equal<type, simd_arch_type>(constant1, constant2);
			natl::Bool compereNotEqualTrueCondition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(compareNotEqualTrueMask);
			ts.assertTrue(compereNotEqualTrueCondition);

			simd_mmask compareNotEqualFalseMask = nasimd::simd_compare_not_equal<type, simd_arch_type>(constant2, constant2);
			natl::Bool compereNotEqualFalseCondition = nasimd::simd_mask_test_all_inactive<type, simd_arch_type>(compareNotEqualFalseMask);
			ts.assertTrue(compereNotEqualFalseCondition);

			simd_mmask mmaskedCompareNotEqualTrueMask = nasimd::simd_mmasked_compare_not_equal<type, simd_arch_type>(constant1, constant2, evenMask);
			natl::Bool mmaskedCompareNotEqualTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareNotEqualTrueMask) == halfCount;
			ts.assertTrue(mmaskedCompareNotEqualTrueCondition);

			simd_mmask mmaskedCompareNotEqualFalseMask = nasimd::simd_mmasked_compare_not_equal<type, simd_arch_type>(constant2, constant2, evenMask);
			natl::Bool mmaskedCompareNotEqualFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareNotEqualFalseMask) == 0;
			ts.assertTrue(mmaskedCompareNotEqualFalseCondition);

			simd_mmask mmaskedSrcCompareNotEqualTrueMask = nasimd::simd_mmasked_src_compare_not_equal<type, simd_arch_type>(constant1, constant2, evenMask, evenMask);
			natl::Bool mmaskedSrcCompareNotEqualTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareNotEqualTrueMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareNotEqualTrueCondition);

			simd_mmask mmaskedSrcCompareNotEqualFalseMask = nasimd::simd_mmasked_src_compare_not_equal<type, simd_arch_type>(constant2, constant2, oddMask, evenMask);
			natl::Bool mmaskedSrcCompareNotEqualFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareNotEqualFalseMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareNotEqualFalseCondition);
		}

		//less than   
		{
			simd_mmask compareLessThanTrueMask = nasimd::simd_compare_less_than<type, simd_arch_type>(constant1, constant2);
			natl::Bool compereLessThanTrueCondition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(compareLessThanTrueMask);
			ts.assertTrue(compereLessThanTrueCondition);

			simd_mmask compareLessThanFalseMask = nasimd::simd_compare_less_than<type, simd_arch_type>(constant2, constant2);
			natl::Bool compereLessThanFalseCondition = nasimd::simd_mask_test_all_inactive<type, simd_arch_type>(compareLessThanFalseMask);
			ts.assertTrue(compereLessThanFalseCondition);

			simd_mmask mmaskedCompareLessThanTrueMask = nasimd::simd_mmasked_compare_less_than<type, simd_arch_type>(constant1, constant2, evenMask);
			natl::Bool mmaskedCompareLessThanTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareLessThanTrueMask) == halfCount;
			ts.assertTrue(mmaskedCompareLessThanTrueCondition);

			simd_mmask mmaskedCompareLessThanFalseMask = nasimd::simd_mmasked_compare_less_than<type, simd_arch_type>(constant2, constant2, evenMask);
			natl::Bool mmaskedCompareLessThanFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareLessThanFalseMask) == 0;
			ts.assertTrue(mmaskedCompareLessThanFalseCondition);

			simd_mmask mmaskedSrcCompareLessThanTrueMask = nasimd::simd_mmasked_src_compare_less_than<type, simd_arch_type>(constant1, constant2, evenMask, evenMask);
			natl::Bool mmaskedSrcCompareLessThanTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareLessThanTrueMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareLessThanTrueCondition);

			simd_mmask mmaskedSrcCompareLessThanFalseMask = nasimd::simd_mmasked_src_compare_less_than<type, simd_arch_type>(constant2, constant2, oddMask, evenMask);
			natl::Bool mmaskedSrcCompareLessThanFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareLessThanFalseMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareLessThanFalseCondition);
		}

		//greater than   
		{
			simd_mmask compareGreaterThanTrueMask = nasimd::simd_compare_greater_than<type, simd_arch_type>(constant2, constant1);
			natl::Bool compereGreaterThanTrueCondition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(compareGreaterThanTrueMask);
			ts.assertTrue(compereGreaterThanTrueCondition);

			simd_mmask compareGreaterThanFalseMask = nasimd::simd_compare_greater_than<type, simd_arch_type>(constant2, constant2);
			natl::Bool compereGreaterThanFalseCondition = nasimd::simd_mask_test_all_inactive<type, simd_arch_type>(compareGreaterThanFalseMask);
			ts.assertTrue(compereGreaterThanFalseCondition);

			simd_mmask mmaskedCompareGreaterThanTrueMask = nasimd::simd_mmasked_compare_greater_than<type, simd_arch_type>(constant2, constant1, evenMask);
			natl::Bool mmaskedCompareGreaterThanTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareGreaterThanTrueMask) == halfCount;
			ts.assertTrue(mmaskedCompareGreaterThanTrueCondition);

			simd_mmask mmaskedCompareGreaterThanFalseMask = nasimd::simd_mmasked_compare_greater_than<type, simd_arch_type>(constant2, constant2, evenMask);
			natl::Bool mmaskedCompareGreaterThanFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareGreaterThanFalseMask) == 0;
			ts.assertTrue(mmaskedCompareGreaterThanFalseCondition);

			simd_mmask mmaskedSrcCompareGreaterThanTrueMask = nasimd::simd_mmasked_src_compare_greater_than<type, simd_arch_type>(constant2, constant1, evenMask, evenMask);
			natl::Bool mmaskedSrcCompareGreaterThanTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareGreaterThanTrueMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareGreaterThanTrueCondition);

			simd_mmask mmaskedSrcCompareGreaterThanFalseMask = nasimd::simd_mmasked_src_compare_greater_than<type, simd_arch_type>(constant2, constant2, oddMask, evenMask);
			natl::Bool mmaskedSrcCompareGreaterThanFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareGreaterThanFalseMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareGreaterThanFalseCondition);
		}

		//less than or equal
		{
			//equal
			simd_mmask compareEqualTrueMask = nasimd::simd_compare_less_than_or_equal<type, simd_arch_type>(constant2, constant2);
			natl::Bool compereEqualTrueCondition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(compareEqualTrueMask);
			ts.assertTrue(compereEqualTrueCondition);

			simd_mmask compareEqualFalseMask = nasimd::simd_compare_less_than_or_equal<type, simd_arch_type>(constant3, constant2);
			natl::Bool compereEqualFalseCondition = nasimd::simd_mask_test_all_inactive<type, simd_arch_type>(compareEqualFalseMask);
			ts.assertTrue(compereEqualFalseCondition);

			simd_mmask mmaskedCompareEqualTrueMask = nasimd::simd_mmasked_compare_less_than_or_equal<type, simd_arch_type>(constant2, constant2, evenMask);
			natl::Bool mmaskedCompareEqualTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareEqualTrueMask) == halfCount;
			ts.assertTrue(mmaskedCompareEqualTrueCondition);

			simd_mmask mmaskedCompareEqualFalseMask = nasimd::simd_mmasked_compare_less_than_or_equal<type, simd_arch_type>(constant3, constant2, evenMask);
			natl::Bool mmaskedCompareEqualFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareEqualFalseMask) == 0;
			ts.assertTrue(mmaskedCompareEqualFalseCondition);

			simd_mmask mmaskedSrcCompareEqualTrueMask = nasimd::simd_mmasked_src_compare_less_than_or_equal<type, simd_arch_type>(constant2, constant2, evenMask, evenMask);
			natl::Bool mmaskedSrcCompareEqualTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareEqualTrueMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareEqualTrueCondition);

			simd_mmask mmaskedSrcCompareEqualFalseMask = nasimd::simd_mmasked_src_compare_less_than_or_equal<type, simd_arch_type>(constant3, constant2, oddMask, evenMask);
			natl::Bool mmaskedSrcCompareEqualFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareEqualFalseMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareEqualFalseCondition);

			//less than
			simd_mmask compareLessThanTrueMask = nasimd::simd_compare_less_than_or_equal<type, simd_arch_type>(constant1, constant2);
			natl::Bool compereLessThanTrueCondition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(compareLessThanTrueMask);
			ts.assertTrue(compereLessThanTrueCondition);

			simd_mmask compareLessThanFalseMask = nasimd::simd_compare_less_than_or_equal<type, simd_arch_type>(constant3, constant2);
			natl::Bool compereLessThanFalseCondition = nasimd::simd_mask_test_all_inactive<type, simd_arch_type>(compareLessThanFalseMask);
			ts.assertTrue(compereLessThanFalseCondition);

			simd_mmask mmaskedCompareLessThanTrueMask = nasimd::simd_mmasked_compare_less_than_or_equal<type, simd_arch_type>(constant1, constant2, evenMask);
			natl::Bool mmaskedCompareLessThanTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareLessThanTrueMask) == halfCount;
			ts.assertTrue(mmaskedCompareLessThanTrueCondition);

			simd_mmask mmaskedCompareLessThanFalseMask = nasimd::simd_mmasked_compare_less_than_or_equal<type, simd_arch_type>(constant3, constant2, evenMask);
			natl::Bool mmaskedCompareLessThanFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareLessThanFalseMask) == 0;
			ts.assertTrue(mmaskedCompareLessThanFalseCondition);

			simd_mmask mmaskedSrcCompareLessThanTrueMask = nasimd::simd_mmasked_src_compare_less_than_or_equal<type, simd_arch_type>(constant1, constant2, evenMask, evenMask);
			natl::Bool mmaskedSrcCompareLessThanTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareLessThanTrueMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareLessThanTrueCondition);

			simd_mmask mmaskedSrcCompareLessThanFalseMask = nasimd::simd_mmasked_src_compare_less_than_or_equal<type, simd_arch_type>(constant3, constant2, oddMask, evenMask);
			natl::Bool mmaskedSrcCompareLessThanFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareLessThanFalseMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareLessThanFalseCondition);
		}

		//greater than or eqaul 
		{
			//eqaul 
			simd_mmask compareEqualTrueMask = nasimd::simd_compare_greater_than_or_equal<type, simd_arch_type>(constant2, constant2);
			natl::Bool compereEqualTrueCondition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(compareEqualTrueMask);
			ts.assertTrue(compereEqualTrueCondition);

			simd_mmask compareEqualFalseMask = nasimd::simd_compare_greater_than_or_equal<type, simd_arch_type>(constant1, constant2);
			natl::Bool compereEqualFalseCondition = nasimd::simd_mask_test_all_inactive<type, simd_arch_type>(compareEqualFalseMask);
			ts.assertTrue(compereEqualFalseCondition);

			simd_mmask mmaskedCompareEqualTrueMask = nasimd::simd_mmasked_compare_greater_than_or_equal<type, simd_arch_type>(constant2, constant2, evenMask);
			natl::Bool mmaskedCompareEqualTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareEqualTrueMask) == halfCount;
			ts.assertTrue(mmaskedCompareEqualTrueCondition);

			simd_mmask mmaskedCompareEqualFalseMask = nasimd::simd_mmasked_compare_greater_than_or_equal<type, simd_arch_type>(constant1, constant2, evenMask);
			natl::Bool mmaskedCompareEqualFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareEqualFalseMask) == 0;
			ts.assertTrue(mmaskedCompareEqualFalseCondition);

			simd_mmask mmaskedSrcCompareEqualTrueMask = nasimd::simd_mmasked_src_compare_greater_than_or_equal<type, simd_arch_type>(constant2, constant2, evenMask, evenMask);
			natl::Bool mmaskedSrcCompareEqualTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareEqualTrueMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareEqualTrueCondition);

			simd_mmask mmaskedSrcCompareEqualFalseMask = nasimd::simd_mmasked_src_compare_greater_than_or_equal<type, simd_arch_type>(constant1, constant2, oddMask, evenMask);
			natl::Bool mmaskedSrcCompareEqualFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareEqualFalseMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareEqualFalseCondition);

			//greater than 
			simd_mmask compareGreaterThanTrueMask = nasimd::simd_compare_greater_than_or_equal<type, simd_arch_type>(constant2, constant1);
			natl::Bool compereGreaterThanTrueCondition = nasimd::simd_mask_test_all_active<type, simd_arch_type>(compareGreaterThanTrueMask);
			ts.assertTrue(compereGreaterThanTrueCondition);

			simd_mmask compareGreaterThanFalseMask = nasimd::simd_compare_greater_than_or_equal<type, simd_arch_type>(constant1, constant2);
			natl::Bool compereGreaterThanFalseCondition = nasimd::simd_mask_test_all_inactive<type, simd_arch_type>(compareGreaterThanFalseMask);
			ts.assertTrue(compereGreaterThanFalseCondition);

			simd_mmask mmaskedCompareGreaterThanTrueMask = nasimd::simd_mmasked_compare_greater_than_or_equal<type, simd_arch_type>(constant2, constant1, evenMask);
			natl::Bool mmaskedCompareGreaterThanTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareGreaterThanTrueMask) == halfCount;
			ts.assertTrue(mmaskedCompareGreaterThanTrueCondition);

			simd_mmask mmaskedCompareGreaterThanFalseMask = nasimd::simd_mmasked_compare_greater_than_or_equal<type, simd_arch_type>(constant1, constant2, evenMask);
			natl::Bool mmaskedCompareGreaterThanFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedCompareGreaterThanFalseMask) == 0;
			ts.assertTrue(mmaskedCompareGreaterThanFalseCondition);

			simd_mmask mmaskedSrcCompareGreaterThanTrueMask = nasimd::simd_mmasked_src_compare_greater_than_or_equal<type, simd_arch_type>(constant2, constant1, evenMask, evenMask);
			natl::Bool mmaskedSrcCompareGreaterThanTrueCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareGreaterThanTrueMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareGreaterThanTrueCondition);

			simd_mmask mmaskedSrcCompareGreaterThanFalseMask = nasimd::simd_mmasked_src_compare_greater_than_or_equal<type, simd_arch_type>(constant1, constant2, oddMask, evenMask);
			natl::Bool mmaskedSrcCompareGreaterThanFalseCondition = nasimd::simd_mask_popcount<type, simd_arch_type>(mmaskedSrcCompareGreaterThanFalseMask) == halfCount;
			ts.assertTrue(mmaskedSrcCompareGreaterThanFalseCondition);
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(compareTest);

	constexpr static void testOpIfEqaul(test_suite_type& ts,
		nasimd::SimdRegister<type, simd_arch_type> result,
		nasimd::SimdRegister<type, simd_arch_type> expectedValue) noexcept {
		nasimd::SimdMask<type, simd_arch_type> mask = nasimd::simd_compare_equal<type, simd_arch_type>(result, expectedValue);
		ts.assertTrue(nasimd::simd_mask_test_all_active<type, simd_arch_type>(mask));
	}

	constexpr static void testOpIfEqaulCount(test_suite_type& ts,
		nasimd::SimdRegister<type, simd_arch_type> result,
		nasimd::SimdRegister<type, simd_arch_type> expectedValue,
		const natl::Size count) noexcept {
		nasimd::SimdMask<type, simd_arch_type> mask = nasimd::simd_compare_equal<type, simd_arch_type>(result, expectedValue);
		ts.assertEqual(count, nasimd::simd_mask_popcount<type, simd_arch_type>(mask));
	}

	constexpr static void testOpIfEqaulMaskedCount(test_suite_type& ts,
		nasimd::SimdRegister<type, simd_arch_type> result,
		nasimd::SimdRegister<type, simd_arch_type> expectedValue,
		nasimd::SimdMask<type, simd_arch_type> mmask,
		const natl::Size count) noexcept {
		nasimd::SimdMask<type, simd_arch_type> mask = nasimd::simd_mmasked_compare_equal<type, simd_arch_type>(result, expectedValue, mmask);
		ts.assertEqual(count, nasimd::simd_mask_popcount<type, simd_arch_type>(mask));
	}

	//arithmetic
	constexpr static void arithmeticTest(test_suite_type& ts) noexcept {
		using simd_register = nasimd::SimdRegister<type, simd_arch_type>;
		using simd_mmask = nasimd::SimdMask<type, simd_arch_type>;
		constexpr natl::Size halfCount = nasimd::SimdRegisterToInfo<simd_register>::count() / 2;

		simd_mmask evenMask = nasimd::createEvenMMask<type, simd_arch_type>();
		simd_mmask oddMask = nasimd::createOddMMask<type, simd_arch_type>();
		simd_register constant0 = nasimd::simd_set<type, simd_arch_type>(0);
		simd_register constant1 = nasimd::simd_set<type, simd_arch_type>(1);
		simd_register constant2 = nasimd::simd_set<type, simd_arch_type>(2);
		simd_register constant3 = nasimd::simd_set<type, simd_arch_type>(3);
		simd_register constant4 = nasimd::simd_set<type, simd_arch_type>(4);
		simd_register constant5 = nasimd::simd_set<type, simd_arch_type>(5);

		simd_register constantMax = nasimd::simd_set<type, simd_arch_type>(natl::Limits<type>::max());
		simd_register constantMin = nasimd::simd_set<type, simd_arch_type>(natl::Limits<type>::min());

		//add
		{
			simd_register addResult = nasimd::simd_add<type, simd_arch_type>(constant1, constant2);
			testOpIfEqaul(ts, addResult, constant3);

			simd_register mmaskedAddResult = nasimd::simd_mmasked_add<type, simd_arch_type>(constant1, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedAddResult, constant3, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedAddResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcAddResult = nasimd::simd_mmasked_src_add<type, simd_arch_type>(constant1, constant2, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcAddResult, constant3, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcAddResult, constant1, oddMask, halfCount);
		}

		//sub
		{
			simd_register subResult = nasimd::simd_sub<type, simd_arch_type>(constant3, constant2);
			testOpIfEqaul(ts, subResult, constant1);

			simd_register mmaskedSubResult = nasimd::simd_mmasked_sub<type, simd_arch_type>(constant3, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedSubResult, constant1, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSubResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcSubResult = nasimd::simd_mmasked_src_sub<type, simd_arch_type>(constant3, constant2, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcSubResult, constant1, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcSubResult, constant2, oddMask, halfCount);
		}

		//mul
		{
			simd_register mulResult = nasimd::simd_mul<type, simd_arch_type>(constant2, constant2);
			testOpIfEqaul(ts, mulResult, constant4);

			simd_register mmaskedMulResult = nasimd::simd_mmasked_mul<type, simd_arch_type>(constant2, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedMulResult, constant4, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedMulResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcMulResult = nasimd::simd_mmasked_src_mul<type, simd_arch_type>(constant2, constant2, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcMulResult, constant4, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcMulResult, constant1, oddMask, halfCount);
		}

		//div
		{
			simd_register divResult = nasimd::simd_div<type, simd_arch_type>(constant4, constant2);
			testOpIfEqaul(ts, divResult, constant2);

			simd_register mmaskedDivResult = nasimd::simd_mmasked_div<type, simd_arch_type>(constant4, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedDivResult, constant2, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedDivResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcDivResult = nasimd::simd_mmasked_src_div<type, simd_arch_type>(constant4, constant2, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcDivResult, constant2, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcDivResult, constant1, oddMask, halfCount);
		}

		//add sat 
		if constexpr (natl::IsBuiltInIntegerC<type>) {
			simd_register addSatResult = nasimd::simd_add_sat<type, simd_arch_type>(constant1, constant2);
			testOpIfEqaul(ts, addSatResult, constant3);

			simd_register mmaskedAddSatResult = nasimd::simd_mmasked_add_sat<type, simd_arch_type>(constant1, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedAddSatResult, constant3, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedAddSatResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcAddSatResult = nasimd::simd_mmasked_src_add_sat<type, simd_arch_type>(constant1, constant2, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcAddSatResult, constant3, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcAddSatResult, constant1, oddMask, halfCount);

			simd_register addSatOverflowMaxResult = nasimd::simd_add_sat<type, simd_arch_type>(constantMax, constantMax);
			testOpIfEqaul(ts, addSatOverflowMaxResult, constantMax);

			simd_register mmaskedAddSatOverflowMaxResult = nasimd::simd_mmasked_add_sat<type, simd_arch_type>(constantMax, constantMax, evenMask);
			testOpIfEqaulCount(ts, mmaskedAddSatOverflowMaxResult, constantMax, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedAddSatOverflowMaxResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcAddSatOverflowMaxResult = nasimd::simd_mmasked_src_add_sat<type, simd_arch_type>(constantMax, constantMax, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcAddSatOverflowMaxResult, constantMax, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcAddSatOverflowMaxResult, constant1, oddMask, halfCount);

			simd_register addSatOverflowMinResult = nasimd::simd_add_sat<type, simd_arch_type>(constantMin, constantMin);
			testOpIfEqaul(ts, addSatOverflowMinResult, constantMin);

			simd_register mmaskedAddSatOverflowMinResult = nasimd::simd_mmasked_add_sat<type, simd_arch_type>(constantMin, constantMin, evenMask);
			if constexpr (natl::IsBuiltInSignedIntegerC<type>) {
				testOpIfEqaulCount(ts, mmaskedAddSatOverflowMinResult, constantMin, halfCount);
				testOpIfEqaulMaskedCount(ts, mmaskedAddSatOverflowMinResult, constant0, oddMask, halfCount);
			} else {
				testOpIfEqaul(ts, mmaskedAddSatOverflowMinResult, constantMin);
			}

			simd_register mmaskedSrcAddSatOverflowMinResult = nasimd::simd_mmasked_src_add_sat<type, simd_arch_type>(constantMin, constantMin, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcAddSatOverflowMinResult, constantMin, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcAddSatOverflowMinResult, constant1, oddMask, halfCount);
		}

		//sub sat 
		if constexpr (natl::IsBuiltInIntegerC<type>) {
			simd_register subSatResult = nasimd::simd_sub_sat<type, simd_arch_type>(constant3, constant2);
			testOpIfEqaul(ts, subSatResult, constant1);

			simd_register mmaskedSubSatResult = nasimd::simd_mmasked_sub_sat<type, simd_arch_type>(constant3, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedSubSatResult, constant1, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSubSatResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcSubSatResult = nasimd::simd_mmasked_src_sub_sat<type, simd_arch_type>(constant3, constant2, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcSubSatResult, constant1, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcSubSatResult, constant2, oddMask, halfCount);

			simd_register subSatOverflowMaxResult = nasimd::simd_sub_sat<type, simd_arch_type>(constantMax, constantMin);
			testOpIfEqaul(ts, subSatOverflowMaxResult, constantMax);

			simd_register mmaskedSubSatOverflowMaxResult = nasimd::simd_mmasked_sub_sat<type, simd_arch_type>(constantMax, constantMin, evenMask);
			testOpIfEqaulCount(ts, mmaskedSubSatOverflowMaxResult, constantMax, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSubSatOverflowMaxResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcSubSatOverflowMaxResult = nasimd::simd_mmasked_src_sub_sat<type, simd_arch_type>(constantMax, constantMin, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcSubSatOverflowMaxResult, constantMax, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcSubSatOverflowMaxResult, constant1, oddMask, halfCount);

			simd_register subSatOverflowMinResult = nasimd::simd_sub_sat<type, simd_arch_type>(constantMin, constantMax);
			testOpIfEqaul(ts, subSatOverflowMinResult, constantMin);

			simd_register mmaskedSubSatOverflowMinResult = nasimd::simd_mmasked_sub_sat<type, simd_arch_type>(constantMin, constantMax, evenMask);
			if constexpr (natl::IsBuiltInSignedIntegerC<type>) {
				testOpIfEqaulCount(ts, mmaskedSubSatOverflowMinResult, constantMin, halfCount);
				testOpIfEqaulMaskedCount(ts, mmaskedSubSatOverflowMinResult, constant0, oddMask, halfCount);
			} else {
				testOpIfEqaul(ts, mmaskedSubSatOverflowMinResult, constantMin);
			}

			simd_register mmaskedSrcSubSatOverflowMinResult = nasimd::simd_mmasked_src_sub_sat<type, simd_arch_type>(constantMin, constantMax, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcSubSatOverflowMinResult, constantMin, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcSubSatOverflowMinResult, constant1, oddMask, halfCount);
		}

		//mul sat 
		if constexpr (natl::IsBuiltInIntegerC<type>) {
			simd_register mulSatResult = nasimd::simd_mul_sat<type, simd_arch_type>(constant2, constant2);
			testOpIfEqaul(ts, mulSatResult, constant4);

			simd_register mmaskedMulSatResult = nasimd::simd_mmasked_mul_sat<type, simd_arch_type>(constant2, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedMulSatResult, constant4, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedMulSatResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcMulSatResult = nasimd::simd_mmasked_src_mul_sat<type, simd_arch_type>(constant2, constant2, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcMulSatResult, constant4, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcMulSatResult, constant1, oddMask, halfCount);

			simd_register mulSatOverflowMaxResult = nasimd::simd_mul_sat<type, simd_arch_type>(constantMax, constantMax);
			testOpIfEqaul(ts, mulSatOverflowMaxResult, constantMax);

			simd_register mmaskedMulSatOverflowMaxResult = nasimd::simd_mmasked_mul_sat<type, simd_arch_type>(constantMax, constantMax, evenMask);
			testOpIfEqaulCount(ts, mmaskedMulSatOverflowMaxResult, constantMax, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedMulSatOverflowMaxResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcMulSatOverflowMaxResult = nasimd::simd_mmasked_src_mul_sat<type, simd_arch_type>(constantMax, constantMax, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcMulSatOverflowMaxResult, constantMax, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcMulSatOverflowMaxResult, constant1, oddMask, halfCount);

			simd_register mulSatOverflowMinResult = nasimd::simd_mul_sat<type, simd_arch_type>(constantMin, constantMax);
			testOpIfEqaul(ts, mulSatOverflowMinResult, constantMin);

			simd_register mmaskedMulSatOverflowMinResult = nasimd::simd_mmasked_mul_sat<type, simd_arch_type>(constantMin, constantMax, evenMask);
			if constexpr (natl::IsBuiltInSignedIntegerC<type>) {
				testOpIfEqaulCount(ts, mmaskedMulSatOverflowMinResult, constantMin, halfCount);
				testOpIfEqaulMaskedCount(ts, mmaskedMulSatOverflowMinResult, constant0, oddMask, halfCount);
			} else {
				testOpIfEqaul(ts, mmaskedMulSatOverflowMinResult, constantMin);
			}

			simd_register mmaskedSrcMulSatOverflowMinResult = nasimd::simd_mmasked_src_mul_sat<type, simd_arch_type>(constantMin, constantMax, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcMulSatOverflowMinResult, constantMin, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcMulSatOverflowMinResult, constant1, oddMask, halfCount);
		}

		//mul div 
		if constexpr (natl::IsBuiltInIntegerC<type>) {
			simd_register divSatResult = nasimd::simd_div_sat<type, simd_arch_type>(constant4, constant2);
			testOpIfEqaul(ts, divSatResult, constant2);

			simd_register mmaskedDivSatResult = nasimd::simd_mmasked_div_sat<type, simd_arch_type>(constant4, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedDivSatResult, constant2, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedDivSatResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcDivSatResult = nasimd::simd_mmasked_src_div_sat<type, simd_arch_type>(constant4, constant2, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcDivSatResult, constant2, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcDivSatResult, constant1, oddMask, halfCount);

			if constexpr (natl::IsBuiltInSignedIntegerC<type>) {
				simd_register constantNeg1 = nasimd::simd_set<type, simd_arch_type>(type(-1));

				simd_register divSatOverflowMaxResult = nasimd::simd_div_sat<type, simd_arch_type>(constantMin, constantNeg1);
				testOpIfEqaul(ts, divSatOverflowMaxResult, constantMax);

				simd_register mmaskedDivSatOverflowMaxResult = nasimd::simd_mmasked_div_sat<type, simd_arch_type>(constantMin, constantNeg1, evenMask);
				testOpIfEqaulCount(ts, mmaskedDivSatOverflowMaxResult, constantMax, halfCount);
				testOpIfEqaulMaskedCount(ts, mmaskedDivSatOverflowMaxResult, constant0, oddMask, halfCount);

				simd_register mmaskedSrcDivSatOverflowMaxResult = nasimd::simd_mmasked_src_div_sat<type, simd_arch_type>(constantMin, constantNeg1, constant1, evenMask);
				testOpIfEqaulCount(ts, mmaskedSrcDivSatOverflowMaxResult, constantMax, halfCount);
				testOpIfEqaulMaskedCount(ts, mmaskedSrcDivSatOverflowMaxResult, constant1, oddMask, halfCount);
			}
		}

		//remainder
		if constexpr (natl::IsBuiltInIntegerC<type>) {
			simd_register remainderResult = nasimd::simd_remainder<type, simd_arch_type>(constant3, constant2);
			testOpIfEqaul(ts, remainderResult, constant1);

			simd_register mmaskedRemainderResult = nasimd::simd_mmasked_remainder<type, simd_arch_type>(constant3, constant2, evenMask);
			testOpIfEqaulCount(ts, mmaskedRemainderResult, constant1, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedRemainderResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcRemainderResult = nasimd::simd_mmasked_src_remainder<type, simd_arch_type>(constant3, constant2, constant4, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcRemainderResult, constant1, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcRemainderResult, constant4, oddMask, halfCount);
		}

		//fused mul add
		if constexpr (natl::IsBuiltInFloatingPointC<type>) {
			simd_register fusedMulAddResult = nasimd::simd_fused_mul_add<type, simd_arch_type>(constant2, constant2, constant1);
			testOpIfEqaul(ts, fusedMulAddResult, constant5);

			simd_register mmaskedFusedMulAddResult = nasimd::simd_mmasked_fused_mul_add<type, simd_arch_type>(constant2, constant2, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedFusedMulAddResult, constant5, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedFusedMulAddResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcFusedMulAddResult = nasimd::simd_mmasked_src_fused_mul_add<type, simd_arch_type>(constant2, constant2, constant1, constant4, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcFusedMulAddResult, constant5, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcFusedMulAddResult, constant4, oddMask, halfCount);
		}

		//fused mul sub
		if constexpr (natl::IsBuiltInFloatingPointC<type>) {
			simd_register fusedMulSubResult = nasimd::simd_fused_mul_sub<type, simd_arch_type>(constant2, constant2, constant1);
			testOpIfEqaul(ts, fusedMulSubResult, constant3);

			simd_register mmaskedFusedMulSubResult = nasimd::simd_mmasked_fused_mul_sub<type, simd_arch_type>(constant2, constant2, constant1, evenMask);
			testOpIfEqaulCount(ts, mmaskedFusedMulSubResult, constant3, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedFusedMulSubResult, constant0, oddMask, halfCount);

			simd_register mmaskedSrcFusedMulSubResult = nasimd::simd_mmasked_src_fused_mul_sub<type, simd_arch_type>(constant2, constant2, constant1, constant4, evenMask);
			testOpIfEqaulCount(ts, mmaskedSrcFusedMulSubResult, constant3, halfCount);
			testOpIfEqaulMaskedCount(ts, mmaskedSrcFusedMulSubResult, constant4, oddMask, halfCount);
		}
	}
	NATL_REGISTER_TEST_CONSTEXPR(arithmeticTest);
};

struct StandardI8SimdTestSuite : public SimdTestSuite<natl::i8, standard_arch> {};
NATL_REGISTER_TESTS(StandardI8SimdTestSuite, SimdTestSuiteTests);

struct StandardI16SimdTestSuite : public SimdTestSuite<natl::i16, standard_arch> {};
NATL_REGISTER_TESTS(StandardI16SimdTestSuite, SimdTestSuiteTests);

struct StandardI32SimdTestSuite : public SimdTestSuite<natl::i32, standard_arch> {};
NATL_REGISTER_TESTS(StandardI32SimdTestSuite, SimdTestSuiteTests);

struct StandardI64SimdTestSuite : public SimdTestSuite<natl::i64, standard_arch> {};
NATL_REGISTER_TESTS(StandardI64SimdTestSuite, SimdTestSuiteTests);

struct StandardUI8SimdTestSuite : public SimdTestSuite<natl::ui8, standard_arch> {};
NATL_REGISTER_TESTS(StandardUI8SimdTestSuite, SimdTestSuiteTests);

struct StandardUI16SimdTestSuite : public SimdTestSuite<natl::ui16, standard_arch> {};
NATL_REGISTER_TESTS(StandardUI16SimdTestSuite, SimdTestSuiteTests);

struct StandardUI32SimdTestSuite : public SimdTestSuite<natl::ui32, standard_arch> {};
NATL_REGISTER_TESTS(StandardUI32SimdTestSuite, SimdTestSuiteTests);

struct StandardUI64SimdTestSuite : public SimdTestSuite<natl::ui64, standard_arch> {};
NATL_REGISTER_TESTS(StandardUI64SimdTestSuite, SimdTestSuiteTests);

struct StandardF32SimdTestSuite : public SimdTestSuite<natl::f32, standard_arch> {};
NATL_REGISTER_TESTS(StandardF32SimdTestSuite, SimdTestSuiteTests);

struct StandardF64SimdTestSuite : public SimdTestSuite<natl::f64, standard_arch> {};
NATL_REGISTER_TESTS(StandardF64SimdTestSuite, SimdTestSuiteTests);

NATL_TESTS_MAIN();