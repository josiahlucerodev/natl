
//natl
#include <natl/util/test.h>
#include <natl/container/flatHashSet.h>
#include <natl/util/stringConvert.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlFlatHashSetTest";

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

constexpr natl::Bool flatHashSetInsertIntTest() noexcept {
	return testSetInsertInt<natl::FlatHashSet<natl::Size>>("flat hash set");
}
constexpr natl::Bool flatHashSetInsertStringTest() noexcept {
	return testSetInsertString<natl::FlatHashSet<natl::String>>("flat hash set");
}

constexpr natl::Bool flatHashSetTest() noexcept {
	natl::Test test(natlTestFrom, "flat hash set", natl::TestType::node);
	natl::subTestAssert(test, flatHashSetInsertIntTest());
	natl::subTestAssert(test, flatHashSetInsertStringTest());
	return test;
}

constexpr natl::Bool smallFlatHashSetInsertIntTest() noexcept {
	return testSetInsertInt<natl::SmallFlatHashSet<natl::Size, 50>>("small flat hash set");
}
constexpr natl::Bool smallFlatHashSetInsertStringTest() noexcept {
	return testSetInsertString<natl::SmallFlatHashSet<natl::String, 50>>("small flat hash set");
}

constexpr natl::Bool smallFlatHashSetTest() noexcept {
	natl::Test test(natlTestFrom, "small flat hash set", natl::TestType::node);
	natl::subTestAssert(test, smallFlatHashSetInsertIntTest());
	natl::subTestAssert(test, smallFlatHashSetInsertStringTest());
	return test;
}

constexpr natl::Bool formatterTest() noexcept {
	auto testSet = generateTestHashSetInt<natl::FlatHashSet<natl::Size>>();
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
	natl::testAssert(test, flatHashSetTest(), "flat hash map");
	natl::testAssert(test, smallFlatHashSetTest(), "small flat hash map");
	return test;
}

int main() {
	tests();
}