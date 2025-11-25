//@file_not_module

//@begin_non_modules
//std
#include <cmath>

//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/math/all.h>
#include <natl/system/timer.h>
#include <natl/util/test.h>

template<typename Type>
struct MathIntegerTestSuite : public natl::TestSuite<MathIntegerTestSuite<Type>> {
	using type = Type;
};

template<typename test_suite_type = MathIntegerTestSuite<natl::i8>>
struct MathIntegerTestSuiteTests {
	using type = test_suite_type::type;

	constexpr static void addsatTest(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Limits<type>::max(), natlm::addsat<type>(natl::Limits<type>::max(), natl::Limits<type>::max()));
		ts.assertEqual(natl::Limits<type>::min(), natlm::addsat<type>(natl::Limits<type>::min(), natl::Limits<type>::min()));
		ts.assertEqual(type(5), natlm::addsat<type>(2, 3));
	}
	NATL_REGISTER_TEST_CONSTEXPR(addsatTest);

	constexpr static void subsatTest(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Limits<type>::max(), natlm::subsat<type>(natl::Limits<type>::max(), natl::Limits<type>::min()));
		ts.assertEqual(natl::Limits<type>::min(), natlm::subsat<type>(natl::Limits<type>::min(), natl::Limits<type>::max()));
		ts.assertEqual(type(5), natlm::subsat<type>(8, 3));
	}
	NATL_REGISTER_TEST_CONSTEXPR(subsatTest);

	constexpr static void mulsatTest(test_suite_type& ts) noexcept {
		ts.assertEqual(natl::Limits<type>::max(), natlm::mulsat<type>(natl::Limits<type>::max(), natl::Limits<type>::max()));
		ts.assertEqual(natl::Limits<type>::min(), natlm::mulsat<type>(natl::Limits<type>::min(), natl::Limits<type>::max()));
		ts.assertEqual(type(20), natlm::mulsat<type>(5, 4));
	}
	NATL_REGISTER_TEST_CONSTEXPR(mulsatTest);

	constexpr static void divsatTest(test_suite_type& ts) noexcept {
		if constexpr (natl::IsBuiltInSignedIntegerC<type>) {
			ts.assertEqual(natl::Limits<type>::max(), natlm::divsat<type>(natl::Limits<type>::min(), type(-1)));
		}
		ts.assertEqual(type(5), natlm::divsat<type>(20, 4));
	}
	NATL_REGISTER_TEST_CONSTEXPR(divsatTest);
};

using namespace natl::literals;

struct MathFloatTestSuite : public natl::TestSuite<MathFloatTestSuite> {};

template<typename test_suite_type = MathFloatTestSuite>
struct MathFloatTestSuiteTests {
	constexpr static void isnanTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isnanF32(natl::Limits<natl::f32>::quietNaN()));
		ts.assertFalse(natlm::isnanF32(1.0_f32));
		ts.assertTrue(natlm::isnanF64(natl::Limits<natl::f64>::quietNaN()));
		ts.assertFalse(natlm::isnanF64(1.0_f64));
	}
	NATL_REGISTER_TEST_CONSTEXPR(isnanTest);

	constexpr static void isInfinityTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isInfinityF32(natl::Limits<natl::f32>::infinity()));
		ts.assertFalse(natlm::isInfinityF32(1.0_f32));
		ts.assertTrue(natlm::isInfinityF64(natl::Limits<natl::f64>::infinity()));
		ts.assertFalse(natlm::isInfinityF64(1.0_f64));
	}
	NATL_REGISTER_TEST_CONSTEXPR(isInfinityTest);

	constexpr static void fabsTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::fabsF32(3.0_f32), 3.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::fabsF32(-3.0_f32), 3.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::fabsF64(3.0_f64), 3.0_f64, 0.01_f64));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::fabsF64(-3.0_f64), 3.0_f64, 0.01_f64));
	}
	NATL_REGISTER_TEST_CONSTEXPR(fabsTest);

	constexpr static void isRoughtlyEqualTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(1.0_f32, 3.0_f32, 3.0_f32));
		ts.assertFalse(natlm::isRoughtlyEqualF32(1.0_f32, 3.0_f32, 1.0_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF64(1.0_f64, 3.0_f64, 3.0_f64));
		ts.assertFalse(natlm::isRoughtlyEqualF64(1.0_f64, 3.0_f64, 1.0_f64));
	}
	NATL_REGISTER_TEST_CONSTEXPR(isRoughtlyEqualTest);

	constexpr static void basicCeilTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicCeilF32(5.5_f32), 6.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicCeilF32(-5.5_f32), -5.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicCeilF64(5.5_f64), 6.0_f64, 0.01_f64));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicCeilF64(-5.5_f64), -5.0_f64, 0.01_f64));
	}
	NATL_REGISTER_TEST_CONSTEXPR(basicCeilTest);

	constexpr static void ceilTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::ceilF32(5.5), 6.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::ceilF32(-5.5), -5.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::ceilF64(5.5), 6.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::ceilF64(-5.5), -5.0, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(ceilTest);

	constexpr static void basicFloorTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicFloorF32(5.5), 5.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicFloorF32(-5.5), -6.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicFloorF64(5.5), 5.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicFloorF64(-5.5), -6.0, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(basicFloorTest);

	constexpr static void floorTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::floorF32(5.5), 5.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::floorF32(-5.5), -6.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::floorF64(5.5), 5.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::floorF64(-5.5), -6.0, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(floorTest);

	constexpr static void basicTruncTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicTruncF32(5.5), 5.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicTruncF32(-5.5), -5.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicTruncF32(0.5), 0.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicTruncF32(-0.5), 0.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicTruncF64(5.5), 5.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicTruncF64(-5.5), -5.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicTruncF64(0.5), 0.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicTruncF64(-0.5), 0.0, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(basicTruncTest);

	constexpr static void truncTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::truncF32(5.5), 5.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::truncF32(-5.5), -5.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::truncF32(0.5), 0.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::truncF32(-0.5), 0.0, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::truncF64(5.5), 5.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::truncF64(-5.5), -5.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::truncF64(0.5), 0.0, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::truncF64(-0.5), 0.0, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(truncTest);

	constexpr static void basicRoundTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicRoundF32(1.4_f32), 1.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicRoundF32(-1.4_f32), -1.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicRoundF32(0.4_f32), 0.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicRoundF32(-0.4_f32), 0.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicRoundF64(1.4_f64), 1.0_f64, 0.01_f64));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicRoundF64(-1.4_f64), -1.0_f64, 0.01_f64));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicRoundF64(0.4_f64), 0.0_f64, 0.01_f64));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicRoundF64(-0.4_f64), 0.0_f64, 0.01_f64));
	}
	NATL_REGISTER_TEST_CONSTEXPR(basicRoundTest);

	constexpr static void roundTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::roundF32(1.4_f32), 1.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::roundF32(-1.4_f32), -1.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::roundF32(0.4_f32), 0.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::roundF32(-0.4_f32), 0.0_f32, 0.01_f32));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::roundF64(1.4_f64), 1.0_f64, 0.01_f64));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::roundF64(-1.4_f64), -1.0_f64, 0.01_f64));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::roundF64(0.4_f64), 0.0_f64, 0.01_f64));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::roundF64(-0.4_f64), 0.0_f64, 0.01_f64));
	}
	NATL_REGISTER_TEST_CONSTEXPR(roundTest);

	constexpr static void reciprocalTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::reciprocalF32(2.0), 0.5, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::reciprocalF32(-2.0), -0.5, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::reciprocalF64(2.0), 0.5, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::reciprocalF64(-2.0), -0.5, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(reciprocalTest);

	constexpr static void basicFmodTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicFmodF32(5.5, 2.0), 1.5, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::basicFmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicFmodF64(5.5, 2.0), 1.5, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::basicFmodF64(-5.5, 2.0), -1.5, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(basicFmodTest);

	constexpr static void fmodTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::fmodF32(5.5, 2.0), 1.5, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::fmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::fmodF64(5.5, 2.0), 1.5, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::fmodF64(-5.5, 2.0), -1.5, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(fmodTest);

	constexpr static void mulFsmodTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::mulFmodF32(5.5, 2.0), 1.5, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::mulFmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::mulFmodF64(5.5, 2.0), 1.5, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::mulFmodF64(-5.5, 2.0), -1.5, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(mulFsmodTest);

	constexpr static void mulFsmodNoscaleTest(test_suite_type& ts) noexcept {
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::mulFmodNoscaleF32(5.5, 2.0), 1.5 / 2, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF32(natlm::mulFmodNoscaleF32(-5.5, 2.0), -1.5 / 2, natl::f32(0.01)));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::mulFmodNoscaleF64(5.5, 2.0), 1.5 / 2, 0.01));
		ts.assertTrue(natlm::isRoughtlyEqualF64(natlm::mulFmodNoscaleF64(-5.5, 2.0), -1.5 / 2, 0.01));
	}
	NATL_REGISTER_TEST_CONSTEXPR(mulFsmodNoscaleTest);


	constexpr static void testMathFunctionF32(test_suite_type& ts, natl::f32(*customFunc)(natl::f32), natl::f32(*testFunc)(natl::f32),
		natl::f32 testStart, natl::f32 testEnd, natl::f32 incrementNum, natl::f32 relativeTolorance, natl::f32 discardSmallDif) noexcept {

		for (natl::f32 testValue = testStart; testValue < testEnd; testValue += incrementNum) {
			const natl::f32 customValue = customFunc(testValue);
			const natl::f32 accurateValue = testFunc(testValue);
			const natl::Bool relativelyClose = natlm::isRelativelyCloseF32(customValue, accurateValue, relativeTolorance);
			const natl::Bool roughtlyEqual = natlm::isRoughtlyEqualF32(customValue, accurateValue, discardSmallDif);
			const natl::Bool closeEnough = relativelyClose || roughtlyEqual;
			ts.tassert(closeEnough, "when given value ", testValue, " did not produce value without the relative tolorance of ", relativeTolorance, " values were ", customValue, " and ", accurateValue);
		}
	}

	constexpr static void testMathFunctionF64(test_suite_type& ts, natl::f64(*customFunc)(natl::f64), natl::f64(*testFunc)(natl::f64),
		natl::f64 testStart, natl::f64 testEnd, natl::f64 incrementNum, natl::f64 relativeTolorance, natl::f64 discardSmallDif) noexcept {

		for (natl::f64 testValue = testStart; testValue < testEnd; testValue += incrementNum) {
			const natl::f64 customValue = customFunc(testValue);
			const natl::f64 accurateValue = testFunc(testValue);
			const natl::Bool relativelyClose = natlm::isRelativelyCloseF64(customValue, accurateValue, relativeTolorance);
			const natl::Bool roughtlyEqual = natlm::isRoughtlyEqualF64(customValue, accurateValue, discardSmallDif);
			const natl::Bool closeEnough = relativelyClose || roughtlyEqual;
			ts.tassert(closeEnough, "when given value ", testValue, " did not produce value without the relative tolorance of ", relativeTolorance, " values were ", customValue, " and ", accurateValue);
		}
	}

	constexpr static void basicSqrtImplSTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::impl::basicSqrtF32ImplS, sqrtf,
			0.0_f32, natl::f32(10000), 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::impl::basicSqrtF64ImplS, sqrt,
			0.0_f64, natl::f64(10000), 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicSqrtImplSTest);

	constexpr static void sqrtImplSTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::impl::sqrtF32ImplS, sqrtf,
			0.0_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::impl::sqrtF64ImplS, sqrt,
			0.0_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(sqrtImplSTest);

	constexpr static void basicSqrtTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicSqrtF32, sqrtf,
			0.0_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicSqrtF64, sqrt,
			0.0_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicSqrtTest);

	constexpr static void sqrtTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::sqrtF32, sqrtf,
			0.0_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::sqrtF64, sqrt,
			0.0_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(sqrtTest);

	constexpr static void basicRsqrtImplSTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::impl::basicRsqrtF32ImplS,
			[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
			0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::impl::basicRsqrtF64ImplS,
			[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
			0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicRsqrtImplSTest);

	constexpr static void rsqrtImplSTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::impl::rsqrtF32ImplS,
			[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
			0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::impl::rsqrtF64ImplS,
			[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
			0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(rsqrtImplSTest);

	constexpr static void basicRsqrtTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicRsqrtF32,
			[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
			0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicRsqrtF64,
			[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
			0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicRsqrtTest);

	constexpr static void rsqrtTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::rsqrtF32,
			[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
			0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::rsqrtF64,
			[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
			0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(rsqrtTest);

	constexpr static void basicExpDivTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicExpDivF32, expf,
			-80.0_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicExpDivF64, exp,
			-80.0_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicExpDivTest);

	constexpr static void basicExpTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicExpF32, expf,
			-80.0_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicExpF64, exp,
			-80.0_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicExpTest);

	constexpr static void basicExp2Test(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicExp2F32, exp2f,
			-80.0_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicExp2F64, exp2,
			-80.0_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicExp2Test);

	constexpr static void basicLog2Test(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicLog2F32, log2f,
			0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicLog2F64, log2,
			0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicLog2Test);

	constexpr static void log2Test(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::log2F32, log2f,
			0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::log2F64, log2,
			0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(log2Test);

	constexpr static void basicLnTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicLnF32, logf,
			0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicLnF64, log,
			0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicLnTest);

	constexpr static void lnTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::lnF32, logf,
			0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::lnF64, log,
			0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(lnTest);

	constexpr static void basicLogTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicLogF32, log10f,
			0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicLogF64, log10,
			0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicLogTest);

	constexpr static void logTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::logF32, log10f,
			0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::logF64, log10,
			0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(logTest);

	constexpr static void sinQuadTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::sinQuadF32, sinf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::sinQuadF64, sin,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(sinQuadTest);

	constexpr static void cosQuadTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::cosQuadF32, cosf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::cosQuadF64, cos,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(cosQuadTest);

	constexpr static void sinQuadMulFmodTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::sinQuadMulFmodF32, sinf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::sinQuadMulFmodF64,
			sin, natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(sinQuadMulFmodTest);

	constexpr static void cosQuadMulFmodTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::cosQuadMulFmodF32, cosf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::cosQuadMulFmodF64, cos,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(cosQuadMulFmodTest);

	constexpr static void cosTaySerd4pTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::cosTaySerd4pF32, cosf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.3_f32, 0.3_f32);
		testMathFunctionF64(ts, natlm::cosTaySerd4pF64, cos,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.3_f64, 0.3_f64);
	}
	NATL_REGISTER_TEST(cosTaySerd4pTest);

	constexpr static void sinTaySerd4pTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::sinTaySerd4pF32, sinf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.3_f32, 0.3_f32);
		testMathFunctionF64(ts, natlm::sinTaySerd4pF64,
			sin, natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.3_f64, 0.3_f64);
	}
	NATL_REGISTER_TEST(sinTaySerd4pTest);

	constexpr static void tan4dpTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::tan4dpF32, tanf,
			natl::f32(-natlm::piOver2 + 0.1), natl::f32(natlm::piOver2 - 0.1), 0.001_f32, 0.02_f32, 0.02_f32);
		testMathFunctionF64(ts, natlm::tan4dpF64, tan,
			natl::f64(-natlm::piOver2 + 0.1), natl::f64(natlm::piOver2 - 0.1), 0.001_f64, 0.02_f64, 0.02_f64);
	}
	NATL_REGISTER_TEST(tan4dpTest);

	constexpr static void asinQuadTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::asinQuadF32, asinf,
			-1.0_f32, 1.0_f32, 0.001_f32, 0.05_f32, 0.05_f32);
		testMathFunctionF64(ts, natlm::asinQuadF64, asin,
			-1.0_f64, 1.0_f64, 0.001_f64, 0.05_f64, 0.05_f64);
	}
	NATL_REGISTER_TEST(asinQuadTest);

	constexpr static void acosQuadTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::acosQuadF32, acosf,
			-1.0_f32, 1.0_f32, 0.001_f32, 0.08_f32, 0.08_f32);
		testMathFunctionF64(ts, natlm::acosQuadF64, acos,
			-1.0_f64, 1.0_f64, 0.001_f64, 0.08_f64, 0.08_f64);
	}
	NATL_REGISTER_TEST(acosQuadTest);

	constexpr static void atan4dpTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::atan4dpF32, atanf,
			-1.0_f32, 1.0_f32, 0.001_f32, 0.008_f32, 0.001_f32);
		testMathFunctionF64(ts, natlm::atan4dpF64, atan,
			-1.0_f64, 1.0_f64, 0.001_f64, 0.008_f64, 0.001_f64);
	}
	NATL_REGISTER_TEST(atan4dpTest);

	constexpr static void basicSinhTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicSinhF32, sinhf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicSinhF64, sinh,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicSinhTest);

	constexpr static void sinhTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::sinhF32, sinhf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::sinhF64, sinh,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(sinhTest);

	constexpr static void basicCoshTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicCoshF32, coshf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicCoshF64, cosh,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicCoshTest);

	constexpr static void coshTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::coshF32, coshf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::coshF64, cosh,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(coshTest);

	constexpr static void basicTanhTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicTanhF32, tanhf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicTanhF64, tanh,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicTanhTest);

	constexpr static void tanhTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::tanhF32, tanhf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::tanhF64, tanh,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(tanhTest);

	constexpr static void basicAsinhTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicAsinhF32, asinhf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicAsinhF64, asinh,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicAsinhTest);

	constexpr static void asinhTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::asinhF32, asinhf,
			natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::asinhF64, asinh,
			natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(asinhTest);

	constexpr static void basicAcoshTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicAcoshF32, acoshf,
			1.0_f32, natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicAcoshF64, acosh,
			1.0_f64, natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicAcoshTest);

	constexpr static void acoshTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::acoshF32, acoshf,
			1.0_f32, natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::acoshF64, acosh,
			1.0_f64, natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(acoshTest);

	constexpr static void basicAtanhTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::basicAtanhF32, atanhf,
			natl::f32(-0.99999), 1.0_f32, 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::basicAtanhF64, atanh,
			natl::f64(-0.99999), 1.0_f64, 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(basicAtanhTest);

	constexpr static void atanhTest(test_suite_type& ts) noexcept {
		testMathFunctionF32(ts, natlm::atanhF32, atanhf,
			-1.0_f32, 1.0_f32, 0.001_f32, 0.1_f32, 0.1_f32);
		testMathFunctionF64(ts, natlm::atanhF64, atanh,
			-1.0_f64, 1.0_f64, 0.001_f64, 0.1_f64, 0.1_f64);
	}
	NATL_REGISTER_TEST(atanhTest);
};

struct MathI8TestSuite : public MathIntegerTestSuite<natl::i8> {};
NATL_REGISTER_TESTS(MathI8TestSuite, MathIntegerTestSuiteTests);

struct MathI16TestSuite : public MathIntegerTestSuite<natl::i16> {};
NATL_REGISTER_TESTS(MathI16TestSuite, MathIntegerTestSuiteTests);

struct MathI32TestSuite : public MathIntegerTestSuite<natl::i32> {};
NATL_REGISTER_TESTS(MathI32TestSuite, MathIntegerTestSuiteTests);

struct MathI64TestSuite : public MathIntegerTestSuite<natl::i64> {};
NATL_REGISTER_TESTS(MathI64TestSuite, MathIntegerTestSuiteTests);

struct MathUI8TestSuite : public MathIntegerTestSuite<natl::ui8> {};
NATL_REGISTER_TESTS(MathUI8TestSuite, MathIntegerTestSuiteTests);

struct MathUI16TestSuite : public MathIntegerTestSuite<natl::ui16> {};
NATL_REGISTER_TESTS(MathUI16TestSuite, MathIntegerTestSuiteTests);

struct MathUI32TestSuite : public MathIntegerTestSuite<natl::ui32> {};
NATL_REGISTER_TESTS(MathUI32TestSuite, MathIntegerTestSuiteTests);

struct MathUI64TestSuite : public MathIntegerTestSuite<natl::ui64> {};
NATL_REGISTER_TESTS(MathUI64TestSuite, MathIntegerTestSuiteTests);

NATL_REGISTER_STD_TESTS(MathFloatTestSuite);

NATL_TESTS_MAIN();
