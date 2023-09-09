
//natl
#include <natl/flatHashMap.h>
#include <natl/repeat.h>
#include <natl/option.h>


NATL_STATIC_ALLOCATOR_INSTANCE(natl::StaticAllocatorWithCounters, TestStaticAllocator);

using testAllocator = natl::Allocator;
constexpr int compileTimeTest() {
	natl::FlatHashMap<int, int, testAllocator> map{};

	for (const std::size_t index : natl::Repeat(1)) {
		map.insert(index, index);
	}

	map.find(0);

	return map.find(0).value()->data;
}


constexpr int testFlatHashMap() {
	natl::FlatHashMap<int, int, testAllocator> map{};
	map.insert(1, 1);
	map.insert(2, 2);
	map.insert(3, 3);
	map.insert(3, compileTimeTest());
	map.insert(3, compileTimeTest());

	return map.find(2).value()->data;
}


int main() {
	constexpr int number = compileTimeTest();
	constexpr int number2 = testFlatHashMap();
}