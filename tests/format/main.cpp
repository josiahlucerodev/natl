
//natl
#include <natl/print.h>
#include <natl/format.h>
#include <natl/string.h>
#include <natl/iteration.h>

constexpr natl::String128 outputTest() noexcept {
	natl::String128 output;
	natl::BackInsertIterator<natl::String128> ouputIter = natl::backInserter(output);
	natl::FormatColumn formatColumn(15);
	natl::formatTo(ouputIter,
		"binary:", formatColumn, natl::formatArgText<"binary">(natl::ui64(3)), natl::FormatNewLine{},
		"decimal:", formatColumn, natl::formatArgText<"decimal">(natl::ui64(3)), natl::FormatNewLine{},
		"hexadecimal:", formatColumn, natl::formatArgText<"hexadecimal">(natl::ui64(3)), natl::FormatNewLine{}
	);
	return output;
}

int main() {
	constexpr natl::String128 output = outputTest();
	natl::print(output.data(), output.size());
	const natl::Size size = natl::cstringLength(output.data());
	return size;
}