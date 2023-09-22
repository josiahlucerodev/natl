#pragma once 

//std
#include <cstdint>
#include <vector>

//own
#include "iterators.h"
#include "array.h"
#include "partition.h"

//interface 
namespace natl {
	template<class Type, std::size_t size>
	class FixedPartitioner {
		using pointer = Type*;
		using const_pointer = const Type*;
		using size_type = std::size_t;
		using iterator = RandomAccessIterator<Type>;
		using const_iterator = RandomAccessIterator<const Type>;
		using reverse_iterator = ReverseRandomAccessIterator<Type>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const Type>;
	public:
		natl::Array<Type, size> partitionData;
		size_type partitionIndex;
	public:
		constexpr FixedPartitioner() : partitionData(), partitionIndex(0) {}

		constexpr size_type size() const noexcept { return partitionData.size(); }
		constexpr size_type capacity() const noexcept { return partitionData.capacity(); }
		constexpr bool isFull() const noexcept { return capacity() < size(); };
		constexpr bool isEmpty() const noexcept { return partitionIndex == 0; };
		constexpr bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		constexpr pointer data() const noexcept { return partitionData; }
		constexpr void clear() noexcept { partitionIndex = 0; }
		constexpr Partition<Type> newPartition(const size_type partiationSize) {
			if (capacity() < partitionIndex + partiationSize) {
				return Partition<Type>(nullptr, 0);
			}

			Partition<Type> partition(&partitionData.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return partition;
		}
	public:
		constexpr pointer beginPtr() noexcept { return partitionData.begin(); }
		constexpr const_pointer beginPtr() const noexcept { return partitionData.cbegin(); }
		constexpr pointer endPtr() noexcept { return partitionData.end(); }
		constexpr const_pointer endPtr() const noexcept { return partitionData.cend(); }

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

	class DynamicBytePartitioner;

	template<class Type>
	class DynamicPartitioner {
	public:
		using pointer = Type*;
		using const_pointer = const Type*;
		using size_type = std::size_t;
		using iterator = RandomAccessIterator<Type>;
		using const_iterator = RandomAccessIterator<const Type>;
		using reverse_iterator = ReverseRandomAccessIterator<Type>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const Type>;
	private:
		std::vector<Type> data;
		size_type partitionIndex;
	public:
		DynamicPartitioner() : data(), partitionIndex(0) {}

		constexpr void resize(const size_type newCapacity) noexcept { data.resize(newCapacity); }
		constexpr size_type size() const noexcept { return partitionIndex + 1; }
		constexpr size_type capacity() const noexcept { return data.capacity(); }
		constexpr bool isFull() const noexcept { return capacity() < partitionIndex + 1; };
		constexpr bool isEmpty() const noexcept { return partitionIndex == 0; };
		constexpr bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		constexpr void shrinkToFit() noexcept { data.shrinkToFit(); }
		constexpr void clear() noexcept { data.clear(); }
		constexpr void fullClear() noexcept { std::vector<Type> trash; trash.swap(data); }
		constexpr Partition<Type> newPartition(const size_type partiationSize) noexcept {
			if (capacity() < partitionIndex + partiationSize) {
				return Partition<Type>(nullptr, 0);
			}

			Partition<Type> partition(&data.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return partition;
		}
		constexpr void swap(DynamicPartitioner<Type>& dst) noexcept {
			dst.data.swap(data);
			size_type tempPartitionIndex = partitionIndex;
			partitionIndex = dst.partitionIndex;
			dst.partitionIndex = tempPartitionIndex;
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
		Partition<Type> newPartition(const size_type partiationSize) noexcept {
			const size_type offset = alignmentOffset(data.data(), alignof(Type));
			const size_type partiationByteSize = partiationSize * sizeof(Type) + offset;
			if (capacity() < partiationByteSize) {
				return Partition<Type>(nullptr, 0);
			}

			DynamicPartitioner<std::uint8_t>& castSelf = *static_cast<DynamicPartitioner<std::uint8_t>*>(this);
			Partition<std::uint8_t> bytePartition = castSelf.newPartition(partiationByteSize);
			Partition<Type> partition(static_cast<pointer>(static_cast<void*>(bytePartition.at(offset))), partiationSize);
			return partition;
		}
	};

	template<class Type>
	class SubPartitioner {
		using size_type = std::size_t;
	private:
		Partition<Type> partition;
		size_type partitionIndex;
	public:
		constexpr SubPartitioner() : partition(), partitionIndex(0) {};
		constexpr SubPartitioner(const Partition<Type>& partition) : partition(partition), partitionIndex(0) {}

		constexpr size_type size() const noexcept { return partitionIndex + 1; }
		constexpr size_type capacity() const noexcept { return partition.size(); }
		constexpr bool isFull() const noexcept { return capacity() < size(); };
		constexpr bool isEmpty() const noexcept { return partitionIndex == 0; };
		constexpr bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		constexpr void clear() noexcept { return partitionIndex = 0; };
		constexpr Partition<Type> getInternalPartition() const noexcept { return partition; };
		constexpr void assign(const Partition<Type>& assignPartition) noexcept { partition = assignPartition; partitionIndex = 0; };

		constexpr Partition<Type> newPartition(const size_type partiationSize) noexcept {
			if (capacity() < partiationSize) {
				return Partition<Type>(nullptr, 0);
			}

			Partition<Type> outputPartition(partition.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return outputPartition;
		}
	};
}