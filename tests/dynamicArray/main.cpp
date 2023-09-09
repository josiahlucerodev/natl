
//natl
#include <natl/dynamicArray.h>
#include <natl/partition.h>

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
	natl::DynamicArray<int, TestStaticAllocator> array{2, 3};

	array.pushBack(1);
	array.pushBack(2);
	array.pushBack(3);
	array.pushBack(4);
	array.pushBack(5);
	array.pushBack(6);

	array = { 3, 4, 5, 6, 7, 8, 9, 10 };

	natl::Partition<int> partition = natl::makePartition<decltype(array)>(array);

	array.fill(2);

	return array.at(5);
}


int main() {
	constexpr int number = compileTimeTest();
	testStaticAllocator();
}