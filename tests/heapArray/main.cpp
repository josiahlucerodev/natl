
//natl
#include <natl/heapArray.h>
#include <natl/test.h>

constexpr natl::i64 compileTimeTest() {
	natl::HeapArray<natl::i64> intArray(100, 5);
	natl::i64 value = intArray.at(10);
	return value;
}

int main() {
	[[maybe_unused]] constexpr natl::i64 value = compileTimeTest();
	[[maybe_unused]] natl::HeapArray<natl::TestTypeStdOut> heapArray(2);
	[[maybe_unused]] natl::HeapArray<natl::TestTypeStdOut> heapArray2 = natl::move(heapArray);
}