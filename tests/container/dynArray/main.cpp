//natl
#include <natl/util/test.h>
#include <natl/container/dynArray.h>
#include <natl/container/smallDynArray.h>
#include <natl/container/batchPool.h>

consteval void constructionTest() {
	natl::DynArray<int> dynArray(4);
	dynArray.resize(5);

	natl::SmallDynArray<int, 8> smallDynArray(3);
}

int main() {
	//constructionTest();
	[[maybe_unused]] natl::DynArray<natl::TestTypeStdOut> dynArray(4);
	dynArray.resize(5);

	[[maybe_unused]] natl::SmallDynArray<int, 8> smallDynArray(3);

	[[maybe_unused]] constexpr natl::Bool test = natl::IsTriviallyDestructible<natl::DynArray<int>>;

	[[maybe_unused]]  natl::RecycleMonotonicBatchPool<int> batchPool;
	batchPool.allocateConstructed();
	batchPool.allocateConstructed();
	batchPool.allocateConstructed();
}