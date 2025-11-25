//@interface
#include "timer.h"

//@begin_non_modules
//own
#include "../util/compilerDependent.h"

//system 
#ifdef NATL_WINDOWS_PLATFORM
#define NOMINMAX
#include <Windows.h>
#endif // NATL_WINDOWS_PLATFORM 

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
#include <unistd.h>
#endif // NATL_UNIX_PLATFORM || NATL_WEB_PLATFORM
//@end_non_modules

//own
#include "../util/random.h"

namespace natl {
#ifdef NATL_WINDOWS_PLATFORM
	HighResolutionTimePoint highResolutionNow() noexcept {
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return HighResolutionTimePoint(counter.QuadPart);
	}

	i64 natlWindowPlatformQueryHighResolutionFrequency() noexcept {
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}

	Nanoseconds<i64> highResolutionTimePointToNanoseconds(const HighResolutionTimePoint timePoint) noexcept {
		static const i64 highResolutionfrequency = natlWindowPlatformQueryHighResolutionFrequency();
		const i64 nanoseconds = timePoint.value() * (1000000000 / highResolutionfrequency);
		return Nanoseconds<i64>(nanoseconds);
	}
#endif

#if defined(NATL_UNIX_PLATFORM) || defined(NATL_WEB_PLATFORM)
	HighResolutionTimePoint highResolutionNow() noexcept {
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
		return HighResolutionTimePoint(ts.tv_nsec);
	}
	Nanoseconds<i64> highResolutionTimePointToNanoseconds(const HighResolutionTimePoint timePoint) noexcept {
		return Nanoseconds<i64>(timePoint.value());
	}
#endif

	Size rand() noexcept {
		static StandardRNG rng = StandardRNG(highResolutionNow().value());
		return rng.random();
	}
}