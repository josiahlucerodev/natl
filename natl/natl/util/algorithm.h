#pragma once

//own
#include "iterators.h"
#include "compare.h"

//interface
namespace natl {
	template<typename DataType> 
	constexpr DataType min(const DataType& a, const DataType& b) noexcept {
		return (b < a) ? b : a;
	}
	template<typename DataType> 
	constexpr DataType max(const DataType& a, const DataType& b) noexcept {
		return (b > a) ? b : a;
	}
	template<typename DataType>
	constexpr static DataType clamp(const DataType value, const DataType min, const DataType max) noexcept {
		return (value < min) ? min : ((value > max) ? max : value);
	}

	template <typename SizeType, typename ArrayViewLike>
		requires(IsArrayViewLike<ArrayViewLike, SizeType>)
	constexpr SizeType findLowerBoundIndex(const SizeType& value, ArrayViewLike& arrayView) noexcept {
		SizeType left = 0;
		SizeType right = arrayView.size();
		while (left < right) {
			SizeType mid = left + (right - left) / 2;
			if (arrayView.data()[mid] < value) {
				left = mid + 1;
			} else {
				right = mid;
			}
		}
		return left;
	}

	//heap
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
			using difference_type = typename IteratorTraits<RandomIter>::difference_type;
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

		template <typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
			requires(IsRandomAccessIteratorC<RandomIter>)
		constexpr void heapify(RandomIter first, RandomIter last, RandomIter root, Compare compare = Compare{}) noexcept {
			RandomIter largest = root;
			RandomIter left = first + 2 * (root - first) + 1;
			RandomIter right = first + 2 * (root - first) + 2;

			if (left < last && compare(*left, *largest)) {
				largest = left;
			}

			if (right < last && compare(*right, *largest)) {
				largest = right;
			}

			if (largest != root) {
				iterSwap(root, largest);
				heapify(first, last, largest, compare);
			}
		}
	}


	template<typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr void makeHeap(RandomIter first, RandomIter last, Compare compare = Compare{}) noexcept {
		const Size size = iterDistance<RandomIter>(first, last);
		const Size startIndex = (size / 2) - 1;
		for (SSize index = static_cast<SSize>(startIndex); index >= SSize(0); index--) {
			impl::heapify<RandomIter, Compare>(first, size, static_cast<Size>(index), compare);
		}
	}

	template<typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr void pushHeap(RandomIter first, RandomIter last, Compare compare = Compare{}) noexcept {
		using difference_type = typename IteratorTraits<RandomIter>::difference_type;
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
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr void popHeap(RandomIter first, RandomIter last, Compare compare) noexcept {
		const Size size = iterDistance<RandomIter>(first, last);
		if (size <= 1) { return; }
		--last;
		iterSwap<RandomIter>(first, last);
		impl::heapify<RandomIter, Compare>(first, size - 1, 0, compare);
	}

	template<typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr Bool isHeap(RandomIter first, RandomIter last, Compare compare = Compare{}) noexcept {
		using difference_type = typename IteratorTraits<RandomIter>::difference_type;
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

	//heap sort
	template<typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr void heapSort(RandomIter first, RandomIter last, Compare compare = Compare{}) noexcept {
		using value_type = typename IteratorTraits<RandomIter>::value_type;
		auto swapedCompare = [&compare](const value_type& lhs, const value_type& rhs) noexcept {
			return compare(rhs, lhs);
		};
		for (RandomIter i = first + (last - first) / 2 - 1; i >= first; --i) {
			impl::heapify(first, last, i, swapedCompare);
		}
		for (RandomIter i = last - 1; i > first; --i) {
			iterSwap(first, i);
			impl::heapify(first, i, first, swapedCompare);
		}
	}

	//insertion sort
	template<typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr void insertionSort(RandomIter first, RandomIter last, Compare compare = Compare{}) noexcept {
		for (RandomIter i = next(first); i != last; ++i) {
			typename IteratorTraits<RandomIter>::value_type key = move(*i);
			RandomIter j = prev(i);

			while (j >= first && compare(key, *j)) {
				*next(j) = move(*j);
				--j;
			}
			*next(j) = move(key);
		}
	}

	//quick sort
	namespace impl {
		template <typename RandomIter, typename Compare>
		constexpr RandomIter quickSortPartition(RandomIter first, RandomIter last, Compare compare) noexcept {
			using value_type = typename IteratorTraits<RandomIter>::value_type;
			value_type pivot = *prev(last);
			RandomIter i = first;
			for (RandomIter j = first; j != prev(last); ++j) {
				if (compare(*j, pivot)) {
					iterSwap(i, j);
					++i;
				}
			}
			iterSwap(i, prev(last));
			return i;
		}
	}

	template <typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr void quickSort(RandomIter first, RandomIter last, Compare compare = Compare{}) noexcept {
		if (first != last && first != prev(last)) {
			RandomIter pivot = impl::quickSortPartition(first, last, compare);
			quickSort(first, pivot, compare);
			quickSort(next(pivot), last, compare);
		}
	}

	//intro sort
	namespace impl {
		template <typename RandomIter, typename Compare>
		constexpr void introSortRecursive(RandomIter first, RandomIter last, Compare compare, Size depthLimit) noexcept {
			if (first < last) {
				using value_type = typename IteratorTraits<RandomIter>::value_type;
				constexpr Size distanceThreshold = (sizeof(ui64) * 50) / sizeof(value_type);
				const Size distance = iterDistance(first, last);
				if (distance <= distanceThreshold) {
					//insertion sort if small amount of elements 
					insertionSort(first, last, compare);
				} else if (depthLimit == 0) {
					//heap sort if depth is too deep
					heapSort(first, last, compare);
				} else {
					//quick sort
					RandomIter pivot = quickSortPartition(first, last, compare);
					introSortRecursive(first, pivot, compare, depthLimit - 1);
					introSortRecursive(next(pivot), last, compare, depthLimit - 1);
				}
			}
		}
	}

	template<typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr void introSort(RandomIter first, RandomIter last, Compare compare = Compare{}) noexcept {
		const Size distance = iterDistance(first, last);
		const Size depthLimit = (distance >> 1) + (distance >> 2);
		impl::introSortRecursive(first, last, compare, depthLimit);
	}

	//sort
	template<typename RandomIter, typename Compare = CompareLess<typename IteratorTraits<RandomIter>::value_type>>
		requires(IsRandomAccessIteratorC<RandomIter>)
	constexpr void sort(RandomIter first, RandomIter last, Compare compare = Compare{}) noexcept {
		introSort(first, last, compare);
	}
}