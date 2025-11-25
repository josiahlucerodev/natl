//@interface
#include "nullptrUtils.h"

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
#include <unistd.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
//@end_non_modules

//own
#include "../util/executionSession.h"
#include "../sync/once.h"

namespace natl {
	namespace impl {
#ifdef NATL_WINDOWS_PLATFORM
		LONG WINAPI nullptrUnhandledExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
			if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
				constexpr ConstAsciiStringView errorMessage = "natl: null pointer dereference";
				if (hasExecutionSessions()) {
					raiseExecutionSessionError(1, errorMessage);
				} 
				fatalError(errorMessage);
			}
			return EXCEPTION_CONTINUE_SEARCH;
		}

		void enableSessionNullptrHandlingOnce() noexcept {
			AddVectoredExceptionHandler(0, nullptrUnhandledExceptionHandler);
		}
#endif

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#endif
		OnceFlag addSessionNullptrHandlingOnceFlag;
		void enableSessionNullptrHandlingImpl() noexcept {
			callOnce(addSessionNullptrHandlingOnceFlag, enableSessionNullptrHandlingOnce);
		}
	}
}