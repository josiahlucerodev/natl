//@interface
#include "memory.h"

//@begin_non_modules
//own
#include "../util/compilerDependent.h"

//system
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#pragma comment(lib, "mincore")
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <unistd.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM

//std
#include <memory>
//@end_non_modules

//own
#include "bits.h"
#include "assert.h"
#include "allocator.h"
#include "../container/flatHashMap.h"
#include "../container/persistentHashMap.h"
#include "../util/pair.h"
#include "../system/timer.h"
#include "../sync/mutex.h"

//own
namespace natl {
#ifdef NATL_WINDOWS_PLATFORM
	Bool osIsValidPageSize(Size pageSize) noexcept {
		return pageSize == 4096;
	}

	struct RemappableMemoryInfo {
		Size mSize;
		Byte* mSection;
	};

	struct RemapMemoryProtectInfo {
		Size mSize;
		volatile LONG mProtectActive = 0;
		HANDLE mProtectCompleteEvent;
		Timer mPersistTimer;

		void free() noexcept {
			CloseHandle(mProtectCompleteEvent);
			mProtectCompleteEvent = nullptr;
		}
	};
	constexpr natl::Milliseconds<natl::Size> protectInfoPersistTime = natl::Milliseconds<natl::Size>(1000);
	constexpr natl::Milliseconds<natl::Size> protectInfoWaitTime = natl::Milliseconds<natl::Size>(200);

	struct OsMemoryGlobal {
		FlatHashMap<Byte*, RemappableMemoryInfo> mRemappableMemoryInfoMap;
		FlatHashMap<Byte*, Size> mRemappableSectionRefCountMap;
		Mutex mRemappableMutex;
		HANDLE mRemapCompleteEvent;
		volatile LONG mRemapActive = 0;

		Mutex mRemapProtectMutex;
		PersistentHashMap<Byte*, RemapMemoryProtectInfo> mRemapProtects;

		~OsMemoryGlobal() {
			LockGuard lock(mRemappableMutex);
			for (auto iter = mRemapProtects.begin(); iter != mRemapProtects.end(); ++iter) {
				auto [base, protectInfo] = *iter;
				protectInfo.free();
			}
			mRemapProtects.clear();
		}
	};

	LONG CALLBACK remapVehHandler(EXCEPTION_POINTERS* ep) noexcept;

	OsMemoryGlobal& getOsMemoryGlobal() noexcept {
		static OsMemoryGlobal memoryGlobal;
		static Bool initialized = [] {
			memoryGlobal.mRemapCompleteEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
			AddVectoredExceptionHandler(1, remapVehHandler);
			return true;
		}();
		return memoryGlobal;
	}
	
	ByteAllocResult osMemoryAllocate(Size alignment, Size size, AllocateFlags flags, Size pageSize) noexcept {
		ByteAllocResult  result;
		result.mPtr = nullptr;
		result.mSize = 0;

		if (!osIsValidPageSize(pageSize)) {
			return result;
		}

		size = roundSizeToPage(size, pageSize);

		SYSTEM_INFO si;
		GetSystemInfo(&si);
		alignment = max<Size>(pow2(bitscanForward(alignment)), si.dwAllocationGranularity);

		if ((flags & AllocateFlags::remappable) != AllocateFlags::none) {
			PVOID section = CreateFileMapping(
				INVALID_HANDLE_VALUE,
				nullptr,
				PAGE_READWRITE,
				0,
				size, nullptr
			);

			if (section == nullptr) {
				return result;
			}

			MEM_ADDRESS_REQUIREMENTS addrReq = { 0 };
			addrReq.Alignment = alignment;

			MEM_EXTENDED_PARAMETER param = { 0 };
			param.Type = MemExtendedParameterAddressRequirements;
			param.Pointer = &addrReq;

			PVOID placeHolder = VirtualAlloc2(
				nullptr,
				nullptr,
				size,
				MEM_RESERVE | MEM_RESERVE_PLACEHOLDER,
				PAGE_NOACCESS,
				&param,
				1
			);

			if (placeHolder == nullptr) {
				CloseHandle(section);
				return result;
			}

			PVOID view = MapViewOfFile3(
				section,
				GetCurrentProcess(),
				placeHolder,
				0,
				size,
				MEM_REPLACE_PLACEHOLDER,
				PAGE_READWRITE,
				NULL,
				0
			);

			if (view == nullptr || view != placeHolder) {
				VirtualFree(placeHolder, 0, MEM_RELEASE);
				CloseHandle(section);
				return result;
			}

			result.mPtr = reinterpret_cast<Byte*>(view);
			result.mSize = size;

			RemappableMemoryInfo info;
			info.mSection = reinterpret_cast<Byte*>(section);
			info.mSize = size;

			{
				LockGuard lock(getOsMemoryGlobal().mRemappableMutex);
				getOsMemoryGlobal().mRemappableMemoryInfoMap.insert(result.mPtr, info);
				getOsMemoryGlobal().mRemappableSectionRefCountMap.insert(info.mSection, 1);
			}

			return result;
		} else {
			MEM_ADDRESS_REQUIREMENTS addrReq = { 0 };
			addrReq.Alignment = alignment;

			MEM_EXTENDED_PARAMETER param = { 0 };
			param.Type = MemExtendedParameterAddressRequirements;
			param.Pointer = &addrReq;

			void* ptr = VirtualAlloc2(
				GetCurrentProcess(),
				nullptr,
				size,
				MEM_RESERVE | MEM_COMMIT,
				PAGE_READWRITE,
				&param,
				1
			);

			if (ptr == nullptr) {
				return result;
			}

			result.mSize = size;
			result.mPtr = reinterpret_cast<Byte*>(ptr);
			return result;
		}
	}

	Bool osMemoryDeallocate(Byte* ptr) noexcept {
		{
			LockGuard lock(getOsMemoryGlobal().mRemappableMutex);
			auto memoryInfoIter = getOsMemoryGlobal().mRemappableMemoryInfoMap.findIter(ptr);
			if (memoryInfoIter != getOsMemoryGlobal().mRemappableMemoryInfoMap.end()) {
				Size refCount;
				LPVOID section;
				{
					RemappableMemoryInfo memoryInfo = memoryInfoIter.keyValue().value();
					section = memoryInfo.mSection;

					auto sectionRefCountIter = getOsMemoryGlobal().mRemappableSectionRefCountMap.findIter(memoryInfo.mSection);
					if (sectionRefCountIter == getOsMemoryGlobal().mRemappableSectionRefCountMap.end()) {
						return false;
					}

					refCount = sectionRefCountIter.keyValue().value() -= 1;
					getOsMemoryGlobal().mRemappableMemoryInfoMap.erase(memoryInfoIter);

					if (refCount == 0) {
						getOsMemoryGlobal().mRemappableSectionRefCountMap.erase(sectionRefCountIter);
					}
				}

				UnmapViewOfFileEx(ptr, 0);
				VirtualFree(ptr, 0, MEM_RELEASE);

				if (refCount == 0) {
					CloseHandle(section);
				}

				return true;
			}
		}

		return VirtualFree(ptr, 0, MEM_RELEASE);
	}

	LONG CALLBACK remapVehHandler(EXCEPTION_POINTERS* ep) noexcept {
		if (ep->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION) {
			return EXCEPTION_CONTINUE_SEARCH;
		}

		void* faultAddress = (void*)ep->ExceptionRecord->ExceptionInformation[1];
		Byte* faultAddressByte = reinterpret_cast<Byte*>(faultAddress);

		LockGuard lock(getOsMemoryGlobal().mRemapProtectMutex);
		RemapMemoryProtectInfo* protectInfo = nullptr;

		for (auto [testBase, testProtectInfo] : getOsMemoryGlobal().mRemapProtects) {
			if (faultAddress >= testBase && faultAddress < testBase + testProtectInfo.mSize) {
				protectInfo = &testProtectInfo;
				break;
			}
		}

		if (protectInfo == nullptr) {
			return EXCEPTION_CONTINUE_SEARCH;
		}

		while (InterlockedCompareExchange(&protectInfo->mProtectActive, 0, 0)) {
			WaitForSingleObject(protectInfo->mProtectCompleteEvent, protectInfoWaitTime.value());
		}

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	Bool osMemoryRemapProtect(Byte* donorPtr, Size size) noexcept {
		DWORD donorOldProtect;
		if (!VirtualProtect(donorPtr, size, PAGE_READONLY, &donorOldProtect)) {
			return false;
		}
		
		LockGuard lock(getOsMemoryGlobal().mRemapProtectMutex);
		
		RemapMemoryProtectInfo protectInfo;
		protectInfo.mSize = size;
		protectInfo.mProtectActive = 1;
		protectInfo.mProtectCompleteEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

		if (getOsMemoryGlobal().mRemapProtects.insert(donorPtr, protectInfo).insertFailed()) {
			DWORD unneededOldProtect;
			return VirtualProtect(donorPtr, size, donorOldProtect, &unneededOldProtect);
		}

		return true;
	}

	Bool osMemoryRemapUnprotect(Byte* donorPtr, Size size) noexcept {
		RemapMemoryProtectInfo* donorProtectInfo = nullptr;
		{
			LockGuard lock(getOsMemoryGlobal().mRemapProtectMutex);
			SmallDynArray<decltype(getOsMemoryGlobal().mRemapProtects)::iterator, 128> freeProtects;

			for (auto iter = getOsMemoryGlobal().mRemapProtects.begin(); iter != getOsMemoryGlobal().mRemapProtects.end(); ++iter) {
				auto [base, protectInfo] = *iter;
				if (protectInfo.mProtectActive == 1) {
					continue;
				}
				if (protectInfo.mPersistTimer.getMillisecondsInt().asInt<natl::Size>() < protectInfoPersistTime) {
					continue;
				}

				protectInfo.free();
				freeProtects.pushBack(iter);
			}


			for (auto iter : freeProtects) {
				getOsMemoryGlobal().mRemapProtects.erase(iter);
			}

			auto findOption = getOsMemoryGlobal().mRemapProtects.find(donorPtr);
			if (!findOption.hasValue()) {
				return false;
			}
			donorProtectInfo = findOption.value().valueAsPtr();
		}

		if (!donorProtectInfo) {
			return false;
		}

		donorProtectInfo->mPersistTimer.start();
		InterlockedExchange(&donorProtectInfo->mProtectActive, 0);
		SetEvent(donorProtectInfo->mProtectCompleteEvent);

		DWORD donorOldProtect;
		return VirtualProtect(donorPtr, size, PAGE_READWRITE, &donorOldProtect);
	}

	Bool osMemoryRemap(Byte* targetPtr, Byte* donorPtr, Size size) noexcept {
		LockGuard lock(getOsMemoryGlobal().mRemappableMutex);

		auto targetMemoryInfoIter = getOsMemoryGlobal().mRemappableMemoryInfoMap.findIter(targetPtr);
		if (targetMemoryInfoIter == getOsMemoryGlobal().mRemappableMemoryInfoMap.end()) {
			return false;
		}
		auto donorMemoryInfoIter = getOsMemoryGlobal().mRemappableMemoryInfoMap.findIter(donorPtr);
		if (donorMemoryInfoIter == getOsMemoryGlobal().mRemappableMemoryInfoMap.end()) {
			return false;
		}

		RemappableMemoryInfo& targetMemoryInfo = targetMemoryInfoIter.keyValue().value();
		RemappableMemoryInfo& donorMemoryInfo = donorMemoryInfoIter.keyValue().value();
		if (targetMemoryInfo.mSize != size && donorMemoryInfo.mSize != size) {
			return false;
		}
		if (targetMemoryInfo.mSection == donorMemoryInfo.mSection) {
			return false;
		}

		auto targetSectionRefCountIter = getOsMemoryGlobal().mRemappableSectionRefCountMap.findIter(targetMemoryInfo.mSection);
		if (targetSectionRefCountIter == getOsMemoryGlobal().mRemappableSectionRefCountMap.end()) {
			return false;
		}
		auto donorSectionRefCountIter = getOsMemoryGlobal().mRemappableSectionRefCountMap.findIter(donorMemoryInfo.mSection);
		if (donorSectionRefCountIter == getOsMemoryGlobal().mRemappableSectionRefCountMap.end()) {
			return false;
		}

		DWORD donorOldProtect;
		if (!VirtualProtect(donorPtr, size, PAGE_NOACCESS, &donorOldProtect)) {
			return false;
		}

		if (!UnmapViewOfFileEx(donorPtr, MEM_PRESERVE_PLACEHOLDER)) {
			cassert();
		}
		
		PVOID newDonorView = MapViewOfFile3(
			targetMemoryInfo.mSection,
			GetCurrentProcess(),
			donorPtr,
			0,
			size,
			MEM_REPLACE_PLACEHOLDER,
			PAGE_READWRITE,
			NULL,
			0
		);

		if (newDonorView != donorPtr) {
			cassert();
		}

		VirtualProtect(donorPtr, size, PAGE_READONLY, &donorOldProtect);

		Size donorRefCount = donorSectionRefCountIter.keyValue().value() -= 1;
		if (donorRefCount == 0) {
			CloseHandle(donorMemoryInfo.mSection);
			getOsMemoryGlobal().mRemappableSectionRefCountMap.erase(donorSectionRefCountIter);
		}

		targetSectionRefCountIter.keyValue().value() += 1;
		donorMemoryInfo.mSection = targetMemoryInfo.mSection;

		return true;
	}

#elif defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	Byte* stdAlignedAllocate(Size alignment, Size size) noexcept {
		return reinterpret_cast<Byte*>(std::aligned_alloc(size, alignment));
	}
	void stdAlignedDeallocate(Byte* ptr) noexcept {
		std::free(ptr);
	}
#else // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
	static_assert(false, "natl: memory not implemented for platform");
#endif
}