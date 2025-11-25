//@interface 
#include "mutex.h"

//@begin_non_modules
//own
#include "../util/compilerDependent.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <pthread.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
//@end_non_modules

namespace natl {
#ifdef NATL_WINDOWS_PLATFORM
	Bool createMutex(NativeMutex& mutexDst) noexcept {
		NativeMutex mutexHandle = CreateMutexA(NULL, FALSE, NULL);
		if (mutexHandle == NULL) {
			return false;
		}
		mutexDst = mutexHandle;
		return true;
	}

	Bool destroyMutex(NativeMutex& mutex) noexcept {
		return CloseHandle(mutex);
	}

	Bool lockMutex(NativeMutex& mutex) noexcept {
		DWORD result = WaitForSingleObject(mutex, INFINITE);
		if (result != WAIT_OBJECT_0) {
			return false;
		}
		return true;
	}

	MutexTryLockResult trylockMutex(NativeMutex& mutex) noexcept {
		MutexTryLockResult tryLockResult{};
		tryLockResult.successful = true;
		tryLockResult.locked = false;

		DWORD result = WaitForSingleObject(mutex, 0);
		if (result == WAIT_OBJECT_0) {
			tryLockResult.locked = true;
		} else if (result == WAIT_TIMEOUT) {
			tryLockResult.locked = false;
		} else {
			tryLockResult.successful = false;
		}

		return tryLockResult;
	}

	Bool unlockMutex(NativeMutex& mutex) noexcept {
		return ReleaseMutex(mutex);
	}

#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	
	pthread_mutex_t* castNativeMutexToPthreadMutexPtr(NativeMutex& mutex) noexcept {
		return &punCast<pthread_mutex_t>(*mutex.value().data());
	}

	Bool createMutex(NativeMutex& mutexDst) noexcept {
		using native_mutex_value_type = NativeMutex::value_type;
		mutexDst = native_mutex_value_type{};
		return pthread_mutex_init(castNativeMutexToPthreadMutexPtr(mutexDst), nullptr) == 0;
	}
	Bool destroyMutex(NativeMutex& mutex) noexcept {
		if (mutex.doesNotHaveValue()) {
			return false;
		}
		return pthread_mutex_destroy(castNativeMutexToPthreadMutexPtr(mutex)) == 0;
	}
	Bool lockMutex(NativeMutex& mutex) noexcept {
		if (mutex.doesNotHaveValue()) {
			return false;
		}
		return pthread_mutex_lock(castNativeMutexToPthreadMutexPtr(mutex)) == 0;
	}
	MutexTryLockResult trylockMutex(NativeMutex& mutex) noexcept {
		MutexTryLockResult tryLockResult{};
		if (mutex.doesNotHaveValue()) {
			tryLockResult.successful = false;
			return tryLockResult;
		}

		tryLockResult.successful = true;
		tryLockResult.locked = pthread_mutex_trylock(castNativeMutexToPthreadMutexPtr(mutex)) == 0;
		return tryLockResult;
	}
	Bool unlockMutex(NativeMutex& mutex) noexcept {
		if (mutex.doesNotHaveValue()) {
			return false;
		}
		return pthread_mutex_unlock(castNativeMutexToPthreadMutexPtr(mutex));;
	}
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
}