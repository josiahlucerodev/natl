#pragma once

//own
#include "dynArray.h"
#include "smallDynArray.h"

//interface
namespace natl {
	namespace impl {
		template<typename Type, typename BatchDynArray, Size BatchSize>
		class BatchPoolBase {
		private:
			BatchDynArray batchs;
			DynArray<Type>* activeBatch;
		public:
			constexpr BatchPoolBase() noexcept : batchs(), activeBatch(nullptr) {}
			constexpr ~BatchPoolBase() noexcept = default;
		public:
			constexpr inline Size capacity() const noexcept { return BatchSize * batchs.size(); }
			constexpr inline Size size() const noexcept { return batchs.size() == 0 ? 0 : ((batchs.size() - 1) * BatchSize) + activeBatch->size(); }
			constexpr inline static Size batchSize() noexcept { return BatchSize; }
			constexpr inline Type* newElement() noexcept {
				if (needNewBatch()) {
					newBatch();
				}
				activeBatch->resize(activeBatch->size() + 1);
				return &activeBatch->back();
			};
			constexpr void clear() noexcept {
				batchs.clear();
				activeBatch = nullptr;
			}
		private:
			constexpr inline void newBatch() noexcept {
				batchs.resize(batchs.size() + 1);
				activeBatch = &batchs.back();
				activeBatch->reserve(BatchSize);
			}
			constexpr inline bool needNewBatch() noexcept {
				return activeBatch ? activeBatch->size() == activeBatch->capacity() : true;
			}
		};

		template<class DataType, typename BatchDynArray, std::size_t BatchSize = 100>
		class BatchHeapBase {
		private:
			BatchPoolBase<DataType, BatchDynArray, BatchSize> pool;
			DynArray<DataType*> freeSlots;
		public:
			constexpr BatchHeapBase() : pool(), freeSlots() {}
		private:

			constexpr bool isFreeSlotsEmpty() noexcept {
				return freeSlots.isEmpty();
			}
			constexpr inline void allocNewElements() {
				if (!isFreeSlotsEmpty()) {
					return;
				}
				freeSlots.reserve(pool.size() + BatchSize);

				for (std::size_t i = 0; i < BatchSize; i++) {
					freeSlots.push_back(pool.newElement());
				}
			}
		public:
			constexpr inline DataType* newElement() noexcept {
				allocNewElements();
				DataType* element = freeSlots.front();
				freeSlots.pop_back();
				return element;
			};
			constexpr inline std::size_t capacity() noexcept { return pool.capacity(); }
			constexpr inline std::size_t size() noexcept { return pool.size(); }
		};
	}

	template<typename Type, Size BatchSize = 100, typename Alloc = DefaultAllocator<Type>>
		requires(IsAllocator<Alloc>)
	using BatchPool = impl::BatchPoolBase<Type, DynArray<DynArray<Type, Alloc>, typename Alloc::template rebind_alloc<DynArray<Type, Alloc>>>, BatchSize>;

	template<typename Type, Size SmallBatchsNumber, Size BatchSize = 100, typename Alloc = DefaultAllocator<Type>>
		requires(IsAllocator<Alloc>)
	using SmallBatchPool = impl::BatchPoolBase<Type, SmallDynArray<DynArray<Type, Alloc>, SmallBatchsNumber, typename Alloc::template rebind_alloc<DynArray<Type, Alloc>>>, BatchSize>;

	template<typename Type, Size BatchSize = 100, typename Alloc = DefaultAllocator<Type>>
		requires(IsAllocator<Alloc>)
	using BatchHeap = impl::BatchHeapBase<Type, DynArray<DynArray<Type, Alloc>, typename Alloc::template rebind_alloc<DynArray<Type, Alloc>>>, BatchSize>;

	template<typename Type, Size SmallBatchsNumber, Size BatchSize = 100, typename Alloc = DefaultAllocator<Type>>
		requires(IsAllocator<Alloc>)
	using SmallBatchHeap = impl::BatchHeapBase<Type, SmallDynArray<DynArray<Type, Alloc>, SmallBatchsNumber, typename Alloc::template rebind_alloc<DynArray<Type, Alloc>>>, BatchSize>;
}