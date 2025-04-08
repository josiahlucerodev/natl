#pragma once

//own
#include "dynArray.h"
#include "smallDynArray.h"
#include "bitArray.h"
#include "../util/uninitialized.h"
#include "../util/memory.h"

//interface
namespace natl {
	constexpr static inline Size defaultBatchSize = 32;

	template<template<typename> typename DynArrayType
		, typename DataType, Size BatchSize = defaultBatchSize
		, typename Alloc = DefaultAllocator>
		requires(BatchSize >= 1 && IsAllocatorC<Alloc>)
	struct MonotonicBatchPoolBase {
	public:
		using batchs_storage_type = DynArrayType<DataType*>;

		using allocator_type = Alloc;
		using typed_allocator_type = allocator_type::template rebind<DataType>;

		using value_type = typename typed_allocator_type::value_type;
		using reference = typename typed_allocator_type::reference;
		using const_reference = typename typed_allocator_type::const_reference;
		using pointer = typename typed_allocator_type::pointer;
		using const_pointer = typename typed_allocator_type::const_pointer;
		using difference_type = typename typed_allocator_type::difference_type;
		using size_type = typename typed_allocator_type::size_type;

	private:
		batchs_storage_type batchs;
		size_type activeBatchIndex;
		size_type activeBatchSize;

	public:
		//constructor 
		constexpr MonotonicBatchPoolBase() noexcept 
			: batchs(), activeBatchIndex(0), activeBatchSize(0) {}
		constexpr MonotonicBatchPoolBase(const MonotonicBatchPoolBase& other) = delete;

		constexpr MonotonicBatchPoolBase(MonotonicBatchPoolBase&& other) noexcept
			: batchs(move(other.batchs)), activeBatchIndex(other.activeBatchIndex)
			, activeBatchSize(other.activeBatchSize) {
			other.activeBatchIndex = 0;
			other.activeBatchSize = 0;
		}

		//destructor
	private:
		constexpr void deconstruct() noexcept {
			if (batchs.isEmpty()) {
				return;
			}

			if (activeBatchIndex > 1) {
				for (size_type i = 0; i < activeBatchIndex - 1; i++) {
					pointer batch = batchs[i];
					deconstructAll<value_type>(batch, BatchSize);
				}
			}
				
			pointer activeBatch = batchs[activeBatchIndex];
			deconstructAll<value_type>(activeBatch, activeBatchSize);

			for (pointer batch : batchs) {
				deallocateBatch(batch);
			}
		}

	public:
		constexpr ~MonotonicBatchPoolBase() noexcept {
			deconstruct();
		}

		//util 
		constexpr MonotonicBatchPoolBase& self() noexcept { return *this; }
		constexpr const MonotonicBatchPoolBase& self() const noexcept { return *this; }

		//assignment 
		constexpr MonotonicBatchPoolBase& operator=(const MonotonicBatchPoolBase& other) = delete;
		constexpr MonotonicBatchPoolBase& operator=(MonotonicBatchPoolBase&& other) noexcept {
			deconstruct();
			batchs = move(other.batchs);
			activeBatchIndex = other.activeBatchIndex;
			activeBatchSize = other.activeBatchSize;

			other.activeBatchIndex = 0;
			other.activeBatchSize = 0;
			return self();
		}

	public:
		//capacity
		constexpr size_type capacity() const noexcept { return BatchSize * batchs.size(); }
		constexpr size_type size() const noexcept {
			if (batchs.isEmpty()) {
				return 0;
			} else {
				return (BatchSize * activeBatchIndex) + batchs[activeBatchIndex].size();
			}
		}
		constexpr size_type batchSize() const noexcept { return BatchSize; }
		constexpr static size_type staticBatchSize() noexcept { return BatchSize; }
		constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
		constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
		explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

		//query 
		constexpr Bool isFrom(const_pointer ptr) const noexcept {
			if (ptr == nullptr) {
				return false;
			}

			for (pointer batch : batchs) {
				if ((ptr >= batch) && (ptr < (batch + BatchSize))) {
					return true;
				}
			}

			return false;
		}

		//modifiers 
		constexpr void reserve(const size_type newCapacity) noexcept {
			if (capacity() < newCapacity) {
				const size_type oldNumberOfBatch = batchs.size();
				const size_type numberOfNewBatchs = max<size_type>((newCapacity - capacity()) / BatchSize, 1);
				batchs.resize(batchs.size() + numberOfNewBatchs);
				for (size_type i = oldNumberOfBatch; i < batchs.size(); i++) {
					batchs[i] = allocateBatch();
				}
			}
		}
		constexpr pointer allocateConstructed() noexcept 
			requires(IsConstructibleC<value_type>) {
			activeBatchSize++;

			pointer value;
			if (batchs.isEmpty()) {
				pointer newBatch = allocateBatch();
				batchs.pushBack(newBatch);
				value = &newBatch[0];
				activeBatchIndex = 0;
				activeBatchSize = 1;
			} else if(activeBatchSize == BatchSize + 1) {
				activeBatchIndex++;
				if (activeBatchIndex == batchs.size()) {
					pointer newBatch = allocateBatch();
					batchs.pushBack(newBatch);
					value = &newBatch[0];
					activeBatchSize = 1;
				} else {
					value = &batchs[activeBatchIndex][0];
					activeBatchSize = 1;
				}
			} else {
				value = &batchs[activeBatchIndex][activeBatchSize - 1];
			}

			construct<value_type>(value);
			return value;
		}
		constexpr void clear() noexcept {
			deconstruct();
			batchs.clear();
			activeBatchIndex = 0;
			activeBatchSize = 0;
		}

	private:
		constexpr pointer allocateBatch() noexcept {
			return allocator_type::template rebind<value_type>::allocate(BatchSize);
		}
		constexpr void deallocateBatch(pointer ptr) noexcept {
			allocator_type::template rebind<value_type>::deallocate(ptr, BatchSize);
		}
	};

	template<typename Type, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	using MonotonicBatchPool = MonotonicBatchPoolBase<
		DynArrayUnboundTypeT<Alloc>::template type,
		Type, BatchSize, Alloc>;

	template<typename Type, Size SmallBatchsNumber, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	using SmallMonotonicBatchPool = MonotonicBatchPoolBase<
		SmallDynArrayUnboundTypeT<SmallBatchsNumber, Alloc>:: template type
		, Type, BatchSize, Alloc>;

	template<Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	struct MonotonicBatchPoolUnboundTypeT {
		template<typename DataType> using type = MonotonicBatchPool<DataType, BatchSize, Alloc>;
	};

	template<Size SmallBatchsNumber, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	struct SmallMonotonicBatchPoolUnboundTypeT {
		template<typename DataType> using type = SmallMonotonicBatchPool<DataType, SmallBatchsNumber, BatchSize, Alloc>;
	};

	//RecycleMonotonicBatchPool
	template<
		template<typename> typename BatchDynArrayType
		, template<typename> typename FreeSlotsDynArrayType
		, typename DataType, Size BatchSize = defaultBatchSize
		, typename Alloc = DefaultAllocator>
		requires(IsConstructibleC<DataType>)
	struct RecycleMonotonicBatchPoolBase {
	public:
		using batchs_storage_type = BatchDynArrayType<DataType*>;
		using storage_pool = MonotonicBatchPoolBase < BatchDynArrayType, DataType, BatchSize, Alloc>;
		using free_slots_storage_type = FreeSlotsDynArrayType<DataType*>;

		using allocator_type = Alloc;
		using typed_allocator_type = allocator_type::template rebind<DataType>;

		using value_type = typename typed_allocator_type::value_type;
		using reference = typename typed_allocator_type::reference;
		using const_reference = typename typed_allocator_type::const_reference;
		using pointer = typename typed_allocator_type::pointer;
		using const_pointer = typename typed_allocator_type::const_pointer;
		using difference_type = typename typed_allocator_type::difference_type;
		using size_type = typename typed_allocator_type::size_type;

	private:
		storage_pool pool;
		free_slots_storage_type freeSlots;

	public:
		//constructor
		constexpr RecycleMonotonicBatchPoolBase() noexcept = default;
		constexpr RecycleMonotonicBatchPoolBase(const RecycleMonotonicBatchPoolBase&) noexcept = delete;
		constexpr RecycleMonotonicBatchPoolBase(RecycleMonotonicBatchPoolBase&&) noexcept = default;

		//deconstructor
		constexpr ~RecycleMonotonicBatchPoolBase() noexcept = default;

		//assignment
		constexpr RecycleMonotonicBatchPoolBase& operator=(const RecycleMonotonicBatchPoolBase&) noexcept = delete;
		constexpr RecycleMonotonicBatchPoolBase& operator=(RecycleMonotonicBatchPoolBase&&) noexcept = default;

		//capacity
		constexpr size_type capacity() const noexcept { return pool.capacity(); }
		constexpr size_type size() const noexcept { return pool.size() - freeSlots.size(); }
		constexpr size_type batchSize() const noexcept { return BatchSize; }
		constexpr static size_type staticBatchSize() noexcept { return BatchSize; }
		constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
		constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
		explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

		//query 
		constexpr Bool isFrom(const_pointer ptr) const noexcept {
			return pool.isFrom(ptr);
		}

		//modifiers
		constexpr void reserve(const size_type newCapacity) noexcept {
			pool.reserve(newCapacity);
		}
		constexpr pointer allocateConstructed() noexcept {
			if (freeSlots.isEmpty()) {
				freeSlots.reserve(BatchSize);
				for (size_type i = 0; i < BatchSize; i++) {
					freeSlots.pushBack(pool.allocateConstructed());
				}
			}

			pointer value = freeSlots.back();
			freeSlots.popBack();
			return value;
		};
		constexpr void deallocate(pointer ptr) noexcept {
			if (ptr != nullptr) {
				construct<value_type>(ptr);
				freeSlots.pushBack(ptr);
			}
		};
		constexpr Bool checkedDeallocate(pointer ptr) noexcept {
			if (!isFrom(ptr)) {
				return false;
			}
			deallocate(ptr);
		};
		constexpr void clear() noexcept {
			pool.clear();
			freeSlots.clear();
		}
	};

	template<typename Type, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	using RecycleMonotonicBatchPool = RecycleMonotonicBatchPoolBase<
		DynArrayUnboundTypeT<Alloc>::template type
		, DynArrayUnboundTypeT<Alloc>::template type
		, Type, BatchSize, Alloc>;

	template<Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	struct RecycleMonotonicBatchPoolUnboundTypeT {
		template<typename DataType> using type = RecycleMonotonicBatchPool<DataType, BatchSize, Alloc>;
	};

	template<typename Type, Size SmallBatchsNumber, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	using SmallRecycleMonotonicBatchPool = RecycleMonotonicBatchPoolBase<
		SmallDynArrayUnboundTypeT<SmallBatchsNumber, Alloc>::template type
		, DynArrayUnboundTypeT<Alloc>::template type
		, Type, BatchSize, Alloc>;

	template<Size SmallBatchsNumber, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	struct SmallRecycleMonotonicBatchPoolUnboundTypeT {
		template<typename DataType> using type = SmallRecycleMonotonicBatchPool<DataType, SmallBatchsNumber, BatchSize, Alloc>;
	};

	//BatchPool
	template<
		template<typename> typename BatchsDynArrayType
		, template<typename> typename FreeSlotsDynArrayType
		, typename DataType, Size BatchSize = defaultBatchSize
		, typename Alloc = DefaultAllocator>
		requires(BatchSize >= 1 && IsAllocatorC<Alloc>)
	struct BatchPoolBase {
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

		struct BatchElement {
			Size active;
			UninitializedValue<value_type> data;
		};

		//array of bytes that contain 
		//active_mask
		//BatchElement[]
		using Batch = BatchElement*;

		template<typename Type> 
		using batchs_dyn_array_type = BatchsDynArrayType<Type>;
		template<typename Type>
		using free_slots_dyn_array_type = FreeSlotsDynArrayType<Type>;

		using batchs_storage_type = batchs_dyn_array_type<Batch>;
		using free_slots_storage_type = free_slots_dyn_array_type<DataType*>;

	private:
		batchs_storage_type batchs;
		free_slots_storage_type freeSlots;
		Size poolSize;

	public:
		//constructor
		constexpr BatchPoolBase() noexcept : batchs(), freeSlots(), poolSize() {}
		constexpr BatchPoolBase(const BatchPoolBase&) noexcept = delete;
		constexpr BatchPoolBase(BatchPoolBase&& other) noexcept : 
			batchs(move(other.batchs)), freeSlots(move(other.freeSlots)), poolSize(poolSize) {
			other.poolSize = 0;
		}

		//destructor
	private:
		constexpr void deconstruct() noexcept {
			for (Batch& batch : batchs) {
				BatchElement* elements = batch;
				for (Size i = 0; i < BatchSize; i++) {
					BatchElement& element = elements[i];
					if (element.active) {
						element.data.deconstruct();
					}
				}
			}
		}

	public:
		constexpr ~BatchPoolBase() noexcept {
			deconstruct();
		}

		//assignment
		constexpr BatchPoolBase& operator=(const BatchPoolBase&) = delete;
		constexpr BatchPoolBase& operator=(BatchPoolBase&& other) noexcept {
			deconstruct();
			batchs = move(other.batchs);
			freeSlots = move(other.freeSlots);
			poolSize = other.poolSize;

			other.poolSize = 0;
		}
			
		//capacity
		constexpr size_type capacity() const noexcept { return BatchSize * batchs.size(); }
		constexpr size_type size() const noexcept {
			return poolSize;
		}
		constexpr size_type batchSize() const noexcept { return BatchSize; }
		constexpr static size_type staticBatchSize() noexcept { return BatchSize; }
		constexpr Bool isEmpty() const noexcept { return !Bool(size()); }
		constexpr Bool isNotEmpty() const noexcept { return Bool(size()); }
		explicit constexpr operator Bool() const noexcept { return isNotEmpty(); }

		//query 
		constexpr Bool isFrom(const_pointer ptr) const noexcept {
			if (ptr == nullptr) {
				return false;
			}

			for (Batch& batch : batchs) {
				const BatchElement* elements = getElementsOfBatch(batch);
				if ((ptr >= elements) && (ptr < (elements + BatchSize))) {
					return true;
				}
			}

			return false;
		}

		//modifiers 
		constexpr void reserve(const size_type newCapacity) noexcept {
			if (capacity() < newCapacity) {
				const size_type oldNumberOfBatch = batchs.size();
				const size_type numberOfNewBatchs = max<size_type>((newCapacity - capacity()) / BatchSize, 1);
				batchs.resize(batchs.size() + numberOfNewBatchs);
				freeSlots.reserve(freeSlots.size() + (numberOfNewBatchs * BatchSize));
				for (size_type i = oldNumberOfBatch; i < batchs.size(); i++) {
					Batch batch = allocateBatch();
					BatchElement* elements = batch;
					batchs[i] = batch;
					for (Size i = 0; i < BatchSize; i++) {
						freeSlots.pushBack(elements[i].data.valueAsPtr());
					}
				}
			}
		}
		constexpr pointer allocate() noexcept {
			if (freeSlots.isEmpty()) {
				reserve(capacity() + BatchSize);
			}

			pointer ptr = freeSlots.back();
			freeSlots.popBack();
			return ptr;
		};
		constexpr pointer allocateConstructed() noexcept requires(IsConstructibleC<value_type>) {
			if (freeSlots.isEmpty()) {
				reserve(capacity() + BatchSize);
			}

			pointer ptr = freeSlots.back();
			freeSlots.popBack();
			natl::construct<value_type>(ptr);
			return ptr;
		};
		constexpr void deallocate(pointer ptr) noexcept {
			if (ptr == nullptr) {
				return;
			}

			Byte* elementStorage = reinterpret_cast<Byte*>(ptr) - memberOffset<Bool, value_type>();
			BatchElement* batchElement = reinterpret_cast<BatchElement*>(elementStorage);
			natl::deconstruct<value_type>(batchElement->data.valueAsPtr());
			batchElement->active = false;
			freeSlots.pushBack(batchElement->data.valueAsPtr());
		}
		constexpr bool checkedDeallocate(pointer ptr) noexcept {
			if (!isFrom(ptr)) {
				return false;
			}
			deallocate(ptr);
			return true;
		}
		constexpr void clear() noexcept {
			deconstruct();
			batchs.clear();
			freeSlots.clear();
			poolSize = 0;
		}

	private:
		inline Batch allocateBatch() noexcept {
			BatchElement* elements = allocator_type::template rebind<BatchElement>::allocate(BatchSize);
			for (Size i = 0; i < BatchSize; i++) {
				BatchElement* element = &elements[i];
				natl::construct<BatchElement>(element);
				element->active = false;
			}

			return elements;
		}

		inline void deallocateBatch(Batch batch) noexcept {
			BatchElement* elements = batch;
			for (Size i = 0; i < BatchSize; i++) {
				BatchElement& element = elements[i];
				if (element.active) {
					element.data.deconstruct();
				}
			}

			allocator_type::template rebind<BatchElement>::deallocate(batch, BatchSize);
		}
	};


	template<typename Type, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	using BatchPool = BatchPoolBase<
		DynArrayUnboundTypeT<Alloc>::template type
		, DynArrayUnboundTypeT<Alloc>::template type
		, Type, BatchSize, Alloc>;

	template<typename Type, Size SmallBatchsNumber, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	using SmallBatchPool = BatchPoolBase<
		SmallDynArrayUnboundTypeT<SmallBatchsNumber, Alloc>::template type
		, DynArrayUnboundTypeT<Alloc>::template type
		, Type, BatchSize, Alloc>;

	template<Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	struct BatchPoolUnboundTypeT {
		template<typename DataType> using type = BatchPool<DataType, BatchSize, Alloc>;
	};

	template<Size SmallBatchsNumber, Size BatchSize = defaultBatchSize, typename Alloc = DefaultAllocator>
		requires(IsAllocatorC<Alloc>)
	struct SmallBatchPoolUnboundTypeT {
		template<typename DataType> using type = SmallBatchPool<DataType, SmallBatchsNumber, BatchSize, Alloc>;
	};
}