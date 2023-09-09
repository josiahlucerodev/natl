
//natl
#include <natl/dynamicArray.h>

constexpr int compileTimeTest() {
	natl::DynamicArray<int> array{};

	array.pushBack(1);
	array.pushBack(2);
	array.pushBack(3);
	array.pushBack(4);
	array.pushBack(5);
	array.pushBack(6);

	return array.at(5);
}

NATL_STATIC_ALLOCATOR_INSTANCE(natl::StaticAllocatorWithCounters, TestStaticAllocator);
int testStaticAllocator() {
	natl::DynamicArray<int, TestStaticAllocator> array{};

	array.pushBack(1);
	array.pushBack(2);
	array.pushBack(3);
	array.pushBack(4);
	array.pushBack(5);
	array.pushBack(6);

	return array.at(5);
}


int main() {
	constexpr int number = compileTimeTest();
	testStaticAllocator();
}