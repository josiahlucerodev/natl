
//natl
#include <natl/util/test.h>
#include <natl/container/persistentHashSet.h>
#include <natl/util/stringConvert.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlPersistentHashSetTest";

constexpr natl::Size getIterationCount() noexcept {
	if (natl::isConstantEvaluated()) {
		return 10;
	} else {
		return 100;
	}
}

template<typename HashSetType> 
constexpr HashSetType generateTestHashSetInt() noexcept {
	HashSetType map;
	for (natl::Size i : natl::Repeat(getIterationCount())) {
		map.insert(i);
	}
	return map;
}

template<typename HashSetType>
constexpr HashSetType generateTestHashSetString() noexcept {
	HashSetType map;
	for (natl::Size i : natl::Repeat(getIterationCount())) {
		map.insert(natl::intToStringDecimal(i));
	}
	return map;
}

template<typename HashSetType>
constexpr natl::Bool testSetInsertInt(const natl::ConstAsciiStringView& name) noexcept {
	natl::Test test(natlTestFrom, natl::String256(name) + "int insert tests", natl::TestType::leaf);
	{
		auto intset = generateTestHashSetInt<HashSetType>();
		natl::Array<natl::Bool, 100> found;
		found.fill(false);

		for (auto& value : intset) {
			natl::testAssert(test, value < 100, natl::sformat("inserted value {", value, "} is out of test range"));
			if (value >= 100) {
				continue;
			}

			natl::testAssert(test, found[value] != true, natl::sformat("value {", value, "} inserted multiple times"));
			found[value] = true;
		}
	}
	return test;
}

template<typename HashSetType>
constexpr natl::Bool testSetInsertString(const natl::ConstAsciiStringView& name) noexcept {
	natl::Test test(natlTestFrom, natl::String256(name) + "string insert tests", natl::TestType::leaf);
	{
		auto stringset = generateTestHashSetString<HashSetType>();
		natl::Array<natl::Bool, 100> found;
		found.fill(false);

		for (auto& value : stringset) {
			auto intValueExpect = natl::stringDecimalToIntExpect<natl::Size>(value.toStringView());
			natl::testAssert(test, intValueExpect.hasValue(), "string has invalid value");
			if (!intValueExpect.hasValue()) {
				continue;
			}
			auto intValue = intValueExpect.value();

			natl::testAssert(test, intValue < 100, natl::sformat("inserted value {", value, "} is out of test range"));
			if (intValue >= 100) {
				continue;
			}

			natl::testAssert(test, found[intValue] != true, natl::sformat("value {", value, "} inserted multiple times"));
			found[intValue] = true;
		}
	}
	return test;
}

constexpr natl::Bool persistentHashSetSetInsertIntTest() noexcept {
	return testSetInsertInt<natl::PersistentHashSet<natl::Size>>("persistent hash set");
}
constexpr natl::Bool persistentHashSetSetInsertStringTest() noexcept {
	return testSetInsertString<natl::PersistentHashSet<natl::String>>("persistent hash set");
}

constexpr natl::Bool persistentHashSetSetTest() noexcept {
	natl::Test test(natlTestFrom, "persistent hash set", natl::TestType::node);
	natl::subTestAssert(test, persistentHashSetSetInsertIntTest());
	natl::subTestAssert(test, persistentHashSetSetInsertStringTest());
	return test;
}

constexpr natl::Bool smallPersistentHashSetInsertIntTest() noexcept {
	return testSetInsertInt<natl::SmallPersistentHashSet<natl::Size, 50>>("small persistent hash set");
}
constexpr natl::Bool smallPersistentHashSetInsertStringTest() noexcept {
	return testSetInsertString<natl::SmallPersistentHashSet<natl::String, 50>>("small persistent hash set");
}

constexpr natl::Bool smallPersistentHashSetTest() noexcept {
	natl::Test test(natlTestFrom, "small persistent hash set", natl::TestType::node);
	natl::subTestAssert(test, smallPersistentHashSetInsertIntTest());
	natl::subTestAssert(test, smallPersistentHashSetInsertStringTest());
	return test;
}

constexpr natl::Bool formatterTest() noexcept {
	auto testSet = generateTestHashSetInt<natl::PersistentHashSet<natl::Size>>();
	natl::sformat(
		natl::formatArg(testSet,
			natl::formatValue(natl::IntFormat::binary)
		)
	);

	natl::sformat(
		natl::formatArg(testSet,
			natl::formatValue(natl::IntFormat::binary)
		)
	);

	natl::sformat(
		natl::formatArgText<"v: hex">(testSet)
	);

	return true;
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::testAssert(test, formatterTest(), "formmatter test");
	natl::testAssert(test, persistentHashSetSetTest(), "persistent hash map");
	natl::testAssert(test, smallPersistentHashSetTest(), "small persistent hash map");
	return test;
}

int main() {
	tests();
}