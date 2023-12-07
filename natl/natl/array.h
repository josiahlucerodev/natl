#pragma once 

//std
#include <initializer_list>

//own
#include "typeTraits.h"
#include "iterators.h"
#include "container.h"
#include "option.h"
#include "dataMovement.h"
#include "allocator.h"
#include "algorithm.h"
#include "peramaterPackOperations.h"

//interface
namespace natl {
	template<class DataType, Size Number>
	class Array {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using optional_pointer = Option<DataType*>;
		using optional_const_pointer = Option<const DataType*>;
		using difference_type = PtrDiff;
		using size_type = Size;

		using iterator = RandomAccessIterator<DataType>;
		using const_iterator = RandomAccessIterator<const DataType>;
		using reverse_iterator = ReverseRandomAccessIterator<DataType>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const DataType>;

		using Alloc = DefaultAllocator<DataType>;

		//movement info  
		constexpr static bool triviallyRelocatable = IsTriviallyRelocatable<DataType>;
		constexpr static bool triviallyDefaultConstructible = IsTriviallyDefaultConstructible<DataType>;
		constexpr static bool triviallyCompareable = IsTriviallyCompareable<DataType>;
		constexpr static bool triviallyDestructible = IsTriviallyDestructible<DataType>;
	private:
		DataType dataStorage[Number];
	public:
		constexpr Array() = default;
		constexpr Array(const Array&) noexcept = default;
		constexpr Array(Array&&) noexcept = default;
		constexpr Array(std::initializer_list<DataType> ilist) noexcept {
			uninitializedCopyCountNoOverlap<const_pointer, pointer>(ilist.begin(), beginPtr(), min<Size>(ilist.size(), size()));
		};

		//destructor
		constexpr ~Array() = default;

		//util
		constexpr Array& self() noexcept { return *this; }
		constexpr const Array& self() const noexcept { return *this; }

		//assignment 
		constexpr Array& operator=(const Array&) noexcept = default;
		constexpr Array& operator=(Array&&) noexcept = default;
		constexpr Array& operator=(std::initializer_list<DataType> ilist) noexcept {
			copyCountNoOverlap<const_pointer, pointer>(ilist.begin(), data(), min<Size>(ilist.size(), size()));
			return self();
		}

		//element access
		constexpr reference at(const size_type index) noexcept { return data()[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return data()[index]; };

		constexpr reference operator[](const size_type index) noexcept { return data()[index]; }
		constexpr const_reference operator[](const size_type index) const noexcept { return data()[index]; }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return Number ? Number - 1 : 0; }

		constexpr reference front() noexcept { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr pointer data() noexcept { 
			return dataStorage; 
		}
		constexpr const_pointer data() const noexcept { 
			return dataStorage;
		};

		//iterators
		constexpr pointer beginPtr() noexcept { return data(); }
		constexpr const_pointer beginPtr() const noexcept { return data(); }
		constexpr pointer endPtr() noexcept { return data() + Number; }
		constexpr const_pointer endPtr() const noexcept { return data() + Number; }

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

		//capacity
		constexpr bool isEmpty() const noexcept { return !bool(Number); }
		constexpr bool isNotEmpty() const noexcept { return !isEmpty(); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		consteval static Size size() noexcept { return Number; }
		consteval static size_type max_size() noexcept { return size(); }

		//operations
		constexpr void fill(const DataType& value) noexcept {
			fillCount<pointer, DataType>(data(), value, size());
		}
		constexpr void swap(Array& other) noexcept {
			swap<Array>(self(), other);
		}
	};

	template<class DataType, Size... Dimensions>
	class MDArray {
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using optional_pointer = Option<DataType*>;
		using optional_const_pointer = Option<const DataType*>;
		using difference_type = PtrDiff;
		using size_type = Size;

		//movement info  
		constexpr static bool triviallyRelocatable = IsTriviallyRelocatable<DataType>;
		constexpr static bool triviallyDefaultConstructible = IsTriviallyDefaultConstructible<DataType>;
		constexpr static bool triviallyCompareable = IsTriviallyCompareable<DataType>;
		constexpr static bool triviallyDestructible = IsTriviallyDestructible<DataType>;
	private:
		using BaseArray = Array<DataType, multiplySizes(Dimensions...)>;
		BaseArray dataArray;
	public:
		//constructor
		constexpr MDArray() noexcept = default;
		constexpr MDArray(const MDArray&) noexcept = default;
		constexpr MDArray(MDArray&&) noexcept = default;
		constexpr MDArray(std::initializer_list<DataType> ilist) noexcept requires(isConst<DataType>) : dataArray(ilist) {};
	
		//util
		constexpr MDArray& self() noexcept { return *this; }
		constexpr const MDArray& self() const noexcept { return *this; }

		constexpr BaseArray& asArray() noexcept { return dataArray; }
		constexpr const BaseArray& asArray() const noexcept { dataArray; }

		//assignment 
		constexpr MDArray& operator=(const MDArray&) noexcept = default;
		constexpr MDArray& operator=(MDArray&&) noexcept = default;
		constexpr MDArray& operator=(std::initializer_list<DataType> ilist) noexcept {
			asArray() = ilist;
			return self();
		}

	public:
		//index calculation
		constexpr Size calculateLinearIndex(AlwaysType<Dimensions, Size>... indexesArgs) const noexcept {
			constexpr Size dimensions[] = { Dimensions... };
			Size indexes[] = { indexesArgs... };

			Size index = 0;
			Size mul = 1;

			for (Size i = 0; i != sizeof...(Dimensions); ++i) {
				index += indexes[i] * mul;
				mul *= dimensions[i];
			}
			return index;
		}
		constexpr Size calculateLinearIndexBounded(AlwaysType<Dimensions, Size>... indexesArgs) const noexcept {
			constexpr Size dimensions[] = { Dimensions... };
			const Size indexes[] = { indexesArgs... };
			Size index = 0;
			Size mul = 1;

			for (Size i = 0; i != sizeof...(Dimensions); ++i) {
				index += min<Size>(indexes[i], dimensions[i] - 1) * mul;
				mul *= dimensions[i];
			}
			return index;
		}

		//element access
		constexpr reference at(AlwaysType<Dimensions, Size>... indices) noexcept { return data()[calculateLinearIndex(indices...)]; }
		constexpr const_reference at(AlwaysType<Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndex(indices...)]; }

		constexpr reference atBounded(AlwaysType<Dimensions, Size>... indices) noexcept { return data()[calculateLinearIndexBounded(indices...)]; }
		constexpr const_reference atBounded(AlwaysType<Dimensions, Size>... indices) const noexcept { return data()[calculateLinearIndexBounded(indices...)]; }

		constexpr pointer data() noexcept { return asArray().data(); }
		constexpr const_pointer data() const noexcept {  return asArray().data(); };

		//capacity 
		constexpr bool isEmpty() const noexcept { return !bool(BaseArray::size()); }
		constexpr bool isNotEmpty() const noexcept { return !isEmpty(); }
		constexpr operator bool() const noexcept { return isNotEmpty(); }

		consteval static Size numberOfDimension() noexcept {
			return sizeof...(Dimensions);
		}
		template<Size dimensionIndex>
		consteval static Size dimensionSize() noexcept {
			static_assert(dimensionIndex < numberOfDimension(), "natl: MDArray dimensionSize() call | error: dimensionIndex >= numberOfDimension");
			constexpr Size dimensions[] = { Dimensions... };
			return dimensions[dimensionIndex];
		}
		consteval static Size size() noexcept {
			return BaseArray::size();
		}

		//operations
		constexpr void fill(const DataType& value) noexcept {
			asArray().fill(value);
		}
		constexpr void swap(MDArray& other) noexcept {
			asArray().swap(asArray(), other);
		}
	};

}