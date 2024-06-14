
//natl
#include <natl/processing/generator.h>
#include <natl/container/string.h>

natl::Generator<natl::Ascii> charExplode(const natl::String& string) {
	for (const natl::Ascii& character : string) {
		natl::Ascii characterStorage = character;
		co_yield characterStorage;
	}
}

int main() {
	for (const natl::Ascii& character : charExplode("3.141592653")) {
		std::cout << character << "  ";
	}
	std::cout << "\n";
}