
//natl
#include <natl/print.h>
#include <natl/format.h>
#include <natl/string.h>

constexpr natl::String256 outputTest() noexcept {
	using type = natl::FormatFloatPrecisionType<3>;
	const natl::FormatColumn formatColumn(20);
	const natl::f64 floatValue = 12.345678;
	const natl::ui64 intValue = 10;
	return natl::format<natl::String256>(
		"int binary:", formatColumn, natl::formatArgText<"binary">(intValue), natl::FormatNewLine{},
		"int decimal:", formatColumn, natl::formatArgText<"decimal">(intValue), natl::FormatNewLine{},
		"int hexadecimal:", formatColumn, natl::formatArgText<"hexadecimal">(intValue), natl::FormatNewLine{},
		"float p2:", formatColumn, natl::formatArgText<"standard", "p: 2">(floatValue), natl::FormatNewLine{},
		"float p4:", formatColumn, natl::formatArgText<"standard">(floatValue, natl::FormatFloatPrecision(4)), natl::FormatNewLine{},
		"bool FULL:", formatColumn, natl::formatArgText<"FULL">(true), natl::FormatNewLine{}
	);
}

int main() {
	constexpr natl::String256 output = outputTest();
	natl::print(output.data(), output.size());
	const natl::Size size = natl::cstringLength(output.data());
	return size;
}