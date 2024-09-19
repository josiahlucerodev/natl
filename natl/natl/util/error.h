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
#include "../container/stringView.h"
#include "compilerDependent.h"
#include "../system/print.h"

//interface
namespace natl {
    constexpr void fatalError(const ConstStringView message) noexcept {
        if (isConstantEvaluated()) {
            natlTerminate();
        } else {
            natl::println(message.c_str());
            natlTerminate();
        }
    }
    constexpr void fatalError(const Bool conditional, const ConstStringView message) noexcept {
        if (conditional) [[unlikely]] {
            fatalError(message);
        }
    }
}