#pragma once 

//own
#include "../container/array.h"
#include "../fundamental/option.h"
#include "../util/compilerDependent.h"
#include "thread.h"

//interface
namespace natl {

#ifdef NATL_WINDOWS_PLATFORM
	using NativeMutex = void*;
#define NATL_NATIVE_INVALID_MUTEX_HANDEL_VALUE 0
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	using NativeMutex = natl::Option<Array<AlignedType<natl::Byte, 8>, 40>>;
#define NATL_NATIVE_INVALID_MUTEX_HANDEL_VALUE NativeMutex();
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM

	Bool createMutex(NativeMutex& mutexDst) noexcept;
	Bool destroyMutex(NativeMutex& mutex) noexcept;
	Bool lockMutex(NativeMutex& mutex) noexcept;
	struct MutexTryLockResult {
		Bool successful;
		Bool locked;
	};
	MutexTryLockResult trylockMutex(NativeMutex& mutex) noexcept;
	Bool unlockMutex(NativeMutex& mutex) noexcept;


	class Mutex {
	public:
		using native_type = NativeMutex;
	private:
		native_type mutex;
	public:
		//constructor 
		constexpr Mutex() noexcept {
			if (!isConstantEvaluated()) {
				const Bool mutexCreated = createMutex(mutex);
				if (mutexCreated != true) {
					natl::terminate();
				}
			}
		}
		constexpr Mutex(const Mutex&) noexcept = delete;

		//destructor 
		constexpr ~Mutex() noexcept {
			if (!isConstantEvaluated()) {
				destroyMutex(mutex);
			}
		}

		//util 
		constexpr Mutex& self() noexcept { return *this; }
		constexpr const Mutex& self() const noexcept { return *this; }

		//locking
		constexpr void lock() noexcept {
			if (!isConstantEvaluated()) {
				if (lockMutex(mutex) == false) {
					natl::terminate();
				}
			}
		}
		constexpr Bool tryLock() noexcept {
			if (!isConstantEvaluated()) {
				MutexTryLockResult tryLockResult = trylockMutex(mutex);
				if (tryLockResult.successful == false) {
					natl::terminate();
				}

				return tryLockResult.locked;
			}
			return true;
		}
		constexpr void unlock() noexcept {
			if (!isConstantEvaluated()) {
				unlockMutex(mutex);
			}
		}
		
		//observers 
		constexpr native_type& native() noexcept { return mutex; };
	};

	class RecursiveMutex {
	public:
		using native_type = Mutex::native_type;
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
					natl::terminate();
				}
			}
		}

		//observers 
		constexpr native_type& native() noexcept { return mutex.native(); };
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