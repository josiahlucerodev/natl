#pragma once

//std
#include <bit>
#include <iostream>
#include <unordered_map>
#include <algorithm>

//own
#include "basicTypes.h"
#include "compilerDependent.h"
#include "typeTraits.h"
#include "bits.h"

//interface
namespace natl {
	constexpr static Bool natl_enable_allocator_tracking = true;

	struct TrackerAllocatorData {
		i64 allocs;
		i64 deallocs;
		std::unordered_map<void*, void*> allocMap;
		TrackerAllocatorData() noexcept {
			allocs = 0;
			deallocs = 0;
		}
		~TrackerAllocatorData() noexcept {
			if (!allocMap.empty()) {
				std::cout << "natl: TrackerAllocatorData error: alloc and decall mismatch - allocs: " << allocs << " deallocs: " << deallocs << " \n";
				for (const std::pair<void *const, void*>& allocPair : allocMap) {
					std::cout << "natl: TrackerAllocatorData error: ptr: " << allocPair.first << " was not deallocated\n";
				}
			} else {
				std::cout << "natl: TrackerAllocatorData info: number of allocs and deallocs: " << allocs << " \n";
			}
		}
		void registerAllocate(void* ptr) noexcept {
			allocs++;
			allocMap.insert({ptr, ptr});
		}
		void registerDeallocate(void* ptr) noexcept {
			deallocs++;
			typename std::unordered_map<void*, void*>::iterator findIter = allocMap.find(ptr);
			if (findIter == allocMap.end()) {
				std::cout << "natl: TrackerAllocator error: ptr " << ptr << " was not allocated from TrackerAllocator\n";
				if constexpr (natlInDebug()) {
					natlDebugBreak();
				}
			} else {
				allocMap.erase(ptr);
			}
		}
	};

	extern TrackerAllocatorData trackerAllocatorData;

#ifdef NATL_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4180)
#endif // NATL_COMPILER_MSVC

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

		template <class Other>
		using rebind_alloc = TrackerAllocator<Other>;
	public:
		TrackerAllocator() = default;
		TrackerAllocator(const TrackerAllocator&) = default;
		TrackerAllocator(TrackerAllocator&&) = default;
		template <typename U> constexpr TrackerAllocator(const TrackerAllocator<U>&) noexcept {}
		~TrackerAllocator() = default;

		constexpr static void registerAllocate(const pointer ptr) noexcept {
			if (isConstantEvaluated()) {
				return;
			}
			trackerAllocatorData.registerAllocate(natl::bitCast<void*, const pointer>(ptr));
		}
		constexpr static void registerDeallocate(const pointer ptr) noexcept {
			if (isConstantEvaluated()) {
				return;
			}
			trackerAllocatorData.registerDeallocate(natl::bitCast<void*, const pointer>(ptr));
		}
	};


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
			{ Alloc::allocate(natl::declval<typename Alloc::size_type>()) } -> natl::SameAs<typename Alloc::pointer>;
			{ Alloc::deallocate(natl::declval<typename Alloc::pointer>(), natl::declval<typename Alloc::size_type>()) };
	};

	template<class Alloc>
	struct AllocatorTraits {
		using allocator_type = Alloc;

		using value_type = typename Alloc::value_type;
		using reference = typename Alloc::reference;
		using const_reference = typename Alloc::const_reference;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using difference_type = typename Alloc::difference_type;
		using size_type = typename Alloc::size_type;

		using void_pointer = typename Alloc::void_pointer;
		using const_void_pointer = typename Alloc::const_void_pointer;

		template <class Other>
		using rebind_alloc = typename Alloc:: template rebind_alloc<Other>;

		template <class Other>
		using rebind_traits = AllocatorTraits<rebind_alloc<Other>>;
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

		template <class Other>
		using rebind_alloc = Allocator<Other>;
	public:
		constexpr Allocator() = default;
		constexpr Allocator(const Allocator&) = default;
		constexpr Allocator(Allocator&&) = default;
		template <typename U> constexpr Allocator(const Allocator<U>&) noexcept {}

		constexpr ~Allocator() = default;
		Allocator operator=(const Allocator&) {}
		Allocator operator=(Allocator&&) {}

		[[nodiscard]] constexpr static pointer allocate(const Size number) noexcept { 
			pointer ptr = std::allocator<DataType>().allocate(static_cast<StdSize>(number));
			if constexpr (natl_enable_allocator_tracking) {
				TrackerAllocator<DataType>::registerAllocate(ptr);
			}
			return ptr;
		}
		constexpr void static deallocate(pointer ptr, const Size number) noexcept {
			if (!ptr) { return; }
			if constexpr (natl_enable_allocator_tracking) {
				TrackerAllocator<DataType>::registerDeallocate(ptr);
			}
			std::allocator<DataType>().deallocate(ptr, static_cast<StdSize>(number));
		}
	};

	template<class DataType>
	using DefaultAllocator = Allocator<DataType>;
	using DefaultAllocatorByte = DefaultAllocator<natl::Byte>;

	template<typename Alloc>
		requires(IsAllocator<Alloc>)
	class AllocatorArg {};

	using DefaultAllocatorArgByte = AllocatorArg<DefaultAllocatorByte>;

#ifdef NATL_COMPILER_MSVC
#pragma warning(pop)
#endif // NATL_COMPILER_MSVC

}