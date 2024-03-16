
//natl
#include <natl/units.h>

constexpr natl::Meter compileTimeTest() {
	natl::Meter meter(3);
	natl::Meter meter2(6);
	//return meter + meter2;
	return meter + meter2;
}

int main() {
	[[maybe_unused]] constexpr natl::Meter testValue = compileTimeTest();
	natl::Meter meter(3);
	natl::Meter meter2(6);
	meter = meter + meter2;

	natl::MeterValue<natl::f32> meters(10);
	[[maybe_unused]] natl::KilometerValue<natl::f32> Kilometer = meters.convertTo<natl::KilometerUnit<1>>();
}