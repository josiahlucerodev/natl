
//natl
#include <natl/array.h>


constexpr int compileTimeTest() {
	natl::Array<int, 3> array = {2, 3, 4};
	array = { 2, 3, 7 };
	return array.at(2);
}



int main() {
	constexpr int number = compileTimeTest();
}