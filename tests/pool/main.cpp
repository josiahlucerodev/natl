
//natl
#include <natl/batchPool.h>
#include <natl/repeat.h>
#include <natl/test.h>

int main() {
	natl::BatchPool<int> batchPool;
	batchPool.newElement();
	natl::SmallBatchPool<int, 5> smallbatchPool;
	smallbatchPool.newElement();

	natl::BatchHeap<int> batchHeap;
	batchHeap.newElement();
	natl::SmallBatchHeap<int, 5> smallbatchHeap;
	smallbatchHeap.newElement();
}