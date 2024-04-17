#pragma once 

//own
#include "compilerDependent.h"
#include "thread.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <pthread.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM

//interface
namespace natl {
#ifdef NATL_WINDOWS_PLATFORM
	class Mutex {
	public:
		using native_handle_type = HANDLE;
	private:
		native_handle_type mutexHandle;
	public:
		//constructor 
		constexpr Mutex() noexcept {
			if (!isConstantEvaluated()) {
				mutexHandle = CreateMutexA(NULL, FALSE, NULL);
				if (mutexHandle == NULL) {
					natlTerminate();
				}
			}
		}
		constexpr Mutex(const Mutex&) noexcept = delete;

		//destructor 
		constexpr ~Mutex() noexcept {
			if (!isConstantEvaluated()) {
				CloseHandle(mutexHandle);
			}
		}

		//util 
		constexpr Mutex& self() noexcept { return *this; }
		constexpr const Mutex& self() const noexcept { return *this; }

		//locking
		constexpr void lock() noexcept {
			if (!isConstantEvaluated()) {
				DWORD result = WaitForSingleObject(mutexHandle, INFINITE);
				if (result != WAIT_OBJECT_0) {
					natlTerminate();
				}
			}
		}
		constexpr Bool tryLock() noexcept {
			if (!isConstantEvaluated()) {
				DWORD result = WaitForSingleObject(mutexHandle, 0);
				if (result == WAIT_OBJECT_0) {
					return true;
				} else if (result == WAIT_TIMEOUT) {
					return false;
				} else {
					natlTerminate();
				}
			}
			return true;
		}
		constexpr void unlock() noexcept {
			if (!isConstantEvaluated()) {
				ReleaseMutex(mutexHandle);
			}
		}
		
		//observers 
		constexpr native_handle_type nativeHandle() noexcept { return mutexHandle; };
	};
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	class Mutex {
	public:
		using native_handle_type = pthread_mutex_t;
	private:
		native_handle_type mutexHandle;
	public:
		//constructor 
		constexpr Mutex() noexcept {
			if (!isConstantEvaluated()) {
				if (pthread_mutex_init(&mutexHandle, nullptr) != 0) {
					natlTerminate();
				}
			}
		}
		constexpr Mutex(const Mutex&) noexcept = delete;

		//destructor 
		constexpr ~Mutex() noexcept {
			if (!isConstantEvaluated()) {
				pthread_mutex_destroy(&mutexHandle);
			}
		}

		//util 
		constexpr Mutex& self() noexcept { return *this; }
		constexpr const Mutex& self() const noexcept { return *this; }

		//locking
		constexpr void lock() noexcept {
			if (!isConstantEvaluated()) {
				if (pthread_mutex_lock(&mutexHandle) != 0) {
					natlTerminate();
				}
			}
		}
		constexpr Bool tryLock() noexcept {
			if (!isConstantEvaluated()) {
				return pthread_mutex_trylock(&mutexHandle) == 0;
			}
			return true;
		}
		constexpr void unlock() noexcept {
			if (!isConstantEvaluated()) {
				pthread_mutex_unlock(&mutexHandle);
			}
		}

		//observers 
		constexpr native_handle_type nativeHandle() noexcept { return mutexHandle; };
	};
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM

	class RecursiveMutex {
	public:
		using native_handle_type = Mutex::native_handle_type;
	private:
		Mutex mutex;
		ThreadId currentLockingThread;
		Size lockLevel;
	public:
		//constructor 
		constexpr RecursiveMutex() noexcept : mutex(), currentLockingThread(), lockLevel(0) {}
		constexpr RecursiveMutex(const RecursiveMutex&) noexcept = delete;

		//destructor 
		constexpr ~RecursiveMutex() noexcept = default;

		//util 
		constexpr RecursiveMutex& self() noexcept { return *this; }
		constexpr const RecursiveMutex& self() const noexcept { return *this; }

		//locking
		constexpr void lock() noexcept {
			if (!isConstantEvaluated()) {
				const ThreadId curretThreadId = getCurrentThreadId();
				if (curretThreadId == currentLockingThread) {
					lockLevel += 1;
					return;
				}

				mutex.lock();
				currentLockingThread = curretThreadId;
				lockLevel = 1;
			}
		}
		constexpr Bool tryLock() noexcept {
			if (!isConstantEvaluated()) {
				const ThreadId curretThreadId = getCurrentThreadId();
				if (curretThreadId == currentLockingThread) {
					lockLevel += 1;
					return true;
				}

				Bool lockResult = mutex.tryLock();
				if (lockResult == true) {
					currentLockingThread = curretThreadId;
					lockLevel = 1;
				}
				return lockResult;
			} else {
				return true;
			}
		}
		constexpr void unlock() noexcept {
			if (!isConstantEvaluated()) {
				const ThreadId curretThreadId = getCurrentThreadId();
				if (curretThreadId == currentLockingThread) {
					if (lockLevel == 0) {
						mutex.unlock();
					} else {
						lockLevel -= 1;
					}
				} else {
					natlTerminate();
				}
			}
		}

		//observers 
		constexpr native_handle_type nativeHandle() noexcept { return mutex.nativeHandle(); };
	};

	struct AdoptLockFlag {};

	template<class MutexType>
	class LockGuard {
	public:
		using mutex_type = MutexType;
	private:
		mutex_type& mutexRef;
	public:
		//constructor 
		constexpr LockGuard() noexcept = delete;
		constexpr LockGuard(const LockGuard&) noexcept = delete;
		constexpr LockGuard(mutex_type& mutexRefIn) noexcept : mutexRef(mutexRefIn) {
			mutexRef.lock();
		}
		constexpr LockGuard(mutex_type& mutexRefIn, AdoptLockFlag) noexcept : mutexRef(mutexRefIn) {}

		//destructor  
		constexpr ~LockGuard() noexcept {
			mutexRef.unlock();
		}

		//util 
		constexpr LockGuard& self() noexcept { *this; }
		constexpr const LockGuard& self() const noexcept { *this; }
	};
}