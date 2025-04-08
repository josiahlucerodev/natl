
//natl
#include <natl/util/test.h>
#include <natl/container/persistentHashMap.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlPersistentHashMapTest";

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
constexpr natl::Bool mapInsertIntPair(const natl::ConstAsciiStringView& name) noexcept {
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
constexpr natl::Bool mapInsertStringPair(const natl::ConstAsciiStringView& name) noexcept {
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

constexpr natl::Bool persistentHashMapInsertIntPairTest() noexcept {
	return mapInsertIntPair<natl::PersistentHashMap<natl::Size, natl::Size>>("persistent hash map");
}
constexpr natl::Bool persistentHashMapInsertStringPairTest() noexcept {
	return mapInsertStringPair<natl::PersistentHashMap<natl::String, natl::String>>("persistent hash map");
}

constexpr natl::Bool persistentHashMapTest() noexcept {
	natl::Test test(natlTestFrom, "persistent hash map", natl::TestType::node);
	natl::subTestAssert(test, persistentHashMapInsertIntPairTest());
	natl::subTestAssert(test, persistentHashMapInsertStringPairTest());
	return test;
}

constexpr natl::Bool smallPersistentHashMapInsertIntPairTest() noexcept {
	return mapInsertIntPair<natl::SmallPersistentHashMap<natl::Size, natl::Size, 50>>("small persistent hash map");
}
constexpr natl::Bool smallPersistentHashMapInsertStringPairTest() noexcept {
	return mapInsertStringPair<natl::SmallPersistentHashMap<natl::String, natl::String, 50>>("small persistent hash map");
}

constexpr natl::Bool smallPersistentHashMapTest() noexcept {
	natl::Test test(natlTestFrom, "small persistent hash map", natl::TestType::node);
	natl::subTestAssert(test, smallPersistentHashMapInsertIntPairTest());
	natl::subTestAssert(test, smallPersistentHashMapInsertStringPairTest());
	return test;
}

constexpr natl::Bool formatterTest() noexcept {
	auto map = generateTestHashMapIntPair<natl::PersistentHashMap<natl::Size, natl::Size>>();
	natl::sformat(
		natl::formatArg(map,
			natl::formatKey(natl::IntFormat::hexadecimal)
		)
	);

	natl::sformat(
		natl::formatArg(map,
			natl::formatKey(natl::IntFormat::hexadecimal),
			natl::formatValue(natl::IntFormat::binary)
		)
	);

	natl::sformat(
		natl::formatArgText<"v: hex", "k: hex">(map)
	);

	return true;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::testAssert(test, formatterTest(), "formmatter test");
	natl::testAssert(test, persistentHashMapTest(), "hash map");
	natl::testAssert(test, smallPersistentHashMapTest(), "small hash map");
	return test;
}

int main() {
	tests();
}