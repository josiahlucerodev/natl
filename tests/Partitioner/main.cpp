#ifdef NATL_MODULES
import natl.timer;
import natl.partitioner;
#else 

#include <natl/pch.h>
#include <natl/partitioner.h>
#include <natl/test.h>
#include <natl/container.h>
#endif //NATL_MODULES


bool partitionAtTest(int) {
	using TestType = std::uint32_t;
	natl::Partition<const TestType> constPartition = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<TestType> numbersVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	natl::Partition<TestType> partition = natl::newPartitionFromContainer<std::vector<TestType>>(numbersVector);

	for (std::size_t i = 0; i < constPartition.size(); i++) {
		natl::testVaildateValue<const TestType>(constPartition.at(i), i + 1, "Partition const Type at(i)");
		natl::testVaildateValue<TestType>(partition.at(i), i + 1, "Partition Type at(i)");
	}

	return true;
}

bool partitionCampedTest(int) {
	using TestType = std::uint32_t;
	natl::Partition<const TestType> constPartition = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<TestType> numbersVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	natl::Partition<TestType> partition = natl::newPartitionFromContainer<std::vector<TestType>>(numbersVector);

	for (std::size_t i = constPartition.size(); i < constPartition.size() * 2; i++) {
		natl::testVaildateValue<const TestType&>(constPartition.atClamped(i), constPartition.back(), "Partition const Type atClamped(i)");
		natl::testVaildateValue<const TestType*>(&constPartition.atClamped(i), &constPartition.back(), "Partition const Type atClamped(i)");
		natl::testVaildateValue<TestType&>(partition.atClamped(i), partition.back(), "Partition Type atClamped(i)");
		natl::testVaildateValue<TestType*>(&partition.atClamped(i), &partition.back(), "Partition Type atClamped(i)");
	}

	return true;
}

bool partitionIterators(int) {
	using TestType = std::uint32_t;
	natl::Partition<const TestType> constPartition = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<TestType> numbersVector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	natl::Partition<TestType> partition = natl::newPartitionFromContainer<std::vector<TestType>>(numbersVector);

	std::uint32_t index = 0;
	for (const TestType& value : constPartition) {
		natl::testVaildateValue<const TestType>(value, index + 1, "partition const Type const iterators");
		index++;
	}

	index = 0;
	for (const TestType& value : partition) {
		natl::testVaildateValue<const TestType>(value, index + 1, "partition Type const iterators");
		index++;
	}

	index = 0;
	for (TestType& value : partition) {
		natl::testVaildateValue<const TestType>(value, index + 1, "partition Type iterators");
		index++;
	}

	return true;
}

int main() {
	static_assert(std::is_trivial<natl::Partition<int>>::value, "natl::Partition Type not trivial");
	static_assert(std::is_trivial<natl::Partition<const int>>::value, "natl::Partition const Type not trivial");

	natl::testStart("Partition Test");
	natl::runTest(partitionAtTest, 1, "partition at");
	natl::runTest(partitionCampedTest, 1, "partition at clamped");
	natl::runTest(partitionIterators, 1, "partition iterators");
}