#pragma once

//compiler 
#if defined(__EMSCRIPTEN__)
#define NATL_COMPILER_EMSCRIPTEN
#elif defined(__GNUC__)
#define NATL_COMPILER_GCC
#elif defined(__clang__)
#define NATL_COMPILER_CLANG
#elif defined(_MSC_VER)
#define NATL_COMPILER_MSVC
#endif 



//force inline 
#ifdef NATL_COMPILER_EMSCRIPTEN 
#define NATL_FORCE_INLINE __attribute__((always_inline)) 
#endif //  NATL_COMPILER_EMSCRIPTEN

#ifdef NATL_COMPILER_CLANG
#define NATL_FORCE_INLINE [[clang::always_inline]] 
#endif // !NATL_FORCE_INLINE

#ifdef NATL_COMPILER_GCC
#define NATL_FORCE_INLINE [[gnu::always_inline]]
#endif //NATL_COMPILER_GCC

#ifdef NATL_COMPILER_MSVC
#define NATL_FORCE_INLINE __forceinline
#endif // NATL_COMPILER_MSVC

//in debug 
//NATL_IN_DEBUG
#ifdef _DEBUG
#define NATL_IN_DEBUG
#endif //  _DEBUG

#ifndef NATL_IN_DEBUG
#ifdef NDEBUG
#define NATL_IN_DEBUG
#endif // NDEBUG
#endif // !NATL_IN_DEBUG


#ifndef NATL_IN_DEBUG
#ifdef _LIBCPP___DEBUG
#define NATL_IN_DEBUG
#include <emscripten.h>
#endif // _LIBCPP___DEBUG
#endif // !NATL_IN_DEBUG

//bit 
#if defined(NATL_COMPILER_EMSCRIPTEN) ||  defined(NATL_COMPILER_CLANG) || defined(NATL_COMPILER_GCC)
#if defined(__LP64__) || defined(_LP64)
#define NATL_64BIT
#else
#define NATL_32BIT
#endif
#endif //  NATL_COMPILER_EMSCRIPTEN || NATL_COMPILER_CLANG || NATL_COMPILER_GCC


#if !(defined(NATL_64BIT) || defined(NATL_32BIT))
#ifdef NATL_COMPILER_MSVC
#if defined(_WIN64)
#define NATL_64BIT
#else
#define NATL_32BIT
#endif
#endif //NATL_COMPILER_MSVC
#endif // ! NATL_64BIT || NATL_32BIT

//platform
#ifdef __unix__
#define NATL_UNIX_PLATFORM
#elif defined(_WIN32) || defined(WIN32)
#define NATL_WINDOWS_PLATFORM
#elif defined(NATL_COMPILER_EMSCRIPTEN)
define NATL_WEB_PLATFORM
#else 
static_assert(false, "natl: platform type not supported");
#endif

//architecture
#ifdef NATL_COMPILER_MSVC

#ifdef _M_X64 
#define NATL_ARCHITECTURE_X86_64
#else 
static_assert(false, "natl: unknown architecture");
#endif

#endif // NATL_COMPILER_MSVC



//c
#include  <stdlib.h>

//own
#include "utility.h"

//interface 
namespace natl {
    enum class ProgramPlatformType : int {
        unknownPlatform,
        unixPlatform, 
        windowsPlatform,
        webPlatform
    };

    consteval ProgramPlatformType getPlatformType() noexcept {
#ifdef NATL_UNIX_PLATFORM
        return ProgramPlatformType::unixPlatform;
#elif defined(NATL_WINDOWS_PLATFORM)
        return ProgramPlatformType::windowsPlatform;
#elif defined(NATL_WEB_PLATFORM)
        return ProgramPlatformType::webPlatform;
#else
        return ProgramPlatformType::unknownPlatform;
#endif
    }

    consteval Bool natlInDebug() noexcept {
#ifdef NATL_IN_DEBUG
        return true;
#else
        return false;
#endif
    }

    NATL_FORCE_INLINE constexpr void constantEvaluatedError() noexcept {
        if (isConstantEvaluated()) {
            int* ptr = nullptr;
            *ptr = 0;
        }
    }


    NATL_FORCE_INLINE constexpr void natlDebugBreak() noexcept {
#ifdef NATL_IN_DEBUG
        if (isConstantEvaluated()) {
            constantEvaluatedError();
        } else {


#ifdef NATL_COMPILER_EMSCRIPTEN 
            emscripten_debugger();
#endif //  NATL_COMPILER_EMSCRIPTEN

#ifdef NATL_COMPILER_CLANG
            __builtin_debugtrap();
#endif //  NATL_COMPILER_CLANG

#ifdef NATL_COMPILER_GCC
            __builtin_trap();
#endif //  NATL_COMPILER_GCC

#ifdef NATL_COMPILER_MSVC
            __debugbreak();
#endif //  NATL_COMPILER_MSVC

        }
#endif
    }

    [[noreturn]] NATL_FORCE_INLINE constexpr void natlTerminate() noexcept {
        if (isConstantEvaluated()) {
            constantEvaluatedError();

#ifdef NATL_COMPILER_GCC
            abort();
#endif // NATL_COMPILER_GCC

        } else {
#ifdef NATL_IN_DEBUG
            natlDebugBreak();
#endif // NATL_IN_DEBUG
            abort();
        }
    }

	[[noreturn]] inline void unreachable() noexcept {
        if constexpr (natlInDebug()) {
            natlDebugBreak();
        }

#ifdef NATL_COMPILER_EMSCRIPTEN
        __builtin_unreachable();
#endif //  NATL_COMPILER_EMSCRIPTEN

#ifdef NATL_COMPILER_CLANG
        __builtin_unreachable();
#endif //  NATL_COMPILER_CLANG

#ifdef NATL_COMPILER_GCC
        __builtin_unreachable();
#endif // NATL_COMPILER_GCC

#ifdef NATL_COMPILER_MSVC
		__assume(false);
#endif // NATL_COMPILER_MSVC
	}

    constexpr const Ascii* platformTypeToString(const ProgramPlatformType platformType) noexcept {
        switch (platformType) {
        case ProgramPlatformType::unixPlatform:
            return "unix";
        case ProgramPlatformType::windowsPlatform:
            return "windows";
        case ProgramPlatformType::webPlatform:
            return "web";
        case ProgramPlatformType::unknownPlatform:
            return "unknown";
        default:
            unreachable();
        }
    }

    consteval const Ascii* getPlatformName() noexcept {
        return platformTypeToString(getPlatformType());
    }
}


#define NATL_TEXT(a) #a