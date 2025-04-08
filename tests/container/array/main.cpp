
//natl
#include <natl/container/array.h>

constexpr int compileTimeTest() {
	natl::Array<int, 4> array = {2, 3, 4, 5};
	natl::Array<int, 4> array2 = {2, 3, 4, 5};

	array = array2;

	array = { 2, 3, 7 };
	array.fill(3);
	return array.at(2);
}

int main() {
	constexpr int number = compileTimeTest();
	[[maybe_unused]] natl::MDArray<int, number, 4> array;
}