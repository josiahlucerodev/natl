#pragma once

//@begin_non_modules
//std
#include <memory>

//own
#include "compilerDependent.h"
//@end_non_modules

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "bits.h"
#include "rawTypeInfo.h"
#include "observerPtr.h"
#include "optionPtr.h"

//@export
namespace natl {
	constexpr inline Size defaultAllocateAlignment = 8;
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

	enum struct AllocateFlags {
		none = 0,
		zeroed = 1 << 1,
		remappable = 1 << 2,
		largePages = 1 << 3,
	};
	NATL_ADD_BIT_FLAG_OPERATIONS(AllocateFlags);

	template<typename Type>
	struct AllocResult {
	public:
		using value_type = Type;
	public:
		Type* mPtr;
		Size mSize;
	};
	using ByteAllocResult = AllocResult<Byte>;

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
			typename Alloc::alloc_result;

			// Allocate and deallocate memory
			//{ Alloc::allocate(natl::declval<typename Alloc::size_type>()) } -> natl::SameAs<typename Alloc::alloc_result>;
			//{ Alloc::allocate(natl::declval<typename Alloc::size_type>()) } -> natl::SameAs<typename Alloc::pointer>;
			{ Alloc::deallocate(natl::declval<typename Alloc::pointer>(), natl::declval<typename Alloc::size_type>()) };
			{ Alloc::deallocate(natl::declval<typename Alloc::pointer>()) };
			{ typename Alloc::value_type{} } -> IsSameC<natl::Byte>;
	};
	template<typename Type> constexpr inline Bool IsAllocator = IsAllocatorC<Type>;
	template<typename Type> struct IsAllocatorV : BoolConstant<IsAllocatorC<Type>> {};

	template<typename Alloc, typename Type>
	struct AllocatorTraits {
		using allocator_type = Alloc;

		using value_type = Type;
		using reference = Type&;
		using const_reference = const Type&;
		using pointer = Type*;
		using const_pointer = const Type*;
		using difference_type = PtrDiff;
		using size_type = Size;

		using void_pointer = void*;
		using const_void_pointer = const void*;

		using alloc_result = AllocResult<Type>;
	};

	Byte* stdAlignedAllocate(Size alignment, Size size) noexcept;
	void stdAlignedDeallocate(Byte* ptr) noexcept;

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
		using alloc_result = AllocResult<DataType>;

		template <typename Other>
		using rebind = UntrackedStandardAllocator<Other>;
		using allocator_type = UntrackedStandardAllocator<value_type>;

	public:
		[[nodiscard]] constexpr static pointer allocate(const Size number) noexcept {
			pointer ptr;
			if (isConstantEvaluated()) {
				ptr = std::allocator<DataType>().allocate(static_cast<StdSize>(number));
			} else {
				ptr = reinterpret_cast<pointer>(stdAlignedAllocate(alignof(DataType), sizeof(DataType) * number));
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
				ptr = reinterpret_cast<pointer>(stdAlignedAllocate(maxFunc(alignof(DataType), alignment), sizeof(DataType) * number));
			}
			return ptr;
		}

		constexpr void static deallocate(pointer ptr, const Size number) noexcept {
			if (!ptr) { return; }
			if (isConstantEvaluated()) {
				std::allocator<DataType>().deallocate(ptr, static_cast<StdSize>(number));
			} else {
				stdAlignedDeallocate(reinterpret_cast<Byte*>(ptr));
			}
		}
		void static deallocate(pointer ptr) noexcept {
			if (!ptr) { return; }
			stdAlignedDeallocate(reinterpret_cast<Byte*>(ptr));
		}
	};

	Bool isAllocationTrackingEnabled() noexcept;
	Bool enableAllocationTracking() noexcept;
	OptionPtr<GlobalTracker> getAllocationGlobalTracker() noexcept;

	namespace impl {
		void standardAllocatorTrack(void* ptr, const Size size) noexcept;
		void standardAllocatorUntrack(void* ptr) noexcept;
	}
	Bool isStandardAllocatorTrackingEnabled() noexcept;
	Bool enableStadardAllocatorTracking() noexcept;

	template<typename DataType = AllocatorDefaultType>
	struct StandardAllocator {
	public:
		using support_allocator = UntrackedStandardAllocator<Byte>;
		using typed_support_allocator = UntrackedStandardAllocator<DataType>;

		using value_type = typed_support_allocator::value_type;
		using reference = typed_support_allocator::reference;
		using const_reference = typed_support_allocator::const_reference;
		using pointer = typed_support_allocator::pointer;
		using const_pointer = typed_support_allocator::const_pointer;
		using difference_type = typed_support_allocator::difference_type;
		using size_type = typed_support_allocator::size_type;
		using alloc_result = AllocResult<DataType>;

		template <typename Other>
		using rebind = StandardAllocator<Other>;
		using allocator_type = StandardAllocator<Byte>;
		using typed_allocator_type = StandardAllocator<DataType>;

		constexpr static inline Bool isMeshable = false;

	public:
		[[nodiscard]] constexpr static pointer allocate(const Size number) noexcept {
			pointer ptr = typed_support_allocator::allocate(number);
			if (!isConstantEvaluated()) {
				impl::standardAllocatorTrack(reinterpret_cast<void*>(ptr), number * sizeof(value_type));
			}
			return ptr;
		}
		[[nodiscard]] constexpr static pointer allocateAligned(const Size number, const Size alignment) noexcept {
			pointer ptr = typed_support_allocator::allocateAligned(number, alignment);
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
				impl::standardAllocatorUntrack(reinterpret_cast<void*>(ptr));
			}
			typed_support_allocator::deallocate(ptr, number);
		}
		void static deallocate(pointer ptr) noexcept {
			if (!ptr) {
				return;
			}
			impl::standardAllocatorUntrack(reinterpret_cast<void*>(ptr));
			typed_support_allocator::deallocate(ptr);
		}
	};

	using DefaultAllocator = StandardAllocator<AllocatorDefaultType>;
	
	template<typename Alloc>
		requires(IsAllocatorC<Alloc>)
	struct AllocatorArg {};
	using DefaultAllocatorArg = AllocatorArg<DefaultAllocator>;

	template<typename DataType, Size MinAlignment, typename Alloc>
		requires(IsAllocatorC<Alloc>)
	struct MinAlignmentAllocator {
	public:
		using support_allocator = Alloc;
		using typed_support_allocator = Alloc::template rebind<DataType>;

		using value_type = typed_support_allocator::value_type;
		using reference = typed_support_allocator::reference;
		using const_reference = typed_support_allocator::const_reference;
		using pointer = typed_support_allocator::pointer;
		using const_pointer = typed_support_allocator::const_pointer;
		using difference_type = typed_support_allocator::difference_type;
		using size_type = typed_support_allocator::size_type;
		using alloc_result = AllocResult<DataType>;

		template <typename Other>
		using rebind = MinAlignmentAllocator<Other, MinAlignment, Alloc>;
		using allocator_type = MinAlignmentAllocator<Byte, MinAlignment, Alloc>;
		using typed_allocator_type = MinAlignmentAllocator<DataType, MinAlignment, Alloc>;

		constexpr static inline Size minAlignment = MinAlignment;

	public:
		[[nodiscard]] constexpr static pointer allocate(const Size number) noexcept {
			return typed_support_allocator::allocateAligned(number, minAlignment);
		}
		[[nodiscard]] constexpr static pointer allocateAligned(const Size number, const Size alignment) noexcept {
			auto maxFunc = [](Size s1, Size s2) -> Size {
				return (s1 > s2) ? s1 : s2;
			};
			return typed_support_allocator::allocateAligned(number, maxFunc(minAlignment, alignment));
		}

		constexpr void static deallocate(pointer ptr, const Size number) noexcept {
			typed_support_allocator::deallocate(ptr, number);
		}
	};

#ifdef NATL_COMPILER_MSVC
#pragma warning(pop)
#endif // NATL_COMPILER_MSVC


	template<typename Type, typename Alloc, typename... ConstructArgs>
	constexpr Type* make(ConstructArgs&&... constructArgs) noexcept {
		Type* ptr = Alloc::template rebind<Type>::allocate(1);
		if (ptr == nullptr) {
			return ptr;
		}
		construct<Type, ConstructArgs...>(ptr, forward<ConstructArgs>(constructArgs)...);
		return ptr;
	}

	template<typename Type, typename Alloc>
	constexpr void destory(Type* ptr) noexcept {
		if (ptr == nullptr) {
			return;
		}

		deconstruct<Type>(ptr);
		Alloc::deallocate(ptr, 1);
	}
}