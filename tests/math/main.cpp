
//natl
#include <natl/math.h>
#include <natl/system/timer.h>
#include <natl/util/test.h>

#define NATL_TEST_FROM "NatlMathTest"

//saturated arithmetic test
template<typename IntegerType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool integerAddsatTest() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(NATL_TEST_FROM, natl::String(IntegerName) + " addsat", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::addsat<IntegerType>(natl::Limits<IntegerType>::max(), natl::Limits<IntegerType>::max()) == natl::Limits<IntegerType>::max(), IntegerName, " addsat overflow max");
	NATL_TEST_ASSERT(natlm::addsat<IntegerType>(natl::Limits<IntegerType>::min(), natl::Limits<IntegerType>::min()) == natl::Limits<IntegerType>::min(), IntegerName, " addsat overflow min");
	NATL_TEST_ASSERT(natlm::addsat<IntegerType>(2, 3) == 5, IntegerName, " addsat standard add");
	return test;
}

constexpr natl::Bool addsatTest() noexcept {
	natl::Bool result = true;
	result &= integerAddsatTest<natl::i8, "natl::i8">();
	result &= integerAddsatTest<natl::i16, "natl::i16">();
	result &= integerAddsatTest<natl::i32, "natl::i32">();
	result &= integerAddsatTest<natl::i64, "natl::i64">();
	result &= integerAddsatTest<natl::ui8, "natl::ui8">();
	result &= integerAddsatTest<natl::ui16, "natl::ui16">();
	result &= integerAddsatTest<natl::ui32, "natl::ui32">();
	result &= integerAddsatTest<natl::ui64, "natl::ui64">();
	return result;
}

template<typename IntegerType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool integerSubsatTest() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(NATL_TEST_FROM, natl::String(IntegerName) + " subsat", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::subsat<IntegerType>(natl::Limits<IntegerType>::max(), natl::Limits<IntegerType>::min()) == natl::Limits<IntegerType>::max(), IntegerName, " subsat overflow max test");
	NATL_TEST_ASSERT(natlm::subsat<IntegerType>(natl::Limits<IntegerType>::min(), natl::Limits<IntegerType>::max()) == natl::Limits<IntegerType>::min(), IntegerName, " subsat overflow min test");
	NATL_TEST_ASSERT(natlm::subsat<IntegerType>(8, 3) == 5, IntegerName, " subsat standard sub test");
	return test;
}

constexpr natl::Bool subsatTest() noexcept {
	natl::Bool result = true;
	result &= integerSubsatTest<natl::i8, "natl::i8">();
	result &= integerSubsatTest<natl::i16, "natl::i16">();
	result &= integerSubsatTest<natl::i32, "natl::i32">();
	result &= integerSubsatTest<natl::i64, "natl::i64">();
	result &= integerSubsatTest<natl::ui8, "natl::ui8">();
	result &= integerSubsatTest<natl::ui16, "natl::ui16">();
	result &= integerSubsatTest<natl::ui32, "natl::ui32">();
	result &= integerSubsatTest<natl::ui64, "natl::ui64">();
	return result;
}

template<typename IntegerType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool integerMulsatTest() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(NATL_TEST_FROM, natl::String(IntegerName) + " mulsat", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::mulsat<IntegerType>(natl::Limits<IntegerType>::max(), natl::Limits<IntegerType>::max()) == natl::Limits<IntegerType>::max(), IntegerName, " mulsat overflow max test");
	NATL_TEST_ASSERT(natlm::mulsat<IntegerType>(natl::Limits<IntegerType>::min(), natl::Limits<IntegerType>::max()) == natl::Limits<IntegerType>::min(), IntegerName, " mulsat overflow min test");
	NATL_TEST_ASSERT(natlm::mulsat<IntegerType>(5, 4) == 20, IntegerName, " mulsat standard mul test");
	return true;
}

constexpr natl::Bool mulsatTest() noexcept {
	natl::Bool result = true;
	result &= integerMulsatTest<natl::i8, "natl::i8">();
	result &= integerMulsatTest<natl::i16, "natl::i16">();
	result &= integerMulsatTest<natl::i32, "natl::i32">();
	result &= integerMulsatTest<natl::i64, "natl::i64">();
	result &= integerMulsatTest<natl::ui8, "natl::ui8">();
	result &= integerMulsatTest<natl::ui16, "natl::ui16">();
	result &= integerMulsatTest<natl::ui32, "natl::ui32">();
	result &= integerMulsatTest<natl::ui64, "natl::ui64">();
	return result;
}

template<typename IntegerType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool integerDivsatTest() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(NATL_TEST_FROM, natl::String(IntegerName) + " integer divsat", natl::TestType::leaf);
	if constexpr (natl::IsBuiltInSignedIntegerC<IntegerType>) {
		NATL_TEST_ASSERT(natlm::divsat<IntegerType>(natl::Limits<IntegerType>::min(), IntegerType(-1)) == natl::Limits<IntegerType>::max(), IntegerName, " divsat overflow max test");
	}
	NATL_TEST_ASSERT(natlm::divsat<IntegerType>(20, 4) == 5, IntegerName, " divsat standard mul test");
	return true;
}

constexpr natl::Bool divsatTest() noexcept {
	natl::Bool result = true;
	result &= integerDivsatTest<natl::i8, "natl::i8">();
	result &= integerDivsatTest<natl::i16, "natl::i16">();
	result &= integerDivsatTest<natl::i32, "natl::i32">();
	result &= integerDivsatTest<natl::i64, "natl::i64">();
	result &= integerDivsatTest<natl::ui8, "natl::ui8">();
	result &= integerDivsatTest<natl::ui16, "natl::ui16">();
	result &= integerDivsatTest<natl::ui32, "natl::ui32">();
	result &= integerDivsatTest<natl::ui64, "natl::ui64">();
	return result;
}

static_assert(addsatTest());
static_assert(subsatTest());
static_assert(mulsatTest());
static_assert(divsatTest());

natl::Bool saturatedArithmeticTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "saturated arithmetic", natl::TestType::node);
	NATL_SUB_TEST_ASSERT(addsatTest());
	NATL_SUB_TEST_ASSERT(subsatTest());
	NATL_SUB_TEST_ASSERT(mulsatTest());
	NATL_SUB_TEST_ASSERT(divsatTest());
	return test;
}

//float operations 
constexpr natl::Bool isnanTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "isnan", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isnanF32(natl::Limits<natl::f32>::quietNaN()), "isnan f32 test");
	NATL_TEST_ASSERT(!natlm::isnanF32(1.0), "not isnan f32 test");
	NATL_TEST_ASSERT(natlm::isnanF64(natl::Limits<natl::f64>::quietNaN()), "isnan f64 test");
	NATL_TEST_ASSERT(!natlm::isnanF64(1.0), "not isnan f64 test");
	return test;
}

constexpr natl::Bool isInfinityTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "is infinity", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isInfinityF32(natl::Limits<natl::f32>::infinity()), "isInfinity f32 test");
	NATL_TEST_ASSERT(!natlm::isInfinityF32(1.0), "not isInfinity f32 test");
	NATL_TEST_ASSERT(natlm::isInfinityF64(natl::Limits<natl::f64>::infinity()), "isInfinity f64 test");
	NATL_TEST_ASSERT(!natlm::isInfinityF64(1.0), "not isInfinity f64 test");
	return test;
}

constexpr natl::Bool fabsTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "fabs", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::fabsF32(3.0), 3.0, natl::f32(0.01)), "fabs f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::fabsF32(-3.0), 3.0, natl::f32(0.01)), "fabs f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::fabsF64(3.0), 3.0, 0.01), "fabs f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::fabsF64(-3.0), 3.0, 0.01), "fabs f64 negative test");
	return test;
}

constexpr natl::Bool isRoughtlyEqualTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "isRoughtlyEqual", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(1.0, 3.0, 3.0), "isRoughtlyEqual f32 test");
	NATL_TEST_ASSERT(!natlm::isRoughtlyEqualF32(1.0, 3.0, 1.0), "not isRoughtlyEqual f32 test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(1.0, 3.0, 3.0), "isRoughtlyEqual f64 test");
	NATL_TEST_ASSERT(!natlm::isRoughtlyEqualF64(1.0, 3.0, 1.0), "not isRoughtlyEqual f64 test");
	return test;
}

constexpr natl::Bool basicCeilTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "basicCeil", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicCeilF32(5.5), 6.0, natl::f32(0.01)), "basicCeil f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicCeilF32(-5.5), -5.0, natl::f32(0.01)), "basicCeil f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicCeilF64(5.5), 6.0, 0.01), "basicCeil f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicCeilF64(-5.5), -5.0, 0.01), "basicCeil f64 negative test");
	return test;
}

constexpr natl::Bool ceilTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "ceil", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::ceilF32(5.5), 6.0, natl::f32(0.01)), "ceil f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::ceilF32(-5.5), -5.0, natl::f32(0.01)), "ceil f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::ceilF64(5.5), 6.0, 0.01), "ceil f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::ceilF64(-5.5), -5.0, 0.01), "ceil f64 negative test");
	return test;
}

constexpr natl::Bool basicFloorTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "basicFloor", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicFloorF32(5.5), 5.0, natl::f32(0.01)), "basicFloor f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicFloorF32(-5.5), -6.0, natl::f32(0.01)), "basicFloor f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicFloorF64(5.5), 5.0, 0.01), "basicFloor f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicFloorF64(-5.5), -6.0, 0.01), "basicFloor f64 negative test");
	return test;
}

constexpr natl::Bool floorTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "floor", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::floorF32(5.5), 5.0, natl::f32(0.01)), "floor f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::floorF32(-5.5), -6.0, natl::f32(0.01)), "floor f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::floorF64(5.5), 5.0, 0.01), "floor f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::floorF64(-5.5), -6.0, 0.01), "floor f64 negative test");
	return test;
}

constexpr natl::Bool basicTruncTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "basicTrunc", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicTruncF32(5.5), 5.0, natl::f32(0.01)), "basicTrunc f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicTruncF32(-5.5), -5.0, natl::f32(0.01)), "basicTrunc f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicTruncF32(0.5), 0.0, natl::f32(0.01)), "basicTrunc f32 positive to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicTruncF32(-0.5), 0.0, natl::f32(0.01)), "basicTrunc f32 negative to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicTruncF64(5.5), 5.0, 0.01), "basicTrunc f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicTruncF64(-5.5), -5.0, 0.01), "basicTrunc f64 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicTruncF64(0.5), 0.0, 0.01), "basicTrunc f64 positive to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicTruncF64(-0.5), 0.0, 0.01), "basicTrunc f64 negative to zero test");
	return test;
}

constexpr natl::Bool truncTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "trunc", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::truncF32(5.5), 5.0, natl::f32(0.01)), "trunc f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::truncF32(-5.5), -5.0, natl::f32(0.01)), "trunc f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::truncF32(0.5), 0.0, natl::f32(0.01)), "trunc f32 positive to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::truncF32(-0.5), 0.0, natl::f32(0.01)), "trunc f32 negative to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::truncF64(5.5), 5.0, 0.01), "trunc f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::truncF64(-5.5), -5.0, 0.01), "trunc f64 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::truncF64(0.5), 0.0, 0.01), "trunc f64 positive to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::truncF64(-0.5), 0.0, 0.01), "trunc f64 negative to zero test");
	return test;
}

constexpr natl::Bool basicRoundTests() noexcept {
	using namespace natl::literals;
	natl::Test test(NATL_TEST_FROM, "basicRound", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicRoundF32(1.4_f32), 1.0_f32, 0.01_f32), "basicRound f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicRoundF32(-1.4_f32), -1.0_f32, 0.01_f32), "basicRound f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicRoundF32(0.4_f32), 0.0_f32, 0.01_f32), "basicRound f32 positive to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicRoundF32(-0.4_f32), 0.0_f32, 0.01_f32), "basicRound f32 negative to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicRoundF64(1.4_f64), 1.0_f64, 0.01_f64), "basicRound f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicRoundF64(-1.4_f64), -1.0_f64, 0.01_f64), "basicRound f64 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicRoundF64(0.4_f64), 0.0_f64, 0.01_f64), "basicRound f64 positive to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicRoundF64(-0.4_f64), 0.0_f64, 0.01_f64), "basicRound f64 negative to zero test");
	return test;
}

constexpr natl::Bool roundTests() noexcept {
	using namespace natl::literals;
	natl::Test test(NATL_TEST_FROM, "round", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::roundF32(1.4_f32), 1.0_f32, 0.01_f32), "round f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::roundF32(-1.4_f32), -1.0_f32, 0.01_f32), "round f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::roundF32(0.4_f32), 0.0_f32, 0.01_f32), "round f32 positive to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::roundF32(-0.4_f32), 0.0_f32, 0.01_f32), "round f32 negative to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::roundF64(1.4_f64), 1.0_f64, 0.01_f64), "round f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::roundF64(-1.4_f64), -1.0_f64, 0.01_f64), "round f64 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::roundF64(0.4_f64), 0.0_f64, 0.01_f64), "round f64 positive to zero test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::roundF64(-0.4_f64), 0.0_f64, 0.01_f64), "round f64 negative to zero test");
	return test;
}

constexpr natl::Bool reciprocalTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "reciprocal", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::reciprocalF32(2.0), 0.5, natl::f32(0.01)), "reciprocal f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::reciprocalF32(-2.0), -0.5, natl::f32(0.01)), "reciprocal f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::reciprocalF64(2.0), 0.5, 0.01), "reciprocal f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::reciprocalF64(-2.0), -0.5, 0.01), "reciprocal f64 negative test");
	return test;
}

constexpr natl::Bool basicFmodTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "basicFmod", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicFmodF32(5.5, 2.0), 1.5, natl::f32(0.01)), "basicFmod f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::basicFmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)), "basicFmod f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicFmodF64(5.5, 2.0), 1.5, 0.01), "basicFmod f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::basicFmodF64(-5.5, 2.0), -1.5, 0.01), "basicFmod f64 negative test");
	return test;
}

constexpr natl::Bool fmodTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "fmod", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::fmodF32(5.5, 2.0), 1.5, natl::f32(0.01)), "fmod f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::fmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)), "fmod f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::fmodF64(5.5, 2.0), 1.5, 0.01), "fmod f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::fmodF64(-5.5, 2.0), -1.5, 0.01), "fmod f64 negative test");
	return test;
}


constexpr natl::Bool mulFsmodTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "mulFmod", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::mulFmodF32(5.5, 2.0), 1.5, natl::f32(0.01)), "mulFmod f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::mulFmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)), "mulFmod f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::mulFmodF64(5.5, 2.0), 1.5, 0.01), "mulFmod f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::mulFmodF64(-5.5, 2.0), -1.5, 0.01), "mulFmod f64 negative test");
	return test;
}

constexpr natl::Bool mulFsmodNoscaleTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "mulFsmodNoscale", natl::TestType::leaf);
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::mulFmodNoscaleF32(5.5, 2.0), 1.5 / 2, natl::f32(0.01)), "mulFsmodNoscale f32 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF32(natlm::mulFmodNoscaleF32(-5.5, 2.0), -1.5 / 2, natl::f32(0.01)), "mulFsmodNoscale f32 negative test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::mulFmodNoscaleF64(5.5, 2.0), 1.5 / 2, 0.01), "mulFsmodNoscale f64 positive test");
	NATL_TEST_ASSERT(natlm::isRoughtlyEqualF64(natlm::mulFmodNoscaleF64(-5.5, 2.0), -1.5 / 2, 0.01), "mulFsmodNoscale f64 negative test");
	return test;
}


static_assert(isnanTests());
static_assert(isInfinityTests());
static_assert(fabsTests());
static_assert(isRoughtlyEqualTests());
static_assert(basicCeilTests());
static_assert(ceilTests());
static_assert(basicFloorTests());
static_assert(floorTests());
static_assert(basicTruncTests());
static_assert(truncTests());
static_assert(basicRoundTests());
static_assert(roundTests());
static_assert(reciprocalTests());
static_assert(basicFmodTests());
static_assert(fmodTests());
static_assert(mulFsmodTests());
static_assert(mulFsmodNoscaleTests());

natl::Bool floatOperationTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "basic float operation", natl::TestType::node);
	NATL_SUB_TEST_ASSERT(isnanTests());
	NATL_SUB_TEST_ASSERT(isInfinityTests());
	NATL_SUB_TEST_ASSERT(fabsTests());
	NATL_SUB_TEST_ASSERT(isRoughtlyEqualTests());
	NATL_SUB_TEST_ASSERT(basicCeilTests());
	NATL_SUB_TEST_ASSERT(ceilTests());
	NATL_SUB_TEST_ASSERT(basicFloorTests());
	NATL_SUB_TEST_ASSERT(floorTests());
	NATL_SUB_TEST_ASSERT(basicTruncTests());
	NATL_SUB_TEST_ASSERT(truncTests());
	NATL_SUB_TEST_ASSERT(basicRoundTests());
	NATL_SUB_TEST_ASSERT(roundTests());
	NATL_SUB_TEST_ASSERT(reciprocalTests());
	NATL_SUB_TEST_ASSERT(basicFmodTests());
	NATL_SUB_TEST_ASSERT(fmodTests());
	NATL_SUB_TEST_ASSERT(mulFsmodTests());
	NATL_SUB_TEST_ASSERT(mulFsmodNoscaleTests());
	return test;
}

constexpr natl::Bool testMathFunctionF32(
	natl::ConstAsciiStringView testName, natl::f32(*customFunc)(natl::f32), natl::f32(*testFunc)(natl::f32),
	natl::f32 testStart, natl::f32 testEnd, natl::f32 incrementNum, natl::f32 relativeTolorance, natl::f32 discardSmallDif) noexcept {
	natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

	natl::Bool successful = true;
	for (natl::f32 testValue = testStart; testValue < testEnd; testValue += incrementNum) {
		const natl::f32 customValue = customFunc(testValue);
		const natl::f32 accurateValue = testFunc(testValue);
		const natl::Bool relativelyClose = natlm::isRelativelyCloseF32(customValue, accurateValue, relativeTolorance);
		const natl::Bool roughtlyEqual = natlm::isRoughtlyEqualF32(customValue, accurateValue, discardSmallDif);
		const natl::Bool closeEnough = relativelyClose || roughtlyEqual;
		NATL_TEST_ASSERT(closeEnough, "when given value ", testValue, " did not produce value without the relative tolorance of ", relativeTolorance, " values were ", customValue, " and ", accurateValue);
		successful &= closeEnough;
	}

	return successful;
}

natl::Bool testMathFunctionF64(
	natl::ConstAsciiStringView testName, natl::f64(*customFunc)(natl::f64), natl::f64(*testFunc)(natl::f64),
	natl::f64 testStart, natl::f64 testEnd, natl::f64 incrementNum, natl::f64 relativeTolorance, natl::f64 discardSmallDif) noexcept {
	natl::Test test(NATL_TEST_FROM, testName, natl::TestType::leaf);

	natl::Bool successful = true;
	for (natl::f64 testValue = testStart; testValue < testEnd; testValue += incrementNum) {
		const natl::f64 customValue = customFunc(testValue);
		const natl::f64 accurateValue = testFunc(testValue);
		const natl::Bool relativelyClose = natlm::isRelativelyCloseF64(customValue, accurateValue, relativeTolorance);
		const natl::Bool roughtlyEqual = natlm::isRoughtlyEqualF64(customValue, accurateValue, discardSmallDif);
		const natl::Bool closeEnough = relativelyClose || roughtlyEqual;

		NATL_TEST_ASSERT(closeEnough, "when given value ", testValue, " did not produce value without the relative tolorance of ", relativeTolorance, " values were ", customValue, " and ", accurateValue);
		successful &= closeEnough;
	}

	return successful;
}

natl::Bool basicSqrtImplSTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicSqrtF32ImplS", natlm::impl::basicSqrtF32ImplS, std::sqrtf,
		natl::f32(0), natl::f32(10000), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicSqrtF64ImplS", natlm::impl::basicSqrtF64ImplS, std::sqrt,
		natl::f64(0), natl::f64(10000), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool sqrtImplSTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sqrtF32ImplS", natlm::impl::sqrtF32ImplS, std::sqrtf,
		natl::f32(0), natl::f32(1000), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("sqrtF64ImplS", natlm::impl::sqrtF64ImplS, std::sqrt,
		natl::f64(0), natl::f64(1000), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool basicSqrtTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicSqrtF32", natlm::basicSqrtF32, std::sqrtf,
		natl::f32(0), natl::f32(1000), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicSqrtF64", natlm::basicSqrtF64, std::sqrt,
		natl::f64(0), natl::f64(1000), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool sqrtTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sqrtF32", natlm::sqrtF32, std::sqrtf,
		natl::f32(0), natl::f32(1000), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("sqrtF64", natlm::sqrtF64, std::sqrt,
		natl::f64(0), natl::f64(1000), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool basicRsqrtImplSTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicRsqrtF32ImplS", natlm::impl::basicRsqrtF32ImplS, 
		[](const natl::f32 value) -> natl::f32 { return natl::f32(1.0) / std::sqrtf(value); },
		natl::f32(0.00001), natl::f32(1000), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicRsqrtF64ImplS", natlm::impl::basicRsqrtF64ImplS, 
		[](const natl::f64 value) -> natl::f64 { return 1.0 / std::sqrt(value); },
		natl::f64(0.00001), natl::f64(1000), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool rsqrtImplSTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("rsqrtF32ImplS", natlm::impl::rsqrtF32ImplS,
		[](const natl::f32 value) -> natl::f32 { return natl::f32(1.0) / std::sqrtf(value); },
		natl::f32(0.00001), natl::f32(1000), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("rsqrtF64ImplS", natlm::impl::rsqrtF64ImplS,
		[](const natl::f64 value) -> natl::f64 { return 1.0 / std::sqrt(value); },
		natl::f64(0.00001), natl::f64(1000), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool basicRsqrtTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicRsqrtF32", natlm::basicRsqrtF32,
		[](const natl::f32 value) -> natl::f32 { return natl::f32(1.0) / std::sqrtf(value); },
		natl::f32(0.00001), natl::f32(1000), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicRsqrtF64", natlm::basicRsqrtF64,
		[](const natl::f64 value) -> natl::f64 { return 1.0 / std::sqrt(value); },
		natl::f64(0.00001), natl::f64(1000), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool rsqrtTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("rsqrtF32", natlm::rsqrtF32,
		[](const natl::f32 value) -> natl::f32 { return natl::f32(1.0) / std::sqrtf(value); },
		natl::f32(0.00001), natl::f32(1000), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("rsqrtF64", natlm::rsqrtF64,
		[](const natl::f64 value) -> natl::f64 { return 1.0 / std::sqrt(value); },
		natl::f64(0.00001), natl::f64(1000), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool powerTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "power", natl::TestType::node);
	NATL_SUB_TEST_ASSERT(basicSqrtImplSTest());
	NATL_SUB_TEST_ASSERT(sqrtImplSTest());
	NATL_SUB_TEST_ASSERT(basicSqrtTest());
	NATL_SUB_TEST_ASSERT(sqrtTest());
	NATL_SUB_TEST_ASSERT(basicRsqrtImplSTest());
	NATL_SUB_TEST_ASSERT(rsqrtImplSTest());
	NATL_SUB_TEST_ASSERT(basicRsqrtTest());
	NATL_SUB_TEST_ASSERT(rsqrtTest());
	return test;
}

natl::Bool basicExpDivTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicExpDivF32", natlm::basicExpDivF32, std::expf,
		natl::f32(-80), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicExpDivF64", natlm::basicExpDivF64, std::exp,
		natl::f64(-80), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool basicExpTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicExpF32", natlm::basicExpF32, std::expf,
		natl::f32(-80), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicExpF64", natlm::basicExpF64, std::exp,
		natl::f64(-80), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool basicExp2Test() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicExp2F32", natlm::basicExp2F32, std::exp2f,
		natl::f32(-80), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicExp2F64", natlm::basicExp2F64, std::exp2,
		natl::f64(-80), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool basicLog2Test() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicLog2F32", natlm::basicLog2F32, std::log2f,
		natl::f32(0.0001), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicLog2F64", natlm::basicLog2F64, std::log2,
		natl::f64(0.0001), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool log2Test() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("log2F32", natlm::log2F32, std::log2f,
		natl::f32(0.0001), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("log2F64", natlm::log2F64, std::log2,
		natl::f64(0.0001), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool basicLnTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicLnF32", natlm::basicLnF32, std::logf,
		natl::f32(0.0001), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicLnF64", natlm::basicLnF64, std::log,
		natl::f64(0.0001), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool lnTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("lnF32", natlm::lnF32, std::logf,
		natl::f32(0.0001), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("lnF64", natlm::lnF64, std::log,
		natl::f64(0.0001), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool basicLogTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicLogF32", natlm::basicLogF32, std::log10f,
		natl::f32(0.0001), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("basicLogF64", natlm::basicLogF64, std::log10,
		natl::f64(0.0001), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool logTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("logF32", natlm::logF32, std::log10f,
		natl::f32(0.0001), natl::f32(80), natl::f32(0.1), natl::f32(0.05), natl::f32(0.1));
	successful &= testMathFunctionF64("logF64", natlm::logF64, std::log10,
		natl::f64(0.0001), natl::f64(80), natl::f64(0.1), natl::f64(0.05), natl::f64(0.1));
	return successful;
}

natl::Bool exponentialTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "exponential tests", natl::TestType::node);
	NATL_SUB_TEST_ASSERT(basicExpDivTest());
	NATL_SUB_TEST_ASSERT(basicExpTest());
	NATL_SUB_TEST_ASSERT(basicExp2Test());
	NATL_SUB_TEST_ASSERT(basicLog2Test());
	NATL_SUB_TEST_ASSERT(log2Test());
	NATL_SUB_TEST_ASSERT(basicLnTest());
	NATL_SUB_TEST_ASSERT(lnTest());
	NATL_SUB_TEST_ASSERT(basicLogTest());
	NATL_SUB_TEST_ASSERT(logTest());
	return test;
}

natl::Bool sinQuadTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sinQuadF32", natlm::sinQuadF32, std::sinf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("sinQuadF64", natlm::sinQuadF64, std::sin, 
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool cosQuadTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("cosQuadTestF32", natlm::cosQuadF32, std::cosf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("cosQuadTestF64", natlm::cosQuadF64, std::cos, 
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f32(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool sinQuadMulFmodTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sinQuadMulFmodF32", natlm::sinQuadMulFmodF32, std::sinf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("sinQuadMulFmodF64", natlm::sinQuadMulFmodF64, 
		std::sin, natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool cosQuadMulFmodTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("cosQuadMulFmodF32", natlm::cosQuadMulFmodF32, std::cosf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("cosQuadMulFmodF64", natlm::cosQuadMulFmodF64, std::cos, 
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool cosTaySerd4pTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("cosTaySerd4pF32", natlm::cosTaySerd4pF32, std::cosf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.3), natl::f32(0.3));
	successful &= testMathFunctionF64("cosTaySerd4pF64", natlm::cosTaySerd4pF64, std::cos,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.3), natl::f64(0.3));
	return successful;
}

natl::Bool sinTaySerd4pTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sinTaySerd4pF32", natlm::sinTaySerd4pF32, std::sinf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.3), natl::f32(0.3));
	successful &= testMathFunctionF64("sinTaySerd4pF64", natlm::sinTaySerd4pF64, 
		std::sin, natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.3), natl::f64(0.3));
	return successful;
}

natl::Bool tan4dpTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("tan4dpTestF32", natlm::tan4dpF32, std::tanf, 
		natl::f32(-natlm::piOver2 + 0.1), natl::f32(natlm::piOver2 - 0.1), natl::f32(0.001), natl::f32(0.02), natl::f32(0.02));
	successful &= testMathFunctionF64("tan4dpTestF64", natlm::tan4dpF64, std::tan, 
		natl::f64(-natlm::piOver2 + 0.1), natl::f64(natlm::piOver2 - 0.1), natl::f64(0.001), natl::f32(0.02), natl::f32(0.02));
	return successful;
}

natl::Bool asinQuadTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("asinQuadF32", natlm::asinQuadF32, std::asinf, 
		natl::f32(-1), natl::f32(1), natl::f32(0.001), natl::f32(0.05), natl::f32(0.05));
	successful &= testMathFunctionF64("asinQuadF64", natlm::asinQuadF64, std::asin, 
		natl::f64(-1), natl::f64(1), natl::f64(0.001), natl::f32(0.05), natl::f32(0.05));
	return successful;
}

natl::Bool acosQuadTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("acosQuadF32", natlm::acosQuadF32, std::acosf, 
		natl::f32(-1), natl::f32(1), natl::f32(0.001), natl::f32(0.08), natl::f32(0.08));
	successful &= testMathFunctionF64("acosQuadF64", natlm::acosQuadF64, std::acos, 
		natl::f64(-1), natl::f64(1), natl::f64(0.001), natl::f32(0.08), natl::f32(0.08));
	return successful;
}

natl::Bool atan4dpTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("atan4dpF32", natlm::atan4dpF32, std::atanf, 
		natl::f32(-1), natl::f32(1), natl::f32(0.001), natl::f32(0.008), natl::f32(0.001));
	successful &= testMathFunctionF64("atan4dpF64", natlm::atan4dpF64, std::atan, 
		natl::f64(-1), natl::f64(1), natl::f64(0.001), natl::f32(0.008), natl::f32(0.001));
	return successful;
}

natl::Bool trigonometricTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "trigonometric tests", natl::TestType::node);
	NATL_SUB_TEST_ASSERT(sinQuadTest());
	NATL_SUB_TEST_ASSERT(cosQuadTest());
	NATL_SUB_TEST_ASSERT(sinQuadMulFmodTest());
	NATL_SUB_TEST_ASSERT(cosQuadMulFmodTest());
	NATL_SUB_TEST_ASSERT(cosTaySerd4pTest());
	NATL_SUB_TEST_ASSERT(sinTaySerd4pTest());
	NATL_SUB_TEST_ASSERT(tan4dpTest());
	NATL_SUB_TEST_ASSERT(asinQuadTest());
	NATL_SUB_TEST_ASSERT(acosQuadTest());
	NATL_SUB_TEST_ASSERT(atan4dpTest());
	return test;
}

natl::Bool basicSinhTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicSinhF32", natlm::basicSinhF32, std::sinhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("basicSinhF64", natlm::basicSinhF64, std::sinh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool sinhTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sinhF32", natlm::sinhF32, std::sinhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("sinhF64", natlm::sinhF64, std::sinh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool basicCoshTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicCoshF32", natlm::basicCoshF32, std::coshf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("basicCoshF64", natlm::basicCoshF64, std::cosh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool coshTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("coshF32", natlm::coshF32, std::coshf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("coshF64", natlm::coshF64, std::cosh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool basicTanhTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicTanhF32", natlm::basicTanhF32, std::tanhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("basicTanhF64", natlm::basicTanhF64, std::tanh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool tanhTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("tanhF32", natlm::tanhF32, std::tanhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("tanhF64", natlm::tanhF64, std::tanh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool basicAsinhTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicAsinhF32", natlm::basicAsinhF32, std::asinhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("basicAsinhF64", natlm::basicAsinhF64, std::asinh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool asinhTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("asinhF32", natlm::asinhF32, std::asinhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("asinhF64", natlm::asinhF64, std::asinh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool basicAcoshTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicAcoshF32", natlm::basicAcoshF32, std::acoshf,
		natl::f32(1), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("basicAcoshF64", natlm::basicAcoshF64, std::acosh,
		natl::f64(1), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool acoshTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("acoshF32", natlm::acoshF32, std::acoshf,
		natl::f32(1), natl::f32(natlm::twoPi), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("acoshF64", natlm::acoshF64, std::acosh,
		natl::f64(1), natl::f64(natlm::twoPi), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool basicAtanhTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicAtanhF32", natlm::basicAtanhF32, std::atanhf,
		natl::f32(-0.99999), natl::f32(1), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("basicAtanhF64", natlm::basicAtanhF64, std::atanh,
		natl::f64(-0.99999), natl::f64(1), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool atanhTest() noexcept {
	natl::Bool successful = true;
	successful &= testMathFunctionF32("atanhF32", natlm::atanhF32, std::atanhf,
		natl::f32(-1), natl::f32(1), natl::f32(0.001), natl::f32(0.1), natl::f32(0.1));
	successful &= testMathFunctionF64("atanhF64", natlm::atanhF64, std::atanh,
		natl::f64(-1), natl::f64(1), natl::f64(0.001), natl::f64(0.1), natl::f64(0.1));
	return successful;
}

natl::Bool hyperbolicTests() noexcept {
	natl::Test test(NATL_TEST_FROM, "hyperbolic tests", natl::TestType::node);
	NATL_SUB_TEST_ASSERT(basicSinhTest());
	NATL_SUB_TEST_ASSERT(sinhTest());
	NATL_SUB_TEST_ASSERT(basicCoshTest());
	NATL_SUB_TEST_ASSERT(coshTest());
	NATL_SUB_TEST_ASSERT(basicTanhTest());
	NATL_SUB_TEST_ASSERT(tanhTest());
	NATL_SUB_TEST_ASSERT(basicAsinhTest());
	NATL_SUB_TEST_ASSERT(asinhTest());
	NATL_SUB_TEST_ASSERT(basicAcoshTest());
	NATL_SUB_TEST_ASSERT(acoshTest());
	NATL_SUB_TEST_ASSERT(basicAtanhTest());
	NATL_SUB_TEST_ASSERT(atanhTest());
	return test;
}

natl::Bool tests() noexcept {
	natl::Test test(NATL_TEST_FROM, "all", natl::TestType::root);
	NATL_SUB_TEST_ASSERT(saturatedArithmeticTests());
	NATL_SUB_TEST_ASSERT(floatOperationTests());
	NATL_SUB_TEST_ASSERT(powerTests());
	NATL_SUB_TEST_ASSERT(exponentialTests());
	NATL_SUB_TEST_ASSERT(trigonometricTests());
	NATL_SUB_TEST_ASSERT(hyperbolicTests());
	return test;
}

int main() noexcept {
	natl::Timer timer;
	tests();
	natl::printlnf("natl: ", NATL_TEST_FROM, " time: ", timer.getMillisecondsInt());
}