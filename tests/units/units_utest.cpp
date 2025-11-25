//@file_not_module

//@begin_non_modules
//natl
#include <natl/util/testMacros.h>
//@end_non_modules

//natl
#include <natl/util/test.h>
#include <natl/units/all.h>
#include <natl/system/printFormatted.h>

struct UnitTestSuite : public natl::TestSuite<UnitTestSuite> {};

template<typename test_suite_type = UnitTestSuite>
struct UnitTestSuiteTests {
	template<typename UnitType>
	constexpr static natl::i64 unitsAdd() noexcept {
		UnitType unit1(4);
		UnitType unit2(6);
		return (unit1 + unit2).value();
	}

	template<typename UnitType>
		requires(natl::IsUnitValue<UnitType>&& natl::IsQuantityOf<UnitType, natl::abbrt::time::negQuaded>)
	constexpr static void work(const UnitType&) noexcept {}

	constexpr static void tests(test_suite_type& ts) noexcept {
		ts.assertEqual(10, unitsAdd<natl::Meter<natl::i64>>());
		ts.assertEqual(10, unitsAdd<natl::Foot<natl::i64>>());
		ts.assertEqual(10, unitsAdd<natl::Gram<natl::i64>>());
		ts.assertEqual(1000000, natl::Seconds<natl::ui64>(1).convertTo<natl::abbrt::mus>().value());

		auto value = natl::Squared<natl::Nanoseconds<natl::i64>::negSquared>(80);
		work(value);

		ts.assertEqual("0x50 ns", natl::sformat(natl::formatArg<natl::FormatUnitAbbr, natl::FormatIntHexFlag>(value)));
		ts.assertEqual("0x50 time", natl::sformat(natl::formatArg<natl::FormatUnitDimension, natl::FormatIntHexFlag>(value)));
		ts.assertEqual("time", natl::sformat(natl::formatArg<natl::FormatUnitDimension>(value.units())));
		ts.assertEqual("Gs", natl::abbrt::Gs::unit_tag::abbreviation_name::toStringView());

	}
	NATL_REGISTER_TEST_CONSTEXPR(tests);
};

NATL_REGISTER_STD_TESTS(UnitTestSuite);

NATL_TESTS_MAIN();