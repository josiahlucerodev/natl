#pragma once

//own
#include "container.h"
#include "iterators.h"
#include "dynArray.h"
#include "smallDynArray.h"
#include "functional.h"
#include "arrayView.h"

//interface
namespace natl {
	namespace impl {
		constexpr Size heapParentIndex(const Size index) noexcept {
			return (index - 1) / 2;
		}

		constexpr Size heapLeftChildIndex(const Size index) noexcept {
			return (2 * index + 1);
		}

		constexpr Size heapRightChildIndex(const Size index) noexcept {
			return (2 * index + 2);
		}

		template <typename RandomIter, typename Compare>
		constexpr void heapify(RandomIter first, const Size size, const Size index, Compare compare = Compare()) noexcept {
			if (size <= 1) { return; }
			const Size leftIndex = heapLeftChildIndex(index);
			const Size rightIndex = heapRightChildIndex(index);
			Size smallest = index;
			using difference_type = typename IterPtrTraits<RandomIter>::difference_type;
			if (leftIndex < size && compare(iterValue<RandomIter>(first + static_cast<difference_type>(leftIndex)), iterValue<RandomIter>(first + static_cast<difference_type>(index)))) {
				smallest = leftIndex;
			}
			if (rightIndex < size && compare(iterValue<RandomIter>(first + static_cast<difference_type>(rightIndex)), iterValue<RandomIter>(first + static_cast<difference_type>(smallest)))) {
				smallest = rightIndex;
			}
			if (smallest != index) {
				iterSwap<RandomIter>(first + static_cast<difference_type>(index), first + static_cast<difference_type>(smallest));
				heapify<RandomIter, Compare>(first, size, smallest, compare);
			}
		}
	}


	template<typename RandomIter, typename Compare>
	constexpr void makeHeap(RandomIter first, RandomIter last, Compare compare = Compare()) noexcept{
		const Size size = iterDistance<RandomIter>(first, last);
		const Size startIndex = (size / 2) - 1;
		for (SSize index = static_cast<SSize>(startIndex); index >= SSize(0); index--) {
			impl::heapify<RandomIter, Compare>(first, size, static_cast<Size>(index), compare);
		}
	}

	template<typename RandomIter, typename Compare>
	constexpr void pushHeap(RandomIter first, RandomIter last, Compare compare) noexcept {
		using difference_type = typename IterPtrTraits<RandomIter>::difference_type;
		const Size size = iterDistance<RandomIter>(first, last);
		if (size <= 1) { return; }
		Size current = size - 1;
		while (current > 0) {
			const Size parent = impl::heapParentIndex(current);
			if (compare(iterValue<RandomIter>(first + static_cast<difference_type>(current)), iterValue<RandomIter>(first + static_cast<difference_type>(parent)))) {
				iterSwap<RandomIter>(first + static_cast<difference_type>(parent), first + static_cast<difference_type>(current));
				current = parent;
			} else {
				break;
			}
		}
	}

	template<typename RandomIter, typename Compare>
	constexpr void popHeap(RandomIter first, RandomIter last, Compare compare) noexcept {
		const Size size = iterDistance<RandomIter>(first, last);
		if (size <= 1) { return; }
		--last;
		iterSwap<RandomIter>(first, last);
		impl::heapify<RandomIter, Compare>(first, size - 1, 0, compare);
	}

	template<class RandomIter, class Compare>
	constexpr Bool isHeap(RandomIter first, RandomIter last, Compare compare) noexcept {
		using difference_type = typename IterPtrTraits<RandomIter>::difference_type;
		Size size = iterDistance<RandomIter>(first, last);
		for (Size index = 1; index < size; index++) {
			if (compare(
				iterValue<RandomIter>(first + static_cast<difference_type>(index)), 
				iterValue<RandomIter>(first + static_cast<difference_type>((index - 1) / 2))
				)) {
				return false;
			}
		}
		return true;
	}
	

	template<typename DataType, typename SequentialDynamicContainer = DynArray<DataType>, typename Compare = CompareLess<DataType>>
	class PriorityQueue {
	public:
		using container_type = SequentialDynamicContainer;
		using value_compare = Compare;
		using value_type = container_type::value_type;
		using size_type	= container_type::size_type;
		using reference	= container_type::reference;
		using const_reference = container_type::const_reference;
		
		using allocation_move_adapater = container_type::allocation_move_adapater;
	private:
		SequentialDynamicContainer sequentialContainer;
		[[no_unique_address]] Compare compareStorage;
	public:
		//constructor
		constexpr PriorityQueue() noexcept = default;
		constexpr PriorityQueue(Compare&& compare) noexcept : sequentialContainer(), compareStorage(natl::move(compare)) {}
		constexpr PriorityQueue(const PriorityQueue& other, Compare&& compare = Compare()) noexcept 
			: sequentialContainer(other.sequentialContainer), compareStorage(natl::move(compare)) {}
		constexpr PriorityQueue(PriorityQueue&& other, Compare&& compare = Compare()) noexcept 
			: sequentialContainer(natl::move(other.sequentialContainer)), compareStorage(natl::move(compare)) {}
		constexpr PriorityQueue(const SequentialDynamicContainer& otherContainer, Compare&& compare = Compare()) noexcept
			: sequentialContainer(otherContainer), compareStorage(natl::move(compare)) {
			makeHeapInternalContainer();
		}
		constexpr PriorityQueue(SequentialDynamicContainer&& otherContainer, Compare&& compare = Compare()) noexcept 
			: sequentialContainer(natl::move(otherContainer)), compareStorage(natl::move(compare)) {
			makeHeapInternalContainer();
		}
		constexpr PriorityQueue(std::initializer_list<value_type> ilist, Compare&& compare = Compare()) 
			noexcept : sequentialContainer(ilist), compareStorage(natl::move(compare)) {
			makeHeapInternalContainer();
		}
		constexpr PriorityQueue(const natl::ArrayView<value_type>& arrayView, Compare&& compare = Compare()) noexcept 
			: sequentialContainer(arrayView), compareStorage(natl::move(compare)) {
			makeHeapInternalContainer();
		}
		constexpr PriorityQueue(allocation_move_adapater&& allocationMoveAdapter, Compare&& compare = Compare()) noexcept 
			: sequentialContainer(natl::move(allocationMoveAdapter)), compareStorage(natl::move(compare)) {
			makeHeapInternalContainer();
		}
		

		//destructor
		constexpr ~PriorityQueue() noexcept = default;

		//util 
		constexpr PriorityQueue& self() noexcept { return *this; }
		constexpr const PriorityQueue& self() const noexcept { return *this; }

		//assignment 
		constexpr PriorityQueue& operator=(const PriorityQueue& other) noexcept {
			sequentialContainer = other.sequentialContainer;
		}
		constexpr PriorityQueue& operator=(PriorityQueue&& other) noexcept {
			sequentialContainer = natl::move(other.sequentialContainer);
		}
		constexpr PriorityQueue& operator=(const SequentialDynamicContainer& otherContainer) noexcept {
			sequentialContainer = otherContainer;
			makeHeapInternalContainer();
		}
		constexpr PriorityQueue& operator=(SequentialDynamicContainer&& otherContainer) noexcept {
			sequentialContainer = natl::move(otherContainer);
			makeHeapInternalContainer();
		}
		constexpr PriorityQueue& operator=(std::initializer_list<value_type> ilist) noexcept {
			sequentialContainer = ilist;
			makeHeapInternalContainer();
		}
		constexpr PriorityQueue& operator=(const natl::ArrayView<value_type>& arrayView) noexcept {
			sequentialContainer = arrayView;
			makeHeapInternalContainer();
		}
		constexpr PriorityQueue& operator=(allocation_move_adapater&& allocationMoveAdapter) noexcept {
			sequentialContainer = natl::move(allocationMoveAdapter);
			makeHeapInternalContainer();
		}

		//capacity
		[[nodiscard]] constexpr Bool empty() const noexcept { return sequentialContainer.empty(); }
		[[nodiscard]] constexpr Bool isEmpty() const noexcept { return empty(); }
		[[nodiscard]] constexpr Bool isNotEmpty() const noexcept { return !empty(); }
		constexpr size_type size() const noexcept { return sequentialContainer.size(); };

		//element access
		constexpr const_reference top() const noexcept { return sequentialContainer.front(); }
		constexpr const container_type& getContainer() const noexcept { return sequentialContainer; }

		//modifiers
		constexpr void push(const value_type& value) noexcept {
			sequentialContainer.push_back(value);
			pushHeapInternalContainer();
		}
		constexpr void push(value_type&& value) noexcept {
			sequentialContainer.push_back(natl::move(value));
			pushHeapInternalContainer();
		}

		template<typename... Args>
		constexpr void emplace(Args&&... args) noexcept {
			sequentialContainer.emplace_back(natl::forward<Args>(args)...);
			pushHeapInternalContainer();
		}

		constexpr void append(const SequentialDynamicContainer& otherContainer) noexcept {
			sequentialContainer.append(otherContainer);
			makeHeapInternalContainer();
		}
		constexpr void append(std::initializer_list<value_type> ilist) noexcept {
			sequentialContainer.append(ilist);
			makeHeapInternalContainer();
		}
		constexpr void append(const natl::ArrayView<value_type>& arrayView) noexcept {
			sequentialContainer.append(arrayView);
			makeHeapInternalContainer();
		}

		constexpr void pop() noexcept {
			popHeapInternalContainer();
			sequentialContainer.pop_back();
		}
		constexpr void pop(const Size number) noexcept {
			const Size limitedNumber = natl::min<Size>(number, size());
			for (Size index; index < limitedNumber; index++) {
				pop();
			}
		}

		constexpr void swap(PriorityQueue& other) noexcept {
			PriorityQueue temp = natl::move(other);
			other = natl::move(self());
			self() = natl::move(self());
		}
		[[nodiscard]] constexpr allocation_move_adapater getAlloctionMoveAdapater() noexcept {
			return natl::move(sequentialContainer.getAlloctionMoveAdapater());
		}
	private:
		constexpr void makeHeapInternalContainer() noexcept {
			makeHeap<typename container_type::iterator, Compare>(sequentialContainer.begin(), sequentialContainer.end(), compareStorage);
		}
		constexpr void pushHeapInternalContainer() noexcept {
			pushHeap<typename container_type::iterator, Compare>(sequentialContainer.begin(), sequentialContainer.end(), compareStorage);
		}
		constexpr void popHeapInternalContainer() noexcept {
			popHeap<typename container_type::iterator, Compare>(sequentialContainer.begin(), sequentialContainer.end(), compareStorage);
		}
	};

	template<typename DataType, typename Alloc = DefaultAllocator<DataType>, typename Compare = CompareLess<DataType>>
		requires(IsAllocator<Alloc>)
	using MinHeap = PriorityQueue<DataType, DynArray<DataType, Alloc>, Compare>;

	template<typename DataType, Size smallBufferSize, typename Alloc = DefaultAllocator<DataType>, typename Compare = CompareLess<DataType>>
		requires(IsAllocator<Alloc>)
	using SmallMinHeap = PriorityQueue<DataType, SmallDynArray<DataType, smallBufferSize, Alloc>, Compare>;


	template<typename DataType, typename Alloc = DefaultAllocator<DataType>, typename Compare = CompareGreater<DataType>>
		requires(IsAllocator<Alloc>)
	using MaxHeap = PriorityQueue<DataType, DynArray<DataType, Alloc>, Compare>;

	template<typename DataType, Size smallBufferSize, typename Alloc = DefaultAllocator<DataType>, typename Compare = CompareGreater<DataType>>
		requires(IsAllocator<Alloc>)
	using SmallMaxHeap = PriorityQueue<DataType, SmallDynArray<DataType, smallBufferSize, Alloc>, Compare>;

}