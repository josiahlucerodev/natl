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
	class DynamicBytePartitioner;

	template<class Type>
	class DynamicPartitioner {
	public:
		using pointer = Type*;
		using const_pointer = const Type*;
		using size_type = Size;
		using iterator = RandomAccessIterator<Type>;
		using const_iterator = RandomAccessIterator<const Type>;
		using reverse_iterator = ReverseRandomAccessIterator<Type>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const Type>;
	private:
		DynArray<Type> data;
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
		constexpr ArrayView<Type> newPartition(const size_type partiationSize) noexcept {
			if (capacity() < partitionIndex + partiationSize) {
				return ArrayView<Type>(nullptr, 0);
			}

			ArrayView<Type> partition(&data.at(partitionIndex), partiationSize);
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
	public:
		friend class DynamicBytePartitioner;
	};


	inline std::size_t alignmentOffset(std::uint8_t* ptr, std::size_t alignment) noexcept {
		std::size_t offset = static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(ptr) & (alignment - 1));
		if (offset != 0) { offset = alignment - offset; }// number of bytes to skip 
		return offset;
	}

	class DynamicBytePartitioner : public DynamicPartitioner<std::uint8_t> {
		using value_type = std::uint8_t;
		using pointer = std::uint8_t*;
		using const_pointer = const std::uint8_t*;
		using size_type = std::size_t;
		using iterator = RandomAccessIterator<std::uint8_t>;
		using const_iterator = RandomAccessIterator<const std::uint8_t>;
		using reverse_iterator = ReverseRandomAccessIterator<std::uint8_t>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const std::uint8_t>;
	public:
		DynamicBytePartitioner() : DynamicPartitioner<std::uint8_t>() {}
	public:
		template<class Type>
		ArrayView<Type> newPartition(const size_type partiationSize) noexcept {
			const size_type offset = alignmentOffset(data.data(), alignof(Type));
			const size_type partiationByteSize = partiationSize * sizeof(Type) + offset;
			if (capacity() < partiationByteSize) {
				return ArrayView<Type>(nullptr, 0);
			}

			DynamicPartitioner<std::uint8_t>& castSelf = *static_cast<DynamicPartitioner<std::uint8_t>*>(this);
			ArrayView<std::uint8_t> bytePartition = castSelf.newPartition(partiationByteSize);
			ArrayView<Type> partition(static_cast<pointer>(static_cast<void*>(bytePartition.at(offset))), partiationSize);
			return partition;
		}
	};

	template<class Type>
	class SubPartitioner {
		using size_type = std::size_t;
	private:
		ArrayView<Type> partition;
		size_type partitionIndex;
	public:
		constexpr SubPartitioner() : partition(), partitionIndex(0) {};
		constexpr SubPartitioner(const ArrayView<Type>& partition) : partition(partition), partitionIndex(0) {}

		constexpr size_type size() const noexcept { return partitionIndex + 1; }
		constexpr size_type capacity() const noexcept { return partition.size(); }
		constexpr bool isFull() const noexcept { return capacity() < size(); };
		constexpr bool isEmpty() const noexcept { return partitionIndex == 0; };
		constexpr bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		constexpr ArrayView<Type> getInternalPartition() const noexcept { return partition; };

		constexpr ArrayView<Type> newPartition(const size_type partiationSize) noexcept {
			if (capacity() < partiationSize) {
				return ArrayView<Type>(nullptr, 0);
			}

			ArrayView<Type> outputPartition(partition.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return outputPartition;
		}
	};
}