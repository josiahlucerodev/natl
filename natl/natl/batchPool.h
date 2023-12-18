#pragma once

//own
#include "dynArray.h"
#include "smallDynArray.h"

//interface
namespace natl {
	template<class Type, std::size_t BatchSize = 100>
	class BatchPool {
	private:
		SmallDynArray<DynArray<Type>, 5> batchs;
		DynArray<Type>* activeBatch;
	public:
		constexpr BatchPool() : batchs(), activeBatch(nullptr) {}
	public:
		constexpr inline std::size_t capacity() const noexcept { return BatchSize * batchs.size(); }
		constexpr inline std::size_t size() const noexcept { return batchs.size() == 0 ? 0 : ((batchs.size() - 1) * BatchSize) + activeBatch->size(); }
		constexpr inline static std::size_t batchSize() const noexcept { return BatchSize; }
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

	template<class DataType, std::size_t BatchSize = 100>
	class BatchHeap {
	private:
		BatchPool<DataType, BatchSize> pool;
		DynArray<DataType*> freeSlots;
	public:
		constexpr BatchHeap() : pool(), freeSlots() {}
	private:

		constexpr bool isFreeSlotsEmpty() noexcept {
			return freeSlots.isEmpty();
		}
		constexpr inline void allocNewElements() {
			if (!isFreeSlotsEmpty()) {
				return;
			}

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