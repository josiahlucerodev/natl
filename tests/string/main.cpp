
//natl
#include <natl/string.h>

//std
#include <iostream>

constexpr int compileTimeTest() {
	natl::String the = "the";
	natl::String the2 = "the";
	natl::String the3 = the + the2;
	the3.resize(9);
	return the3.at(8);
}

int main() {
	constexpr int number = compileTimeTest();

	natl::String the = "the";
	natl::String the2 = "the";
	std::cout << (the + the2).cStr();
}