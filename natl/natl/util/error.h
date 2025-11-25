#pragma once

//@begin_non_modules
//own
#include "compilerDependent.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef __EMSCRIPTEN__ 
#include <emscripten.h>
#endif
//@end_non_modules

//own
#include "../container/stringView.h"
#include "../system/print.h"

//@export
namespace natl {
    constexpr void fatalError(const ConstStringView message) noexcept {
        if (isConstantEvaluated()) {
            natl::terminate();
        } else {
            natl::println(message.c_str());
            natl::terminate();
        }
    }
    constexpr void fatalError(const Bool conditional, const ConstStringView message) noexcept {
        if (conditional) [[unlikely]] {
            fatalError(message);
        }
    }
}