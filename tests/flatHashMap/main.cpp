
//natl
#include <natl/util/test.h>
#include <natl/container/flatHashMap.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlFlatHashMapTest";

constexpr natl::FlatHashMap<natl::Size, natl::Size> getTestHashMap() noexcept {
	natl::FlatHashMap<natl::Size, natl::Size> map;
	map.reserve(8);
	map.insert(1, 1);
	map.insert(2, 2);
	map.insert(3, 3);
	map.insert(4, 4);
	map.insert(5, 5);
	map.insert(6, 6);
	map.insert(7, 7);
	map.insert(8, 8);
	return map;
}

constexpr natl::Bool testIteration() noexcept {
	natl::FlatHashMap<natl::Size, natl::Size> testMap = getTestHashMap();
	for (auto&& [key, value] : testMap) {
		if (key == 4 && value == 4) {
			return true;
		}
	}
	return false;
}

constexpr natl::Bool testConstIteration() noexcept {
	natl::FlatHashMap<natl::Size, natl::Size> testMap = getTestHashMap();
	for (const auto& [key, value] : natl::toConst(testMap)) {
		if (key == 4 && value == 4) {
			return true;
		}
	}
	return false;
}

constexpr natl::Bool testErase() noexcept {
	auto testMap = getTestHashMap();
	testMap.erase(1);
	return testMap.find(1).doesNotHaveValue();
}

constexpr natl::Bool testFind() noexcept {
	auto testMap = getTestHashMap();
	return testMap.find(4).value()->value;
}

constexpr natl::Bool testFormatter() noexcept {
	auto testMap = getTestHashMap();
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

static_assert(testIteration());
static_assert(testConstIteration());
static_assert(testFormatter());
static_assert(testErase());
static_assert(testFind());

natl::Bool tests() noexcept {
	natl::TestTimer timer(natlTestFrom);
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::testAssert(test, testIteration(), "iteration");
	natl::testAssert(test, testConstIteration(), "const iteration");
	natl::testAssert(test, testFormatter(), "formatter");
	natl::testAssert(test, testErase(), "erase");
	natl::testAssert(test, testFind(), "find");
	return test;
}

int main() {
	tests();
}