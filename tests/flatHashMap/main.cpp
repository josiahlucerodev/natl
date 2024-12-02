
//natl
#include <natl/util/test.h>
#include <natl/container/flatHashMap.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlFlatHashMapTest";

constexpr natl::Size getIterationCount() noexcept {
	if (natl::isConstantEvaluated()) {
		return 10;
	} else {
		return 100;
	}
}

template<typename HashMapType> 
constexpr HashMapType generateTestHashMapIntPair() noexcept {
	HashMapType map;
	for (natl::Size i : natl::Repeat(getIterationCount())) {
		map.insert(i, i);
	}
	return map;
}

template<typename HashMapType>
constexpr HashMapType generateTestHashMapStringPair() noexcept {
	HashMapType map;
	for (natl::Size i : natl::Repeat(getIterationCount())) {
		map.insert(natl::intToStringDecimal(i), natl::intToStringDecimal(i));
	}
	return map;
}

template<typename HashMapType> 
constexpr natl::Bool testMapInsertIntPair(const natl::ConstAsciiStringView& name) noexcept {
	natl::Test test(natlTestFrom, natl::String256(name) + "int pair insert tests", natl::TestType::leaf);
	{
		auto intmap = generateTestHashMapIntPair<HashMapType>();
		natl::Size index = 0;
		for (auto&& [key, value] : intmap) {
			natl::testAssertEquals(test, key, value, "test int key and int value equal");
			index++;
		}
		natl::testAssertEquals(test, getIterationCount(), index, "all int key and int value equal tested");
	}
	return test;
}

template<typename HashMapType>
constexpr natl::Bool testMapInsertStringPair(const natl::ConstAsciiStringView& name) noexcept {
	natl::Test test(natlTestFrom, natl::String256(name) + "string pair insert tests", natl::TestType::leaf);
	{
		auto stringmap = generateTestHashMapStringPair<HashMapType>();
		natl::Size index = 0;
		for (auto&& [key, value] : stringmap) {
			natl::testAssertEquals(test, key, value, "test string key and string value equal");
			index++;
		}
		natl::testAssertEquals(test, getIterationCount(), index, "all string key and string value equal tested");
	}
	return test;
}

constexpr natl::Bool flatHashMapInsertIntPairTest() noexcept {
	return testMapInsertIntPair<natl::FlatHashMap<natl::Size, natl::Size>>("flat hash map");
}
constexpr natl::Bool flatHashMapInsertStringPairTest() noexcept {
	return testMapInsertStringPair<natl::FlatHashMap<natl::String, natl::String>>("flat hash map");
}

constexpr natl::Bool flatHashMapTest() noexcept {
	natl::Test test(natlTestFrom, "flat hash map", natl::TestType::node);
	natl::subTestAssert(test, flatHashMapInsertIntPairTest());
	natl::subTestAssert(test, flatHashMapInsertStringPairTest());
	return test;
}

constexpr natl::Bool smallFlatHashMapInsertIntPairTest() noexcept {
	return testMapInsertIntPair<natl::SmallFlatHashMap<natl::Size, natl::Size, 50>>("small flat hash map");
}
constexpr natl::Bool smallFlatHashMapInsertStringPairTest() noexcept {
	return testMapInsertStringPair<natl::SmallFlatHashMap<natl::String, natl::String, 50>>("small flat hash map");
}

constexpr natl::Bool smallFlatHashMapTest() noexcept {
	natl::Test test(natlTestFrom, "small flat hash map", natl::TestType::node);
	natl::subTestAssert(test, smallFlatHashMapInsertIntPairTest());
	natl::subTestAssert(test, smallFlatHashMapInsertStringPairTest());
	return test;
}

constexpr natl::Bool formatterTest() noexcept {
	auto testMap = generateTestHashMapIntPair<natl::FlatHashMap<natl::Size, natl::Size>>();
	natl::sformat(
		natl::formatArg(testMap,
			natl::formatKey(natl::IntFormat::hexadecimal)
		)
	);

	natl::sformat(
		natl::formatArg(testMap,
			natl::formatKey(natl::IntFormat::hexadecimal),
			natl::formatValue(natl::IntFormat::binary)
		)
	);

	natl::sformat(
		natl::formatArgText<"v: hex", "k: hex">(testMap)
	);

	return true;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::testAssert(test, formatterTest(), "formmatter test");
	natl::testAssert(test, flatHashMapTest(), "flat hash map");
	natl::testAssert(test, smallFlatHashMapTest(), "small flat hash map");
	return test;
}

int main() {
	tests();
}