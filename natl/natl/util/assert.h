#pragma once

//own
#include "../processing/format.h"
#include "../system/printFormatted.h"
#include "utility.h"

//@export
namespace natl {
    template<typename... ArgTypes>
    constexpr void cassertPre(const natl::Bool condition, ArgTypes&&... args) noexcept {
        if (!condition) {
            if (natl::isConstantEvaluated()) { 
                natl::constantEvaluatedError(); 
            } 
            natl::printlnf("natl: ", " Assertion failed: ", natl::forward<ArgTypes>(args)...);
            natl::terminate();
        }
    }
    template<typename... ArgTypes>
    constexpr void cassert(const natl::Bool condition, ArgTypes&&... args) noexcept {
        if (!condition) {
            if (natl::isConstantEvaluated()) {
                natl::constantEvaluatedError();
            } else {
                natl::printlnf("natl: ", " Assertion failed: ", natl::forward<ArgTypes>(args)...);
                natl::terminate();
            }
        }
    }

    [[noreturn]] constexpr void cassert() noexcept {
        if (natl::isConstantEvaluated()) {
            natl::constantEvaluatedError();
        } else {
            natl::printlnf("natl: cassert");
            natl::terminate();
        }
    }
}
