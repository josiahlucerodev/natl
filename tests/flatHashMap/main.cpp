
//natl
#include <natl/flatHashMap.h>
#include <natl/repeat.h>
#include <natl/option.h>
#include <natl/string.h>


constexpr int compileTimeTest() {
	natl::FlatHashMap<int, int> map{};

	for (const std::size_t index : natl::Repeat(1)) {
		map.insert(index, index);
	}

	map.find(0);

	return map.find(0).value()->data;
}


constexpr int testFlatHashMap() {

	natl::FlatHashMap<int, int> map{};
	map.insert(1, 1);
	map.insert(2, 2);
	map.insert(3, 3);
	map.insert(3, compileTimeTest());
	map.insert(3, compileTimeTest());

	return map.find(2).value()->data;
}

constexpr int testStringMap() {
	natl::FlatHashMap<natl::String, int> map{};
	map.insert("1", 1);
	map.insert("2", 2);
	map.insert("3", 3);
	map.insert("3", compileTimeTest());
	map.insert("3", compileTimeTest());

	return map.find("1").value()->data;
}

int main() {
	constexpr int number = compileTimeTest();
	constexpr int number2 = testFlatHashMap();
	constexpr int number3 = testStringMap();
}