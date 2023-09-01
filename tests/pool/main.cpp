
//natl
#include <natl/batchPool.h>
#include <natl/repeat.h>
#include <natl/test.h>
#include <natl/container.h>


bool standardBatchPoolTest(int) {
	using TestType = std::uint32_t;
	
	natl::BatchPool<TestType> batchPool;
	std::vector<TestType*> elementPtrs;
	

	for (const int index : natl::Repeat(1000)) {
		elementPtrs.push_back(batchPool.newElement());
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