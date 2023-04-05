#pragma once

//std
#include <optional>
#include <cstdint>

//own
#include "iterators.h"
#include "container.h"
#include "typeTraits.h"

//interface
namespace natl {
	template <class Type>
	class Partition {
	public:
		using value_type = Type;
		using reference = Type&;
		using const_reference = const Type&;
		using optional_reference = std::optional<std::reference_wrapper<Type>>;
		using optional_const_reference = std::optional<std::reference_wrapper<const Type>>;
		using pointer = Type*;
		using const_pointer = const Type*;
		using optional_pointer = std::optional<Type*>;
		using optional_const_pointer = std::optional<const Type*>;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;

		using iterator = RandomAccessIterator<Type>;
		using const_iterator = RandomAccessIterator<const Type>;
		using reverse_iterator = ReverseRandomAccessIterator<Type>; 
		using const_reverse_iterator = ReverseRandomAccessIterator<const Type>;
	private:
		pointer dataPtr;
		size_type sizePartition;
	public:
		constexpr Partition() = default;
		constexpr ~Partition() = default;
		constexpr Partition(pointer dataPtr, const size_type size) : dataPtr(dataPtr), sizePartition(size) {}
		constexpr Partition(std::initializer_list<Type> initList) 
			noexcept requires(isConst<Type>) : 
			dataPtr(initList.begin()), 
			sizePartition(initList.size()) {}

		constexpr size_type size() const noexcept { return sizePartition; }
		constexpr pointer data() noexcept requires(isNotConst<Type>) { return dataPtr; }
		constexpr const_pointer data() const noexcept { return dataPtr; };

		constexpr pointer beginPtr() noexcept requires(isNotConst<Type>) { return dataPtr; }
		constexpr const_pointer beginPtr() const noexcept { return dataPtr; }
		constexpr pointer endPtr() noexcept requires(isNotConst<Type>)  { return dataPtr + sizePartition; }
		constexpr const_pointer endPtr() const noexcept { return dataPtr + sizePartition; }

		constexpr iterator begin() noexcept requires(isNotConst<Type>) { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept requires(isNotConst<Type>) { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(isNotConst<Type>) { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(isNotConst<Type>) { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		constexpr bool isEmpty() const noexcept { return !bool(sizePartition); }
		constexpr bool isNotEmpty() const noexcept { return bool(sizePartition); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		constexpr reference at(const size_type index) noexcept requires(isNotConst<Type>) { return dataPtr[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return dataPtr[index]; };

		constexpr size_type clamp(const size_type value, const size_type min, const size_type max) const noexcept {
			const size_type t = value < min ? min : value;
			return t > max ? max : t;
		}

		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp(index, 0, sizePartition - 1); }

		constexpr reference atClamped(const size_type index) noexcept requires(isNotConst<Type>) { return dataPtr[clampIndex(index)]; }
		constexpr const_pointer atClamped(const size_type index) const noexcept { return dataPtr[clampIndex(index)]; }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return sizePartition ? sizePartition - 1 : 0; }

		constexpr reference front() noexcept requires(isNotConst<Type>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr bool has(const size_type index) const noexcept { return index < sizePartition; }
		constexpr bool notHave(const size_type index) const noexcept { return index >= sizePartition; }


		/*
		optional_const_reference optionalGetConstRefAt(const size_type index) const {
			if (notHave(index)) { return optional_const_reference(); }
			return optional_const_reference(at(index));
		};
		optional_reference optionalGetRefAt(const size_type index) {
			if (notHave(index)) { return optional_reference(); }
			return optional_reference(at(index));
		};

		optional_const_pointer optionalGetConstPtrAt(const size_type index) {
			if (notHave(index)) { return optional_const_pointer(); }
			return optional_const_pointer(at(index));
		};
		optional_pointer optionalGetPtrAt(const size_type index) {
			if (notHave(index)) { return optional_pointer(); }
			return optional_pointer(at(index));
		};

		optional_reference optionalFront() { return optionalGetRefAt(frontIndex()); }
		optional_const_reference optionalFront() const { return optionalGetConstRefAt(frontIndex()); }
		optional_reference optionalBack() { return optionalGetRefAt(backIndex()); }
		optional_const_reference optionalBack() const { return optionalGetConstRefAt(backIndex()); }
		*/

		constexpr void assign(std::initializer_list<Type> initList) noexcept requires(isConst<Type>) {
			dataPtr = initList.begin();
			sizePartition = initList.size();
		}

		constexpr void copy(std::initializer_list<Type> initList) noexcept requires(isNotConst<Type>) {
			const size_type count = std::max<size_type>(initList.size(), size());
			for (size_type i = 0; i < count; i++) {
				at(i) = initList[i];
			}
		}
		constexpr bool sizeMatchCopy(std::initializer_list<Type> initList) noexcept requires(isNotConst<Type>) {
			if (initList.size() != size()) { 
				return false; 
			}
			else {
				copy(initList);
				return true;
			}
		}

		constexpr void operator=(std::initializer_list<Type> initList) noexcept requires(isConst<Type>) {  
			dataPtr = initList.begin();
			sizePartition = initList.size();
		}
	};

	template<class Container>
	concept ContainerPartitionConstructable =
		HasIteratorType<Container> &&
		HasBeginIteratorToPtr<Container> &&
		HasSizeMethod<Container> &&
		std::random_access_iterator<typename Container::iterator>;
	template<class Container>
		requires(ContainerPartitionConstructable<Container>)
	Partition<typename ContainerIteratorTraits<Container>::value_type> newPartitionFromContainer(Container& container) {
		return Partition<typename ContainerIteratorTraits<Container>::value_type>(&*container.begin(), container.size());
	}

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
		std::array<Type, size> partitionData;
		size_type partitionIndex;
	public:
		 size_type size() const noexcept { return partitionData.size(); }
		 size_type capacity() const noexcept { return partitionData.capacity(); }
		 bool isFull() const noexcept { return capacity() < size(); };
		 bool isEmpty() const noexcept { return partitionIndex == 0; };
		 bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		 pointer data() const noexcept { return partitionData; }
		 void clear() noexcept { partitionIndex = 0; }
		 Partition<Type> newPartition(const size_type partiationSize) {
			if (capacity() < partitionIndex + partiationSize) {
				return Partition<Type>(nullptr, 0);
			}

			Partition<Type> partition(&partitionData.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return partition;
		}
	public:
		pointer beginPtr() noexcept { return partitionData.begin(); }
		const_pointer beginPtr() const noexcept { return partitionData.cbegin(); }
		pointer endPtr() noexcept { return partitionData.end(); }
		const_pointer endPtr() const noexcept { return partitionData.cend(); }

		iterator begin() noexcept { return iterator(beginPtr()); }
		const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		iterator end() noexcept { return iterator(endPtr()); }
		const_iterator end() const noexcept { return const_iterator(endPtr()); }
		const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		reverse_iterator rbegin() noexcept { return reverse_iterator(endPtr()); }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		reverse_iterator rend() noexcept { return reverse_iterator(beginPtr()); }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }
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

		 void resize(const size_type newCapacity) { data.resize(newCapacity); }
		 size_type size() const noexcept { return partitionIndex + 1; }
		 size_type capacity() const noexcept { return data.capacity(); }
		 bool isFull() const noexcept { return capacity() < partitionIndex + 1; };
		 bool isEmpty() const noexcept { return partitionIndex == 0; };
		 bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		 void shrinkToFit() { data.shrink_to_fit(); }
		 void clear() { data.clear(); }
		 void fullClear() { std::vector<Type> trash; trash.swap(data); }
		 Partition<Type> newPartition(const size_type partiationSize) {
			if (capacity() < partitionIndex + partiationSize) {
				return Partition<Type>(nullptr, 0);
			}

			Partition<Type> partition(&data.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return partition;
		}
		void swap(DynamicPartitioner<Type>& dst) {
			dst.data.swap(data);
			size_type tempPartitionIndex = partitionIndex;
			partitionIndex = dst.partitionIndex;
			dst.partitionIndex = tempPartitionIndex;
		}
	
		pointer beginPtr() noexcept { return data.begin(); }
		const_pointer beginPtr() const noexcept { return data.cbegin(); }
		pointer endPtr() noexcept { return data.end(); }
		const_pointer endPtr() const noexcept { return data.cend(); }

		iterator begin() noexcept { return iterator(beginPtr()); }
		const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		iterator end() noexcept { return iterator(endPtr()); }
		const_iterator end() const noexcept { return const_iterator(endPtr()); }
		const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		reverse_iterator rbegin() noexcept { return reverse_iterator(endPtr()); }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		reverse_iterator rend() noexcept { return reverse_iterator(beginPtr()); }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }
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
		Partition<Type> newPartition(const size_type partiationSize) {
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
		SubPartitioner() = default;
		SubPartitioner(const Partition<Type>& partition) : partition(partition), partitionIndex(0) {}

		 size_type size() const noexcept { return partitionIndex + 1; }
		 size_type capacity() const noexcept { return partition.size(); }
		 bool isFull() const noexcept { return capacity() < size(); };
		 bool isEmpty() const noexcept { return partitionIndex == 0; };
		 bool isNotEmpty() const noexcept { return partitionIndex != 0; };
		 void clear() noexcept { return partitionIndex = 0; };
		 Partition<Type> getInternalPartition() const { return partition; };
		 void assign(const Partition<Type>& assignPartition) { partition = assignPartition; partitionIndex = 0; };

		Partition<Type> newPartition(const size_type partiationSize) {
			if (capacity() < partiationSize) {
				return Partition<Type>(nullptr, 0);
			}

			Partition<Type> outputPartition(partition.at(partitionIndex), partiationSize);
			partitionIndex += partiationSize;
			return outputPartition;
		}
	};
}