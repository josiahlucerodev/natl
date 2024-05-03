
//natl
#include <natl/units.h>

template<typename UnitType>
constexpr natl::Bool unitConstructionTest() noexcept {
	UnitType unit1(4);
	UnitType unit2(6);
	return (unit1 + unit2) == UnitType(10);
}

static_assert(unitConstructionTest<natl::Meter<natl::i64>>());
static_assert(unitConstructionTest<natl::Feet<natl::i64>>());
static_assert(unitConstructionTest<natl::Gram<natl::i64>>());

int main() noexcept {}