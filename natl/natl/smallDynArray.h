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

		using allocation_move_adapater = AllocationMoveAdapater<value_type, Alloc>;

		//movement info 
		constexpr static bool triviallyRelocatable = true;
		constexpr static bool triviallyDefaultConstructible = true;
		constexpr static bool triviallyCompareable = false;
		constexpr static bool triviallyDestructible = false;
		constexpr static bool triviallyConstRefConstructedable = false;
		constexpr static bool triviallyMoveConstructedable = false;

		constexpr static bool enableSmallArray = true;
	private:
		size_type arraySizeAndSmallArrayFlag;
		pointer arrayDataPtr;
		size_type arrayCapacity;
		value_type smallArrayStorage[bufferSize];

		//small array 
	private:
		constexpr static size_type setNthBitToZero(const size_type num, const size_type n) noexcept {
			const size_type mask = ~(size_type(1) << n);
			const size_type result = num & mask;
			return result;
		}
		constexpr static size_type setNthBitToOne(const size_type num, const size_type n) {
			const size_type mask = size_type(1) << n;
			const size_type result = num | mask;
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
		constexpr void setSize(const size_type newSize) {
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
		constexpr static size_type smallArrayCapacity() noexcept {
			if (!enableSmallArray) {
				return 0;
			} else {
				return bufferSize;
			}
		}

		constexpr const_pointer smallArrayLocation() const noexcept {
			if (!enableSmallArray) {
				return nullptr;
			} else {
				return smallArrayStorage;
			}
		}
		constexpr pointer smallArrayLocation() noexcept {
			if (!enableSmallArray) {
				return nullptr;
			} else {
				return smallArrayStorage;
			}
		}

		constexpr size_type capacity() const noexcept { return  isSmallArray() ? smallArrayCapacity() : arrayCapacity; };
		constexpr size_type size() const noexcept { return arraySizeAndSmallArrayFlag & 0x7FFFFFFFFFFFFFFFULL; }
		constexpr size_type max_size() const noexcept { return 0x7FFFFFFFFFFFFFFFULL; };

		constexpr const_pointer data() const noexcept {
			return isSmallArray() ? smallArrayLocation() : arrayDataPtr;
		}
		constexpr pointer data() noexcept {
			return isSmallArray() ? smallArrayLocation() : arrayDataPtr;
		}

	public:
		constexpr void baseConstructorInit() noexcept {
			arraySizeAndSmallArrayFlag = 0;
			arrayCapacity = 0;
			arrayDataPtr = nullptr;

			if (std::is_constant_evaluated()) {
				uninitializedFill<pointer, value_type>(smallArrayStorage, smallArrayStorage + bufferSize, value_type());
			}
		}
		
		//constructors
		constexpr SmallDynArray() noexcept {
			baseConstructorInit();
		}
		constexpr SmallDynArray(const_pointer srcPtr, const size_type& count) noexcept {
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
		constexpr SmallDynArray(const size_type count, const value_type& value = value_type()) noexcept {
			baseConstructorInit();
			construct(count, value);
		}
		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, value_type>)
		constexpr SmallDynArray(Iter first, Iter last) noexcept {
			baseConstructorInit();
			construct<Iter>(first, last);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr SmallDynArray(const ArrayViewLike& arrayViewLike) noexcept {
			baseConstructorInit();
			construct(arrayViewLike.data(), arrayViewLike.size());
		}
		constexpr SmallDynArray(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			baseConstructorInit();
			construct(natl::move(allocationMoveAdapater));
		}
		constexpr SmallDynArray(std::initializer_list<value_type> ilist) noexcept {
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
		constexpr SmallDynArray& construct(const_pointer otherPtr, const size_type& count) noexcept {
			if (count == 0) {
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else if (count < smallArrayCapacity()) {
				const_pointer srcDataPtrFirst = otherPtr;
				const_pointer srcDataPtrLast = srcDataPtrFirst + count;
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, smallArrayLocation());

				setAsSmallArray();

				const size_type newSize = count;
				setSize(newSize);

				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else {
				const size_type newSize = count;
				reserve(newSize);

				const_pointer srcDataPtrFirst = otherPtr;
				const_pointer srcDataPtrLast = srcDataPtrFirst + count;
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, data());

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
				const_pointer srcDataPtrFirst = other.data();
				const_pointer srcDataPtrLast = srcDataPtrFirst + other.size();
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, smallArrayLocation());

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
		constexpr SmallDynArray& construct(const size_type count, const value_type& value = value_type()) noexcept {
			if (count == 0) {
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
				return self();
			}

			reserve(count);

			pointer fillDstPtr = data();
			pointer fillDstPtrLast = fillDstPtr + count;
			uninitializedFill<pointer, value_type>(fillDstPtr, fillDstPtrLast, value);

			setSize(count);
			return self();
		}

		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, value_type>)
		constexpr SmallDynArray& construct(Iter first, Iter last) noexcept {
			if constexpr (std::contiguous_iterator<Iter>) {
				const size_type count = iterDistance<Iter>(first, last);
				const_pointer firstPtr = iteratorToAddress<Iter>(first);
				return construct(firstPtr, count);
			}

			reserve(10);
			for (; first != last; first++) {
				push_back(*first);
			}
			return self();
		}

		constexpr SmallDynArray& construct(allocation_move_adapater&& allocationMoveAdapater) noexcept {
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
			allocationMoveAdapater.release();
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
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr SmallDynArray& operator=(const ArrayViewLike& arrayViewLike) noexcept {
			return assign<ArrayViewLike>(arrayViewLike);
		}
		constexpr SmallDynArray& operator=(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			return assign(natl::move(allocationMoveAdapater));
		}
		constexpr SmallDynArray& operator=(std::initializer_list<value_type> ilist) noexcept {
			return assign(ilist);
		}

		constexpr SmallDynArray& assign(const_pointer otherPtr, const size_type& count) noexcept {
			release();

			if (count == 0) {
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else if(count < smallArrayCapacity()) {
				const_pointer srcDataPtrFirst = otherPtr;
				const_pointer srcDataPtrLast = srcDataPtrFirst + count;
				internalCopyNoOverlap(srcDataPtrFirst, srcDataPtrLast, smallArrayLocation());

				setAsSmallArray();

				const size_type newSize = count;
				setSize(newSize);

				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else {
				const size_type newSize = count;
				reserve(newSize);

				const_pointer srcDataPtrFirst = otherPtr;
				const_pointer srcDataPtrLast = srcDataPtrFirst + count;
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
				const_pointer srcDataPtrFirst = other.data();
				const_pointer srcDataPtrLast = srcDataPtrFirst + other.size();
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
		constexpr SmallDynArray& assign(const size_type count, const value_type& value = value_type()) noexcept {
			release();

			if (count == 0) { 
				arraySizeAndSmallArrayFlag = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
				return self(); 
			} 

			reserve(count);

			if constexpr (IsTriviallyDestructible<value_type>) {
				internalFill(data(), data() + count, value);
			} else {
				if (count > size()) {
					size_type uninitializedFillAmount = count - size();
					pointer fillDstPtr = data() + size();
					pointer fillDstPtrLast = fillDstPtr + uninitializedFillAmount;
					uninitializedFill<pointer, value_type>(fillDstPtr, fillDstPtrLast, value);
				}

				internalFill(data(), data() + size(), value);
			}

			setSize(count);
			return self();
		}
		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, value_type>)
		constexpr SmallDynArray& assign(Iter first, Iter last) noexcept {
			if constexpr (std::contiguous_iterator<Iter>) {
				const size_type count = iterDistance<Iter>(first, last);
				const_pointer firstPtr = iteratorToAddress<Iter>(first);
				return assign(firstPtr, count);
			}

			resize(0);
			for (; first != last; first++) {
				push_back(*first);
			}
			return self();
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr SmallDynArray& assign(const ArrayViewLike& arrayViewLike) noexcept {
			return assign(arrayViewLike.data(), arrayViewLike.size());
		}
		constexpr SmallDynArray& assign(allocation_move_adapater&& allocationMoveAdapater) noexcept {
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
			allocationMoveAdapater.release();
			return self();
		}
		constexpr SmallDynArray& assign(std::initializer_list<value_type> ilist) noexcept {
			return assign(ilist.begin(), ilist.size());
		}

		//strongly linked methods 
		[[nodiscard]] constexpr allocation_move_adapater getAlloctionMoveAdapater() noexcept {
			const bool arrayIsSmallBuffer = isSmallArray();
			AllocationMoveAdapaterRequireCopy requireCopy = arrayIsSmallBuffer ? AllocationMoveAdapaterRequireCopy::v_true : AllocationMoveAdapaterRequireCopy::v_false;
			AllocationMoveAdapaterCanDealloc canBeDealloc = arrayIsSmallBuffer ? AllocationMoveAdapaterCanDealloc::v_false : AllocationMoveAdapaterCanDealloc::v_true;
			allocation_move_adapater allocationMoveAdapater(data(), size(), capacity(), requireCopy, canBeDealloc);
			arrayDataPtr = nullptr;
			arraySizeAndSmallArrayFlag = 0;
			arrayCapacity = 0;
			return natl::move(allocationMoveAdapater);
		}

		constexpr void resize(const size_type count) noexcept {
			if (count < size()) {
				if constexpr (!IsTriviallyDestructible<value_type>) {
					defaultDeconstructAll<value_type>(data() + count, size() - count);
					setSize(count);
					return;
				} else {
					if (std::is_constant_evaluated()) {
						defaultDeconstructAll<value_type>(data() + count, size() - count);
					}
					setSize(count);
				}
			} else if (count > size()) {
				insert(cend(), count - size(), value_type());
			}
		}

		constexpr void resize(const size_type count, const value_type value) noexcept {
			if (count < size()) {
				if constexpr (!IsTriviallyDestructible<value_type> || std::is_constant_evaluated()) {
					defaultDeconstructAll<value_type>(data() + count, size() - count);
				}
				setSize(count);
			} else if (count > size()) {
				insert(cend(), count - size(), value);
			}
		}

		constexpr void reserve(size_type newCapacity) noexcept {
			if (newCapacity <= capacity()) {
				return;
			} else if (newCapacity < smallArrayCapacity()) {
				setAsSmallArray();
				return;
			}

			newCapacity *= 2;

			if (newCapacity < capacity()) { return; }

			pointer newDataPtr = Alloc::allocate(newCapacity);

			if (data()) {
				const_pointer srcDataPtrFirst = data();
				const_pointer srcDataPtrLast = srcDataPtrFirst + size();
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, newDataPtr);

				if constexpr (!IsTriviallyRelocatable<value_type>) {
					defaultDeconstructAll<value_type>(data(), size());
				} else {
					if (std::is_constant_evaluated()) {
						defaultDeconstructAll<value_type>(data(), size());
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
			const size_type newCapacity = size();
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

				pointer dstDataPtr = smallArrayLocation();
				const_pointer srcDataPtrFirst = arrayDataPtr;
				const_pointer srcDataPtrLast = arrayDataPtr + newCapacity;
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, dstDataPtr);

				Alloc::deallocate(arrayDataPtr, capacity());
				arrayCapacity = 0;
				arrayDataPtr = nullptr;

				setAsSmallArray();
			} else if (sameSize) {
				return;
			}


			pointer newDataPtr = Alloc::allocate(newCapacity);
			const_pointer srcDataPtrFirst = data();
			const_pointer srcDataPtrLast = srcDataPtrFirst + size();
			uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, newDataPtr);

			if (!IsTriviallyRelocatable<value_type> || std::is_constant_evaluated()) {
				defaultDeconstructAll<value_type>(data(), size());
			}

			Alloc::deallocate(arrayDataPtr, capacity());

			arrayDataPtr = newDataPtr;
			arrayCapacity = newCapacity;
			setAsNotSmallArray();
		}

		constexpr void swap(SmallDynArray& other) noexcept {
			natl::swap<SmallDynArray>(self(), other);
		}

		constexpr void clear() noexcept {
			if (!IsTriviallyDestructible<value_type> || std::is_constant_evaluated()) {
				defaultDeconstructAll<value_type>(data(), size());
			}
			setSize(0);
		}

	private:
		constexpr void release() noexcept {
			if (arrayDataPtr) {
				if constexpr (!IsTriviallyDestructible<value_type>) {
					defaultDeconstructAll<value_type>(data(), size());
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

		constexpr operator ArrayView<value_type>() noexcept {
			return ArrayView<value_type>(data(), size());
		}
		constexpr operator ArrayView<const value_type>() const noexcept {
			return ArrayView<const value_type>(data(), size());
		}

		constexpr ArrayView<value_type> toArrayView() noexcept {
			return ArrayView<value_type>(data(), size());
		}
		constexpr ArrayView<const value_type> toArrayView() const noexcept {
			return ArrayView<const value_type>(data(), size());
		}

		//special element access
		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp<size_type>(index, frontIndex(), backIndex()); }

		constexpr reference atClamped(const size_type index) noexcept requires(isNotConst<value_type>) { return at(clampIndex(index)); }
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
		constexpr size_type shiftRelocateLeft(const size_type index, const size_type count) noexcept {
			const size_type relocateCount = size() - index;
			if (relocateCount == 0) { return relocateCount; }

			if constexpr (IsTriviallyRelocatable<value_type>) {
				if (!std::is_constant_evaluated()) {
					pointer relocateDstPtr = data() + index + count;
					const_pointer relocateSrcPtrFirst = data() + index;
					const_pointer relocateSrcPtrLast = relocateSrcPtrFirst + relocateCount;
					uninitializedCopy<const_pointer, pointer>(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);
					return relocateCount;
				}
			}

			const size_type uninitializedCount = count;
			const size_type normalCount = relocateCount - uninitializedCount;

			pointer relocateUninitializedDstPtr = data() + backIndex();
			const_pointer relocateUninitializedSrcPtrFirst = data() + index + normalCount;
			const_pointer relocateUninitializedSrcPtrLast = relocateUninitializedSrcPtrFirst + uninitializedCount;
			uninitializedCopy<const_pointer, pointer>(relocateUninitializedSrcPtrFirst, relocateUninitializedSrcPtrLast, relocateUninitializedDstPtr);

			pointer relocateNormalDstPtr = data() + index + count;
			const_pointer relocateNormalSrcPtrFirst = data() + index;
			const_pointer relocateNormalSrcPtrLast = relocateNormalSrcPtrFirst + normalCount;
			copy<const_pointer, pointer>(relocateNormalSrcPtrFirst, relocateNormalSrcPtrLast, relocateNormalDstPtr);
			return relocateCount;
		}
	public:

		constexpr iterator insert(const_iterator pos, const value_type& value) noexcept {
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const size_type newSize = size() + 1;
			reserve(newSize);

			shiftRelocateLeft(index, 1);
			set(index, value);

			setSize(newSize);
			return iterator(data() + index);

		}
		constexpr iterator insert(const_iterator pos, value_type&& value) noexcept {
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const size_type newSize = size() + 1;
			reserve(newSize);

			shiftRelocateLeft(index, 1);

			set(index, forward<value_type>(value));

			setSize(newSize);
			return iterator(data() + index);
		}
		constexpr iterator insert(const_iterator pos, const_pointer srcPtr, const size_type count) noexcept {
			if (count == 0) { return constIteratorToIterator(pos); }
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const size_type newSize = size() + count;
			reserve(newSize);

			const size_type relocateCount = shiftRelocateLeft(index, count);

			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					pointer insertDstPtr = data() + index;
					const_pointer insertSrcPtrFirst = srcPtr;
					const_pointer insertSrcPtrLast = insertSrcPtrFirst + count;
					internalCopyNoOverlap(insertSrcPtrFirst, insertSrcPtrLast, insertDstPtr);

					setSize(newSize);
					return iterator(data() + index);
				}
			}

			const size_type regularCopyCount = relocateCount;
			pointer insertRegularDstPtr = data() + index;
			const_pointer insertRegularSrcPtrFirst = srcPtr;
			const_pointer insertRegularSrcPtrLast = insertRegularSrcPtrFirst + regularCopyCount;
			natl::copyCountNoOverlap<const_pointer, pointer>(insertRegularSrcPtrFirst, insertRegularSrcPtrLast, insertRegularDstPtr);

			const size_type uninilizedCopyCount = count - relocateCount;
			pointer insertUninilizedDstPtr = data() + index + regularCopyCount;
			const_pointer insertUninilizedSrcPtrFirst = insertRegularSrcPtrLast;
			const_pointer insertUninilizedSrcPtrLast = insertUninilizedSrcPtrFirst + uninilizedCopyCount;
			natl::uninitializedCopyNoOverlap<const_pointer, pointer>(insertUninilizedSrcPtrFirst, insertUninilizedSrcPtrLast, insertUninilizedDstPtr);

			setSize(newSize);
			return iterator(data() + index);
		}
		constexpr iterator insert(const_iterator pos, const size_type count, const value_type& value) {
			if (count == 0) { return constIteratorToIterator(pos); }
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const size_type newSize = size() + count;
			reserve(newSize);

			const size_type relocateCount = shiftRelocateLeft(index, count);

			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					pointer fillDstPtrFirst = data() + index;
					pointer fillDstPtrLast = fillDstPtrFirst + count;
					internalFill(fillDstPtrFirst, fillDstPtrLast, value);

					setSize(newSize);
					return iterator(data() + index);
				}
			}

			const size_type regularFillCount = relocateCount;
			pointer fillRegularDstPtrFirst = data() + index;
			pointer fillRegularDstPtrLast = fillRegularDstPtrFirst + regularFillCount;
			natl::fill<pointer, value_type>(fillRegularDstPtrFirst, fillRegularDstPtrLast, value);

			const size_type uninilizedFillCount = count - relocateCount;
			pointer fillUninilizedDstPtrFirst = fillRegularDstPtrLast;
			pointer fillUninilizedDrcPtrLast = fillUninilizedDstPtrFirst + uninilizedFillCount;
			natl::uninitializedFill<pointer, value_type>(fillUninilizedDstPtrFirst, fillUninilizedDrcPtrLast, value);

			setSize(newSize);
			return iterator(data() + index);
		}
		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, value_type>)
		constexpr iterator insert(const_iterator pos, Iter first, Iter last) {
			if constexpr (IsRandomAccessIterator<Iter>) {
				const size_type count = iterDistance<Iter>(first, last);
				const_pointer firstPtr = iteratorToAddress<Iter>(first);
				insert(pos, firstPtr, count);
			}

			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const size_type count = iterDistance<Iter>(first, last);
			const size_type newSize = size() + count;
			reserve(newSize);

			const size_type relocateCount = shiftRelocateLeft(index, count);

			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					pointer insertDstPtr = data() + index;
					internalCopyNoOverlap<Iter>(first, last, insertDstPtr);

					setSize(newSize);
					return iterator(data() + index);
				}
			}

			Iter srcIter = first;
			pointer dstPtr = data() + index;
			const size_type regularCopyCount = relocateCount;
			const size_type uninilizedCopyCount = count - relocateCount;
			for (size_type i = 0; i < regularCopyCount; i++, dstPtr++, srcIter++) {
				*dstPtr = *srcIter;
			}
			for (size_type i = 0; i < uninilizedCopyCount; i++, dstPtr++, srcIter++) {
				std::construct_at<value_type, value_type>(dstPtr, *srcIter);
			}

			setSize(newSize);
			return iterator(data() + index);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr iterator insert(const_iterator pos, const ArrayViewLike& arrayViewLike) noexcept {
			return insert(pos, arrayViewLike.data(), arrayViewLike.size());
		}
		constexpr iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) noexcept {
			return insert(pos, ilist.begin(), ilist.size());
		}

		template<class... Args >
		constexpr iterator emplace(const_iterator pos, Args&&... args) {
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const size_type newSize = size() + 1;
			reserve(newSize);

			shiftRelocateLeft(index, 1);

			std::construct_at<value_type, Args...>(&*pos, std::forward<Args>(args)...);

			setSize(newSize);
			return iterator(data() + index);
		}

		constexpr iterator erase(const_iterator pos) noexcept {
			const size_type newSize = size() - 1;
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);


			pointer relocateDstPtr = data() + index;
			const_pointer relocateSrcPtrFirst = data() + index + 1;
			const_pointer relocateSrcPtrLast = endPtr();
			internalCopy(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);

			if (!IsTriviallyDestructible<value_type> || std::is_constant_evaluated()) {
				defaultDeconstruct<value_type>(at(backIndex()));
			}

			setSize(newSize);
			return iterator(data() + index);
		}
		constexpr iterator erase(const_iterator first, const_iterator last) noexcept {
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*first);
			const size_type count = iterDistance<typename const_iterator::pointer>(&*first, &*last);
			const size_type newSize = size() - count;

			pointer relocateDstPtr = data() + index;
			const_pointer relocateSrcPtrFirst = data() + index + count;
			const_pointer relocateSrcPtrLast = endPtr();
			internalCopy(relocateSrcPtrFirst, relocateSrcPtrLast, relocateDstPtr);

			if (!IsTriviallyDestructible<value_type> || std::is_constant_evaluated()) {
				defaultDeconstructAll<value_type>(data() + newSize, count);
			}

			setSize(newSize);
			return iterator(data() + index);
		}

		constexpr iterator eraseAtIndex(const std::size_t index) noexcept {
			return erase(begin() + index);
		}

		constexpr reference push_back(const value_type& value) noexcept {
			const size_type index = size();
			const size_type newSize = index + 1;
			reserve(newSize);
			return set(index, value);
		}
		constexpr reference push_back(value_type&& value) noexcept {
			const size_type index = size();
			const size_type newSize = index + 1;
			reserve(newSize);
			setSize(newSize);
			return set(index, forward<value_type>(value));;
		}

		template<class... Args >
		constexpr reference emplace_back(Args&&... args) noexcept {
			const size_type index = size();
			const size_type newSize = index + 1;
			reserve(newSize);
			setSize(newSize);
			reference value = at(index);
			std::construct_at<value_type, Args...>(value, std::forward<Args>(args)...);
			return value;
		}

		constexpr SmallDynArray& append(const_pointer srcPtr, const size_type count) noexcept {
			const size_type index = size();
			const size_type newSize = index + 1;
			reserve(newSize);

			pointer insertDstPtr = data() + index;
			const_pointer insertSrcPtrFirst = srcPtr;
			const_pointer insertSrcPtrLast = insertSrcPtrFirst + count;
			internalCopyNoOverlap(insertSrcPtrFirst, insertSrcPtrLast, insertDstPtr);

			setSize(newSize);
			return self();
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr SmallDynArray& append(const ArrayViewLike& arrayViewLike) noexcept {
			return append(arrayViewLike.data(), arrayViewLike.size());
		}
		template<class ArrayViewLikeConvertible>
			requires(std::is_convertible_v<ArrayViewLikeConvertible, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLikeConvertible, value_type>)
		constexpr SmallDynArray& append(const ArrayViewLikeConvertible& arrayViewLikeConvertible) noexcept {
			return append<ArrayView<const value_type>>(static_cast<ArrayView<const value_type>>(arrayViewLikeConvertible));
		}
		constexpr SmallDynArray& append(const std::initializer_list<value_type> ilist) noexcept {
			return append(ilist.begin(), ilist.end());
		}

		constexpr void pop_back() noexcept {
			if (!IsTriviallyDestructible<value_type> || std::is_constant_evaluated()) {
				defaultDeconstruct<value_type>(&at(backIndex()));
			}
			setSize(size() - 1);
			return;
		}

	private:
		constexpr reference set(const size_type index, const value_type& value) noexcept {
			reference valueDst = at(index);
			if (IsTriviallyDestructible<value_type>) {
				valueDst = value;
			}
			else {
				value_type tempValue = value;
				std::construct_at<value_type, value_type>(&valueDst, forward<value_type>(tempValue));
			}
			return valueDst;
		}
		constexpr reference set(const size_type index, value_type&& value) noexcept {
			reference valueDst = at(index);
			if (IsTriviallyDestructible<value_type>) {
				valueDst = value;
			}
			else {
				std::construct_at<value_type, value_type>(&valueDst, forward<value_type>(value));
			}
			return valueDst;
		}

		constexpr void internalCopyNoOverlap(const_pointer srcPtr, const_pointer srcPtrLast, pointer dstPtr) noexcept {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedCopyNoOverlap<const_pointer, pointer>(srcPtr, srcPtrLast, dstPtr);
					return;
				}
			}

			copyNoOverlap<const_pointer, pointer>(srcPtr, srcPtrLast, dstPtr);
		}
		template<class Iter>
		constexpr void internalCopyNoOverlap(Iter first, Iter last, pointer dstPtr) {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedCopyNoOverlap<Iter, pointer>(first, last, dstPtr);
					return;
				}
			}
			copyNoOverlap<Iter, pointer>(first, last, dstPtr);
		}

		constexpr void internalCopy(const_pointer srcPtr, const_pointer srcPtrLast, pointer dstPtr) noexcept {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedCopy<const_pointer, pointer>(srcPtr, srcPtrLast, dstPtr);
					return;
				}
			}
			copy<const_pointer, pointer>(srcPtr, srcPtrLast, dstPtr);
		}
		template<class Iter>
		constexpr void internalCopy(Iter first, Iter last, pointer dstPtr) noexcept {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedCopy<Iter, pointer>(first, last, dstPtr);
					return;
				}
			}
			copy<Iter, pointer>(first, last, dstPtr);
		}

		constexpr void internalFill(pointer dstPtr, pointer dstPtrLast, const value_type& value) noexcept {
			if (!std::is_constant_evaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedFill<pointer, value_type>(dstPtr, dstPtrLast, value);
					return;
				}
			}
			fill<pointer, value_type>(dstPtr, dstPtrLast, value);
		}
	public:
		//compare operators 
		friend constexpr bool operator==(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() == rhs.toArrayView();
		}
		friend constexpr bool operator==(const SmallDynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator==(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator==(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		friend constexpr bool operator==(const SmallDynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}

		friend constexpr bool operator!=(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() != rhs.toArrayView();
		}
		friend constexpr bool operator!=(const SmallDynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator!=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator!=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		friend constexpr bool operator!=(const SmallDynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}

		friend constexpr bool operator<(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() < rhs.toArrayView();
		}
		friend constexpr bool operator<(const SmallDynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator<(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator<(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		friend constexpr bool operator<(const SmallDynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}

		friend constexpr bool operator<=(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() <= rhs.toArrayView();
		}
		friend constexpr bool operator<=(const SmallDynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator<=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator<=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		friend constexpr bool operator<=(const SmallDynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}

		friend constexpr bool operator>(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() > rhs.toArrayView();
		}
		friend constexpr bool operator>(const SmallDynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator>(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator>(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		friend constexpr bool operator>(const SmallDynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}

		friend constexpr bool operator>=(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() >= rhs.toArrayView();
		}
		friend constexpr bool operator>=(const SmallDynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator>=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator>=(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		friend constexpr bool operator>=(const SmallDynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}

		friend constexpr std::strong_ordering operator<=>(const SmallDynArray& lhs, const SmallDynArray& rhs) noexcept {
			return lhs.toArrayView() <=> rhs.toArrayView();
		}
		friend constexpr std::strong_ordering operator<=>(const SmallDynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr std::strong_ordering operator<=>(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr std::strong_ordering operator<=>(const SmallDynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		friend constexpr bool operator<=>(const SmallDynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}

		//operations 
		constexpr SmallDynArray& reverse() noexcept {
			reverse<iterator>(begin(), end());
			return self();
		}

		//hash
		constexpr size_type hash() const noexcept requires(Hashable<value_type>) {
			size_type seed = size();
			const value_type srcDataPtr = beginPtr();
			const value_type srcDataPtrEnd = endPtr();
			for (; srcDataPtr < srcDataPtrEnd; srcDataPtr++) {
				seed ^= Hash<value_type>{}(*srcDataPtr) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
		constexpr static size_type staticHash(const SmallDynArray& dynArray) noexcept requires(Hashable<value_type>) {
			return dynArray.hash();
		}
	};
}