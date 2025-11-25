//@interface
#include "allocator.h"

//own
#include "executionSession.h"
#include "../container/flatHashSet.h"
#include "../container/flatHashMap.h"
#include "../container/persistentHashMap.h"

namespace natl {
	struct AllocationInfo {
		void* ptr;
		Size size;

		template<typename Hasher>
			requires(IsHasherC<Hasher>)
		constexpr void hashAppend(Hasher& hasher) const noexcept {
			hasher.hashAppend(bitCast<Size>(ptr));
		}

		constexpr Bool operator==(const AllocationInfo rhs) const {
			const AllocationInfo& lhs = *this;
			return lhs.ptr == rhs.ptr;
		}
	};

	struct TrackedAllocator {
	public:
		using allocation_set = FlatHashSetAlloc<AllocationInfo, ExecutionSessionAlloc>;
		TrackedAllocatorId id;
		TrackedAllocatorDeallocateFunc deallocateFunc;
		OptionPtr<TopTrackedAllocator> top;
		ui64 numberOfTracks;
		ui64 numberOfUntracks;
		allocation_set allocationSet;
	};

	struct AllocationTrakerStorage {
	public:
		using allocator_map = PersistentHashMapAlloc<TrackedAllocatorId, TrackedAllocator, ExecutionSessionAlloc>;

	public:
		ui64 numberOfTracks;
		ui64 numberOfUntracks;
		allocator_map allocatorMap;
		AllocationTrakerStorage() : numberOfTracks(0)
			, numberOfUntracks(0), allocatorMap() {}
	};

	void destoryGlobalAllocationTraker(GlobalTracker& globalTracker) {
		OptionPtr<AllocationTrakerStorage> storageOption = globalTracker.getStoredAs<AllocationTrakerStorage>();
		if (storageOption.doesNotHaveValue()) {
			return;
		}
		AllocationTrakerStorage& allocationTrakerStorage = storageOption.value();
		
		DynArray<TrackedAllocator*, ExecutionSessionAlloc> trackedAllocators;
		trackedAllocators.reserve(allocationTrakerStorage.allocatorMap.size());

		for (auto [id, trackedAllocator] : allocationTrakerStorage.allocatorMap) {
			trackedAllocators.pushBack(&trackedAllocator);
		}
		sort(trackedAllocators.begin(), trackedAllocators.end()
			, [](TrackedAllocator* lhs, TrackedAllocator* rhs) -> Bool {
			return lhs->id < rhs->id;
		});

		for (TrackedAllocator* trackedAllocator : trackedAllocators) {
			auto foundAllocator = allocationTrakerStorage.allocatorMap.findIter(trackedAllocator->id);
			if (foundAllocator == allocationTrakerStorage.allocatorMap.end()) {
				continue;
			}

			if (trackedAllocator->deallocateFunc) {
				if (trackedAllocator->numberOfTracks != trackedAllocator->numberOfUntracks) {
					for (auto [ptr, size] : trackedAllocator->allocationSet) {
						trackedAllocator->deallocateFunc(ptr, size);
					}
				}
			}
		}
	}
	
	GlobalTracker createAllocationGlobalTraker() {
		return GlobalTracker(false, 
			makeGlobalTrackerStorage<AllocationTrakerStorage>(),
			[](ObserverPtr<GlobalTracker>) -> GlobalTracker {
				return move(createAllocationGlobalTraker());
			}, 
			[](GlobalTracker& gt) -> void {
				destoryGlobalAllocationTraker(gt);
			},
			[](GlobalTracker& top) -> void {
				OptionPtr<AllocationTrakerStorage> storageOption = top.getStoredAs<AllocationTrakerStorage>();
				if (not storageOption.hasValue()) {
					return;
				}
				AllocationTrakerStorage& allocationTrakerStorage = storageOption.value();

				for (auto [id, trackedAllocator] : allocationTrakerStorage.allocatorMap) {
					if (trackedAllocator.top) {
						trackedAllocator.top.get()->setTrackedAllocator(&trackedAllocator);
					}
				}

			}
		);
	}

	Expect<ObserverPtr<GlobalTracker>, RegisterGlobalTrakerError> registerAllocationGlobalTraker(
		const ConstAsciiStringView& name, OptionPtr<TopGlobalTracker> top) noexcept {
		GlobalTracker gt = createAllocationGlobalTraker();
		gt.top = top;
		Expect<ObserverPtr<GlobalTracker>, RegisterGlobalTrakerError>  registrationResult = registerGlobalTraker(name, move(gt));
		if (registrationResult.hasError()) {
			gt.destroyFunc(gt);
		}
		return registrationResult;
	}

	//allocator
	TrackAllocatorResult trackAllocator(ObserverPtr<GlobalTracker> allocationGlobalTraker, 
		TrackedAllocatorDeallocateFunc deallocateFunc, OptionPtr<TopTrackedAllocator> topTrackedAllocator) noexcept {
		TrackAllocatorResult result{};

		if (allocationGlobalTraker == nullptr) {
			result.error = TrackAllocatorError::nullAllocationGlobalTracker;
			return result;
		}

		if (!hasExecutionSessions()) {
			result.error = TrackAllocatorError::noExecutionSession;
			return result;
		}

		OptionPtr<AllocationTrakerStorage> storageOption = allocationGlobalTraker->getStoredAs<AllocationTrakerStorage>();
		if (not storageOption.hasValue()) {
			result.error = TrackAllocatorError::wrongStorage;
			return result;
		}
		AllocationTrakerStorage& allocationTrakerStorage = storageOption.value();
		const TrackedAllocatorId id = allocationTrakerStorage.allocatorMap.size();

		TrackedAllocator trackedAllocator;
		trackedAllocator.numberOfTracks = 0;
		trackedAllocator.numberOfUntracks = 0;
		trackedAllocator.id = id;
		trackedAllocator.deallocateFunc = deallocateFunc;
		trackedAllocator.top = topTrackedAllocator;

		MapInsertResult insertResult = allocationTrakerStorage.allocatorMap.insert(id, move(trackedAllocator));
		result.trackedAllocator = ObserverPtr<TrackedAllocator>(insertResult.iter().keyValue().valueAsPtr());
		topTrackedAllocator.get()->setTrackedAllocator(result.trackedAllocator.get());
		return result;
	}

	UntrackAllocatorError untrackAllocator(ObserverPtr<GlobalTracker> allocationGlobalTraker,
		ObserverPtr<TrackedAllocator> trackedAllocator) noexcept {
		if (allocationGlobalTraker == nullptr) {
			return UntrackAllocatorError::nullAllocationGlobalTracker;
		}
		if (trackedAllocator == nullptr) {
			return UntrackAllocatorError::nullTrackedAllocator;
		}
		if (!hasExecutionSessions()) {
			return UntrackAllocatorError::noExecutionSession;
		}

		OptionPtr<AllocationTrakerStorage> storageOption = allocationGlobalTraker->getStoredAs<AllocationTrakerStorage>();
		if (not storageOption.hasValue()) {
			return UntrackAllocatorError::wrongStorage;
		}
		AllocationTrakerStorage& allocationTrakerStorage = storageOption.value();
		const TrackedAllocatorId id = trackedAllocator->id;

		auto foundAllocator = allocationTrakerStorage.allocatorMap.findIter(id);
		if (foundAllocator == allocationTrakerStorage.allocatorMap.end()) {
			return UntrackAllocatorError::allocaterNotFound;
		}

		if (trackedAllocator->deallocateFunc) {
			if (trackedAllocator->numberOfTracks != trackedAllocator->numberOfUntracks) {
				for (auto [ptr, size] : trackedAllocator->allocationSet) {
					trackedAllocator->deallocateFunc(ptr, size);
				}
			}
		}

		allocationTrakerStorage.allocatorMap.erase(foundAllocator);

		return UntrackAllocatorError::none;
	}

	//allocation
	TrackAllocationError trackAllocation(ObserverPtr<TrackedAllocator> trackedAllocator, void* ptr, const Size size) noexcept {
		if (trackedAllocator == nullptr) {
			return TrackAllocationError::nullTrackedAllocator;
		}

		SetInsertResult insertResult = trackedAllocator->allocationSet.insert(AllocationInfo{ ptr, size });
		if (insertResult.insertFailed()) {
			return TrackAllocationError::alreadyExist;
		}
		trackedAllocator->numberOfTracks++;

		return TrackAllocationError::none;
	}

	UntrackAllocationError untrackAllocation(ObserverPtr<TrackedAllocator> trackedAllocator, void* ptr, const Size size) noexcept {
		if (trackedAllocator == nullptr) {
			return UntrackAllocationError::nullTrackedAllocator;
		}

		auto findAllocation = trackedAllocator->allocationSet.findIter(AllocationInfo{ ptr, size });
		if (findAllocation == trackedAllocator->allocationSet.end()) {
			return UntrackAllocationError::notFound;
		}
		trackedAllocator->allocationSet.erase(findAllocation);
		trackedAllocator->numberOfUntracks++;

		return UntrackAllocationError::none;
	}

	thread_local TopGlobalTracker allocationGlobalTrackerTop{};
	Bool isAllocationTrackingEnabled() noexcept {
		return allocationGlobalTrackerTop.getGlobalTracker() != nullptr;
	}
	Bool enableAllocationTracking() noexcept {
		if (isAllocationTrackingEnabled()) {
			return true;
		}
		Expect<ObserverPtr<GlobalTracker>, RegisterGlobalTrakerError> expectAllocationTracker
			= registerAllocationGlobalTraker("natl::AllocationTracker", &allocationGlobalTrackerTop);
		return expectAllocationTracker.hasValue();
	}
	OptionPtr<GlobalTracker> getAllocationGlobalTracker() noexcept {
		return allocationGlobalTrackerTop.getGlobalTracker();
	}

	thread_local TopTrackedAllocator standardAllocatorTrackerTop{};
	Bool isStandardAllocatorTrackingEnabled() noexcept {
		return standardAllocatorTrackerTop.getTrackedAllocator() != nullptr;
	};
	Bool enableStadardAllocatorTracking() noexcept {
		if (isStandardAllocatorTrackingEnabled()) {
			return true;
		}
		
		ObserverPtr<GlobalTracker> allocationGlobalTracker(allocationGlobalTrackerTop.getGlobalTracker().get());
		natl::TrackAllocatorResult result = trackAllocator(allocationGlobalTracker,
			[](void* ptr, const Size size) -> void {
				UntrackedStandardAllocator<Byte>::deallocate(reinterpret_cast<Byte*>(ptr), size);
			}
		, &standardAllocatorTrackerTop);
		return not result.hasError();
	}

	
	namespace impl {
		void standardAllocatorTrack(void* ptr, const Size size) noexcept {
			if (isStandardAllocatorTrackingEnabled()) {
				trackAllocation(makeObserver(standardAllocatorTrackerTop.getTrackedAllocator().get()), ptr, size);
			}
		}
		void standardAllocatorUntrack(void* ptr, const Size size) noexcept {
			if (isStandardAllocatorTrackingEnabled()) {
				untrackAllocation(makeObserver(standardAllocatorTrackerTop.getTrackedAllocator().get()), ptr, size);
			}
		}
	}
}