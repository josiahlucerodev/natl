//@interface
#include "thread.h"

//@begin_non_modules
#include "../util/compilerDependent.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <pthread.h>
#include <unistd.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
//@end_non_modules

namespace natl {
#ifdef NATL_WINDOWS_PLATFORM

	namespace impl {
		ThreadId getCurrentThreadIdNativeImpl() noexcept {
			return ThreadId(static_cast<typename ThreadId::value_type>(GetCurrentThreadId()));
		}
	}

	Size getNumberOfLogicalProcessors() noexcept {
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		DWORD numLogicalProcessors = sysInfo.dwNumberOfProcessors;
		return static_cast<Size>(numLogicalProcessors);
	}

	ThreadCreateResult createThread(NativeThreadFunctionType threadFunction, void* paramater) noexcept {
		ThreadCreateResult threadCreateResult{};

		DWORD dwThreadId;
		HANDLE threadHandle = CreateThread(NULL, 0, threadFunction, reinterpret_cast<LPVOID>(paramater), 0, &dwThreadId);
		
		if (threadHandle == NULL) {
			threadCreateResult.successful = false;
			return threadCreateResult;
		}

		threadCreateResult.successful = true;
		threadCreateResult.threadHandel = threadHandle;
		threadCreateResult.threadId.setValue(static_cast<typename ThreadId::value_type>(dwThreadId));
		return threadCreateResult;
	}
	Bool joinThread(NativeThreadHandle thread) noexcept {
		DWORD waitResult = WaitForSingleObject(thread, INFINITE);
		if (waitResult == WAIT_FAILED) {
			return false;
		}
		CloseHandle(thread);
		return true;
	}

	void thisThreadSleep(const Milliseconds<i64> milliseconds) noexcept {
		Sleep(static_cast<DWORD>(milliseconds.value()));
	}
	void thisThreadYield() noexcept {
		SwitchToThread();
	}
#endif // NATL_WINDOWS_PLATFORM

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	namespace impl {
		ThreadId getCurrentThreadIdNativeImpl() noexcept {
			return ThreadId(static_cast<typename ThreadId::value_type>(pthread_self()));
		}
	}

	Size getNumberOfLogicalProcessors() noexcept {
		return static_cast<Size>(sysconf(_SC_NPROCESSORS_ONLN));
	}
	ThreadCreateResult createThread(NativeThreadFunctionType threadFunction, void* paramater) noexcept {
		ThreadCreateResult threadCreateResult{};
		
		pthread_t threadId;
		if (pthread_create(&threadId, nullptr, threadFunction, paramater) != 0) {
			threadCreateResult.successful = false;
			return threadCreateResult;
		}

		threadCreateResult.successful = true;
		threadCreateResult.threadHandel = threadId;
		threadCreateResult.threadId.setValue(static_cast<typename ThreadId::value_type>(threadId));
		return threadCreateResult;
	}
	Bool joinThread(NativeThreadHandle thread) noexcept {
		return pthread_join(static_cast<pthread_t>(thread), nullptr) == 0;
	}

	void thisThreadSleep(const Milliseconds<i64> milliseconds) noexcept {
		usleep(static_cast<useconds_t>(milliseconds.convertTo<natl::abbrt::mus>().value()));
	}
	void thisThreadYield() noexcept {
		sched_yield();
	}

#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
}