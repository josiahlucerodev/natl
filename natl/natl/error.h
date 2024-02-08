#pragma once

//compiler dependent 
#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef __EMSCRIPTEN__ 
#include <emscripten.h>
#endif

//std
#include <iostream>

//own
#include "stringView.h"
#include "compilerDependent.h"

//interface 
namespace natl {
    consteval bool natlInDebug() noexcept {
#ifdef NATL_IN_DEBUG
        return true;
#else
        return false;
#endif
    }

    NATL_FORCE_INLINE constexpr void constant_evaluated_error() noexcept { 
        if (std::is_constant_evaluated()) {
            int* ptr = nullptr;
            *ptr = 0;
        }
    }

    NATL_FORCE_INLINE constexpr void natl_debug_break() noexcept {
#ifdef NATL_IN_DEBUG
        if (std::is_constant_evaluated()) {
            constant_evaluated_error();
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

}


//interface
namespace natl {
    [[noreturn]] NATL_FORCE_INLINE constexpr void natl_terminate() noexcept {
        if (std::is_constant_evaluated()) {
            constant_evaluated_error();
        } else {
#ifdef NATL_IN_DEBUG
            natl_debug_break();
#endif // NATL_IN_DEBUG
            std::abort();
        }
    }
    constexpr void fatalError(const StringView message) noexcept {
        if (std::is_constant_evaluated()) {
            natl_terminate();
        } else {
            std::cout << message.c_str();
            natl_terminate();
        }
    }
    constexpr void fatalError(const bool conditional, const StringView message) noexcept {
        if (conditional) [[unlikely]] {
            fatalError(message);
        }
    }
}