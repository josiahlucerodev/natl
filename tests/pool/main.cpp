
//natl
#include <natl/batchPool.h>
#include <natl/dynamicArray.h>
#include <natl/repeat.h>
#include <natl/test.h>


bool standardBatchPoolTest(int) {
	using TestType = std::uint32_t;
	
	natl::BatchPool<TestType> batchPool;
	natl::DynamicArray<TestType*> elementPtrs;

	for (const int index : natl::Repeat(1000)) {
		elementPtrs.pushBack(batchPool.newElement());
	}
	for (const int index : natl::Repeat(1000)) {
		*elementPtrs[index] = index;
	}


	return true;
}

int main() {
	natl::testStart("batch pool Test");
	natl::runTest(standardBatchPoolTest, 1, "standard batch pool test");
}