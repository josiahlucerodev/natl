
//natl
#include <natl/heapArray.h>

constexpr natl::i64 compileTimeTest() {
	natl::HeapArray<natl::i64> intArray(100, 5);
	int value = intArray.at(10);
	intArray.deallocate();
	return value;
}

int main() {
	[[maybe_unused]] constexpr natl::i64 value = compileTimeTest();
}