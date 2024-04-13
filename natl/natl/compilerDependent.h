#pragma once

#if __EMSCRIPTEN__
#define NATL_COMPILER_EMSCRIPTEN
#define NATL_COMPILER_DEFINED
#endif // __EMSCRIPTEN__


#ifndef NATL_COMPILER_DEFINED
#ifdef __GNUC__
#define NATL_COMPILER_GCC
#define NATL_COMPILER_DEFINED
#endif // __GNUC__
#endif // !NATL_COMPILER_DEFINED

#ifndef NATL_COMPILER_DEFINED
#ifdef __clang__
#define NATL_COMPILER_CLANG
#define NATL_COMPILER_DEFINED
#endif //__clang__
#endif // !NATL_COMPILER_DEFINED

#ifndef NATL_COMPILER_DEFINED
#ifdef _MSC_VER
#define NATL_COMPILER_MSVC
#define NATL_COMPILER_DEFINED
#endif // _MSC_VER
#endif // !NATL_COMPILER_DEFINED

#undef NATL_COMPILER_DEFINED


//NATL_FORCE_INLINE
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

//NATL_FORCE_INLINE
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


#ifdef __unix__
#define NATL_UNIX_PLATFORM
#elif defined(_WIN32) || defined(WIN32)
#define NATL_WINDOWS_PLATFORM
#elif defined(NATL_COMPILER_EMSCRIPTEN)
define NATL_WEB_PLATFORM
#else 
static_assert(false, "natl: platform type not supported");
#endif

//own
#include "utility.h"

//interface 
namespace natl {
    enum class ProgramPlatformType : int {
        unknownPlatform,
        unixPlatform, 
        windowsPlatform,
    };

    consteval ProgramPlatformType getPlatformType() noexcept {
#ifdef NATL_UNIX_PLATFORM
        return ProgramPlatformType::unixPlatform;
#elif defined(NATL_WINDOWS_PLATFORM)
        return ProgramPlatformType::windowsPlatform;
#else
        return ProgramPlatformType::unknownPlatform;
#endif
    }

    consteval bool natlInDebug() noexcept {
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
#define NATL_UNREACHABLE_DEFINE
#endif // NATL_COMPILER_GCC


#ifdef NATL_COMPILER_MSVC
		__assume(false);
#define NATL_UNREACHABLE_DEFINE
#endif // NATL_COMPILER_MSVC
	}
}


#define NATL_TEXT(a) #a