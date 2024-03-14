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
    [[noreturn]] NATL_FORCE_INLINE constexpr void natlTerminate() noexcept {
        if (isConstantEvaluated()) {
            constantEvaluatedError();
        } else {
#ifdef NATL_IN_DEBUG
            natlDebugBreak();
#endif // NATL_IN_DEBUG
            std::abort();
        }
    }
    constexpr void fatalError(const StringView message) noexcept {
        if (isConstantEvaluated()) {
            natlTerminate();
        } else {
            std::cout << message.c_str();
            natlTerminate();
        }
    }
    constexpr void fatalError(const bool conditional, const StringView message) noexcept {
        if (conditional) [[unlikely]] {
            fatalError(message);
        }
    }
}