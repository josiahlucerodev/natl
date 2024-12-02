
//std
#include <cmath>

//natl
#include <natl/math.h>
#include <natl/system/timer.h>
#include <natl/util/test.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlMathTest";

//saturated arithmetic test
template<typename IntegerType, natl::TemplateStringLiteral IntegerNameTemplate>
constexpr natl::Bool integerAddsatTest() noexcept {
	constexpr auto IntegerName = natl::StringLiteral<IntegerNameTemplate>::toStringView();
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " addsat", natl::TestType::leaf);
	natl::testAssert(test, natlm::addsat<IntegerType>(natl::Limits<IntegerType>::max(), natl::Limits<IntegerType>::max()) == natl::Limits<IntegerType>::max(), IntegerName, " addsat overflow max");
	natl::testAssert(test, natlm::addsat<IntegerType>(natl::Limits<IntegerType>::min(), natl::Limits<IntegerType>::min()) == natl::Limits<IntegerType>::min(), IntegerName, " addsat overflow min");
	natl::testAssert(test, natlm::addsat<IntegerType>(2, 3) == 5, IntegerName, " addsat standard add");
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
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " subsat", natl::TestType::leaf);
	natl::testAssert(test, natlm::subsat<IntegerType>(natl::Limits<IntegerType>::max(), natl::Limits<IntegerType>::min()) == natl::Limits<IntegerType>::max(), IntegerName, " subsat overflow max test");
	natl::testAssert(test, natlm::subsat<IntegerType>(natl::Limits<IntegerType>::min(), natl::Limits<IntegerType>::max()) == natl::Limits<IntegerType>::min(), IntegerName, " subsat overflow min test");
	natl::testAssert(test, natlm::subsat<IntegerType>(8, 3) == 5, IntegerName, " subsat standard sub test");
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
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " mulsat", natl::TestType::leaf);
	natl::testAssert(test, natlm::mulsat<IntegerType>(natl::Limits<IntegerType>::max(), natl::Limits<IntegerType>::max()) == natl::Limits<IntegerType>::max(), IntegerName, " mulsat overflow max test");
	natl::testAssert(test, natlm::mulsat<IntegerType>(natl::Limits<IntegerType>::min(), natl::Limits<IntegerType>::max()) == natl::Limits<IntegerType>::min(), IntegerName, " mulsat overflow min test");
	natl::testAssert(test, natlm::mulsat<IntegerType>(5, 4) == 20, IntegerName, " mulsat standard mul test");
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
	natl::Test test(natlTestFrom, natl::String(IntegerName) + " integer divsat", natl::TestType::leaf);
	if constexpr (natl::IsBuiltInSignedIntegerC<IntegerType>) {
		natl::testAssert(test, natlm::divsat<IntegerType>(natl::Limits<IntegerType>::min(), IntegerType(-1)) == natl::Limits<IntegerType>::max(), IntegerName, " divsat overflow max test");
	}
	natl::testAssert(test, natlm::divsat<IntegerType>(20, 4) == 5, IntegerName, " divsat standard mul test");
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
	natl::Test test(natlTestFrom, "saturated arithmetic", natl::TestType::node);
	natl::subTestAssert(test, addsatTest());
	natl::subTestAssert(test, subsatTest());
	natl::subTestAssert(test, mulsatTest());
	natl::subTestAssert(test, divsatTest());
	return test;
}

//float operations 
constexpr natl::Bool isnanTests() noexcept {
	natl::Test test(natlTestFrom, "isnan", natl::TestType::leaf);
	natl::testAssert(test, natlm::isnanF32(natl::Limits<natl::f32>::quietNaN()), "isnan f32 test");
	natl::testAssert(test, !natlm::isnanF32(1.0), "not isnan f32 test");
	natl::testAssert(test, natlm::isnanF64(natl::Limits<natl::f64>::quietNaN()), "isnan f64 test");
	natl::testAssert(test, !natlm::isnanF64(1.0), "not isnan f64 test");
	return test;
}

constexpr natl::Bool isInfinityTests() noexcept {
	natl::Test test(natlTestFrom, "is infinity", natl::TestType::leaf);
	natl::testAssert(test, natlm::isInfinityF32(natl::Limits<natl::f32>::infinity()), "isInfinity f32 test");
	natl::testAssert(test, !natlm::isInfinityF32(1.0), "not isInfinity f32 test");
	natl::testAssert(test, natlm::isInfinityF64(natl::Limits<natl::f64>::infinity()), "isInfinity f64 test");
	natl::testAssert(test, !natlm::isInfinityF64(1.0), "not isInfinity f64 test");
	return test;
}

constexpr natl::Bool fabsTests() noexcept {
	using namespace natl::literals;
	natl::Test test(natlTestFrom, "fabs", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::fabsF32(3.0_f32), 3.0_f32, 0.01_f32), "fabs f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::fabsF32(-3.0_f32), 3.0_f32, 0.01_f32), "fabs f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::fabsF64(3.0_f64), 3.0_f64, 0.01_f64), "fabs f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::fabsF64(-3.0_f64), 3.0_f64, 0.01_f64), "fabs f64 negative test");
	return test;
}

constexpr natl::Bool isRoughtlyEqualTests() noexcept {
	using namespace natl::literals;
	natl::Test test(natlTestFrom, "isRoughtlyEqual", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(1.0_f32, 3.0_f32, 3.0_f32), "isRoughtlyEqual f32 test");
	natl::testAssert(test, !natlm::isRoughtlyEqualF32(1.0_f32, 3.0_f32, 1.0_f32), "not isRoughtlyEqual f32 test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(1.0_f64, 3.0_f64, 3.0_f64), "isRoughtlyEqual f64 test");
	natl::testAssert(test, !natlm::isRoughtlyEqualF64(1.0_f64, 3.0_f64, 1.0_f64), "not isRoughtlyEqual f64 test");
	return test;
}

constexpr natl::Bool basicCeilTests() noexcept {
	using namespace natl::literals;
	natl::Test test(natlTestFrom, "basicCeil", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicCeilF32(5.5_f32), 6.0_f32, 0.01_f32), "basicCeil f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicCeilF32(-5.5_f32), -5.0_f32, 0.01_f32), "basicCeil f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicCeilF64(5.5_f64), 6.0_f64, 0.01_f64), "basicCeil f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicCeilF64(-5.5_f64), -5.0_f64, 0.01_f64), "basicCeil f64 negative test");
	return test;
}

constexpr natl::Bool ceilTests() noexcept {
	natl::Test test(natlTestFrom, "ceil", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::ceilF32(5.5), 6.0, natl::f32(0.01)), "ceil f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::ceilF32(-5.5), -5.0, natl::f32(0.01)), "ceil f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::ceilF64(5.5), 6.0, 0.01), "ceil f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::ceilF64(-5.5), -5.0, 0.01), "ceil f64 negative test");
	return test;
}

constexpr natl::Bool basicFloorTests() noexcept {
	natl::Test test(natlTestFrom, "basicFloor", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicFloorF32(5.5), 5.0, natl::f32(0.01)), "basicFloor f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicFloorF32(-5.5), -6.0, natl::f32(0.01)), "basicFloor f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicFloorF64(5.5), 5.0, 0.01), "basicFloor f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicFloorF64(-5.5), -6.0, 0.01), "basicFloor f64 negative test");
	return test;
}

constexpr natl::Bool floorTests() noexcept {
	natl::Test test(natlTestFrom, "floor", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::floorF32(5.5), 5.0, natl::f32(0.01)), "floor f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::floorF32(-5.5), -6.0, natl::f32(0.01)), "floor f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::floorF64(5.5), 5.0, 0.01), "floor f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::floorF64(-5.5), -6.0, 0.01), "floor f64 negative test");
	return test;
}

constexpr natl::Bool basicTruncTests() noexcept {
	natl::Test test(natlTestFrom, "basicTrunc", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicTruncF32(5.5), 5.0, natl::f32(0.01)), "basicTrunc f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicTruncF32(-5.5), -5.0, natl::f32(0.01)), "basicTrunc f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicTruncF32(0.5), 0.0, natl::f32(0.01)), "basicTrunc f32 positive to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicTruncF32(-0.5), 0.0, natl::f32(0.01)), "basicTrunc f32 negative to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicTruncF64(5.5), 5.0, 0.01), "basicTrunc f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicTruncF64(-5.5), -5.0, 0.01), "basicTrunc f64 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicTruncF64(0.5), 0.0, 0.01), "basicTrunc f64 positive to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicTruncF64(-0.5), 0.0, 0.01), "basicTrunc f64 negative to zero test");
	return test;
}

constexpr natl::Bool truncTests() noexcept {
	natl::Test test(natlTestFrom, "trunc", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::truncF32(5.5), 5.0, natl::f32(0.01)), "trunc f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::truncF32(-5.5), -5.0, natl::f32(0.01)), "trunc f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::truncF32(0.5), 0.0, natl::f32(0.01)), "trunc f32 positive to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::truncF32(-0.5), 0.0, natl::f32(0.01)), "trunc f32 negative to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::truncF64(5.5), 5.0, 0.01), "trunc f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::truncF64(-5.5), -5.0, 0.01), "trunc f64 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::truncF64(0.5), 0.0, 0.01), "trunc f64 positive to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::truncF64(-0.5), 0.0, 0.01), "trunc f64 negative to zero test");
	return test;
}

constexpr natl::Bool basicRoundTests() noexcept {
	using namespace natl::literals;
	natl::Test test(natlTestFrom, "basicRound", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicRoundF32(1.4_f32), 1.0_f32, 0.01_f32), "basicRound f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicRoundF32(-1.4_f32), -1.0_f32, 0.01_f32), "basicRound f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicRoundF32(0.4_f32), 0.0_f32, 0.01_f32), "basicRound f32 positive to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicRoundF32(-0.4_f32), 0.0_f32, 0.01_f32), "basicRound f32 negative to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicRoundF64(1.4_f64), 1.0_f64, 0.01_f64), "basicRound f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicRoundF64(-1.4_f64), -1.0_f64, 0.01_f64), "basicRound f64 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicRoundF64(0.4_f64), 0.0_f64, 0.01_f64), "basicRound f64 positive to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicRoundF64(-0.4_f64), 0.0_f64, 0.01_f64), "basicRound f64 negative to zero test");
	return test;
}

constexpr natl::Bool roundTests() noexcept {
	using namespace natl::literals;
	natl::Test test(natlTestFrom, "round", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::roundF32(1.4_f32), 1.0_f32, 0.01_f32), "round f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::roundF32(-1.4_f32), -1.0_f32, 0.01_f32), "round f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::roundF32(0.4_f32), 0.0_f32, 0.01_f32), "round f32 positive to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::roundF32(-0.4_f32), 0.0_f32, 0.01_f32), "round f32 negative to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::roundF64(1.4_f64), 1.0_f64, 0.01_f64), "round f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::roundF64(-1.4_f64), -1.0_f64, 0.01_f64), "round f64 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::roundF64(0.4_f64), 0.0_f64, 0.01_f64), "round f64 positive to zero test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::roundF64(-0.4_f64), 0.0_f64, 0.01_f64), "round f64 negative to zero test");
	return test;
}

constexpr natl::Bool reciprocalTests() noexcept {
	natl::Test test(natlTestFrom, "reciprocal", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::reciprocalF32(2.0), 0.5, natl::f32(0.01)), "reciprocal f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::reciprocalF32(-2.0), -0.5, natl::f32(0.01)), "reciprocal f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::reciprocalF64(2.0), 0.5, 0.01), "reciprocal f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::reciprocalF64(-2.0), -0.5, 0.01), "reciprocal f64 negative test");
	return test;
}

constexpr natl::Bool basicFmodTests() noexcept {
	natl::Test test(natlTestFrom, "basicFmod", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicFmodF32(5.5, 2.0), 1.5, natl::f32(0.01)), "basicFmod f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::basicFmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)), "basicFmod f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicFmodF64(5.5, 2.0), 1.5, 0.01), "basicFmod f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::basicFmodF64(-5.5, 2.0), -1.5, 0.01), "basicFmod f64 negative test");
	return test;
}

constexpr natl::Bool fmodTests() noexcept {
	natl::Test test(natlTestFrom, "fmod", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::fmodF32(5.5, 2.0), 1.5, natl::f32(0.01)), "fmod f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::fmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)), "fmod f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::fmodF64(5.5, 2.0), 1.5, 0.01), "fmod f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::fmodF64(-5.5, 2.0), -1.5, 0.01), "fmod f64 negative test");
	return test;
}


constexpr natl::Bool mulFsmodTests() noexcept {
	natl::Test test(natlTestFrom, "mulFmod", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::mulFmodF32(5.5, 2.0), 1.5, natl::f32(0.01)), "mulFmod f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::mulFmodF32(-5.5, 2.0), -1.5, natl::f32(0.01)), "mulFmod f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::mulFmodF64(5.5, 2.0), 1.5, 0.01), "mulFmod f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::mulFmodF64(-5.5, 2.0), -1.5, 0.01), "mulFmod f64 negative test");
	return test;
}

constexpr natl::Bool mulFsmodNoscaleTests() noexcept {
	natl::Test test(natlTestFrom, "mulFsmodNoscale", natl::TestType::leaf);
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::mulFmodNoscaleF32(5.5, 2.0), 1.5 / 2, natl::f32(0.01)), "mulFsmodNoscale f32 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF32(natlm::mulFmodNoscaleF32(-5.5, 2.0), -1.5 / 2, natl::f32(0.01)), "mulFsmodNoscale f32 negative test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::mulFmodNoscaleF64(5.5, 2.0), 1.5 / 2, 0.01), "mulFsmodNoscale f64 positive test");
	natl::testAssert(test, natlm::isRoughtlyEqualF64(natlm::mulFmodNoscaleF64(-5.5, 2.0), -1.5 / 2, 0.01), "mulFsmodNoscale f64 negative test");
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
	natl::Test test(natlTestFrom, "basic float operation", natl::TestType::node);
	natl::subTestAssert(test, isnanTests());
	natl::subTestAssert(test, isInfinityTests());
	natl::subTestAssert(test, fabsTests());
	natl::subTestAssert(test, isRoughtlyEqualTests());
	natl::subTestAssert(test, basicCeilTests());
	natl::subTestAssert(test, ceilTests());
	natl::subTestAssert(test, basicFloorTests());
	natl::subTestAssert(test, floorTests());
	natl::subTestAssert(test, basicTruncTests());
	natl::subTestAssert(test, truncTests());
	natl::subTestAssert(test, basicRoundTests());
	natl::subTestAssert(test, roundTests());
	natl::subTestAssert(test, reciprocalTests());
	natl::subTestAssert(test, basicFmodTests());
	natl::subTestAssert(test, fmodTests());
	natl::subTestAssert(test, mulFsmodTests());
	natl::subTestAssert(test, mulFsmodNoscaleTests());
	return test;
}

constexpr natl::Bool testMathFunctionF32(
	natl::ConstAsciiStringView testName, natl::f32(*customFunc)(natl::f32), natl::f32(*testFunc)(natl::f32),
	natl::f32 testStart, natl::f32 testEnd, natl::f32 incrementNum, natl::f32 relativeTolorance, natl::f32 discardSmallDif) noexcept {
	natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

	natl::Bool successful = true;
	for (natl::f32 testValue = testStart; testValue < testEnd; testValue += incrementNum) {
		const natl::f32 customValue = customFunc(testValue);
		const natl::f32 accurateValue = testFunc(testValue);
		const natl::Bool relativelyClose = natlm::isRelativelyCloseF32(customValue, accurateValue, relativeTolorance);
		const natl::Bool roughtlyEqual = natlm::isRoughtlyEqualF32(customValue, accurateValue, discardSmallDif);
		const natl::Bool closeEnough = relativelyClose || roughtlyEqual;
		natl::testAssert(test, closeEnough, "when given value ", testValue, " did not produce value without the relative tolorance of ", relativeTolorance, " values were ", customValue, " and ", accurateValue);
		successful &= closeEnough;
	}

	return successful;
}

natl::Bool testMathFunctionF64(
	natl::ConstAsciiStringView testName, natl::f64(*customFunc)(natl::f64), natl::f64(*testFunc)(natl::f64),
	natl::f64 testStart, natl::f64 testEnd, natl::f64 incrementNum, natl::f64 relativeTolorance, natl::f64 discardSmallDif) noexcept {
	natl::Test test(natlTestFrom, testName, natl::TestType::leaf);

	natl::Bool successful = true;
	for (natl::f64 testValue = testStart; testValue < testEnd; testValue += incrementNum) {
		const natl::f64 customValue = customFunc(testValue);
		const natl::f64 accurateValue = testFunc(testValue);
		const natl::Bool relativelyClose = natlm::isRelativelyCloseF64(customValue, accurateValue, relativeTolorance);
		const natl::Bool roughtlyEqual = natlm::isRoughtlyEqualF64(customValue, accurateValue, discardSmallDif);
		const natl::Bool closeEnough = relativelyClose || roughtlyEqual;

		natl::testAssert(test, closeEnough, "when given value ", testValue, " did not produce value without the relative tolorance of ", relativeTolorance, " values were ", customValue, " and ", accurateValue);
		successful &= closeEnough;
	}

	return successful;
}

natl::Bool basicSqrtImplSTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicSqrtF32ImplS", natlm::impl::basicSqrtF32ImplS, sqrtf,
		0.0_f32, natl::f32(10000), 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicSqrtF64ImplS", natlm::impl::basicSqrtF64ImplS, sqrt,
		0.0_f64, natl::f64(10000), 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool sqrtImplSTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sqrtF32ImplS", natlm::impl::sqrtF32ImplS, sqrtf,
		0.0_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("sqrtF64ImplS", natlm::impl::sqrtF64ImplS, sqrt,
		0.0_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool basicSqrtTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicSqrtF32", natlm::basicSqrtF32, sqrtf,
		0.0_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicSqrtF64", natlm::basicSqrtF64, sqrt,
		0.0_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool sqrtTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sqrtF32", natlm::sqrtF32, sqrtf,
		0.0_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("sqrtF64", natlm::sqrtF64, sqrt,
		0.0_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool basicRsqrtImplSTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicRsqrtF32ImplS", natlm::impl::basicRsqrtF32ImplS, 
		[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
		0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicRsqrtF64ImplS", natlm::impl::basicRsqrtF64ImplS, 
		[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
		0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool rsqrtImplSTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("rsqrtF32ImplS", natlm::impl::rsqrtF32ImplS,
		[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
		0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("rsqrtF64ImplS", natlm::impl::rsqrtF64ImplS,
		[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
		0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool basicRsqrtTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicRsqrtF32", natlm::basicRsqrtF32,
		[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
		0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicRsqrtF64", natlm::basicRsqrtF64,
		[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
		0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool rsqrtTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("rsqrtF32", natlm::rsqrtF32,
		[](const natl::f32 value) -> natl::f32 { return 1.0_f32 / sqrtf(value); },
		0.00001_f32, 1000.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("rsqrtF64", natlm::rsqrtF64,
		[](const natl::f64 value) -> natl::f64 { return 1.0 / sqrt(value); },
		0.00001_f64, 1000.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool powerTests() noexcept {
	natl::Test test(natlTestFrom, "power", natl::TestType::node);
	natl::subTestAssert(test, basicSqrtImplSTest());
	natl::subTestAssert(test, sqrtImplSTest());
	natl::subTestAssert(test, basicSqrtTest());
	natl::subTestAssert(test, sqrtTest());
	natl::subTestAssert(test, basicRsqrtImplSTest());
	natl::subTestAssert(test, rsqrtImplSTest());
	natl::subTestAssert(test, basicRsqrtTest());
	natl::subTestAssert(test, rsqrtTest());
	return test;
}

natl::Bool basicExpDivTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicExpDivF32", natlm::basicExpDivF32, expf,
		-80.0_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicExpDivF64", natlm::basicExpDivF64, exp,
		-80.0_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool basicExpTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicExpF32", natlm::basicExpF32, expf,
		-80.0_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicExpF64", natlm::basicExpF64, exp,
		-80.0_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool basicExp2Test() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicExp2F32", natlm::basicExp2F32, exp2f,
		-80.0_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicExp2F64", natlm::basicExp2F64, exp2,
		-80.0_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool basicLog2Test() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicLog2F32", natlm::basicLog2F32, log2f,
		0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicLog2F64", natlm::basicLog2F64, log2,
		0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool log2Test() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("log2F32", natlm::log2F32, log2f,
		0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("log2F64", natlm::log2F64, log2,
		0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool basicLnTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicLnF32", natlm::basicLnF32, logf,
		0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicLnF64", natlm::basicLnF64, log,
		0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool lnTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("lnF32", natlm::lnF32, logf,
		0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("lnF64", natlm::lnF64, log,
		0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool basicLogTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicLogF32", natlm::basicLogF32, log10f,
		0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicLogF64", natlm::basicLogF64, log10,
		0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool logTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("logF32", natlm::logF32, log10f,
		0.0001_f32, 80.0_f32, 0.1_f32, 0.05_f32, 0.1_f32);
	successful &= testMathFunctionF64("logF64", natlm::logF64, log10,
		0.0001_f64, 80.0_f64, 0.1_f64, 0.05_f64, 0.1_f64);
	return successful;
}

natl::Bool exponentialTests() noexcept {
	natl::Test test(natlTestFrom, "exponential tests", natl::TestType::node);
	natl::subTestAssert(test, basicExpDivTest());
	natl::subTestAssert(test, basicExpTest());
	natl::subTestAssert(test, basicExp2Test());
	natl::subTestAssert(test, basicLog2Test());
	natl::subTestAssert(test, log2Test());
	natl::subTestAssert(test, basicLnTest());
	natl::subTestAssert(test, lnTest());
	natl::subTestAssert(test, basicLogTest());
	natl::subTestAssert(test, logTest());
	return test;
}

natl::Bool sinQuadTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sinQuadF32", natlm::sinQuadF32, sinf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("sinQuadF64", natlm::sinQuadF64, sin, 
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool cosQuadTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("cosQuadTestF32", natlm::cosQuadF32, cosf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("cosQuadTestF64", natlm::cosQuadF64, cos, 
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool sinQuadMulFmodTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sinQuadMulFmodF32", natlm::sinQuadMulFmodF32, sinf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("sinQuadMulFmodF64", natlm::sinQuadMulFmodF64, 
		sin, natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool cosQuadMulFmodTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("cosQuadMulFmodF32", natlm::cosQuadMulFmodF32, cosf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("cosQuadMulFmodF64", natlm::cosQuadMulFmodF64, cos, 
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool cosTaySerd4pTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("cosTaySerd4pF32", natlm::cosTaySerd4pF32, cosf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.3_f32, 0.3_f32);
	successful &= testMathFunctionF64("cosTaySerd4pF64", natlm::cosTaySerd4pF64, cos,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.3_f64, 0.3_f64);
	return successful;
}

natl::Bool sinTaySerd4pTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sinTaySerd4pF32", natlm::sinTaySerd4pF32, sinf, 
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.3_f32, 0.3_f32);
	successful &= testMathFunctionF64("sinTaySerd4pF64", natlm::sinTaySerd4pF64, 
		sin, natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.3_f64, 0.3_f64);
	return successful;
}

natl::Bool tan4dpTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("tan4dpTestF32", natlm::tan4dpF32, tanf, 
		natl::f32(-natlm::piOver2 + 0.1), natl::f32(natlm::piOver2 - 0.1), 0.001_f32, 0.02_f32, 0.02_f32);
	successful &= testMathFunctionF64("tan4dpTestF64", natlm::tan4dpF64, tan, 
		natl::f64(-natlm::piOver2 + 0.1), natl::f64(natlm::piOver2 - 0.1), 0.001_f64, 0.02_f64, 0.02_f64);
	return successful;
}

natl::Bool asinQuadTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("asinQuadF32", natlm::asinQuadF32, asinf, 
		-1.0_f32, 1.0_f32, 0.001_f32, 0.05_f32, 0.05_f32);
	successful &= testMathFunctionF64("asinQuadF64", natlm::asinQuadF64, asin, 
		-1.0_f64, 1.0_f64, 0.001_f64, 0.05_f64, 0.05_f64);
	return successful;
}

natl::Bool acosQuadTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("acosQuadF32", natlm::acosQuadF32, acosf, 
		-1.0_f32, 1.0_f32, 0.001_f32, 0.08_f32, 0.08_f32);
	successful &= testMathFunctionF64("acosQuadF64", natlm::acosQuadF64, acos, 
		-1.0_f64, 1.0_f64, 0.001_f64, 0.08_f64, 0.08_f64);
	return successful;
}

natl::Bool atan4dpTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("atan4dpF32", natlm::atan4dpF32, atanf, 
		-1.0_f32, 1.0_f32, 0.001_f32, 0.008_f32, 0.001_f32);
	successful &= testMathFunctionF64("atan4dpF64", natlm::atan4dpF64, atan, 
		-1.0_f64, 1.0_f64, 0.001_f64, 0.008_f64, 0.001_f64);
	return successful;
}

natl::Bool trigonometricTests() noexcept {
	natl::Test test(natlTestFrom, "trigonometric tests", natl::TestType::node);
	natl::subTestAssert(test, sinQuadTest());
	natl::subTestAssert(test, cosQuadTest());
	natl::subTestAssert(test, sinQuadMulFmodTest());
	natl::subTestAssert(test, cosQuadMulFmodTest());
	natl::subTestAssert(test, cosTaySerd4pTest());
	natl::subTestAssert(test, sinTaySerd4pTest());
	natl::subTestAssert(test, tan4dpTest());
	natl::subTestAssert(test, asinQuadTest());
	natl::subTestAssert(test, acosQuadTest());
	natl::subTestAssert(test, atan4dpTest());
	return test;
}

natl::Bool basicSinhTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicSinhF32", natlm::basicSinhF32, sinhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicSinhF64", natlm::basicSinhF64, sinh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool sinhTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("sinhF32", natlm::sinhF32, sinhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("sinhF64", natlm::sinhF64, sinh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool basicCoshTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicCoshF32", natlm::basicCoshF32, coshf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicCoshF64", natlm::basicCoshF64, cosh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool coshTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("coshF32", natlm::coshF32, coshf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("coshF64", natlm::coshF64, cosh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool basicTanhTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicTanhF32", natlm::basicTanhF32, tanhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicTanhF64", natlm::basicTanhF64, tanh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool tanhTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("tanhF32", natlm::tanhF32, tanhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("tanhF64", natlm::tanhF64, tanh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool basicAsinhTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicAsinhF32", natlm::basicAsinhF32, asinhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicAsinhF64", natlm::basicAsinhF64, asinh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool asinhTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("asinhF32", natlm::asinhF32, asinhf,
		natl::f32(-natlm::twoPi), natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("asinhF64", natlm::asinhF64, asinh,
		natl::f64(-natlm::twoPi), natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool basicAcoshTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicAcoshF32", natlm::basicAcoshF32, acoshf,
		1.0_f32, natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicAcoshF64", natlm::basicAcoshF64, acosh,
		1.0_f64, natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool acoshTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("acoshF32", natlm::acoshF32, acoshf,
		1.0_f32, natl::f32(natlm::twoPi), 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("acoshF64", natlm::acoshF64, acosh,
		1.0_f64, natl::f64(natlm::twoPi), 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool basicAtanhTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("basicAtanhF32", natlm::basicAtanhF32, atanhf,
		natl::f32(-0.99999), 1.0_f32, 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("basicAtanhF64", natlm::basicAtanhF64, atanh,
		natl::f64(-0.99999), 1.0_f64, 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool atanhTest() noexcept {
	using namespace natl::literals;
	natl::Bool successful = true;
	successful &= testMathFunctionF32("atanhF32", natlm::atanhF32, atanhf,
		-1.0_f32, 1.0_f32, 0.001_f32, 0.1_f32, 0.1_f32);
	successful &= testMathFunctionF64("atanhF64", natlm::atanhF64, atanh,
		-1.0_f64, 1.0_f64, 0.001_f64, 0.1_f64, 0.1_f64);
	return successful;
}

natl::Bool hyperbolicTests() noexcept {
	natl::Test test(natlTestFrom, "hyperbolic tests", natl::TestType::node);
	natl::subTestAssert(test, basicSinhTest());
	natl::subTestAssert(test, sinhTest());
	natl::subTestAssert(test, basicCoshTest());
	natl::subTestAssert(test, coshTest());
	natl::subTestAssert(test, basicTanhTest());
	natl::subTestAssert(test, tanhTest());
	natl::subTestAssert(test, basicAsinhTest());
	natl::subTestAssert(test, asinhTest());
	natl::subTestAssert(test, basicAcoshTest());
	natl::subTestAssert(test, acoshTest());
	natl::subTestAssert(test, basicAtanhTest());
	natl::subTestAssert(test, atanhTest());
	return test;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, saturatedArithmeticTests());
	natl::subTestAssert(test, floatOperationTests());
	natl::subTestAssert(test, powerTests());
	natl::subTestAssert(test, exponentialTests());
	natl::subTestAssert(test, trigonometricTests());
	natl::subTestAssert(test, hyperbolicTests());
	return test;
}

int main() noexcept {
	tests();
}