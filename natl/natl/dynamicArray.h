#pragma once 

//std
#include <initializer_list>

//own
#include "typeTraits.h"
#include "iterators.h"
#include "option.h"
#include "allocator.h"
#include "dataMovement.h"

//interface
namespace natl {

	template<class DataType,
		class Alloc = Allocator>
	class DynamicArray : AllocatorStorage<DataType, Alloc> {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using optional_pointer = Option<DataType*>;
		using optional_const_pointer = Option<const DataType*>;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		using allocator_type = Alloc;

		using iterator = RandomAccessIterator<DataType>;
		using const_iterator = RandomAccessIterator<const DataType>;
		using reverse_iterator = ReverseRandomAccessIterator<DataType>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const DataType>;
		using alloc_traits = AllocatorTraits<Alloc>;
	private:
		pointer beginDataPtr;
		pointer endDataPtr;
		size_type inCapacity;
	public:
		constexpr DynamicArray() = default;

		constexpr DynamicArray(const DynamicArray& src) {
			beginDataPtr = 0;
			endDataPtr = 0;
			inCapacity = 0;

			if (src.isNotEmpty()) {
				reserve(src.capacity());
				uninitializedCopyCountNoOverlap<const_pointer, pointer>(src.beginDataPtr, beginDataPtr, src.size());
				endDataPtr = beginDataPtr + src.size();
			}
		}
		constexpr DynamicArray(DynamicArray&& src) {
			beginDataPtr = src.beginDataPtr; endDataPtr = src.endDataPtr; inCapacity = src.inCapacity;
			src.beginDataPtr = nullptr; src.endDataPtr = nullptr; src.inCapacity = 0;
		}

		constexpr DynamicArray(std::initializer_list<DataType> src) : beginDataPtr{}, endDataPtr{}, inCapacity{} {
			if (src.size() > 0) {
				this->reserve(src.size());
				uninitializedCopyCountNoOverlap<const_pointer, pointer>(src.begin(), beginDataPtr, src.size());
				endDataPtr = beginDataPtr + src.size();
			}
		}

		constexpr ~DynamicArray() { 
			deconstructAllData(); 
			freeData();
		};

		constexpr size_type size() const noexcept { return endDataPtr - beginDataPtr; }
		constexpr size_type capacity() const noexcept { return inCapacity; }
		constexpr size_type freeSpaceSize() const noexcept { return capacity() - size(); }
		constexpr pointer data() noexcept requires(isNotConst<DataType>) { return beginDataPtr; }
		constexpr const_pointer data() const noexcept { return beginDataPtr; };

		constexpr pointer beginPtr() noexcept requires(isNotConst<DataType>) { return beginDataPtr; }
		constexpr const_pointer beginPtr() const noexcept { return beginDataPtr; }
		constexpr pointer endPtr() noexcept requires(isNotConst<DataType>) { return endDataPtr; }
		constexpr const_pointer endPtr() const noexcept { return endDataPtr; }

		constexpr iterator begin() noexcept requires(isNotConst<DataType>) { return iterator(beginPtr()); }
		constexpr const_iterator begin() const noexcept { return const_iterator(beginPtr()); }
		constexpr const_iterator cbegin() const noexcept { return const_iterator(beginPtr()); }
		constexpr iterator end() noexcept requires(isNotConst<DataType>) { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return const_iterator(endPtr()); }
		constexpr reverse_iterator rbegin() noexcept requires(isNotConst<DataType>) { return reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(endPtr()); }
		constexpr reverse_iterator rend() noexcept requires(isNotConst<DataType>) { return reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(beginPtr()); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(beginPtr()); }

		constexpr bool isEmpty() const noexcept { return !bool(size()); }
		constexpr bool isNotEmpty() const noexcept { return bool(size()); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		constexpr reference at(const size_type index) noexcept requires(isNotConst<DataType>) { return beginDataPtr[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return beginDataPtr[index]; };
		constexpr reference operator[] (const size_type index) noexcept { return at(index); }
		constexpr const_reference operator[] (const size_type index) const noexcept { return at(index); }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return size() ? size() - 1 : 0; }

		constexpr reference front() noexcept requires(isNotConst<DataType>) { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr bool has(const size_type index) const noexcept { return index < size(); }
		constexpr bool notHave(const size_type index) const noexcept { return index >= size(); }

		constexpr size_type clamp(const size_type value, const size_type min, const size_type max) const noexcept {
			const size_type t = value < min ? min : value;
			return t > max ? max : t;
		}
		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp(index, 0, size() - 1); }

		constexpr reference atClamped(const size_type index) noexcept requires(isNotConst<DataType>) { return beginDataPtr[clampIndex(index)]; }
		constexpr const_pointer atClamped(const size_type index) const noexcept { return beginDataPtr[clampIndex(index)]; }

		constexpr void reserve(const size_type newCapacity) noexcept {
			if (newCapacity <= capacity()) { return; }
			pointer newDataBeginLocation = this->template alloc<DataType>(newCapacity);
			pointer newDataEndLocation = newDataBeginLocation + size();

			if (beginDataPtr) {
				uninitializedCopyCountNoOverlap<const_pointer, pointer>(beginDataPtr, newDataBeginLocation, size());
				freeData();
			}

			beginDataPtr = newDataBeginLocation;
			endDataPtr = newDataEndLocation;
			inCapacity = newCapacity;
		}

		constexpr void resize(const size_type newSize) noexcept {
			const size_type oldSize = size();
			if (oldSize == newSize) {
				return;
			}

			if (newSize < oldSize) {
				const size_type destorySize = oldSize - newSize;
				endDataPtr = beginDataPtr + newSize;

				if constexpr (!std::is_trivially_destructible_v<DataType>) {
					defualtDeconstructAll<DataType>(endDataPtr, destorySize);
				}
				return;
			}

			if (newSize > capacity()) {
				reserve(newSize);
			}

			const size_type constructSize = newSize - oldSize;
			defualtConstructAll<DataType>(endDataPtr, constructSize);
			endDataPtr = beginDataPtr + newSize;
		}

		constexpr void clear() noexcept {
			deconstructAllData();
			endDataPtr = beginDataPtr;
		}

		constexpr void pushBack(const DataType& value) noexcept {
			if (size() == capacity()) {
				reserve(size() + 1);
			}
			endDataPtr++;
			std::construct_at<DataType, const DataType&>(&back(), value);
		}

		constexpr void pushBack(DataType&& value) noexcept {
			if (size() == capacity()) {
				reserve((size() + 1) * 2);
			}
			endDataPtr++;
			std::construct_at<DataType, DataType>(&back(), static_cast<DataType&&>(value));
		}

		constexpr void popBack() {
			if (isEmpty()) { return; }
			std::destroy_at<DataType>(endDataPtr);
			endDataPtr--;
		}

		constexpr void append(const DynamicArray& src) noexcept {
			if (src.isEmpty()) { return; }
			const size_type newSize = size() + src.size();
			reserve(newSize);

			uninitializedCopyCountNoOverlap<const_pointer, pointer>(src.beginDataPtr, endDataPtr, src.size());
			endDataPtr = beginDataPtr + newSize;
		}

		constexpr void append(const DataType* srcPtr, const size_type srcSize) noexcept {
			if (!srcPtr || srcSize == 0) { return; }
			const size_type newSize = size() + srcSize;
			reserve(newSize);

			uninitializedCopyCountNoOverlap<const_pointer, pointer>(srcPtr, endDataPtr, srcSize);
			endDataPtr = beginDataPtr + newSize;
		}

		constexpr void append(std::initializer_list<DataType>& dataType) noexcept {
			if (dataType.size() == 0) { return; }
			const size_type newSize = size() + dataType.size();
			reserve(newSize);

			uninitializedCopyCountNoOverlap<const_pointer, pointer>(dataType.begin(), endDataPtr, dataType.size());
			endDataPtr = beginDataPtr + newSize;
		}

		constexpr void fill(const DataType&& value) noexcept {
			if (isEmpty()) { return; }
			fillCount<pointer, DataType>(data(), value, size());
		}

		constexpr DynamicArray& operator=(const DynamicArray& src) noexcept {
			deconstructAllData();
			freeDataVerify();

			beginDataPtr = nullptr;
			endDataPtr = nullptr;
			inCapacity = 0;

			if (src.isNotEmpty()) {
				reserve(src.capacity());
				uninitializedCopyCountNoOverlap<const_pointer, pointer>(src.beginDataPtr, beginDataPtr, src.size());
				endDataPtr = beginDataPtr + src.size();
			}

			return *this;
		}

		constexpr DynamicArray& operator=(DynamicArray&& src) noexcept {
			deconstructAllData();
			freeDataVerify();

			beginDataPtr = src.beginDataPtr; endDataPtr = src.endDataPtr; inCapacity = src.inCapacity;
			src.beginDataPtr = nullptr; src.endDataPtr = nullptr; src.inCapacity = 0;

			return *this;
		}

		constexpr DynamicArray& operator=(std::initializer_list<DataType> src) noexcept {
			deconstructAllData();
			freeDataVerify();

			beginDataPtr = nullptr;
			endDataPtr = nullptr;
			inCapacity = 0;

			if (src.size() > 0) {
				this->reserve(src.size());
				uninitializedCopyCountNoOverlap<const_pointer, pointer>(src.begin(), beginDataPtr, src.size());
				endDataPtr = beginDataPtr + src.size();
			}

			return *this;
		}

	private:
		constexpr void deconstructAllData() noexcept {
			if constexpr (!std::is_trivially_destructible_v<DataType>) {
				defualtDeconstructAll<DataType>(beginDataPtr, size());
			}
		}
		constexpr void freeData() noexcept {
			this->template free<DataType>(beginDataPtr);
		}
		constexpr void freeDataVerify() noexcept {
			if (beginDataPtr != nullptr) {
				this->template free<DataType>(beginDataPtr);
			}
		}
	};

}