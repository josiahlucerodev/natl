
//natl
#include <natl/container/priorityQueue.h>
#include <natl/container/stringView.h>

template<typename HeapType>
constexpr void testHeap(const natl::ConstAsciiStringView& name) noexcept {
	HeapType heap({ 1, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17 });
	heap.push(16); heap.push(11); heap.push(7); heap.push(12);

	std::cout << name << ": ";
	if (!natl::isHeap(heap.getContainer().begin(), heap.getContainer().end(), typename HeapType::value_compare())) {
		std::cout << "error is not heap";
	}
	std::cout << "\n";
	while (heap.isNotEmpty()) {
		std::cout << heap.top() << " ";
		heap.pop();

	}
	std::cout << "\n";
}


int main() {
	testHeap<natl::PriorityQueue<int>>("priority queue");
	testHeap<natl::MinHeap<int>>("min heap");
	testHeap<natl::SmallMinHeap<int, 100>>("small min heap");
	testHeap<natl::MaxHeap<int>>("max heap");
	testHeap<natl::SmallMaxHeap<int, 100>>("small max heap");
}