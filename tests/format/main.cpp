
//natl
#include <natl/print.h>
#include <natl/format.h>
#include <natl/string.h>

//boolean format
static_assert(natl::sFormat(true) == "true");
static_assert(natl::sFormat(false) == "false");

static_assert(natl::sFormat(natl::formatArgText<"Full">(true)) == "True");
static_assert(natl::sFormat(natl::formatArgText<"Full">(false)) == "False");

static_assert(natl::sFormat(natl::formatArgText<"FULL">(true)) == "TRUE");
static_assert(natl::sFormat(natl::formatArgText<"FULL">(false)) == "FALSE");

static_assert(natl::sFormat(natl::formatArgText<"shorthand">(true)) == "t");
static_assert(natl::sFormat(natl::formatArgText<"shorthand">(false)) == "f");

static_assert(natl::sFormat(natl::formatArgText<"Shorthand">(true)) == "T");
static_assert(natl::sFormat(natl::formatArgText<"Shorthand">(false)) == "F");

//string literal format
static_assert(natl::sFormat("Hello") == "Hello");

//string view
static_assert(natl::sFormat(natl::AsciiStringView("Hello")) == "Hello");
static_assert(natl::sFormat(natl::ConstAsciiStringView("Hello")) == "Hello");

//string 
static_assert(natl::sFormat(natl::String("Hello")) == "Hello");


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