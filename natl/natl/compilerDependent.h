#pragma once

//NATL_FORCE_INLINE
#ifdef _MSC_VER
#define NATL_FORCE_INLINE __forceinline
#endif

#ifdef __GNUC__
#define NATL_FORCE_INLINE [[gnu::always_inline]]
#endif

#ifdef __clang__
#define NATL_FORCE_INLINE [[clang::always_inline]] 
#endif

#ifdef __EMSCRIPTEN__ 
#define NATL_FORCE_INLINE __attribute__((always_inline)) 
#endif

//NATL_IN_DEBUG
#ifdef _DEBUG
#define NATL_IN_DEBUG
#endif

#ifdef NDEBUG
#define NATL_IN_DEBUG
#endif

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
        if (std::is_constant_evaluated()) {
            int* ptr = nullptr;
            *ptr = 0;
        }
    }

    NATL_FORCE_INLINE constexpr void natlDebugBreak() noexcept {
#ifdef NATL_IN_DEBUG
        if (std::is_constant_evaluated()) {
            constantEvaluatedError();
        } else {
#ifdef _MSC_VER
            __debugbreak();
#endif

#ifdef __GNUC__
            __builtin_trap();
#endif

#ifdef __clang__
            __builtin_debugtrap();
#endif

#ifdef __EMSCRIPTEN__ 
            EM_ASM({ debugger; });
#endif
        }
#endif
    }

	[[noreturn]] inline void unreachable() noexcept {
        if constexpr (natlInDebug()) {
            natlDebugBreak();
        }
#ifdef _MSC_VER
		__assume(false);
#endif

#ifdef __GNUC__
		__builtin_unreachable();
#endif

#ifdef __clang__
		__builtin_unreachable();
#endif
	}
}