
//natl
#include <natl/generator.h>
#include <natl/string.h>

natl::Generator<natl::Assci> charExplode(const natl::String& string) {
	for (const natl::Assci& character : string) {
		natl::Assci characterStorage = character;
		co_yield characterStorage;
	}
}

int main() {
	for (const natl::Assci& character : charExplode("3.141592653")) {
		std::cout << character << "  ";
	}
	std::cout << "\n";
}