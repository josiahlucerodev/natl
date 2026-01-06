#pragma once

//@begin_non_modules
//own
#include "../util/compilerDependent.h"
//@end_non_modules

//own
#include "basicTypes.h"
#include "allocator.h"
#include "memory.h"
#include "pointer.h"
#include "../units/standard.h"
#include "random.h"
#include "../math/satruatedArithmetic.h"
#include "../math/standard.h"
#include "../container/priorityQueue.h"
#include "../container/smallDynArray.h"
#include "../container/flatHashSet.h"
#include "../system/timer.h"
#include "../sync/mutex.h"


//@export
namespace natl {
	constexpr inline Size allocRootTag = defaultRandomSeed;
	constexpr inline Size allocRegionTag = ~defaultRandomSeed;
	using AllocThreadId = Size;

	template<typename Alloc>
	struct CanMeshResult {
		Bool mCanMesh;
		Size mMeshMoveCount;
		Alloc* mTargetAlloc;
		Alloc* mDonorAlloc;
	};

	struct BaseAllocRoot {
	public:
		virtual Bool deallocate(Byte* ptr) noexcept = 0;
		constexpr void zeroAllocation(ByteAllocResult allocResult, const AllocateFlags flags) noexcept {
			if ((flags & AllocateFlags::zeroed) != AllocateFlags::none) {
				zeroMemory(allocResult);
			}
		}
		constexpr Bool isStandardValidFlags(const AllocateFlags& flags) noexcept {
			if (flags == AllocateFlags::none) {
				return true;
			}
			if ((flags & AllocateFlags::zeroed) != AllocateFlags::none) {
				return true;
			}
			return false;
		}
	};

	template<typename AllocRegionManager>
	concept IsAllocRegionManagerC = requires() {
		typename AllocRegionManager::template rebind<true, BaseAllocRoot>;
		typename AllocRegionManager::allocs_container;
		typename AllocRegionManager::alloc_region_info;

		{ AllocRegionManager::regionSize };
		{ AllocRegionManager::threadLocal };
		{ AllocRegionManager::meshing };
		{ AllocRegionManager::memorySize };

		{ AllocRegionManager::template regionAllocate<DefaultAllocator>(declval<AllocateFlags>()) } -> SameAs<ByteAllocResult>;
		{ AllocRegionManager::template regionAllocate<DefaultAllocator>() } -> SameAs<ByteAllocResult>;

		{ AllocRegionManager::getMemoryPtr(declval<Byte*>()) } -> SameAs<Byte*>;
		{ AllocRegionManager::getMemorySize() } -> SameAs<Size>;
		{ AllocRegionManager::getAllocThreadId(declval<Byte*>()) } -> SameAs<AllocThreadId>;

		{ AllocRegionManager::allocCount(declval<Byte*>()) } -> SameAs<Size&>;
		{ AllocRegionManager::maxAllocCount(declval<Byte*>()) } -> SameAs<Size>;
		{ AllocRegionManager::isAllocsFull(declval<Byte*>()) } -> SameAs<Bool>;

		{ AllocRegionManager::isRootAlloc(declval<Byte*>(), declval<BaseAllocRoot*>()) } -> SameAs<Bool>;
		{ AllocRegionManager::initRegion(declval<Byte*>(), declval<BaseAllocRoot*>()) };
		{ AllocRegionManager::getRootAlloc(declval<Byte*>()) } -> SameAs<BaseAllocRoot*>;

		{ AllocRegionManager::findAlloc(declval<Byte*>()) } -> SameAs<BaseAllocRoot*>;
		{ AllocRegionManager::removeAlloc(declval<Byte*>(), declval<BaseAllocRoot*>()) } -> SameAs<Bool>;
		{ AllocRegionManager::addAllocs(declval<Byte*>(), declval<typename AllocRegionManager::allocs_container>()) };
	};

	//@export 
	template <Bool vThreadLocal>
	struct AllocRegionManagerThreadId {};

	template <>
	struct AllocRegionManagerThreadId<false> {};
	template <>
	struct AllocRegionManagerThreadId<true> {
	protected:
		static thread_local AllocThreadId mThreadAllocId;

	public:
		static void setAllocThreadIdOfThread(const AllocThreadId threadId) noexcept {
			mThreadAllocId = threadId;
		}
		static AllocThreadId getAllocThreadIdOfThread() noexcept {
			return mThreadAllocId;
		}
	};

	template<typename AllocRoot>
	struct AllocRegionInfo {
		Byte* mRegionVirtualAddress;
		AllocRoot* mAlloc;
	};

	template<Size vRegionSize, Size vThreadLocal, Size vMeshing, Size vMetaDataSize, typename AllocRootType>
	struct InlineAllocRegionManager : public AllocRegionManagerThreadId<vThreadLocal> {
	public:
		using alloc_root = AllocRootType;

		constexpr static inline Size regionSize = vRegionSize;
		constexpr static inline Bool threadLocal = vThreadLocal;
		constexpr static inline Bool meshing = vMeshing;
		constexpr static inline Size memorySize = vRegionSize - vMetaDataSize;
		constexpr static inline Size metaDataSize = vMetaDataSize;
		constexpr static inline Size sizeOfMeshSlot = sizeof(Size) + sizeof(alloc_root*);

		template <Bool vMeshingOther, typename OtherAllocRootType = BaseAllocRoot>
		using rebind = InlineAllocRegionManager<vRegionSize, vThreadLocal, vMeshingOther, vMetaDataSize, OtherAllocRootType>;

		using alloc_region_info = AllocRegionInfo<AllocRootType>;
		using allocs_container = SmallDynArray<alloc_region_info, metaDataSize / 16>;
	private:
		static Bool testRegionTag(Byte* regionPtr) noexcept {
			Byte* ptrToRegionTagLocation = regionPtr;
			Size* regionTag = reinterpret_cast<Size*>(ptrToRegionTagLocation);
			return *regionTag == allocRegionTag;
		}
		static Byte* toAllocs(Byte* regionPtr) noexcept {
			Byte* metaDataPtr = regionPtr;
			metaDataPtr += sizeof(Size); // Tag
			if (threadLocal) {
				metaDataPtr += sizeof(AllocThreadId); // Thread Id 
			}
			metaDataPtr += sizeof(Size); // Alloc Count
			return metaDataPtr;
		}
		static ArrayView<alloc_region_info> getAllocSlots(Byte* regionPtr) noexcept {
			Byte* byteAllocSlots = toAllocs(regionPtr);
			return ArrayView<alloc_region_info>(reinterpret_cast<alloc_region_info*>(byteAllocSlots), maxAllocCount(regionPtr));
		}

	public:
		template<typename RegionAlloc>
			requires(IsAllocatorC<RegionAlloc>)
		static ByteAllocResult regionAllocate(AllocateFlags flags = AllocateFlags::none) noexcept {
			return RegionAlloc::allocateAligned(regionSize, regionSize, flags | (vMeshing ? AllocateFlags::remappable : AllocateFlags::none));
		}

		static Byte* getMemoryPtr(Byte* regionPtr) noexcept {
			return regionPtr + metaDataSize;
		}
		static Size getMemorySize() noexcept {
			return regionSize - metaDataSize;
		}

		static AllocThreadId getAllocThreadId(Byte* regionPtr) noexcept {
			if (!threadLocal) {
				return Limits<AllocThreadId>::max();
			}
			Byte* metaDataPtr = regionPtr;
			metaDataPtr += sizeof(Size); // Tag
			Byte* ptrToAllocThreadIdLocation = metaDataPtr;
			AllocThreadId* allocThreadId = reinterpret_cast<AllocThreadId*>(ptrToAllocThreadIdLocation);
			return *allocThreadId;
		}

		static Size& allocCount(Byte* regionPtr) noexcept {
			Byte* metaDataPtr = regionPtr;
			metaDataPtr += sizeof(Size); // Tag
			if (threadLocal) {
				metaDataPtr += sizeof(AllocThreadId); // Thread Id 
			}
			Byte* ptrToallocCountLocation = metaDataPtr;
			Size* allocCount = reinterpret_cast<Size*>(ptrToallocCountLocation);
			return *allocCount;
		}

		static Size maxAllocCount(Byte*) noexcept {
			Size allocSlotsByteSize = metaDataSize;
			allocSlotsByteSize -= sizeof(Size);  // Tag
			if (threadLocal) {
				allocSlotsByteSize -= sizeof(AllocThreadId); // Thread Id 
			}
			allocSlotsByteSize -= sizeof(Size); // Alloc Count
			return allocSlotsByteSize / (sizeof(Size) + sizeof(alloc_root*));
		}

		static Bool isAllocsFull(Byte* regionPtr) noexcept {
			return allocCount(regionPtr) == maxAllocCount(regionPtr);
		}

		static Bool isRootAlloc(Byte* regionPtr, alloc_root* allocRootIn) noexcept {
			if (vMetaDataSize == 0) {
				return true;
			}

			Byte* allocSlots = toAllocs(regionPtr);
			alloc_root* allocRoot = *reinterpret_cast<alloc_root**>(allocSlots + sizeof(Byte*));
			return allocRoot == allocRootIn;
		}

		static void initRegion(Byte* regionPtr, alloc_root* allocRootIn) noexcept {
			if (vMetaDataSize == 0) {
				return;
			}
			zeroMemory(regionPtr, metaDataSize);


			allocCount(regionPtr) += 1;
			auto allocSlots = getAllocSlots(regionPtr);
			allocSlots[0].mAlloc = allocRootIn;
			allocSlots[0].mRegionVirtualAddress = regionPtr;
		}

		static alloc_root* getRootAlloc(Byte* regionPtr) noexcept {
			auto allocSlots = getAllocSlots(regionPtr);
			return allocSlots[0].mAlloc;
		}

		static alloc_root* findAlloc(Byte* regionPtr) noexcept {
			auto allocSlots = getAllocSlots(regionPtr);
			for (alloc_region_info& allocSlot : allocSlots) {
				if (allocSlot.mRegionVirtualAddress == regionPtr) {
					return allocSlot.mAlloc;
				}
			}
			return nullptr;
		}

		static Bool removeAlloc(Byte* regionPtr, alloc_root* allocRootIn) noexcept {
			auto allocSlots = getAllocSlots(regionPtr);
			for (alloc_region_info& allocSlot : allocSlots) {
				if (allocSlot.mAlloc == allocRootIn) {
					allocCount(regionPtr) -= 1;
					allocSlot.mAlloc = nullptr;
					allocSlot.mRegionVirtualAddress = nullptr;
					return true;
				}
			}
			return false;
		}

		static allocs_container getAllocs(Byte* regionPtr) noexcept {
			allocs_container allocs;
			auto allocSlots = getAllocSlots(regionPtr);

			for (alloc_region_info& allocSlot : allocSlots) {
				if (allocSlot.mAlloc != nullptr) {
					allocs.pushBack(allocSlot);
				}
			}

			return allocs;
		}

		/*
		static Bool addMeshDonor(Byte* targetRegionPtr, Byte* donorRegionPtr, AllocRoot* donorAllocPtr) noexcept {
			Byte* allocSlots = toAllocs(targetRegionPtr);

			for (; allocSlots < targetRegionPtr + metaDataSize; allocSlots += sizeOfMeshSlot) {
				Byte** regionVirtualAddress = *reinterpret_cast<Byte**>(allocSlots);
				AllocRoot** allocRoot = *reinterpret_cast<AllocRoot**>(allocSlots + sizeof(Byte*);
				if (*regionVirtualAddress == nullptr) {

					allocCount(targetRegionPtr) += 1;
					*regionVirtualAddress = donorRegionPtr;
					*allocRoot = donorAllocPtr;
					return true;
				}
			}

			return false;
		}
		*/

		template<typename RegionAlloc>
		static Bool canMesh(CanMeshResult<alloc_root>& result, Size maxMeshMoves, alloc_root* allocA, alloc_root* allocB) noexcept {
			result.mCanMesh = false;

			if (!meshing) {
				return false;
			}
			if (!allocA->isRootAlloc() && !allocB->isRootAlloc()) {
				return false;
			}

			Size aAllocCount = allocA->allocCount();
			Size bAllocCount = allocB->allocCount();

			alloc_root* targetAlloc = nullptr;
			alloc_root* donorAlloc = nullptr;

			const Size combinedAllocCount = aAllocCount + bAllocCount;
			if (aAllocCount < bAllocCount && combinedAllocCount < allocA->maxAllocCount() && aAllocCount <= maxMeshMoves) {
				targetAlloc = allocB;
				donorAlloc = allocA;
				result.mMeshMoveCount = aAllocCount;
			} else if (combinedAllocCount < allocB->maxAllocCount() && bAllocCount <= maxMeshMoves) {
				targetAlloc = allocA;
				donorAlloc = allocB;
				result.mMeshMoveCount = bAllocCount;
			} else {
				return false;
			}

			if (!RegionAlloc::canMesh(targetAlloc->getRegion(), regionSize, donorAlloc->getRegion(), regionSize)) {
				return false;
			}

			result.mTargetAlloc = targetAlloc;
			result.mDonorAlloc = donorAlloc;
			return true;
		}

		static Bool addAllocs(Byte* targetRegionPtr, const allocs_container& donorAllocs) noexcept {
			Byte* targetAllocSlots = toAllocs(targetRegionPtr);
			auto allocSlots = getAllocSlots(targetRegionPtr);
			Size freeAllocSlots = maxAllocCount(targetRegionPtr) - allocCount(targetRegionPtr);
			if (freeAllocSlots < donorAllocs.size()) {
				return false;
			}

			Size donorIndex = 0;
			for (alloc_region_info& allocSlot : allocSlots) {
				if (donorIndex >= donorAllocs.size()) {
					return true;
				}

				if (allocSlot.mAlloc == nullptr) {
					const alloc_region_info& donorAlloc = donorAllocs[donorIndex];
					allocSlot = donorAlloc;
					donorIndex += 1;
				}
			}

			allocCount(targetRegionPtr) += donorAllocs.size();

			return true;
		}
	};

	//@export 
	namespace impl {
		using TestAllocRegionManager = InlineAllocRegionManager<4096, true, true, 128, BaseAllocRoot>;
	}

	template<typename AllocComposer>
	concept IsBindedAllocComposerC = requires (AllocComposer & allocComposer) {
		{ allocComposer.isInit() } -> IsSameC<Bool>;
		{ allocComposer.init() } -> IsSameC<Bool>;
		{ allocComposer.deinit() } -> IsSameC<Bool>;
		{ allocComposer.allocate(declval<Size>(), declval<Size>(), declval<AllocateFlags>()) } -> IsSameC<ByteAllocResult>;
	};

	template<typename AllocComposer>
	concept IsBindedMeshableAllocComposerC = IsBindedAllocComposerC<AllocComposer> && requires(AllocComposer & allocComposer) {
		{ allocComposer.mesh(declval<TimeoutTraker>(), declval<f64>(), declval<Size>()) };
	};

	template<typename AllocComposer>
	concept IsBindedMergableAllocComposerC = IsBindedAllocComposerC<AllocComposer> && requires(AllocComposer & allocComposer) {
		{ AllocComposer::merge(declval<AllocComposer*>(), declval<AllocComposer*>()) };
	};

	template<typename AllocComposer>
	concept IsBindedOptimizableAllocComposerC = IsBindedAllocComposerC<AllocComposer> && requires(AllocComposer & allocComposer) {
		{ allocComposer.optimize(declval<TimeoutTraker>()) };
	};

	template<template<typename> typename AllocComposer>
	concept IsAllocComposerC = requires () {
		{ AllocComposer<impl::TestAllocRegionManager>() } -> IsBindedAllocComposerC<>;
	};

	template<template<typename> typename AllocComposer>
	concept IsMeshableAllocComposerC = requires () {
		{ AllocComposer<impl::TestAllocRegionManager>() } -> IsBindedMeshableAllocComposerC<>;
	};

	template<template<typename> typename AllocComposer>
	concept IsMergableAllocComposerC = requires () {
		{ AllocComposer<impl::TestAllocRegionManager>() } -> IsBindedMergableAllocComposerC<>;
	};

	template<template<typename> typename AllocComposer>
	concept IsOptimizingAllocComposerC = requires () {
		{ AllocComposer<impl::TestAllocRegionManager>() } -> IsBindedOptimizableAllocComposerC<>;
	};


	/*
			Bool optimize(const TimeoutTraker timeoutTraker) noexcept {

			}
			Size mesh(const TimeoutTraker timeoutTraker, f32 fragmentationThreshold, Size maxMeshAttempts) noexcept {

			}
			static Bool merge(const TimeoutTraker timeoutTraker, Alloc* targetAlloc, Alloc* donorAlloc) noexcept {

			}
	*/

	template<typename AllocRoot>
	concept IsBindedAllocRootC = IsBindedAllocComposerC<AllocRoot> && requires(AllocRoot & allocRoot) {
		{ static_cast<BaseAllocRoot*>(&allocRoot) };
		{ allocRoot.deallocate(declval<Byte*>()) } -> IsSameC<Bool>;
		{ allocRoot.isFull() } -> IsSameC<Bool>;
		{ allocRoot.isEmpty() } -> IsSameC<Bool>;
		{ allocRoot.couldOwn(declval<Byte*>()) } -> IsSameC<Bool>;
		{ allocRoot.capacity() } -> IsSameC<Size>;
		{ allocRoot.freeCapacity() } -> IsSameC<Size>;
	};

	template<typename AllocRoot>
	concept IsBindedMeshableAllocRootC = IsBindedAllocRootC<AllocRoot> && requires(AllocRoot& allocRoot) {
		{ allocRoot.isRootAlloc() } -> IsSameC<Bool>;
		{ allocRoot.allocCount() } -> IsSameC<Size>;
		{ allocRoot.maxAllocCount() } -> IsSameC<Size>;
		{ allocRoot.getRegion() } -> IsSameC<Byte*>;
		{ AllocRoot::canMesh(declval<Size>(), declval<AllocRoot*>(), declval<AllocRoot*>()) } -> IsSameC<CanMeshResult<AllocRoot>>;
		{ AllocRoot::mesh(declval<AllocRoot*>(), declval<AllocRoot*>()) };
	};

	template<template<typename> typename AllocRoot>
	concept IsAllocRootC = requires() {
		{ AllocRoot<impl::TestAllocRegionManager>() } -> IsBindedAllocRootC<>;
	};

	template<template<typename> typename AllocRoot>
	concept IsMeshableAllocRootC = requires() {
		{ AllocRoot<impl::TestAllocRegionManager>() } -> IsBindedMeshableAllocRootC<>;
	};

	template<typename AllocRoot>
	concept IsBindedOptimizableAllocRootC = IsBindedAllocRootC<AllocRoot> && requires(AllocRoot & allocRoot) {
		{ allocRoot.optimize(declval<TimeoutTraker>()) } -> IsSameC<Bool>;
	};

	template<typename AllocRoot>
		requires(IsBindedAllocRootC<AllocRoot>)
	constexpr f64 allocFreeRatio(AllocRoot& alloc) noexcept {
		return alloc.freeCapacity() / alloc.capacity();
	}

	template<typename AllocRoot, typename RegionManager, typename RegionAlloc>
	void allocPartMesh(AllocRoot* targetAlloc, AllocRoot* donorAlloc, void (*meshMemcpy)(AllocRoot*, AllocRoot*)) noexcept {
		auto donorAllocs = RegionManager::getAllocs(donorAlloc->getRegion());
		for (AllocRegionInfo<AllocRoot> regionInfo : donorAllocs) {
			RegionAlloc::meshMoveProtect(regionInfo.mRegionVirtualAddress, RegionManager::regionSize);
		}

		for (AllocRegionInfo<AllocRoot> regionInfo : donorAllocs) {
			meshMemcpy(targetAlloc, regionInfo.mAlloc);
		}

		for (AllocRegionInfo<AllocRoot> regionInfo : donorAllocs) {
			RegionAlloc::mesh(targetAlloc->getRegion(), RegionManager::regionSize, regionInfo.mRegionVirtualAddress, RegionManager::regionSize);
		}
		for (AllocRegionInfo<AllocRoot> regionInfo : donorAllocs) {
			RegionAlloc::meshMoveUnprotect(regionInfo.mRegionVirtualAddress, RegionManager::regionSize);
		}

		RegionManager::addAllocs(targetAlloc->getRegion(), donorAllocs);
	}

	template<Size vAllocBlockSize, typename SupportAlloc, template<typename> typename AllocComposer, typename RandomGenerator>
		requires(IsAllocComposerC<AllocComposer>&& IsRandomGenerator<RandomGenerator>)
	struct GroupAllocComposer {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc {
		public:
			using support_alloc = SupportAlloc;
			using region_manager = AllocRegionManager::template rebind<false>;
			using alloc_composer = AllocComposer<AllocRegionManager>;
			constexpr static inline Bool isMeshable = alloc_composer::isMeshable;
			constexpr static inline Bool isMergeable = true;
			constexpr static inline Bool isOptimizable = true;

		private:
			using alloc_array = Array<alloc_composer, (vAllocBlockSize - alignof(alloc_composer)) / sizeof(alloc_composer)>;

		public:
			struct GroupNode {
				GroupNode* mNext;
				alloc_array mAllocs;
			};
			static_assert(sizeof(GroupNode) <= vAllocBlockSize);

		private:
			UtilRandomGenerator<RandomGenerator> mRandomGenerator;
			GroupNode* mGroupNode;
			GroupNode* mCachedGroupNode;
			alloc_composer* mCachedAlloc;
			Size mCachedAllocsIndex;

		public:
			Alloc() noexcept
				: mRandomGenerator()
				, mGroupNode(nullptr)
				, mCachedGroupNode(nullptr)
				, mCachedAlloc(nullptr)
				, mCachedAllocsIndex(0) {
			}

			Bool isInit() noexcept {
				return mGroupNode != nullptr;
			}

			Bool init() noexcept {
				ByteAllocResult allocResult = support_alloc::allocate(vAllocBlockSize);
				if (allocResult.ptr == nullptr) {
					return false;
				}

				GroupNode* groupNode = reinterpret_cast<GroupNode*>(allocResult.ptr);
				construct<GroupNode>(groupNode);
				groupNode->mNext = nullptr;

				mGroupNode = groupNode;
				mCachedGroupNode = nullptr;
				mCachedAlloc = nullptr;
				mCachedAllocsIndex = 0;
				mRandomGenerator.seed();
				return true;
			}
			Bool deinit() noexcept {
				if (mGroupNode) {
					GroupNode* firstNode = mGroupNode;
					GroupNode* currentNode = mGroupNode;

					while (true) {
						for (Size i = 0; i < currentNode->mAllocs.size(); i++) {
							alloc_composer& alloc = currentNode->mAllocs[i];
							if (alloc.isInit()) {
								alloc.deinit();
							}
						}

						GroupNode* nextNode = currentNode->mNext;
						deconstruct<GroupNode>(currentNode);
						SupportAlloc::deallocate(currentNode);

						if (nextNode == firstNode) {
							break;
						}
						currentNode = nextNode;
					}
				}

				mGroupNode = nullptr;
				mCachedGroupNode = nullptr;
				mCachedAlloc = nullptr;
				mCachedAllocsIndex = 0;
				return true;
			}

		private:
			Option<ByteAllocResult> onceAllocate(const Size alignment, const Size size, const AllocateFlags flags, Bool forceNewGroup) noexcept {
				GroupNode* firstNode = mGroupNode;
				GroupNode* currentNode = mCachedGroupNode;

				if (forceNewGroup) {
					mCachedAlloc = nullptr;
					mCachedAllocsIndex = alloc_array::number;
				}

				if (mCachedAlloc == nullptr || mCachedAlloc->remainingCapacity() <= size) {
					while (true) {
						for (; mCachedAllocsIndex < currentNode->mAllocs.size(); mCachedAllocsIndex++) {
							alloc_composer& alloc = currentNode->mAllocs[mCachedAllocsIndex];
							if (alloc.isInit()) {
								if (alloc.remainingCapacity() >= size) {
									mCachedAlloc = &alloc;
									break;
								}
							} else {
								if (alloc.init()) {
									if (alloc.remainingCapacity() <= size) {
										return OptionEmpty{};
									}

									mCachedAlloc = &alloc;
									break;
								}
							}
						}


						Bool newGroup = false;
						GroupNode* nextNode = currentNode->mNext;
						if (forceNewGroup || mRandomGenerator.oneIn(4)) {
							newGroup = true;
						}

						if (newGroup) {
							ByteAllocResult allocResult = support_alloc::allocateAligned(pageSize, pageSize);
							if (allocResult.ptr == nullptr) {
								return false;
							}
							GroupNode* groupNode = reinterpret_cast<GroupNode*>(allocResult.ptr);
							construct<GroupNode>(groupNode);

							groupNode->mNext = currentNode->mNext;
							currentNode->mNext = groupNode;
							nextNode = currentNode->mNext;
							mCachedGroupNode = groupNode;
						}

						if (nextNode == nullptr) {
							nextNode = mGroupNode;
						}

						currentNode = nextNode;
						mCachedAllocsIndex = 0;
						mCachedGroupNode = currentNode;
					}
				}

				return mCachedAlloc.allocate(alignment, size, flags);
			}
		public:
			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				//Try allocate multiple times with different allocs 
				//Second attempt should only occur if first alloc is not full and has enough remaining capacity 
				//but is unable to allocate because of memory fragmentation or overhead (alignment, metadata, etc)
				for (Size i = 0; i < 2; i++) {
					Option<ByteAllocResult> resultOption = onceAllocate(alignment, size, flags, false);
					if (!resultOption.hasValue()) {
						ByteAllocResult result;
						result.mPtr = nullptr;
						result.mSize = 0;
						return result;
					}

					if (resultOption.value().ptr != nullptr) {
						return resultOption.value();
					}
					mCachedAlloc = nullptr;
					mCachedAllocsIndex++;
				}

				Option<ByteAllocResult> resultOption = onceAllocate(alignment, size, flags, true);
				if (!resultOption.hasValue()) {
					ByteAllocResult result;
					result.mPtr = nullptr;
					result.mSize = 0;
					return result;
				}
				return resultOption.value();
			}

			Bool deallocate(Byte* ptr) noexcept {
				return region_manager::deallocate(ptr);
			}

			Bool optimize(const TimeoutTraker timeoutTraker) noexcept {
				if (mGroupNode == nullptr) {
					return true;
				}

				GroupNode* previousNode = nullptr;
				GroupNode* currentNode = mGroupNode;

				while (true) {
					Bool isGroupEmpty = true;
					for (Size i = 0; i < currentNode->mAllocs.size(); i++) {
						alloc_composer& alloc = currentNode->mAllocs[i];
						if (alloc.isInit()) {
							if (alloc.isEmpty()) {
								alloc.deinit();
							} else {
								isGroupEmpty = false;
							}
						}
					}

					GroupNode* nextNode = currentNode->mNext;
					if (isGroupEmpty && previousNode != nullptr) {
						if (currentNode == mCachedGroupNode) {
							mCachedGroupNode = mGroupNode;
							mCachedAlloc = nullptr;
							mCachedAllocsIndex = 0;
						}

						deconstruct<GroupNode>(currentNode);
						SupportAlloc::deallocate(currentNode);

						currentNode = previousNode;
						previousNode->mNext = nextNode;
					}

					if (nextNode == nullptr) {
						break;
					}
					previousNode = currentNode;
					currentNode = nextNode;

					if (timeoutTraker.isTimedOut()) {
						return false;
					}
				}

				if constexpr (alloc_composer::isOptimizable) {
					currentNode = mGroupNode;
					while (true) {
						for (Size i = 0; i < currentNode->mAllocs.size(); i++) {
							alloc_composer& alloc = currentNode->mAllocs[i];
							if (alloc.isInit()) {
								if (timeoutTraker.isTimedOut()) {
									return false;
								}
								alloc.optimize(timeoutTraker);
							}
						}

						GroupNode* nextNode = currentNode->mNext;
						if (nextNode == nullptr) {
							break;
						}
						currentNode = nextNode;

						if (timeoutTraker.isTimedOut()) {
							return false;
						}
					}
				}

				return true;
			}

			struct MeshAllocState {
				alloc_composer* alloc;
				Size attemptsLeft;
				Bool wasTargetLastRound = false;
			};

			struct AllocMeshPair {
				alloc_composer* donor;
				alloc_composer* target;

				Bool operator==(const AllocMeshPair& other) const noexcept {
					return donor == other.donor && target == other.target;
				}

				template<typename Hasher>
					requires(IsHasherC<Hasher>)
				constexpr void hashAppend(Hasher& hasher) const noexcept {
					hasher.hashAppend(bitCast<Size>(donor));
					hasher.hashAppend(bitCast<Size>(target));
				}
			};


			Size mesh(const TimeoutTraker timeoutTraker, f32 freeRatioThreshold, Size maxMeshAttemptsPerAllocator) noexcept
				requires(IsMeshableAllocRootC<alloc_composer>) {

				if (mGroupNode == nullptr) {
					return 0;
				}

				natl::DynArray<MeshAllocState> allocStates;
				alloc_composer* currentNode = mGroupNode;
				while (true) {
					for (Size i = 0; i < currentNode->mAllocs.size(); i++) {
						alloc_composer& alloc = currentNode->mAllocs[i];
						if (alloc.isInit() && !alloc.isEmpty() && !alloc.isFull()) {
							MeshAllocState allocState;
							allocState.alloc = alloc;
							allocState.attemptsLeft = maxMeshAttemptsPerAllocator;
							allocState.wasTargetLastRound = false;

							allocStates.pushBack(allocState);
						}
					}

					GroupNode* nextNode = currentNode->mNext;
					if (nextNode == nullptr) {
						break;
					}
					currentNode = nextNode;
				}


				Size meshCount = 0;
				natl::FlatHashSet<AllocMeshPair> failedPairs;
				failedPairs.reserve(allocStates.size() * 4);

				auto sortByFreeRatio = [&]() {
					natl::sort(
						allocStates.begin(),
						allocStates.end(),
						[](const MeshAllocState& lhs, const MeshAllocState& rhs) {
							return allocFreeRatio(*lhs->alloc) < allocFreeRatio(*rhs->alloc);
						}
					);
					};

				while (!timeoutTraker.isTimedOut() && allocStates.size() >= 2) {
					sortByFreeRatio();
					MeshAllocState& donorState = allocStates.front();
					MeshAllocState& targetState = allocStates.back();;

					alloc_composer* donor = donorState.alloc;
					alloc_composer* target = targetState.alloc;

					// Balanced enough
					if (allocFreeRatio(*donor) >= freeRatioThreshold) {
						break;
					}

					// Reset attempts if allocator was target and is now donor
					if (donorState.wasTargetLastRound) {
						donorState.attemptsLeft = maxMeshAttemptsPerAllocator;
						donorState.wasTargetLastRound = false;
					}

					// Mark target role
					targetState.wasTargetLastRound = true;

					// Donor exhausted
					if (donorState.attemptsLeft == 0) {
						allocStates.erase(allocStates.begin());
						if (allocStates.size() < 2) {
							break;
						}
						continue;
					}

					AllocMeshPair meshPair{ donor, target };

					// Skip known-bad pairs
					if (meshPair.contains()) {
						--donorState.attemptsLeft;
						continue;
					}

					// Capability check
					if (!alloc_composer::canMesh(target, donor)) {
						failedPairs.insert(meshPair);
						--donorState.attemptsLeft;
						continue;
					}

					alloc_composer::mesh(target, donor);

					Bool progressed = donor->mesh(*target, *donor);
					--donorState.attemptsLeft;
					meshCount++;
				}

				return meshCount;
			}

			static void merge(Alloc* targetAlloc, Alloc* donorAlloc) noexcept {
				GroupNode* endNode = mGroupNode;

				while (true) {
					GroupNode* nextNode = endNode->mNext;
					if (nextNode == nullptr) {
						break;
					}
					endNode = nextNode;
				}

				endNode->mNext = donorAlloc->mGroupNode;
				donorAlloc->mGroupNode = nullptr;
				donorAlloc->mCachedGroupNode = nullptr;
				donorAlloc->mCachedAlloc = nullptr;
				donorAlloc->mCachedAllocsIndex = 0;
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	//@export 
	namespace impl {
		template<typename PowerAllocComposerElement>
		concept IsPowerAllocComposerElementC = requires() {
			typename PowerAllocComposerElement::natl_is_power_composer_element;
		};
	}

	template<Size vPower, template<typename> typename Alloc>
	struct PowerAllocComposerElement {
	public:
		using natl_is_power_composer_element = int;
		constexpr static inline Size power = vPower;
		constexpr static inline Size maxSize = pow2<Size>(power);

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<typename... AllocElements>
		requires(impl::IsPowerAllocComposerElementC<AllocElements> && ...)
	struct PowerAllocComposer {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc {
		public:
			using region_manager = AllocRegionManager::template rebind<false>;
			constexpr static inline Bool isMeshable = (AllocElements::template alloc<AllocRegionManager>::isMeshable || ...);
			constexpr static inline Bool isMergeable = (AllocElements::template alloc<AllocRegionManager>::isMergeable && ...);
			constexpr static inline Bool isOptimizable = (AllocElements::template alloc<AllocRegionManager>::isOptimizable || ...);

		private:
			template <typename AllocElement>
			struct AllocTransform {
				using type = AllocElement::template alloc<AllocRegionManager>;
			};
			using raw_allocs = TypePackTransform<AllocTransform, TypePack<AllocElements...>>;
			using allocs_tuple = InstantiateWithTypePack<Tuple, raw_allocs>;

		private:
			Bool mIsInit;
			allocs_tuple mAllocs;

		public:
			Alloc() noexcept : mIsInit(false), mAllocs() {}

			Bool isInit() noexcept {
				return mIsInit;
			}

			Bool init() noexcept {
				Bool& successful = true;
				auto initSubAlloc = [&] <typename AllocElement> (AllocElement & allocElement) {
					if (successful == false) {
						return;
					}
					successful = allocElement.alloc.init();
				};
				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(initSubAlloc(mAllocs.template get<vIndices>()), ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});

				if (successful) {
					mIsInit = true;
				} else {
					deinit();
				}
				return successful;
			}
			Bool deinit() noexcept {
				Bool& successful = true;
				auto deinitSubAlloc = [&] <typename AllocElement> (AllocElement & allocElement) {
					Bool result = allocElement.alloc.deinit();
					if (result == false) {
						successful = false;
					}
				};
				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(deinitSubAlloc(mAllocs.template get<vIndices>()), ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});
				mIsInit = false;
				return successful;
			}

			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				if (!isPowerOf2<Size>(alignment) || alignment > size) {
					Size power;
					bitscanForward(alignment, power);

					Size newAlignment = pow2<Size>(power);
					Size newSize = alignment + size;

					ByteAllocResult allocResult = rawAllocate(newAlignment, newSize, flags);
					AlignPtrExpect alignExpect = alignPtr(alignment, size, allocResult.ptr, allocResult.size);

					if (alignExpect.hasError()) {
						rawDeallocate(allocResult.ptr, newSize);
						allocResult.ptr = nullptr;
						allocResult.size = 0;
					} else {
						AlignPtrInfo alignPtrInfo = alignExpect.value();
						allocResult.ptr = alignPtrInfo.alignedPtr;
						allocResult.size = size + alignPtrInfo.remainingSpace;
					}
					return allocResult;
				} else {
					return rawAllocate(alignment, size, flags);
				}
			}

			Bool deallocate(Byte* ptr) noexcept {
				return region_manager::deallocate(ptr);
			}

		private:
			ByteAllocResult rawAllocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				Bool& found = true;
				ByteAllocResult allocResult;

				auto subAlloc = [&] <typename AllocElement> (AllocElement & allocElement) {
					if (found) {
						return;
					}
					if (size <= AllocElement::maxSize) {
						allocResult = allocElement.alloc.allocate(alignment, size, flags);
						found = true;
					}
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(subAlloc(mAllocs.template get<vIndices>()), ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});

				if (!found) {
					allocResult.mPtr = nullptr;
					allocResult.mSize = 0;
				}

				return allocResult;
			}
			void rawDeallocate(Byte* ptr, const Size size) noexcept {
				Bool& found = true;
				auto subDealloc = [&] <typename AllocElement> (AllocElement & allocElement) {
					if (found) {
						return;
					}
					if (size <= AllocElement::maxSize) {
						allocElement.alloc.deallocate(ptr);
						found = true;
					}
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(subDealloc(mAllocs.template get<vIndices>()), ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});
			}
		public:
			Bool optimize(const TimeoutTraker timeoutTraker) noexcept requires(isOptimizable) {
				Array<Bool, allocs_tuple::tupleSize> optimizeDoneArray;
				Size allocCount = 0;

				auto hasOptimize = [&] <typename AllocElement, Size Index> (AllocElement&, TypeValue<Index>) {
					optimizeDoneArray[Index] = !AllocElement::isOptimizable;
					allocCount++;
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(hasOptimize(mAllocs.template get<vIndices>(), TypeValue<vIndices>{}) && ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});

				auto splitTimeoutOptimizeAlloc = [&] <typename AllocElement, Size Index> (AllocElement & allocElement, TypeValue<Index>) {
					if (optimizeDoneArray[Index] && timeoutTraker.isTimedOut()) {
						return;
					}

					if constexpr (IsBindedOptimizableAllocComposerC<AllocElement>) {
						TimeoutTraker subTimeout;
						subTimeout.start(Microseconds<Size>(timeoutTraker.remainingTime().value() / allocCount));
						allocCount--;
						optimizeDoneArray[Index] = allocElement.optimize(subTimeout);
					} else {
						optimizeDoneArray[Index] = allocElement.optimize();
					}
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(splitTimeoutOptimizeAlloc(mAllocs.template get<vIndices>(), TypeValue<vIndices>{}) && ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});

				if (timeoutTraker.isTimedOut()) {
					return false;
				}

				auto optimizeAlloc = [&] <typename AllocElement, Size Index> (AllocElement & allocElement, TypeValue<Index>) {
					if (optimizeDoneArray[Index] || timeoutTraker.isTimedOut()) {
						return;
					}

					if constexpr (IsBindedOptimizableAllocComposerC<AllocElement>) {
						allocElement.optimize(timeoutTraker);
					} else {
						allocElement.optimize();
					}
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(optimizeAlloc(mAllocs.template get<vIndices>(), TypeValue<vIndices>{}) && ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});

				return timeoutTraker.isTimedOut();
			}
			Size mesh(const TimeoutTraker timeoutTraker, f32 freeRatioThreshold, Size maxMeshAttemptsPerAllocator) noexcept requires(isMeshable) {
				Array<Bool, allocs_tuple::tupleSize> meshDoneArray;
				Size allocCount = 0;
				Size meshCount = 0;

				auto hasMesh = [&] <typename AllocElement, Size Index> (AllocElement&, TypeValue<Index>) {
					meshDoneArray[Index] = !AllocElement::isOptimizable;
					allocCount++;
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(hasMesh(mAllocs.template get<vIndices>(), TypeValue<vIndices>{}) && ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});

				auto splitTimeoutMeshAlloc = [&] <typename AllocElement, Size Index> (AllocElement & allocElement, TypeValue<Index>) {
					if (meshDoneArray[Index] && timeoutTraker.isTimedOut()) {
						return;
					}

					if constexpr (IsBindedOptimizableAllocComposerC<AllocElement>) {
						TimeoutTraker subTimeout;
						subTimeout.start(Microseconds<Size>(timeoutTraker.remainingTime().value() / allocCount));
						allocCount--;
						Size subMeshCount = allocElement.mesh(subTimeout, freeRatioThreshold, maxMeshAttemptsPerAllocator);
						meshDoneArray[Index] = subMeshCount == 0;
						meshCount += subMeshCount;
					}
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(splitTimeoutMeshAlloc(mAllocs.template get<vIndices>(), TypeValue<vIndices>{}) && ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});

				if (timeoutTraker.isTimedOut()) {
					return meshCount;
				}

				auto meshAlloc = [&] <typename AllocElement, Size Index> (AllocElement & allocElement, TypeValue<Index>) {
					if (meshDoneArray[Index] || timeoutTraker.isTimedOut()) {
						return;
					}

					if constexpr (IsBindedOptimizableAllocComposerC<AllocElement>) {
						meshCount += allocElement.mesh(timeoutTraker, freeRatioThreshold, maxMeshAttemptsPerAllocator);
					}
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(meshAlloc(mAllocs.template get<vIndices>(), TypeValue<vIndices>{}) && ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});

				return meshCount;
			}

			static void merge(Alloc* targetAlloc, Alloc* donorAlloc) noexcept requires(isMergeable) {
				auto mergeAlloc = [&] <typename AllocElement> (AllocElement & target, AllocElement & donor) {
					Decay<AllocElement>::merge(target, donor);
				};

				[&] <Size... vIndices> (IndexSequence<vIndices...>) {
					(mergeAlloc(mAllocs.template get<vIndices>(), TypeValue<vIndices>{}) && ...);
				}(MakeIndexSequence<sizeof...(AllocElements)>{});
			};
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<template<typename> typename PrimaryAlloc, template<typename> typename FallBackAlloc>
		requires(IsAllocComposerC<PrimaryAlloc>&& IsAllocComposerC<FallBackAlloc>)
	struct FallbackAllocComposer {
		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc {
		public:
			using region_manager = AllocRegionManager::template rebind<false>;
			using primary_alloc = PrimaryAlloc<AllocRegionManager>;
			using fallback_alloc = PrimaryAlloc<AllocRegionManager>;

			constexpr static inline Bool isMeshable = (primary_alloc::isMeshable || fallback_alloc::isMeshable);
			constexpr static inline Bool isMergeable = (primary_alloc::isMergeable || fallback_alloc::isMergeable);
			constexpr static inline Bool isOptimizable = (primary_alloc::isOptimizable || fallback_alloc::isOptimizable);

		private:
			primary_alloc mPrimaryAlloc;
			fallback_alloc mFallbackAlloc;

		public:
			Alloc() noexcept
				: mPrimaryAlloc(nullptr)
				, mFallbackAlloc(nullptr) {
			}

			Bool isInit() noexcept {
				return mPrimaryAlloc.isInit() && mFallbackAlloc.isInit();
			}

			Bool init() noexcept {
				if (!mPrimaryAlloc.init()) {
					return false;
				}
				if (!mFallbackAlloc.init()) {
					mPrimaryAlloc.deinit();
					return false;
				}
				return true;
			}

			Bool deinit() noexcept {
				Bool successful = true;
				successful = successful && mPrimaryAlloc.deinit();
				successful = successful && mFallbackAlloc.deinit();
				return successful;
			}

			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				ByteAllocResult byteAllocResult = mPrimaryAlloc.allocate(alignment, size, flags);
				if (byteAllocResult.ptr == nullptr) {
					byteAllocResult = mFallbackAlloc.allocate(alignment, size, flags);
				}
				return byteAllocResult;
			}

			Bool deallocate(Byte* ptr) noexcept {
				return region_manager::deallocate(ptr);
			}

			Bool optimize(const TimeoutTraker timeoutTraker) noexcept requires(isOptimizable) {
				Size allocCount = asNumber(primary_alloc::isOptimizable) + asNumber(fallback_alloc::isOptimizable);

				Bool completed = false;
				if constexpr (primary_alloc::isOptimizable) {
					if (IsAllocComposerC<primary_alloc>) {
						TimeoutTraker subTimeout;
						subTimeout.start(timeoutTraker.remainingTime() / allocCount);
						completed = completed && mPrimaryAlloc.optimize(subTimeout);
					} else {
						completed = completed && mPrimaryAlloc.optimize();
					}
				}
				if constexpr (fallback_alloc::isOptimizable) {
					if (IsAllocComposerC<fallback_alloc>) {
						TimeoutTraker subTimeout;
						subTimeout.start(timeoutTraker.remainingTime());
						completed = completed && mFallbackAlloc.optimize(subTimeout);
					} else {
						completed = completed && mFallbackAlloc.optimize();
					}
				}

				return completed;
			}
			Size mesh(const TimeoutTraker timeoutTraker, f32 freeRatioThreshold, Size maxMeshAttemptsPerAllocator) noexcept requires(isMeshable) {
				Size allocCount = asNumber(primary_alloc::isMeshable) + asNumber(fallback_alloc::isMeshable);
				Size meshCount = 0;

				if constexpr (primary_alloc::isMeshable) {
					if (IsAllocComposerC<primary_alloc>) {
						TimeoutTraker subTimeout;
						subTimeout.start(timeoutTraker.remainingTime() / allocCount);
						meshCount += primary_alloc::mesh(subTimeout, freeRatioThreshold, maxMeshAttemptsPerAllocator);
					}
				}
				Bool completed = true;
				if constexpr (fallback_alloc::isMeshable) {
					if (IsAllocComposerC<fallback_alloc>) {
						TimeoutTraker subTimeout;
						subTimeout.start(timeoutTraker.remainingTime());
						completed = completed && mFallbackAlloc.optimize(subTimeout);
					}
				}

				return completed;
			}
			static Bool merge(Alloc* targetAlloc, Alloc* donorAlloc) noexcept requires(isMergeable) {
				primary_alloc::merge(targetAlloc->mFallbackAlloc, donorAlloc->mFallbackAlloc);
				fallback_alloc::merge(targetAlloc->mFallbackAlloc, donorAlloc->mFallbackAlloc);
			};
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	struct OsAllocRoot {
		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc : public BaseAllocRoot {
		public:
			constexpr static inline Bool isMeshable = false;
			using region_manager = AllocRegionManager::template rebind<isMeshable, Alloc>;

		private:
			Size mAllocCount;

		public:
			Alloc() noexcept : mAllocCount(0) {};

			Bool isInit() noexcept {
				return true;
			}

			Bool init() noexcept {
				mAllocCount = 0;
				return true;
			};

			Bool deinit() noexcept {
				return mAllocCount != 0;
			}

			ByteAllocResult allocate(Size alignment, Size size, const AllocateFlags flags) noexcept {
				if (region_manager::metaDataSize != 0 && alignment > region_manager::metaDataSize) {
					return ByteAllocResult{};
				}
				if (region_manager::metaDataSize != 0) {
					size = max<Size>(region_manager::regionSize, size);
				}
				ByteAllocResult result = osMemoryAllocate(region_manager::regionSize, size, flags, pageSize);
				if (result.mPtr != nullptr) {
					region_manager::initRegion(result.mPtr, this);
					Byte* ptr = region_manager::getMemoryPtr(result.mPtr);
					result.mPtr = ptr;
					result.mSize = region_manager::getMemorySize();
					mAllocCount++;
				}
				return result;
			}

			Bool deallocate(Byte* ptr) noexcept override {
				if (region_manager::metaDataSize != 0) {
					ptr = alignDown(ptr, region_manager::regionSize);
				}

				if (mAllocCount != 0 && osMemoryDeallocate(ptr) == true) {
					mAllocCount--;
					return true;
				} else {
					return false;
				}
			}

			Bool isFull() noexcept {
				return false;
			}
			Bool isEmpty() noexcept {
				return mAllocCount == 0;
			}
			Bool couldOwn(Byte*) noexcept {
				return true;
			}
			Size capacity() noexcept {
				return Limits<Size>::max();
			}
			Size freeCapacity() noexcept {
				return Limits<Size>::max();
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<Size vSize, Size vAlignment>
	struct SignalStackAllocRoot {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc : public BaseAllocRoot {
		public:
			constexpr static inline Bool isMeshable = false;
			using region_manager = AllocRegionManager::template rebind<isMeshable, Alloc>;
			constexpr static inline Size alignment = max<Size>(vAlignment, region_manager::regionSize);

		private:
			alignas(alignment) Array<Byte*, vSize> mStorage;
			Bool* allocated;

		public:
			Alloc() noexcept
				: mStorage()
				, allocated(false) {
			};

			Bool isInit() noexcept {
				return true;
			}

			Bool init() noexcept {
				allocated = false;
				region_manager::initRegion(mStorage.data(), this);
				return true;
			};

			Bool deinit() noexcept {
				allocated = false;
				return true;
			}

			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				ByteAllocResult result;
				AlignPtrExpect alignExpect = alignPtr(alignment, size, region_manager::getMemoryPtr(mStorage.data()), freeCapacity());
				if (alignExpect.value()) {
					AlignPtrInfo alignInfo = alignExpect.value();
					result.mPtr = alignInfo.alignedPtr;
					result.mSize = size + alignInfo.remainingSpace;
					allocated = true;

					zeroAllocation(result, flags);
				} else {
					result.mPtr = 0;
					result.mSize = 0;
				}

				return result;
			}

			Bool deallocate(Byte* ptr) noexcept override {
				if (!couldOwn(ptr)) {
					return false;
				}
				allocated = false;
				return true;
			}

			Bool isFull() noexcept {
				return allocated == true;
			}
			Bool isEmpty() noexcept {
				return allocated == false;
			}
			Bool couldOwn(Byte* ptr) noexcept {
				return ptr >= mStorage.data() && ptr < (mStorage.data() + capacity());
			}
			Size capacity() noexcept {
				return region_manager::memorySize;
			}
			Size freeCapacity() noexcept {
				return allocated == true ? 0 : capacity();
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<Size vSize, typename RegionAlloc>
	struct SignalAllocRoot {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc : public BaseAllocRoot {
		public:
			using region_alloc = RegionAlloc;
			constexpr static inline Bool isMeshable = false;
			using region_manager = AllocRegionManager::template rebind<isMeshable, Alloc>;

		private:
			Byte* mRegion;
			Byte* mMemory;
			Size mSize;
			Bool* allocated;

		public:
			Alloc() noexcept
				: mRegion(nullptr)
				, mMemory(nullptr)
				, mSize(0)
				, allocated(false) {};

			Bool isInit() noexcept {
				return mMemory != nullptr;
			}

			Bool init() noexcept {
				ByteAllocResult result = region_alloc::allocateAligned(region_manager::regionSize, vSize);
				if (result.mPtr == nullptr) {
					return false;
				}
				Byte* region = result.mPtr;
				mRegion = region;

				region_manager::initRegion(region, this);
				mMemory = region_manager::getMemoryPtr(region);
				mSize = result.mSize;
				allocated = false;
				return true;
			};

			Bool deinit() noexcept {
				if (mRegion) {
					region_manager::removeAlloc(mRegion, this);
					if (!region_alloc::deallocate(mRegion)) {
						return false;
					}
				}

				mRegion = nullptr;
				mMemory = nullptr;
				mSize = 0;
				allocated = false;
				return true;
			}

			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				ByteAllocResult result;
				AlignPtrExpect alignExpect = alignPtr(alignment, size, mMemory + mSize, freeCapacity());
				if (alignExpect.value()) {
					AlignPtrInfo alignInfo = alignExpect.value();
					result.mPtr = alignInfo.alignedPtr;
					result.mSize = size + alignInfo.remainingSpace;
					allocated = true;

					zeroAllocation(result, flags);
				} else {
					result.mPtr = 0;
					result.mSize = 0;
				}

				return result;
			}

			Bool deallocate(Byte* ptr) noexcept override {
				if (!couldOwn(ptr)) {
					return false;
				}
				allocated = false;
				return true;
			}

			Bool isFull() noexcept {
				return allocated == true;
			}
			Bool isEmpty() noexcept {
				return allocated == false;
			}
			Bool couldOwn(Byte* ptr) noexcept {
				return ptr >= mMemory && ptr < (mMemory + capacity());
			}
			Size capacity() noexcept {
				return max<Size>(mSize, vSize);
			}
			Size freeCapacity() noexcept {
				return allocated == true ? 0 : capacity();
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<typename RegionAlloc>
	struct MonotonicAllocRoot {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc : public BaseAllocRoot {
		public:
			using region_alloc = RegionAlloc;
			constexpr static inline Bool isMeshable = false;
			using region_manager = AllocRegionManager::template rebind<isMeshable, Alloc>;

		private:
			Byte* mRegion;
			Byte* mMemory;
			Byte* mSize;

		public:
			Alloc() noexcept
				: mRegion(nullptr)
				, mMemory(nullptr)
				, mSize(0) {};

			Bool isInit() noexcept {
				return mMemory != nullptr;
			}

			Bool init() noexcept {
				Byte* region = region_alloc::allocateAligned(region_manager::regionSize, region_manager::regionSize);
				if (region == nullptr) {
					return false;
				}
				mRegion = region;

				region_manager::initRegion(region, this);
				mMemory = region_manager::getMemoryPtr(region);
				mSize = 0;
				return true;
			};

			Bool deinit() noexcept {
				if (mRegion) {
					region_manager::removeAlloc(mRegion, this);
					if (!region_alloc::deallocate(mRegion)) {
						return false;
					}
				}

				mRegion = nullptr;
				mMemory = nullptr;
				mSize = 0;
				return true;
			}

			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				ByteAllocResult result;

				AlignPtrExpect alignExpect = alignPtr(alignment, size, mMemory + mSize, freeCapacity());
				if (alignExpect.value()) {
					AlignPtrInfo alignInfo = alignExpect.value();
					result.mPtr = alignInfo.alignedPtr;
					result.mSize = size;
					mSize = ptrDistance(mMemory, alignInfo.nextPtr);
					zeroAllocation(result, flags);
				} else {
					result.mPtr = 0;
					result.mSize = 0;
				}

				return result;
			}

			Bool deallocate(Byte*) noexcept override {
				return true;
			}

			Bool isFull() noexcept {
				return mSize == region_manager::memorySize;
			}
			Bool isEmpty() noexcept {
				return mSize == 0;
			}
			Bool couldOwn(Byte* ptr) noexcept {
				return ptr >= mMemory && ptr < (mMemory + region_manager::memorySize);
			}
			Size capacity() noexcept {
				return region_manager::memorySize;
			}
			Size freeCapacity() noexcept {
				return region_manager::memorySize - mSize;
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<Size vBlockSize, typename RegionAlloc>
		requires(vBlockSize >= 16)
	struct SlabListAllocRoot {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc : public BaseAllocRoot {
		public:
			using region_alloc = RegionAlloc;
			constexpr static inline Bool isMeshable = region_alloc::isMeshable;
			using region_manager = AllocRegionManager::template rebind<isMeshable, Alloc>;
			using can_mesh_result_type = CanMeshResult<Alloc>;

		private:
			constexpr static inline Size subMaskSize = TypeBitSize<Size>;
			using mask_type = natl::Array<Size, natl::math::divCeil<Size>(region_manager::memorySize / vBlockSize, subMaskSize)>;
			constexpr static inline Size maxBlocks = region_manager::memorySize / vBlockSize;
			constexpr static inline Size freeTag = allocRootTag;
			constexpr static inline Size blockSize = vBlockSize;

			struct BlockMetaData {
				Size mTag;
				BlockMetaData* mNext;
			};

		private:
			Byte* mRegion;
			Byte* mMemory;
			Size mFreeCount;
			BlockMetaData* mFreeBlock;
			BlockMetaData* mRightmostFreeBlock;

		public:
			Alloc() noexcept
				: mRegion(nullptr)
				, mMemory(nullptr)
				, mFreeCount(0)
				, mFreeBlock(nullptr)
				, mRightmostFreeBlock(nullptr) {
			};

			Bool isInit() noexcept {
				return mMemory != nullptr;
			}

			Bool init() noexcept {
				ByteAllocResult region = region_manager::template regionAllocate<region_alloc>();
				if (region.mPtr == nullptr) {
					return false;
				}
				mRegion = region.mPtr;

				region_manager::initRegion(region.mPtr, this);
				mMemory = region_manager::getMemoryPtr(region.mPtr);
				mFreeCount = maxBlocks;
				mFreeBlock = nullptr;
				mRightmostFreeBlock = reinterpret_cast<BlockMetaData*>(mMemory);
				return true;
			};

			Bool deinit() noexcept {
				if (mRegion) {
					region_manager::removeAlloc(mRegion, this);
					if (!region_alloc::deallocate(mRegion)) {
						return false;
					}
				}

				mRegion = nullptr;
				mMemory = nullptr;
				mFreeCount = 0;
				mFreeBlock = nullptr;
				mRightmostFreeBlock = nullptr;
				return true;
			}

			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				ByteAllocResult result;

				if (alignment > blockSize || size > blockSize || mFreeCount == 0) {
					result.mPtr = nullptr;
					result.mSize = 0;
					return result;
				}

				mFreeCount--;
				if (mFreeBlock == nullptr) {
					result.mPtr = reinterpret_cast<Byte*>(mRightmostFreeBlock);
					result.mSize = blockSize;
					reinterpret_cast<Byte*&>(mRightmostFreeBlock) += blockSize;
				} else {
					if (mFreeBlock->mTag != freeTag) {
						result.mPtr = nullptr;
						result.mSize = 0;
						return result;
					}

					result.mPtr = reinterpret_cast<Byte*>(mFreeBlock);
					result.mSize = blockSize;

					mFreeBlock->mTag = 0;
					BlockMetaData* next = mFreeBlock->mNext;
					mFreeBlock->mNext = nullptr;
					mFreeBlock = next;
				}

				zeroAllocation(result, flags);

				return result;
			}
			Bool deallocate(Byte* ptr) noexcept override {
				if (!isRootAlloc()) {
					mFreeCount++;
					auto rootAlloc = region_manager::getRootAlloc(mRegion);
					return rootAlloc->deallocate(meshNormilizePtr(rootAlloc, this, ptr));
				}

				Byte* basePtr = region_manager::getMemoryPtr(alignDown(ptr, region_manager::regionSize));
				Size baseDelta = ptr - basePtr;
				Byte* alignedPtr = basePtr + (baseDelta / blockSize) * blockSize;

				BlockMetaData* blockMetaData = reinterpret_cast<BlockMetaData*>(alignedPtr);
				blockMetaData->mTag = freeTag;
				blockMetaData->mNext = mFreeBlock;
				mFreeBlock = blockMetaData;
				mFreeCount++;
				return true;
			}

			Bool isFull() noexcept {
				return mFreeCount == 0 || !isRootAlloc();
			}
			Bool isEmpty() noexcept {
				return mFreeCount == maxBlocks && (isRootAlloc() ? allocCount() == 1 : true);
			}
			Bool couldOwn(Byte* ptr) noexcept {
				return ptr >= mMemory && ptr < (mMemory + region_manager::getMemorySize());
			}
			Size capacity() noexcept {
				return maxBlocks * vBlockSize;
			}
			Size freeCapacity() noexcept {
				return isRootAlloc() ? mFreeCount * vBlockSize : 0;
			}

			Bool isRootAlloc() noexcept {
				return region_manager::isRootAlloc(mRegion, this);
			}
			Size allocCount() noexcept {
				return region_manager::allocCount(mRegion);
			}
			Size maxAllocCount() noexcept {
				return region_manager::maxAllocCount(mRegion);
			}
			Byte* getRegion() noexcept {
				return mRegion;
			}

			static Byte* meshNormilizePtr(const Alloc* rootAlloc, const Alloc* donorAlloc, Byte* ptr) noexcept {
				return rootAlloc->mMemory + (ptr - donorAlloc->mMemory);
			}
		private:
			const mask_type getFreeMask() noexcept {
				mask_type mask{};
				const BlockMetaData* current = mFreeBlock;

				while (current != nullptr) {
					Size index = (reinterpret_cast<const Byte*>(current) - mMemory) / blockSize;
					Size& submask = mask[index / subMaskSize];
					Size submaskIndex = index % subMaskSize;
					submask = setBit<Size>(submask, submaskIndex, true);
					current = current->mNext;
				}

				current = mRightmostFreeBlock;
				while (reinterpret_cast<const Byte*>(current) < (mMemory + region_manager::getMemorySize())) {
					Size index = (reinterpret_cast<const Byte*>(current) - mMemory) / blockSize;
					Size& submask = mask[index / subMaskSize];
					Size submaskIndex = index % subMaskSize;
					submask = setBit<Size>(submask, submaskIndex, true);
					current = reinterpret_cast<const BlockMetaData*>(reinterpret_cast<const Byte*>(current) + blockSize);
				}
				return mask;
			}

			static mask_type getNotFreeMask(mask_type freeMask) noexcept {
				mask_type notFreeMask{};
				for (Size maskIndex = 0; maskIndex < freeMask.size(); maskIndex++) {
					notFreeMask[maskIndex] = ~freeMask[maskIndex];
				}
				if (maxBlocks % subMaskSize == 0) {
					return notFreeMask;
				}

				Size& backMask = notFreeMask.back();
				for (Size i = maxBlocks % subMaskSize; i < subMaskSize; i++) {
					backMask = setBit(backMask, i, false);
				}
				return notFreeMask;
			}

		public:

			static can_mesh_result_type canMesh(Size maxMeshMoves, Alloc* allocA, Alloc* allocB) noexcept requires(region_alloc::isMeshable) {
				can_mesh_result_type result;
				result.mCanMesh = false;

				if (!region_manager::template canMesh<region_alloc>(result, maxMeshMoves, allocA, allocB)) {
					return result;
				}


				mask_type targetNotFreeMask = getNotFreeMask(result.mTargetAlloc->getFreeMask());
				mask_type donorNotFreeMask = getNotFreeMask(result.mDonorAlloc->getFreeMask());
				
				for (Size maskIndex = 0; maskIndex < targetNotFreeMask.size(); maskIndex++) {
					if ((targetNotFreeMask[maskIndex] & donorNotFreeMask[maskIndex]) != 0) {
						return result;
					}
				}

				result.mCanMesh = true;
				return result;
			}

			static void meshMemcpy(Alloc* targetAlloc, Alloc* donorAlloc) noexcept requires(region_alloc::isMeshable) {
				mask_type targetFreeMask = targetAlloc->getFreeMask();
				mask_type donorFreeMask = donorAlloc->getFreeMask();

				natl::Array<Size, maxBlocks> freePages;
				Size freePagesSize = 0;
				mask_type donorNonFreeMask = getNotFreeMask(donorFreeMask);


				for (Size maskIndex = 0; maskIndex < donorNonFreeMask.size(); maskIndex++) {
					Size& targetFreeSubmask = targetFreeMask[maskIndex];
					Size& donorNotFreeSubmask = donorNonFreeMask[maskIndex];
					for (Size subMaskIndex = 0; subMaskIndex < subMaskSize; subMaskIndex++) {
						Size index = (maskIndex * subMaskSize) + subMaskIndex;
						if (index < maxBlocks) {
							if (getBit<Size>(donorNotFreeSubmask, index)) {
								Byte* targetPage = targetAlloc->mMemory + (index * blockSize);
								Byte* donorPage = donorAlloc->mMemory + (index * blockSize);
								memcpy(targetPage, donorPage, blockSize);
							} else if (getBit<Size>(targetFreeSubmask, index)) {
								freePages[freePagesSize] = index;
								freePagesSize++;
							}
						}
					}
				}

				targetAlloc->mFreeBlock = nullptr;
				while (freePagesSize != 0) {
					Size freePageIndex = freePages[freePagesSize - 1];
					Byte* byteBlockMetaData = targetAlloc->mMemory + (freePageIndex * blockSize);
					BlockMetaData* blockMetaData = reinterpret_cast<BlockMetaData*>(byteBlockMetaData);
					blockMetaData->mTag = freeTag;
					blockMetaData->mNext = targetAlloc->mFreeBlock;

					targetAlloc->mFreeBlock = blockMetaData;
					freePagesSize--;
				}

				//relative
				Size rTargetRightMost = reinterpret_cast<Byte*>(targetAlloc->mRightmostFreeBlock) - targetAlloc->mMemory;
				Size rDonorRightMost = reinterpret_cast<Byte*>(donorAlloc->mRightmostFreeBlock) - donorAlloc->mMemory;

				if (rDonorRightMost > rTargetRightMost) {
					targetAlloc->mRightmostFreeBlock = reinterpret_cast<BlockMetaData*>(targetAlloc->mMemory + rDonorRightMost);
				}

				Size donorNotFreeCount = maxBlocks - donorAlloc->mFreeCount;
				targetAlloc->mFreeCount -= donorNotFreeCount;
			}

			static void mesh(Alloc* targetAlloc, Alloc* donorAlloc) noexcept requires(region_alloc::isMeshable) {
				allocPartMesh<Alloc, region_manager, region_alloc>(targetAlloc, donorAlloc, meshMemcpy);
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<typename RegionAlloc>
	struct FreeListAllocRoot {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc : public BaseAllocRoot {
		public:
			using region_alloc = RegionAlloc;
			constexpr static inline Bool isMeshable = region_alloc::isMeshable;
			using region_manager = AllocRegionManager::template rebind<isMeshable, Alloc>;

		private:
			struct BlockMetaData {
				Size mIsFree : 1;
				Size mTag : 63;
				Size mSize;
				BlockMetaData* mNext;
				BlockMetaData* mPrevious;
			};
			constexpr static inline Size blockMetaDataSize = sizeof(BlockMetaData);
			constexpr static inline Size tagValue = 0x0FFFFFFFFFFFFFFF & allocRootTag;
			constexpr static inline Size isTargetSentinal = Limits<Size>::max();
			constexpr static inline Size minMeshNewBlockSize = blockMetaDataSize * 2;
			constexpr static inline Size maxFreeCapacity = region_manager::memorySize - blockMetaDataSize;

		private:
			Byte* mRegion;
			Byte* mMemory;
			Size mFreeCapacity;
			BlockMetaData* mFreeBlock; // circular doubly linked list 
			Size mAllocCount;

		private:
			void corruptedFreeList() noexcept {}
			BlockMetaData* ptrToMetaData(Byte* ptr) noexcept {
				return reinterpret_cast<BlockMetaData*>(ptr - blockMetaDataSize);
			}
			Byte* metaDataToData(BlockMetaData* metaData) noexcept {
				return reinterpret_cast<Byte*>(metaData) + blockMetaDataSize;
			}

		public:
			Alloc() noexcept
				: mRegion(nullptr)
				, mMemory(nullptr)
				, mFreeCapacity(0)
				, mFreeBlock(nullptr)
				, mAllocCount(0) {
			};

			Bool isInit() noexcept {
				return mRegion != nullptr;
			}
			Bool init() noexcept {
				ByteAllocResult region = region_manager::template regionAllocate<region_alloc>();
				if (region.mPtr == nullptr) {
					return false;
				}
				mRegion = region.mPtr;

				region_manager::initRegion(mRegion, this);
				mMemory = region_manager::getMemoryPtr(mRegion);
				mFreeCapacity = maxFreeCapacity;
				mAllocCount = 0;

				mFreeBlock = reinterpret_cast<BlockMetaData*>(mMemory);
				mFreeBlock->mIsFree = true;
				mFreeBlock->mSize = region_manager::getMemorySize() - blockMetaDataSize;
				mFreeBlock->mNext = mFreeBlock;
				mFreeBlock->mPrevious = mFreeBlock;
				mFreeBlock->mTag = tagValue;
			}
			Bool deinit() noexcept {
				if (mRegion) {
					region_manager::removeAlloc(mRegion, this);
					if (!region_alloc::deallocate(mRegion)) {
						return false;
					}
				}

				mRegion = nullptr;
				mMemory = nullptr;
				mFreeCapacity = 0;
				mFreeBlock = nullptr;
				mAllocCount = 0;

				return true;
			}
			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				if (freeCapacity() < size) {
					ByteAllocResult result;
					result.mPtr = nullptr;
					result.mSize = 0;
					return result;
				}

				const Size minAlignment = max<Size>(8, alignment);

				BlockMetaData* startBlock = mFreeBlock;
				BlockMetaData* currentBlock = startBlock;
				while (true) {
					if (currentBlock->mIsFree && currentBlock->mSize >= size + blockMetaDataSize) {
						if (minAlignment == 8) { // alloc from front
							Byte* allocPtr = metaDataToData(currentBlock);
							BlockMetaData* newBlock = reinterpret_cast<BlockMetaData*>(allocPtr + size);

							newBlock->mIsFree = true;
							newBlock->mSize = currentBlock->mSize - size;
							newBlock->mPrevious = currentBlock;
							newBlock->mNext = currentBlock->mNext;
							newBlock->mTag = tagValue;

							currentBlock->mIsFree = false;
							currentBlock->mSize = size;
							currentBlock->mNext->mPrevious = newBlock;
							currentBlock->mNext = newBlock;

							mFreeCapacity -= (size + blockMetaDataSize);

							if (newBlock->mSize >= mFreeBlock->mSize || mFreeBlock->mIsFree == false) {
								mFreeBlock = newBlock;
							}

							ByteAllocResult result;
							result.mPtr = allocPtr;
							result.mSize = size;
							zeroAllocation(result, flags);
							mAllocCount++;
							return result;
						} else { // alloc from back
							Byte* beginPtr = reinterpret_cast<Byte*>(currentBlock) + blockMetaDataSize;
							Byte* endPtr = beginPtr + currentBlock->mSize;
							Byte* allocPtr = endPtr - (size + blockMetaDataSize);
							allocPtr = alignDown(allocPtr, alignof(BlockMetaData));
							BlockMetaData* newBlock = reinterpret_cast<BlockMetaData*>(allocPtr - blockMetaDataSize);

							if (allocPtr != nullptr && beginPtr <= reinterpret_cast<Byte*>(newBlock)) {
								Size blockSize = endPtr - allocPtr;

								newBlock->mIsFree = false;
								newBlock->mSize = blockSize;
								newBlock->mPrevious = currentBlock;
								newBlock->mNext = currentBlock->mNext;
								newBlock->mTag = tagValue;

								currentBlock->mIsFree = true;
								currentBlock->mSize -= blockSize + blockMetaDataSize;
								currentBlock->mNext->mPrevious = newBlock;
								currentBlock->mNext = newBlock;
								currentBlock->mTag = tagValue;

								mFreeCapacity -= (blockSize + blockMetaDataSize);

								if (currentBlock->mSize >= mFreeBlock->mSize || mFreeBlock->mIsFree == false) {
									mFreeBlock = currentBlock;
								}

								ByteAllocResult result;
								result.mPtr = allocPtr;
								result.mSize = blockSize;
								zeroAllocation(result, flags);
								mAllocCount++;
								return result;
							}
						}
					}

					currentBlock = currentBlock->mNext;

					if (currentBlock == startBlock) {
						ByteAllocResult result;
						result.mPtr = nullptr;
						result.mSize = 0;
						return result;
					}
				}
			}
			Bool deallocate(Byte* ptr) noexcept override {
				BlockMetaData* currentBlock = ptrToMetaData(ptr);
				if (currentBlock->mTag != tagValue) {
					return false;
				}

				if (!isRootAlloc()) {
					mAllocCount--;
					auto rootAlloc = region_manager::getRootAlloc(mRegion);
					return rootAlloc->deallocate(meshNormilizePtr(rootAlloc, this, ptr));
				}

				BlockMetaData* nextBlock = currentBlock->mNext;
				BlockMetaData* previousBlock = currentBlock->mPrevious;

				if (currentBlock == previousBlock && currentBlock == nextBlock) {
					return true;
				}

				Bool mergePreviousBlock = currentBlock != previousBlock && previousBlock->mIsFree && currentBlock > previousBlock;
				Bool mergeNextBlock = currentBlock != nextBlock && nextBlock->mIsFree && currentBlock < nextBlock;

				Size freeSize;
				BlockMetaData* mergedBlock;
				if (mergePreviousBlock && mergeNextBlock) {
					nextBlock->mTag = 0;
					currentBlock->mTag = 0;
					previousBlock->mSize += currentBlock->mSize + nextBlock->mSize + blockMetaDataSize + blockMetaDataSize;
					previousBlock->mNext = nextBlock->mNext;
					freeSize = previousBlock->mSize;
					mergedBlock = previousBlock;
				} else if (mergePreviousBlock) {
					currentBlock->mTag = 0;
					previousBlock->mSize += currentBlock->mSize + blockMetaDataSize;
					previousBlock->mNext = currentBlock->mNext;
					freeSize = previousBlock->mSize;
					mergedBlock = previousBlock;

					if (previousBlock == nextBlock) {
						currentBlock->mPrevious = currentBlock;
					}
				} else if (mergeNextBlock) {
					nextBlock->mTag = 0;
					currentBlock->mIsFree = true;
					currentBlock->mSize += nextBlock->mSize;
					currentBlock->mNext = nextBlock->mNext;
					freeSize = currentBlock->mSize;
					mergedBlock = currentBlock;

					if (previousBlock == nextBlock) {
						currentBlock->mPrevious = currentBlock;
					}
				} else {
					currentBlock->mIsFree = true;
					mergedBlock = currentBlock;
					freeSize = currentBlock->mSize;
				}

				if (mergedBlock->mSize >= mFreeBlock->mSize) {
					mFreeBlock = mergedBlock;
				}
				mFreeCapacity += freeSize;
				mAllocCount--;

				return true;
			}

			Bool isFull() noexcept {
				return mFreeCapacity == 0 || !isRootAlloc();
			}
			Bool isEmpty() noexcept {
				return mAllocCount == 0 && (isRootAlloc() ? allocCount() == 1 : true);
			}
			Bool couldOwn(Byte* ptr) noexcept {
				return ptr >= mMemory && ptr < (mMemory + region_manager::getMemorySize());
			}
			Size capacity() noexcept {
				return maxFreeCapacity;
			}
			Size freeCapacity() noexcept {
				return isRootAlloc() ? mFreeCapacity : 0;
			}

			Bool isRootAlloc() noexcept {
				return region_manager::isRootAlloc(mRegion, this);
			}
			Size allocCount() noexcept {
				return region_manager::allocCount(mRegion);
			}
			Size maxAllocCount() noexcept {
				return region_manager::maxAllocCount(mRegion);
			}

			static Byte* meshNormilizePtr(const Alloc* targetAlloc, const Alloc* donorAlloc, Byte* ptr) noexcept {
				return targetAlloc->mMemory + (ptr - donorAlloc->mMemory);
			}

			static Bool canMesh(const Alloc* targetAlloc, const Alloc* donorAlloc) noexcept {
				if (!isMeshable) {
					return false;
				}
				if (!targetAlloc->isRootAlloc() && !donorAlloc->isRootAlloc()) {
					return false;
				}
				if (targetAlloc->allocCount() + donorAlloc->allocCount() > targetAlloc->maxAllocCount()) {
					return false;
				}
				if (!region_alloc::canMesh(targetAlloc->mRegion, region_manager::regionSize, donorAlloc->mRegion, region_manager::regionSize)) {
					return false;
				}

				const BlockMetaData* targetStartBlock = reinterpret_cast<const BlockMetaData*>(targetAlloc->mMemory);
				const BlockMetaData* donorStartBlock = reinterpret_cast<const BlockMetaData*>(donorAlloc->mMemory);
				const BlockMetaData* targetBlock = targetStartBlock;
				const BlockMetaData* donorBlock = donorStartBlock;
				while (true) {
					if (targetBlock->mTag != tagValue) {
						corruptedFreeList();
						return false;
					}

					while (true) {
						if (donorBlock->mTag != tagValue) {
							corruptedFreeList();
							return false;
						}

						Size rDonorSize = donorBlock->mSize;
						Size rDonorStart = reinterpret_cast<Size>(donorBlock) - reinterpret_cast<Size>(donorStartBlock);
						Size rDonorEnd = rDonorStart + rDonorSize + blockMetaDataSize;

						Size rTargetSize = targetBlock->mSize;
						Size rTargetStart = reinterpret_cast<Size>(targetBlock) - reinterpret_cast<Size>(targetStartBlock);
						Size rTargetEnd = rTargetStart + rTargetSize + blockMetaDataSize;

						if (rDonorStart > rTargetEnd) {
							break;
						}

						if (donorBlock->mIsFree == false) {
							// Target and Donor header start at same index
							if (rTargetStart == rDonorStart && targetBlock->mIsFree == true) {
								//Target block and Donor block are equal in size 
								Bool targetDonorEqual = targetBlock->mSize == rDonorSize;
								//Target block can fit Donor block plus block meta data header
								Bool targetFitDonorAndHeader = targetBlock->mSize >= rDonorSize + blockMetaDataSize;
								if (!targetDonorEqual && !targetFitDonorAndHeader) {
									return false;
								}
							} else if (targetBlock->mIsFree == true) { // Target free block header collide with Donor non-free block
								if (buffersOverlapIndexRange(rTargetStart, rTargetStart + blockMetaDataSize, rDonorStart, rDonorEnd)) {
									return false;
								}
							} else { // Target non-free block overlaps with Donor non-free block
								if (buffersOverlapIndexRange(rTargetStart, rTargetEnd, rDonorStart, rDonorEnd)) {
									return false;
								}
							}
						}

						if (donorBlock->mNext == donorStartBlock) {
							return true;
						}
						donorBlock = donorBlock->mNext;
					}

					if (targetBlock->mNext == targetStartBlock) {
						break;
					}

					targetBlock = targetBlock->mNext;
				}

				return true;
			}

		private:
			static void meshFreeLists(FreeListAllocRoot* targetAlloc, FreeListAllocRoot* donorAlloc) noexcept {
				const BlockMetaData* targetStartBlock = reinterpret_cast<const BlockMetaData*>(targetAlloc->mMemory);
				const BlockMetaData* donorStartBlock = reinterpret_cast<const BlockMetaData*>(donorAlloc->mMemory);
				const BlockMetaData* targetBlock = targetStartBlock;
				const BlockMetaData* donorBlock = donorStartBlock;
				while (true) {
					while (true) {
						Size rDonorSize = donorBlock->mSize;
						Size rDonorStart = reinterpret_cast<Size>(donorBlock) - reinterpret_cast<Size>(donorStartBlock);
						Size rDonorEnd = rDonorStart + rDonorSize + blockMetaDataSize;

						Size rTargetSize = targetBlock->mSize;
						Size rTargetStart = reinterpret_cast<Size>(targetBlock) - reinterpret_cast<Size>(targetStartBlock);
						Size rTargetEnd = rTargetStart + rTargetSize + blockMetaDataSize;

						if (rDonorStart > rTargetEnd) {
							break;
						}

						if (donorBlock->mIsFree == false && targetBlock->mIsFree == true) {
							// Target and Donor header start at same index
							if (rTargetStart == rDonorStart) {
								targetBlock->mIsFree = false;
								Byte* targetData = metaDataToData(targetBlock);
								Byte* donorData = metaDataToData(donorBlock);
								memcpy(targetData, donorData, rDonorSize);

								Size targetExtraSize = rTargetSize - rDonorSize;
								if (targetExtraSize >= minMeshNewBlockSize) {
									targetBlock->mSize = rDonorSize;

									BlockMetaData* newBlock = reinterpret_cast<BlockMetaData*>(donorData + rDonorSize);
									newBlock->mIsFree = true;
									newBlock->mTag = tagValue;
									newBlock->mSize = targetExtraSize - blockMetaDataSize;

									newBlock->mPrevious = targetBlock;
									newBlock->mNext = targetBlock->mNext;

									targetBlock->mNext->mPrevious = newBlock;
									targetBlock->mNext = newBlock;

									targetBlock = newBlock;
								}
							} else {
								Size newTargetFullSize = rDonorStart - rTargetStart;
								Size newTargetSize = newTargetFullSize - blockMetaDataSize;
								Size donorFullSize = rDonorSize + blockMetaDataSize;
								Size targetExtraFullSize = rTargetSize - (newTargetFullSize + donorFullSize);

								targetBlock->mSize = newTargetSize;

								BlockMetaData* donorInTargetBlock = reinterpret_cast<BlockMetaData*>(reinterpret_cast<Byte*>(targetStartBlock) + rDonorStart);
								Byte* targetData = metaDataToData(donorInTargetBlock);
								Byte* donorData = metaDataToData(donorBlock);
								memcpy(targetData, donorData, rDonorSize);

								donorInTargetBlock->mTag = tagValue;
								donorInTargetBlock->mIsFree = false;

								if (targetExtraFullSize >= minMeshNewBlockSize) {
									BlockMetaData* extraBlock = reinterpret_cast<BlockMetaData*>(reinterpret_cast<Byte*>(donorInTargetBlock) + donorFullSize);
									extraBlock->mIsFree = true;
									extraBlock->mTag = tagValue;
									extraBlock->mSize = targetExtraFullSize - blockMetaDataSize;

									extraBlock->mPrevious = donorInTargetBlock;
									extraBlock->mNext = targetBlock->mNext;

									donorInTargetBlock->mSize = rDonorSize;
									donorInTargetBlock->mPrevious = targetBlock;
									donorInTargetBlock->mNext = extraBlock;

									targetBlock->mNext->mPrevious = extraBlock;
									targetBlock->mNext = donorInTargetBlock;
								} else {
									donorInTargetBlock->mSize = rDonorSize + targetExtraFullSize;
									donorInTargetBlock->mPrevious = targetBlock;
									donorInTargetBlock->mNext = targetBlock->mNext;

									targetBlock->mNext->mPrevious = donorInTargetBlock;
									targetBlock->mNext = donorInTargetBlock;
								}
							}
						}

						if (donorBlock->mNext == donorStartBlock) {
							return;
						}
						donorBlock = donorBlock->mNext;
					}

					if (targetBlock->mNext == targetStartBlock) {
						break;
					}

					targetBlock = targetBlock->mNext;
				}

				return;
			}

		public:

			static void mesh(Alloc* targetAlloc, Alloc* donorAlloc) noexcept {
				meshFreeLists(targetAlloc, donorAlloc);

				Size donorNotFreeCapacity = maxFreeCapacity - donorAlloc->mFreeCapacity;
				targetAlloc->mFreeCapacity -= donorNotFreeCapacity;
				targetAlloc->mAllocCount += donorAlloc->mAllocCount;
				region_manager::mesh(targetAlloc->mRegion, donorAlloc->mRegion);
				region_alloc::mesh(targetAlloc->mRegion, region_manager::regionSize, donorAlloc->mRegion, region_manager::regionSize);
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<Size vBlockSize, typename RegionAlloc>
	struct SlabBitmapAllocRoot {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager>)
		struct Alloc : public BaseAllocRoot {
		public:
			using region_alloc = RegionAlloc;
			constexpr static inline Bool isMeshable = region_alloc::isMeshable;
			using region_manager = AllocRegionManager::template rebind<isMeshable, Alloc>;
			using can_mesh_result_type = CanMeshResult<Alloc>;
			friend AllocRegionManager;

		private:
			using mask_type = natl::Array<Size, natl::math::divCeil<Size>(region_manager::memorySize / vBlockSize, TypeBitSize<Size>)>;
			constexpr static inline Size blockSize = vBlockSize;
			constexpr static inline Size maxBlocks = region_manager::memorySize / vBlockSize;
			constexpr static inline Size blocksPreSubMask = TypeBitSize<Size>;

		private:
			Size mFreeCount;
			Byte* mRegion;
			Byte* mMemory;
			Size mNotFreeMaskIndex;
			mask_type mNotFreeMask;

		public:
			Alloc() noexcept 
				: mFreeCount(0)
				, mRegion(nullptr)
				, mMemory(nullptr)
				, mNotFreeMaskIndex(0)
				, mNotFreeMask(mask_type{}) {};

			Bool isInit() noexcept {
				return mMemory != nullptr;
			}

			Bool init() noexcept {
				ByteAllocResult region = region_manager::template regionAllocate<region_alloc>();
				if (region.mPtr == nullptr) {
					return false;
				}
				mRegion = region.mPtr;

				region_manager::initRegion(mRegion, this);
				mMemory = region_manager::getMemoryPtr(mRegion);
				mFreeCount = maxBlocks;
				mNotFreeMaskIndex = 0;
				mNotFreeMask = mask_type{};
				return true;
			};

			Bool deinit() noexcept {
				if (mRegion) {
					region_manager::removeAlloc(mRegion, this);
					region_alloc::deallocate(mRegion);
				}

				mRegion = nullptr;
				mMemory = nullptr;
				mFreeCount = 0;
				mNotFreeMaskIndex = 0;
				mNotFreeMask = mask_type{};
				return true;
			}

			ByteAllocResult allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				ByteAllocResult result;

				if (size > vBlockSize || alignment > maxAlignment(blockSize) || isFull() || !isStandardValidFlags(flags)) {
					result.mPtr = nullptr;
					result.mSize = 0;
					return result;
				}

				Byte* ptr = nullptr;
				while (true) {
					Size& subMask = mNotFreeMask[mNotFreeMaskIndex];
					Size subMaskIndex = 0;

					Size index = mNotFreeMaskIndex * blocksPreSubMask;
					if (bitscanForward(~subMask, subMaskIndex) && (index + subMaskIndex) < maxBlocks) {
						ptr = mMemory + (blocksPreSubMask * mNotFreeMaskIndex + subMaskIndex) * blockSize;
						subMask = setBit<Size>(subMask, subMaskIndex, true);
						break;
					}
					mNotFreeMaskIndex = (mNotFreeMaskIndex + 1) % mNotFreeMask.size();
				}

				mFreeCount--;
				result.mPtr = ptr;
				result.mSize = blockSize;
				zeroAllocation(result, flags);
				return result;
			}

			Bool deallocate(Byte* ptr) noexcept override {
				if (!isRootAlloc()) {
					mFreeCount++;
					auto rootAlloc = region_manager::getRootAlloc(mRegion);
					return rootAlloc->deallocate(meshNormilizePtr(rootAlloc, this, ptr));
				}

				const Size index = (ptr - mMemory) / blockSize;
				const Size freeMaskIndex = index / blocksPreSubMask;
				const Size subMaskIndex = index % blocksPreSubMask;

				Size& subMask = mNotFreeMask[freeMaskIndex];
				subMask = setBit<Size>(subMask, subMaskIndex, false);
				mFreeCount++;
				return true;
			}

			Bool isFull() noexcept {
				return mFreeCount == 0 || !isRootAlloc();
			}
			Bool isEmpty() noexcept {
				return mFreeCount == maxBlocks && (isRootAlloc() ? allocCount() == 1 : true);
			}
			Bool couldOwn(Byte* ptr) noexcept {
				return ptr >= mMemory && ptr < mMemory + capacity();
			}
			Size capacity() noexcept {
				return region_manager::getMemorySize();
			}
			Size freeCapacity() noexcept {
				return isRootAlloc() ? mFreeCount * blockSize : 0;
			}

			Bool isRootAlloc() noexcept {
				return region_manager::isRootAlloc(mRegion, this);
			}
			Size allocCount() noexcept {
				return region_manager::allocCount(mRegion);
			}
			Size maxAllocCount() noexcept {
				return region_manager::maxAllocCount(mRegion);
			}
			Byte* getRegion() noexcept {
				return mRegion;
			}

			static Byte* meshNormilizePtr(const Alloc* rootAlloc, const Alloc* donorAlloc, Byte* ptr) noexcept {
				return rootAlloc->mMemory + (ptr - donorAlloc->mMemory);
			}

			static can_mesh_result_type canMesh(Size maxMeshMoves, Alloc* allocA, Alloc* allocB) noexcept requires(region_alloc::isMeshable) {
				can_mesh_result_type result;
				if (!region_manager::template canMesh<region_alloc>(result, maxMeshMoves, allocA, allocB)) {
					return result;
				}

				for (Size freeMaskI = 0; freeMaskI < result.mTargetAlloc->mNotFreeMask.size(); freeMaskI++) {
					if ((result.mTargetAlloc->mNotFreeMask[freeMaskI] & result.mDonorAlloc->mNotFreeMask[freeMaskI]) != 0) {
						return result;
					}
				}

				result.mCanMesh = true;
				return result;
			}

			static void meshMemcpy(Alloc* targetAlloc, Alloc* donorAlloc) noexcept requires(region_alloc::isMeshable) {
				Byte* targetBlocksPos = targetAlloc->mMemory;
				Size* targetMaskPos = targetAlloc->mNotFreeMask.data();
				Size* targetMaskEnd = targetAlloc->mNotFreeMask.data() + targetAlloc->mNotFreeMask.size();

				Byte* donorBlocksPtr = donorAlloc->mMemory;
				Size* donorMaskPos = donorAlloc->mNotFreeMask.data();

				for (; targetMaskPos < targetMaskEnd; targetMaskPos++, donorMaskPos++) {
					Size& targetMask = *targetMaskPos;
					Size& donorMask = *donorMaskPos;

					Size subMask = donorMask;
					Size subMaskIndex = 0;
					while (bitscanForward(subMask, subMaskIndex)) {
						Byte* targetBlockPtr = targetBlocksPos + subMaskIndex * blockSize;
						Byte* donorBlockPtr = donorBlocksPtr + subMaskIndex * blockSize;

						memcpy(targetBlockPtr, donorBlockPtr, blockSize);
						subMask = setBit<Size>(subMask, subMaskIndex, false);
					}

					targetMask = targetMask | donorMask;
					targetBlocksPos += blocksPreSubMask * blockSize;
					donorBlocksPtr += blocksPreSubMask * blockSize;
				}

				Size donorNotFreeCount = (maxBlocks - donorAlloc->mFreeCount);
				targetAlloc->mFreeCount -= donorNotFreeCount;
			}

			static void mesh(Alloc* targetAlloc, Alloc* donorAlloc) noexcept requires(region_alloc::isMeshable) {
				allocPartMesh<Alloc, region_manager, region_alloc>(targetAlloc, donorAlloc, meshMemcpy);
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<template<typename> typename AllocComposer, typename SupportAlloc, Size vSmallThreadCount>
	struct ThreadLocalAllocComposer {

		template<typename AllocRegionManager>
			requires(IsAllocRegionManagerC<AllocRegionManager> && (AllocRegionManager::threadLocal == true))
		struct Alloc {
		public:
			constexpr static inline Size smallThreadCount = vSmallThreadCount;
			constexpr static inline Bool isMeshable = false;
			using alloc_composer = AllocComposer<AllocRegionManager>;
			using alloc_result = ByteAllocResult;
			using region_manager = AllocRegionManager::template rebind<false>;

			struct FreeList {
			public:
				struct Node {
					Node* mNext;
				};

			private:
				Node* mHead;
				Size* mSize;

			public:
				FreeList() noexcept : mHead(nullptr), mSize(0) {}
				FreeList(Node* headIn, Size* sizeIn) noexcept : mHead(headIn), mSize(sizeIn) {}

				void push(Node* node) noexcept {
					node->mNext = mHead;
					mHead = node;
					++mSize;
				}

				Node* pop() noexcept {
					if (!mHead) {
						mSize = 0;
						return nullptr;
					}

					Node* node = mHead;
					mHead = node->next;
					node->next = nullptr;
					--mSize;
					return node;
				}

				Bool empty() const noexcept {
					return mHead == nullptr;
				}

				Size size() const noexcept {
					return mSize;
				}
			};

			struct RemoteFreeList {
			public:
				struct Node {
					Node* mNext;
				};

			private:
				Atomic<Node*> mHead;
				Atomic<Size> mSize;

			public:
				RemoteFreeList() noexcept : mHead(nullptr), mSize(0) {}
				RemoteFreeList(const RemoteFreeList&) = delete;
				RemoteFreeList& operator=(const RemoteFreeList&) = delete;

				// Multi-producer
				void push(Node* node) noexcept {
					Node* oldHead = mHead.load(MemoryOrder::relaxed);
					do {
						node->mNext = oldHead;
					} while (!mHead.compare_exchange_weak(
						oldHead,
						node,
						MemoryOrder::release,
						MemoryOrder::relaxed
					));
					mSize.fetch_add(1, MemoryOrder::relaxed);
				}

				// Single-consumer
				FreeList drain() noexcept {
					Node* head = mHead.exchange(nullptr, MemoryOrder::acquire);
					Size dSize = size();
					mSize.store(0, MemoryOrder::relaxed);
					return FreeList(head, dSize);
				}
				// Approximate
				Size size() const noexcept {
					return mSize.load(MemoryOrder::relaxed);
				}

				Bool isEmpty() const noexcept {
					return size() == 0;
				}
			};

			struct AllocInfo {
				alloc_composer mAlloc;
				AllocThreadId mAllocThreadId;
				Size mNumAllocs;
				Size mNumDeallocs;
				FreeList mFreeList;
				RemoteFreeList mRemoteFreeList;

				Size liveAllocations() const noexcept {
					return natl::math::subsat(mNumAllocs, mNumDeallocs);
				}

				void freeLists(FreeList& freeList) noexcept {
					typename FreeList::Node* node = freeList.pop();
					while (node != nullptr) {
						typename FreeList::Node* nextNode = freeList.pop();
						if (mAlloc.deallocate(bitCast<Byte*>(node))) {
							mNumDeallocs++;
						}
						node = nextNode;
					}
				}

				void emptyFreeLists() noexcept {
					FreeList drainedFreeList = mRemoteFreeList.drain();
					freeLists(drainedFreeList);
					freeLists(mFreeList);
				}
			};

			struct AllocInfoCompare {
				Bool operator()(const AllocInfo& lhs, const AllocInfo& rhs) const noexcept {
					return lhs.liveAllocations() < rhs.liveAllocations();
				}
			};

			static Mutex mGlobalMutex;
			static SmallDynArray<AllocInfo*, smallThreadCount, SupportAlloc> mAllThreadAllocs;
			static PriorityQueue<AllocInfo*, SmallDynArray<AllocInfo*, smallThreadCount, SupportAlloc>, AllocInfoCompare> mFreeThreadAllocs;


			struct ThreadAlloc {
				AllocInfo* mAllocInfo;

				ThreadAlloc() noexcept {
					mGlobalMutex.lock();

					if (mFreeThreadAllocs.isEmpty()) {
						AllocInfo* allocInfo = make<AllocInfo, SupportAlloc>();
						allocInfo->mAllocThreadId = bitCast<AllocThreadId>(allocInfo);
						allocInfo->mNumAllocs = 0;
						allocInfo->mNumDeallocs = 0;
						allocInfo->mRemoteFreeList = nullptr;
						allocInfo->mAlloc.init();

						mAllThreadAllocs.pushBack(allocInfo);
						mAllocInfo = allocInfo;
					} else {
						mAllocInfo = mFreeThreadAllocs.top();
						mFreeThreadAllocs.pop();
					}

					region_manager::setAllocThreadIdOfThread(mAllocInfo->mAllocThreadId);

					mGlobalMutex.unlock();
				}
			};
			static thread_local ThreadAlloc mThreadAlloc;

		public:
			Bool isInit() noexcept {
				return true;
			}
			Bool init() noexcept {
				return true;
			}
			Bool deinit() noexcept {
				return true;
			}

			alloc_result allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				alloc_result result = mThreadAlloc->mAllocInfo->mAlloc.allocate(alignment, max<Size>(16, size), flags);
				if (result.mPtr != 0) {
					mThreadAlloc->mAllocInfo->mNumAllocs += 1;
				}
				return result;

			}

			Bool deallocate(Byte* ptr) noexcept {
				AllocThreadId allocationsThreadId = region_manager::getAllocThreadId(ptr);
				AllocInfo* allocInfo = bitCast<AllocInfo*>(allocationsThreadId);
				if (allocationsThreadId == region_manager::getAllocThreadIdOfThread()) {
					FreeList& freeList = allocInfo->mFreeList;
					freeList.push(ptr);
				} else {
					AllocInfo* remoteAllocInfo = bitCast<AllocInfo*>(allocationsThreadId);
					RemoteFreeList& remoteFreeList = remoteAllocInfo->mRemoteFreeList;
					remoteFreeList.push(bitCast<typename RemoteFreeList::Node*>(ptr));
				}

				if (getThreadLocalStdRng().oneIn(16) && (allocInfo->mFreeList.size() > 64 && allocInfo->mRemoteFreeList.size() > 64)) {
					allocInfo->emptyFreeLists();
				}

				return true;
			}

			alloc_composer& getThreadLocalAlloc() noexcept {
				return mThreadAlloc.mAllocInfo->mAlloc;
			}
		};

		template<typename AllocRegionManager>
		using alloc = Alloc<AllocRegionManager>;
	};

	template<template<typename> typename AllocComposer, typename AllocRegionManager, Bool vDeallocWithRegionManager = true>
		requires(IsAllocComposerC<AllocComposer>&& IsAllocRegionManagerC<AllocRegionManager>)
	struct BindAllocComposer {
	public:
		using alloc_result = AllocResult<Byte>;
		using alloc_composer = AllocComposer<AllocRegionManager>;
		using alloc_result = ByteAllocResult;
		using region_manager = AllocRegionManager;

	private:
		alloc_composer mAllocComposer;

	public:
		Bool isInit() noexcept {
			return mAllocComposer.isInit();
		}
		Bool init() noexcept {
			return mAllocComposer.init();
		}
		Bool deinit() noexcept {
			return mAllocComposer.deinit();
		}

		alloc_result allocate(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
			return mAllocComposer.allocate(alignment, size, flags);
		}

		Bool deallocate(Byte* ptr) noexcept {
			if (!ptr) {
				return false;
			}

			Byte* bytePtr = reinterpret_cast<Byte*>(ptr);
			if (vDeallocWithRegionManager) {
				BaseAllocRoot* allocRoot = region_manager::findAlloc(alignDown(ptr, region_manager::regionSize));
				if (allocRoot == nullptr) {
					return false;
				}

				return allocRoot->deallocate(bytePtr);
			} else {
				return mAllocComposer.deallocate(bytePtr);
			}
		}

		alloc_composer& getAlloc() noexcept {
			return mAllocComposer;
		}
	};

	template <typename BindedAllocComposer>
		requires(IsBindedAllocComposerC<BindedAllocComposer>)
	struct GlobalAllocator : public AllocatorTraits<GlobalAllocator<BindedAllocComposer>, Byte>  {
		using alloc_result = ByteAllocResult;
		using pointer = Byte*;
		using binded_alloc_composer = BindedAllocComposer;

		struct AllocStorage {
			binded_alloc_composer alloc;
			
			AllocStorage() noexcept {
				alloc.init();
			}
			~AllocStorage() noexcept {
				alloc.deinit();
			}
		};

		inline static AllocStorage allocStorage;

		[[nodiscard]] static alloc_result allocate(const Size size, const AllocateFlags flags) noexcept {
			return allocStorage.alloc.allocate(defaultAllocateAlignment, size, flags);
		}
		[[nodiscard]] static alloc_result allocateAligned(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
			return allocStorage.alloc.allocate(alignment, size, flags);
		}
		static Bool deallocate(pointer ptr, [[maybe_unused]] const Size number = 0) noexcept {
			return allocStorage.alloc.deallocate(ptr);
		}
		static binded_alloc_composer& getGlobalAlloc() noexcept {
			return allocStorage.alloc;
		}
	};

	template<typename ByteAlloc>
		requires(IsAllocatorC<ByteAlloc>)
	struct MakeAllocator {
	public:

		template<typename Type>
		struct Alloc : public AllocatorTraits<Alloc<Type>, Type> {
		public:
			constexpr static inline Bool isMeshable = ByteAlloc::isMeshable;
			using alloc_result = AllocResult<Type>;
			using pointer = Type*;

		public:
			[[nodiscard]] constexpr static alloc_result allocate(const Size size, const AllocateFlags flags) noexcept {
				alloc_result result;
				result.mSize = 0;
				result.mPtr = nullptr;

				if (isConstantEvaluated()) {
					result.mPtr = constexprAllocate<Type>(size);
					result.mSize = size;
				} else {
					ByteAllocResult rawBytes = ByteAlloc::allocateAligned(alignof(Type), size * sizeof(Type), flags);
					if (rawBytes.mPtr == nullptr) {
						return result;
					}

					result.mPtr = reinterpret_cast<Type*>(rawBytes.mPtr);
					result.mSize = rawBytes.mSize / sizeof(Type);
				}

				return result;
			}
			[[nodiscard]] constexpr static alloc_result allocateAligned(const Size alignment, const Size size, const AllocateFlags flags) noexcept {
				alloc_result result;
				result.mSize = 0;
				result.mPtr = nullptr;

				if (isConstantEvaluated()) {
					result.mPtr = constexprAllocate<Type>(size);
					result.mSize = size;
				} else {
					if (!isPowerOf2(alignment)) {
						return result;
					}

					ByteAllocResult rawBytes = ByteAlloc::allocateAligned(max<Size>(alignof(Type), alignment), size * sizeof(Type), flags);
					if (rawBytes.mPtr == nullptr) {
						return result;
					}

					result.mPtr = reinterpret_cast<Type*>(rawBytes.mPtr);
					result.mSize = rawBytes.mSize / sizeof(Type);
				}
				return result;
			}

			constexpr static Bool deallocate(pointer ptr, [[maybe_unused]] const Size number = 0) noexcept {
				if (!ptr) {
					return false;
				}

				if (isConstantEvaluated()) {
					constexprDeallocate<Type>(ptr, number);
					return true;
				} else {
					Byte* bytePtr = reinterpret_cast<Byte*>(ptr);
					return ByteAlloc::deallocate(bytePtr);
				}
			}

			static Bool meshMoveProtect(Type* donor, Size donorSize) noexcept requires(isMeshable) {
				return ByteAlloc::meshMoveProtect(reinterpret_cast<Byte*>(donor), donorSize);
			}
			static Bool meshMoveUnprotect(Type* donor, Size donorSize) noexcept requires(isMeshable) {
				return ByteAlloc::meshMoveUnprotect(reinterpret_cast<Byte*>(donor), donorSize);
			}
			static Bool canMesh(Type* target, Size targetSize, Type* donor, Size donorSize) noexcept requires(isMeshable) {
				return ByteAlloc::canMesh(reinterpret_cast<Byte*>(target), targetSize, reinterpret_cast<Byte*>(donor), donorSize);
			}
			static void mesh(Type* target, Size targetSize, Type* donor, Size donorSize) noexcept requires(isMeshable) {
				ByteAlloc::mesh(reinterpret_cast<Byte*>(target), targetSize, reinterpret_cast<Byte*>(donor), donorSize);
			}
		};

		template<typename Type>
		using alloc = Alloc<Type>;

		using byte_alloc = alloc<Byte>;
	};

	template<typename RegionManager>
		requires(IsAllocRegionManagerC<RegionManager>)
	struct GlobalOsAlloc : public GlobalAllocator<BindAllocComposer<OsAllocRoot::alloc, RegionManager, false>> {
		constexpr static inline Bool isMeshable = true;
		

		static Bool meshMoveProtect(Byte* donor, Size donorSize) {
			return osMemoryRemapProtect(donor, donorSize);
		}
		static Bool meshMoveUnprotect(Byte* donor, Size donorSize) {
			return osMemoryRemapUnprotect(donor, donorSize);
		}
		static Bool canMesh(Byte* target, Size targetSize, Byte* donor, Size donorSize) noexcept {
			return bitCast<Size>(target) % natl::pageSize == 0 && bitCast<Size>(donor) % natl::pageSize == 0 && targetSize == donorSize;
		}

		static void mesh(Byte* target, Size targetSize, Byte* donor, Size) noexcept {
			osMemoryRemap(target, donor, targetSize);
		}
	};

	struct NullRegionManager : InlineAllocRegionManager<natl::pageSize, false, false, 0, BaseAllocRoot> {};

	template<typename RegionManager = NullRegionManager>
		requires(IsAllocRegionManagerC<RegionManager>)
	using OsAlloctor = MakeAllocator<GlobalOsAlloc<RegionManager>>::byte_alloc;
}
