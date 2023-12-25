#pragma once 

//own
#include "allocator.h"
#include "container.h"
#include "iterators.h"
#include "arrayView.h"
#include "hash.h"

//interface 
namespace natl {
	template<class DataType, Size bufferSize, class Alloc = DefaultAllocator<DataType>>
		requires(IsAllocator<Alloc>)
	class SmallDynArray {
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

		using container_allocation_move_adapater = AllocationMoveAdapater<DataType, Alloc>;

		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;

		constexpr static bool enableSmallArray = true;
	private:
		Size arraySizeAndSmallArrayFlag;
		DataType* arrayDataPtr;
		Size arrayCapacity;
		DataType smallArrayStorage[bufferSize];

		//small array 
	private:
		constexpr static Size setNthBitToZero(const Size num, const Size n) noexcept {
			const Size mask = ~(Size(1) << n);
			const Size result = num & mask;
			return result;
		}
		constexpr static Size setNthBitToOne(const Size num, const Size n) {
			const Size mask = Size(1) << n;
			const Size result = num | mask;
			return result;
		}

		constexpr void setAsSmallArray() noexcept {
			if (enableSmallArray) {
				arraySizeAndSmallArrayFlag = setNthBitToZero(arraySizeAndSmallArrayFlag, 63);
			}
		}
		constexpr void setAsNotSmallArray() noexcept {
			if (enableSmallArray) {
				arraySizeAndSmallArrayFlag = setNthBitToOne(arraySizeAndSmallArrayFlag, 63);
			}
		}
		constexpr void setSize(const Size newSize) {
			arraySizeAndSmallArrayFlag = (arraySizeAndSmallArrayFlag & ~0x7FFFFFFFFFFFFFFFULL) | newSize;
		}
	public:
		constexpr bool isSmallArray() const noexcept {
			if (!enableSmallArray) {
				return false;
			} else {
				return !(arraySizeAndSmallArrayFlag & ~0x7FFFFFFFFFFFFFFFULL);
			}
		}
		constexpr bool isNotSmallArray() const noexcept {
			return !isSmallArray();
		}
		constexpr static Size smallArrayCapacity() noexcept {
			if (!enableSmallArray) {
				return 0;
			} else {
				return bufferSize;
			}
		}

		constexpr const DataType* smallArrayLocation() const noexcept {
			if (!enableSmallArray) {
				return nullptr;
			} else {
				return smallArrayStorage;
			}
		}
		constexpr DataType* smallArrayLocation() noexcept {
			if (!enableSmallArray) {
				return nullptr;
			} else {
				return smallArrayStorage;
			}
		}

		constexpr Size capacity() const noexcept { return  isSmallArray() ? smallArrayCapacity() : arrayCapacity; };
		constexpr Size size() const noexcept { return arraySizeAndSmallArrayFlag & 0x7FFFFFFFFFFFFFFFULL; }
		constexpr size_type max_size() const noexcept { return 0x7FFFFFFFFFFFFFFFULL; };

		constexpr const DataType* data() const noexcept {
			return isSmallArray() ? smallArrayLocation() : arrayDataPtr;
		}
		constexpr DataType* data() noexcept {
			return isSmallArray() ? smallArrayLocation() : arrayDataPtr;
		}

	public:
		constexpr void baseConstructorInit() noexcept {
			arraySizeAndSmallArrayFlag = 0;
			arrayCapacity = 0;
			arrayDataPtr = nullptr;

			if (std::is_constant_evaluated()) {
				uninitializedFill<DataType*, DataType>(smallArrayStorage, smallArrayStorage + bufferSize, DataType());
			}
		}
		
		//constructors
		constexpr SmallDynArray() noexcept {
			baseConstructorInit();
		}
		constexpr SmallDynArray(const DataType* srcPtr, const Size& count) noexcept {
			baseConstructorInit();
			construct(srcPtr, count);
		}
		constexpr SmallDynArray(const SmallDynArray& other) noexcept {
			baseConstructorInit();
			construct(other.data(), other.size());
		}
		constexpr SmallDynArray(SmallDynArray&& other) noexcept {
			construct(forward<SmallDynArray>(other));
		}
		constexpr SmallDynArray(const size_type count, const DataType& value = DataType()) noexcept {
			baseConstructorInit();
			construct(count, value);
		}
		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, DataType>)
		constexpr SmallDynArray(Iter first, Iter last) noexcept {
			baseConstructorInit();
			construct<Iter>(first, last);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		constexpr SmallDynArray(const ArrayViewLike& arrayViewLike) noexcept {
			baseConstructorInit();
			construct(arrayViewLike.data(), arrayViewLike.size());
		}
		constexpr SmallDynArray(const container_allocation_move_adapater& allocationMoveAdapater) noexcept {
			baseConstructorInit();
			construct(allocationMoveAdapater);
		}
		constexpr SmallDynArray(std::initializer_list<DataType> ilist) noexcept {
			baseConstructorInit();
			construct(ilist.begin(), ilist.size());
		}

		//destructor 
		constexpr ~SmallDynArray() {
			if (isNotSmallArray() && arrayDataPtr) {
				Alloc::deallocate(arrayDataPtr, capacity());
			}
		}

		//util 
		constexpr SmallDynArray& self() noexcept { return *this; }
		constexpr const SmallDynArray& self() const noexcept { return *this; }

		//construct
	private:
		constexpr SmallDynArray& construct(const DataType* otherPtr, const Size& count) noexcept {
			if (count == 0) {
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else if (count < smallArrayCapacity()) {
				const DataType* srcDataPtrFirst = otherPtr;
				const DataType* srcDataPtrLast = srcDataPtrFirst + count;
				uninitializedCopyNoOverlap<const DataType*, DataType*>(srcDataPtrFirst, srcDataPtrLast, smallArrayLocation());

				setAsSmallArray();

				const Size newSize = count;
				setSize(newSize);

				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else {
				const Size newSize = count;
				reserve(newSize);

				const DataType* srcDataPtrFirst = otherPtr;
				const DataType* srcDataPtrLast = srcDataPtrFirst + count;
				uninitializedCopyNoOverlap<const DataType*, DataType*>(srcDataPtrFirst, srcDataPtrLast, data());

				setSize(newSize);
			}

			return self();
		}
		constexpr SmallDynArray& construct(SmallDynArray&& other) noexcept {
			if (other.isEmpty()) {
				arraySizeAndSmallArrayFlag = other.arraySizeAndSmallArrayFlag;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else if (other.isSmallArray()) {
				const DataType* srcDataPtrFirst = other.data();
				const DataType* srcDataPtrLast = srcDataPtrFirst + other.size();
				uninitializedCopyNoOverlap<const DataType*, DataType*>(srcDataPtrFirst, srcDataPtrLast, smallArrayLocation());

				arraySizeAndSmallArrayFlag = other.arraySizeAndSmallArrayFlag;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else {
				arraySizeAndSmallArrayFlag = other.arraySizeAndSmallArrayFlag;
				arrayCapacity = other.arrayCapacity;
				arrayDataPtr = other.arrayDataPtr;
			}

			other.arraySizeAndSmallArrayFlag = 0;
			other.arrayCapacity = 0;
			other.arrayDataPtr = nullptr;

			return self();
		}
		constexpr SmallDynArray& construct(const size_type count, const DataType& value = DataType()) noexcept {
			if (count == 0) {
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
				return self();
			}

			reserve(count);

			DataType* fillDstPtr = data();
			DataType* fillDstPtrLast = fillDstPtr + count;
			uninitializedFill<DataType*, DataType>(fillDstPtr, fillDstPtrLast, value);

			setSize(count);
			return self();
		}

		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, DataType>)
		constexpr SmallDynArray& construct(Iter first, Iter last) noexcept {
			if constexpr (std::contiguous_iterator<Iter>) {
				const Size count = iterDistance<Iter>(first, last);
				const DataType* firstPtr = iteratorToAddress<Iter>(first);
				return construct(firstPtr, count);
			}

			reserve(10);
			for (; first != last; first++) {
				push_back(*first);
			}
			return self();
		}

		constexpr SmallDynArray& construct(const container_allocation_move_adapater& allocationMoveAdapater) noexcept {
			if (allocationMoveAdapater.isEmpty()) {
				arraySizeAndSmallArrayFlag = 0;
			} else if (allocationMoveAdapater.requiresCopy() || allocationMoveAdapater.size() < smallArrayCapacity()) {
				construct(allocationMoveAdapater.data(), allocationMoveAdapater.size());
				if (allocationMoveAdapater.canDealloc()) {
					allocationMoveAdapater.deallocate();
				}
			} else {
				arrayCapacity = allocationMoveAdapater.capacity();
				arrayDataPtr = allocationMoveAdapater.data();
				setSize(allocationMoveAdapater.size());
				setAsNotSmallArray();
			}
			return self();
		}

	public:

		//assignment 
		constexpr SmallDynArray& operator=(const SmallDynArray& other) noexcept {
			return assign(other);
		}
		constexpr SmallDynArray& operator=(SmallDynArray&& other) noexcept {
			return assign(forward<SmallDynArray>(other));
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		constexpr SmallDynArray& operator=(const ArrayViewLike& arrayViewLike) noexcept {
			return assign<ArrayViewLike>(arrayViewLike);
		}
		constexpr SmallDynArray& operator=(const container_allocation_move_adapater& allocationMoveAdapater) noexcept {
			return assign(allocationMoveAdapater);
		}
		constexpr SmallDynArray& operator=(std::initializer_list<DataType> ilist) noexcept {
			return assign(ilist);
		}

		constexpr SmallDynArray& assign(const DataType* otherPtr, const Size& count) noexcept {
			release();

			if (count == 0) {
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else if(count < smallArrayCapacity()) {
				const DataType* srcDataPtrFirst = otherPtr;
				const DataType* srcDataPtrLast = srcDataPtrFirst + count;
				internalCopyNoOverlap(srcDataPtrFirst, srcDataPtrLast, smallArrayLocation());

				setAsSmallArray();

				const Size newSize = count;
				setSize(newSize);

				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else {
				const Size newSize = count;
				reserve(newSize);

				const DataType* srcDataPtrFirst = otherPtr;
				const DataType* srcDataPtrLast = srcDataPtrFirst + count;
				internalCopyNoOverlap(srcDataPtrFirst, srcDataPtrLast, data());

				setSize(newSize);
			}

			return self();
		}
		constexpr SmallDynArray& assign(const SmallDynArray& other) noexcept {
			return assign(other.data(), other.size());
		}
		constexpr SmallDynArray& assign(SmallDynArray&& other) noexcept {
			release();

			if (other.isEmpty()) {
				arraySizeAndSmallArrayFlag = other.arraySizeAndSmallArrayFlag;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
				
			} else if (other.isSmallArray()) {
				const DataType* srcDataPtrFirst = other.data();
				const DataType* srcDataPtrLast = srcDataPtrFirst + other.size();
				internalCopyNoOverlap(srcDataPtrFirst, srcDataPtrLast, smallArrayLocation());

				arraySizeAndSmallArrayFlag = other.arraySizeAndSmallArrayFlag;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else {
				arraySizeAndSmallArrayFlag = other.arraySizeAndSmallArrayFlag;
				arrayCapacity = other.arrayCapacity;
				arrayDataPtr = other.arrayDataPtr;
			}

			other.arraySizeAndSmallArrayFlag = 0;
			other.arrayCapacity = 0;
			other.arrayDataPtr = nullptr;

			return self();
		}
		constexpr SmallDynArray& assign(const size_type count, const DataType& value = DataType()) noexcept {
			release();

			if (count == 0) { 
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
				return self(); 
			} 

			reserve(count);

			if constexpr (IsTriviallyDestructible<DataType>) {
				internalFill(data(), data() + count, value);
			} else {
				if (count > size()) {
					Size uninitializedFillAmount = count - size();
					DataType* fillDstPtr = data() + size();
					DataType* fillDstPtrLast = fillDstPtr + uninitializedFillAmount;
					uninitializedFill<DataType*, DataType>(fillDstPtr, fillDstPtrLast, value);
				}

				internalFill(data(), data() + size(), value);
			}

			setSize(count);
			return self();
		}
		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, DataType>)
		constexpr SmallDynArray& assign(Iter first, Iter last) noexcept {
			if constexpr (std::contiguous_iterator<Iter>) {
				const Size count = iterDistance<Iter>(first, last);
				const DataType* firstPtr = iteratorToAddress<Iter>(first);
				return assign(firstPtr, count);
			}

			resize(0);
			for (; first != last; first++) {
				push_back(*first);
			}
			return self();
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		constexpr SmallDynArray& assign(const ArrayViewLike& arrayViewLike) noexcept {
			return assign(arrayViewLike.data(), arrayViewLike.size());
		}
		constexpr SmallDynArray& assign(const container_allocation_move_adapater& allocationMoveAdapater) noexcept {
			if (allocationMoveAdapater.isEmpty()) {
				release();
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else if (allocationMoveAdapater.requiresCopy() || allocationMoveAdapater.size() < smallArrayCapacity()) {
				assign(allocationMoveAdapater.data(), allocationMoveAdapater.size());
				if (allocationMoveAdapater.canDealloc()) {
					allocationMoveAdapater.deallocate();
				}
			} else {
				if (isNotSmallArray()) {
					release();
				}

				arrayCapacity = allocationMoveAdapater.capacity();
				arrayDataPtr = allocationMoveAdapater.data();
				setSize(allocationMoveAdapater.size());
				setAsNotSmallArray();
			}
			return self();
		}
		constexpr SmallDynArray& assign(std::initializer_list<DataType> ilist) noexcept {
			return assign(ilist.begin(), ilist.size());
		}

		//strongly linked methods 
		[[nodiscard]] constexpr container_allocation_move_adapater getAlloctionMoveAdapater() noexcept {
			const bool arrayIsSmallBuffer = isSmallArray();
			AllocationMoveAdapaterRequireCopy requireCopy = arrayIsSmallBuffer ? AllocationMoveAdapaterRequireCopy::v_true : AllocationMoveAdapaterRequireCopy::v_false;
			AllocationMoveAdapaterCanDealloc canBeDealloc = arrayIsSmallBuffer ? AllocationMoveAdapaterCanDealloc::v_false : AllocationMoveAdapaterCanDealloc::v_true;
			container_allocation_move_adapater allocationMoveAdapater(data(), size(), capacity(), requireCopy, canBeDealloc);
			arrayDataPtr = nullptr;
			arraySizeAndSmallArrayFlag = 0;
			arrayCapacity = 0;
			return allocationMoveAdapater;
		}

		constexpr void resize(const size_type count) noexcept {
			if (count < size()) {
				if constexpr (!IsTriviallyDestructible<DataType>) {
					defualtDeconstructAll<DataType>(data() + count, size() - count);
					setSize(count);
					return;
				} else {
					if (std::is_constant_evaluated()) {
						defualtDeconstructAll<DataType>(data() + count, size() - count);
					}
					setSize(count);
				}
			} else if (count > size()) {
				insert(cend(), count, DataType());
			}
		}

		constexpr void resize(const size_type count, const DataType value) noexcept {
			if (count < size()) {
				if constexpr (!IsTriviallyDestructible<DataType> || std::is_constant_evaluated()) {
					defualtDeconstructAll<DataType>(data() + count, size() - count);
				}
				setSize(count);
			} else if (count > size()) {
				insert(cend(), count, value);
			}
		}

		constexpr void reserve(Size newCapacity) noexcept {
			if (newCapacity <= capacity()) {
				return;
			} else if (newCapacity < smallArrayCapacity()) {
				setAsSmallArray();
				return;
			}

			newCapacity *= 2;

			if (newCapacity < capacity()) { return; }

			DataType* newDataPtr = Alloc::allocate(newCapacity);

			if (data()) {
				const DataType* srcDataPtrFirst = data();
				const DataType* srcDataPtrLast = srcDataPtrFirst + size();
				uninitializedCopyNoOverlap<const DataType*, DataType*>(srcDataPtrFirst, srcDataPtrLast, newDataPtr);

				if constexpr (!IsTriviallyRelocatable<DataType>) {
					defualtDeconstructAll<DataType>(data(), size());
				} else {
					if (std::is_constant_evaluated()) {
						defualtDeconstructAll<DataType>(data(), size());
					}
				}
			}
			if (arrayDataPtr) {
				Alloc::deallocate(arrayDataPtr, capacity());
			}

			arrayDataPtr = newDataPtr;
			arrayCapacity = newCapacity;
			setAsNotSmallArray();
		}

		constexpr void shrink_to_fit() noexcept {
			const Size newCapacity = size();
			const bool sameSize = newCapacity == capacity();
			const bool canBeSmallArray = newCapacity < smallArrayCapacity();

			if (isSmallArray()) {
				return;
			} else if(isNotSmallArray() && canBeSmallArray) {
				if (!arrayDataPtr) {
					arraySizeAndSmallArrayFlag = 0;
					arrayCapacity = 0;
					arrayDataPtr = nullptr;
					return;
				}

				DataType* dstDataPtr = smallArrayLocation();
				const DataType* srcDataPtrFirst = arrayDataPtr;
				const DataType* srcDataPtrLast = arrayDataPtr + newCapacity;
				uninitializedCopyNoOverlap<const DataType*, DataType*>(srcDataPtrFirst, srcDataPtrLast, dstDataPtr);

				Alloc::deallocate(arrayDataPtr, capacity());
				arrayCapacity = 0;
				arrayDataPtr = nullptr;

				setAsSmallArray();
			} else if (sameSize) {
				return;
			}


			DataType* newDataPtr = Alloc::allocate(newCapacity);
			const DataType* srcDataPtrFirst = data();
			const DataType* srcDataPtrLast = srcDataPtrFirst + size();
			uninitializedCopyNoOverlap<const DataType*, DataType*>(srcDataPtrFirst, srcDataPtrLast, newDataPtr);

			if (!IsTriviallyRelocatable<DataType> || std::is_constant_evaluated()) {
				defualtDeconstructAll<DataType>(data(), size());
			}

			Alloc::deallocate(arrayDataPtr, capacity());

			arrayDataPtr = newDataPtr;
			arrayCapacity = newCapacity;
			setAsNotSmallArray();
		}

		constexpr void swap(SmallDynArray& other) noexcept {
			swap<SmallDynArray>(self(), other);
		}

		constexpr void clear() noexcept {
			if (!IsTriviallyDestructible<DataType> || std::is_constant_evaluated()) {
				defualtDeconstructAll<DataType>(data(), size());
			}
			setSize(0);
		}

	private:
		constexpr void release() noexcept {
			if (arrayDataPtr) {
				if constexpr (!IsTriviallyDestructible<DataType>) {
					defualtDeconstructAll<DataType>(data(), size());
				}

				Alloc::deallocate(arrayDataPtr, capacity());
				arrayDataPtr = nullptr;
			}
		}
	public:

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

		//capacity 
	public:
		constexpr bool empty() const noexcept { return size() == 0; }
		constexpr bool isEmpty() const noexcept { return empty(); }
		constexpr bool isNotEmpty() const noexcept { return !empty(); }

		//modifiers
	private:
		constexpr iterator constIteratorToIterator(const_iterator pos) {
			return iterator(data() + iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos));
		}
		constexpr Size shiftRelocateLeft(const Size index, const Size count) noexcept {
			const Size relocateCount = size() - index;
			if (relocateCount == 0) { return relocateCount; }

			if constexpr (IsTriviallyRelocatable<DataType>) {
				if (!std::is_constant_evaluated()) {
					DataType* relocateDstPtr = data() + index + count;
					const DataType* relocateSrcPtrFirst = data() + index;
					const DataType* relocateSrcPtrLast = relocateSrcPtrFirst + relocateCount;
					uninitializedCopy<const DataType*, DataType*>(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);
					return relocateCount;
				}
			}

			const Size uninitializedCount = count;
			const Size normalCount = relocateCount - uninitializedCount;

			DataType* relocateUninitializedDstPtr = data() + backIndex();
			const DataType* relocateUninitializedSrcPtrFirst = data() + index + normalCount;
			const DataType* relocateUninitializedSrcPtrLast = relocateUninitializedSrcPtrFirst + uninitializedCount;
			uninitializedCopy<const DataType*, DataType*>(relocateUninitializedSrcPtrFirst, relocateUninitializedSrcPtrLast, relocateUninitializedDstPtr);

			DataType* relocateNormalDstPtr = data() + index + count;
			const DataType* relocateNormalSrcPtrFirst = data() + index;
			const DataType* relocateNormalSrcPtrLast = relocateNormalSrcPtrFirst + normalCount;
			copy<const DataType*, DataType*>(relocateNormalSrcPtrFirst, relocateNormalSrcPtrLast, relocateNormalDstPtr);
			return relocateCount;
		}
	public:

		constexpr iterator insert(const_iterator pos, const DataType& value) noexcept {
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const Size newSize = size() + 1;
			reserve(newSize);

			shiftRelocateLeft(index, 1);
			set(index, value);

			setSize(newSize);
			return iterator(data() + index);

		}
		constexpr iterator insert(const_iterator pos, DataType&& value) noexcept {
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const Size newSize = size() + 1;
			reserve(newSize);

			shiftRelocateLeft(index, 1);

			set(index, forward<DataType>(value));

			setSize(newSize);
			return iterator(data() + index);
		}
		constexpr iterator insert(const_iterator pos, const DataType* srcPtr, const Size count) noexcept {
			if (count == 0) { return constIteratorToIterator(pos); }
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const Size newSize = size() + count;
			reserve(newSize);

			const Size relocateCount = shiftRelocateLeft(index, count);

			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<DataType>) {
					DataType* insertDstPtr = data() + index;
					const DataType* insertSrcPtrFirst = srcPtr;
					const DataType* insertSrcPtrLast = insertSrcPtrFirst + count;
					internalCopyNoOverlap(insertSrcPtrFirst, insertSrcPtrLast, insertDstPtr);

					setSize(newSize);
					return iterator(data() + index);
				}
			}

			const Size regularCopyCount = relocateCount;
			DataType* insertRegularDstPtr = data() + index;
			const DataType* insertRegularSrcPtrFirst = srcPtr;
			const DataType* insertRegularSrcPtrLast = insertRegularSrcPtrFirst + regularCopyCount;
			natl::copyCountNoOverlap<const DataType*, DataType*>(insertRegularSrcPtrFirst, insertRegularSrcPtrLast, insertRegularDstPtr);

			const Size uninilizedCopyCount = count - relocateCount;
			DataType* insertUninilizedDstPtr = data() + index + regularCopyCount;
			const DataType* insertUninilizedSrcPtrFirst = insertRegularSrcPtrLast;
			const DataType* insertUninilizedSrcPtrLast = insertUninilizedSrcPtrFirst + uninilizedCopyCount;
			natl::uninitializedCopyNoOverlap<const DataType*, DataType*>(insertUninilizedSrcPtrFirst, insertUninilizedSrcPtrLast, insertUninilizedDstPtr);

			setSize(newSize);
			return iterator(data() + index);
		}
		constexpr iterator insert(const_iterator pos, const size_type count, const DataType& value) {
			if (count == 0) { return constIteratorToIterator(pos); }
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const Size newSize = size() + count;
			reserve(newSize);

			const Size relocateCount = shiftRelocateLeft(index, count);

			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<DataType>) {
					DataType* fillDstPtrFirst = data() + index;
					DataType* fillDstPtrLast = fillDstPtrFirst + count;
					internalFill(fillDstPtrFirst, fillDstPtrLast, value);

					setSize(newSize);
					return iterator(data() + index);
				}
			}

			const Size regularFillCount = relocateCount;
			DataType* fillRegularDstPtrFirst = data() + index;
			DataType* fillRegularDstPtrLast = fillRegularDstPtrFirst + regularFillCount;
			natl::fill<DataType*, DataType>(fillRegularDstPtrFirst, fillRegularDstPtrLast, value);

			const Size uninilizedFillCount = count - relocateCount;
			DataType* fillUninilizedDstPtrFirst = fillRegularDstPtrLast;
			DataType* fillUninilizedDrcPtrLast = fillUninilizedDstPtrFirst + uninilizedFillCount;
			natl::uninitializedFill<DataType*, DataType>(fillUninilizedDstPtrFirst, fillUninilizedDrcPtrLast, value);

			setSize(newSize);
			return iterator(data() + index);
		}
		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, DataType>)
		constexpr iterator insert(const_iterator pos, Iter first, Iter last) {
			if constexpr (IsRandomAccessIterator<Iter>) {
				const Size count = iterDistance<Iter>(first, last);
				const DataType* firstPtr = iteratorToAddress<Iter>(first);
				insert(pos, firstPtr, count);
			}

			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const Size count = iterDistance<Iter>(first, last);
			const Size newSize = size() + count;
			reserve(newSize);

			const Size relocateCount = shiftRelocateLeft(index, count);

			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<DataType>) {
					DataType* insertDstPtr = data() + index;
					internalCopyNoOverlap<Iter>(first, last, insertDstPtr);

					setSize(newSize);
					return iterator(data() + index);
				}
			}

			Iter srcIter = first;
			DataType* dstPtr = data() + index;
			const Size regularCopyCount = relocateCount;
			const Size uninilizedCopyCount = count - relocateCount;
			for (Size i = 0; i < regularCopyCount; i++, dstPtr++, srcIter++) {
				*dstPtr = *srcIter;
			}
			for (Size i = 0; i < uninilizedCopyCount; i++, dstPtr++, srcIter++) {
				std::construct_at<DataType, DataType>(dstPtr, *srcIter);
			}

			setSize(newSize);
			return iterator(data() + index);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		constexpr iterator insert(const_iterator pos, const ArrayViewLike& arrayViewLike) noexcept {
			return insert(pos, arrayViewLike.data(), arrayViewLike.size());
		}
		constexpr iterator insert(const_iterator pos, std::initializer_list<DataType> ilist) noexcept {
			return insert(pos, ilist.begin(), ilist.size());
		}

		template<class... Args >
		constexpr iterator emplace(const_iterator pos, Args&&... args) {
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const Size newSize = size() + 1;
			reserve(newSize);

			shiftRelocateLeft(index, 1);

			std::construct_at<DataType, Args...>(&*pos, std::forward<Args>(args)...);

			setSize(newSize);
			return iterator(data() + index);
		}

		constexpr iterator erase(const_iterator pos) noexcept {
			const Size newSize = size() - 1;
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);


			DataType* relocateDstPtr = data() + index;
			const DataType* relocateSrcPtrFirst = data() + index + 1;
			const DataType* relocateSrcPtrLast = endPtr();
			internalCopy(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);

			if (!IsTriviallyDestructible<DataType> || std::is_constant_evaluated()) {
				defualtDeconstruct<DataType>(at(backIndex()));
			}

			setSize(newSize);
			return iterator(data() + index);
		}
		constexpr iterator erase(const_iterator first, const_iterator last) noexcept {
			const Size index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*first);
			const Size count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const Size newSize = size() - count;

			DataType* relocateDstPtr = data() + index;
			const DataType* relocateSrcPtrFirst = data() + index + count;
			const DataType* relocateSrcPtrLast = endPtr();
			internalCopy(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);

			if (!IsTriviallyDestructible<DataType> || std::is_constant_evaluated()) {
				defualtDeconstructAll<DataType>(data() + newSize, count);
			}

			setSize(newSize);
			return iterator(data() + index);
		}

		constexpr iterator eraseAtIndex(const std::size_t index) noexcept {
			return erase(begin() + index);
		}

		constexpr reference push_back(const DataType& value) noexcept {
			const Size index = size();
			const Size newSize = index + 1;
			reserve(newSize);
			return set(index, value);
		}
		constexpr reference push_back(DataType&& value) noexcept {
			const Size index = size();
			const Size newSize = index + 1;
			reserve(newSize);
			setSize(newSize);
			return set(index, forward<DataType>(value));;
		}

		template<class... Args >
		constexpr reference emplace_back(Args&&... args) noexcept {
			const Size index = size();
			const Size newSize = index + 1;
			reserve(newSize);
			setSize(newSize);
			reference value = at(index);
			std::construct_at<DataType, Args...>(value, std::forward<Args>(args)...);
			return value;
		}

		constexpr SmallDynArray& append(const DataType* srcPtr, const Size count) noexcept {
			const Size index = size();
			const Size newSize = index + 1;
			reserve(newSize);

			DataType* insertDstPtr = data() + index;
			const DataType* insertSrcPtrFirst = srcPtr;
			const DataType* insertSrcPtrLast = insertSrcPtrFirst + count;
			internalCopyNoOverlap(insertSrcPtrFirst, insertSrcPtrLast, insertDstPtr);

			setSize(newSize);
			return self();
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		constexpr SmallDynArray& append(const ArrayViewLike& arrayViewLike) noexcept {
			return append(arrayViewLike.data(), arrayViewLike.size());
		}
		template<class ArrayViewLikeConvertible>
			requires(std::is_convertible_v<ArrayViewLikeConvertible, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLikeConvertible, DataType>)
		constexpr SmallDynArray& append(const ArrayViewLikeConvertible& arrayViewLikeConvertible) noexcept {
			return append<ArrayView<const DataType>>(static_cast<ArrayView<const DataType>>(arrayViewLikeConvertible));
		}
		constexpr SmallDynArray& append(const std::initializer_list<DataType> ilist) noexcept {
			return append(ilist.begin(), ilist.end());
		}

		constexpr SmallDynArray& pop_back() noexcept {
			if (!IsTriviallyDestructible<DataType> || std::is_constant_evaluated()) {
				defualtDeconstruct<DataType>(at(backIndex()));
			}
			setSize(size() - 1);
			return;
		}

	private:
		constexpr reference set(const Size index, const DataType& value) noexcept {
			reference valueDst = at(index);
			if (IsTriviallyDestructible<DataType>) {
				valueDst = value;
			}
			else {
				DataType tempValue = value;
				std::construct_at<DataType, DataType>(&valueDst, forward<DataType>(tempValue));
			}
			return valueDst;
		}
		constexpr reference set(const Size index, DataType&& value) noexcept {
			reference valueDst = at(index);
			if (IsTriviallyDestructible<DataType>) {
				valueDst = value;
			}
			else {
				std::construct_at<DataType, DataType>(&valueDst, forward<DataType>(value));
			}
			return valueDst;
		}

		constexpr void internalCopyNoOverlap(const DataType* srcPtr, const DataType* srcPtrLast, DataType* dstPtr) noexcept {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<DataType>) {
					uninitializedCopyNoOverlap<const DataType*, DataType*>(srcPtr, srcPtrLast, dstPtr);
					return;
				}
			}

			copyNoOverlap<const DataType*, DataType*>(srcPtr, srcPtrLast, dstPtr);
		}
		template<class Iter>
		constexpr void internalCopyNoOverlap(Iter first, Iter last, DataType* dstPtr) {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<DataType>) {
					uninitializedCopyNoOverlap<Iter, DataType*>(first, last, dstPtr);
					return;
				}
			}
			copyNoOverlap<Iter, DataType*>(first, last, dstPtr);
		}

		constexpr void internalCopy(const DataType* srcPtr, const DataType* srcPtrLast, DataType* dstPtr) noexcept {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<DataType>) {
					uninitializedCopy<const DataType*, DataType*>(srcPtr, srcPtrLast, dstPtr);
					return;
				}
			}
			copy<const DataType*, DataType*>(srcPtr, srcPtrLast, dstPtr);
		}
		template<class Iter>
		constexpr void internalCopy(Iter first, Iter last, DataType* dstPtr) noexcept {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<DataType>) {
					uninitializedCopy<Iter, DataType*>(first, last, dstPtr);
					return;
				}
			}
			copy<Iter, DataType*>(first, last, dstPtr);
		}

		constexpr void internalFill(DataType* dstPtr, DataType* dstPtrLast, const DataType& value) noexcept {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<DataType>) {
					uninitializedFill<DataType*, DataType>(dstPtr, dstPtrLast, value);
					return;
				}
			}
			fill<DataType*, DataType>(dstPtr, dstPtrLast, value);
		}
	public:
		//compare operators 
		friend constexpr bool operator==(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() == rhs.toArrayView();
		}
		friend constexpr bool operator==(const SmallDynArray& lhs, const DataType rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator==(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator==(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		friend constexpr bool operator==(const SmallDynArray& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}

		friend constexpr bool operator!=(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() != rhs.toArrayView();
		}
		friend constexpr bool operator!=(const SmallDynArray& lhs, const DataType rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator!=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator!=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		friend constexpr bool operator!=(const SmallDynArray& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}

		friend constexpr bool operator<(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() < rhs.toArrayView();
		}
		friend constexpr bool operator<(const SmallDynArray& lhs, const DataType rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator<(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator<(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		friend constexpr bool operator<(const SmallDynArray& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}

		friend constexpr bool operator<=(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() <= rhs.toArrayView();
		}
		friend constexpr bool operator<=(const SmallDynArray& lhs, const DataType rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator<=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator<=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		friend constexpr bool operator<=(const SmallDynArray& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}

		friend constexpr bool operator>(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() > rhs.toArrayView();
		}
		friend constexpr bool operator>(const SmallDynArray& lhs, const DataType rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator>(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator>(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		friend constexpr bool operator>(const SmallDynArray& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}

		friend constexpr bool operator>=(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() >= rhs.toArrayView();
		}
		friend constexpr bool operator>=(const SmallDynArray& lhs, const DataType rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator>=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr bool operator>=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		friend constexpr bool operator>=(const SmallDynArray& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}

		friend constexpr std::strong_ordering operator<=>(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() <=> rhs.toArrayView();
		}
		friend constexpr std::strong_ordering operator<=>(const SmallDynArray& lhs, const DataType rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr std::strong_ordering operator<=>(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const DataType>> && !IsArrayViewLike<ArrayViewLike, const DataType>)
		friend constexpr std::strong_ordering operator<=>(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		friend constexpr bool operator<=>(const SmallDynArray& lhs, std::initializer_list<DataType> rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}

		//operations 
		constexpr SmallDynArray& reverse() noexcept {
			reverse<iterator>(begin(), end());
			return self();
		}

		//hash
		constexpr Size hash() const noexcept requires(Hashable<DataType>) {
			Size seed = size();
			const DataType srcDataPtr = beginPtr();
			const DataType srcDataPtrEnd = endPtr();
			for (; srcDataPtr < srcDataPtrEnd; srcDataPtr++) {
				seed ^= Hash<DataType>{}(*srcDataPtr) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
		constexpr static Size staticHash(const SmallDynArray& dynArray) noexcept requires(Hashable<DataType>) {
			return dynArray.hash();
		}
	};
}