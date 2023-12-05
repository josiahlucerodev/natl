#pragma once

//std
#include <bit>
#include <iostream>

//own
#include "basicTypes.h"

//interface
namespace natl {
	template <typename Alloc>
	concept IsAllocator = requires(Alloc alloc) {
			typename Alloc::value_type;
			typename Alloc::pointer;
			typename Alloc::const_pointer;
			typename Alloc::reference;
			typename Alloc::const_reference;
			typename Alloc::size_type;
			typename Alloc::difference_type;

			// Allocate and deallocate memory
			{ Alloc::allocate(std::declval<typename Alloc::size_type>()) } -> std::same_as<typename Alloc::pointer>;
			{ Alloc::deallocate(std::declval<typename Alloc::pointer>(), std::declval<typename Alloc::size_type>()) };
	};


	template<class DataType>
	class Allocator {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using difference_type = PtrDiff;
		using size_type = Size;
	public:
		constexpr Allocator() = default;
		constexpr Allocator(const Allocator&) = default;
		constexpr Allocator(Allocator&&) = default;
		template <typename U> constexpr Allocator(const Allocator<U>&) noexcept {}

		constexpr ~Allocator() = default;
		Allocator operator=(const Allocator&) {}
		Allocator operator=(Allocator&&) {}

		[[nodiscard]] constexpr static pointer allocate(const Size number) noexcept { 
			return std::allocator<DataType>().allocate(static_cast<std::size_t>(number));
		}
		constexpr void static deallocate(pointer ptr, const Size number) noexcept {
			if (!ptr) { return; }
			std::allocator<DataType>().deallocate(ptr, static_cast<std::size_t>(number));
		}
	};

	struct TrackerAllocatorData {
		i64 allocs;
		i64 deallocs;
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
		using difference_type = PtrDiff;
		using size_type = Size;
	public:
		constexpr TrackerAllocator() = default;
		constexpr TrackerAllocator(const TrackerAllocator&) = default;
		constexpr TrackerAllocator(TrackerAllocator&&) = default;
		template <typename U> constexpr TrackerAllocator(const TrackerAllocator<U>&) noexcept {}

		constexpr ~TrackerAllocator() = default;
		TrackerAllocator operator=(const TrackerAllocator&) {}
		TrackerAllocator operator=(TrackerAllocator&&) {}

		[[nodiscard]] constexpr static pointer allocate(const Size number) noexcept {
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

			std::cout << "natl: TrackerAllocator error: ptr " << reinterpret_cast<void*>(ptr) << " was not allocated from TrackerAllocator\n";
			return false;
		}

		constexpr static void deallocate(pointer ptr, const Size number) noexcept {
			std::allocator<DataType>().deallocate(ptr, number);
		}
	};

	template<class DataType>
	using DefaultAllocator = Allocator<DataType>;
}