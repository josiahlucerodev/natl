#pragma once 

#include "../util/allocator.h"
#include "../util/dataMovement.h"
#include "container.h"
#include "arrayView.h"

//interface 
namespace natl {
	template<typename DataType, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	struct HeapArray {
	public:
		using allocator_type = Alloc;
		using typed_allocator_type = allocator_type::template rebind<DataType>;

		using value_type = typename typed_allocator_type::value_type;
		using reference = typename typed_allocator_type::reference;
		using const_reference = typename typed_allocator_type::const_reference;
		using pointer = typename typed_allocator_type::pointer;
		using const_pointer = typename typed_allocator_type::const_pointer;
		using difference_type = typename typed_allocator_type::difference_type;
		using size_type = typename typed_allocator_type::size_type;

		using optional_pointer = Option<pointer>;
		using optional_const_pointer = Option<const_pointer>;

		using iterator = RandomAccessIteratorAlloc<value_type, allocator_type>;
		using const_iterator = ConstRandomAccessIteratorAlloc<value_type, allocator_type>;
		using reverse_iterator = ReverseRandomAccessIteratorAlloc<value_type, allocator_type>;
		using const_reverse_iterator = ReverseConstRandomAccessIteratorAlloc<value_type, allocator_type>;

		using allocation_move_adapater = AllocationMoveAdapater<value_type, allocator_type>;
	private:
		pointer arrayDataPtr;
		size_type arraySize;
	public:
		//constructor
		constexpr HeapArray() : arrayDataPtr(nullptr), arraySize(0) {}
		constexpr HeapArray(const HeapArray& other) noexcept = delete;
		constexpr HeapArray(HeapArray&& other) noexcept : arrayDataPtr(other.arrayDataPtr), arraySize(other.arraySize) {
			other.arrayDataPtr = nullptr;
			other.arraySize = 0;
		}
		constexpr HeapArray(const size_type count) noexcept : arrayDataPtr(typed_allocator_type::allocate(count)), arraySize(count) {
			constructAll();
		}
		constexpr HeapArray(const size_type count, const value_type& value) noexcept : arrayDataPtr(typed_allocator_type::allocate(count)), arraySize(count) {
			pointer fillDstPtrFirst = arrayDataPtr;
			pointer fillDstPtrLast = fillDstPtrFirst + count;
			natl::uninitializedFill<pointer, value_type>(fillDstPtrFirst, fillDstPtrLast, value);
		}
		constexpr HeapArray(const_pointer srcPtr, const size_type count) noexcept : arrayDataPtr(srcPtr), arraySize(count) {}
		constexpr HeapArray(allocation_move_adapater&& allocationMoveAdapater) {
			if (allocationMoveAdapater.isEmpty()) {
				arrayDataPtr = 0;
				arraySize = 0;
			} else if (allocationMoveAdapater.requiresCopy()) {
				arraySize = allocationMoveAdapater.capacity();
				arrayDataPtr = typed_allocator_type::allocate(arraySize);

				pointer copyDstPtr = arrayDataPtr;
				const_pointer copySrcPtrFirst = allocationMoveAdapater.data();
				const_pointer copySrcPtrLast = copySrcPtrFirst + allocationMoveAdapater.size();
				natl::uninitializedCopyNoOverlap<const_pointer, pointer>(copySrcPtrFirst, copySrcPtrLast, copyDstPtr);

				if (arraySize != allocationMoveAdapater.size()) {
					const size_type constructCount = arraySize - allocationMoveAdapater.size();
					pointer constructPtr = arrayDataPtr + allocationMoveAdapater.size();
					natl::defaultConstructAll<value_type>(constructPtr, constructCount);
				}

				if (allocationMoveAdapater.canDealloc()) {
					allocationMoveAdapater.deallocate();
				}
			} else if (allocationMoveAdapater.size() != allocationMoveAdapater.capacity()) {
				arraySize = allocationMoveAdapater.capacity();
				arrayDataPtr = allocationMoveAdapater.data();

				const size_type constructCount = arraySize - allocationMoveAdapater.size();
				pointer constructPtr = arrayDataPtr + allocationMoveAdapater.size();
				natl::defaultConstructAll<value_type>(constructPtr, constructCount);
			} else {
				arraySize = allocationMoveAdapater.capacity();
				arrayDataPtr = allocationMoveAdapater.data();
			}
			allocationMoveAdapater.release();
		}

		//destructor
		constexpr ~HeapArray() noexcept {
			deallocate();
		};
		
		//util 
		constexpr HeapArray& self() noexcept { return *this; }
		constexpr const HeapArray& self() const noexcept { return *this; }
		constexpr HeapArray& operator=(const HeapArray& other) noexcept = delete;
		constexpr HeapArray& operator=(HeapArray&& other) noexcept {
			deallocate();
			arrayDataPtr = other.arrayDataPtr;
			arraySize = other.arraySize;
			other.arrayDataPtr = nullptr;
			other.arraySize = 0;
			return self();
		}

		//assignment
		constexpr HeapArray& operator=(allocation_move_adapater&& allocationMoveAdapater) noexcept {
			deallocate();
			self() = natl::move(HeapArray(natl::move(allocationMoveAdapater)));
			return self();
		}

		//convert 
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

		[[nodiscard]] constexpr allocation_move_adapater getAlloctionMoveAdapater() noexcept {
			allocation_move_adapater allocationMoveAdapater(data(), size(), size(), AllocationMoveAdapaterRequireCopy::False, AllocationMoveAdapaterCanDealloc::True);
			arrayDataPtr = nullptr;
			arraySize = 0;
			return natl::move(allocationMoveAdapater);
		}

		//capacity 
		constexpr Bool empty() const noexcept { return size() == 0; }
		constexpr Bool isEmpty() const noexcept { return empty(); }
		constexpr Bool isNotEmpty() const noexcept { return !empty(); }

		constexpr size_type size() const noexcept { return arraySize; }

		//element access
		constexpr reference at(const size_type index) noexcept { return data()[index]; };
		constexpr const_reference at(const size_type index) const noexcept { return data()[index]; };

		constexpr reference operator[] (const size_type index) noexcept { return at(index); }
		constexpr const_reference operator[] (const size_type index) const noexcept { return at(index); }

		constexpr size_type frontIndex() const noexcept { return 0; }
		constexpr size_type backIndex() const noexcept { return size() - 1; }

		constexpr reference front() noexcept { return at(frontIndex()); }
		constexpr const_reference front() const noexcept { return at(frontIndex()); }

		constexpr reference back() noexcept { return at(backIndex()); }
		constexpr const_reference back() const noexcept { return at(backIndex()); }

		constexpr pointer data() noexcept { return arrayDataPtr; }
		constexpr const_pointer data() const noexcept { return arrayDataPtr; }

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
		constexpr size_type clampIndex(const size_type index) const noexcept { return clamp<size_type>(index, frontIndex(), backIndex()); }

		constexpr reference atClamped(const size_type index) noexcept requires(IsNotConst<value_type>) { return at(clampIndex(index)); }
		constexpr const_pointer atClamped(const size_type index) const noexcept { return at(clampIndex(index)); }

		constexpr optional_pointer optionalAt(const size_type index) noexcept {
			if (notHave(index)) { return optional_pointer(); }
			return optional_pointer(at(index));
		};
		constexpr optional_const_pointer optionalAt(const size_type index) const noexcept {
			if (notHave(index)) { return optional_const_pointer(); }
			return optional_const_pointer(at(index));
		};

		constexpr optional_pointer optionalFront() noexcept { return optionalAt(frontIndex()); }
		constexpr optional_const_pointer optionalFront() const noexcept { return optionalAt(frontIndex()); }
		constexpr optional_pointer optionalBack() noexcept { return optionalAt(backIndex()); }
		constexpr optional_const_pointer optionalBack() const noexcept { return optionalAt(backIndex()); }

		constexpr Bool has(const size_type index) const noexcept { return index < size(); }
		constexpr Bool notHave(const size_type index) const noexcept { return index >= size(); }

		//allocation 
		constexpr void deallocate() noexcept {
			if (arrayDataPtr) {
				destructAll();
				typed_allocator_type::deallocate(arrayDataPtr, size());
			}
		}
		constexpr void allocate(const size_type count) noexcept {
			deallocate();
			arrayDataPtr = typed_allocator_type::allocate(count);
			arraySize = count;
			constructAll();
		}

		constexpr void allocate(const size_type count, const value_type& value) noexcept {
			deallocate();
			arrayDataPtr = typed_allocator_type::allocate(count);
			arraySize = count;
			uninitializedFill(value);
		}
		constexpr void allocate(const_pointer srcPtr, const size_type count) noexcept {
			deallocate();
			arrayDataPtr = typed_allocator_type::allocate(count);
			arraySize = count;

			pointer copyDstPtr = arrayDataPtr;
			const_pointer copySrcPtrFirst = srcPtr;
			const_pointer copySrcPtrLast = copySrcPtrFirst + count;
			natl::uninitializedCopyNoOverlap<const_pointer, pointer>(copySrcPtrFirst, copySrcPtrLast, copyDstPtr);
		}

		//modifier 
	private:
		constexpr void constructAll() noexcept {
			if (isNotEmpty()) {
				defaultConstructAll(arrayDataPtr, size());
			}
		}
		constexpr void destructAll() noexcept {
			if (IsTriviallyDestructible<value_type>) {
				if (!isConstantEvaluated()) {
					return;
				}
			}

			if (isNotEmpty()) {
				deconstructAll(arrayDataPtr, size());
			}
		}
	public:
		constexpr void fill(const value_type& value = value_type()) noexcept {
			pointer fillDstFirst = data();
			pointer fillDstLast = fillDstFirst + size();
			natl::fill<pointer, value_type>(fillDstFirst, fillDstLast, value);
		}
	};

	template<typename DataType, typename Alloc>
	struct IsTriviallyCompareableV<HeapArray<DataType, Alloc>>
		: TrueType {};

	template<typename DataType, typename Alloc>
	struct IsTriviallyRelocatableV<HeapArray<DataType, Alloc>>
		: TrueType {};
	template<typename DataType, typename Alloc>
	struct IsTriviallyConstructibleV<HeapArray<DataType, Alloc>>
		: TrueType {};
	template<typename DataType, typename Alloc>
	struct IsTriviallyDestructibleV<HeapArray<DataType, Alloc>>
		: FalseType {};

	template<typename DataType, typename Alloc>
	struct IsTriviallyConstRefConstructibleV<HeapArray<DataType, Alloc>>
		: FalseType {};
	template<typename DataType, typename Alloc>
	struct IsTriviallyMoveConstructibleV<HeapArray<DataType, Alloc>>
		: FalseType {};

	template<typename DataType, typename Alloc>
	struct IsTriviallyConstRefAssignableV<HeapArray<DataType, Alloc>>
		: FalseType {};
	template<typename DataType, typename Alloc>
	struct IsTriviallyMoveAssignableV<HeapArray<DataType, Alloc>>
		: FalseType {};
}