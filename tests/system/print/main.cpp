
//natl
#include <natl/system/print.h>
#include <natl/system/printFormatted.h>
#include <natl/system/printColor.h>


int main() {
	natl::enablePrintExtendedColor();

	natl::println(natl::getTypeInfo<natl::Size>().name());
	natl::printlnf(false);

	natl::printf(natl::formatArgText<"b">(natl::PrintColor::red), true, natl::PrintAllDefaultColor{});

	natl::printlnf(false);

	natl::printlnc("the", 3, natl::PrintBackgroundColor::red);
	natl::printlnc(natl::getTypeInfo<natl::Size>().name(), natl::PrintBackgroundColor::red);

	natl::printlnfc(natl::PrintExtendedForegroundColor(128, 128, 0), "the");


	//natl::printf(natl::PrintExtendedBackgroundColor(0, 0, 0));
	//natl::Ascii inputBuffer[256] = {0};
	//natl::input(inputBuffer, 256);
	//natl::println(inputBuffer);
}