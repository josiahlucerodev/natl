
//natl
#include <natl/util/test.h>
#include <natl/container/batchPool.h>
#include <natl/container/string.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NathBatchPoolTest";

constexpr natl::Size getTestCount() noexcept {
	if (natl::isConstantEvaluated()) {
		return 10;
	} else {
		return 1000;
	}
}

template<typename BatchPoolType, natl::Bool dealloc>
constexpr natl::Bool genericBatchPoolTest(natl::ConstAsciiStringView name) noexcept {
	natl::Test test(natlTestFrom, name, natl::TestType::leaf);
	BatchPoolType batchPool;
	batchPool.reserve(100);
	
	constexpr natl::ConstAsciiStringView testString = "TestString";
	natl::String* value1 = batchPool.allocateConstructed();
	*value1 = testString;

	natl::testAssertEquals(test, testString, *value1, "value 1 construction");
	std::vector<natl::String*> allocatedStrings;
	for ([[maybe_unused]] natl::Size index : natl::Repeat(getTestCount())) {
		natl::String* generatedValue = batchPool.allocateConstructed();
		allocatedStrings.push_back(generatedValue);
		*generatedValue = testString;
		natl::testAssertEquals(test, testString, *generatedValue, "generated value construction");
	}

	if constexpr (dealloc) {
		for (natl::String* generatedValue : allocatedStrings) {
			batchPool.deallocate(generatedValue);
		}
		natl::testAssertEquals(test, testString, *value1, "value 1 persistence");
		batchPool.deallocate(value1);
	}

	return test;
}

constexpr natl::Bool recycleMonotonicBatchPoolTest() noexcept {
	return genericBatchPoolTest<natl::MonotonicBatchPool<natl::String, 10>, false>("MonotonicBatchPool");
}
constexpr natl::Bool monotonicBatchPoolTest() noexcept {
	return genericBatchPoolTest<natl::RecycleMonotonicBatchPool<natl::String, 10>, true>("RecycleMonotonicBatchPool");
}
constexpr natl::Bool batchPoolTest() noexcept {
	return genericBatchPoolTest<natl::BatchPool<natl::String, 10>, true>("BatchPool");
}

natl::Bool tests() noexcept {
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	natl::subTestAssert(test, recycleMonotonicBatchPoolTest());
	natl::subTestAssert(test, monotonicBatchPoolTest());
	natl::subTestAssert(test, batchPoolTest());
	return test;
}

int main() {
	tests();
}