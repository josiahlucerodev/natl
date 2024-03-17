
//natl
#include <natl/generator.h>
#include <natl/string.h>

natl::Generator<natl::AssciCode> charExplode(const natl::String& string) {
	for (const natl::AssciCode& character : string) {
		natl::AssciCode characterStorage = character;
		co_yield characterStorage;
	}
}

int main() {
	for (const natl::AssciCode& character : charExplode("3.141592653")) {
		std::cout << character << "  ";
	}
	std::cout << "\n";
}