
//natl
#include <natl/string.h>

//std
#include <iostream>

constexpr int compileTimeTest() {
	natl::String the = "the";
	natl::String the2 = "the";
	natl::String the3 = the + the2;
	return 3;
}

int main() {
	//constexpr 
	constexpr int number = compileTimeTest();

	natl::String the = "the";
	natl::String the2 = "the";
	std::cout << (the + the2).cStr();
}