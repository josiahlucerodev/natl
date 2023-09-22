#pragma once

//own
#include <cstdint>
#include <iostream>
#include <bit>

//interface
namespace natl {

	template<class DataType>
	class Allocator {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
	public:
		constexpr Allocator() = default;
		constexpr Allocator(const Allocator&) = default;
		constexpr Allocator(Allocator&&) = default;
		template <typename U> constexpr Allocator(const Allocator<U>&) noexcept {}

		constexpr ~Allocator() = default;
		Allocator operator=(const Allocator&) {}
		Allocator operator=(Allocator&&) {}

		[[nodiscard]] constexpr pointer allocate(const std::size_t number) noexcept { 
			return std::allocator<DataType>().allocate(number);
		}
		constexpr void deallocate(const pointer ptr, const std::size_t number) noexcept { 
			if (!ptr) { return; }
			std::allocator<DataType>().deallocate(ptr, number);
		}
		constexpr void deallocate(const pointer ptr) noexcept { 
			if (!ptr) { return; }
			std::allocator<DataType>().deallocate(ptr, 0);
		}
	};

	template<class DataType, class Alloc = Allocator<DataType>>
	class AllocatorStorage {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		Alloc allocator;
	public:
		constexpr AllocatorStorage(const Alloc& allocator = Alloc()) : allocator(allocator) {}
		constexpr ~AllocatorStorage() = default;
		[[nodiscard]] constexpr pointer allocate(const std::size_t number) noexcept { return allocator.allocate(number); }
		constexpr void deallocate(const pointer ptr, const std::size_t number) noexcept { return allocator.deallocate(ptr, number); }
		constexpr void deallocate(const pointer ptr) noexcept { allocator.deallocate(ptr); }
		constexpr Alloc& getAllocator(const pointer ptr) noexcept { return allocator; }
		constexpr const Alloc& getAllocator(const pointer ptr) const noexcept { return allocator; }
	};

	struct TrackerAllocatorData {
		std::uint64_t allocs;
		std::uint64_t deallocs;
		std::vector<void*> allocPtrs;
		std::vector<void*> deallocPtrs;
	};

	extern TrackerAllocatorData trackerAllocatorData;

	template<class DataType>
	class TrackerAllocator {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
	public:
		constexpr TrackerAllocator() = default;
		constexpr TrackerAllocator(const TrackerAllocator&) = default;
		constexpr TrackerAllocator(TrackerAllocator&&) = default;
		template <typename U> constexpr TrackerAllocator(const TrackerAllocator<U>&) noexcept {}

		constexpr ~TrackerAllocator() = default;
		TrackerAllocator operator=(const TrackerAllocator&) {}
		TrackerAllocator operator=(TrackerAllocator&&) {}

		[[nodiscard]] constexpr pointer allocate(const std::size_t number) noexcept { 
			pointer dataPtr = std::allocator<DataType>().allocate(number);
			trackerAllocatorData.allocs++;
			trackerAllocatorData.allocPtrs.push_back(dataPtr);
			return dataPtr;
		}

		constexpr static bool havePtr(const pointer ptr) noexcept {
			for (void* testPtr : trackerAllocatorData.allocPtrs) {
				if (std::bit_cast<void*, const pointer>(ptr) == testPtr) {
					return true;
				}
			}

			std::cout << "natl: TrackerAllocator error: ptr " << ptr << " was not allocated from TrackerAllocator\n";
			return false;
		}

		constexpr void deallocate(const pointer ptr) noexcept {
			if (!ptr || !havePtr(ptr)) { return; }
			trackerAllocatorData.deallocs++;
			trackerAllocatorData.deallocPtrs.push_back(ptr);
			std::allocator<DataType>().deallocate(ptr, 0);
		}
		constexpr void deallocate(const pointer ptr, const std::size_t number) noexcept {
			std::allocator<DataType>().deallocate(ptr, number);
		}
	};

	template<class DataType>
	using DefaultAllocator = Allocator<DataType>;
}