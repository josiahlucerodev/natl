#pragma once

//std
#include <cstdlib>

//own
#include "basicTypes.h"
#include "compilerDependent.h"
#include "typeTraits.h"
#include "bits.h"
#include "rawTypeInfo.h"
#include "../fundamental/observerPtr.h"
#include "../fundamental/optionPtr.h"

//interface
namespace natl {

	namespace impl {
		inline void* alignedAllocate(Size byteSize, Size alignment) noexcept {
#if defined(NATL_COMPILER_MSVC)
			return _aligned_malloc(byteSize, alignment);
#else
			return std::aligned_alloc(byteSize, alignment);
#endif
		}

		inline void alignedFree(void* ptr) noexcept {
#if defined(NATL_COMPILER_MSVC)
			_aligned_free(ptr);
#else
			std::free(ptr);
#endif
		}
	}

	struct TrackedAllocator;
	using TrackedAllocatorId = Id;

	enum struct TrackAllocatorError {
		none = 0,
		noExecutionSession,
		nullAllocationGlobalTracker,
		wrongStorage,
	};

	enum struct UntrackAllocatorError {
		none = 0,
		noExecutionSession,
		nullAllocationGlobalTracker,
		nullTrackedAllocator,
		wrongStorage,
		allocaterNotFound,
	};

	using TrackedAllocatorDeallocateFunc = void(*)(void*, Size);

	struct TrackAllocatorResult {
	public:
		ObserverPtr<TrackedAllocator> trackedAllocator;
		TrackAllocatorError error;
	public:
		inline Bool hasError() {
			return error != TrackAllocatorError::none;
		}
		inline Bool hasTraker() {
			return error == TrackAllocatorError::none;
		}
	};

	struct TopTrackedAllocator {
	public:
		OptionPtr<TrackedAllocator> trackedAllocator;

	public:
		constexpr OptionPtr<TrackedAllocator> getTrackedAllocator() noexcept {
			return trackedAllocator;
		}
		constexpr void setTrackedAllocator(TrackedAllocator* trackedAllocatorIn) noexcept {
			trackedAllocator = trackedAllocatorIn;
		}
	};

	struct GlobalTracker;
	TrackAllocatorResult trackAllocator(ObserverPtr<GlobalTracker> allocationGlobalTraker, 
		TrackedAllocatorDeallocateFunc deallocateFunc, OptionPtr<TopTrackedAllocator> topTrackedAllocator = nullptr) noexcept;
	UntrackAllocatorError untrackAllocator(ObserverPtr<GlobalTracker> allocationGlobalTraker, 
		ObserverPtr<TrackedAllocator> trackedAllocator) noexcept;


	enum struct TrackAllocationError {
		none = 0,
		nullTrackedAllocator,
		alreadyExist,
	};
	enum struct UntrackAllocationError {
		none = 0,
		nullTrackedAllocator,
		notFound,
	};

	TrackAllocationError trackAllocation(ObserverPtr<TrackedAllocator> trackedAllocator, const void* ptr, const Size size) noexcept;
	UntrackAllocationError untrackAllocation(ObserverPtr<TrackedAllocator> trackedAllocator, const void* ptr, const Size size) noexcept;

#ifdef NATL_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4180)
#endif // NATL_COMPILER_MSVC

	template <typename Alloc>
	concept IsAllocatorC = requires(Alloc alloc) {
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
			{ typename Alloc::value_type{} } -> IsSameC<natl::Byte>;
	};
	template<typename Type> constexpr static Bool IsAllocator = IsAllocatorC<Type>;
	template<typename Type> struct IsAllocatorV : BoolConstant<IsAllocatorC<Type>> {};

	template<typename Alloc>
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

		template <typename Other>
		using rebind = typename Alloc:: template rebind<Other>;

		template <typename Other>
		using rebind_traits = AllocatorTraits<rebind<Other>>;
	};

	using AllocatorDefaultType = natl::Byte;
	template<typename DataType = AllocatorDefaultType>
	struct UntrackedStandardAllocator {
	public:
		using value_type = DataType;
		using reference = DataType&;
		using const_reference = const DataType&;
		using pointer = DataType*;
		using const_pointer = const DataType*;
		using difference_type = PtrDiff;
		using size_type = Size;

		template <typename Other>
		using rebind = UntrackedStandardAllocator<Other>;
		using allocator_type = UntrackedStandardAllocator<value_type>;

	public:
		[[nodiscard]] constexpr static pointer allocate(const Size number) noexcept {
			pointer ptr;
			if (isConstantEvaluated()) {
				ptr = std::allocator<DataType>().allocate(static_cast<StdSize>(number));
			} else {
				ptr = reinterpret_cast<pointer>(impl::alignedAllocate(sizeof(DataType) * number, alignof(DataType)));
			}
			return ptr;
		}
		[[nodiscard]] constexpr static pointer allocateAligned(const Size number, const Size alignment) noexcept {
			auto maxFunc = [](Size s1, Size s2) -> Size {
				return (s1 > s2) ? s1 : s2;
				};

			pointer ptr;
			if (isConstantEvaluated()) {
				ptr = std::allocator<DataType>().allocate(static_cast<StdSize>(number));
			} else {
				ptr = reinterpret_cast<pointer>(impl::alignedAllocate(sizeof(DataType) * number, maxFunc(alignof(DataType), alignment)));
			}
			return ptr;
		}

		constexpr void static deallocate(pointer ptr, const Size number) noexcept {
			if (!ptr) { return; }
			if (isConstantEvaluated()) {
				std::allocator<DataType>().deallocate(ptr, static_cast<StdSize>(number));
			} else {
				impl::alignedFree(reinterpret_cast<void*>(ptr));
			}
		}
	};

	Bool isAllocationTrackingEnabled() noexcept;
	Bool enableAllocationTracking() noexcept;
	OptionPtr<GlobalTracker> getAllocationGlobalTracker() noexcept;

	namespace impl {
		void standardAllocatorTrack(void* ptr, const Size size) noexcept;
		void standardAllocatorUntrack(void* ptr, const Size size) noexcept;
	}
	Bool isStandardAllocatorTrackingEnabled() noexcept;
	Bool enableStadardAllocatorTracking() noexcept;

	template<typename DataType = AllocatorDefaultType>
	struct StandardAllocator {
	public:
		using fallback_allocator = UntrackedStandardAllocator<Byte>;
		using typed_fallback_allocator = UntrackedStandardAllocator<DataType>;
		using support_allocator = UntrackedStandardAllocator<Byte>;
		using typed_support_allocator = UntrackedStandardAllocator<DataType>;

		using value_type = typed_fallback_allocator::value_type;
		using reference = typed_fallback_allocator::reference;
		using const_reference = typed_fallback_allocator::const_reference;
		using pointer = typed_fallback_allocator::pointer;
		using const_pointer = typed_fallback_allocator::const_pointer;
		using difference_type = typed_fallback_allocator::difference_type;
		using size_type = typed_fallback_allocator::size_type;

		template <typename Other>
		using rebind = StandardAllocator<Other>;
		using allocator_type = StandardAllocator<Byte>;
		using typed_allocator_type = StandardAllocator<DataType>;
	public:
		[[nodiscard]] constexpr static pointer allocate(const Size number) noexcept {
			pointer ptr = typed_fallback_allocator::allocate(number);
			if (!isConstantEvaluated()) {
				impl::standardAllocatorTrack(reinterpret_cast<void*>(ptr), number * sizeof(value_type));
			}
			return ptr;
		}
		[[nodiscard]] constexpr static pointer allocateAligned(const Size number, const Size alignment) noexcept {
			pointer ptr = typed_fallback_allocator::allocateAligned(number, alignment);
			if (!isConstantEvaluated()) {
				impl::standardAllocatorTrack(reinterpret_cast<void*>(ptr), number * sizeof(value_type));
			}
			return ptr;
		}

		constexpr void static deallocate(pointer ptr, const Size number) noexcept {
			if (!ptr) { 
				return; 
			}
			if (!isConstantEvaluated()) {
				impl::standardAllocatorUntrack(reinterpret_cast<void*>(ptr), number * sizeof(value_type));
			}
			typed_fallback_allocator::deallocate(ptr, number);
		}
	};

	using DefaultAllocator = StandardAllocator<AllocatorDefaultType>;
	
	template<typename Alloc>
		requires(IsAllocatorC<Alloc>)
	struct AllocatorArg {};
	using DefaultAllocatorArg = AllocatorArg<DefaultAllocator>;

#ifdef NATL_COMPILER_MSVC
#pragma warning(pop)
#endif // NATL_COMPILER_MSVC

}