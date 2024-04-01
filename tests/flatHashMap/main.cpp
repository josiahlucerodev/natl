
//natl
#include <natl/flatHashMap.h>
#include <natl/iteration.h>
#include <natl/option.h>
#include <natl/string.h>


constexpr natl::Size compileTimeTest() {
	natl::FlatHashMap<natl::Size, natl::Size> map{};

	for (const natl::Size index : natl::Repeat(1)) {
		map.insert(index, index);
	}

	map.find(0);

	return map.find(0).value()->data;
}


constexpr natl::Size testFlatHashMap() {

	natl::FlatHashMap<natl::Size, natl::Size> map{};
	map.insert(1, 1);
	map.insert(2, 2);
	map.insert(3, 3);
	map.insert(3, compileTimeTest());
	map.insert(3, compileTimeTest());

	return map.find(2).value()->data;
}

 constexpr natl::Size testStringMap() {
	natl::FlatHashMap<natl::String, natl::Size> map{};
	map.insert("1", 1);

	char character = 'a';
	for (const natl::Size index : natl::Repeat(10)) {
		map.insert(natl::String(1, char(static_cast<natl::Size>(character) + index)), index);
	}

	natl::SmallFlatHashMap<natl::Size, natl::Size, 8> smallMap{};
	smallMap.insert(1, 1);
	smallMap.insert(2, 2);
	smallMap.insert(3, 3);
	smallMap.insert(4, 4);
	smallMap.insert(5, 5);
	smallMap.insert(6, 6);
	//map.insert("2", 2);
	//map.insert("3", 3);
	//map.insert("3", compileTimeTest());
	//map.insert("3", compileTimeTest());
	//
	//return map.find("1").value()->data;
	return 0;
}

int main() {
	[[maybe_unused]] constexpr natl::Size number = compileTimeTest();
	[[maybe_unused]] constexpr natl::Size number2 = testFlatHashMap();
	//constexpr int test = natl::String().();
	[[maybe_unused]] constexpr natl::Size number3 = testStringMap();
	[[maybe_unused]] natl::SmallFlatHashMap<natl::Size, natl::Size, 8> map{};
	map.insert(1, 1);
	map.insert(2, 2);
	map.insert(3, 3);
	map.insert(4, 4);
	[[maybe_unused]] natl::Size number4 = map.find(1).value()->data;
}