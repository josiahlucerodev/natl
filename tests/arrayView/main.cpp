
#include <natl/arrayView.h>
#include <natl/test.h>
#include <natl/container.h>


bool partitionAtTest(int) {
	using TestType = std::uint32_t;
	natl::ArrayView<const TestType> constPartition = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<TestType> numbersVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	natl::ArrayView<TestType> arrayview = natl::makeArrayView<std::vector<TestType>>(numbersVector);

	for (std::size_t i = 0; i < constPartition.size(); i++) {
		//natl::testVaildateValue<const TestType>(constPartition.at(i), i + 1, "ArrayView const Type at(i)");
		//natl::testVaildateValue<TestType>(arrayview.at(i), i + 1, "ArrayView Type at(i)");
	}

	return true;
}

bool partitionCampedTest(int) {
	using TestType = std::uint32_t;
	natl::ArrayView<const TestType> constPartition = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<TestType> numbersVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	natl::ArrayView<TestType> arrayview = natl::makeArrayView<std::vector<TestType>>(numbersVector);

	for (std::size_t i = constPartition.size(); i < constPartition.size() * 2; i++) {
		//natl::testVaildateValue<const TestType&>(constPartition.atClamped(i), constPartition.back(), "ArrayView const Type atClamped(i)");
		//natl::testVaildateValue<const TestType*>(&constPartition.atClamped(i), &constPartition.back(), "ArrayView const Type atClamped(i)");
		//natl::testVaildateValue<TestType&>(arrayview.atClamped(i), arrayview.back(), "ArrayView Type atClamped(i)");
		//natl::testVaildateValue<TestType*>(&arrayview.atClamped(i), &arrayview.back(), "ArrayView Type atClamped(i)");
	}

	return true;
}

bool partitionIterators(int) {
	using TestType = std::uint32_t;
	natl::ArrayView<const TestType> constPartition = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<TestType> numbersVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	natl::ArrayView<TestType> arrayview = natl::makeArrayView<std::vector<TestType>>(numbersVector);

	std::uint32_t index = 0;
	for (const TestType& value : constPartition) {
		//natl::testVaildateValue<const TestType>(value, index + 1, "arrayview const Type const iterators");
		index++;
	}

	index = 0;
	for (const TestType& value : arrayview) {
		//natl::testVaildateValue<const TestType>(value, index + 1, "arrayview Type const iterators");
		index++;
	}

	index = 0;
	for (TestType& value : arrayview) {
		//natl::testVaildateValue<const TestType>(value, index + 1, "arrayview Type iterators");
		index++;
	}

	return true;
}

int main() {
	constexpr static bool value = natl::IsTriviallyRelocatable<natl::ui16>;
	if (!value) {
		std::cout << "hello world!";
	}
	//static_assert(std::is_trivial<natl::ArrayView<int>>::value, "natl::ArrayView Type not trivial");
	//static_assert(std::is_trivial<natl::ArrayView<const int>>::value, "natl::ArrayView const Type not trivial");

	//natl::testStart("ArrayView Test");
	//natl::runTest(partitionAtTest, 1, "arrayview at");
	//natl::runTest(partitionCampedTest, 1, "arrayview at clamped");
	//natl::runTest(partitionIterators, 1, "arrayview iterators");
}