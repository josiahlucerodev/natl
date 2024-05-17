
//natl
#include <natl/units.h>
#include <natl/printFormatted.h>
#include <natl/typeInfo.h>

template<typename UnitType>
constexpr natl::Bool unitConstructionTest() noexcept {
	UnitType unit1(4);
	UnitType unit2(6);
	return (unit1 + unit2) == UnitType(10);
}

static_assert(unitConstructionTest<natl::Meter<natl::i64>>());
static_assert(unitConstructionTest<natl::Foot<natl::i64>>());
static_assert(unitConstructionTest<natl::Gram<natl::i64>>());


template<typename UnitType>
	requires(natl::IsUnitValue<UnitType> 
	&& natl::IsQuantityOf<UnitType, natl::abbrt::time::negQuaded>)
constexpr void work(const UnitType&) noexcept {}

int main() noexcept {
	auto value = natl::Squared<natl::Nanoseconds<natl::i64>::negSquared>(80);
	work(value);
	natl::printlnf(natl::formatArg<natl::FormatUnitDimension, natl::FormatIntBinFlag>(value));
	natl::printlnf(natl::formatArg<natl::FormatUnitDimension>(value.units()));

	natl::printlnf(natl::getNameOfType<natl::abbrt::time::squared>());
	using namespace natl;
	natl::printlnf(natl::abbrt::Gs::unit_tag::abbreviation_name::toStringView());
	Microseconds<natl::f64>(3).convertTo<decltype(natl::abbr::Gs)>().value();
}