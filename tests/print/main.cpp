
//natl
#include <natl/print.h>
#include <natl/format.h>
#include <natl/string.h>
#include <natl/stringLiteral.h>
#include <natl/iteration.h>

int main() {
	natl::String output;
	natl::BackInsertIterator<natl::String> ouputIter = natl::backInserter(output);
	natl::formatTo(ouputIter, natl::formatArgText<"hexadecimal">(natl::ui64(3)));
	natl::println(output.c_str());


	natl::Ascii inputBuffer[256] = {0};
	natl::input(inputBuffer, 256);

	natl::println(inputBuffer);
}