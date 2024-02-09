#pragma once 

//std
#include <cstdint>
#include <vector>

//own
#include "iterators.h"
#include "array.h"
#include "arrayView.h"
#include "array.h"
#include "dynArray.h"

//interface 
namespace natl {
	template<class DataType, class Alloc>
		requires(IsAllocator<Alloc>)
	class DynamicPartitioner {
	public:
		using allocator_type = Alloc;

		using value_type = typename Alloc::value_type;
		using reference = typename Alloc::reference;
		using const_reference = typename Alloc::const_reference;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using difference_type = typename Alloc::difference_type;
		using size_type = typename Alloc::size_type;

		using optional_pointer = Option<pointer>;
		using optional_const_pointer = Option<const_pointer>;

		using iterator = RandomAccessIteratorAlloc<value_type, Alloc>;
		using const_iterator = ConstRandomAccessIteratorAlloc<value_type, Alloc>;
		using reverse_iterator = ReverseRandomAccessIteratorAlloc<value_type, Alloc>;
		using const_reverse_iterator = ReverseConstRandomAccessIteratorAlloc<value_type, Alloc>;

		using container_allocation_move_adapater = AllocationMoveAdapater<value_type, Alloc>;

		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;
	private:
		DynArray<value_type, Alloc> data;
		size_type partitionIndex;
	public:
		DynamicPartitioner() : data(), partitionIndex(0) {}

		constexpr void resize(const size_type newCapacity) noexcept { data.resize(newCapacity); }
		constexpr size_type size() const noexcept { return partitionIndex + 1; }
		constexpr size_type capacity() const noexcept { return data.capacity(); }
		constexpr bool isFull() const noexcept { return capacity() < partitionIndex + 1; };
		constexpr bool isEmpty() const noexcept { return partitionIndex == 0; };
		constexpr bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		constexpr void shrinkToFit() noexcept { data.shrink_to_fit(); }
		constexpr void clear() noexcept { data.clear(); }
		constexpr ArrayView<value_type> newPartition(const size_type partiationSize) noexcept {
			if (capacity() < partitionIndex + partiationSize) {
				return ArrayView<value_type>(nullptr, 0);
			}

			ArrayView<value_type> partition(&data.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return partition;
		}

		constexpr pointer beginPtr() noexcept { return data.begin(); }
		constexpr const_pointer beginPtr() const noexcept { return data.cbegin(); }
		constexpr pointer endPtr() noexcept { return data.end(); }
		constexpr const_pointer endPtr() const noexcept { return data.cend(); }

		constexpr iterator begin() noexcept { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }
	};


	inline std::size_t alignmentOffset(std::uint8_t* ptr, std::size_t alignment) noexcept {
		std::size_t offset = static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(ptr) & (alignment - 1));
		if (offset != 0) { offset = alignment - offset; }// number of bytes to skip 
		return offset;
	}

	template<class Alloc>
		requires(IsAllocator<Alloc>)
	class DynamicBytePartitioner : public DynamicPartitioner<std::uint8_t, Alloc> {
		using allocator_type = Alloc;

		using value_type = typename Alloc::value_type;
		using reference = typename Alloc::reference;
		using const_reference = typename Alloc::const_reference;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using difference_type = typename Alloc::difference_type;
		using size_type = typename Alloc::size_type;

		using optional_pointer = Option<pointer>;
		using optional_const_pointer = Option<const_pointer>;

		using iterator = RandomAccessIteratorAlloc<value_type, Alloc>;
		using const_iterator = ConstRandomAccessIteratorAlloc<value_type, Alloc>;
		using reverse_iterator = ReverseRandomAccessIteratorAlloc<value_type, Alloc>;
		using const_reverse_iterator = ReverseConstRandomAccessIteratorAlloc<value_type, Alloc>;

		using container_allocation_move_adapater = AllocationMoveAdapater<value_type, Alloc>;

		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;
	public:
		DynamicBytePartitioner() : DynamicPartitioner<std::uint8_t, Alloc>() {}
	public:
		template<class value_type>
		ArrayView<value_type> newPartition(const size_type partiationSize) noexcept {
			const size_type offset = alignmentOffset(this->data.data(), alignof(value_type));
			const size_type partiationByteSize = partiationSize * sizeof(value_type) + offset;
			if (this->capacity() < partiationByteSize) {
				return ArrayView<value_type>(nullptr, 0);
			}

			DynamicPartitioner<std::uint8_t, Alloc>& castSelf = *static_cast<DynamicPartitioner<std::uint8_t, Alloc>*>(this);
			ArrayView<std::uint8_t> bytePartition = castSelf.newPartition(partiationByteSize);
			ArrayView<value_type> partition(static_cast<pointer>(static_cast<void*>(bytePartition.at(offset))), partiationSize);
			return partition;
		}
	};

	template<class value_type>
	class SubPartitioner {
		using size_type = std::size_t;
	private:
		ArrayView<value_type> partition;
		size_type partitionIndex;
	public:
		constexpr SubPartitioner() : partition(), partitionIndex(0) {};
		constexpr SubPartitioner(const ArrayView<value_type>& partition) : partition(partition), partitionIndex(0) {}

		constexpr size_type size() const noexcept { return partitionIndex + 1; }
		constexpr size_type capacity() const noexcept { return partition.size(); }
		constexpr bool isFull() const noexcept { return capacity() < size(); };
		constexpr bool isEmpty() const noexcept { return partitionIndex == 0; };
		constexpr bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		constexpr ArrayView<value_type> getInternalPartition() const noexcept { return partition; };

		constexpr ArrayView<value_type> newPartition(const size_type partiationSize) noexcept {
			if (capacity() < partiationSize) {
				return ArrayView<value_type>(nullptr, 0);
			}

			ArrayView<value_type> outputPartition(partition.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return outputPartition;
		}
	};
}