
//natl
#include <natl/container/priorityQueue.h>
#include <natl/container/stringView.h>
#include <natl/util/test.h>

constexpr inline natl::ConstAsciiStringView natlTestFrom = "NatlPriorityQueue";

template<typename HeapType>
constexpr natl::Bool testConstruction() noexcept {
	natl::Test test(natlTestFrom, "construction test", natl::TestType::leaf);

	HeapType heap({ 1, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17 });
	heap.push(16); heap.push(11); heap.push(7); heap.push(12);

	natl::testAssert(test, natl::isHeap(heap.getContainer().begin(), heap.getContainer().end(), typename HeapType::value_compare()),
		"test if after construction if the container is a valid heap");

	return test;
}

template<typename HeapType>
constexpr natl::Bool testHeap(const natl::ConstAsciiStringView& name) noexcept {
	natl::Test test(natlTestFrom, name, natl::TestType::node);
	natl::subTestAssert(test, testConstruction<HeapType>());
	return test;
}

natl::Bool tests() noexcept {
	natl::TestTimer timer(natlTestFrom);
	natl::Test test(natlTestFrom, "all", natl::TestType::root);
	testHeap<natl::PriorityQueue<int>>("priority queue");
	testHeap<natl::MinHeap<int>>("min heap");
	testHeap<natl::SmallMinHeap<int, 100>>("small min heap");
	testHeap<natl::MaxHeap<int>>("max heap");
	testHeap<natl::SmallMaxHeap<int, 100>>("small max heap");
	return test;
}

int main() {
	tests();
}