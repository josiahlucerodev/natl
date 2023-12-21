#pragma once 

#include "allocator.h"
#include "container.h"
#include "arrayView.h"
#include "dataMovement.h"

//interface 
namespace natl {

	template<class DataType, class Alloc = DefaultAllocator<DataType>>
		requires(IsAllocator<Alloc>)
	class HeapArray {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using optional_pointer = Option<DataType*>;
		using optional_const_pointer = Option<const DataType*>;
		using difference_type = std::ptrdiff_t;
		using size_type = Size;

		using iterator = RandomAccessIterator<DataType>;
		using const_iterator = RandomAccessIterator<const DataType>;
		using reverse_iterator = ReverseRandomAccessIterator<DataType>;
		using const_reverse_iterator = ReverseRandomAccessIterator<const DataType>;

		//movement info  
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = true;
		constexpr static bool triviallyDestructible = true;
	private:
		DataType* arrayDataPtr;
		Size arraySize;
	public:
		//constructor
		constexpr HeapArray() : arrayDataPtr(nullptr), arraySize(0) {}
		constexpr HeapArray(const Size count) : arrayDataPtr(Alloc::allocate(count)), arraySize(count) {}
		constexpr HeapArray(const Size count, const DataType& value) : arrayDataPtr(Alloc::allocate(count)), arraySize(count) {
			uninitializedFill(value);
		}
		constexpr HeapArray(const DataType* srcPtr, const Size count) : arrayDataPtr(Alloc::allocate(count)), arraySize(count) {
			DataType* copyDstPtr = arrayDataPtr;
			const DataType* copySrcPtrFirst = srcPtr;
			const DataType* copySrcPtrLast = copySrcPtrFirst + count;
			natl::uninitializedCopyNoOverlap<const DataType*, DataType*>(copySrcPtrFirst, copySrcPtrLast, copyDstPtr);
		}

		//destructor
		constexpr ~HeapArray() noexcept = default;
		
		//util 
		constexpr HeapArray& self() noexcept { return *this; }
		constexpr const HeapArray& self() const noexcept { return *this; }

		//convert 
		constexpr operator ArrayView<DataType>() noexcept {
			return ArrayView<DataType>(data(), size());
		}
		constexpr operator ArrayView<const DataType>() const noexcept {
			return ArrayView<const DataType>(data(), size());
		}

		constexpr ArrayView<DataType> toArrayView() noexcept {
			return ArrayView<DataType>(data(), size());
		}
		constexpr ArrayView<const DataType> toArrayView() const noexcept {
			return ArrayView<const DataType>(data(), size());
		}

		//capacity 
		constexpr bool empty() const noexcept { return size() == 0; }
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return !empty(); }

		constexpr Size size() const noexcept { return arraySize; }

		//element access
		constexpr reference at(const size_type index) noexcept { return data()[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return data()[index]; };

		constexpr reference operator[] (const size_type index) { return at(index); }
		constexpr const_reference operator[] (const size_type index) const { return at(index); }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return size() - 1; }

		constexpr reference front() noexcept { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr DataType* data() noexcept { return arrayDataPtr; }
		constexpr const DataType* data() const noexcept { return arrayDataPtr; }

		//iterators 
		constexpr pointer beginPtr() noexcept { return data(); }
		constexpr const_pointer beginPtr() const noexcept { return data(); }
		constexpr pointer endPtr() noexcept { return data() + size(); }
		constexpr const_pointer endPtr() const noexcept { return data() + size(); }

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

		//special element access
		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp<Size>(index, frontIndex(), backIndex()); }

		constexpr reference atClamped(const size_type index) noexcept requires(isNotConst<DataType>) { return at(clampIndex(index)); }
		constexpr const_pointer atClamped(const size_type index) const noexcept { return at(clampIndex(index)); }

		optional_pointer optionalAt(const size_type index) {
			if (notHave(index)) { return optional_pointer(); }
			return optional_pointer(at(index));
		};
		optional_const_pointer optionalAt(const size_type index) const {
			if (notHave(index)) { return optional_const_pointer(); }
			return optional_const_pointer(at(index));
		};

		optional_pointer optionalFront() { return optionalAt(frontIndex()); }
		optional_const_pointer optionalFront() const { return optionalAt(frontIndex()); }
		optional_pointer optionalBack() { return optionalAt(backIndex()); }
		optional_const_pointer optionalBack() const { return optionalAt(backIndex()); }

		constexpr bool has(const size_type index) const noexcept { return index < size(); }
		constexpr bool notHave(const size_type index) const noexcept { return index >= size(); }

		//allocation 
		constexpr void deallocate() noexcept {
			if (arrayDataPtr) {
				Alloc::deallocate(arrayDataPtr, size());
			}
		}
		constexpr void allocate(const Size count) noexcept {
			deallocate();
			arrayDataPtr = Alloc::allocate(count);
			arraySize = count;
		}

		constexpr void allocate(const Size count, const DataType& value) noexcept {
			deallocate();
			arrayDataPtr = Alloc::allocate(count);
			arraySize = count;
			uninitializedFill(value);
		}
		constexpr void allocate(const DataType* srcPtr, const Size count) noexcept {
			deallocate();
			arrayDataPtr = Alloc::allocate(count);
			arraySize = count;

			DataType* copyDstPtr = arrayDataPtr;
			const DataType* copySrcPtrFirst = srcPtr;
			const DataType* copySrcPtrLast = copySrcPtrFirst + count;
			natl::uninitializedCopyNoOverlap<const DataType*, DataType*>(copySrcPtrFirst, copySrcPtrLast, copyDstPtr);
		}

		//modifier 
		constexpr void constructAll() noexcept {
			if (isNotEmpty()) {
				defualtConstructAll(arrayDataPtr, size());
			}
		}
		constexpr void destructAll() noexcept {
			if (!IsTriviallyDestructible<DataType>) {
				if (!std::is_constant_evaluated()) {
					return;
				}
			}

			if (isNotEmpty()) {
				defualtDeconstructAll(arrayDataPtr, size());
			}
		}

		constexpr void fill(const DataType& value = DataType()) noexcept {
			DataType* fillDstFirst = data();
			DataType* fillDstLast = fillDstFirst + size();
			natl::fill<DataType*, DataType>(fillDstFirst, fillDstLast, value);
		}
		constexpr void uninitializedFill(const DataType& value = DataType()) noexcept {
			DataType* fillDstFirst = data();
			DataType* fillDstLast = fillDstFirst + size();
			natl::uninitializedFill<DataType*, DataType>(fillDstFirst, fillDstLast, value);
		}
	};
}