//interface 
#include "mutex.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <pthread.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM

//implementation 
namespace natl {
#ifdef NATL_WINDOWS_PLATFORM
	natl::Option<MutexHandle> createMutex() noexcept {
		MutexHandle mutexHandle = CreateMutexA(NULL, FALSE, NULL);
		if (mutexHandle == NULL) {
			return OptionEmpty{};
		}
		return mutexHandle;
	}

	Bool destroyMutex(MutexHandle mutex) noexcept {
		return CloseHandle(mutex);
	}

	Bool lockMutex(MutexHandle mutex) noexcept {
		DWORD result = WaitForSingleObject(mutex, INFINITE);
		if (result != WAIT_OBJECT_0) {
			return false;
		}
		return true;
	}

	MutexTryLockResult trylockMutex(MutexHandle mutex) noexcept {
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

	Bool unlockMutex(MutexHandle mutex) noexcept {
		return ReleaseMutex(mutex);
	}

#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	
	pthread_mutex_t* castMutexHandleToPthreadMutexPtr(MutexHandle& mutex) noexcept {
		return &punCast<pthread_mutex_t>(*mutex.value().data());
	}

	natl::Option<MutexHandle> createMutex() noexcept {
		MutexHandle mutex{};
		using MutexHandleValueType = typename MutexHandle::value_type;
		mutex = MutexHandleValueType();

		if (pthread_mutex_init(castMutexHandleToPthreadMutexPtr(mutex), nullptr) != 0) {
			return OptionEmpty{};
		}

		return mutex;
	}
	Bool destroyMutex(MutexHandle mutex) noexcept {
		if (mutex.doesNotHaveValue()) {
			return false;
		}
		return pthread_mutex_destroy(castMutexHandleToPthreadMutexPtr(mutex)) == 0;
	}
	Bool lockMutex(MutexHandle mutex) noexcept {
		if (mutex.doesNotHaveValue()) {
			return false;
		}
		return pthread_mutex_lock(castMutexHandleToPthreadMutexPtr(mutex)) == 0;
	}
	MutexTryLockResult trylockMutex(MutexHandle mutex) noexcept {
		MutexTryLockResult tryLockResult{};
		if (mutex.doesNotHaveValue()) {
			tryLockResult.successful = false;
			return tryLockResult;
		}

		tryLockResult.successful = true;
		tryLockResult.locked = pthread_mutex_trylock(castMutexHandleToPthreadMutexPtr(mutex)) == 0;
		return tryLockResult;
	}
	Bool unlockMutex(MutexHandle mutex) noexcept {
		if (mutex.doesNotHaveValue()) {
			return false;
		}
		return pthread_mutex_unlock(castMutexHandleToPthreadMutexPtr(mutex));;
	}
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
}