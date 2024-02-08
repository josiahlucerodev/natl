//natl
#include <natl/test.h>
#include <natl/dynArray.h>
#include <natl/smallDynArray.h>
#include <natl/batchPool.h>

consteval void constructionTest() {
	natl::DynArray<int> dynArray(4);
	dynArray.resize(5);

	natl::SmallDynArray<int, 8> smallDynArray(3);
}

int main() {
	//constructionTest();
	natl::DynArray<natl::TestTypeStdOut> dynArray(4);
	dynArray.resize(5);

	natl::SmallDynArray<int, 8> smallDynArray(3);

	constexpr bool test = natl::IsTriviallyDestructible<natl::DynArray<int>>;

	natl::BatchPool<int> batchPool;
	batchPool.newElement();
	batchPool.newElement();
	batchPool.newElement();
}