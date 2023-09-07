
//natl
#include <natl/dynamicArray.h>

constexpr int compileTimeTest() {
	natl::DynamicArray<int> array{};

	array.push_back(1);
	array.push_back(2);
	array.push_back(3);
	array.push_back(4);
	array.push_back(5);
	array.push_back(6);

	return array.at(5);
}

NATL_STATIC_ALLOCATOR_INSTANCE(natl::StaticAllocatorWithCounters, TestStaticAllocator);
int testStaticAllocator() {
	natl::DynamicArray<int, TestStaticAllocator> array{};

	array.push_back(1);
	array.push_back(2);
	array.push_back(3);
	array.push_back(4);
	array.push_back(5);
	array.push_back(6);

	return array.at(5);
}


int main() {
	constexpr int number = compileTimeTest();
	testStaticAllocator();
}