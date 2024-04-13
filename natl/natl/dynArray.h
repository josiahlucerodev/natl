#pragma once 

//own
#include "allocator.h"
#include "container.h"
#include "arrayView.h"
#include "hash.h"

//interface 
namespace natl {
	template<typename DynArrayLike, typename DataType>
	concept IsDynamicArrayLike = HasBeginIterator<DynArrayLike> && HasEndIterator<DynArrayLike> && 
		requires(DynArrayLike & dynArray, const Size newSize) {
		{ dynArray.reserve(newSize) };
		{ dynArray.resize(newSize) };
		{ dynArray.data() } -> ConvertibleTo<DataType*>;
	};
	template<typename DynArrayLike, typename DataType>
	concept IsCloselyDynamicArrayLike =
		HasBeginIterator<DynArrayLike> &&
		HasEndIterator<DynArrayLike> &&
		(sizeof(typename DynArrayLike::value_type) == sizeof(DataType)) &&
		requires(DynArrayLike& dynArray, const Size newSize) {
			{ dynArray.reserve(newSize) };
			{ dynArray.resize(newSize) };
			{ dynArray.data() };
	};
	template<typename DynArrayLike>
	concept IsByteDynamicArrayLike = IsCloselyDynamicArrayLike<DynArrayLike, Byte>;

	template<class DataType, class Alloc = DefaultAllocator<DataType>>
		requires(IsAllocator<Alloc>)
	class DynArray {
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
	private:
		size_type arraySize;
		size_type arrayCapacity;
		pointer arrayDataPtr;
	private:
		constexpr void baseConstructorInit() noexcept {
			arraySize = 0;
			arrayDataPtr = 0;
			arrayCapacity = 0;
		}
	public:
		//constructors
		constexpr DynArray() noexcept {
			baseConstructorInit();
		}
		constexpr DynArray(const_pointer srcPtr, const size_type& count) noexcept {
			baseConstructorInit();
			construct(srcPtr, count);
		}
		constexpr DynArray(const DynArray& other) noexcept {
			baseConstructorInit();
			construct(other.data(), other.size());
		}
		constexpr DynArray(DynArray&& other) noexcept {
			baseConstructorInit();
			construct(forward<DynArray>(other));
		}
		constexpr DynArray(const size_type count, const value_type& value = value_type()) noexcept {
			baseConstructorInit();
			construct(count, value);
		}
		template<class Iter>
			requires(IsIterPtr<Iter> && std::is_same_v<typename IterPtrTraits<Iter>::value_type, value_type>)
		constexpr DynArray(Iter first, Iter last) noexcept {
			baseConstructorInit();
			construct<Iter>(first, last);
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr DynArray(const ArrayViewLike& arrayViewLike) noexcept {
			baseConstructorInit();
			construct(arrayViewLike.data(), arrayViewLike.size());
		}
		constexpr DynArray(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			baseConstructorInit();
			construct(natl::move(allocationMoveAdapater));
		}
		constexpr DynArray(std::initializer_list<value_type> ilist) noexcept {
			baseConstructorInit();
			construct(ilist.begin(), ilist.size());
		}

		//destructor 
		constexpr ~DynArray() {
			release();
		}

		//util 
		constexpr DynArray& self() noexcept { return *this; }
		constexpr const DynArray& self() const noexcept { return *this; }

		//construct
	private:
		constexpr DynArray& construct(const_pointer otherPtr, const size_type& count) noexcept {
			if (count == 0) {
				arraySize = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
				return self();
			}

			const size_type newSize = count;
			factorReserve(newSize);
			const_pointer srcDataPtrFirst = otherPtr;
			const_pointer srcDataPtrLast = srcDataPtrFirst + count;
			uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, data());
			setSize(newSize);

			return self();
		}
		constexpr DynArray& construct(DynArray&& other) noexcept {
			arraySize = other.arraySize;
			arrayCapacity = other.arrayCapacity;
			arrayDataPtr = other.arrayDataPtr;

			other.arraySize = 0;
			other.arrayCapacity = 0;
			other.arrayDataPtr = nullptr;

			return self();
		}
		constexpr DynArray& construct(const size_type count, const value_type& value = value_type()) noexcept {
			if (count == 0) { 
				arraySize = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
				return self(); 
			}

			factorReserve(count);

			pointer fillDstPtr = data();
			pointer fillDstPtrLast = fillDstPtr + count;
			uninitializedFill<pointer, value_type>(fillDstPtr, fillDstPtrLast, value);
			setSize(count);
			return self();
		}

		template<class Iter>
			requires(IsIterPtr<Iter>&& std::is_same_v<typename IterPtrTraits<Iter>::value_type, value_type>)
		constexpr DynArray& construct(Iter first, Iter last) noexcept {
			if constexpr (std::contiguous_iterator<Iter>) {
				const size_type count = iterDistance<Iter>(first, last);
				const_pointer firstPtr = iteratorToAddress<Iter>(first);
				return construct(firstPtr, count);
			}

			factorReserve(10);
			for (; first != last; first++) {
				push_back(*first);
			}
			return self();
		}

		constexpr DynArray& construct(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			if (allocationMoveAdapater.isEmpty()) {
				arraySize = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else if (allocationMoveAdapater.requiresCopy()) {
				construct(allocationMoveAdapater.data(), allocationMoveAdapater.size());
				if (allocationMoveAdapater.canDealloc()) {
					allocationMoveAdapater.deallocate();
				}
			} else {
				arraySize = allocationMoveAdapater.size();
				arrayCapacity = allocationMoveAdapater.capacity();
				arrayDataPtr = allocationMoveAdapater.data();
			}
			allocationMoveAdapater.release();
			return self();
		}

	public:

		//assignment 
		constexpr DynArray& operator=(const DynArray& other) noexcept {
			return assign(other);
		}
		constexpr DynArray& operator=(DynArray&& other) noexcept {
			return assign(forward<DynArray>(other));
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr DynArray& operator=(const ArrayViewLike& arrayViewLike) noexcept {
			return assign<ArrayViewLike>(arrayViewLike);
		}
		constexpr DynArray& operator=(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			return assign(natl::move(allocationMoveAdapater));
		}
		constexpr DynArray& operator=(std::initializer_list<value_type> ilist) noexcept {
			return assign(ilist);
		}

		constexpr DynArray& assign(const_pointer otherPtr, const size_type& count) noexcept {
			if (count == 0) {
				release();
				arraySize = 0;
				arrayCapacity = 0;
				return self();
			}

			const size_type newSize = count;
			factorReserve(newSize);
			internalCopyNoOverlap(otherPtr, otherPtr + newSize, data());
			setSize(newSize);

			return self();
		}
		constexpr DynArray& assign(const DynArray& other) noexcept {
			return assign(other.data(), other.size());
		}
		constexpr DynArray& assign(DynArray&& other) noexcept {
			release();
			arraySize = other.arraySize;
			arrayCapacity = other.arrayCapacity;
			arrayDataPtr = other.arrayDataPtr;

			other.arraySize = 0;
			other.arrayCapacity = 0;
			other.arrayDataPtr = nullptr;

			return self();
		}
		constexpr DynArray& assign(const size_type count, const value_type& value = value_type()) noexcept {
			release();
			if (count == 0) { 
				arraySize = 0;
				arrayCapacity = 0;
				return self(); 
			}
			factorReserve(count);

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
			requires(IsIterPtr<Iter> && std::is_same_v<typename IterPtrTraits<Iter>::value_type, value_type>)
		constexpr DynArray& assign(Iter first, Iter last) noexcept {
			if constexpr (IsRandomAccessIterator<Iter>) {
				const size_type count = iterDistance<Iter>(first, last);
				const_pointer firstPtr = iteratorToAddress<Iter>(first);
				assign(firstPtr, count);
			}

			release();
			size_type index = 0;
			for (; first != last && index < size(); first++) {
				at(index) = *first;
			}

			for (; first != last; first++) {
				push_back(*first);
			}
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr DynArray& assign(const ArrayViewLike& arrayViewLike) noexcept {
			assign(arrayViewLike.data(), arrayViewLike.size());
		}
		constexpr DynArray& assign(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			if (allocationMoveAdapater.isEmpty()) {
				release();
				arraySize = 0;
				arrayCapacity = 0;
				arrayDataPtr = nullptr;
			} else if (allocationMoveAdapater.requiresCopy()) {
				assign(allocationMoveAdapater.data(), allocationMoveAdapater.size());
				if (allocationMoveAdapater.canDealloc()) {
					allocationMoveAdapater.deallocate();
				}
			} else {
				release();
				arraySize = allocationMoveAdapater.size();
				arrayCapacity = allocationMoveAdapater.capacity();
				arrayDataPtr = allocationMoveAdapater.data();
			}
			allocationMoveAdapater.release();
			return self();
		}
		constexpr DynArray& assign(std::initializer_list<value_type> ilist) noexcept {
			assign(ilist.begin(), ilist.size());
		}


		//strongly linked methods 
		constexpr size_type size() const noexcept { return arraySize; }
		constexpr size_type max_size() const noexcept { return 0xFFFFFFFFFFFFFFFFULL; };

		[[nodiscard]] constexpr allocation_move_adapater getAlloctionMoveAdapater() noexcept {
			allocation_move_adapater allocationMoveAdapater(data(), size(), capacity(), AllocationMoveAdapaterRequireCopy::False, AllocationMoveAdapaterCanDealloc::True);
			arrayDataPtr = nullptr;
			arraySize = 0;
			arrayCapacity = 0;
			return natl::move(allocationMoveAdapater);
		}


		constexpr void resize(const size_type count) noexcept {
			if (count < size()) {
				if constexpr (!IsTriviallyDestructible<value_type>) {
					defaultDeconstructAll<value_type>(data() + count, size() - count);
					setSize(count);
					return;
				}
				else {
					if (isConstantEvaluated()) {
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
				if constexpr (!IsTriviallyDestructible<value_type> || isConstantEvaluated()) {
					defaultDeconstructAll<value_type>(data() + count, size() - count);
				}
				setSize(count);
			} else if (count > size()) {
				insert(cend(), count - size(), value);
			}
		}

		constexpr void factorReserve(const size_type newCapacity) noexcept {
			if (newCapacity <= capacity()) { return; }
			reserveExact(newCapacity * 2);
		}

		constexpr void reserve(const size_type newCapacity) noexcept {
			if (newCapacity <= capacity()) { return; }
			reserveExact(newCapacity);
		}

		constexpr void reserveExact(const size_type newCapacity) noexcept {
			if (newCapacity <= capacity()) { return; }

			pointer newDataPtr = Alloc::allocate(newCapacity);

			if (data()) {
				const_pointer srcDataPtrFirst = data();
				const_pointer srcDataPtrLast = srcDataPtrFirst + size();
				uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, newDataPtr);

				if constexpr (!IsTriviallyRelocatable<value_type>) {
					defaultDeconstructAll<value_type>(data(), size());
				}
				else {
					if (isConstantEvaluated()) {
						defaultDeconstructAll<value_type>(data(), size());
					}
				}

				Alloc::deallocate(arrayDataPtr, capacity());
			}

			arrayDataPtr = newDataPtr;
			arrayCapacity = newCapacity;
		}

		constexpr size_type capacity() const noexcept { return  arrayCapacity; };

		constexpr void shrink_to_fit() noexcept {
			if (size() == capacity()) { return; }

			const size_type newCapacity = size();
			pointer newDataPtr = Alloc::allocate(newCapacity);
			const_pointer srcDataPtrFirst = data();
			const_pointer srcDataPtrLast = srcDataPtrFirst + size();
			uninitializedCopyNoOverlap<const_pointer, pointer>(srcDataPtrFirst, srcDataPtrLast, newDataPtr);

			if (!IsTriviallyRelocatable<value_type> || isConstantEvaluated()) {
				defaultDeconstructAll<value_type>(data(), size());
			}

			Alloc::deallocate(arrayDataPtr, capacity());

			arrayDataPtr = newDataPtr;
			arrayCapacity = newCapacity;
		}

		constexpr void swap(DynArray& other) noexcept {
			size_type tempArraysize_type = arraySize;
			size_type tempArrayCapacity = arrayCapacity;
			pointer tempArrayDataPtr = arrayDataPtr;

			arraySize = other.arraySize;
			arrayCapacity = other.arrayCapacity;
			arrayDataPtr = other.arrayDataPtr;

			other.arraySize = tempArraysize_type;
			other.arrayCapacity = tempArrayCapacity;
			other.arrayDataPtr = tempArrayDataPtr;
		}

		constexpr void clear() noexcept {
			if (typeHasToBeDestructed<value_type>()) {
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

		constexpr void setSize(const size_type newSize) { arraySize = newSize; }
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
	
		constexpr pointer data() noexcept { return arrayDataPtr; }
		constexpr const_pointer data() const noexcept { return arrayDataPtr; }

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

		constexpr reference atClamped(const size_type index) noexcept requires(IsNotConstV<value_type>) { return at(clampIndex(index)); }
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
		constexpr const_iterator cbegin() const noexcept { return begin(); }
		constexpr iterator end() noexcept { return iterator(endPtr()); }
		constexpr const_iterator end() const noexcept { return const_iterator(endPtr()); }
		constexpr const_iterator cend() const noexcept { return end(); }
		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
		constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
		constexpr reverse_iterator rend() noexcept { return reverse_iterator(--begin()); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(--begin()); }
		constexpr const_reverse_iterator crend() const noexcept { return rend(); }

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
				if (!isConstantEvaluated()) {
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
			factorReserve(newSize);

			shiftRelocateLeft(index, 1);
			set(index, value);

			setSize(newSize);
			return iterator(data() + index);

		}
		constexpr iterator insert(const_iterator pos, value_type&& value) noexcept {
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const size_type newSize = size() + 1;
			factorReserve(newSize);

			shiftRelocateLeft(index, 1);

			set(index, forward<value_type>(value));

			setSize(newSize);
			return iterator(data() + index);
		}
		constexpr iterator insert(const_iterator pos, const_pointer srcPtr, const size_type count) noexcept {
			if (count == 0) { return constIteratorToIterator(pos); }
			const size_type index = iterDistance<typename const_iterator::pointer>(&*cbegin(), &*pos);
			const size_type newSize = size() + count;
			factorReserve(newSize);

			const size_type relocateCount = shiftRelocateLeft(index, count);

			if (!isConstantEvaluated()) {
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
			factorReserve(newSize);

			const size_type relocateCount = shiftRelocateLeft(index, count);

			if (!isConstantEvaluated()) {
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
			factorReserve(newSize);

			const size_type relocateCount = shiftRelocateLeft(index, count);

			if (!isConstantEvaluated()) {
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
			factorReserve(newSize);

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

			if (!IsTriviallyDestructible<value_type> || isConstantEvaluated()) {
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

			if (!IsTriviallyDestructible<value_type> || isConstantEvaluated()) {
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
			factorReserve(newSize);
			setSize(newSize);
			return set(index, value);
		}
		constexpr reference push_back(value_type&& value) noexcept {
			const size_type index = size();
			const size_type newSize = index + 1;
			factorReserve(newSize);
			setSize(newSize);
			return set(index, forward<value_type>(value));;
		}

		template<class... Args >
		constexpr reference emplace_back(Args&&... args) noexcept {
			const size_type index = size();
			const size_type newSize = index + sizeof...(Args);
			factorReserve(newSize);
			setSize(newSize);
			reference value = at(index);
			std::construct_at<value_type, Args...>(value, std::forward<Args>(args)...);
			return value;
		}

		constexpr DynArray& append(const_pointer srcPtr, const size_type count) noexcept {
			const size_type index = size();
			const size_type newSize = index + count;
			factorReserve(newSize);

			pointer insertDstPtr = data() + index;
			const_pointer insertSrcPtrFirst = srcPtr;
			const_pointer insertSrcPtrLast = insertSrcPtrFirst + count;
			internalCopyNoOverlap(insertSrcPtrFirst, insertSrcPtrLast, insertDstPtr);

			setSize(newSize);
			return self();
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		constexpr DynArray& append(const ArrayViewLike& arrayViewLike) noexcept {
			return append(arrayViewLike.data(), arrayViewLike.size());
		}
		template<class ArrayViewLikeConvertible>
			requires(std::is_convertible_v<ArrayViewLikeConvertible, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLikeConvertible, value_type>)
		constexpr DynArray& append(const ArrayViewLikeConvertible& arrayViewLikeConvertible) noexcept {
			return append<ArrayView<const value_type>>( static_cast<ArrayView<const value_type>>(arrayViewLikeConvertible) );
		}
		constexpr DynArray& append(const std::initializer_list<value_type> ilist) noexcept {
			return append(ilist.begin(), ilist.end());
		}

		constexpr void pop_back() noexcept {
			if (!IsTriviallyDestructible<value_type> || isConstantEvaluated()) {
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
			} else {
				value_type tempValue = value;
				std::construct_at<value_type, value_type>(&valueDst, forward<value_type>(tempValue));
			}
			return valueDst;
		}
		constexpr reference set(const size_type index, value_type&& value) noexcept {
			reference valueDst = at(index);
			if (IsTriviallyDestructible<value_type>) {
				valueDst = value;
			} else {
				std::construct_at<value_type, value_type>(&valueDst, forward<value_type>(value));
			}
			return valueDst;
		}

		constexpr void internalCopyNoOverlap(const_pointer srcPtr, const_pointer srcPtrLast, pointer dstPtr) noexcept {
			if (!isConstantEvaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedCopyNoOverlap<const_pointer, pointer>(srcPtr, srcPtrLast, dstPtr);
					return;
				}
			}

			copyNoOverlap<const_pointer, pointer>(srcPtr, srcPtrLast, dstPtr);
		}
		template<class Iter>
		constexpr void internalCopyNoOverlap(Iter first, Iter last, pointer dstPtr) {
			if (!isConstantEvaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedCopyNoOverlap<Iter, pointer>(first, last, dstPtr);
					return;
				}
			}
			copyNoOverlap<Iter, pointer>(first, last, dstPtr);
		}

		constexpr void internalCopy(const_pointer srcPtr, const_pointer srcPtrLast, pointer dstPtr) noexcept {
			if (!isConstantEvaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedCopy<const_pointer, pointer>(srcPtr, srcPtrLast, dstPtr);
					return;
				}
			}
			copy<const_pointer, pointer>(srcPtr, srcPtrLast, dstPtr);
		}
		template<class Iter>
		constexpr void internalCopy(Iter first, Iter last, pointer dstPtr) noexcept {
			if (!isConstantEvaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedCopy<Iter, pointer>(first, last, dstPtr);
					return;
				}
			} 
			copy<Iter, pointer>(first, last, dstPtr);
		}

		constexpr void internalFill(pointer dstPtr, pointer dstPtrLast, const value_type& value) noexcept {
			if (!isConstantEvaluated()) {
				if constexpr (IsTriviallyDestructible<value_type>) {
					uninitializedFill<pointer, value_type>(dstPtr, dstPtrLast, value);
					return;
				}
			}
			fill<pointer, value_type>(dstPtr, dstPtrLast, value);
		}
	public:
		//compare operators 
		friend constexpr bool operator==(const DynArray& lhs, const DynArray& rhs) noexcept {
			return lhs.toArrayView() == rhs.toArrayView();
		}
		friend constexpr bool operator==(const DynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator==(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator==(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}
		friend constexpr bool operator==(const DynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() == rhs;
		}

		friend constexpr bool operator!=(const DynArray& lhs, const DynArray& rhs) noexcept {
			return lhs.toArrayView() != rhs.toArrayView();
		}
		friend constexpr bool operator!=(const DynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator!=(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator!=(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}
		friend constexpr bool operator!=(const DynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() != rhs;
		}

		friend constexpr bool operator<(const DynArray& lhs, const DynArray& rhs) noexcept {
			return lhs.toArrayView() < rhs.toArrayView();
		}
		friend constexpr bool operator<(const DynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator<(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator<(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}
		friend constexpr bool operator<(const DynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() < rhs;
		}

		friend constexpr bool operator<=(const DynArray& lhs, const DynArray& rhs) noexcept {
			return lhs.toArrayView() <= rhs.toArrayView();
		}
		friend constexpr bool operator<=(const DynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator<=(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator<=(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}
		friend constexpr bool operator<=(const DynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() <= rhs;
		}

		friend constexpr bool operator>(const DynArray& lhs, const DynArray& rhs) noexcept {
			return lhs.toArrayView() > rhs.toArrayView();
		}
		friend constexpr bool operator>(const DynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator>(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator>(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}
		friend constexpr bool operator>(const DynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() > rhs;
		}

		friend constexpr bool operator>=(const DynArray& lhs, const DynArray& rhs) noexcept {
			return lhs.toArrayView() >= rhs.toArrayView();
		}
		friend constexpr bool operator>=(const DynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator>=(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr bool operator>=(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}
		friend constexpr bool operator>=(const DynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() >= rhs;
		}

		friend constexpr StrongOrdering operator<=>(const DynArray& lhs, const DynArray& rhs) noexcept {
			return lhs.toArrayView() <=> rhs.toArrayView();
		}
		friend constexpr StrongOrdering operator<=>(const DynArray& lhs, const value_type rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		template<class ArrayViewLike>
			requires(IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr StrongOrdering operator<=>(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		template<class ArrayViewLike>
			requires(std::is_convertible_v<ArrayViewLike, ArrayView<const value_type>> && !IsArrayViewLike<ArrayViewLike, const value_type>)
		friend constexpr StrongOrdering operator<=>(const DynArray& lhs, const ArrayViewLike& rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}
		friend constexpr bool operator<=>(const DynArray& lhs, std::initializer_list<value_type> rhs) noexcept {
			return lhs.toArrayView() <=> rhs;
		}

		//operations 
		constexpr DynArray& reverse() noexcept {
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
		constexpr static size_type staticHash(const DynArray& dynArray) noexcept requires(Hashable<value_type>) {
			return dynArray.hash();
		}
	};
}