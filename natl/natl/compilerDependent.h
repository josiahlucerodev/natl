#pragma once

//NATL_FORCE_INLINE
#ifdef __EMSCRIPTEN__ 
#define NATL_FORCE_INLINE __attribute__((always_inline)) 
#endif //  __EMSCRIPTEN__


#ifndef NATL_FORCE_INLINE
#ifdef __clang__
#define NATL_FORCE_INLINE [[clang::always_inline]] 
#endif //__clang__
#endif // !NATL_FORCE_INLINE

#ifndef NATL_FORCE_INLINE
#ifdef __GNUC__
#define NATL_FORCE_INLINE [[gnu::always_inline]]
#endif //__GNUC__
#endif // !NATL_FORCE_INLINE

#ifndef NATL_FORCE_INLINE
#ifdef _MSC_VER
#define NATL_FORCE_INLINE __forceinline
#endif // _MSC_VER
#endif // !NATL_FORCE_INLINE


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


//own
#include "utility.h"

//interface 
namespace natl {
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


#ifdef __EMSCRIPTEN__ 
            emscripten_debugger();
#define NATL_DEBUG_BREAK_DEFINE
#endif //  __EMSCRIPTEN__

#ifndef NATL_DEBUG_BREAK_DEFINE
#ifdef __clang__
            __builtin_debugtrap();
#define NATL_DEBUG_BREAK_DEFINE
#endif //  __clang__
#endif // !NATL_DEBUG_BREAK_DEFINE

#ifndef NATL_DEBUG_BREAK_DEFINE
#ifdef __GNUC__
            __builtin_trap();
#define NATL_DEBUG_BREAK_DEFINE
#endif //  __GNUC__
#endif // !NATL_DEBUG_BREAK_DEFINE

#ifndef NATL_DEBUG_BREAK_DEFINE
#ifdef _MSC_VER
            __debugbreak();
#define NATL_DEBUG_BREAK_DEFINE
#endif //  _MSC_VER
#endif // !NATL_DEBUG_BREAK_DEFINE

#undef NATL_DEBUG_BREAK_DEFINE

        }
#endif
    }

	[[noreturn]] inline void unreachable() noexcept {
        if constexpr (natlInDebug()) {
            natlDebugBreak();
        }


#ifdef __EMSCRIPTEN__
        __builtin_unreachable();
#define NATL_UNREACHABLE_DEFINE
#endif //  __EMSCRIPTEN__

#ifndef NATL_UNREACHABLE_DEFINE
#ifdef __clang__
        __builtin_unreachable();
#define NATL_UNREACHABLE_DEFINE
#endif //  __clang__
#endif //  !NATL_UNREACHABLE_DEFINE

#ifndef NATL_UNREACHABLE_DEFINE
#ifdef __GNUC__
        __builtin_unreachable();
#define NATL_UNREACHABLE_DEFINE
#endif // __GNUC__
#endif //  !NATL_UNREACHABLE_DEFINE


#ifndef NATL_UNREACHABLE_DEFINE
#ifdef _MSC_VER
		__assume(false);
#define NATL_UNREACHABLE_DEFINE
#endif // _MSC_VER
#endif //  !NATL_UNREACHABLE_DEFINE

#undef NATL_UNREACHABLE_DEFINE
	}
}


#define NATL_TEXT(a) #a