#pragma once 

//own
#include "compilerDependent.h"
#include "thread.h"
#include "option.h"
#include "array.h"

//interface
namespace natl {

#ifdef NATL_WINDOWS_PLATFORM
	using MutexHandle = void*;
#define NATL_NATIVE_INVALID_MUTEX_HANDEL_VALUE 0
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	using MutexHandle = natl::Option<Array<AlignedType<natl::Byte, 8>, 40>>;
#define NATL_NATIVE_INVALID_MUTEX_HANDEL_VALUE MutexHandle();
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM

	natl::Option<MutexHandle> createMutex() noexcept;
	Bool destroyMutex(MutexHandle mutex) noexcept;
	Bool lockMutex(MutexHandle mutex) noexcept;
	struct MutexTryLockResult {
		Bool successful;
		Bool locked;
	};
	MutexTryLockResult trylockMutex(MutexHandle mutex) noexcept;
	Bool unlockMutex(MutexHandle mutex) noexcept;


	class Mutex {
	public:
		using native_handle_type = MutexHandle;
	private:
		native_handle_type mutexHandle;
	public:
		//constructor 
		constexpr Mutex() noexcept {
			if (!isConstantEvaluated()) {
				natl::Option<MutexHandle> mutexHandleOption = createMutex();
				if (mutexHandleOption.doesNotHaveValue()) {
					natlTerminate();
				}
				mutexHandle = mutexHandleOption.value();
			}
		}
		constexpr Mutex(const Mutex&) noexcept = delete;

		//destructor 
		constexpr ~Mutex() noexcept {
			if (!isConstantEvaluated()) {
				destroyMutex(mutexHandle);
			}
		}

		//util 
		constexpr Mutex& self() noexcept { return *this; }
		constexpr const Mutex& self() const noexcept { return *this; }

		//locking
		constexpr void lock() noexcept {
			if (!isConstantEvaluated()) {
				if (lockMutex(mutexHandle) == false) {
					natlTerminate();
				}
			}
		}
		constexpr Bool tryLock() noexcept {
			if (!isConstantEvaluated()) {
				MutexTryLockResult tryLockResult = trylockMutex(mutexHandle);
				if (tryLockResult.successful == false) {
					natlTerminate();
				}

				return tryLockResult.locked;
			}
			return true;
		}
		constexpr void unlock() noexcept {
			if (!isConstantEvaluated()) {
				unlockMutex(mutexHandle);
			}
		}
		
		//observers 
		constexpr native_handle_type nativeHandle() noexcept { return mutexHandle; };
	};

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