
//natl
#include <natl/print.h>
#include <natl/format.h>
#include <natl/string.h>

constexpr natl::String128 outputTest() noexcept {
	const natl::FormatColumn formatColumn(15);
	return natl::format<natl::String128>(
		"binary:", formatColumn, natl::formatArgText<"binary">(natl::ui64(3)), natl::FormatNewLine{},
		"decimal:", formatColumn, natl::formatArgText<"decimal">(natl::ui64(3)), natl::FormatNewLine{},
		"hexadecimal:", formatColumn, natl::formatArgText<"hexadecimal">(natl::ui64(3)), natl::FormatNewLine{}
	);
}

int main() {
	constexpr natl::String128 output = outputTest();
	natl::print(output.data(), output.size());
	const natl::Size size = natl::cstringLength(output.data());
	return size;
}